# AGENTS.md

This file provides guidelines for agentic coding agents working in the asc-devkit repository.

## Build Commands

### Standard Build
```bash
bash build.sh
```

### Build with Custom Options
```bash
bash build.sh -j 8                    # Use 8 threads
bash build.sh --build-type=Debug      # Debug build
bash build.sh --asan                  # Enable address sanitizer
```

### Build Package
```bash
bash build.sh --pkg -j 8
```

## Test Commands

### Run All Tests
```bash
bash build.sh -t                      # Build and run all unit tests
bash build.sh -t --cov                # Run tests with code coverage
```

### Run Specific Test Parts
```bash
bash build.sh --adv_test              # Run advanced API tests
bash build.sh --basic_test_one        # Run basic test part one
bash build.sh --basic_test_two        # Run basic test part two
bash build.sh --basic_test_three      # Run basic test part three
```

### Run Single Test
```bash
# Build specific test target
cd build && cmake --build . --target ascendc_ut_basic_api_ascend910 -j 8

# Run specific test executable
cd build && ./tests/api/basic_api/ascendc_ut_basic_api_ascend910
```

### Clean Build
```bash
bash build.sh --make_clean
```

## Code Style Guidelines

### File Headers
All source files must start with the Huawei copyright header:
```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
```

### Formatting
- Use clang-format with the provided .clang-format configuration
- Column limit: 120 characters
- Indentation: 4 spaces (no tabs)
- Pointer alignment: Left (`int* ptr` not `int *ptr`)
- Function braces on new line
- Sort includes: false (maintain manual ordering)

### C++ Standards
- C++17 standard required
- C++11 ABI disabled by default (USE_CXX11_ABI=0)

### Naming Conventions
- Classes: PascalCase (e.g., `ShapeImpl`, `Tensor`)
- Functions: PascalCase for public APIs (e.g., `GetDimNum()`)
- Variables: camelCase for local variables (e.g., `blockIdx`, `perBlockDataSize`)
- Constants: UPPER_SNAKE_CASE (e.g., `UNKNOWN_DIM_SIZE`)
- Namespaces: lowercase (e.g., `ge`, `AscendC`, `context_ascendc`)

### Include Ordering
1. System headers (`#include <inttypes.h>`)
2. Standard library headers (`#include <algorithm>`)
3. Project headers (`#include "kernel_operator.h"`)

### Namespaces
- Use `namespace Name {` without indentation for namespace contents
- Close with `}  // namespace Name`
- Common namespaces: `ge`, `AscendC`, `context_ascendc`

### Error Handling
- Use return codes for error handling in API functions
- Return false/nullptr for failure cases where appropriate
- Avoid exceptions in performance-critical code

### Testing Guidelines
- Use gtest framework
- Expose private/protected members with `#define private public` in test files
- Use `using namespace std;` and `using namespace AscendC;` in tests
- Test naming: `ascendc_ut_<module>_<product_type>`

### Project Structure
- `include/`: Public API headers (basic_api, adv_api, c_api, aicpu_api, utils)
- `impl/`: Implementation source files
- `tests/`: Unit tests organized by API type
- `cmake/`: Build configuration
- `examples/`: Sample code

### Common Patterns
- Use `ComGraphMakeShared<T>()` for creating shared pointers
- Use `__gm__` and `__aicore__` attributes for kernel functions
- Use `GlobalTensor<T>` and `LocalTensor<T>` for tensor operations
- Use `TPipe` and `TBuf` for buffer management

### Linting
Run clang-format before committing:
```bash
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```
