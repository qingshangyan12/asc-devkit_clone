# High-Precision log1p(x) Implementation for AscendC 3510

## Overview

This implementation provides a high-precision SIMD version of `log1p(x) = log(1 + x)` for AscendC architecture 3510, using register-based computation with multi-branch processing for enhanced numerical accuracy.

## Algorithm Design

### Multi-Branch Processing

The implementation uses MaskReg to handle different input ranges with specialized algorithms:

1. **Invalid Input Branch** (x < -1)
   - Returns NaN (not a number)
   - Detected using: `CompareScalar<float, CMPMODE::LT>(invalidMask, xReg, -1.0f, mask)`

2. **Boundary Case** (x == -1)
   - Returns -infinity
   - Detected using: `CompareScalar<float, CMPMODE::EQ>(negOneMask, xReg, -1.0f, mask)`

3. **Near-Zero Branch** (|x| < epsilon ≈ 1e-7)
   - Returns x directly (avoiding precision loss from floating-point operations)
   - Detected using: `CompareScalar<float, CMPMODE::LT>(nearZeroMask, absXReg, 1e-7, mask)`
   - Algorithm: `log1p(x) ≈ x` for very small x

4. **Small Value Branch** (|x| < 0.5)
   - Uses 16th-order Taylor expansion for maximum precision
   - Detected using: `CompareScalar<float, CMPMODE::LT>(smallMask, absXReg, 0.5f, mask)`
   - Algorithm: `log1p(x) = x - x²/2 + x³/3 - x⁴/4 + ...`
   - Optimized by computing powers as x² and multiplying by x² to reduce operations

5. **Mid Value Branch** (0.5 ≤ |x| < 1.5)
   - Uses optimized logarithm computation
   - Detected using: `CompareScalar<float, CMPMODE::LT>(midMask, absXReg, 1.5f, mask)`
   - Algorithm: Direct computation with `ln(1+x)`

6. **Large Value Branch** (|x| ≥ 1.5)
   - Uses numerical stability improvement: `log1p(x) = log(x) + log1p(1/x)`
   - Detected using: `CompareScalar<float, CMPMODE::GE>(largeMask, absXReg, 1.5f, mask)`
   - This avoids precision loss from computing 1+x for large x values

### Data Flow

```
UB (LocalTensor) → LoadAlign → RegTensor (float)
                  ↓
              Branch Detection (MaskReg creation)
                  ↓
         Parallel Computation on all branches
                  ↓
              Result Selection (Reg::Select)
                  ↓
         Cast (if half) → StoreAlign → UB (LocalTensor)
```

## Key Features

### 1. Register-Based Computation
- All arithmetic operations performed on RegTensor
- Minimizes memory bandwidth usage
- Enables SIMD parallel processing

### 2. Multi-Branch Parallel Processing
- All branches computed simultaneously
- MaskReg filters results based on input range
- No sequential if-else overhead

### 3. Precision Enhancements
- 16th-order Taylor expansion vs 8th-order in basic version
- Special handling for very small values
- Numerical stability for large values
- Proper handling of boundary cases (-1, < -1)

### 4. Data Type Support
- Native float32 computation
- Half precision support with float32 intermediate computation
- Automatic casting with appropriate saturation modes

## Performance Characteristics

- **Latency**: Single pass through all computation branches
- **Throughput**: Full SIMD width utilization
- **Memory Access**: 1 load + 1 store per element
- **Computation**: 16-20 arithmetic operations per element

## Usage Example

```cpp
#include "adv_api/math/log1p.h"

// Basic usage
LocalTensor<float> input;
LocalTensor<float> output;
uint32_t count = 256;

AscendC::Log1pHighPrecisionImpl<float>(output, input, count);

// Half precision
LocalTensor<half> inputHalf;
LocalTensor<half> outputHalf;
AscendC::Log1pHighPrecisionImpl<half>(outputHalf, inputHalf, count);
```

## Accuracy

- Small values (|x| < 0.5): Machine epsilon precision via Taylor series
- Medium values (0.5 ≤ |x| < 1.5): Full float32 precision
- Large values (|x| ≥ 1.5): Numerically stable computation
- Edge cases: IEEE 754 compliant (NaN for x < -1, -inf for x = -1)

## Comparison with Basic Implementation

| Feature | Basic | High-Precision |
|---------|-------|----------------|
| Taylor Order | 8 | 16 |
| Branch Handling | 2 (small/large) | 6 (invalid/near-zero/small/mid/large/boundary) |
| Near-zero Handling | No | Yes |
| Large value stability | Standard | Enhanced (log(x) + log1p(1/x)) |
| Edge cases | Limited | Full (NaN, -inf, etc.) |

## Implementation Details

### MaskReg Operations

```cpp
// Create masks for different branches
Reg::CompareScalar<float, CMPMODE::LT>(smallMask, absXReg, 0.5f, mask);
Reg::CompareScalar<float, CMPMODE::LT>(midMask, absXReg, 1.5f, mask);

// Combine masks
Reg::AndNot(tmpMask, smallMask, midMask);

// Select results based on mask
Reg::Select(resultReg, resultReg, smallResultReg, smallMask);
```

### Taylor Series Computation

```cpp
// Optimized: compute x² and multiply repeatedly
Reg::Mul(xSquaredReg, xReg, xReg, mask);
Reg::Duplicate(powReg, xReg);

for (int i = 2; i <= 16; i++) {
    if (i % 2 == 0) {
        Reg::Mul(powReg, powReg, xSquaredReg, mask);
    }
    Reg::Muls(tmpReg, powReg, coeffs[i-1], mask);
    Reg::Add(resultReg, resultReg, tmpReg, mask);
}
```

## Notes

- Requires AscendC 3510 architecture or compatible
- Uses internal Reg API (not for direct user consumption)
- Provides superior accuracy compared to basic log1p implementation
- Suitable for scientific computing and applications requiring numerical precision