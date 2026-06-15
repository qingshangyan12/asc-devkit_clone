# SIMT API 在 DML 不支持 subnormal 场景下的受影响接口与重测方案

## 1. 背景与硬件约束

目标硬件移除了 SFU（Special Function Unit）指令，`Div / Ln / Sqrt / Exp`
四类超越/算术原语改由 DML（向量数学单元，对应底层 `vdiv / vln / vsqrt / vexp`
指令）实现。该 DML 实现 **不支持 subnormal（非规格化数）**。

本文采用的 subnormal 语义（已确认）：

> **输入侧与输出侧均 FTZ（Flush-To-Zero）。**
> 即：当 DML 原语的输入是 subnormal 时，被当作 `±0` 参与运算；当 DML 原语
> 产生的真实结果落入 subnormal 区间时，输出被刷零为 `±0`。

F32/F16/BF16 三类数据类型的 subnormal 阈值（绝对值下界，即最小规格化数）：

| 类型  | 最小规格化数（subnormal 上界） | 备注 |
|-------|-------------------------------|------|
| F32   | `1.17549435e-38`（`0x00800000`） | 代码中多处以此为分界常量 |
| F16   | `6.103515625e-05`（`2^-14`）     | F16 直接路径 |
| BF16  | `1.17549435e-38`（与 F32 同阶）  | BF16 先 upcast 到 F32 再运算，按 F32 阈值 |

> **关键点（FTZ 双侧语义的含义）**：受影响的不仅是「用户可见输入/输出落在
> subnormal 区」的接口。由于输入侧也 FTZ，**任何在算法内部把一个 subnormal
> 中间量喂给 DML 原语、或 DML 原语内部产生 subnormal 中间结果的接口，即使其
> 对外输入和输出都完全正常，也可能因为中间被刷零而结果失真。** 因此本文按
> 「全量传递闭包」口径统计受影响接口（含中间值）。

## 2. 代码路径与 DML 原语映射

> **测试范围**：仅 C 风格接口（`math_functions.h` 的 `*f`）与 F16/BF16 `h*`。
> 这些接口的实现位于 `impl/simt_api/` **顶层**，**不经过** `impl/simt_api/cpp/`
> （后者是 C++ 大写接口层 `Sqrt/Exp/...` 的实现，整层不在测试范围）。

被测接口的实现文件：

- `impl/simt_api/math_functions_impl.h`（C 风格 `*f`，独立实现，不 include cpp/）
- `impl/simt_api/asc_fp16_impl.h`（F16 `h*`）
- `impl/simt_api/asc_bf16_impl.h`（BF16 `h*`，upcast F32 后调 `expf/logf/sqrtf`）

四个 DML 原语在 **C 风格实现** `math_functions_impl.h` 中的入口
（**subnormal 风险的真正源头**）：

| DML 原语 | 代码入口 | 行号 | 说明 |
|----------|----------|------|------|
| Exp | `return __expf(x)` | :98 | 大负输入 → 输出 subnormal 被刷零 |
| Log | `return __logf(x)` | :106 | 已带 subnormal 输入预缩放（见下）|
| Sqrt | `return __sqrtf(x)` | :116 | subnormal 输入被当 0 |
| Div/Rcp | `1.0f / x`、`a / b` | 散布各函数 | 如 `rsqrtf`=`1.0f/sqrtf(x)` :121 |

> **既有的 subnormal 规避代码**：`logf`（math_functions_impl.h:103-104）与
> `ComputeLn`（:1810-1812）已对「输入为 subnormal」做了预缩放
> （`x * 2^23` 后再 `__logf`，结果减 23）。这说明 Log 路径对**输入 subnormal**
> 已有保护，重测应重点验证该保护在新硬件 FTZ 行为下是否仍然成立、以及
> **输出 subnormal** 和**中间 subnormal** 是否仍未覆盖。其余三个原语
> （Exp/Sqrt/Div）未见等价保护。

## 3. 受影响接口清单（按 DML 原语依赖分类）

风险等级定义：

- **高**：DML 原语直接暴露给用户，或正常输入即可触发 subnormal 输入/输出
  （如 Exp 大负数输出、Log 接近 0 的输入、Sqrt/Div 小值）。
- **中**：算法内部存在 subnormal 中间量风险，需在特定输入域才触发。
- **低**：仅在极端/退化输入下才可能间接受影响。

### 3.1 直接原语接口（高风险，必测）

> 接口名一律用 **C 风格**（`math_functions.h` 的 `*f` 小写）；C++ 风格大写接口层
> （`include/simt_api/cpp/`）不在本次测试范围。F16/BF16 用 `asc_fp16.h`/`asc_bf16.h` 的 `h*`。

| 公开 API | 类型 | 依赖原语 | subnormal 介入点 | 风险 |
|----------|------|----------|------------------|------|
| `expf` / `exp2f` / `exp10f` / `expm1f` | F32 | Exp | 大负输入 → 输出 subnormal 被刷零；`exp2f/exp10f/expm1f` 经 `powf/expf` | 高 |
| `logf` / `log2f` / `log10f` / `log1pf` | F32 | Log(+Div) | 输入接近 0 / subnormal 输入；`log2f/log10f` 还含 `LogImpl/LogImpl` 除法 | 高 |
| `sqrtf` | F32 | Sqrt | subnormal 输入被当 0；接近 0 的小正数 | 高 |
| `rsqrtf` | F32 | Sqrt+Div | `1.0f/SqrtImpl(x)`，大输入 → Sqrt 大 → 倒数 subnormal | 高 |
| `powf` | F32 | `__powf`(Exp+Log) | 底数接近 0 / 结果接近 0 | 高 |
| `hexp/hexp2/hexp10` `h2exp*` | F16 | Exp(`__expf`) | F16 subnormal 阈值更大(2^-14)，更易触发 | 高 |
| `hlog/hlog2/hlog10` `h2log*` | F16 | Log | 同上 | 高 |
| `hsqrt` `h2sqrt` | F16 | Sqrt | 同上 | 高 |
| `hrsqrt` `h2rsqrt` | F16 | Sqrt+Div | `1.0/hsqrt(x)` | 高 |
| `hrcp` `h2rcp` | F16 | Div | `1.0/x`，大输入 → 倒数 subnormal | 高 |
| `hexp/hlog/hsqrt/hrsqrt/hrcp`(BF16) `h2*` | BF16 | Exp/Log/Sqrt/Div | upcast F32 后调 `expf/logf/sqrtf`，按 F32 阈值 | 高 |
| `__hdiv` `__hdivx2`(F16/BF16) | F16/BF16 | Div | 商落入 subnormal | 高 |
| `fdividef` | F32 | Div | 商落入 subnormal | 高 |

### 3.2 含 subnormal 中间值的组合接口（中风险，重点排查）

这些接口对外输入/输出可能完全正常，但内部把中间量送入 DML 原语（接口名同样取 C 风格）：

| 公开 API | 类型 | 内部依赖 | 中间 subnormal 风险点 | 风险 |
|----------|------|----------|------------------------|------|
| `rsqrtf` | F32 | `1/Sqrt` | 见 3.1（也算直接） | 高 |
| `asinf` / `acosf` | F32 | Sqrt | `SqrtImpl(1-x*x)`，x→±1 时 `1-x*x`→0（subnormal 中间） | 中 |
| `acoshf` | F32 | Log+Sqrt | `Log(x+Sqrt(x*x-1))`，x→1⁺ 时 `x*x-1`→subnormal | 中 |
| `asinhf` | F32 | Log+Sqrt | `Log(x±Sqrt(x*x+1))` | 中 |
| `atanhf` | F32 | Log+Div | `Log((1+x)/(1-x))/2`，x→±1 时商溢出/趋零 | 中 |
| `atanf` / `atan2f` | F32 | Div | `y/x`、内部 `tmp/denominator` | 中 |
| `tanf` | F32 | Div | `-1.0f/z`（quadrant 调整） | 中 |
| `coshf` / `sinhf` | F32 | Exp+Div | `Exp(...)±0.25/Exp(...)`，大 \|x\| 时 Exp 项 subnormal | 中 |
| `tanhf` | F32 | Exp+Div | `1-2/(Exp(2x)+1)` | 中 |
| `hypotf` / `rhypotf` | F32 | Sqrt+Div | `r=b/a`、`a*Sqrt(fma(r,r,1))`；`rhypotf=1/hypotf` | 中 |
| `norm3df`/`norm4df`/`normf` | F32 | Sqrt+Div | `a/m` 归一化项可能 subnormal；`m*Sqrt(sum)` | 中 |
| `rnorm3df`/`rnorm4df`/`rnormf` | F32 | Sqrt+Div | `1/Norm*`，大模长 → 倒数 subnormal | 中 |
| `cbrtf` / `rcbrtf` | F32 | Div | 牛顿迭代 `xAdjusted/(y*y)`、`4-x*y³` | 中 |
| `erff`/`erfcf`/`erfcxf` | F32 | Exp2+Div | 多处 `1.0f/term`、`Exp2(...)` | 中 |
| `erfinvf`/`erfcinvf` | F32 | Log2+Rsqrt+Div | `Rsqrt(negLog)`、`Log2`、`1/rsqrt` | 中 |
| `normcdff` | F32 | Erfc(→Exp/Div) | 经 `ErfcImpl` | 中 |
| `tgammaf`/`lgammaf` | F32 | Exp/Log/Sqrt/Div/Pow | Stirling/Euler 反射含 `Pow(2,f)`、`1/(...)`、`ComputeLn` | 中 |
| `cyl_bessel_i0f`/`cyl_bessel_i1f` | F32 | Exp+Rsqrt | `Exp(absX*0.5)`、`y*Rsqrt(absX)` | 中 |
| `j0f`/`j1f`/`jnf` | F32 | Div(+Trig) | 大 \|x\| 渐近支 `inv_x=1/x`、`__internal_cal_*_alpha` 含除法；jnf 递推 `2n/x` | 中 |
| `y0f`/`y1f`/`ynf` | F32 | Div+Log(+Trig) | 同 j*f 的 `1/x` 渐近支；小 x 支含 `logf(x)`(如 `__internal_cal_y1_x_less1dot2`)；ynf 递推 `2n/x` | 中 |
| `cospif`/`sinpif`/`tanpif`/`sincosf`/`sincospif` | F32 | (经 cos/sin/tan/Div) | 三角主体含 Div（tan 路径） | 中 |
| `scalbnf`/`scalblnf` | F32 | Pow(Exp+Log)+Div | 实现非纯位操作：`x / __powf(2,n)`(n>127 分两步)，`__powf`=Exp+Log | 中 |
| `frexpf`/`ldexpf`/`logbf`/`ilogbf` | F32 | 位运算+Div | 主要位操作，Div 介入有限 | 低 |
| `atan2f`(inf 分支) | F32 | — | 特例分支不走 Div | 低 |

### 3.3 不受影响（参考，无需重测 subnormal）

纯整数/位运算或纯比较类接口，不经过 DML 原语：
`Abs / Fabs / Max / Min / Fmax / Fmin / CopySign / Signbit / Trunc / Round /
Rint / Floor / Ceil / NearbyInt / Fma / Labs / Llabs / Clz / Popc / Brev /
Ffs / IsNan / IsInf / IsFinite / Fdim / Fmod / Remainder`（不含内部除法的实现）。

> 注：`fmodf/remainderf/remquof` 若实现含除法需复核，已在 3.2/低风险归类外
> 单独确认其 `math_functions_impl.h` 实现是否调用 DML 除法。

## 4. 重测方案

### 4.1 总体策略：双轨验证

对每个受影响接口，在两个输入域分别用不同准则验证：

1. **正常域 ULP 精度轨**：输入取规格化数（避开 subnormal 边界），验证移除
   SFU 改用 DML 后**精度不退化**。准则：与黄金参考的 ULP 误差 ≤ 接口既定阈值。
2. **subnormal 边界契约轨**：输入/输出/中间量刻意落在 subnormal 区，验证
   **FTZ 行为符合预期且不产生意外 NaN/Inf**。准则见 4.4。

### 4.2 黄金参考（golden）

- 基准：主机 CPU 高精度 `double` 计算后转目标类型；或 glibc `libm` 的 `*f` 版本。
- **FTZ 对齐**：CPU 默认不 FTZ，而目标硬件双侧 FTZ。为公平对比，golden 侧需
  套一层 FTZ 包装：
  - 对每个进入「等效 DML 原语」的输入，若 `|v| < 最小规格化数` 则先置 `±0`；
  - 对每个等效 DML 原语的输出，若结果落入 subnormal 则置 `±0`。
  - 即：用一个**软件 DML 模型**（带 FTZ 的 `exp/log/sqrt/div`）复算组合函数，
    作为「设计期望」golden，而非直接拿不 FTZ 的 libm 结果当 golden。

> 理由：若直接用未 FTZ 的 libm 当 golden，会把「硬件按设计 FTZ」误判为精度
> 失败。双轨中的契约轨正是为了把「设计内的 FTZ」与「真正的 bug」区分开。

### 4.3 输入用例构造

每个接口的输入集合至少覆盖：

**A. 正常域（ULP 轨）**
- 典型区间均匀/对数采样（如 `Exp`：`[-87, 88]`；`Log`：`[1e-30, 1e30]`）。
- 各接口已知的算法分支切换点附近（如 `Asin` 的 `±0.7071`、`Atan` 的 `10000`
  clip、`Tgamma` 的 `1.5 / 33 / 41.1` 阈值、`Erf` 的 `1.00296` 分支）。

**B. subnormal 边界域（契约轨）** —— 按介入点分别构造：
- **输入 subnormal**：直接送入最小规格化数下方的值，如 F32
  `0x007FFFFF`(最大 subnormal)、`0x00000001`(最小 subnormal)、`±0`；
  F16 取 `2^-14` 下方各值。验证「输入被当 0」。
- **输出 subnormal**：构造使真实结果落入 subnormal 的输入，如
  `Exp(-88f)`、`Sqrt(最小 subnormal)`、`hrcp(大值)`、`Rsqrt(大值)`。
  验证「输出被刷零为 ±0」。
- **中间 subnormal**（针对 3.2 组合接口）：构造使内部某一步进入 subnormal，
  例如 `Asin(x→±1)` 使 `1-x²`→subnormal、`Acosh(x→1⁺)` 使 `x²-1`→subnormal、
  `Atanh(x→±1)`、`Cosh(大|x|)` 使单个 `Exp` 项 subnormal、`Norm*` 的归一化项。

**C. 特殊值**（沿用现有 UT 习惯，见 test_operator_simt_transcendental.cpp）
- `NaN / +Inf / -Inf / ±0`，验证与文档 special cases 一致，不被 FTZ 干扰。

### 4.4 判定准则（两层）

判定分两层，回答两个不同的问题：

**第一层 —— 与非 FTZ libm 是否一致（决定要不要改资料）**：这是总准则，
**与非 FTZ libm 不一致即记为「有问题/需在资料说明」**。一致则无需动资料；
不一致则进第二层归因并在资料中说明。

> subnormal 输入被刷零后结果必然与 libm 不一致 → 即"要写资料"，不要因"刷零干净"
> 当成没事。

**第二层 —— 归因（决定怎么处理，仅对不一致的点）**：

| 归因 | 判据 | 处理 |
|------|------|------|
| 设计内 FTZ | ① subnormal 输入结果 == 把输入换 ±0 的结果；② 落入 subnormal 的输出 == `±0`（符号正确）；③ 无设计外 `NaN/Inf`；④ 特殊值与文档 special cases 一致 | 改资料，说明为 DML FTZ 设计行为 |
| 真 bug | 上述任一条不满足 | 报实现缺陷，不当 FTZ 写资料 |

**ULP 精度轨（正常域）**：规格化输入下与 golden 比较，误差 ≤ 接口阈值（高阶超越函数
建议 ≤ 4 ULP，原语 Exp/Log/Sqrt ≤ 2 ULP，Div/Rcp ≤ 1 ULP），验证精度不退化。

### 4.5 测试实现落点（与现有框架对齐）

复用 `tests/api/simt_api/` 既有 gtest + `__simt_vf__` kernel + mode 分发结构：

- 新增/扩展用例文件（建议）：
  - `test_operator_simt_subnormal_f32.cpp`（math + transcendental，F32）
  - `test_operator_simt_subnormal_f16.cpp`（`asc_fp16` 接口）
  - `test_operator_simt_subnormal_bf16.cpp`（`asc_bf16` 接口，放 950pr 目录或对应 3510 目录）
- 每个 kernel 用 `mode` 分发到一个被测接口，`src0/src1` 预置 4.3 的输入向量，
  `dst` 写回后在 host 侧与 golden 比对（参考现有文件 `src0[0]=NAN; src0[1]=INFINITY; ...` 的预置写法）。
- host 侧实现 4.2 的「FTZ 软件 DML golden」工具函数（`ftz_exp/ftz_log/ftz_sqrt/ftz_div`
  及其组合），统一供三个文件调用。

### 4.6 优先级（执行顺序建议）

1. **P0**：四个直接原语 `Exp/Log/Sqrt/(Rcp/Div)` 的 F32 + F16/BF16 包装（3.1 全部）。
2. **P1**：`Rsqrt/Pow/Hypot/Rhypot/Norm*/Rnorm*/Cosh/Sinh/Tanh`（高频且中间 subnormal 明确）。
3. **P2**：`Asin/Acos/Acosh/Asinh/Atanh/Atan/Atan2/Cbrt/Rcbrt`。
4. **P3**：`erff/erfcf/erfcxf/erfinvf/erfcinvf/normcdff/normcdfinvf/tgammaf/lgammaf/cyl_bessel_i*f/j0f/j1f/jnf/y0f/y1f/ynf/scalbnf/scalblnf` 等复杂多分支函数。

## 5. 附：受影响接口总览（去重计数）

> **统计口径（已用脚本对 `math_functions_impl.h` 全量公开函数做传递闭包核对）**：
> 88 个 F32 公开接口中，**55 个**实现路径（含宏展开 `__INTERNAL_NORMF` 等、
> 内部 helper、`__powf`=Exp+Log 内联）上出现 `__expf/__logf/__sqrtf/__powf/除法`，
> 即受 subnormal/FTZ 影响；**29 个**纯位运算/比较/多项式（无 DML 原语）不受影响
> （`ceilf copysignf cosf cospif fabsf fdimf floorf fmaf fmaxf fminf frexpf ilogbf
> isfinite isinf isnan logbf modff nearbyintf nextafterf remainderf remquof
> rintf roundf signbit sincosf sincospif sinf sinpif truncf`；其中 `fmodf` 唯一的
> `/` 是 `INF/INF` 常量 NaN，不走 subnormal；`normf` 经宏 `__INTERNAL_NORMF` 含
> `a[i]/m`、`m*sqrtf(sum)`，归入受影响，与 `norm3df/norm4df` 同列）。

- **直接 DML 原语接口**（高）：约 13 组（含 F32/F16/BF16 与标量/向量变体）。
- **含中间 subnormal 的组合接口**（中）：约 41 个（脚本核对补入：`j0f/j1f/jnf/
  y0f/y1f/ynf` 贝塞尔、`scalbnf/scalblnf`(`x/__powf(2,n)`)、`normcdfinvf` 等先前
  凭经验枚举时遗漏的接口）。
- **数据类型**：F32、F16、BF16 全覆盖；BF16 经 F32 路径，按 F32 阈值。
- **核心结论**：凡是其 C 风格实现（`math_functions_impl.h`）或 F16/BF16 实现
  （`asc_fp16_impl.h`/`asc_bf16_impl.h`）路径上出现 `__expf`、
  `__logf`、`__sqrtf`、或除法 `/`(Div/Rcp) 的接口，
  均在「输入/输出/中间任一处出现 subnormal」时存在结果不符合预期的风险，
  需按第 4 节双轨方案重测。

## 6. 实测记录（逐接口闭环）

按第 4 节方案，在 `/dev/davinci0` 真机（`--npu-arch=dav-3510`，非 CPU_DEBUG）
上逐个接口实测，golden 用非 FTZ libm（`std::sqrt` 等的 double 版转 float），
判定准则：**与非 FTZ libm 不一致即记为问题**，并据此修订对应接口 `.md` 资料。

测试用例位于 `simt_rdv_new/test_case_agent/test_f32/`，复用该目录
`build.sh dav-3510 <test_dir>` 框架（ACL + `__simt_vf__` kernel + `asc_vf_call`）。

| 序号 | 接口 | 类型 | 输入 subnormal | 输出 subnormal | 特殊值 | 正常域 ULP | 结论 | 资料已改 |
|------|------|------|----------------|----------------|--------|------------|------|----------|
| 1 | `sqrtf` | F32 | **FTZ**：5/5 例输出 0（libm 为 ~1e-19~1e-23）→ 不一致 | 本批未触发（sqrt 输出落不进 subnormal 区） | NaN/±Inf/负数/±0 全一致，无设计外 NaN/Inf | 1.0/2.0/4.0/0.25/1e30/1e-30 全 ok，精度不退化 | **输入侧 FTZ 确认**；行为属设计内但原资料未载明 | 是（sqrtf.md 补「返回值说明/约束说明」）|
| 2 | `y0f` | F32 组合(Div+Log+Sqrt) | 5/5 例**一致**：subnormal 输入经 logf 预缩放保护，不触发 FTZ | 未触发 | NaN/±Inf/±0/负数/1e14 全一致（1e14>1e13 为实现截断 return 0，非 FTZ） | 0.1~1e6 各分支全 ok，精度不退化 | **不受 subnormal/FTZ 影响**；logf 预缩放(x*2^23)保护有效；预筛曾错判 IMPACT | 否（实测与 libm 一致，无需补充）|
| 3 | `ynf` | F32 组合(Div+Exp+Log+Sqrt) | 8/8 例**一致**（数学值已超 FLT_MAX→两路均 -Inf） | **中间值 FTZ 真bug**：n≈85,x≈22 时递推 `scale_factor` 变 subnormal(-8.55e-39)→最终 `1/scale_factor` DML Div FTZ→0→返回 -Inf（libm 正确返回 -1.17e38 finite） | NaN/±Inf/±0/负数/n<0 全一致 | n=5~60 ok；n=85 x=22 **真 bug**（-Inf vs -1.17e38）；n=85 x≥50 ok | **真 bug**：递推缩放 scale_factor 落入 subnormal 被 FTZ→除法结果错误为 ±Inf；非设计内 FTZ | 是（ynf.md 约束说明补中间值 subnormal bug + 规避建议）|
| 4 | `expf` | F32 直接(Exp) | subnormal 输入 FTZ→exp(subnormal)≈1 一致（数学值≈1） | **输出侧 FTZ**：exp(-88)=0 vs libm 6.05e-39(subnormal)；exp(-149)=0 两路均 0 ok | 全一致 | 正常域全 ok | **输出侧 FTZ 确认**：exp 大负输入的 subnormal 输出被刷零为 +0；设计内 FTZ | 待补 |
| 5 | `logf` | F32 直接(Log) | 3/3 例**一致**：subnormal 输入经预缩放保护(x*2^23)完全正确 | 未触发（log 输出不落 subnormal） | 全一致 | 全 ok | **不受 subnormal/FTZ 影响**：预缩放保护有效 | 否 |
| 6 | `rsqrtf` | F32 直接(Sqrt+Div) | **输入侧 FTZ**：3/3 subnormal 输入→FTZ→1/√0=+Inf vs libm finite(~2e22)→**设计外 Inf** | 未触发 | -0→-Inf vs libm +Inf（符号翻转） | 正常域全 ok | **输入侧 FTZ 确认+真bug**：subnormal x 被 FTZ→0→√0=0→1/0=Inf而非数学正确值 finite | 待补 |
| 7 | `fdividef` | F32 直接(Div) | **输入+输出侧 FTZ**：1/max_subnormal→Inf vs libm 8.5e37(**设计外 Inf**)；subnormal numerator→0 | **输出侧 FTZ**：1/FLT_MAX→0 vs libm 2.94e-39(subnormal)；1e-38/10→0 vs 1e-39 | 全一致 | 正常域全 ok | **双侧 FTZ 确认**：商落入 subnormal 被 FTZ→±0；subnormal 操作数也被 FTZ→±0 再除 | 待补 |
| 8 | `powf` | F32 直接(Exp+Log) | subnormal base→FTZ→pow(0,exp)=0 vs libm nonzero | 输出侧 FTZ：2^-126→0 vs libm MIN_NORMAL；2^-149→0（输出 subnormal FTZ） | Inf^0→NaN vs libm 1(**unexpected**) | 正常域全 ok | **双侧 FTZ+特殊值偏差**：base/result subnormal 被 FTZ；Inf^0 应返回 1 但返回 NaN | 待补 |
| 9 | `exp2f` | F32 直接(=powf(2,x)) | subnormal 输入→exp2f(sn)≈1 一致 | **输出侧 FTZ**：2^-126→0 vs libm FLT_MIN；2^-149→0 vs smallest subnormal | 全一致 | 正常域全 ok | **输出侧 FTZ 确认**：大负输入的 subnormal 输出被刷零；路径经 expf DML 原语，设计内 FTZ | 待补 |
| 10 | `exp10f` | F32 直接(=powf(10,x)) | subnormal 输入→exp10f(sn)≈1 一致 | **输出侧 FTZ**：10^-38→0 vs libm 1e-38(subnormal)；10^-40→0 vs 1e-40 | 全一致 | 正常域全 ok | **输出侧 FTZ 确认**：与 expf 同机制，设计内 FTZ | 待补 |
| 11 | `expm1f` | F32 直接(=expf(x)-1) | **输入侧 FTZ**：subnormal x→1+x=1.0→expm1=0 vs libm≈x | 未触发 | 全一致 | 1e-10→0 vs libm 1e-10(**精度偏差**) | **输入侧 FTZ+精度真bug**：subnormal 被 float 加法吸收→expm1=0；正常域小 x(≤1e-10)也无 Taylor 退化路径→0 vs libm≈x | 待补 |
| 12 | `log2f` | F32 直接(=logf/logf(2)) | 3/3 例**一致**：logf 预缩放保护有效 | 未触发 | 全一致 | 全 ok | **不受 subnormal/FTZ 影响**：logf 预缩放 + 常量除法不引入 FTZ | 否 |
| 13 | `log10f` | F32 直接(=logf/logf(10)) | 3/3 例**一致**：同 log2f | 未触发 | 全一致 | 全 ok | **不受 subnormal/FTZ 影响**：同 log2f | 否 |
| 14 | `log1pf` | F32 直接(=logf(1+x)) | **输入侧 FTZ**：subnormal x→1+x=1.0→log1p=0 vs libm≈x | 未触发 | 全一致 | 1e-10→0 vs libm 1e-10；1e-8→0 vs 9.999e-9(**精度偏差**) | **输入侧 FTZ+精度真bug**：1+subnormal=1.0→log1p=0；正常域小 x 也因 float 加法截断→0 vs libm≈x | 待补 |
| 15 | F16 `hexp` | F16 直接(__expf on half) | subnormal→exp(≈0)≈1 一致 | 未触发（F16 exp 输出域无 subnormal 产生） | 全一致 | 全 ok | **不受 subnormal/FTZ 影响**：half subnormal 经 DML __expf 后≈1，与 libm F16 结果一致 | 否 |
| 16 | F16 `hexp2` | F16(=powf(2,cvt_float)) | subnormal→≈1 一致 | 1 DIFF：hexp2(-24)=0 vs 0.25（F16 输出域截断） | 全一致 | 正常域全 ok | **输出侧 FTZ**：F16 值域极窄(2^-14~2^16)，subnormal 输出在 F16 中无表示 | 否 |
| 17 | F16 `hexp10` | F16(=powf(10,cvt_float)) | subnormal→≈1 一致 | 未触发 | 全一致 | 全 ok | **不受影响**：F16 值域内无额外 FTZ | 否 |
| 18 | F16 `hlog` | F16 直接(__logf on half) | 3/3 例**一致**：subnormal 输入经 DML __logf 保护 | 未触发 | 全一致 | 全 ok | **不受 subnormal/FTZ 影响**：__logf 对 half subnormal 处理正确 | 否 |
| 19 | F16 `hlog2` | F16(=logf/log2) | 3/3 例**一致** | 未触发 | 全一致 | 全 ok | 同 hlog | 否 |
| 20 | F16 `hlog10` | F16(=logf/log10) | 3/3 例**一致** | 未触发 | 全一致 | 全 ok | 同 hlog | 否 |
| 21 | F16 `hsqrt` | F16 直接(__sqrtf on half) | 3/3 例**一致**：sqrt(subnormal_half)→F16 normal | 未触发 | 全一致 | 全 ok | **不受影响**：sqrt 将 F16 subnormal 输入放大到 normal 范围 | 否 |
| 22 | F16 `hrsqrt` | F16(=1/hsqrt) | 3/3 例**一致**：1/sqrt(sn)=large F16 normal 或 Inf | 未触发 | 全一致 | 全 ok | **不受影响**：结果在 F16 正常域或 math overflow | 否 |
| 23 | F16 `hrcp` | F16(=1/half) | 3/3 例**一致**：1/sn=F16 Inf 或 large normal（math overflow） | 未触发 | 全一致 | 全 ok | **不受影响**：F16 值域限制使 1/sn overflow 为 Inf，与数学一致 | 否 |
| 24 | BF16 `hexp` | BF16(cvt→F32→expf→cvt) | subnormal→≈1 一致 | **输出侧 FTZ**：hexp(bf16(-88))→0 vs libm 5.97e-39 | 全一致 | 正常域全 ok | **输出侧 FTZ 确认**：与 F32 expf 同机制，设计内 FTZ | 待补 |
| 25 | BF16 `hexp2` | BF16(cvt→F32→exp2f→cvt) | subnormal→≈1 一致 | **输出侧 FTZ**：2^-126→0 vs FLT_MIN；1 borderline | 全一致 | 正常域全 ok | 同 F32 exp2f，设计内 FTZ | 待补 |
| 26 | BF16 `hexp10` | BF16(cvt→F32→exp10f→cvt) | subnormal→≈1 一致 | **输出侧 FTZ**：10^-38→0；10^-40→0 | 全一致 | 正常域全 ok | 同 F32 exp10f，设计内 FTZ | 待补 |
| 27 | BF16 `hlog` | BF16(cvt→F32→logf→cvt) | 3/3 例**一致**：logf 预缩放保护有效 | 未触发 | 全一致 | 全 ok | **不受影响**：同 F32 logf | 否 |
| 28 | BF16 `hlog2` | BF16(cvt→F32→log2f→cvt) | 3/3 例**一致** | 未触发 | 全一致 | 全 ok | 同 hlog | 否 |
| 29 | BF16 `hlog10` | BF16(cvt→F32→log10f→cvt) | 3/3 例**一致** | 未触发 | 全一致 | 全 ok | 同 hlog | 否 |
| 30 | BF16 `hsqrt` | BF16(cvt→F32→sqrtf→cvt) | **输入侧 FTZ**：3/3 BF16 subnormal→F32 subnormal→sqrtf FTZ→0 | 未触发 | 全一致 | 正常域全 ok | **输入侧 FTZ 确认**：同 F32 sqrtf，设计内 FTZ | 待补 |
| 31 | BF16 `hrsqrt` | BF16(cvt→F32→1/sqrtf→cvt) | **输入侧 FTZ+真bug**：3/3 BF16 subnormal→sqrtf→0→1/0=Inf vs libm finite | 未触发 | 全一致 | 正常域全 ok | **真bug**：同 F32 rsqrtf，subnormal→FTZ→0→1/√0=Inf | 待补 |
| 32 | BF16 `hrcp` | BF16(=1/bfloat16_t) | **真bug**：2/3 BF16 subnormal→1/sn→Inf vs libm finite | 未触发 | 全一致 | 正常域全 ok | **真bug**：BF16 除法对 subnormal denominator FTZ→0→1/0=Inf | 待补 |
| 33 | `coshf` | F32 组合(Exp) | subnormal→coshf(sn)≈1 一致 | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：FTZ→0→cosh(0)=1，与数学≈1一致 | 否 |
| 34 | `sinhf` | F32 组合(Exp+Taylor) | 3/3 例**一致**：Taylor 分支保留 subnormal | 未触发 | 全一致 | 正常域全 ok | **不受影响**：|x|≤0.1 Taylor 分支用纯 float 算术，subnormal 保留 | 否 |
| 35 | `tanhf` | F32 组合(Exp+Div) | **输入侧 FTZ+精度真bug**：5/5 DIFF：sn→0 vs ≈sn；MN→0 vs ≈MN | 未触发 | 全一致 | 正常域全 ok | **精度真bug**：tanhf=1-2/(expf(2x)+1)，小 x→expf≈1→1-1=0，无 Taylor 退化路径 | 待补 |
| 36 | `acoshf` | F32 组合(Log+Sqrt) | subnormal<1→NaN 一致（数学正确） | 未触发 | 全一致 | FLT_MAX→inf vs libm≈89(**溢出bug**) | **不受 FTZ 影响**；FLT_MAX 溢出为独立 bug（sqrt(x²+1)溢出） | 否（溢出另报） |
| 37 | `asinhf` | F32 组合(Log+Sqrt+Taylor) | 3/3 例**一致**：Taylor 分支保留 subnormal | 未触发 | 全一致 | FLT_MAX→inf vs≈89(**溢出bug**) | **不受 FTZ 影响**；FLT_MAX 溢出为独立 bug | 否（溢出另报） |
| 38 | `hypotf` | F32 组合(Sqrt+Div+FMA) | **真bug**：hypotf(sn,sn)→NaN vs libm finite（DML div 0/0=NaN） | 未触发 | hypotf(0,sn)=|sn| ✓；hypotf(sn,normal)=normal ✓ | 正常域全 ok | **真bug**：两 subnormal→DML b/a FTZ→0/0=NaN→整条链 NaN | 待补 |
| 39 | `rhypotf` | F32 组合(Sqrt+Div+FMA+Div) | **真bug**：rhypotf(sn,sn)=NaN vs libm finite（1/hypotf-NaN=NaN） | 未触发 | 同 hypotf 正确案例 | 正常域全 ok | **真bug**：同 hypotf 根因，1/NaN=NaN | 待补 |
| 40 | `asinf` | F32 组合(Sqrt+Taylor) | 3/3 例**一致**：|x|<0.707 走 Taylor 分支，subnormal 保留 | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：小 |x| Taylor 分支不调 sqrtf | 否 |
| 41 | `acosf` | F32 组合(=PIO2-asinf) | 3/3 例**一致**：同 asinf | 未触发 | 全一致 | 正常域全 ok | **不受影响**：同 asinf | 否 |
| 42 | `atanf` | F32 组合(Div+Taylor) | **真bug**：5/5 DIFF（4 in-FTZ+1 near-MN）：sn→0 vs ≈sn；MN→0.2×MN vs MN | 未触发 | 全一致 | 正常域全 ok | **真bug**：`__internal_sign(clip)` 对 subnormal/near-MN 精度不足→`clip*2^62/(|clip*2^62|+2.168e-19)`≈0.2而非±1→dst×0.2毁结果；最小 sn→dst×3e-8→double-subnormal→FTZ→0 | 待补 |
| 43 | `atan2f` | F32 组合(Div+atanf) | **真bug**：3 unexpected NaN（sn/sn→DML div 0/0=NaN）+2 in-FTZ（sn/normal→0/normal=0→atanf(0)=0） | 未触发 | NaN/±Inf/±0 全一致 | 正常域全 ok | **真bug**：dual-subnormal→y/x FTZ→0/0=NaN；single-subnormal→numerator FTZ→0→atanf(0)=0 vs libm≈y/x | 待补 |
| 44 | `atanhf` | F32 组合(Log+Div) | **精度真bug**：4/4 DIFF（in-FTZ）：sn→0 vs ≈sn；logf((1+sn)/(1-sn))=0 | 未触发 | ±1→inf vs nan（domain handling差） | 正常域全 ok | **精度真bug**：`logf((1+x)/(1-x))/2` 对 subnormal x 无 Taylor 退化→(1±sn)≈1→logf(1)=0；±1 返回 inf vs libm nan 非 FTZ | 待补 |
| 45 | `cbrtf` | F32 组合(位运算+Div) | **真bug**：3/3 DIFF（in-FTZ）：subnormal 位操作缺 implicit 1→x_adjusted 错→Newton 迭代错 | 未触发 | 全一致 | 正常域全 ok | **真bug**：subnormal→exp_bits=0→exponent=-127→k=-42→x_adjusted 仅改 exponent 未加 implicit 1→mantissa 错→Newton 迭代收敛到错值→结果错 | 待补 |
| 46 | `rcbrtf` | F32 组合(位运算+Div) | **真bug**：2/2 DIFF（in-FTZ）：同 cbrtf 根因 | 未触发 | 全一致 | 正常域全 ok | **真bug**：同 cbrtf，subnormal 位操作缺 implicit 1 | 待补 |
| 47 | `sinf` | F32 组合(纯Taylor) | 3/3 例**一致**：纯 Taylor+fmaf，subnormal 保留 | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：纯 Taylor+float 算术，无 DML 原语 | 否 |
| 48 | `cosf` | F32 组合(纯Taylor) | 3/3 例**一致**：同 sinf | 未触发 | 全一致 | 正常域全 ok | **不受影响**：同 sinf | 否 |
| 49 | `tanf` | F32 组合(Taylor+Div) | 3/3 例**一致**：Taylor 保留 subnormal | 未触发 | 全一致 | 正常域全 ok | **不受影响**：小 x 赴 Taylor 路径，subnormal 保留 | 否 |
| 50 | `sinpif` | F32(=sinf(x*PI)) | 3/3 例**一致**：x*PI→normal→sinf(subnormal*PI)≈subnormal*PI | 未触发 | ±1→-8.7e-8 vs 1.22e-16(精度差,非FTZ) | 正常域全 ok | **不受 FTZ 影响**：精度差属 argument reduction 问题，非 subnormal/FTZ | 否 |
| 51 | `cospif` | F32(=cosf(x*PI)) | 3/3 例**一致**：x*PI→normal→cosf(sn*PI)≈1 | 未触发 | 0.5→-4.37e-8 vs 6.12e-17(精度差,非FTZ) | 正常域全 ok | **不受 FTZ 影响**：同 sinpif | 否 |
| 52 | `tanpif` | F32(=tanf(x*PI)) | 3/3 例**一致**：x*PI→normal→tanf(sn*PI)≈sn*PI | 未触发 | ±1→±8.7e-8(精度差,非FTZ) | 正常域全 ok | **不受 FTZ 影响**：同 sinpif | 否 |
| 53 | `norm3df` | F32 组合(Sqrt+Div+FMA) | **真bug**：8/8 unexpected NaN（所有含 subnormal 的输入→NaN vs libm finite） | 未触发 | NaN/±Inf/±0/normal 全一致；(sn,normal,normal)正确 | 正常域全 ok | **真bug**：同 hypotf 根因，m=subnormal→a/m/b/m/c/m DML div 0/0=NaN→fmaf(NaN,...)=NaN→sqrtf(NaN)=NaN→m*NaN=NaN | 待补 |
| 54 | `norm4df` | F32 组合(Sqrt+Div+FMA) | **真bug**：6/6 unexpected NaN（含 subnormal 的输入→NaN vs libm finite） | 未触发 | NaN/±Inf/±0/normal 全一致；(sn,normal,normal,normal)正确 | 正常域全 ok | **真bug**：同 hypotf/norm3df 根因，m=subnormal→DML div 0/0=NaN | 待补 |
| 55 | `rnorm3df` | F32 组合(Sqrt+Div+FMA+Div) | **真bug**：8/8 unexpected NaN（1/norm3df(sn,sn,sn)=1/NaN=NaN） | 未触发 | 同 norm3df 正确案例 | 正常域全 ok | **真bug**：1/NaN=NaN，同 norm3df 根因 | 待补 |
| 56 | `rnorm4df` | F32 组合(Sqrt+Div+FMA+Div) | **真bug**：6/6 unexpected NaN | 未触发 | 同 norm4df 正确案例 | 正常域全 ok | **真bug**：1/NaN=NaN，同 norm4df 根因 | 待补 |
| 57 | `erff` | F32 组合(Exp2+Taylor) | 3/3 例**一致**：subnormal→|x|<1.00296→Taylor分支→fmaf保留 | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：小|x| Taylor 纯 float 算术 | 否 |
| 58 | `erfcf` | F32 组合(Exp2+Div) | 3/3 例**一致**：erfcf(sn)≈1 | **输出侧 FTZ**：erfcf(10)=0 vs libm 1.4e-45 | 全一致 | 正常域全 ok | **输出侧 FTZ 确认**：erfcf 大正输入 → 1-exp2f(-poly) → exp2f 输出 subnormal → FTZ→0 → 1-0=0 | 待补 |
| 59 | `erfcxf` | F32 组合(Exp2+Div) | 3/3 例**一致**：erfcxf(sn)≈1 | 未触发 | 全一致(subnormal) | 正常域全 ok | **不受 subnormal/FTZ 影响**：subnormal→abs_x<10.055→poly+div(正常中间值) | 否 |
| 60 | `erfinvf` | F32 组合(Log2+Rsqrt+Div) | 1/3 例**一致**(最小sn)；2/3 近-MN 精度DIFF(ratio≈0.84，非FTZ) | 未触发 | 全一致(subnormal) | 正常域精度 DIFF(非FTZ) | **不受 FTZ 影响**：subnormal→else分支(pure fmaf)；近-MN 精度偏差系 SIMT 多项式精度 | 否 |
| 60b | `erfcinvf` | F32 组合(Log2+Rsqrt+Div) | **无法定量比较**：libm golden erfcinv 对极小输入(subnormal→erfcinv(sn)≈10)返回 NAN(golden bug)；SIMT 返回 finite≈10(数学正确) | 未触发 | libm golden 有 bug(返回NAN)，SIMT返回finite | SIMT 正常域 ok | **golden 不可用**：SIMT 对 sn 返回 finite≈10(数学≈Φ^{-1}(1-sn)≈√(-2ln(sn))≈10)，与数学预期一致；但 libm golden 有 NAN bug 无法直接比较；实现路径同 erfinvf(log2f+rsqrtf)，subnormal→log2f 保护有效 | 否(无法定量确认) |
| 61 | `tgammaf` | F32 组合(Log+Div+Pow) | **不受影响**：subnormal→overflow→两路均Inf；MN→frexpf+ldexpf保护→correct | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：最小sn→overflow(两路Inf)；MN→__internal_euler_gamma 用 frexpf+ldexpf(1/m)保护 | 否 |
| 62 | `lgammaf` | F32 组合(Log+Taylor) | 5/5 例**一致**：__internal_compute_ln 有 subnormal 预缩放保护 | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：__internal_compute_ln 同 logf 的 x*2^23 预缩放保护 | 否 |
| 63 | `cyl_bessel_i0f` | F32 组合(Exp+Rsqrt+Div) | 5/5 例**一致**：subnormal→else分支(x<9)→pure Taylor→≈1 | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：|x|<9 Taylor 分支无 DML 原语 | 否 |
| 64 | `cyl_bessel_i1f` | F32 组合(Exp+Rsqrt+Div) | 5/5 例**一致**：同 i0f | 未触发 | 全一致 | 正常域精度 DIFF(非FTZ) | **不受 subnormal/FTZ 影响**：同 i0f；正常域 DIFF 系 SIMT 精度 | 否 |
| 65 | `j0f` | F32 组合(Trig+Div) | 5/5 例**一致**：subnormal→else(x≤8)→Taylor→≈1 | 未触发 | 全一致(subnormal) | 正常域精度 DIFF(非FTZ) | **不受 subnormal/FTZ 影响**：小 x Taylor 分支；正常域 DIFF 系 SIMT 精度 | 否 |
| 66 | `j1f` | F32 组合(Trig+Div) | 5/5 例**一致**：subnormal→Taylor→≈0或≈x/2 | 未触发 | 全一致(subnormal) | 正常域精度 DIFF(非FTZ) | **不受 subnormal/FTZ 影响**：同 j0f | 否 |
| 67 | `y1f` | F32 组合(Log+Div+Trig) | 2/2 例**一致**(最小sn)→-Inf(overflow，数学正确) | 未触发 | y1f(MN)=finite ✓ | 正常域精度 DIFF | **不受 FTZ 影响**：最小sn→Y_1(sn)超 FLT_MAX→两路均-Inf；MN→finite ✓ | 否 |
| 68 | `normcdff` | F32 组合(Erfcf) | 5/5 例**一致**：normcdff(sn)=0.5(数学正确) | 未触发 | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：经 erfcf(x/√2)，subnormal→x/√2→subnormal→erfcf≈1→0.5*(1+erf(≈0))≈0.5 | 否 |
| 69 | `scalbnf` | F32 组合(Pow+Div) | 1/1 DIFF(in-FTZ)：scalbnf(sn,-10)→subnormal 结果 FTZ→0 vs libm finite | 未触发 | 全一致 | 正常域全 ok | **输出侧 FTZ 确认**：scalbnf(sn,-10)=sn*2^-10→结果 subnormal→FTZ→0；实现含 `x/__powf(2,n)` DML 路径 | 待补 |
| 70 | `__hdiv`(F16) | F16 直接(Div) | 0 DIFF：F16 subnormal 经 DML 后结果与 libm F16 一致（F16 值域极窄，sn→≈0→div 行为与数学一致） | 未触发（F16 值域窄，商一般不落 subnormal） | 全一致 | 正常域全 ok | **不受 subnormal/FTZ 影响**：F16 值域限制(MIN_NORMAL=2^-14)使 subnormal 输入经 DML 后与数学预期一致 | 否 |
| 71 | `__hdiv`(BF16) | BF16 直接(Div) | **真bug**：sn/sn→DML 0/0=NaN vs libm 1；1/sn_max→Inf vs libm finite(8.57e37)；MN/sn_min→Inf vs 128 | **输出侧 FTZ**：MN/2→0 vs libm 5.88e-39；MN/4→0 vs 2.94e-39；1/BF16_MAX→0 vs 2.94e-39 | **真bug**：sn/sn→unexpected NaN(3/3 例)；1/sn_max→unexpected Inf | 正常域全 ok | **真bug+双侧 FTZ**：BF16 经 F32 upcast 路径，DML 除法双侧 FTZ；dual-subnormal→0/0=NaN；与 F32 fdividef/BF16 hrcp 同根因 | 待补 |
| 72 | `jnf` | F32 组合(Div+Exp+Log+Sqrt) | 0 DIFF(subnormal)：subnormal x→case3(logf/expf 保护)→与 libm 一致 | **输出侧 FTZ**：jnf(85,22.04)→0 vs libm 3.31e-41(subnormal)；数学结果落入 subnormal 被 FTZ→0 | NaN/±Inf/±0/n<0 全一致 | n=5/10/20 ok；n=85 x=22 **输出 FTZ**(0 vs 3.31e-41)；n=85 x≥50 ok | **输出侧 FTZ 确认**：jnf 大 n 小 x 的数学结果落入 subnormal → FTZ→0；subnormal 输入经 case3 logf 保护不受影响；设计内 FTZ | 待补 |
| 73 | `scalblnf` | F32 组合(=scalbnf(x,cast_int32)) | 1 DIFF(in-FTZ)：scalblnf(sn_mid,-10)→0 vs libm 5.74e-42 | 未触发 | 全一致 | 正常域全 ok | **与 scalbnf 同根因**：scalblnf→scalbnf→DML __powf 路径；sn*2^-10→subnormal→FTZ→0；设计内 FTZ | 待补 |
| 74 | `normcdfinvf` | F32 组合(Div+Exp+Log+Sqrt) | 4 DIFF(in-FTZ)：subnormal→算法路径正常(logf 保护)，但 SIMT 多项式对极小输入精度不足 | 未触发 | 3 unexpected：+Inf→NaN vs Inf；-Inf→NaN vs -Inf；-1→NaN vs -Inf | 正常域全 ok(0.01→-2.326✓; 0.5→0✓; 0.999→3.090✓) | **SIMT 精度偏差(非 FTZ)**：subnormal 输入路径正常(logf 预缩放有效)，但多项式对 log2f(item)≈-149 精度不足；特殊值域处理差异非 FTZ | 否(非 FTZ 主导) |
| 75 | `normf` | F32 组合(Div+Sqrt) | **真bug**：normf(2,[sn,sn])→NaN vs libm finite；dual-subnormal→DML 0/0=NaN(同 hypotf 根因) | 未触发 | 全一致 | 正常域全 ok；normf(2,[sn,normal])→1 ✓ | **真bug**：dual-subnormal→m=subnormal→a/m DML div 0/0=NaN→fma(NaN,..)=NaN→sqrtf(NaN)=NaN→m*NaN=NaN | 待补 |
| 76 | `rnormf` | F32 组合(Div) | **真bug**：rnormf(2,[sn,sn])→NaN vs libm finite(1/NaN=NaN，同 normf 根因) | 未触发 | 全一致 | 正常域全 ok | **真bug**：1/normf(NaN)=NaN，同 normf/hypotf 根因 | 待补 |

> sqrtf 实测明细：`0x00000001 / 0x00000010 / 0x007FFFFF / 0x00400000 / 1e-40`
> 五个 subnormal 输入，设备一律返回 `+0`，而非 FTZ libm 返回对应小正数。
> `±0`、最小规格化数 `0x00800000`、NaN/±Inf/负输入均与 libm 一致。
> 即：sqrtf 的偏差**完全来自输入侧 FTZ**，未观察到输出 FTZ 或设计外 NaN/Inf。

> y0f 实测明细：`0x00000001 / 0x00000010 / 0x007FFFFF / 0x00400000 / 1e-40`
> 五个 subnormal 输入，设备输出与 libm golden 一致（-65.82/-64.06/-56.12/-55.67/-58.71）。
> y0f 小 x 支内含 `logf(x)`，但 logf 对 subnormal 输入已有预缩放保护（`x*2^23` 后再 `__logf`），
> 使得 subnormal 输入不被 FTZ 破坏。预筛曾误判为 IMPACT（假设 logf(subnormal) → FTZ → -Inf），
> 真机实测修正为 NO-IMPACT。x>1e13 时实现 return 0（截断设计，非 FTZ 问题，数学值≈4e-08）。

> ynf 实测明细：subnormal 输入(n=0~10, x=subnormal)全一致——数学值 Y_n(subnormal) 已超 FLT_MAX，
> 两路均返回 -Inf，overflow 掩盖 FTZ 误差。**真 bug 出在正常域**：ynf(85,22.041975) 设备返回
> -Inf，libm 返回 -1.17e+38（finite）。根因：递推 case3 的缩放因子 `scale_factor` 经 84 步
> 累积后绝对值落入 subnormal 区（-8.55e-39 < F32_MIN_NORMAL），最终 `value/scale_factor`
> 的 DML 除法对 subnormal denominator FTZ→0，导致 `1/(-0) = -Inf`。此行为属于**实现缺陷**
> （非 DML 设计内 FTZ）：数学结果 -1.17e38 为正常 finite 值，不应产生 -Inf。触发条件：
> n 较大（约≥70）且 x 在 n\*0.25~n\*10 范围时 scale_factor 易变 subnormal。

后续接口（Exp/Log/Div/Rsqrt/Pow 及组合函数）按 4.6 节优先级依此模板逐个补测与改资料。

> exp2f 实测明细：subnormal 输入(0x00000001/0x007FFFFF/1e-40)→设备返回 1，
> 与 libm 一致（数学值 ≈1）。输出侧 FTZ：exp2f(-126)→0 vs libm FLT_MIN，
> exp2f(-149)→0 vs libm 1.401e-45。前者 FLT_MIN 为正常域边界值但经 expf 路径
> 计算时中间结果落入 subnormal 区被 FTZ；后者为标准输出侧 FTZ。均属设计内 FTZ。

> exp10f 实测明细：subnormal 输入同 exp2f 一致(返回 1)。输出侧 FTZ：
> exp10f(-38)→0 vs libm 9.999e-39，exp10f(-40)→0 vs libm 9.999e-41。
> 路径经 expf DML 原语，同 expf 设计内 FTZ 机制。

> expm1f 实测明细：subnormal 输入→0 vs libm nonzero（3 例 in-FTZ）。
> 根因：1+subnormal=1.0（float 加法截断），expf(1.0)=1.0，expm1=0。
> **精度真bug**：expm1f(1e-10)→0 vs libm 1.00000001e-10。
> SIMT expm1f 实现疑似直接 `expf(x)-1` 而无 small-x Taylor 退化路径，
> 导致正常域小 x 也因 float 加法截断返回 0。libm expm1f 有 Taylor 退化
> 保证对小 |x| 返回 ≈x。

> log2f 实测明细：0 DIFF。logf 预缩放(x*2^23)保护对 subnormal 输入完全有效，
> 除以常量 logf(2) 不引入 FTZ。不受 subnormal/FTZ 影响。

> log10f 实测明细：0 DIFF。同 log2f，logf 预缩放保护+常量除法不引入 FTZ。
> 不受 subnormal/FTZ 影响。

> log1pf 实测明细：subnormal 输入→0 vs libm nonzero（3 例 in-FTZ）。
> 根因同 expm1f：1+subnormal=1.0→logf(1.0)=0。
> **精度真bug**：log1pf(1e-10)→0 vs libm 1e-10；log1pf(1e-8)→0 vs libm 9.999e-9。
> SIMT log1pf 实现疑似直接 `logf(1+x)` 而无 small-x Taylor 退化路径，
> 1+x 在 float 中对小 x 截断为 1.0→logf(1)=0。libm log1pf 有 Taylor 退化
> 保证对小 |x| 返回 ≈x。

> F16 3.1 实测明细：9 个 F16 直接原语（hexp/hexp2/hexp10/hlog/hlog2/hlog10/hsqrt/hrsqrt/hrcp）
> 全部在 dav-3510 上实测。**核心发现**：F16 的 subnormal 输入几乎不触发 FTZ 问题，
> 因为 F16 值域极窄（MIN_NORMAL=2^-14，MIN_SUBNORMAL=2^-24），DML __expf/__logf/__sqrtf
> 对 half subnormal 的处理结果与 libm golden（经 F16 舍入后）一致。
> hexp/hlog/hsqrt 直接调用 DML __expf/__logf/__sqrtf(half)，half subnormal 被 FTZ→±0 后
> exp(±0)=1/log(-0)=-Inf/sqrt(±0)=±0，这些结果与 F16 精度下的数学正确值一致。
> hsqrt(half_subnormal)→F16 normal（sqrt 放大了约 2^12 倍），无 FTZ。
> hrsqrt/hrcp 对 subnormal 输入的 overflow（1/sn=Inf）是数学正确行为（F16 值域溢出）。
> 仅 hexp2 有 1 个输出侧差异（hexp2(-24)=0 vs 0.25），因 F16 无法表示 2^-24 的 subnormal 输出。

> BF16 3.1 实测明细：9 个 BF16 直接原语全部实测。BF16 经 cvt_float→F32→DML→cvt_bfloat16 路径，
> BF16 subnormal 映射为 F32 subnormal（相同指数范围），因此 BF16 的 subnormal/FTZ 行为
> **与 F32 对应接口完全一致**：
> - hexp/hexp2/hexp10 的输出侧 FTZ 与 F32 expf/exp2f/exp10f 同机制、同阈值
> - hlog/hlog2/hlog10 不受影响（logf 预缩放保护）
> - hsqrt 输入侧 FTZ 与 F32 sqrtf 同机制
> - hrsqrt 真bug（unexpected Inf）与 F32 rsqrtf 同根因
> - hrcp 真bug（unexpected Inf）与 F32 fdividef 同根因

> P1 组合接口实测明细（coshf/sinhf/tanhf/acoshf/asinhf/hypotf/rhypotf）：
> **核心发现**：DML FTZ 仅影响 __expf/__logf/__sqrtf/__powf/除法 DML 原语内部，
> 不影响一般 float 算术（加减乘、比较、fabsf/fmaxf/fminf 等位运算/比较函数）。
> sinhf/asinhf 的 Taylor 分支（|x|≤0.1）使用纯 float 算术，subnormal 输入完整保留，
> 与 libm golden 一致。coshf 的 subnormal 输入→coshf(sn)≈1 也与 libm 一致。
> **tanhf 真bug**：tanhf(subnormal)=0 vs libm≈sn，tanhf(MIN_NORMAL)=0 vs libm≈MN；
> 根因：tanhf=1-2/(expf(2x)+1)，2*sn 或 2*MN 经 __expf DML 后≈1→1-2/2=0，
> 而 libm tanhf 对小 |x| 有退化路径返回≈x。无 Taylor 退化路径→精度真bug。
> **asinhf 溢出**：asinhf(FLT_MAX)→inf vs libm≈89.4（sqrt(x²+1)溢出→Inf→logf(Inf)=inf），
> 非 FTZ 问题，是中间值溢出 bug。
> **hypotf 真bug**：hypotf(sn,sn)→NaN vs libm finite！根因：b/a 中两 subnormal 经
> DML 除法 FTZ→0/0=NaN→fmaf(NaN,...)=NaN→sqrtf(NaN)=NaN→a*NaN=NaN。
> 但 hypotf(0,sn)→|sn|（正确），hypotf(sn,normal)→normal（正确，因 b/a=FTZ→0→sqrt(1)=1→max值）。
> **rhypotf 真bug**：1/hypotf(sn,sn)=1/NaN=NaN vs libm finite，同根因。

> P3 接口实测明细（erff/erfcf/erfcxf/erfinvf/tgammaf/lgammaf/cyl_bessel_i0f/i1f/j0f/j1f/y1f/normcdff/scalbnf）：
> **核心发现**：多数 P3 接口对 subnormal 输入不受 FTZ 影响，因为 subnormal 落入"小输入分支"（Taylor/fmaf），不含 DML 原语。
> **erff**：0 DIFF。|x|<1.00296 Taylor 纯 fmaf，subnormal 保留。
> **erfcf**：erfcf(sn)≈1 OK；1 DIFF 为输出侧 FTZ：erfcf(10)=0 vs libm 1.4e-45（exp2f 大负→subnormal 输出 FTZ→0→1-0=0），同 exp2f/expf 设计内 FTZ。
> **erfcxf**：0 DIFF(subnormal)；erfcxf(+Inf)=0 vs -nan 1 DIFF（特殊值处理差异，非 FTZ）。
> **erfinvf**：subnormal 输入 0 DIFF（最小sn 保留）；近-MN 有精度 DIFF(ratio≈0.84)，系 SIMT 多项式精度而非 FTZ。
> **erfcinvf（序号60b）**：golden 函数 libm erfcinv 对极小输入（subnormal→erfcinv(sn)≈10）返回 NAN（golden bug），
> SIMT 实际对 sn 返回 finite≈10（数学正确：erfcinv(sn) ≈ Φ^{-1}(1-sn) ≈ √(-2ln(sn)) ≈ 10）。
> 实现路径同 erfinvf：subnormal→negLog=ln(sn) → log2f 有预缩放保护 → rsqrtf(negLog) 正常域 → 结果 ≈ 10 finite。
> 无法定量确认一致性（golden NAN bug），但 SIMT 输出与数学预期一致。
> **tgammaf**：0 DIFF。sn→Inf(overflow，两路一致)；MN→8.5e37(正确，frexpf+ldexpf 保护生效)。
> **lgammaf**：0 DIFF。__internal_compute_ln 同 logf 的 x*2^23 预缩放保护有效，subnormal 输入完全正确。
> **cyl_bessel_i0f/i1f**：0 DIFF(subnormal)。|x|<9 Taylor 分支，sn→≈1/≈x/2。
> **j0f/j1f**：0 DIFF(subnormal)。小 x Taylor，sn→≈1/≈0。
> **y1f**：sn→-Inf(overflow，数学 Y_1(sn)>FLT_MAX→两路均 -Inf)；MN→-5.4e37 ✓(finite)。不受 FTZ 影响。
> **normcdff**：0 DIFF。经 erfcf(x/√2)→erfcf(sn/1.414≈sn)≈1→(1+erf(≈0))/2≈0.5，数学正确。
> **scalbnf**：1 DIFF：scalbnf(sn,-10)→sn*2^-10→subnormal 结果 FTZ→0 vs libm finite。实现含 DML __powf 路径。

> P2+三角接口实测明细（asinf/acosf/atanf/atan2f/atanhf/cbrtf/rcbrtf/sinf/cosf/tanf/sinpif/cospif/tanpif）：
> **asinf/acosf**：0 DIFF。|x|<0.707 走 Taylor 分支（不调 sqrtf），subnormal 完整保留。
> **atanf 真bug**：5 DIFF（4 in-FTZ+1 near-MN）。根因：`__internal_sign(clip)` 对 subnormal/near-MN 精度不足。
> `clip * ATAN_FP32_MAX(2^62)` 对 subnormal→小正常数（≈5.4e-20 for MN），
> `fabsf(result) + ATAN_FP32_MIN(2.168e-19)` → ATAN_FP32_MIN 主导 denominator → ratio ≈ 0.2 而非 ±1.0。
> 最终 `dst * 0.2` 毁坏 dst=subnormal 值（对最小 sn，dst*3e-8→double-subnormal→FTZ→0）。
> **atan2f 真bug**：5 DIFF。dual-subnormal→y/x DML div 0/0=NaN→atanf(NaN)=NaN；
> single-subnormal numerator→FTZ→0/normal=0→atanf(0)=0 vs libm≈y/x。
> float 比较保留 subnormal（x==0 对 sn 为 false），但 DML 除法 FTZes operands。
> **atanhf 精度真bug**：7 DIFF（4 in-FTZ）。`logf((1+x)/(1-x))/2` 对 subnormal→(1±sn)≈1→logf(1)=0。
> 无 Taylor 退化路径，与 tanhf/expm1f/log1pf 同类精度真bug。±1→inf vs nan 为 domain 处理差异，非 FTZ。
> **cbrtf/rcbrtf 真bug**：subnormal 输入→位操作缺 implicit 1 bit→x_adjusted 错误→Newton 迭代收敛到错值。
> 对 cbrtf(0x00000001)：mantissa=0x000001，exp_adjusted=126→float = (1+1/2^23)*2^-1≈0.5（应≈2^-23）→cbrt(0.5)*2^-42≈1.8e-13 vs golden 1.12e-15。
> **sinf/cosf/tanf**：0 DIFF。纯 Taylor+float 算术，无 DML 原语，subnormal 保留。
> **sinpif/cospif/tanpif**：subnormal 输入 0 DIFF（x*PI→normal→三角函数保留）。
> 正常域有精度 DIFF（如 sinpif(1)→-8.7e-8 vs 1.22e-16）属 argument reduction 精度问题，非 subnormal/FTZ。

> 补测接口实测明细（序号 70~76）：

> **F16 __hdiv（序号70）**：0 DIFF。F16 值域极窄（MIN_NORMAL=2^-14），subnormal 输入经 DML 后≈±0，
> div(≈0, normal)≈0（与数学一致），div(normal, ≈0)→Inf（与数学一致），
> div(≈0, ≈0)=NaN（与 0/0 IEEE 一致）。与 F32 fdividef 不同，F16 无额外的 FTZ 问题。

> **BF16 __hdiv（序号71）**：13 DIFF。BF16 经 cvt→F32→DML 除法→cvt_bfloat16 路径，
> BF16 subnormal 映射为 F32 subnormal（相同指数范围），因此 BF16 的 subnormal/FTZ 行为
> **与 F32 fdividef 完全一致**：
> - 输入侧 FTZ：sn/1→0 vs libm nonzero（10 例）
> - 输出侧 FTZ：MN/2→0 vs 5.88e-39（3 例）
> - 真 bug：sn/sn→DML 0/0=NaN vs 1；1/sn_max→Inf vs 8.57e37；MN/sn_min→Inf vs 128（5 unexpected NaN/Inf）

> **jnf（序号72）**：1 DIFF(out-FTZ)。subnormal x(0x00000001~0x007FFFFF) 各 n(0~85) 输入全一致：
> jnf(0,sn)=j0f(sn)≈1；jnf(1,sn)=j1f(sn)≈0；jnf(2~85,sn)→case3(logf/expf 保护)→0。
> jnf(85, 22.041975)→0 vs libm 3.31e-41(subnormal)：数学结果 J_85(22) ≈ 3e-41，
> 落入 subnormal 区被 FTZ→0。与 expf 输出侧 FTZ 同机制，属设计内 FTZ。

> **scalblnf（序号73）**：1 DIFF(in-FTZ)。scalblnf(sn_mid=0x00400000, -10)→0 vs libm 5.74e-42。
> scalblnf 实现 = scalbnf(x, (int32_t)n)，与 scalbnf 完全同路径同根因。
> sn*2^-10 结果落入 subnormal → FTZ→0。设计内 FTZ。

> **normcdfinvf（序号74）**：13 DIFF，但**非 FTZ 主导**。subnormal 输入(0x00000001~0x00400000)→
> 算法路径正常：float 加法 x+x 保留 subnormal（不 FTZ），log2f(subnormal) 有保护，
> rsqrtf 正常域运算正确。但 SIMT 多项式对极小输入(log2f(item)≈-149)精度不足，
> 导致结果偏离数学 Φ^{-1}(sn)≈-20~-13 约 6~7 个单位（-14.12 vs math ≈-20）。
> 特殊值差异：+Inf→NaN vs Inf 等，属 domain handling 差异而非 FTZ。
> 正常域精度正确：0.01→-2.326✓、0.5→0✓、0.999→3.090✓。

> **normf/rnormf 真 bug（序号75/76）**：normf(2,[sn,sn])→NaN vs libm 1.4e-45。
> 根因与 hypotf/norm3df/norm4df 完全相同：m=max(|a[i]|)=subnormal，
> a/m 中 DML 除法 sn/sn → FTZ→0/0=NaN → fma(NaN,...)=NaN → sqrtf(NaN)=NaN → m*NaN=NaN。
> normf(2,[sn,normal])→1 ✓（m=normal→a/m=sn/normal→FTZ→0→sqrt(0²+1²)=1→m*1=normal）。
> rnormf = 1/normf → 1/NaN=NaN。
