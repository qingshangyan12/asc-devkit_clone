# asc_set_l13d_rpt

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

用于设置Load3Dv2接口的repeat参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入 | 用于设置Load3Dv2接口的repeat参数，详细说明请参考[asc_load3d_v2_config.md](../struct/asc_load3d_v2_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需配合[asc_load3d_v2_config.md](../struct/asc_load3d_v2_config.md)使用，用于设置Load3Dv2接口的repeat参数。

<!-- npu="950" id8 -->
### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- 本接口仅设置repeat控制参数，实际搬运由后续调用的3D格式分形矩阵搬运接口执行，须先调用本接口写入配置，再调用对应搬运接口。
- 配置写入后对后续所有3D格式分形矩阵搬运接口生效，直至再次调用本接口覆盖配置；不同搬运任务间若需不同repeat参数，应在调用搬运接口前重新调用本接口设置。
- 本接口须与其他配套setter接口配合使用：先调用配置Feature Map属性、填充属性、L1 Buffer边界值的配套setter接口，再调用本接口配置repeat参数，最后调用3D格式分形搬运接口执行搬运（配套setter名见功能说明章节）。

### repeat参数约束

- `rpt_time`不可为0，否则对应3D格式分形搬运指令被置为空操作，不执行搬运。
- `rpt_stride`单位随`rpt_mode`切换：`rpt_mode=0`时单位为16个元素，`rpt_mode=1`时单位为分形列大小（随搬运数据类型变化，int4b_t为64元素，int8_t/uint8_t为32元素，half/int16_t/bfloat16_t为16元素，int32_t/float为8元素）；用户须按所选repeat方向设置对应单位的步长值。
- `rpt_mode`仅取0或1，其他值会按位宽截断处理（如2→0、3→1），硬件不报错但实际repeat方向与预期不符，可能沿错误维度重复搬运。

### 字段位宽截断约束

- `rpt_stride`，结构体`uint64_t`类型可表示0~2^64−1，传入>2^16−1的值会按位宽截断处理（如2^16→0、2^16+1→1），硬件不报错。截断后实际生效值偏小，导致相邻repeat之间的地址偏移量小于预期，后一次repeat读到错误位置，与前一次repeat数据重叠或错位。
- `rpt_time`，结构体`uint64_t`类型可表示0~2^64−1，传入>2^8−1的值会按位宽截断处理（如2^8→0、2^8+1→1），硬件不报错。截断后实际生效值偏小，导致实际迭代次数小于预期，循环提前结束，部分数据未被搬运；截断后若值为0会同时触发`rpt_time`不可为0的异常，指令被置为空操作。
- `rpt_mode`，结构体`uint64_t`类型可表示0~2^64−1，传入>1的值会按位宽截断处理（如2→0、3→1），硬件不报错。截断后实际生效的repeat方向与预期不符，可能沿错误维度重复搬运。

<!-- end id8 -->

## 调用示例

```cpp
asc_load3d_v2_config config;
asc_set_l13d_rpt(config); // 设置Load3D的repeat相关参数为默认值rpt_stride：0，rpt_time：1，rpt_mode：0
```