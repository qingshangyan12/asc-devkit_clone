# DivS64Impl 任意数除以 INT64_MIN 精度问题分析与修复

## 1. 背景

3510 (Ascend950/DAV_3510) 架构下，NPU 硬件不原生支持 int64_t 数据类型。int64_t 由两个 uint32_t 拼接构成，
存储在 `RegTensor<int64_t, RegTraitNumTwo>` 中：
- `.reg[0]` — 低 32 位 (int32_t / uint32_t)
- `.reg[1]` — 高 32 位 (int32_t / uint32_t)

int64_t 除法由 `DivS64Impl` 实现，位于:
`/usr/local/Ascend/cann/asc/impl/basic_api/reg_compute/dav_3510/kernel_reg_compute_vec_binary_impl.h:541`

## 2. DivS64Impl 算法原理

### 2.1 整体流程

```
srcReg0 (被除数) × srcReg1 (除数) → DivS64Impl → dstReg (商)

Step 1: AbsUsingS32 → 计算被除数和除数的绝对值 (vAbsSrc0, vAbsSrc1)
Step 2: 零除数检查 → 结果 = -1 (0xFFFFFFFFFFFFFFFF)
Step 3: |除数| == 1 检查 → 结果 = |被除数| (符号后处理)
Step 4: Newton-Raphson 倒数迭代 → 计算 |被除数| / |除数| 的商
Step 5: DivSignCal → 根据原操作数符号决定结果是否取反
Step 6: 按符号取反或保持结果
Step 7: 零除数 → 输出 -1
```

### 2.2 Newton-Raphson 倒数迭代详细分析

核心思想：用 float32 的倒数作为初始猜测，通过两次 Newton 迭代精细化，最终在整数域计算商。

```
(1)  vTmp2 = float32(|除数|)                              // VcvtS642F32
(2)  vTmp3 = 1.0f / vTmp2                                 // F32PreProcess: vdiv
(3)  vTmp4 = vTmp3 + 0x1FFFFFFEU (按 uint32 位运算)        // F32PreProcess: vadds
(4)  vTmp5 = int64_t(vTmp4)                                // VcvtF322S64, ROUND_Z
(5)  vTmp6 = |除数| × vTmp5                                // VmulUsingU32
(6)  vTmp6 = ~vTmp6 + 1 = -(|除数| × vTmp5)                // VnotInPlace + AddB64Impl
(7)  vTmp6 = vTmp5 × vTmp6 + 0                        // B128Calc (第一次 Newton 迭代结果)
(8)  vTmp7 = vTmp5 + vTmp6                             // VaddUsingU32

步骤 (5)-(8) 重复一次 (第二次 Newton 迭代):
(9)  vTmp6 = |除数| × vTmp7
(10) vTmp6 = -(vTmp6)                                      // ~x + 1
(11) vTmp7 = vTmp7 × vTmp6 + 0                          // B128Calc
(12) vTmp6 = vTmp7 + vTmp6                              // VaddUsingU32

得到倒数近似值 vTmp6 ≈ 2^N / |除数| (定点表示)

(13) vTmp7 = |被除数| × vTmp6 + 0                           // B128Calc: 商 ≈ |被除数| × 倒数
(14) vTmp7 = |被除数| - vTmp7 × |除数|                       // 余数
(15) 若余数 >= |除数|: 商 += 1                               // 商修正 (两次 VcmpGeUsingU32)
(16) 针对 |除数| == 1 的特殊选择                              // VselUsingU32
```

**F32PreProcess 中的魔数 `0x1FFFFFFE` 分析**：

此常量按 uint32 位模式与 float32 的 `1.0f / |除数|_f32` 的位表示做整数加法，
目的是调整初始倒数猜测，使其经过 int64 截断后保留足够的有效位供后续定点乘法运算
(类似于固定 Q 格式的缩放)。

例如 |除数| = 2^63:
- `1.0f / 2^63` 的 float32 位表示 = `0x20000000`
- `0x20000000 + 0x1FFFFFFE = 0x3FFFFFFE` (float32 约 1.99999976)
- 截断为 int64 → 1

### 2.3 关键辅助函数说明

| 函数 | 功能 |
|------|------|
| `AbsUsingS32` | 计算 int64 绝对值: 若高32位 < 0, 执行 `0 - x` (vsubc/vsubcs) |
| `VcvtS642F32` | int64_t → float32 转换 (vcvt, ROUND_R) |
| `VcvtF322S64` | float32 → int64_t 转换 (vcvt, ROUND_Z) |
| `F32PreProcess` | `1.0f / |除数|_f32` 后对 uint32 位表示加偏差 |
| `B128Calc` | 64×64 + 64 → 128 位乘加: vSrc0 × vSrc1 + vSrc2, 结果低64位存回 vSrc1 |
| `VmulUsingU32` | 64×64 → 128 位乘法 (Mull + vmula 累加), 取低64位 |
| `DivSignCal` | `signResP = ~(src0≥0 XOR src1≥0)`, 即同号=false (不取反) |

## 3. 根因分析

### 3.1 INT64_MIN 的绝对值无法用 int64_t 表示

```
INT64_MIN = 0x8000000000000000 = -9223372036854775808  (即 -2^63)
|INT64_MIN| = 9223372036854775808 = 2^63
INT64_MAX = 0x7FFFFFFFFFFFFFFF = 9223372036854775807 = 2^63 - 1
```

**|INT64_MIN| = 2^63 超出了 int64_t 的正数表示范围 [-2^63, 2^63-1]**

### 3.2 AbsUsingS32 对 INT64_MIN 的计算错误

`AbsUsingS32` 的逻辑是：若值为负 (高32位 < 0)，则计算 `0 - x` 作为绝对值。

对于 `x = INT64_MIN = 0x8000000000000000`:

```
低32位 = 0x00000000, 高32位 = 0x80000000 (int32: 负数)

vcmp_lt(carry0, 高32位, 0) → carry0 = true

vsubc(carry1, low_dst, 0, 0x00000000, carry0):
  low_dst  = 0 - 0x00000000 - 1(借位) = 0xFFFFFFFF
  borrow1  = true (0 < 0 + 1)

vsubcs(carry2, high_dst, 0, 0x80000000, carry1 + carry0):
  high_dst = 0 - 0x80000000 - 1 = 0x7FFFFFFF
  borrow2  = true

vAbs = {0xFFFFFFFF, 0x7FFFFFFF} = 0x7FFFFFFFFFFFFFFF = 2^63 - 1
```

**实际输出 "绝对值": `2^63 - 1`, 偏差为 1。**

在 int64 补码体系中, `-(INT64_MIN) = INT64_MIN` (自身取反结果不变),
而 `0 - INT64_MIN` 通过多字减法的计算结果为 `2^63 - 1`。

### 3.3 除数 = INT64_MIN 时的错误传播

当 srcReg1 = INT64_MIN 时，算法使用错误的 vAbsSrc1 = 2^63 - 1 进入 Newton-Raphson 迭代:

```
vTmp2 = float32(2^63 - 1) ≈ 2^63 (float32 仅 24 位有效, 低 39 位截断)

vTmp3 = 1.0f / 2^63 = 2^(-63) → 位表示 0x20000000
vTmp4 = 0x20000000 + 0x1FFFFFFE = 0x3FFFFFFE ≈ 1.99999976 (float32 值)

vTmp5 = int64_t(1.99999976) = 1 (ROUND_Z 截断)
```

此时初始猜测 x0 = 1。真正的倒数约为 `1/2^63 ≈ 10^-19`。x0 = 1 是数量级完全错误的初始值。

后续 Newton 迭代在错误初始值上完全发散:
- `d × x0 = (2^63-1) × 1 = 2^63-1` (按 uint64 运算)
- `~(d × x0) + 1 = -(2^63-1)` (按补码理解)
- `x0 × (-(2^63-1))` 产生巨大的负数结果
- 迭代不收敛, 商完全错误

### 3.4 被除数 = INT64_MIN 的次要影响 (不在本次修复范围内)

当被除数 = INT64_MIN 而除数 ≠ INT64_MIN 时:
- |被除数|_计算 = 2^63 - 1, 而非 2^63
- 相对误差 ≈ 1/2^63 ≈ 10^-19
- 对于大多数除数, 此误差经过 float32 运算 (仅 7 位有效数字) 后不会显著改变最终整数商
- **但在被除数是除数的整数倍附近时可能产生 ±1 的商偏差**

## 4. 修复方案

### 4.1 策略

在 `DivS64Impl` 中增加对除数为 INT64_MIN 的特殊处理分支: 在进入 Newton-Raphson 迭代前
从 mask 中排除 INT64_MIN 除数元素, 迭代结束后直接赋值结果。

### 4.2 原理

除数为 INT64_MIN 时，商仅可能有两种值:
- **0**: 对于所有被除数 x ≠ INT64_MIN (因为 |x| < 2^63 = |INT64_MIN|)
- **1**: 当被除数 = INT64_MIN (自身除自身)

符号由 `DivSignCal` 正常处理, 无需额外操作。

### 4.3 修改位置

#### (a) 变量声明 (`DivS64Impl` 行 547-548)

增加 `vInt64MinConst` (`RegTensor<uint64_t>`) 和 `isInt64MinDivMask`, `dividendIsInt64MinMask`, `nonInt64MinMask` (`MaskReg`)。

#### (b) INT64_MIN 检测和 mask 收窄 (插入于 oneMask 处理之后)

```cpp
VbrUsingU32(vInt64MinConst, 0, 0x80000000);             // INT64_MIN 常量 = {low:0, high:0x80000000}
VcmpEqUsingU32(isInt64MinDivMask, srcReg1, vInt64MinConst, mask); // 检测 divisor == INT64_MIN
pnot(nonInt64MinMask, isInt64MinDivMask, mask);
pand(mask, mask, nonInt64MinMask, mask);                 // 从主计算 mask 中排除 INT64_MIN
```

#### (c) INT64_MIN 结果赋值 (插入于 oneMask 结果选择之后, DivSignCal 之前)

```cpp
VcmpEqUsingU32(dividendIsInt64MinMask, srcReg0, vInt64MinConst, isInt64MinDivMask);
VselUsingU32(vTmp9, vConstDup1, vConstDup0, dividendIsInt64MinMask); // vTmp9 = 1 当 srcReg0==INT64_MIN, 否则 0
VselUsingU32(vTmp6, vTmp9, vTmp6, isInt64MinDivMask);                // 覆盖 vTmp6 的 INT64_MIN 元素
```

### 4.4 数据流影响分析

修复不会影响非 INT64_MIN 除数路径:
- `isInt64MinDivMask`, `dividendIsInt64MinMask`, `nonInt64MinMask` 仅在 INT64_MIN 除数元素上为 true
- `pand(mask, mask, nonInt64MinMask, mask)` 仅排除 INT64_MIN 元素, 其他元素不受影响
- 新增变量不修改现有变量的值路径
- vTmp9 在 INT64_MIN 结果赋值中复用, 其先前值 (lines 585, 589) 已不再需要

## 5. 验证用例

| # | 被除数 | 除数 | 期望结果 | 说明 |
|---|--------|------|---------|------|
| 1 | INT64_MIN | INT64_MIN | 1 | 自身除自身, 同符号→保持正结果 |
| 2 | 0 | INT64_MIN | 0 | 0除以任何非零数 = 0 |
| 3 | 1 | INT64_MIN | 0 | \|被除数\| < \|除数\| |
| 4 | -1 | INT64_MIN | 0 | \|被除数\| < \|除数\| |
| 5 | INT64_MAX | INT64_MIN | 0 | \|被除数\| = 2^63-1 < 2^63 |
| 6 | INT64_MIN + 1 | INT64_MIN | 0 | \|被除数\| < \|除数\| |
| 7 | 42 | INT64_MIN | 0 | 任意正常正值 |
| 8 | -42 | INT64_MIN | 0 | 任意正常负值 |
| 9 | 1 | 2 | 0 | 回归: 非 INT64_MIN 路径不受影响 |
| 10 | 100 | 3 | 33 | 回归: 正常除法 |
| 11 | -100 | 3 | -33 | 回归: 负被除数 |
| 12 | 100 | -3 | -33 | 回归: 负除数 (非 INT64_MIN) |
| 13 | INT64_MAX | 2 | 4611686018427387903 | 回归: 大数除法 |
| 14 | INT64_MIN | INT64_MAX | -1 | 回归: 负被除数 / 正除数 |

## 6. 已知局限

本次修复仅解决除数为 INT64_MIN 的精度问题。被除数为 INT64_MIN 且除数非 INT64_MIN 的
场景仍存在 ±1 的偏差 (根因 §3.4), 需单独修复 `AbsUsingS32` 或增加对称的特殊处理分支。
