# asc_ndim_copy_gm2ub（废弃）

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

头文件路径为：`"c_api/vector_datamove/vector_datamove.h"`。

末尾参数类型为`uint8_t cache_mode`的接口重载已废弃，请使用[asc_ndim_copy_gm2ub](../vector_datamove/asc_ndim_copy_gm2ub.md)中末尾参数类型为[asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md)的重载。

该接口将最多5个维度的数据从Global Memory（GM）搬运到Unified Buffer（UB），支持按维配置步长和随路Padding。

## 函数原型

```cpp
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(__ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size, uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode, uint8_t cache_mode)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的UB地址。 |
| src | 输入 | 源GM地址。 |
| loop0_size～loop4_size | 输入 | 各维度处理的元素个数，不包含Padding元素，取值范围为[1, 2^20-1]。 |
| loop0_lp_count | 输入 | 0维左侧需要补齐的元素个数，取值范围为[0, 255]。 |
| loop0_rp_count | 输入 | 0维右侧需要补齐的元素个数，取值范围为[0, 255]。 |
| padding_mode | 输入 | Padding值填充方式。`true`表示使用固定值填充，`false`表示使用当前维度最左或最右的值填充。 |
| cache_mode | 输入 | 旧的L2 Cache管理策略配置值。该参数类型已废弃，请改用`asc_load_l2_cache_mode`。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 该废弃重载仅保留用于兼容已有代码，新代码不应继续调用。

## 调用示例

以下示例使用`asc_load_l2_cache_mode`类型的替代重载：

```cpp
asc_ndim_copy_gm2ub(dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
    loop0_lp_count, loop0_rp_count, padding_mode, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
```
