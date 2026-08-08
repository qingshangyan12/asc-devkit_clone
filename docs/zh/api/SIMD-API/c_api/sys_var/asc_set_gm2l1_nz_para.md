# asc_set_gm2l1_nz_para

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

本接口用于配置GM到L1 Buffer的ND2NZ和DN2NZ搬运参数，包括矩阵数量以及目的NZ矩阵的三项步长。配置写入后，由后续的[asc_copy_gm2l1_nd2nz](../cube_data_move/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.md)或[asc_copy_gm2l1_dn2nz](../cube_data_move/asc_copy_gm2l1_dn2nz.md)接口使用。

本接口只配置目的侧的NZ参数。源矩阵步长、矩阵行列数和SmallC0模式等参数由后续搬运接口配置。

## 函数原型

```c
__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| config | 输入 | ND2NZ或DN2NZ搬运的64 bit目的侧配置。接口不检查或转换字段值，需按照[表2](#table2)完成拼装。 |

**表2** config字段说明 <a id="table2"></a>

| bit范围 | 对应Basic API参数 | 描述 |
|---|---|---|
| 15:0 | ndNum/dnNum | 源矩阵中ND矩阵或DN矩阵的数量，取值范围为[0, 4095]。取值为0时，后续搬运接口被视为NOP（空操作）。 |
| 31:16 | dstNzNStride | 目的NZ矩阵中相邻行起始地址的偏移，取值范围为[1, 16384]，单位为C0_SIZE（32字节）。 |
| 47:32 | dstNzC0Stride | 目的NZ矩阵中相邻Z矩阵起始地址的偏移，取值范围为[1, 16384]，单位为C0_SIZE（32字节）。 |
| 63:48 | loop4DstStride（对应dstNzMatrixStride） | 目的矩阵中相邻NZ矩阵起始地址的偏移，字段取值范围为[0, 65535]，单位为C0_SIZE（32字节）。对应Basic API中以元素个数为单位的`dstNzMatrixStride`，其取值范围为[0, 16384]；本字段的计算方式为：`dstNzMatrixStride * sizeof(T) / 32`。矩阵数量为1时，本字段无意义，可设置为0；矩阵数量大于1且本字段为0时，后写入的NZ矩阵会覆盖第一个NZ矩阵。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅在AIC上生效，在AIV上调用不生效。
- 本接口需在对应的ND2NZ或DN2NZ搬运接口执行前调用。配置会持续生效，直至再次调用本接口覆盖；不同搬运任务使用的目的侧参数不同时，需分别配置。
- `config`的四个字段均为16 bit，接口会将`config`整体写入，不会单独检查字段范围。拼装前需确保各字段满足[表2](#table2)中的取值要求；不得将超出16 bit的值直接左移，否则高位会写入相邻字段。
- 参照Basic API的`dstNzMatrixStride`配置目的NZ矩阵间距时，`dstNzMatrixStride * sizeof(T)`需32字节对齐，再换算为[63:48]字段的值。
- 搬运至L1 Buffer的数据不能重叠。矩阵数量大于1时，需根据单个NZ矩阵占用的空间正确设置[63:48]字段。

## 调用示例

```cpp
constexpr uint16_t matrix_num = 1;           // [15:0]，搬运1个ND或DN矩阵
constexpr uint16_t dst_nz_n_stride = 1;      // [31:16]，单位为32字节
constexpr uint16_t dst_nz_c0_stride = 16;    // [47:32]，单位为32字节
constexpr uint16_t loop4_dst_stride = 0;      // [63:48]，仅搬运1个矩阵时设置为0

uint64_t config = (static_cast<uint64_t>(loop4_dst_stride) << 48) |
                  (static_cast<uint64_t>(dst_nz_c0_stride) << 32) |
                  (static_cast<uint64_t>(dst_nz_n_stride) << 16) |
                  static_cast<uint64_t>(matrix_num);
asc_set_gm2l1_nz_para(config);
```
