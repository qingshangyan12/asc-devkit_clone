# ASC_IS_AIC/ASC_IS_AIV

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:06:31.589Z pushedAt=2026-10-28T12:47:03.113Z nl=2 -->

## Description

The header file path is `"c_api/defs/macro.h"`.

`ASC_IS_AIC` and `ASC_IS_AIV` are used to conditionally compile and run the code paths of the Cube Core (AIC) and Vector Core (AIV) in [separation mode](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md), and can be used as the condition of an `if` statement. When compiling or running the AIC code path, the `ASC_IS_AIC` condition is true; when compiling or running the AIV code path, the `ASC_IS_AIV` condition is true.

These two macros are the C API aliases of `ASCEND_IS_AIC` and `ASCEND_IS_AIV`, respectively, and the corresponding macros have the same functionality. When developing a `__mix__` fusion operator based on the AIC and AIV separation mode, you can use these two macros to isolate the code executed only by the AIC, such as matrix computation and matrix data movement, as well as the code executed only by the AIV, such as vector computation and vector data movement.

## Macro Definition

```cpp
#define ASC_IS_AIC ASCEND_IS_AIC
#define ASC_IS_AIV ASCEND_IS_AIV
```

## Value Description

| Current Code Path | `ASC_IS_AIC` Condition Result | `ASC_IS_AIV` Condition Result |
| --- | --- | --- |
| AIC | `true` | `false` |
| AIV | `false` | `true` |

## Constraints

- These two macros apply only to [separation mode](../../../../../guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md).
- In fusion operators in AIC and AIV separation mode, AIC and AIV execute different types of instructions. Code that can be executed only on AIC must be placed in the `ASC_IS_AIC` conditional branch; code that can be executed only on AIV must be placed in the `ASC_IS_AIV` conditional branch.
