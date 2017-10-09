// Copyright 2012, Google Inc.
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
// Author: tomasz.kaftal@gmail.com (Tomasz Kaftal)
//
// This file contains a custom googletest main method to launch tests.
// It launches the Supersonic initialisation routine.

#include <iostream>

#include "supersonic/base/infrastructure/init.h"
#include "gtest/gtest.h"
#include <absl/strings/string_view.h>

GTEST_API_ int main(int argc, char** argv) {
  std::cout << "Running main() from supersonic_test_main.cc\n";

  std::vector<char *> supersonicArguments;
  std::copy_if(argv,
               argv + argc,
               std::back_inserter(supersonicArguments),
               [](char * arg) {
                   return absl::string_view{arg}.substr(0, 7) != "--gtest";
               });
  auto supersonicArgc = static_cast<int>(supersonicArguments.size());
  char** supersonicArgv = supersonicArguments.data();

  supersonic::SupersonicInit(&supersonicArgc, &supersonicArgv);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
