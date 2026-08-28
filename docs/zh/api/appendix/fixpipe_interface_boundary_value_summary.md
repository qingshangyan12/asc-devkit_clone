# 矩阵计算搬出相关接口边界值汇总

本节汇总介绍了L0C Buffer->GM/L0C Buffer->L1 Buffer/L0C Buffer->UB搬运接口在L0C Buffer中存在inf/-inf/nan等边界值时的输出结果，涉及接口如下：

**表1**  矩阵计算搬出相关接口说明

| 搬运路径 | 基础API | C API |
|---|---|---|
| L0C Buffer->GM | [Fixpipe（L0C到GM）](../SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md) | [asc_copy_l0c2gm](../SIMD-API/c_api/cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md) |
| L0C Buffer->L1 Buffer | [Fixpipe（L0C到L1）](../SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToL1.md) | [asc_copy_l0c2l1](../SIMD-API/c_api/cube_data_move/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md) |
| L0C Buffer->UB | [Fixpipe（L0C到UB）](../SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md) | [asc_copy_l0c2ub](../SIMD-API/c_api/cube_data_move/asc_copy_l0c2ub.md) |

以下内容针对如下型号生效：

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT
<!-- end id1 -->

## 概述

三个搬运指令均将L0C Buffer中的矩阵计算结果搬运到目标Buffer（GM、L1 Buffer、UB），在搬运过程中支持随路量化，将L0C Buffer中的float或int32_t数据转换为目标数据类型输出。随路量化的inf/nan处理行为在三个搬运指令间完全一致，通过[asc_set_ctrl](../SIMD-API/c_api/sys_var/asc_set_ctrl.md)设置CTRL[48]比特位控制饱和模式：

- **饱和模式（CTRL[48]=1'b0）**：inf输出会被饱和为±MAX，nan输出会被饱和为0。
- **非饱和模式（CTRL[48]=1'b1）**：inf/nan保持原输出。

> [!NOTE]说明
>
>- CTRL[48]仅支持如下数据类型的精度转换：half、bfloat16_t、fp8_e4m3fn_t、hifloat8_t。对于float数据类型，CTRL[48]不生效，inf/nan始终保持原样输出。
>
>- 整数类型（int8_t、uint8_t、int4b_t）只有饱和模式，无论CTRL[48]如何设置，均按饱和模式处理。
>
>- 对于fp8_e4m3fn_t类型，CTRL[50]比特位在饱和模式下进一步控制nan的输出：CTRL[50]=1'b0（默认）时nan被转换为0；CTRL[50]=1'b1时nan保持不变。

### 数据搬运路径差异

三个搬运指令的随路量化inf/nan处理行为完全一致，差异仅在于搬运目标及验证路径：

- **L0C Buffer->GM**：直接将L0C Buffer数据搬运到GM，无需中转路径，非饱和模式下inf/nan能完整传递到GM输出，无验证路径限制。
- **L0C Buffer->L1 Buffer**：将L0C Buffer数据搬运到L1 Buffer，由于L1 Buffer数据无法直接搬运到GM，样例通过二次矩阵计算（量化结果×单位矩阵D）将数据从L1 Buffer经L0A->L0C Buffer搬运到GM，最终输出为int32_t或float。当量化结果中包含inf时，二次矩阵计算中inf与单位矩阵的非对角线零元素相乘会产生nan，累加后整行变为nan。此为验证路径的限制，不影响该搬运指令本身对inf/nan的处理行为。golden数据已适配此行为：非饱和模式下inf/-inf/nan位置的golden值设为nan，使测试能端到端通过。
- **L0C Buffer->UB**：将L0C Buffer数据搬运到UB，再从UB搬运到GM。AIC负责L0C Buffer到UB的搬运，AIV负责UB到GM的搬运，通过跨核同步协调。此路径无二次矩阵计算，不存在inf×0=nan问题，非饱和模式下inf/nan能完整传递到GM输出。

## float类型L0C Buffer搬运至各输出类型

L0C Buffer中存储的float类型数据来自矩阵乘计算的结果，当输入矩阵A中包含inf/-inf/nan时，L0C Buffer中对应行会出现inf/-inf/nan。

**表2**  float类型L0C Buffer中存在inf/-inf/nan时，各输出类型的计算结果说明

| 输出数据类型 | 随路量化模式 | L0C Buffer中的元素取值 | 饱和模式输出元素取值       | 非饱和模式输出元素取值 |
| --- | --- | --- |------------------|-------------|
| half | QF322F16_PRE / VQF322F16_PRE | +inf | 65504.0（MAX）     | +inf        |
| half | QF322F16_PRE / VQF322F16_PRE | -inf | -65504.0（MIN）    | -inf        |
| half | QF322F16_PRE / VQF322F16_PRE | nan | 0.0              | nan         |
| half | F322F16（Cast） | +inf | 65504.0（MAX）     | +inf        |
| half | F322F16（Cast） | -inf | -65504.0（MIN）    | -inf        |
| half | F322F16（Cast） | nan | 0.0              | nan         |
| bfloat16_t | QF322BF16_PRE / VQF322BF16_PRE | +inf | 0x7F7F（MAX）      | 0x7F80（+inf） |
| bfloat16_t | QF322BF16_PRE / VQF322BF16_PRE | -inf | 0xFF7F（MIN）      | 0xFF80（-inf） |
| bfloat16_t | QF322BF16_PRE / VQF322BF16_PRE | nan | 0x0000（0）        | 0x7FFF（nan） |
| bfloat16_t | F322BF16（Cast） | +inf | 0x7F7F（MAX）      | 0x7F80（+inf） |
| bfloat16_t | F322BF16（Cast） | -inf | 0xFF7F（MIN）      | 0xFF80（-inf） |
| bfloat16_t | F322BF16（Cast） | nan | 0x0000（0）        | 0x7FFF（nan） |
| float | QF322F32_PRE / VQF322F32_PRE | +inf | +inf             | +inf        |
| float | QF322F32_PRE / VQF322F32_PRE | -inf | -inf             | -inf        |
| float | QF322F32_PRE / VQF322F32_PRE | nan | nan              | nan         |
| fp8_e4m3fn_t | QF322FP8_PRE / VQF322FP8_PRE | +inf | 0x7E（448.0，MAX）  | 0x7F（nan）   |
| fp8_e4m3fn_t | QF322FP8_PRE / VQF322FP8_PRE | -inf | 0xFE（-448.0，MIN） | 0x7F（nan）   |
| fp8_e4m3fn_t | QF322FP8_PRE / VQF322FP8_PRE | nan | 0x00（0）          | 0x7F（nan）   |
| hifloat8_t | QF322HIF8_PRE / VQF322HIF8_PRE | +inf | 111（HiF8+inf）    | 111（HiF8+inf） |
| hifloat8_t | QF322HIF8_PRE / VQF322HIF8_PRE | -inf | 239（HiF8-inf）    | 239（HiF8-inf） |
| hifloat8_t | QF322HIF8_PRE / VQF322HIF8_PRE | nan | 0                | 128（HiF8NAN） |
| hifloat8_t | QF322HIF8_PRE_HYBRID / VQF322HIF8_PRE_HYBRID | +inf | 111（HiF8+inf）    | 111（HiF8+inf） |
| hifloat8_t | QF322HIF8_PRE_HYBRID / VQF322HIF8_PRE_HYBRID | -inf | 239（HiF8-inf）    | 239（HiF8-inf） |
| hifloat8_t | QF322HIF8_PRE_HYBRID / VQF322HIF8_PRE_HYBRID | nan | 0                | 128（HiF8NAN） |
| int8_t | QF322B8_PRE / VQF322B8_PRE | +inf | 127（MAX）         | 127（MAX）    |
| int8_t | QF322B8_PRE / VQF322B8_PRE | -inf | -128（MIN）        | -128（MIN）   |
| int8_t | QF322B8_PRE / VQF322B8_PRE | nan | 0                | 0           |
| uint8_t | QF322B8_PRE / VQF322B8_PRE | +inf | 255（MAX）         | 255（MAX）    |
| uint8_t | QF322B8_PRE / VQF322B8_PRE | -inf | 0（MIN）           | 0（MIN）      |
| uint8_t | QF322B8_PRE / VQF322B8_PRE | nan | 0                | 0           |
| int4b_t | QF322S4_PRE / VQF322S4_PRE | +inf | 7（MAX）           | 7（MAX）      |
| int4b_t | QF322S4_PRE / VQF322S4_PRE | -inf | -8（MIN）          | -8（MIN）     |
| int4b_t | QF322S4_PRE / VQF322S4_PRE | nan | 0                | 0           |

注：

- float类型的inf/nan在饱和模式和非饱和模式下均保持原样输出，因为CTRL[48]不支持float数据类型。
- fp8_e4m3fn_t类型没有inf的编码格式。非饱和模式下，若结果绝对值为inf或大于fp8_e4m3fn_t的最大规格化值（448），输出为nan（0x7F）。
- hifloat8_t类型中，111（0b01101111）为HiF8+inf编码，239（0b11101111）为HiF8-inf编码，128（0b10000000）为HiF8NAN编码。饱和模式下，inf保持为对应的inf编码，nan被饱和为0。
- hifloat8_t的溢出判断阈值为1.25×2^15=40960，当量化后的值绝对值大于等于40960时输出inf编码（111/239），而非饱和为240。
- 整数类型只有饱和模式，饱和模式和非饱和模式下的输出结果一致。
- VQF322HIF8_PRE使用Half to Away舍入模式，VQF322HIF8_PRE_HYBRID使用Hybrid舍入模式（|exp|<4时为Half to Away，|exp|≥4时为SSR随机舍入）。两种模式对inf/nan的处理一致。
- 同一输出类型的scalar模式（如QF322F16_PRE）与tensor模式（如VQF322F16_PRE）使用相同的类型转换算法，inf/nan边界值处理行为完全一致，差异仅在于量化参数的传递方式（单个标量vs每列张量），因此表中将scalar模式与tensor模式合并列出。

## int32_t类型L0C Buffer搬运至各输出类型

L0C Buffer中存储的int32_t类型数据来自矩阵乘计算的结果，不会产生inf/nan，但量化后可能产生溢出。

**表3**  int32_t类型L0C Buffer量化溢出时的计算结果说明

| 输出数据类型 | 随路量化模式 | L0C Buffer中的元素取值 | 输出元素取值 |
| --- | --- | --- | --- |
| half | DEQF16 / VDEQF16 | 量化后超出half表示范围 | 65504.0（MAX） / -65504.0（MIN） |
| bfloat16_t | QS322BF16_PRE / VQS322BF16_PRE | 量化后超出bfloat16表示范围 | 0x7F7F（MAX） / 0xFF7F（MIN） |
| int8_t | REQ8 / VREQ8 | 量化后大于127 | 127（MAX） |
| int8_t | REQ8 / VREQ8 | 量化后小于-128 | -128（MIN） |
| uint8_t | REQ8 / VREQ8 | 量化后大于255 | 255（MAX） |
| uint8_t | REQ8 / VREQ8 | 量化后小于0 | 0（MIN） |
| int4b_t | REQ4 / VREQ4 | 量化后大于7 | 7（MAX） |
| int4b_t | REQ4 / VREQ4 | 量化后小于-8 | -8（MIN） |

注：整数类型只有饱和模式，溢出时饱和为目标类型的最值。half和bfloat16_t在饱和模式和非饱和模式下均对溢出值进行饱和处理。

## NoQuant（无量化）场景

NoQuant模式不做类型转换和量化，L0C Buffer中的float数据原样搬运到目标Buffer。由于CTRL[48]不支持float数据类型，饱和模式和非饱和模式下inf/nan均保持原样输出。NoQuant场景还覆盖了NZ2NZ、NZ2ND、双目标按M维度拆分、双目标按N维度拆分等输出格式，验证inf/-inf/nan在各种格式转换和双目标拆分场景下均能正确传递。

**表4**  NoQuant模式下float类型L0C Buffer中存在inf/-inf/nan时的输出结果

| 场景 | 输出格式 | 双目标 | L0C Buffer中的元素取值 | 输出元素取值 | 说明 |
| --- | --- | --- | --- | --- | --- |
| 1 | Nz | 否 | +inf/-inf/nan | +inf/-inf/nan | 原样保留 |
| 2 | ND | 否 | +inf/-inf/nan | +inf/-inf/nan | 原样保留 |
| 3 | ND | 是（按M拆分） | +inf/-inf/nan | +inf/-inf/nan | 双目标均正确传递 |
| 4 | ND | 是（按N拆分） | +inf/-inf/nan | +inf/-inf/nan | 双目标均正确传递 |

注：NoQuant场景的输出为float类型，CTRL[48]不生效，饱和模式和非饱和模式结果一致。场景3按M维度拆分时M必须为2的倍数，场景4按N维度拆分时N必须为32的倍数。
