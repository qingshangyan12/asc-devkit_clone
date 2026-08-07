# asc_storealign_1st

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

将矢量数据寄存器中的首个元素搬出到Unified Buffer（UB），单次搬出量为一个元素。本接口忽略掩码，固定搬出矢量数据寄存器中第一个元素到`dst`起始地址处，搬运过程中数据格式与内容保持不变。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，源操作数为矢量数据寄存器，目的操作数为UB地址。

本接口提供三种参数列表不同的功能模式，用户按需选择对应重载：

- **无偏移搬出模式**：偏移固定为0，从矢量数据寄存器首个元素搬出到UB起始地址，由用户自行更新目的地址。
- **int32_t偏移搬出模式**：通过`int32_t offset`指定相对目的起始地址的元素偏移，用户可选择更新偏移或更新目的地址。
- **addr_reg偏移搬出模式**：通过地址寄存器`addr_reg offset`指定相对目的起始地址的偏移，常用于Hardware Loop内偏移随循环计数变化的对齐搬出场景。需要与`asc_create_addr_reg_b8`、`asc_create_addr_reg_b16`、`asc_create_addr_reg_b32`配合使用。

三种模式参数列表完全不同，用户必须按使用场景选择对应重载系列，不可混用。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 无偏移搬出模式

#### 模板原型（占位符形式）

本接口对16种dtype暴露相同结构的原型（`<dtype>`∈{int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, half, float, bfloat16_t, fp8_e4m3fn_t, fp8_e5m2_t, fp8_e8m0_t, hifloat8_t, fp4x2_e2m1_t, fp4x2_e1m2_t, int4b_t}，对应矢量寄存器类型`vector_<dtype>`，`int4b_t`对应`vector_int4x2_t`）。模板原型如下：

```c
__simd_callee__ inline void asc_storealign_1st(__ubuf__ <dtype>* dst_align32b,
                                               vector_<dtype> src)
```

#### 典型示例

```c
// 示例：int8_t类型无偏移搬出
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst_align32b,
                                               vector_int8_t src)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float`、`bfloat16_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`hifloat8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`

### int32_t偏移搬出模式

#### 模板原型（占位符形式）

本接口对16种dtype暴露相同结构的原型。模板原型如下：

```c
__simd_callee__ inline void asc_storealign_1st(__ubuf__ <dtype>* dst_align32b,
                                               vector_<dtype> src,
                                               int32_t offset)
```

#### 典型示例

```c
// 示例：int8_t类型int32_t偏移搬出
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst_align32b,
                                               vector_int8_t src,
                                               int32_t offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float`、`bfloat16_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`hifloat8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`

### addr_reg偏移搬出模式

#### 模板原型（占位符形式）

本接口对16种dtype暴露相同结构的原型。模板原型如下：

```c
__simd_callee__ inline void asc_storealign_1st(__ubuf__ <dtype>* dst_align32b,
                                               vector_<dtype> src,
                                               addr_reg offset)
```

#### 典型示例

```c
// 示例：int8_t类型addr_reg偏移搬出
__simd_callee__ inline void asc_storealign_1st(__ubuf__ int8_t* dst_align32b,
                                               vector_int8_t src,
                                               addr_reg offset)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float`、`bfloat16_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`hifloat8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`

## 参数说明

### 无偏移搬出模式

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst_align32b | 输出 | 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`。起始地址需dtype对齐。单次搬出量为一个元素（`sizeof(dtype)`字节），dst偏移+`sizeof(dtype)`不可超过UB可用容量。 |
| src | 输入 | 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。 |

### int32_t偏移搬出模式

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst_align32b | 输出 | 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`。起始地址需dtype对齐。 |
| src | 输入 | 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。 |
| offset | 输入 | 相对dst_align32b起始地址的偏移量，单位为元素。实际访问地址为`dst_align32b + offset`，起始地址需dtype对齐，且须落在UB地址范围内。 |

### addr_reg偏移搬出模式

**表 3**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst_align32b | 输出 | 目的操作数（矢量）的起始地址。类型为`__ubuf__ <dtype>*`。起始地址需dtype对齐。 |
| src | 输入 | 源操作数（矢量数据寄存器）。仅首个元素参与搬出，其余元素被忽略。 |
| offset | 输入 | 地址寄存器偏移，类型为`addr_reg`，由配套的地址寄存器构造接口预先构造。单位为元素。偏移后实际访问地址仍需dtype对齐，且须落在UB地址范围内。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为矢量数据寄存器，目的操作数为UB地址，UB地址空间外的指针不可作为dst_align32b传入。
- dst_align32b起始地址需dtype对齐。
- 本接口固定搬出矢量数据寄存器的首个元素，单次搬出数据量为`sizeof(dtype)`字节（dtype字节数为1/2/4时对应搬出1/2/4字节）。
- 本接口忽略掩码，不要求也不读取`vector_bool`掩码寄存器，掩码状态不影响搬出行为。
- UB容量上限：UB总容量256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。dst偏移+`sizeof(dtype)`字节不可超过实际可用容量。
- offset取值偏移后需要落在UB地址范围内，且仍需dtype对齐。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

```cpp
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
asc_storealign_1st(dst_align32b, src);
```
