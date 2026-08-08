# asc_loadalign_deintlv

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

从Unified Buffer（UB）中起始地址32字节对齐的位置读取2×VL长度数据，将偶数下标元素写入矢量数据寄存器`dst0`，将奇数下标元素写入矢量数据寄存器`dst1`，每个矢量数据寄存器写入VL长度数据。本接口提供三种功能模式：

- 无偏移搬入：从`src`起始地址搬入数据。
- 带偏移搬入：从`src`起始地址加上`offset * sizeof(dtype)`字节偏移位置开始搬入数据。
- 带地址寄存器偏移搬入：通过`addr_reg`类型的地址寄存器指定起始地址偏移，读取2×VL长度数据。需要与[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)配合使用。接口不会修改`src`指针或按值传入的`offset`。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 无偏移搬入

```c
__simd_callee__ inline void asc_loadalign_deintlv(vector_<dtype>& dst0,
                                                  vector_<dtype>& dst1,
                                                  __ubuf__ <dtype>* src)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

```c
// 示例：float类型无偏移去交织加载。
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0,
                                                  vector_float& dst1,
                                                  __ubuf__ float* src)
```

### 带偏移搬入

```c
__simd_callee__ inline void asc_loadalign_deintlv(vector_<dtype>& dst0,
                                                  vector_<dtype>& dst1,
                                                  __ubuf__ <dtype>* src,
                                                  int32_t offset)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

```c
// 示例：float类型立即数偏移去交织加载。
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0,
                                                  vector_float& dst1,
                                                  __ubuf__ float* src,
                                                  int32_t offset)
```

### 带地址寄存器偏移搬入

```c
__simd_callee__ inline void asc_loadalign_deintlv(vector_<dtype>& dst0,
                                                  vector_<dtype>& dst1,
                                                  __ubuf__ <dtype>* src,
                                                  addr_reg offset)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

```c
// 示例：float类型地址寄存器偏移去交织加载。
__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0,
                                                  vector_float& dst1,
                                                  __ubuf__ float* src,
                                                  addr_reg offset)
```

## 参数说明

**表1** 参数说明（无偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst0 | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入源数据中的偶数下标元素，数据长度为VL。 |
| dst1 | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入源数据中的奇数下标元素，数据长度为VL。 |
| src | 输入 | 源UB地址，实际读取地址需32字节对齐。从`src`起始位置读取2×VL长度数据。 |

**表2** 参数说明（带偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst0 | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入源数据中的偶数下标元素，数据长度为VL。 |
| dst1 | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入源数据中的奇数下标元素，数据长度为VL。 |
| src | 输入 | 源UB地址，实际读取地址需32字节对齐。从`src`起始位置读取2×VL长度数据。 |
| offset | 输入 | 相对`src`起始地址的偏移，单位为元素个数。`offset`为负值表示源地址向低地址方向移动。 |

**表3** 参数说明（带地址寄存器偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst0 | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入源数据中的偶数下标元素，数据长度为VL。 |
| dst1 | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入源数据中的奇数下标元素，数据长度为VL。 |
| src | 输入 | 源UB地址，实际读取地址需32字节对齐。从`src`起始位置读取2×VL长度数据。 |
| offset | 输入 | 地址寄存器，类型为`addr_reg`，须通过[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)生成。该寄存器记录的实际字节偏移由创建地址寄存器时选择的位宽模式决定：b8、b16、b32模式分别以1字节、2字节、4字节为偏移单位。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- src与dst0、dst1的dtype必须完全一致，dtype不一致触发异常。
- 实际读取的UB地址和偏移后的UB地址必须32字节对齐，并且在UB地址空间内且不越界，否则会报错。
- dst0与dst1必须为不同的矢量数据寄存器。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。
- 针对带地址寄存器偏移搬入：必须先调用`asc_update_addr_reg`接口。

## 调用示例

从UB中连续读取以32B对齐地址`src`为起始位置的`float`数据。

### 基址寻址接口

```cpp
for (uint16_t i = 0; i < repeat_times; i++) {
    asc_loadalign_deintlv(reg0, reg1, src + i * one_repeat_size);
    ...
}
```

### 标量偏移寻址接口

```cpp
// 方式1：通过offset控制偏移。
for (uint16_t i = 0; i < repeat_times; i++) {
    asc_loadalign_deintlv(reg0, reg1, src, i * one_repeat_size);
    ...
}

// 方式2：通过src控制偏移。
for (uint16_t i = 0; i < repeat_times; i++) {
    asc_loadalign_deintlv(reg0, reg1, src + i * one_repeat_size, 0);
    ...
}
```

### 地址寄存器偏移寻址接口

```cpp
for (uint16_t i = 0; i < repeat_times; i++) {
    addr_reg addr_reg = asc_update_addr_reg_b32(one_repeat_size);
    asc_loadalign_deintlv(reg0, reg1, src, addr_reg);
    ...
}
```
