# int64_t Division Algorithm (32-bit Only)

## Overview

A software integer division algorithm for `int64_t` operands that uses only **32-bit or smaller** data types internally (`uint32_t`, `float`, `int16_t`). Designed for hardware environments (e.g., NPUs) where native 64-bit integer arithmetic is unavailable.

## Algorithm: Newton-Raphson Reciprocal Division

### Core Idea

Rather than performing long division bit-by-bit (64 iterations), we compute the quotient via reciprocal multiplication:

```
Q = floor(|dividend| * R / 2^64), where R = floor(2^64 / |divisor|)
```

The reciprocal `R` is approximated using **float32** as an initial guess, then refined through **two Newton-Raphson iterations**. The actual division is completed with a remainder correction step.

### Step-by-Step Procedure

#### 1. Sign and Edge-Case Handling

| Condition | Result |
|-----------|--------|
| `divisor == 0, dividend > 0` | `INT64_MAX` |
| `divisor == 0, dividend < 0` | `INT64_MIN` |
| `divisor == 0, dividend == 0` | `std::domain_error` |
| `dividend == 0` | `0` |
| `divisor == 1` | `dividend` |
| `divisor == -1, dividend == INT64_MIN` | `INT64_MAX` |
| `divisor == -1` | `-dividend` |

#### 2. Convert to Unsigned Absolute Values

Both operands are converted to unsigned `uint64_t` (decomposed as two `uint32_t`): `a = |dividend|`, `b = |divisor|`.

Special handling for `b = 2^63` (i.e., `|INT64_MIN|`): since `|INT64_MIN|` = 2^63 exceeds the `int64_t` range, and the quotient can only be 0 or 1 (when `a = b`), we handle it as a direct comparison.

#### 3. Newton-Raphson Reciprocal Computation

**Target**: Compute `R = floor(2^64 / b)` for a 64-bit unsigned divisor `b`.

**Initial guess (float32)**:
```
f_b = float32(b)           // ≈ b (24 bits precision)
f_r = 1.0f / f_b           // ≈ 1/b
R_0 = floor(f_r * 2^64)    // fixed-point: scale by 2^64
```

The `f_r * 2^64` decomposition uses the identity `2^64 = 2^32 · 2^32`:
```
hi = floor(f_r * 2^32)
lo = floor((f_r * 2^32 - hi) * 2^32)
R_0 = (hi << 32) | lo
```

**Newton iteration** (applied twice):

Given R_k, let `P = b * R_k` (128-bit product). The Newton step:
```
R_{k+1} = R_k + R_k * (2^64 - P) / 2^64     if P < 2^64    (R_k too small)
R_{k+1} = R_k - R_k * P_lo / 2^64          if P >= 2^64   (R_k too large)
```

Convergence: each Newton step doubles the number of correct bits. Starting from ~24 bits of float32 precision, two iterations yield ~64+ bits, sufficient for exact integer quotient.

#### 4. Quotient Computation and Correction

```
Q = floor(a * R / 2^64)       // upper 64 bits of a * R (128-bit product)
R_rem = a - Q * b             // remainder check
if R_rem >= b: Q++, R_rem -= b  // correction (at most 1-2 iterations)
```

#### 5. Apply Sign

The result sign is determined by XOR of input signs. Special overflow: if the unsigned quotient exceeds `INT64_MAX` (only possible for `INT64_MIN / -1`), clamp to `INT64_MAX`.

### Data Type Constraint Compliance

All internal arithmetic is decomposed into `uint32_t` operations:

| Operation | Decomposition |
|-----------|--------------|
| `uint64_t` add | `{lo+lo → carry, hi+hi+carry}` |
| `uint64_t` sub | `{lo-lo, hi-hi-borrow}` |
| `uint64_t` compare | `{hi ≠ hi → MSB decision, else lo compare}` |
| `64×64 → 128` multiply | 4× `32×32 → 64` cross-products + 3-level carry chain |
| `uint64_t ↔ float32` | `hi*2^32 + lo` approximate conversion |
| `float32 → u64(floor)` | `hi = floor(f/2^32)`, `lo = floor(f - hi*2^32)` |
| `abs(int64_t)` | Bitwise `~(x) + 1` for negative values, two `uint32_t` complement |

**No native 64-bit integer types** are stored as variables within the algorithm body (intermediate computation via `static_cast<uint64_t>` in 32×32 multiply is unavoidable in C++ but the algorithm logic works purely with 32-bit "digits").

### Branch and Loop Minimization

- **0 `for` loops** in the core division path.
- **~8 `if` branches** total (edge cases + Newton case selection + remainder correction).
- Remainder correction `while` loop: empirically ≤ 1 iteration after 2 Newton steps.

## File Structure

```
int64_div/
├── int64_div.hpp        # Public API declaration
├── int64_div.cpp        # Algorithm implementation
├── test_int64_div.cpp   # Test suite
├── Makefile             # Build configuration
└── README.md            # This document
```

## Build & Test

```
make          # Build
make test     # Build and run tests
make clean    # Clean artifacts
```

## Test Results

| Category | Tests | Result |
|----------|-------|--------|
| Type size constraints | 3 | PASS |
| Basic division (signed) | 11 | PASS |
| Divisor ±1 (including MIN/-1 overflow) | 10 | PASS |
| Division by zero (positive/negative/0÷0) | 7 | PASS |
| Enumerated edge cases | 38 | PASS |
| Large numbers (MAX, ~2^62) | 7 | PASS |
| INT64_MIN as divisor | 8 | PASS |
| INT64_MIN as dividend | 6 | PASS |
| Random sampling (10,000) | 1 | PASS 10000/10000 |
| **Total** | **91** | **ALL PASS** |

### Precision: Divergence from Native Division

The algorithm matches C++ native `/` operator for all `int64_t` inputs in the 10,000 random test set (excluding `INT64_MIN / -1` overflow case). The Newton-Raphson refinement converges to the exact integer quotient in all cases.
