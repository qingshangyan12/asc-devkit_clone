# asc_loadunalign_pre

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

将Unified Buffer（UB）中从任意字节地址开始的32字节数据搬入非对齐寄存器[vector_load_unalign](../reg_data_types/data_type_definition.md#非对齐寄存器)，作为非对齐搬入的预处理阶段。本接口需要与[asc_loadunalign](asc_loadunalign.md)或[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)配合使用。本接口提供两种功能模式：

- 无偏移预处理搬入：从`src`起始地址搬入数据。
- 带地址寄存器偏移预处理搬入：通过`addr_reg`类型的地址寄存器指定起始地址偏移并搬入数据。需要与[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)配合使用。接口不会修改`src`指针或按值传入的`offset`。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 无偏移预处理搬入

#### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ <dtype>* src)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

#### 典型示例

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ float* src)
```

### 带地址寄存器偏移预处理搬入

#### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ <dtype>* src,
                                                addr_reg offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

#### 典型示例

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ float* src,
                                                addr_reg offset)
```

## 参数说明

**表1** 参数说明（无偏移预处理搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的非对齐寄存器。搬入32字节数据。 |
| src | 输入 | 源UB地址。起始地址无需32字节对齐。 |

**表2** 参数说明（带地址寄存器偏移预处理搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的非对齐寄存器。搬入32字节数据。 |
| src | 输入 | 源UB地址。起始地址无需32字节对齐。 |
| offset | 输入 | 地址寄存器，类型为`addr_reg`，须通过[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)生成。该寄存器记录的实际字节偏移由创建地址寄存器时选择的位宽模式决定：b8、b16、b32模式分别以1字节、2字节、4字节为偏移单位。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- 实际访问地址在接口内部向低地址方向对齐到32字节边界后读取数据。传入的`src`或`src`与`offset`确定的地址无需32字节对齐，对齐后的32字节读取范围须在UB地址空间内，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。
- 该接口必须与[asc_loadunalign](asc_loadunalign.md)或[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)配合使用。

## 调用示例

```cpp
vector_load_unalign dst;
__ubuf__ int8_t* src;
asc_loadunalign_pre(dst, src);
```
