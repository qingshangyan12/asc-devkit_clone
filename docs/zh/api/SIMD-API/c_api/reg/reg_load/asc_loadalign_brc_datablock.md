# asc_loadalign_brc_datablock

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

头文件路径：`"c_api/reg_compute/reg_load.h"`。

对齐数据搬运接口，从UB连续对齐搬入目的操作数，实现DataBlock广播搬入模式：读取一个DataBlock（32B），并广播到矢量数据寄存器（256B）。本接口提供三种功能模式：

- **连续对齐搬入**：dst为矢量数据寄存器，从`src`起始地址搬入VL长度数据。
- **带偏移搬入**：从`src`起始地址加上`offset`偏移位置开始搬入。dst为矢量数据寄存器，搬入VL长度数据。
- **带地址寄存器偏移搬入**：通过`addr_reg`类型的地址寄存器偏移指定起始位置开始搬入。dst为矢量数据寄存器，搬入VL长度数据。需要与[地址寄存器构造接口](../reg_data_types/asc_update_addr_reg.md)配合使用。

## 函数原型

### 连续对齐搬入

```c
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_<dtype>& dst,
                                                        __ubuf__ <dtype>* src)
```

dtype支持的数据类型为`bfloat16_t`、`float`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`half`、`hifloat8_t`、`int16_t`、`int32_t`、`int4b_t`、`int8_t`、`uint16_t`、`uint32_t`、`uint8_t`

```c
// 示例：从 UB 读取 32 字节 half DataBlock 并广播填充 half 矢量寄存器
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst,
                                                        __ubuf__ half* src)
```

### 带偏移搬入

```c
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_<dtype>& dst,
                                                        __ubuf__ <dtype>* src,
                                                        int32_t offset)
```

dtype支持的数据类型为`bfloat16_t`、`float`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`half`、`hifloat8_t`、`int16_t`、`int32_t`、`int4b_t`、`int8_t`、`uint16_t`、`uint32_t`、`uint8_t`

```c
// 示例：从 UB 偏移 32 字节处读取 half DataBlock 并广播填充
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst,
                                                        __ubuf__ half* src,
                                                        int32_t offset)
```

### 带地址寄存器偏移搬入

```c
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_<dtype>& dst,
                                                        __ubuf__ <dtype>* src,
                                                        addr_reg offset)
```

dtype支持的数据类型为`bfloat16_t`、`float`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`half`、`hifloat8_t`、`int16_t`、`int32_t`、`int4b_t`、`int8_t`、`uint16_t`、`uint32_t`、`uint8_t`

```c
// 示例：按地址寄存器偏移从 UB 读取 half DataBlock 并广播填充
__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst,
                                                        __ubuf__ half* src,
                                                        addr_reg offset)
```

## 参数说明

**表1** 参数说明（连续对齐搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype必须和src一致，搬入VL长度数据。|
| src | 输入 | 源UB地址，起始地址需32字节对齐。 |

**表2** 参数说明（带偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype必须和src一致，搬入VL长度数据。 |
| src | 输入 | 源UB地址，起始地址需32字节对齐。 |
| offset | 输入 | 相对`src`起始地址的偏移，偏移后的地址仍需32字节对齐，且须在UB地址范围内，单位为元素个数。|

**表3** 参数说明（带地址寄存器偏移搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype必须和src一致，搬入VL长度数据。 |
| src | 输入 | 源UB地址，起始地址需32字节对齐。 |
| offset | 输入 | 地址寄存器偏移，类型为`addr_reg`，地址偏移量，单位为元素个数。偏移后的地址仍需32字节对齐，且须在UB地址范围内。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- src起始地址需32字节对齐，否则会报错。
- src与dst的dtype必须完全一致，dtype不一致触发异常。
- 一次读取数据量为32字节（1个DataBlock），实际读取地址 + 32字节不可超出UB可用容量，否则会报错。UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。src偏移 + 单次搬入数据量不可超过实际可用容量，否则会报错。
- 如果需要执行多条本接口指令，且源地址存在重叠，需通过同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)保证串行化，防止读到非预期数据。

### 带偏移搬入约束

- offset单位为元素个数，offset取值需使`src + offset × sizeof(dtype)`在UB地址范围内，且`src + offset × sizeof(dtype)`仍需32字节对齐，否则触发搬运异常。
- offset可为负值，但 `src + offset × sizeof(dtype)` 不可小于UB起始地址0，否则会报错。

### 带地址寄存器偏移搬入约束

- 必须先调用[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)接口。
- offset单位为元素个数，offset取值需使`src + offset × sizeof(dtype)`在UB地址范围内，且`src + offset × sizeof(dtype)`仍需32字节对齐，否则触发搬运异常。

## 调用示例

```cpp
vector_half dst;
__ubuf__ half* src;
asc_loadalign_brc_datablock(dst, src);
```
