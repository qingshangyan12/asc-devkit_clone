# asc_storealign_1st_postupdate

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

将矢量数据寄存器`src`中的第一个元素（忽略mask）写入Unified Buffer（UB）中目的地址，单次搬出单个元素的数据，搬运过程中数据格式和内容保持不变。写入完成后启用Post Update模式自动累加目的地址指针`dst_align32b`，便于硬件循环内连续多次调用时无需手动维护目的地址。本接口在Vector Function（`__simd_vf__`标记的函数）内使用，源操作数为矢量数据寄存器，目的操作数为UB地址。本接口仅提供一种功能模式：忽略mask向`dst_align32b`搬出`src`第一个元素并启用Post Update。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

本接口对9种dtype暴露相同结构的原型（`<dtype>` ∈ {int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, half, float, bfloat16_t}，对应矢量数据寄存器类型`vector_<dtype>`）。模板原型如下：

```c
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ <dtype>*& dst_align32b,
                                                          vector_<dtype> src,
                                                          int32_t offset)
```

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float`、`bfloat16_t`。

以`int16_t`为例的典型调用原型如下：

```c
__simd_callee__ inline void asc_storealign_1st_postupdate(__ubuf__ int16_t*& dst_align32b,
                                                          vector_int16_t src,
                                                          int32_t offset)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst_align32b | 输入/输出 | 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致。 |
| src | 输入 | 源操作数（矢量数据寄存器），dtype须与dst_align32b一致。接口仅搬出src中第一个元素写入目的地址，其余元素忽略，不受mask影响。 |
| offset | 输入 | 地址偏移量，单位为元素个数。接口执行后将dst_align32b指针的底层UB地址向后偏移`offset × sizeof(dtype)`字节。通常取非负值用于向前推进；取0时dst_align32b指针不变，可重复向同一地址写入；取负值时dst_align32b指针向后回退，需确保回退后地址dtype对齐且不低于UB起始地址。实际有效范围受UB可用容量约束（见约束说明章节），超出UB区间会报错。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，src为矢量数据寄存器，dst_align32b为UB地址，UB地址空间外的指针不可作为dst_align32b传入。
- dst_align32b起始地址需按照dtype数据类型对齐。
- UB容量上限：UB总容量为256KB，默认预留6KB SIMD VF栈与2KB Ascend C预留空间后可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少。dst_align32b起始地址与Post Update累计偏移量之和不可超过实际可用容量，否则会报错。
- 单次搬出的数据量为单个元素（dtype字节大小为1/2/4时对应1/2/4字节），由本接口模式决定，不可由参数调整。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 单元素搬出与Post Update约束

- offset单位为元素个数。例如dtype为`int16_t`时offset=2表示dst_align32b指针底层UB地址向后偏移4字节；dtype为`float`时offset=1表示偏移4字节。
- offset取0时dst_align32b指针不变，dst仍按当前dst_align32b起始位置写入src第一个元素，可重复向同一地址写入同一值。
- offset取负值时dst_align32b指针向后回退，需确保回退后的地址仍按照dtype对齐且不低于UB起始地址。
- Post Update后的dst_align32b新地址仍需满足dtype对齐，否则下一次调用本接口会报错。
- Post Update仅更新dst_align32b指针，不影响src寄存器内容；多次调用时每次都搬出当前src第一个元素，若需搬出不同元素应先在src中更新元素或使用其他搬出接口。
- 本接口忽略mask；如需按mask过滤的有效元素搬出，应使用带mask参数的搬出接口。

## 调用示例

```cpp
__ubuf__ half* dst_align32b = (__ubuf__ half*)asc_get_phy_buf_addr(0);
vector_half src;
int32_t offset = 128;
asc_storealign_1st_postupdate(dst_align32b, src, offset);
```
