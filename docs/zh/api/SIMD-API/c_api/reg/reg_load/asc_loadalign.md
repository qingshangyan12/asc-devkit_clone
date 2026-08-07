# asc_loadalign

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

将UB中起始地址32字节对齐的连续数据搬入矢量数据寄存器或掩码寄存器，搬运过程中数据格式和内容保持不变。本接口提供四种功能模式：

- 无偏移搬入：当dst为矢量数据寄存器，从`src`起始地址搬入VL长度数据；当dst为掩码寄存器，从`src`起始地址搬入VL/8长度数据。

- 带偏移搬入：当dst为矢量数据寄存器，从`src`起始地址加上`offset * sizeof(dtype)`偏移位置开始搬入VL长度数据；当dst为掩码寄存器，从`src`起始地址加上`offset`字节偏移位置开始搬入VL/8长度数据。

- 带地址寄存器偏移搬入：通过`addr_reg`类型的地址寄存器偏移指定起始位置开始搬入。当dst为矢量数据寄存器，搬入VL长度数据；当dst为掩码寄存器，搬入VL/8长度数据。需要与[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)配合使用。

- 非连续对齐搬入：从当前`src`偏移`repeat_stride × 32`字节地址开始，按照`block_stride`指定的DataBlock间隔读取8个DataBlock，搬入矢量数据寄存器。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 无偏移搬入

```c
// 搬入矢量数据寄存器
__simd_callee__ inline void asc_loadalign(vector_<dtype>& dst,
                                          __ubuf__ <dtype>* src)
// 搬入掩码寄存器
__simd_callee__ inline void asc_loadalign(vector_bool& dst,
                                          __ubuf__ uint32_t* src)
```

dtype支持的数据类型为`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`。

```c
// 示例：float类型
__simd_callee__ inline void asc_loadalign(vector_float& dst,
                                          __ubuf__ float* src)
```

### 带偏移搬入

```c
// 搬入矢量数据寄存器
__simd_callee__ inline void asc_loadalign(vector_<dtype>& dst,
                                          __ubuf__ <dtype>* src,
                                          int32_t offset)
// 搬入掩码寄存器
__simd_callee__ inline void asc_loadalign(vector_bool& dst,
                                          __ubuf__ uint32_t* src,
                                          int32_t offset)
```

dtype支持的数据类型为`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`。

```c
// 示例：float类型
__simd_callee__ inline void asc_loadalign(vector_float& dst,
                                          __ubuf__ float* src,
                                          int32_t offset)
```

### 带地址寄存器偏移搬入

```c
// 搬入矢量数据寄存器
__simd_callee__ inline void asc_loadalign(vector_<dtype>& dst,
                                          __ubuf__ <dtype>* src,
                                          addr_reg offset)
// 搬入掩码寄存器
__simd_callee__ inline void asc_loadalign(vector_bool& dst,
                                          __ubuf__ uint32_t* src,
                                          addr_reg offset)
```

dtype支持的数据类型为`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`。

```c
// 示例：float类型
__simd_callee__ inline void asc_loadalign(vector_float& dst,
                                          __ubuf__ float* src,
                                          addr_reg offset)
```

### 非连续对齐搬入

```c
__simd_callee__ inline void asc_loadalign(vector_<dtype>& dst,
                                          __ubuf__ <dtype>* src,
                                          uint16_t block_stride,
                                          uint16_t repeat_stride,
                                          vector_bool mask);
```

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`int64_t`、`bfloat16_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`、`int4b_t`、`half`、`float`。

```c
// 示例：float类型
__simd_callee__ inline void asc_loadalign(vector_float& dst,
                                          __ubuf__ float* src,
                                          uint16_t block_stride,
                                          uint16_t repeat_stride,
                                          vector_bool mask);
```

## 参数说明

**表1** 参数说明（无偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器或掩码寄存器。<br>&bull; 当dst为矢量数据寄存器，dtype必须和src一致，搬入VL长度数据。<br>&bull; 当dst为掩码寄存器，搬入VL/8长度数据。|
| src | 输入 | 源UB地址，起始地址需32字节对齐。 |

**表2** 参数说明（带偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器或掩码寄存器。 <br>&bull; 当dst为矢量数据寄存器，dtype必须和src一致，搬入VL长度数据。<br>&bull; 当dst为掩码寄存器，搬入VL/8长度数据。 |
| src | 输入 | 源UB地址，起始地址需32字节对齐。 |
| offset | 输入 | 相对`src`起始地址的偏移。<br>&bull; 当dst为矢量数据寄存器，单位为元素个数。<br>&bull; 当dst为掩码寄存器，单位为字节。offset为负值表示源地址向低地址方向移动。 |

**表3** 参数说明（带地址寄存器偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器或掩码寄存器。 <br>&bull; 当dst为矢量数据寄存器，dtype必须和src一致，搬入VL长度数据。<br>&bull; 当dst为掩码寄存器，搬入VL/8长度数据。 |
| src | 输入 | 源UB地址，起始地址需32字节对齐。 |
| offset | 输入 | 地址寄存器偏移，类型为`addr_reg`，单位为元素个数。vector_bool使用b32模式，实际字节偏移为元素偏移乘4。 |

**表4** 参数说明（非连续对齐搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器，dtype必须和src一致。 |
| src | 输入 | 源UB地址，起始地址需32字节对齐。 |
| block_stride | 输入 | 块间步长，控制一次搬入中相邻32字节数据块之间的源UB地址间隔，单位为32字节（DataBlock）。第i个块（i=0..7）的地址为`src`偏移`i × block_stride × 32`字节。取值范围：[0, 2^16−1]。 |
| repeat_stride | 输入 | 重复步长，控制起始读取位置相对src基地址的偏移，单位为32字节（DataBlock）。取值范围为[0, 2^16−1]。 |
| mask | 输入 | 掩码寄存器，用于指示在计算过程中哪些元素参与计算。<br>&bull; 因该接口以DataBlock为数据搬运单元，当DataBlock中的任意一个元素被mask筛选成有效元素时，该DataBlock中所有数据都会搬入至矢量数据寄存器。<br>&bull; 当DataBlock中所有元素都被mask筛选成无效元素时，该DataBlock中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- src与dst的dtype必须完全一致，dtype不一致触发异常。
- 实际读取的UB地址和偏移后的UB地址必须32字节对齐，并且在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。
- 针对带地址寄存器偏移搬入：必须先调用`asc_update_addr_reg`接口。
- 针对非连续对齐搬入：因该接口以DataBlock为数据搬运单元，当DataBlock中的任意一个元素被mask筛选成有效元素时，该DataBlock中所有数据都会搬入至矢量数据寄存器。当DataBlock中所有元素都被mask筛选成无效元素时，该DataBlock中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。

## 调用示例

```cpp
vector_half dst;
__ubuf__ half* src;
asc_loadalign(dst, src);
```
