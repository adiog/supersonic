// Copyright 2010 Google Inc.  All Rights Reserved
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "supersonic/expression/base/expression.h"

#include <algorithm>
#include <memory>
#include "supersonic/utils/std_namespace.h"
using std::make_unique;

#include "supersonic/utils/exception/failureor.h"
#include "supersonic/base/exception/exception.h"
#include "supersonic/base/exception/exception_macros.h"
#include "supersonic/proto/supersonic.pb.h"
#include "supersonic/utils/strings/join.h"

namespace supersonic {
class BufferAllocator;

// ----------------- BoundExpression -------------------------------------------

set<string> BoundExpression::referred_attribute_names() const {
  set<string> attributes_names;
  CollectReferredAttributeNames(&attributes_names);
  return attributes_names;
}

// ----------------- BoundExpressionTree ---------------------------------------

FailureOrVoid BoundExpressionTree::Init(BufferAllocator* allocator,
                                        rowcount_t max_row_count) {
  PROPAGATE_ON_FAILURE(skip_vector_storage_.TryReallocate(max_row_count));
  return Success();
}

FailureOrOwned<BoundExpressionTree>
    CreateBoundExpressionTree(BoundExpression* expression,
                              BufferAllocator* allocator,
                              rowcount_t max_row_count) {
  auto expression_tree = make_unique<BoundExpressionTree>(expression, allocator);
  PROPAGATE_ON_FAILURE(expression_tree->Init(allocator, max_row_count));
  return Success(expression_tree.release());
}

EvaluationResult BoundExpressionTree::Evaluate(const View& input) {
  if (row_capacity() < input.row_count()) {
    THROW(new Exception(
        ERROR_TOO_MANY_ROWS,
        StrCat("Trying to evaluate expression: ",
               result_schema().GetHumanReadableSpecification(),
               " with number of rows: ", input.row_count(),
               ", while the expression has capacity for less rows: ",
               row_capacity())));
  }
  // Fill the skip_vector with falses - evaluate everything.
  for (int i = 0; i < skip_vector_storage_.column_count(); ++i) {
    bit_pointer::FillWithFalse(skip_vector_storage_.view().column(i),
                               input.row_count());
  }
  EvaluationResult result =
      root_->DoEvaluate(input, skip_vector_storage_.view());
  PROPAGATE_ON_FAILURE(result);
  return result;
}

rowcount_t BoundExpressionTree::row_capacity() const {
  return std::min(skip_vector_storage_.row_capacity(), root_->row_capacity());
}

// ------------------ Expression -----------------------------------------------

FailureOrOwned<BoundExpressionTree> Expression::Bind(
    const TupleSchema& input_schema,
    BufferAllocator* allocator,
    rowcount_t max_row_count) const {
  FailureOrOwned<BoundExpression> bound_root = DoBind(input_schema, allocator,
                                                      max_row_count);
  PROPAGATE_ON_FAILURE(bound_root);
  return CreateBoundExpressionTree(bound_root.release(),
                                   allocator,
                                   max_row_count);
}

// -------------------- Lists --------------------------------------------------

FailureOrOwned<BoundExpressionList> ExpressionList::DoBind(
    const TupleSchema& input_schema,
    BufferAllocator* allocator,
    rowcount_t max_row_count) const {
  auto bound_list = make_unique<BoundExpressionList>();

  for (auto& expression : expressions_) {
    FailureOrOwned<BoundExpression> result =
        expression->DoBind(input_schema, allocator, max_row_count);
    PROPAGATE_ON_FAILURE(result);
    bound_list->add(result.release());
  }
  return Success(bound_list.release());
}

const string BoundExpressionList::ToString(bool verbose) const {
  string result_description;
  for (const auto &expression : expressions_) {
    if (&expression != &expressions_.front()) result_description.append(", ");
    // The next for lines could be replaced by GetMultiExpressionName, but I
    // want to avoid a dependency on expression_utils.
    const TupleSchema& subexpression_schema = expression.get()->result_schema();
    for (const auto& attribute : subexpression_schema.attributes()) {
      result_description.append(attribute.name());
    }
  }
  return result_description;
}

void BoundExpressionList::CollectReferredAttributeNames(
    set<string>* referred_attribute_names) const {
  for (const auto& expression : expressions_) {
    expression->CollectReferredAttributeNames(referred_attribute_names);
  }
}

const string ExpressionList::ToString(bool verbose) const {
  string result_description;
  for (const auto& expression : expressions_) {
    if (&expression != &expressions_.front()) result_description.append(", ");
    result_description.append(expression->ToString(verbose));
  }
  return result_description;
}

}  // namespace supersonic
