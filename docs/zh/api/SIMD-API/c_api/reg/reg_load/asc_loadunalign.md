# asc_loadunalign

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

将Unified Buffer（UB）中起始地址任意字节对齐的连续数据搬入矢量数据寄存器，搬入数据量为VL。本接口是非对齐搬入的第二阶段，需要先调用[asc_loadunalign_pre](asc_loadunalign_pre.md)初始化非对齐寄存器[vector_load_unalign](../reg_data_types/data_type_definition.md#非对齐寄存器)。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_loadunalign(vector_<dtype>& dst,
                                            vector_load_unalign& src0,
                                            __ubuf__ <dtype>* src1)
```

### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

### 典型示例

```c
// 示例：int8_t类型非对齐搬入。
__simd_callee__ inline void asc_loadunalign(vector_int8_t& dst,
                                            vector_load_unalign& src0,
                                            __ubuf__ int8_t* src1)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype须与src1一致，搬入VL长度数据。 |
| src0 | 输入 | 非对齐寄存器。保存非32字节对齐的数据，该寄存器大小为32字节，须通过[asc_loadunalign_pre](asc_loadunalign_pre.md)预先初始化。 |
| src1 | 输入 | 源UB地址，实际读取地址无需32字节对齐。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- src1与dst的dtype必须完全一致，dtype不一致触发异常。
- `src1`起始地址无需32字节对齐，但实际访问范围须在UB地址空间内，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。
- 该接口必须与[asc_loadunalign_pre](asc_loadunalign_pre.md)配合使用。

## 调用示例

```cpp
vector_load_unalign ureg;
__ubuf__ half* src = (__ubuf__ half*)asc_get_phy_buf_addr(0);
uint32_t offset = 5;
asc_loadunalign_pre(ureg, src + offset);
vector_half dst;
asc_loadunalign(dst, ureg, src + offset);
```
