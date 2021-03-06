// Copyright 2011 Google Inc.  All Rights Reserved
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
// Author: onufry@google.com (Onufry Wojtaszczyk)
// Author: ptab@google.com (Piotr Tabor)
//
// Type definitions for standard bound expression factories (constructors)
// by arity.

#ifndef SUPERSONIC_EXPRESSION_INFRASTRUCTURE_BOUND_EXPRESSION_CREATORS_H_
#define SUPERSONIC_EXPRESSION_INFRASTRUCTURE_BOUND_EXPRESSION_CREATORS_H_

#include <cstddef>

#include "supersonic/base/exception/result.h"
#include "supersonic/base/infrastructure/types.h"

namespace supersonic {

class BoundExpression;
class BoundExpressionList;
class BufferAllocator;
class Expression;

// Shorthands for function pointers to expression creation functions.
using ConstExpressionCreator = const Expression *(*)();

using UnaryExpressionCreator = const Expression *(*)(const Expression *);

using BinaryExpressionCreator = const Expression *(*)(const Expression *, const Expression *);

using TernaryExpressionCreator = const Expression *(*)(const Expression *, const Expression *, const Expression *);

using QuaternaryExpressionCreator = const Expression *(*)(const Expression *, const Expression *, const Expression *, const Expression *);

using QuinaryExpressionCreator = const Expression *(*)(const Expression *, const Expression *, const Expression *, const Expression *, const Expression *);

using SenaryExpressionCreator = const Expression *(*)(const Expression *, const Expression *, const Expression *, const Expression *, const Expression *, const Expression *);


// TODO(ptab): Rename to *Creator (for example: BoundConstExpressionCreator).
using BoundConstExpressionFactory = FailureOrOwned<BoundExpression> (*)(BufferAllocator *, rowcount_t);

using BoundUnaryExpressionFactory = FailureOrOwned<BoundExpression> (*)(BoundExpression *, BufferAllocator *, rowcount_t);

using BoundBinaryExpressionFactory = FailureOrOwned<BoundExpression> (*)(BoundExpression *, BoundExpression *, BufferAllocator *, rowcount_t);

using BoundTernaryExpressionFactory = FailureOrOwned<BoundExpression> (*)(BoundExpression *, BoundExpression *, BoundExpression *, BufferAllocator *, rowcount_t);

using BoundQuaternaryExpressionFactory = FailureOrOwned<BoundExpression> (*)(BoundExpression *, BoundExpression *, BoundExpression *, BoundExpression *, BufferAllocator *, rowcount_t);

using BoundSenaryExpressionFactory = FailureOrOwned<BoundExpression> (*)(BoundExpression *, BoundExpression *, BoundExpression *, BoundExpression *, BoundExpression *, BoundExpression *, BufferAllocator *, rowcount_t);

using BoundExpressionListExpressionFactory = FailureOrOwned<BoundExpression> (*)(BoundExpressionList *, BufferAllocator *, rowcount_t);

}  // namespace supersonic
#endif  // SUPERSONIC_EXPRESSION_INFRASTRUCTURE_BOUND_EXPRESSION_CREATORS_H_
