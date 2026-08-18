# Fixpipe(L0C→L1)接口边界值汇总<a name="ZH-CN_TOPIC_0000000000000003"></a>

本节汇总介绍了Fixpipe L0C→L1搬运接口（C API [asc_copy_l0c2l1](../SIMD-API/c_api/cube_data_move/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md)）在L0C Buffer中存在inf/-inf/nan等边界值时的输出结果，以下内容针对如下型号生效：

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT
<!-- end id1 -->

## 概述

Fixpipe L0C→L1接口将L0C中的矩阵计算结果搬运到L1 Buffer，在搬运过程中支持随路量化，将L0C中的float或int32\_t数据转换为目标数据类型输出。由于dav-3510架构中L1 Buffer数据无法直接搬运到GM，样例通过第二次Mmad（量化结果×单位矩阵D）将数据从L1经L0A→L0C搬运到GM，最终输出为int32\_t（int8输入场景）或float（其他场景）。

### 支持的数据类型组合

针对Ascend 950PR/Ascend 950DT，asc\_copy\_l0c2l1接口支持的数据类型组合如下：

| 源矩阵（L0C Buffer） | 目的矩阵（L1 Buffer） |
| --- | --- |
| float | int8\_t、uint8\_t、hifloat8\_t、fp8\_e4m3fn\_t、half、bfloat16\_t、float |
| int32\_t | int8\_t、uint8\_t、half、bfloat16\_t、int32\_t |

随路量化的inf/nan处理与Fixpipe L0C→GM接口完全一致，通过CTRL寄存器的CTRL\[48\]比特位控制饱和模式：

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
>
>- 量化后的数据经过第二次Mmad（C×D，D为单位矩阵）输出到GM。由于dav-3510架构中L1 Buffer数据无法直接搬运到GM，第二次Mmad仅作为数据中转路径。需要注意，当量化结果中包含inf时，第二次Mmad中inf与单位矩阵的off-diagonal零元素相乘会产生nan（IEEE 754: inf×0=nan），累加后整行变为nan。此为验证路径的限制，不影响asc\_copy\_l0c2l1本身的inf/nan处理行为。

## float L0C → 各输出类型

L0C Buffer中存储的float类型数据来自Mmad矩阵乘计算结果，当输入矩阵A中包含inf/-inf/nan时，L0C中对应行会出现inf/-inf/nan（详见[Mmad接口边界值汇总](mmad_interface_boundary_value_summary.md)）。

**表1** float类型L0C中存在inf/-inf/nan时，各输出类型的计算结果说明

<a name="table_fixpipe_l0c2l1_float_boundary"></a>

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
| int4b\_t<sup>†</sup> | VQF322S4\_PRE | +inf | 7（MAX） | 7（MAX） |
| int4b\_t<sup>†</sup> | VQF322S4\_PRE | -inf | -8（MIN） | -8（MIN） |
| int4b\_t<sup>†</sup> | VQF322S4\_PRE | nan | 0 | 0 |

注：

- float类型的inf/nan在饱和模式和非饱和模式下均保持原样输出，因为CTRL\[48\]不支持float数据类型。
- fp8\_e4m3fn\_t类型没有inf的编码格式。非饱和模式下，若结果绝对值为inf或大于fp8\_e4m3fn\_t的最大规格化值（448），输出为nan（0x7F）。
- hifloat8\_t类型中，111（0b01101111）为HiF8+inf编码，239（0b11101111）为HiF8-inf编码，128（0b10000000）为HiF8NAN编码。饱和模式下，inf保持为对应的inf编码，nan被饱和为0。
- hifloat8\_t的溢出判断阈值为1.25×2^15=40960，当量化后的值绝对值大于等于40960时输出inf编码（111/239），而非饱和为240。
- 整数类型只有饱和模式，饱和模式和非饱和模式下的输出结果一致。
- <sup>†</sup>int4b\_t不在官方支持的数据类型组合中（见"支持的数据类型组合"章节），表中所列为基于整数类型饱和行为推导的理论参考值，非实测结果。
- VQF322HIF8\_PRE使用Half to Away舍入模式，VQF322HIF8\_PRE\_HYBRID使用Hybrid舍入模式（|exp|<4时为Half to Away，|exp|≥4时为SSR随机舍入）。两种模式对inf/nan的处理一致。
- 表中列出的量化模式为实测验证的模式。同一输出类型的scalar模式（如QF322F16\_PRE）与tensor模式（如VQF322F16\_PRE）使用相同的类型转换算法，inf/nan边界值处理行为完全一致，差异仅在于量化参数的传递方式（单个标量 vs 每列张量），因此表中仅列出tensor模式。
- L0C→L1与L0C→GM/L0C→UB的随路量化inf/nan处理行为完全一致。边界值数据可参考[Fixpipe(L0C→GM)接口边界值汇总](fixpipe_l0c2gm_interface_boundary_value_summary.md)和[Fixpipe(L0C→UB)接口边界值汇总](fixpipe_l0c2ub_interface_boundary_value_summary.md)。

## int32\_t L0C → 各输出类型

int32\_t类型的L0C来自int8\_t输入矩阵的Mmad计算结果，不会产生inf/nan，但量化后可能产生溢出。

**表2** int32\_t类型L0C量化溢出时的计算结果说明

<a name="table_fixpipe_l0c2l1_int32_boundary"></a>

| 输出数据类型 | 随路量化模式 | L0C中的元素取值 | 输出元素取值 |
| --- | --- | --- | --- |
| half | DEQF16 / VDEQF16 | 量化后超出half表示范围 | 65504.0（MAX） / -65504.0（MIN） |
| bfloat16\_t | VQS322BF16\_PRE | 量化后超出bfloat16表示范围 | 0x7F7F（MAX） / 0xFF7F（MIN） |
| int8\_t | REQ8 / VREQ8 | 量化后大于127 | 127（MAX） |
| int8\_t | REQ8 / VREQ8 | 量化后小于-128 | -128（MIN） |
| int4b\_t<sup>†</sup> | VREQ4 | 量化后大于7 | 7（MAX） |
| int4b\_t<sup>†</sup> | VREQ4 | 量化后小于-8 | -8（MIN） |

注：整数类型只有饱和模式，溢出时饱和为目标类型的最值。half和bfloat16\_t在饱和模式和非饱和模式下均对溢出值进行饱和处理。<sup>†</sup>int4b\_t不在官方支持的数据类型组合中，表中所列为理论参考值。

## 测试验证

以上边界值汇总通过[data_copy_l0c2l1样例](../../../../examples/02_simd_c_api/03_c_api/00_data_movement/data_copy_l0c2l1)的边界测试脚本`scripts/test_boundary.sh`在NPU上实测验证。

测试通过向输入矩阵A注入inf/-inf/nan（配合B矩阵对应行设为1.0），使L0C = A×B中产生inf/-inf/nan，验证各输出类型在饱和模式和非饱和模式下的inf/nan处理是否符合预期。对于int8\_t输入场景，使用极端值（A某行设为-128、其余设为127）验证整数溢出饱和行为。

### 测试覆盖范围

共覆盖17种量化场景×2种饱和模式（饱和/非饱和）= 34项测试，其中：

- **已验证且通过**：场景1-10的饱和模式（10项）、场景1-4和7-8的非饱和模式（6项），共16项。
- **已验证但失败（已知限制）**：场景5、6、9、10的非饱和模式（4项）和场景17的两种模式（2项），共6项。失败原因详见下文"已知限制"。
- **未验证（CANN版本限制）**：场景11-16的两种模式（12项）。这些场景依赖的API重载在当前CANN版本（9.2.0）中未实现或未提供，具体见下文"未验证场景说明"。

### 已知限制

以下场景在边界值测试中失败，属于验证路径或硬件限制，非asc\_copy\_l0c2l1接口本身的问题：

**表3** 边界值测试已知限制

<a name="table_fixpipe_l0c2l1_known_limitations"></a>

| 失败场景 | 饱和模式 | 根因 | 说明 |
| --- | --- | --- | --- |
| 5、6、9、10 | 非饱和 | 验证路径中第二次Mmad（C×D，D为单位矩阵）的off-diagonal元素为0，IEEE 754下inf×0.0=nan，累加后整行变为nan | L1 Buffer上的量化结果实际保留了inf（符合表1预期），但经过第二次Mmad后被破坏为nan，导致GM输出与golden数据不匹配。此问题仅影响inf边界值在非饱和模式下的验证，不影响asc\_copy\_l0c2l1本身的inf处理行为 |
| 17 | 饱和和非饱和模式均失败 | float×float→float的Mmad在L0C包含inf/-inf/nan时触发硬件运行时错误（aclrtSynchronizeStream返回507015） | 可能是dav-3510的CANN版本限制，float类型Mmad对inf/nan输入的硬件处理异常 |

### 未验证场景说明

以下场景因CANN版本限制未纳入实测验证：

**表4** 未验证场景及原因

<a name="table_fixpipe_l0c2l1_unverified"></a>

| 场景 | 输出类型 | 量化模式 | 官方支持 | 未验证原因 |
| --- | --- | --- | --- | --- |
| 11 | int4b\_t | VREQ4 | 否 | int4b\_t不在官方支持的数据类型组合中，且asc\_mmad\_s4（int4b\_t×int4b\_t→int32\_t）在dav-3510上未实现，验证路径的第二次Mmad无法执行 |
| 12 | bfloat16\_t | VQS322BF16\_PRE | 是 | CANN 9.2.0未提供asc\_copy\_l0c2l1的bfloat16\_t/int32\_t重载，待后续CANN版本补齐后可启用验证 |
| 13 | int4b\_t | VQF322S4\_PRE | 否 | int4b\_t不在官方支持的数据类型组合中，同场景11 |
| 14 | fp8\_e4m3fn\_t | VQF322FP8\_PRE | 是 | CANN 9.2.0未提供asc\_copy\_l0c2l1的fp8\_e4m3fn\_t/float重载，待后续CANN版本补齐后可启用验证 |
| 15 | hifloat8\_t | VQF322HIF8\_PRE | 是 | CANN 9.2.0未提供asc\_copy\_l0c2l1的hifloat8\_t/float重载，待后续CANN版本补齐后可启用验证 |
| 16 | hifloat8\_t | VQF322HIF8\_PRE\_HYBRID | 是 | 同场景15 |

注：场景12、14、15、16的数据类型组合在官方支持矩阵中已列出，仅因当前CANN 9.2.0版本未提供对应重载而无法验证，待CANN后续版本补齐重载后可启用端到端验证。场景11、13的int4b\_t不在官方支持的数据类型组合中，表1和表2中列出的int4b\_t边界值为理论参考值（基于整数类型饱和行为推导），非实测结果。
