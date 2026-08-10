# asc_loadalign_brc_elem2datablock_postupdate

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

从Unified Buffer（UB）连续读取8个元素，并将第i个源元素广播到目的矢量数据寄存器的第i个DataBlock（32字节）中。b16类型的实际读取地址需16字节对齐，b32类型的实际读取地址需32字节对齐。搬运完成后启用Post Update模式，自动将传入的源地址指针向后偏移`offset * sizeof(dtype)`字节，无需手动偏移地址。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型（占位符形式）

```c
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(vector_<dtype>& dst,
                                                                        __ubuf__ <dtype>*& src,
                                                                        int32_t offset)
```

### dtype支持数据类型

dtype支持的数据类型为`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：b16重载，读取N个half元素并分别广播到对应DataBlock。
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(vector_half& dst,
                                                                        __ubuf__ half*& src,
                                                                        int32_t offset)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype须与src一致，搬入VL长度数据。 |
| src | 输入/输出 | 源UB地址。从`src`起始位置连续读取8个元素，b16类型的实际读取地址需16字节对齐，b32类型的实际读取地址需32字节对齐。 |
| offset | 输入 | Post Update步长，单位为元素个数。接口使用当前`src`地址完成搬入后，将`src`指针增加`offset * sizeof(dtype)`字节。`offset`为负值表示源地址向低地址方向移动。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- src与dst的dtype必须完全一致，dtype不一致触发异常。
- b16类型的实际读取地址和偏移后地址需16字节对齐，b32类型的实际读取地址和偏移后地址需32字节对齐，并且在UB地址空间内且不越界，否则会报错。
- offset取0时，接口仍从当前src地址搬入数据，src指针保持不变，可重复读取同一组源数据。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

```cpp
vector_half dst;
__ubuf__ half* src;
asc_loadalign_brc_elem2datablock_postupdate(dst, src, 0);
```
