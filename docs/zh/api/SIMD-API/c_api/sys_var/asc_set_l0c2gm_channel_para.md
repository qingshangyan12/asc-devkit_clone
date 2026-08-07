# asc_set_l0c2gm_channel_para

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
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

头文件路径：`"c_api/sys_var/sys_var.h"`。

本接口用于配置L0C Buffer搬出并执行NZ2DN格式转换时，源NZ矩阵中相邻行的地址偏移。配置写入后，由后续开启NZ2DN功能的[asc_copy_l0c2gm](../cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)等L0C Buffer搬出接口使用；未开启NZ2DN功能时，本配置不生效。

## 函数原型

```c
__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | NZ2DN格式转换的源侧步长配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config字段说明 <a id="table2"></a>

| bit范围 | 对应Basic API参数 | 描述 |
|---|---|---|
| 47:0 | - | 保留位，需设置为0。 |
| 63:48 | srcNzC0Stride | 源矩阵NZ分形中相邻行的地址偏移，取值范围为[1, 65535]，单位为C0_SIZE（`16 * sizeof(T)`，T为源操作数的数据类型）。仅当后续L0C Buffer搬出接口开启NZ2DN功能时生效。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的L0C Buffer搬出接口执行前调用，且搬出接口需开启NZ2DN功能。配置会持续生效，直至再次调用本接口覆盖。
- `config[47:0]`为保留位，需设置为0；`config[63:48]`不能为0。接口会将`config`整体写入，不会检查字段值。
- 开启NZ2DN功能时，如果`srcNzC0Stride`不等于1，则不能同时开启unitFlag。

## 调用示例

```cpp
constexpr uint16_t src_nz_c0_stride = 1; // 单位为C0_SIZE
uint64_t config = static_cast<uint64_t>(src_nz_c0_stride) << 48;
asc_set_l0c2gm_channel_para(config);
```
