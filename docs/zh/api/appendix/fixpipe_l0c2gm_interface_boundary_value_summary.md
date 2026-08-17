# Fixpipe(L0C→GM)接口边界值汇总<a name="ZH-CN_TOPIC_0000000000000001"></a>

本节汇总介绍了Fixpipe L0C→GM搬运接口（C API [asc_copy_l0c2gm](../SIMD-API/c_api/cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)、高阶API [Fixpipe](../SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)）在L0C Buffer中存在inf/-inf/nan等边界值时的输出结果，以下内容针对如下型号生效：

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT
<!-- end id1 -->

## 概述

Fixpipe L0C→GM接口在搬运过程中支持随路量化，将L0C中的float或int32\_t数据转换为目标数据类型输出。对于浮点类型的inf/nan输入输出，可通过CTRL寄存器的CTRL\[48\]比特位控制饱和模式：

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

**表1** float类型L0C中存在inf/-inf/nan时，各输出类型的计算结果说明

<a name="table_fixpipe_l0c2gm_float_boundary"></a>

| 输出数据类型 | L0C中的元素取值 | 输出元素取值（饱和模式，CTRL\[48\]=0） | 输出元素取值（非饱和模式，CTRL\[48\]=1） |
| --- | --- | --- | --- |
| half | +inf | 65504.0（MAX） | +inf |
| half | -inf | -65504.0（MIN） | -inf |
| half | nan | 0.0 | nan |
| bfloat16\_t | +inf | 0x7F7F（MAX） | 0x7F80（+inf） |
| bfloat16\_t | -inf | 0xFF7F（MIN） | 0xFF80（-inf） |
| bfloat16\_t | nan | 0x0000（0） | 0x7FFF（nan） |
| float | +inf | +inf | +inf |
| float | -inf | -inf | -inf |
| float | nan | nan | nan |
| fp8\_e4m3fn\_t | +inf | 448.0（MAX，0x7E） | nan（0x7F） |
| fp8\_e4m3fn\_t | -inf | -448.0（MIN，0xFE） | nan（0x7F） |
| fp8\_e4m3fn\_t | nan | 0.0（CTRL\[50\]=0） / nan（CTRL\[50\]=1） | nan（0x7F） |
| hifloat8\_t | +inf | 111（HiF8+inf编码） | 111（HiF8+inf编码） |
| hifloat8\_t | -inf | 239（HiF8-inf编码） | 239（HiF8-inf编码） |
| hifloat8\_t | nan | 0 | 128（HiF8NAN编码） |
| int8\_t | +inf | 127（MAX） | 127（MAX） |
| int8\_t | -inf | -128（MIN） | -128（MIN） |
| int8\_t | nan | 0 | 0 |
| int4b\_t | +inf | 7（MAX） | 7（MAX） |
| int4b\_t | -inf | -8（MIN） | -8（MIN） |
| int4b\_t | nan | 0 | 0 |

注：

- float类型的inf/nan在饱和模式和非饱和模式下均保持原样输出，因为CTRL\[48\]不支持float数据类型。
- fp8\_e4m3fn\_t类型没有inf的编码格式。非饱和模式下，若结果绝对值为inf或大于fp8\_e4m3fn\_t的最大规格化值（448），输出为nan（0x7F）。
- hifloat8\_t类型中，111（0b01101111）为HiF8+inf编码，239（0b11101111）为HiF8-inf编码，128（0b10000000）为HiF8NAN编码。饱和模式下，inf保持为对应的inf编码，nan被饱和为0。
- 整数类型只有饱和模式，饱和模式和非饱和模式下的输出结果一致。

## int32\_t L0C → 各输出类型

**表2** int32\_t类型L0C中存在极值时的计算结果说明

<a name="table_fixpipe_l0c2gm_int32_boundary"></a>

| 输出数据类型 | L0C中的元素取值 | 输出元素取值 |
| --- | --- | --- |
| half | 正向溢出（超出half表示范围） | 65504.0（MAX） |
| half | 负向溢出（超出half表示范围） | -65504.0（MIN） |
| bfloat16\_t | 正向溢出 | 0x7F7F（MAX） |
| bfloat16\_t | 负向溢出 | 0xFF7F（MIN） |
| int8\_t | 大于127 | 127（MAX） |
| int8\_t | 小于-128 | -128（MIN） |
| int4b\_t | 大于7 | 7（MAX） |
| int4b\_t | 小于-8 | -8（MIN） |

注：int32\_t L0C不会产生inf/nan，但量化后可能产生溢出。整数类型只有饱和模式，溢出时饱和为目标类型的最值。
