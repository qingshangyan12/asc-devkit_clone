# asc_store

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

将矢量数据寄存器中的数据搬出到Unified Buffer（UB），不区分目的地址是否32字节对齐。底层经非对齐搬出指令完成单次连续写入，搬运过程中数据格式和内容保持不变，在追求极致性能时，对齐场景推荐使用[asc_storealign](asc_storealign.md)接口。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，源操作数为矢量数据寄存器，目的操作数为UB地址。本接口提供两种参数列表不同的功能模式，用户按使用场景选择对应重载：

- **连续搬出**：将整个矢量数据寄存器内容写入`dst`起始地址，搬出量为一个矢量长度（VL，默认256字节）。
- **带搬出数量**：仅将矢量数据寄存器前`count`个元素写入`dst`起始地址，用于部分搬出场景。

本接口在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 连续搬出

#### 模板原型（占位符形式）

```cpp
__simd_callee__ inline void asc_store(__ubuf__ <dtype>* dst,
                                      vector_<dtype> src)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。对应矢量寄存器类型`vector_<dtype>`，`int4b_t`对应`vector_int4x2_t`。

#### 典型示例

```cpp
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst,
                                      vector_int8_t src)
```

### 带搬出数量

本接口对16种dtype暴露相同结构的原型，模板原型如下：

#### 模板原型（占位符形式）

```cpp
__simd_callee__ inline void asc_store(__ubuf__ <dtype>* dst,
                                      vector_<dtype> src,
                                      uint32_t count)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`int16_t`、`uint16_t`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。对应矢量寄存器类型`vector_<dtype>`，`int4b_t`对应`vector_int4x2_t`。

#### 典型示例

```cpp
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst,
                                      vector_int8_t src,
                                      uint32_t count)
```

## 参数说明

### 连续搬出

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址可非32字节对齐（本接口为非对齐搬出），但须落在UB地址空间内，且`dst`起始地址+VL字节不可越过UB可用容量上限。 |
| src | 输入 | 源矢量数据寄存器，dtype决定元素类型，元素个数=VL/sizeof(dtype)。VL默认256字节。 |

### 带搬出数量

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数（矢量）的起始地址，类型为`__ubuf__ <dtype>*`。地址可非32字节对齐（本接口为非对齐搬出），但须落在UB地址空间内，且`dst`起始地址+count×sizeof(dtype)字节不可越过UB可用容量上限。 |
| src | 输入 | 源矢量数据寄存器，dtype须与dst一致。仅前`count`个元素参与搬出，其余元素被忽略。 |
| count | 输入 | 实际搬出的元素个数。不可大于矢量数据寄存器容纳的最大元素个数VL/sizeof(dtype)。传入量大于寄存器容纳上限时，超出寄存器范围的元素无有效数据，写入dst的内容不确定；建议按需取[0, VL/sizeof(dtype)]范围内有效值。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为矢量数据寄存器，目的操作数为UB地址，UB地址空间外的指针不可作为dst传入。
- dst起始地址可非32字节对齐；本接口按非对齐方式将矢量数据寄存器内容写入UB，不要求对齐，不会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。dst偏移+搬出字节数不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 连续搬出约束

- 搬出量固定为VL字节（默认256字节），不可由本接口参数调整；dst写入元素个数为VL/sizeof(dtype)。

### 带搬出数量约束

- count的取值范围为[0, VL/sizeof(dtype)]，VL为矢量长度。超出该范围会导致未定义行为。偏移后不可超过实际UB可用容量，否则会报错。

## 调用示例

```cpp
vector_half src;
__ubuf__ half* dst = (__ubuf__ half*)asc_get_phy_buf_addr(0);
asc_store(dst, src);
```
