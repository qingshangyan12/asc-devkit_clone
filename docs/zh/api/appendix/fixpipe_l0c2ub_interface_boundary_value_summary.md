# Fixpipe(L0C→UB)接口边界值汇总<a name="ZH-CN_TOPIC_0000000000000002"></a>

本节汇总介绍了Fixpipe L0C→UB搬运接口（C API [asc_copy_l0c2ub](../SIMD-API/c_api/cube_data_move/asc_copy_l0c2ub.md)）在L0C Buffer中存在inf/-inf/nan等边界值时的输出结果，以下内容针对如下型号生效：

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT
<!-- end id1 -->

## 概述

Fixpipe L0C→UB接口将L0C中的矩阵计算结果搬运到UB（Unified Buffer），在搬运过程中支持随路量化，将L0C中的float或int32\_t数据转换为目标数据类型输出。随后通过`asc_copy_ub2gm`将数据从UB搬运到GM。随路量化的inf/nan处理与Fixpipe L0C→GM接口完全一致，通过CTRL寄存器的CTRL\[48\]比特位控制饱和模式：

- **饱和模式（CTRL\[48\]=1'b0）**：inf输出会被饱和为±MAX，nan输出会被饱和为0。
- **非饱和模式（CTRL\[48\]=1'b1）**：inf/nan保持原输出。

可通过[asc_set_ctrl](../SIMD-API/c_api/sys_var/asc_set_ctrl.md)（C API）或[SetCtrlSpr](../SIMD-API/basic_api/special_register_access/SetCtrlSpr_ISASI.md)（高阶API）设置CTRL\[48\]。

> [!NOTE]说明
>
>- CTRL\[48\]仅支持如下数据类型的精度转换：half、bfloat16\_t、fp8\_e4m3fn\_t、hifloat8\_t。对于float数据类型，CTRL\[48\]不生效，inf/nan始终保持原样输出。
>
>- 整数类型（int8\_t、uint8\_t、int4b\_t）只有饱和模式，无论CTRL\[48\]如何设置，均按饱和模式处理。
>
>- 对于fp8\_e4m3fn\_t类型，CTRL\[50\]比特位在饱和模式下进一步控制nan的输出：CTRL\[50\]=1'b0（默认）时nan被转换为0；CTRL\[50\]=1'b1时nan保持不变。
>
>- 下表中的MAX/MIN对应目标数据类型下的最大值和最小值。

## float L0C → 各输出类型

L0C Buffer中存储的float类型数据来自Mmad矩阵乘计算结果，当输入矩阵A中包含inf/-inf/nan时，L0C中对应行会出现inf/-inf/nan（详见[Mmad接口边界值汇总](mmad_interface_boundary_value_summary.md)）。

**表1** float类型L0C中存在inf/-inf/nan时，各输出类型的计算结果说明

<a name="table_fixpipe_l0c2ub_float_boundary"></a>

| 输出数据类型 | 随路量化模式 | L0C中的元素取值 | 输出元素取值<br>（饱和模式，CTRL\[48\]=0） | 输出元素取值<br>（非饱和模式，CTRL\[48\]=1） |
| --- | --- | --- | --- | --- |
| half | VQF322F16\_PRE | +inf | 65504.0（MAX） | +inf |
| half | VQF322F16\_PRE | -inf | -65504.0（MIN） | -inf |
| half | VQF322F16\_PRE | nan | 0.0 | nan |
| half | F322F16（Cast） | +inf | 65504.0（MAX） | +inf |
| half | F322F16（Cast） | -inf | -65504.0（MIN） | -inf |
| half | F322F16（Cast） | nan | 0.0 | nan |
| bfloat16\_t | VQF322BF16\_PRE | +inf | 0x7F7F（MAX） | 0x7F80（+inf） |
| bfloat16\_t | VQF322BF16\_PRE | -inf | 0xFF7F（MIN） | 0xFF80（-inf） |
| bfloat16\_t | VQF322BF16\_PRE | nan | 0x0000（0） | 0x7FFF（nan） |
| bfloat16\_t | F322BF16（Cast） | +inf | 0x7F7F（MAX） | 0x7F80（+inf） |
| bfloat16\_t | F322BF16（Cast） | -inf | 0xFF7F（MIN） | 0xFF80（-inf） |
| bfloat16\_t | F322BF16（Cast） | nan | 0x0000（0） | 0x7FFF（nan） |
| float | VQF322F32\_PRE | +inf | +inf | +inf |
| float | VQF322F32\_PRE | -inf | -inf | -inf |
| float | VQF322F32\_PRE | nan | nan | nan |
| fp8\_e4m3fn\_t | VQF322FP8\_PRE | +inf | 0x7E（448.0，MAX） | 0x7F（nan） |
| fp8\_e4m3fn\_t | VQF322FP8\_PRE | -inf | 0xFE（-448.0，MIN） | 0x7F（nan） |
| fp8\_e4m3fn\_t | VQF322FP8\_PRE | nan | 0x00（0）<br>CTRL\[50\]=0时 | 0x7F（nan） |
| hifloat8\_t | VQF322HIF8\_PRE | +inf | 111（HiF8+inf） | 111（HiF8+inf） |
| hifloat8\_t | VQF322HIF8\_PRE | -inf | 239（HiF8-inf） | 239（HiF8-inf） |
| hifloat8\_t | VQF322HIF8\_PRE | nan | 0 | 128（HiF8NAN） |
| hifloat8\_t | VQF322HIF8\_PRE\_HYBRID | +inf | 111（HiF8+inf） | 111（HiF8+inf） |
| hifloat8\_t | VQF322HIF8\_PRE\_HYBRID | -inf | 239（HiF8-inf） | 239（HiF8-inf） |
| hifloat8\_t | VQF322HIF8\_PRE\_HYBRID | nan | 0 | 128（HiF8NAN） |
| int8\_t | QF322B8\_PRE / VQF322B8\_PRE | +inf | 127（MAX） | 127（MAX） |
| int8\_t | QF322B8\_PRE / VQF322B8\_PRE | -inf | -128（MIN） | -128（MIN） |
| int8\_t | QF322B8\_PRE / VQF322B8\_PRE | nan | 0 | 0 |
| int4b\_t | VQF322S4\_PRE | +inf | 7（MAX） | 7（MAX） |
| int4b\_t | VQF322S4\_PRE | -inf | -8（MIN） | -8（MIN） |
| int4b\_t | VQF322S4\_PRE | nan | 0 | 0 |

注：

- float类型的inf/nan在饱和模式和非饱和模式下均保持原样输出，因为CTRL\[48\]不支持float数据类型。
- fp8\_e4m3fn\_t类型没有inf的编码格式。非饱和模式下，若结果绝对值为inf或大于fp8\_e4m3fn\_t的最大规格化值（448），输出为nan（0x7F）。
- hifloat8\_t类型中，111（0b01101111）为HiF8+inf编码，239（0b11101111）为HiF8-inf编码，128（0b10000000）为HiF8NAN编码。饱和模式下，inf保持为对应的inf编码，nan被饱和为0。
- hifloat8\_t的溢出判断阈值为1.25×2^15=40960，当量化后的值绝对值大于等于40960时输出inf编码（111/239），而非饱和为240。
- 整数类型只有饱和模式，饱和模式和非饱和模式下的输出结果一致。
- VQF322HIF8\_PRE使用Half to Away舍入模式，VQF322HIF8\_PRE\_HYBRID使用Hybrid舍入模式（|exp|<4时为Half to Away，|exp|≥4时为SSR随机舍入）。两种模式对inf/nan的处理一致。
- 表中列出的量化模式为实测验证的模式。同一输出类型的scalar模式（如QF322F16\_PRE）与tensor模式（如VQF322F16\_PRE）使用相同的类型转换算法，inf/nan边界值处理行为完全一致，差异仅在于量化参数的传递方式（单个标量 vs 每列张量），因此表中仅列出tensor模式。NoQuant模式不做类型转换和量化，数据原样搬运。
- L0C→UB与L0C→GM/L0C→L1的随路量化inf/nan处理行为完全一致，差异仅在于搬运目标（UB vs GM vs L1 Buffer）及内核函数中是否需要AIC/AIV跨核同步。边界值数据可参考[Fixpipe(L0C→GM)接口边界值汇总](fixpipe_l0c2gm_interface_boundary_value_summary.md)和[Fixpipe(L0C→L1)接口边界值汇总](fixpipe_l0c2l1_interface_boundary_value_summary.md)。

## int32\_t L0C → 各输出类型

int32\_t类型的L0C来自int8\_t输入矩阵的Mmad计算结果，不会产生inf/nan，但量化后可能产生溢出。

**表2** int32\_t类型L0C量化溢出时的计算结果说明

<a name="table_fixpipe_l0c2ub_int32_boundary"></a>

| 输出数据类型 | 随路量化模式 | L0C中的元素取值 | 输出元素取值 |
| --- | --- | --- | --- |
| half | DEQF16 / VDEQF16 | 量化后超出half表示范围 | 65504.0（MAX） / -65504.0（MIN） |
| bfloat16\_t | VQS322BF16\_PRE | 量化后超出bfloat16表示范围 | 0x7F7F（MAX） / 0xFF7F（MIN） |
| int8\_t | REQ8 / VREQ8 | 量化后大于127 | 127（MAX） |
| int8\_t | REQ8 / VREQ8 | 量化后小于-128 | -128（MIN） |
| int4b\_t | VREQ4 | 量化后大于7 | 7（MAX） |
| int4b\_t | VREQ4 | 量化后小于-8 | -8（MIN） |

注：整数类型只有饱和模式，溢出时饱和为目标类型的最值。half和bfloat16\_t在饱和模式和非饱和模式下均对溢出值进行饱和处理。

## 量化模式说明

asc\_copy\_l0c2ub接口支持如下随路量化模式，scalar模式使用单个标量量化参数，tensor模式使用每列独立的量化参数：

**表3** 量化模式说明

<a name="table_fixpipe_l0c2ub_quant_mode"></a>

| 源数据类型 | 量化模式 | 目标数据类型 | 量化方式 | 说明 |
| --- | --- | --- | --- | --- |
| - | NoQuant | - | 不量化 | 不开启量化功能 |
| int32\_t | DEQF16 | half | scalar | int32\_t量化成half |
| int32\_t | VDEQF16 | half | tensor | int32\_t量化成half |
| int32\_t | REQ4 | int4b\_t | scalar | int32\_t量化成int4b\_t |
| int32\_t | VREQ4 | int4b\_t | tensor | int32\_t量化成int4b\_t |
| int32\_t | REQ8 | int8\_t、uint8\_t | scalar | int32\_t量化成int8\_t/uint8\_t |
| int32\_t | VREQ8 | int8\_t、uint8\_t | tensor | int32\_t量化成int8\_t/uint8\_t |
| int32\_t | QS322BF16\_PRE | bfloat16\_t | scalar | int32\_t量化成bfloat16\_t |
| int32\_t | VQS322BF16\_PRE | bfloat16\_t | tensor | int32\_t量化成bfloat16\_t |
| float | QF322F16\_PRE | half | scalar | float量化成half |
| float | VQF322F16\_PRE | half | tensor | float量化成half |
| float | QF322BF16\_PRE | bfloat16\_t | scalar | float量化成bfloat16\_t |
| float | VQF322BF16\_PRE | bfloat16\_t | tensor | float量化成bfloat16\_t |
| float | F322F16 | half | cast | float cast成half，cast mode为CAST\_RINT模式 |
| float | F322BF16 | bfloat16\_t | cast | float cast成bfloat16\_t，cast mode为CAST\_RINT模式 |
| float | QF322S4\_PRE | int4b\_t | scalar | float量化成int4b\_t |
| float | VQF322S4\_PRE | int4b\_t | tensor | float量化成int4b\_t |
| float | QF322B8\_PRE | int8\_t、uint8\_t | scalar | float量化成int8\_t/uint8\_t |
| float | VQF322B8\_PRE | int8\_t、uint8\_t | tensor | float量化成int8\_t/uint8\_t |
| float | QF322FP8\_PRE | fp8\_e4m3fn\_t | scalar | float量化成fp8\_e4m3fn\_t |
| float | VQF322FP8\_PRE | fp8\_e4m3fn\_t | tensor | float量化成fp8\_e4m3fn\_t |
| float | QF322HIF8\_PRE | hifloat8\_t | scalar | float量化成hifloat8\_t，Half to Away舍入 |
| float | VQF322HIF8\_PRE | hifloat8\_t | tensor | float量化成hifloat8\_t，Half to Away舍入 |
| float | QF322HIF8\_PRE\_HYBRID | hifloat8\_t | scalar | float量化成hifloat8\_t，Hybrid舍入 |
| float | VQF322HIF8\_PRE\_HYBRID | hifloat8\_t | tensor | float量化成hifloat8\_t，Hybrid舍入 |
| float | QF322F32\_PRE | float | scalar | float量化成float，精度可达双千分之一，无法达双万分之一，有双万分之一精度要求时建议使用AscendDeQuant高阶API |
| float | VQF322F32\_PRE | float | tensor | 同上 |

注：同一输出类型的scalar模式与tensor模式使用相同的类型转换算法，inf/nan边界值处理行为完全一致，差异仅在于量化参数的传递方式（单个标量 vs 每列张量）。NoQuant模式不做类型转换和量化，数据原样搬运，详见下文NoQuant场景。L0C→UB与L0C→GM/L0C→L1的随路量化模式及inf/nan处理行为完全一致。

## NoQuant（无量化）场景

NoQuant模式不做类型转换和量化，L0C中的float数据原样搬运到UB。由于CTRL\[48\]不支持float数据类型，饱和模式和非饱和模式下inf/nan均保持原样输出。NoQuant场景还覆盖了NZ2NZ、NZ2ND、双目标按M维度拆分、双目标按N维度拆分等输出格式，验证inf/-inf/nan在各种格式转换和双目标拆分场景下均能正确传递。

**表4** NoQuant模式下float类型L0C中存在inf/-inf/nan时的输出结果

<a name="table_fixpipe_l0c2ub_noquant_boundary"></a>

| 场景 | 输出格式 | 双目标 | L0C中的元素取值 | 输出元素取值 | 说明 |
| --- | --- | --- | --- | --- | --- |
| 1 | Nz | 否 | +inf/-inf/nan | +inf/-inf/nan | 原样保留 |
| 2 | ND | 否 | +inf/-inf/nan | +inf/-inf/nan | 原样保留 |
| 3 | ND | 是（按M拆分） | +inf/-inf/nan | +inf/-inf/nan | 双目标均正确传递 |
| 4 | ND | 是（按N拆分） | +inf/-inf/nan | +inf/-inf/nan | 双目标均正确传递 |

注：NoQuant场景的输出为float类型，CTRL\[48\]不生效，饱和模式和非饱和模式结果一致。场景3按M维度拆分时M必须为2的倍数，场景4按N维度拆分时N必须为32的倍数。

## 测试验证

以上边界值汇总通过[data_copy_l0c2ub样例](../../../../examples/02_simd_c_api/03_c_api/00_data_movement/data_copy_l0c2ub)的边界测试脚本`scripts/test_boundary.sh`在NPU上实测验证，结果为**38/38项全部通过**。共覆盖21种场景：

| 场景范围 | 场景数 | 测试模式 | 测试项 | 说明 |
| --- | --- | --- | --- | --- |
| 1-4（NoQuant） | 4 | 仅测试1次 | 4 | float输出，CTRL[48]不生效，饱和/非饱和结果一致 |
| 5-21（量化） | 17 | 饱和+非饱和 | 34 | 每种量化场景×2种CTRL[48]模式 |
| **合计** | **21** | | **38** | |

测试通过向输入矩阵A注入inf/-inf/nan（配合B矩阵对应行设为1.0），使L0C = A×B中产生inf/-inf/nan，验证各输出类型在饱和模式和非饱和模式下的inf/nan处理是否符合预期。对于int8\_t输入场景，使用极端值（A某行设为-128、其余设为127）验证整数溢出饱和行为。对于NoQuant场景，验证inf/-inf/nan在NZ2NZ、NZ2ND及双目标拆分模式下的正确传递。
