# AGENTS.md - Ascend C Development Kit

## Project Overview

Ascend C is a C++ programming language for developing operators on Huawei Ascend AI processors. This is a **header-only** library with the following structure:

```
├── include/        # API declarations (.h files)
├── impl/           # API implementations (.h files, inline)
├── tests/          # Unit tests (gtest-based)
├── docs/           # Contribution guides
├── cmake/          # Build configuration
└── build.sh        # Main build script
```

## Build Commands

### Full Build
```bash
# Build project
bash build.sh -j <threads>

# Build and run all unit tests
bash build.sh -t

# Clean build artifacts
bash build.sh --make_clean
```

### Running Specific Tests
```bash
# Advanced API tests
bash build.sh --adv_test

# Basic API tests (split parts for parallel execution)
bash build.sh --basic_test_one
bash build.sh --basic_test_two
bash build.sh --basic_test_three
```

### Build Options
```bash
bash build.sh -p /path/to/cann    # Set CANN package path
bash build.sh --asan              # Enable Address Sanitizer
bash build.sh --cov              # Enable code coverage
```

## Code Style Guidelines

### Formatting (from .clang-format)
- **Language**: C++
- **Standard**: C++11
- **Column Limit**: 120 characters
- **Indent Width**: 4 spaces
- **Tab Width**: 4 spaces
- **Use Tabs**: Never
- **Pointer Alignment**: Left
- **Access Modifier Offset**: -4

Run `clang-format` on changed files before committing.

### File Organization

#### Header Files
- Location: `include/<api_category>/`
- Implementation: `impl/<api_category>/` (inline in headers)
- Naming: `snake_case.h` (e.g., `kernel_operator_vec_unary_intf.h`)

#### Header Guards
```cpp
#ifndef ASCENDC_MODULE_OPERATOR_VEC_UNARY_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_VEC_UNARY_INTERFACE_H
// ... content ...
#endif
```

### Naming Conventions
- **Classes/Functions**: PascalCase (e.g., `Relu`, `TPipe`, `LocalTensor`)
- **Templates**: PascalCase with type parameters (e.g., `template <typename T>`)
- **Types**: PascalCase (e.g., `LocalTensor<T>`, `GlobalTensor<T>`)
- **Namespaces**: `AscendC` (use `using namespace AscendC;` in test files)
- **Macros**: SCREAMING_SNAKE_CASE with `__` prefix for internal (e.g., `__NPU_ARCH__`)

### Copyright Header (Required)
Every source file must include:
```cpp
/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
```

### Documentation Style (Doxygen-like)
```cpp
/*!
 * \file kernel_operator_vec_unary_intf.h
 * \brief Brief description
 */
 
/*
 * @ingroup Relu Level 0
 * @brief dst[i] = (src[i] < 0) ? 0 : src[i]
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] count number Number of data involved in calculation
 */
```

### Code Patterns

#### Template Functions (Common Pattern)
```cpp
template <typename T, bool isSetMask = true>
__aicore__ inline void Relu(const LocalTensor<T>& dst, 
                             const LocalTensor<T>& src, 
                             const int32_t& count);
```

#### Architecture-Specific Code
```cpp
#if (__NPU_ARCH__ == 3101) || (__NPU_ARCH__ == 5102)
// Architecture-specific implementation
#endif
```

#### CPU Debug Support
```cpp
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif
```

### Error Handling
- Use `ASC_ASSERT` for runtime assertions
- Error codes returned from functions
- Use `set_flag`/`wait_flag` for pipeline synchronization in tests

### Import Conventions
```cpp
#include "kernel_macros.h"
#include "kernel_tensor.h"
#include "kernel_struct_unary.h"
// Platform-specific includes with guards
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3101 || __NPU_ARCH__ == 5102)
#include "micro_api/kernel_micro_utils.h"
#endif
```

## Testing Guidelines

### Test Structure
- Location: `tests/api/<api_category>/`
- Framework: Google Test (gtest)
- Test executables: `ascendc_ut_<api>_<product_type>`

### Test File Organization
```
tests/api/basic_api/
├── ascendc_case_ascend910/
├── ascendc_case_ascend610/
└── ascendc_case_ascend310b1/
```

### Writing Tests
```cpp
#include <gtest/gtest.h>
#include "kernel_operator.h"

using namespace AscendC;

TEST(OperatorTest, ReluBasic)
{
    // Test implementation using TPipe, GlobalTensor, LocalTensor
    TPipe tpipe;
    // ... setup and execution ...
}
```

### Product Types Supported
- `ascend910`
- `ascend610`
- `ascend310p`
- `ascend910B1_AIC`
- `ascend910B1_AIV`
- `ascend310B1`
- `ascend950pr_9599_AIC`
- `ascend950pr_9599_AIV_BASIC`
- `ascend950pr_9599_AIV_FRAMEWORK`

## Architecture Notes

### API Layers
1. **C API** - Pure C interfaces, pointer-based programming
2. **Basic API** - C++ class library, tensor-based,指令级封装
3. **Advanced API** - High-level algorithms, composed from Basic APIs

### Key Types
- `LocalTensor<T>` - On-chip memory tensor
- `GlobalTensor<T>` - Off-chip memory tensor
- `TBuf<TPosition>` - Temporary buffer
- `TPipe` - Pipeline management

### NPU Architecture Macros
- `__NPU_ARCH__` - Architecture identifier (3101, 5102, etc.)
- `__DAV_C100__`, `__DAV_M200__`, etc. - Die identification
- `__aicore__` - Marks functions running on AI Core

## Key Files
- `.clang-format` - Code formatting rules
- `CMakeLists.txt` - Root build configuration
- `build.sh` - Main build script (577 lines)
- `CONTRIBUTING.md` - Contribution guidelines
- `docs/asc_*_api_contributing.md` - API-specific development guides
