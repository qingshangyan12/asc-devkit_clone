# asc_storealign_pack_postupdate

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/reg_compute/store/storealign.h"`。

将矢量数据寄存器中的数据压缩搬出到Unified Buffer（UB），写入完成后启用Post Update模式自动累加目的地址指针，便于Hardware Loop内连续多次调用时无需手动维护目的地址。`mask`用于指示参与搬出的元素，`mask`对应位置为1时，将有效元素的低半部分bit数据写入压缩后对应的目的位置；`mask`对应位置为0时，压缩后对应的目的位置保持原值。`offset`单位为元素，搬出完成后目的地址累加`offset × sizeof(dtype)`字节。

掩码寄存器压缩Post Update搬出请使用[asc_storealign_mask_pack_postupdate](asc_storealign_mask_pack_postupdate.md)。

以**b32位宽压缩至b16位宽搬出**过程为例，示意图如下：

**图1** 压缩搬出数据

![](../../../../figures/reg_store_pack.png)

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

### 矢量数据寄存器搬出模式

```c
// 占位符形式
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ <dtype>*& dst,
                                                           vector_<dtype> src,
                                                           int32_t offset,
                                                           vector_bool mask)
```

#### dtype支持数据类型

`dtype`支持的数据类型为`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`、`uint64_t`。对应矢量寄存器类型为`vector_<dtype>`。

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign_pack_postupdate(__ubuf__ uint32_t*& dst,
                                                           vector_uint32_t src,
                                                           int32_t offset,
                                                           vector_bool mask)
```

## 参数说明

### 矢量数据寄存器搬出模式

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输入/输出 | 目的操作数的起始地址，按指针引用传入。类型为`__ubuf__ <dtype>*&`，起始地址需32字节对齐。搬出完成后，该指针由硬件自动更新。 |
| src | 输入 | 源操作数（矢量数据寄存器）。`dtype`须与`dst`一致。 |
| offset | 输入 | 地址偏移量，类型为`int32_t`，单位为元素。接口执行后，目的地址累加`offset × sizeof(dtype)`字节。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。 |

矢量数据寄存器和作为执行掩码的掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 源操作数为矢量数据寄存器，目的操作数为UB地址。UB地址空间外的指针不可作为目的地址传入。
- UB总容量为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈和2KB Ascend C预留空间，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作为Data Cache，可用容量进一步减少）。目的操作数的写出范围和Post Update后的地址不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 矢量数据寄存器压缩搬出场景

- `dst`起始地址需32字节对齐。
- 未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- `mask`比特位为1的源操作数元素参与搬出，并将低半部分bit数据写入压缩后对应的目的位置；为0的元素不参与搬出，压缩后对应的目的位置保持原值。
- `offset`的单位为元素。Post Update后的`dst`新地址仍需32字节对齐，且本次写出范围和更新后的地址均需落在UB可用容量范围内。

## 调用示例

以下示例将一个`uint16_t`类型的矢量数据寄存器中有效元素的低8bit压缩搬出到UB，并在搬出后将目的地址更新128个`uint16_t`元素。

```c
__simd_vf__ inline void store_pack_postupdate(__ubuf__ uint16_t* output,
                                              __ubuf__ uint16_t* input)
{
    vector_uint16_t src;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    __ubuf__ uint16_t* dst = output;
    asc_loadalign(src, input);
    // 压缩搬出后将目的地址更新128个uint16_t元素。
    asc_storealign_pack_postupdate(dst, src, 128, mask);
}
```
