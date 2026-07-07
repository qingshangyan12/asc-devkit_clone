# SIMT 小值/Subnormal 结果异常接口汇总（按根因分两类）

本文汇总 SIMT 数学接口中已在 dav-3510 真机实测、与非 FTZ libm 不一致，且
**属于缺陷（需报修，不只是改资料）** 的接口。按根因分为两类：

- **类别一 · 接口本身实现有问题（实现逻辑缺陷）**：缺陷在接口自身代码逻辑
  （small-x 退化路径缺失、float 精度吸收、位操作缺 implicit 1、缩放常量精度不足）。
  **即使 DML 不做 FTZ、subnormal 原样参与运算，结果依然错误。**
- **类别二 · 接口中间受 FTZ 影响导致结果出错（FTZ 连锁缺陷）**：接口对外输入/输出
  可能正常，但算法把一个 subnormal 中间量喂给 DML 原语（`__expf/__logf/__sqrtf`/除法），
  被输入/输出侧 FTZ 刷零为 `±0`，后续运算在零上产生 `Inf/NaN` 或错误 0。
  **若硬件不 FTZ（subnormal 原样参与），这类缺陷不会出现。**

## 判定口径

- Golden 使用非 FTZ libm，设备侧在 dav-3510（`--npu-arch=dav-3510`，非 CPU_DEBUG）实测。
- 与非 FTZ libm 不一致即记录为问题。
- **两类的判别关键**：假设 DML 不 FTZ（subnormal 原样参与运算），看缺陷是否仍存在。
  - 仍然出错 → **类别一（实现逻辑缺陷）**，根因在代码逻辑，与 FTZ 无关。
  - 不再出错 → **类别二（FTZ 连锁缺陷）**，根因在 DML FTZ，实现缺少防御。
- 与「**设计内 FTZ**」区分：`sqrtf`、`expf`、`exp2f`、`exp10f`、`fdividef`、`scalbnf`、
  `scalblnf`、`erfcf`、`erfcxf`、`jnf` 及 BF16 `hexp*/hsqrt` 等，仅是 DML 原语直接
  暴露的输入/输出 FTZ，结果可预期、无连锁错误，**只需改资料、不属本文缺陷**。

## 问题接口总览

### 类别一 · 接口本身实现有问题（实现逻辑缺陷）

| 接口 | 类型 | 现象 | 逻辑缺陷根因 |
| --- | --- | --- | --- |
| `expm1f` | F32 | subnormal / 小 `x` 返回 0，libm 返回约 `x` | `expf(x)-1.0f` 缺 small-x 退化，float `1+x`=1.0 精度吸收 |
| `log1pf` | F32 | subnormal / 小 `x` 返回 0，libm 返回约 `x` | `logf(1.0f+x)` 缺 small-x 退化，float `1+x`=1.0 精度吸收 |
| `tanhf` | F32 | subnormal / 小 `x` 返回 0，libm 返回约 `x` | `1-2/(expf(2x)+1)` 缺 small-x 退化，float `1+2x`=1.0 精度吸收 |
| `atanhf` | F32 | subnormal / 小 `x` 返回 0，libm 返回约 `x` | `logf((1+x)/(1-x))/2` 缺 small-x 退化，float `1±x`=1.0 精度吸收 |
| `atanf` | F32 | subnormal 输入结果被压小，甚至返回 0 | `__internal_sign` 中 `clip*2^62` 对 subnormal 精度不足，ratio≈0.2 而非 ±1 |
| `cbrtf` | F32 | subnormal 输入结果错误 | 位操作 `(bits&0x7FFFFF)\|(exp<<23)` 对 subnormal 缺 implicit 1 |
| `rcbrtf` | F32 | subnormal 输入结果错误 | 同 `cbrtf` |

### 类别二 · 接口中间受 FTZ 影响导致结果出错（FTZ 连锁缺陷）

| 接口 | 类型 | 现象 | FTZ 连锁路径 |
| --- | --- | --- | --- |
| `rsqrtf` | F32 | subnormal 输入 → `+Inf`，libm 为 finite | sn → DML `sqrtf` FTZ→0 → `1/0`=Inf |
| `hrsqrt` / `h2rsqrt` | BF16 | subnormal 输入 → `Inf`，libm 为 finite | 同 `rsqrtf`（BF16 upcast F32 后同路径） |
| `hrcp` / `h2rcp` | BF16 | subnormal 输入 → `Inf`，libm 为 finite | sn → DML 除法 FTZ→0 → `1/0`=Inf |
| `__hdiv` / `__hdivx2` | BF16 | `sn/sn` → `NaN`，libm 为 `1.0` | 双侧 FTZ→0 → `0/0`=NaN；商落 subnormal → 输出 FTZ→0 |
| `hypotf` / `rhypotf` | F32 | `hypotf(sn,sn)` → `NaN`，libm 为 finite | `b/a` DML 除法 `0/0`=NaN → fmaf → sqrtf 全 NaN |
| `norm3df` / `norm4df` | F32 | 含 subnormal 分量 → `NaN`，libm 为 finite | 归一化 `a/m` `0/0`=NaN，同 `hypotf` |
| `rnorm3df` / `rnorm4df` | F32 | 含 subnormal 分量 → `NaN`，libm 为 finite | `1/norm*`=`1/NaN`=NaN |
| `normf` / `rnormf` | F32 | dual-subnormal → `NaN`，libm 为 finite | 同 `hypotf`/`norm3df` |
| `atan2f` | F32 | `sn/sn` → `NaN`；`sn/normal` → `0` | `y/x` DML 除法双侧 FTZ→`0/0`=NaN 或 →0 |
| `ynf` | F32 | 大 `n` 小 `x`（如 `ynf(85,22)`）→ `±Inf`，libm 为 finite | 递推 `scale_factor` 落入 subnormal → DML 除法 FTZ→0 → `1/0`=Inf |
| `y1f` | F32 | max/mid subnormal 输入 → `-Inf`，libm 为 finite | 小 x 支 `-0.6366/x` subnormal 分母 FTZ→0 → 有限结果变 -Inf |

---

# 类别一 · 接口本身实现有问题（实现逻辑缺陷）

这些接口的缺陷完全在自身代码逻辑，**与 DML FTZ 无关**：即便把它们放到不 FTZ 的
硬件上、让 subnormal 原样参与运算，结果依然错误（多数是 float 加法在 `1.0` 附近
的精度吸收，或位操作/缩放常量本身有问题）。资料应写「实现缺少小幅值退化路径 /
对 subnormal 位模式处理不完整」，**不要**套用「输入/输出被刷零」的 DML FTZ 表述。

## expm1f

实现等价于：

```cpp
return expf(x) - 1.0f;
```

当 `x` 是 subnormal 或极小 normal 时，数学结果应为 `expm1f(x) ~= x`。但 `expf(x)`
在 F32 精度下舍入为 `1.0f`，随后 `1.0f - 1.0f = 0`，设备返回 0。libm `expm1f`
使用 small-x 专门路径保留约 `x`。

关键点：即使 DML 不 FTZ，`expf(subnormal)` ≈ `1+sn`，但 float 加法 `1+sn` = `1.0`
（精度吸收），bug 仍存在。正常域小 x 同样受影响：`expm1f(1e-10)` → 0 vs libm `1e-10`。

若 `x` 是较大负数使 `expf(x)` 本身落入 subnormal/被刷为 0，则 `expm1f(x)` 返回
`-1.0f`，这与 F32 舍入后的 libm 结果通常一致，不属本条缺陷。

## log1pf

实现等价于：

```cpp
return logf(1.0f + x);
```

`logf` 自身有 subnormal 输入预缩放保护（`x*2^23` 后 `__logf`），但 `log1pf` 的问题
发生在进入 `logf` 之前。F32 在 `1.0f` 附近 ULP 为 `2^-23`，当 `|x|` 小于半个 ULP 时
`1.0f + x == 1.0f`，随后 `logf(1.0f) = 0`。subnormal 输入远小于该阈值，必然被吸收。

即使 DML 不 FTZ，`1+sn`=1.0 在 float 加法中成立，bug 仍存在。正常域：`log1pf(1e-10)`
→ 0 vs libm `1e-10`；`log1pf(1e-8)` → 0 vs libm `9.999e-9`。根因是 float 精度吸收，
不是 `logf` 缺 subnormal 保护。

## tanhf

实现可近似理解为：

```text
tanhf(x) = 1 - 2 / (expf(2*x) + 1)
```

当 `x` 为 subnormal 或极小值时数学结果应为 `tanhf(x) ~= x`。但 `expf(2*x)` 在 F32
精度下舍入为 `1.0f`，`expf(2*x)+1` 的小增量被吸收，表达式退化为 `1 - 2/2 = 0`。

即使 DML 不 FTZ，`expf(2*sn)` ≈ `1+2sn`，但 float `1+2sn` = `1.0`，bug 仍存在。
与 `expm1f`/`log1pf` 同类，都缺 small-x Taylor 退化路径。

## atanhf

实现等价于：

```text
atanhf(x) = logf((1+x)/(1-x)) / 2
```

当 `x` 为 subnormal 或极小值时数学结果应为 `atanhf(x) ~= x`。但 `1.0f+x` 和 `1.0f-x`
都在 F32 精度下舍入为 `1.0f`，随后 `(1+x)/(1-x) = 1.0f`、`logf(1.0f)/2 = 0`。

即使底层 `logf` 有 subnormal 缩放保护也不生效，因为传给 `logf` 的已经是 `1.0f`；
即使 DML 不 FTZ，`1±sn`=1.0 在 float 加法中成立，bug 仍存在。`atanhf(±1)` 返回
`inf` vs libm `nan` 属 domain 处理差异，另计。

## atanf

`atanf` 的问题不由 DML FTZ 触发。subnormal 输入进入小值路径后，内部 `__internal_sign`
用 `clip * ATAN_FP32_MAX(2^62)`，对 subnormal 只得到小正常数（最小 normal 约 `5.4e-20`），
随后 `fabsf(result) + ATAN_FP32_MIN(2.168e-19)` 中的固定小常量主导分母，使比例项
`ratio ≈ 0.2` 而非 `±1`，最终 `dst * 0.2` 破坏结果，对最小 subnormal 甚至返回 0。

这是算法对 subnormal 精度处理不足（缩放常量选取问题），根因在代码逻辑而非 FTZ。
资料应写「subnormal 输入下结果可能明显偏小或返回 0」，不应写「输入被刷零」。

## cbrtf

问题来自位级初值构造。实现对 F32 exponent/mantissa 拆分重组，但 subnormal 数没有
规格化数的隐含 leading 1。若按规格化路径处理：

```text
mantissa = x_bits & 0x007fffff
exponent = 0        // subnormal 的 exp_bits=0
```

`(x_bits & 0x7FFFFF) | (exp_adj<<23)` 缺 implicit 1 bit，重组出的 `x_adjusted` 缺少
正确的 subnormal 规格化步骤，Newton 迭代从错误基准值开始，收敛到错误结果。例如
`cbrtf(0x00000001)` 得 ≈`1.8e-13` vs golden `1.12e-15`。

纯位操作/初值构造问题，**完全不经过 DML**，与 FTZ 无关。

## rcbrtf

与 `cbrtf` 同根因，依赖同类立方根初值/迭代路径，subnormal 输入在位级规格化阶段
处理不完整导致结果错误。

---

# 类别二 · 接口中间受 FTZ 影响导致结果出错（FTZ 连锁缺陷）

这些接口的实现逻辑对正常输入是正确的，缺陷在于：算法内部把一个 subnormal 中间量
送入 DML 原语（`__sqrtf`/除法等），被 FTZ 刷零为 `±0` 后，后续运算在零上产生
`Inf`/`NaN` 或错误 0。**假设硬件不 FTZ（subnormal 原样参与），这些缺陷不会出现。**
根因在 DML FTZ + 实现缺少 subnormal 防御（如未做输入预缩放 / 未旁路除法）。
资料应说明触发条件与规避建议。

## rsqrtf（及 BF16 hrsqrt / h2rsqrt）

实现为 `1.0f / sqrtf(x)`。subnormal 输入 `x` 被 DML `sqrtf` 输入侧 FTZ 当作 `0`，
`sqrtf(0)=0`，随后 `1.0f/0 = +Inf`，而 libm 返回 finite（约 `2e22`）。`-0` 输入返回
`-Inf` vs libm `+Inf`（符号翻转）。

若不 FTZ：`sqrtf(sn)` = small finite，`1/small` = finite，无缺陷。→ FTZ 连锁缺陷。
BF16 `hrsqrt`/`h2rsqrt` 经 upcast 到 F32 走同一路径，同根因（如 `rsqrt(2^-127)`→Inf
vs libm `1.30e19`）。

## BF16 hrcp / h2rcp

实现为 `1.0 / x`。subnormal denominator 被 DML 除法输入侧 FTZ 当作 `0`，`1/0 = Inf`，
而 libm 为 finite（如 `rcp(2^-127)` → Inf vs libm `1.70e38`）。若不 FTZ 则 `1/sn` =
finite。→ FTZ 连锁缺陷。（部分极小 subnormal 的 `1/sn` 数学上本就溢出为 Inf，与 libm
一致，不计入。）

## BF16 __hdiv / __hdivx2

BF16 经 `cvt_float→F32→DML 除法→cvt_bfloat16`。DML 除法双侧 FTZ：

- `sn/sn` → `0/0` = `NaN` vs libm `1.0`（FTZ 连锁缺陷）
- `1/sn_max` → `Inf` vs libm `8.57e37`（输入侧 FTZ 连锁）
- 商落入 subnormal（如 `MN/2`）→ 输出侧 FTZ → `0` vs libm `5.88e-39`（设计内输出 FTZ）

`sn/sn`→NaN 与 `1/sn_max`→Inf 若不 FTZ 都不会发生 → FTZ 连锁缺陷。`__hdivx2` 为逐分量
同实现路径，同结论。（注：**F16** `__hdiv` 真机实测 0 DIFF，不受影响。）

## hypotf / rhypotf

`hypotf` 实现含 `r = b/a`、`a*sqrtf(fmaf(r,r,1))`。当两操作数都是 subnormal 时，
`b/a` 的 DML 除法双侧 FTZ → `0/0` = `NaN` → `fmaf(NaN,...)` = NaN → `sqrtf(NaN)` = NaN
→ `a*NaN` = NaN，而 libm 返回 finite。

正常案例正确：`hypotf(0,sn)`→`|sn|`；`hypotf(sn,normal)`→`normal`（`b/a`→FTZ→0→`sqrt(1)`=1）。
`rhypotf` = `1/hypotf`，`1/NaN`=NaN 同根因。若不 FTZ，`b/a`=finite 比值，无缺陷。

## norm3df / norm4df / rnorm3df / rnorm4df / normf / rnormf

与 `hypotf` 完全同根因。归一化时 `m = max(|分量|)`；当 `m` 为 subnormal 时，
`分量/m` 的 DML 除法 `0/0` = `NaN` → fmaf → sqrtf → `m*NaN` 全程 NaN，libm 为 finite。
含 subnormal 分量的输入几乎全部触发 unexpected NaN。`rnorm*`/`rnormf` = `1/norm*` =
`1/NaN` = NaN。正常案例（如 `normf(2,[sn,normal])`→1）正确。

## atan2f

`atan2f(y,x)` 内部 `y/x` 用 DML 除法：

- dual-subnormal：`sn/sn` → 双侧 FTZ → `0/0` = `NaN` → `atanf(NaN)` = NaN
- single-subnormal numerator：`sn/normal` → FTZ → `0` → `atanf(0)` = 0 vs libm ≈`y/x`

float 比较本身保留 subnormal（`x==0` 对 sn 为 false），但 DML 除法对操作数 FTZ。
若不 FTZ，`y/x` = finite 比值，无缺陷。特殊值 `NaN/±Inf/±0` 与 libm 一致。

## ynf

subnormal 输入本身一致（数学值超 FLT_MAX，两路均 -Inf）。**缺陷出在正常域**：
`ynf(85, 22.041975)` 设备返回 `-Inf`，libm 返回 `-1.17e38`（finite）。根因：递推
case3 的缩放因子 `scale_factor` 经 84 步累积后落入 subnormal 区（`-8.55e-39`），最终
`value/scale_factor` 的 DML 除法对 subnormal denominator FTZ → `0`，`1/(-0)` = `-Inf`。

数学结果 `-1.17e38` 是正常 finite，不应出现 `-Inf` → FTZ 连锁缺陷。触发条件：`n` 较大
（约 ≥70）且 `x` 在 `n*0.25 ~ n*10` 范围时 `scale_factor` 易落入 subnormal。

## y1f

小 x 分支入口先算 `-0.636619772367f / x`。max subnormal（`0x007FFFFF`）和 mid
subnormal（`0x00400000`）输入时，subnormal 分母被 DML 除法 FTZ → `0`，有限数学结果
变为 `-Inf`，而 libm 返回 finite（`-5.42e37` / `-1.08e38`）。

最小 subnormal / `1e-40` 输入时数学值 `Y_1(x)` 已超 FLT_MAX，两路均 `-Inf`（overflow
掩盖 FTZ，不计）；`MIN_NORMAL`（`0x00800000`）→ finite ✓。→ FTZ 连锁缺陷。

---

## 资料修订建议

### 类别一（实现逻辑缺陷）措辞

```text
针对Ascend 950PR/Ascend 950DT，本接口在Subnormal或极小输入场景下存在精度偏差：
由于实现缺少小幅值退化路径/对Subnormal位模式处理不完整，返回值可能为0或明显偏离数学结果。
（该偏差源于接口实现逻辑，与DML的Subnormal刷零无关。）
```

### 类别二（FTZ 连锁缺陷）措辞

```text
针对Ascend 950PR/Ascend 950DT，本接口在输入/中间量落入Subnormal范围时，
内部DML原语（开方/除法）会将其刷新为保留符号的0，后续运算可能产生非预期的Inf/NaN或0。
对精度敏感的业务，建议避免在该幅值区间调用，或在调用前对输入做预缩放。
```

### 不要使用的统一表述

```text
处于Subnormal范围内的输入和输出值，都会被刷新为保留符号的0。
```

该表述仅适用于 `sqrtf`、`expf`、`fdividef`、`scalbnf`、`erfcf/erfcxf` 等**设计内 FTZ**
接口（结果可预期、无连锁错误，只需改资料），**不适用于本文两类缺陷接口**。

> 完整逐接口实测留痕见 `subnormal_impact_simt_api.md` 第 6 节；缺陷分类总览见其
> 第 5.1 节（本文类别一 = 该文「C. 实现逻辑缺陷」，类别二 = 该文「B. FTZ 连锁缺陷」）。
