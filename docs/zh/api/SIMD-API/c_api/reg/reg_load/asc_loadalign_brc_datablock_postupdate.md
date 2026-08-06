# asc_loadalign_brc_datablock_postupdate

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

本接口为矢量数据寄存器对齐搬入接口，从Unified Buffer（UB）中起始地址32字节对齐的位置读取一个DataBlock（32字节），并将该DataBlock广播填充到整个矢量长度（VL）的矢量数据寄存器`dst`中，即VL范围内每个DataBlock位置都写入同一份32字节数据。搬运完成后，按`offset`参数自动更新源地址`src`（post-update模式），便于在硬件循环内连续多次调用时无需手动累加源地址。

## 函数原型

```c
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_<dtype>& dst,
                                                                   __ubuf__ <dtype>*& src,
                                                                   int32_t offset)
```

dtype支持的数据类型为`bfloat16_t`、`float`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`half`、`hifloat8_t`、`int16_t`、`int32_t`、`int4b_t`、`int8_t`、`uint16_t`、`uint32_t`、`uint8_t`

```c
// 示例：b8重载（读取32字节即32个元素广播到VL）
__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(vector_int8_t& dst,
                                                                   __ubuf__ int8_t*& src,
                                                                   int32_t offset)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的矢量数据寄存器，dtype必须和src一致，搬入VL长度数据。 |
| `src` | 输入/输出 | 源UB地址，起始地址需32字节对齐。 |
| `offset` | 输入 | post-update地址步长，单位元素个数。接口执行后`src`按此值自动累加（`src = src + offset * sizeof(dtype)`），实现硬件循环内连续搬入时无需手动维护源地址。offset为负值表示源地址向低地址方向移动，需确保更新后的`src`仍指向有效UB区域且满足32字节对齐，否则下一次调用触发异常。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- 本接口不能与编译选项`--enable-simt`同时使用，否则编译报错。
- `src`起始地址需32字节对齐，否则触发搬运异常。
- 如果需要执行多条本接口指令，且源地址存在重叠，需通过同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)保证串行化，防止读到非预期数据。
- UB容量上限：UB总容量256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- post-update后的`src`新地址（`src + offset`字节）仍需满足32字节对齐，否则下一次调用本接口触发源地址未对齐异常。
- `dst`与`src`的dtype必须一致，dtype不一致触发类型异常。
- `offset × sizeof(dtype)`应为32字节的整数倍，以保证post-update后的`src`仍满足32字节对齐；若`offset × sizeof(dtype)`不是32字节整数倍，post-update后的`src`在下一次调用时会报错。
- offset可为负值，但 `src + offset × sizeof(dtype)` 不可小于UB起始地址0，否则会报错。

## 调用示例

```cpp
vector_half dst;
__ubuf__ half* src;
asc_loadalign_brc_datablock_postupdate(dst, src, 0);
```
