# Reg数据搬出概述

Reg数据搬出接口用于将矢量数据寄存器或掩码寄存器中的数据写入Unified Buffer（UB）。接口在Vector Function（`__simd_vf__`标记的函数）内使用，仅在AIV上生效。开发者可根据目的地址是否32字节对齐、是否需要压缩数据，以及是否需要自动更新目的地址，选择对应接口。

目的地址32字节对齐时，优先使用对齐搬出接口；目的地址不满足32字节对齐要求时，使用非对齐搬出接口，并按照对应模式调用后处理接口完成尾块搬出。各接口的数据类型、地址对齐和参数取值约束请参见对应接口说明。

## 矢量数据寄存器搬出

**表1**  矢量数据寄存器搬出接口

| 接口名称 | 适用场景 | 功能简述 |
| --- | --- | --- |
| [asc_storealign](asc_storealign.md) | 对齐搬出，支持连续、带偏移、地址寄存器偏移和非连续搬出。 | 将一个VL（256字节）的矢量数据寄存器数据搬出到32字节对齐的UB地址；支持通过掩码选择参与搬出的元素。 |
| [asc_storealign_postupdate](asc_storealign_postupdate.md) | 对齐搬出后需要自动更新目的地址。 | 将矢量数据寄存器数据连续或非连续搬出到32字节对齐的UB地址，并在搬出后更新目的地址指针。 |
| [asc_store](asc_store.md) | 连续搬出整个矢量数据寄存器或前`count`个元素。 | 将矢量数据寄存器数据搬出到按数据类型对齐的UB地址；在追求极致性能的对齐场景，推荐使用`asc_storealign`。 |
| [asc_storeunalign](asc_storeunalign.md) | 连续搬出到非32字节对齐的UB地址，需要由用户更新目的地址。 | 将前`count`个元素的主块搬出，并将尾块暂存至非对齐寄存器；需使用[asc_storeunalign_post](asc_storeunalign_post.md)完成尾块搬出。 |
| [asc_storeunalign_postupdate](asc_storeunalign_postupdate.md) | 连续搬出到非32字节对齐的UB地址，需要自动更新目的地址指针或地址寄存器。 | 将矢量数据寄存器主块搬出，并自动更新目的地址；立即数偏移模式需使用[asc_storeunalign_post_postupdate](asc_storeunalign_post_postupdate.md)收尾，地址寄存器偏移模式需使用`asc_storeunalign_post`收尾。 |
| [asc_storealign_pack](asc_storealign_pack.md) | 对齐压缩搬出，支持连续、带偏移和地址寄存器偏移。 | 按掩码将有效元素的低半部分bit数据压缩后搬出到32字节对齐的UB地址。 |
| [asc_storealign_pack_postupdate](asc_storealign_pack_postupdate.md) | 对齐压缩搬出后需要自动更新目的地址。 | 按掩码压缩搬出有效元素的低半部分bit数据，并在搬出后更新目的地址指针。 |
| [asc_storealign_pack_quarter](asc_storealign_pack_quarter.md) | 将32位元素的低8bit压缩搬出，支持连续、带偏移和地址寄存器偏移。 | 按掩码将有效元素的低8bit压缩后搬出到32字节对齐的UB地址。 |
| [asc_storealign_pack_quarter_postupdate](asc_storealign_pack_quarter_postupdate.md) | 将32位元素的低8bit压缩搬出后需要自动更新目的地址。 | 按掩码将有效元素的低8bit压缩搬出，并在搬出后更新目的地址指针。 |

## 掩码寄存器搬出

**表2**  掩码寄存器搬出接口

| 接口名称 | 适用场景 | 功能简述 |
| --- | --- | --- |
| [asc_storealign](asc_storealign.md) | 直接对齐搬出，支持连续、立即数偏移和地址寄存器偏移。 | 将32字节掩码寄存器数据直接搬出到32字节对齐的UB地址，不支持配置掩码。 |
| [asc_storealign_postupdate](asc_storealign_postupdate.md) | 直接对齐搬出后需要自动更新目的地址。 | 将32字节掩码寄存器数据直接搬出到32字节对齐的UB地址，并在搬出后更新目的地址指针。 |
| [asc_storealign_pack](asc_storealign_pack.md) | 对齐压缩搬出，支持连续、立即数偏移和地址寄存器偏移。 | 每间隔1bit丢弃1bit，将保留的bit连续压缩后搬出到32字节对齐的UB地址。 |
| [asc_storealign_pack_postupdate](asc_storealign_pack_postupdate.md) | 对齐压缩搬出后需要自动更新目的地址。 | 将掩码寄存器数据压缩搬出，并在搬出后更新目的地址指针。 |
| [asc_storeunalign_postupdate](asc_storeunalign_postupdate.md) | 非32字节对齐搬出。 | 对于`b16`和`b32`数据类型，提取掩码寄存器数据的最低有效位（LSB）并打包搬出；需使用[asc_storeunalign_post](asc_storeunalign_post.md)完成尾块搬出。 |

## 专用和后处理接口

**表3**  专用和后处理接口

| 接口名称 | 适用场景 | 功能简述 |
| --- | --- | --- |
| [asc_storealign_1st](asc_storealign_1st.md) | 对齐搬出矢量数据寄存器的首个元素。 | 将首个元素搬出到UB，支持连续、立即数偏移和地址寄存器偏移模式。 |
| [asc_storealign_1st_postupdate](asc_storealign_1st_postupdate.md) | 对齐搬出首个元素后需要自动更新目的地址。 | 将首个元素搬出到UB，并在搬出后更新目的地址指针。 |
| [asc_storealign_intlv](asc_storealign_intlv.md) | 对齐搬出两个矢量数据寄存器的数据。 | 按元素顺序交织两个矢量数据寄存器的数据后连续搬出，单次搬出量为`2 × VL`（512字节）。 |
| [asc_storeunalign_post](asc_storeunalign_post.md) | 非对齐搬出收尾。 | 将非对齐寄存器中暂存的尾块搬出到UB，配合`asc_storeunalign`或`asc_storeunalign_postupdate`使用。 |
| [asc_storeunalign_post_postupdate](asc_storeunalign_post_postupdate.md) | 非对齐搬出立即数偏移模式收尾后需要自动更新目的地址。 | 将非对齐寄存器中暂存的尾块搬出到UB，并在搬出后更新目的地址指针。 |

## 对齐连续搬出寻址方式对比

以下代码片段节选自[reg_load_store_align完整样例](../../../../../../../examples/02_simd_c_api/03_c_api/00_data_movement/reg_load_store_align)。四种方式均对两个长度为1024、数据类型为`half`的输入进行逐元素相加，并将结果连续写入UB。单次迭代处理128个元素，共迭代8次，最终结果均为`dst[i] = src0[i] + src1[i]`。

完整样例包含GM与UB之间的数据搬运、流水同步、核函数启动和结果校验；本节仅展示VF函数中的主要计算和搬出代码。样例的场景1、2、3和7已在Ascend 950PR（`dav-3510`，CANN 9.2.0）上通过golden校验，四个场景的输出结果逐字节一致。

**表4**  对齐连续搬出寻址方式对比

| 场景 | 搬出方式 | 目的地址维护方式 | 适用场景 |
| --- | --- | --- | --- |
| 场景1 | 用户手动偏移地址搬出 | 调用方通过指针运算计算每次搬出的目的地址。 | 目的地址偏移规则简单，且需要由用户显式控制地址。 |
| 场景7 | 通过接口立即数偏移搬出 | 将相对目的基地址的固定元素偏移作为`int32_t`参数传入。 | 偏移量在编译期确定，且无需修改目的地址指针。 |
| 场景3 | 地址寄存器偏移搬出 | 通过[asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md)生成`addr_reg`，由硬件在迭代中更新偏移。 | Hardware Loop内偏移随迭代变化。 |
| 场景2 | Post Update搬出 | `asc_storealign_postupdate`在每次搬出后自动更新目的地址指针。 | 连续搬出且无需用户手动维护目的地址。 |

### 用户手动偏移地址搬出

调用`asc_storealign`的连续对齐搬出模式。目的地址通过`dst + i * one_repeat_size`计算，`one_repeat_size`为128个`half`元素。

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
for (uint16_t i = 0; i < repeat_times; ++i) {
    asc_loadalign(src0_reg, src0 + i * one_repeat_size);
    asc_loadalign(src1_reg, src1 + i * one_repeat_size);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst + i * one_repeat_size, dst_reg, mask);
}
```

### 通过接口立即数偏移搬出

调用`asc_storealign`的立即数偏移搬出模式。每个`offset`均相对`dst`基地址，单位为元素；本样例中的8个固定偏移覆盖1024个`half`元素。

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
constexpr uint16_t REPEAT_TIMES = 8;
for (uint16_t i = 0; i < REPEAT_TIMES; ++i) {
    asc_loadalign(src0_reg, src0 + i * one_repeat_size);
    asc_loadalign(src1_reg, src1 + i * one_repeat_size);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    switch (i) {
        case 0:
            asc_storealign(dst, dst_reg, 0, mask);
            break;
        case 1:
            asc_storealign(dst, dst_reg, 128, mask);
            break;
        case 2:
            asc_storealign(dst, dst_reg, 256, mask);
            break;
        case 3:
            asc_storealign(dst, dst_reg, 384, mask);
            break;
        case 4:
            asc_storealign(dst, dst_reg, 512, mask);
            break;
        case 5:
            asc_storealign(dst, dst_reg, 640, mask);
            break;
        case 6:
            asc_storealign(dst, dst_reg, 768, mask);
            break;
        default:
            asc_storealign(dst, dst_reg, 896, mask);
            break;
    }
}
```

### 地址寄存器偏移搬出

调用`asc_storealign`的地址寄存器偏移搬出模式。`addr_reg`须在最内层循环中赋值并使用，偏移单位为元素。

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask;
addr_reg offset;
for (uint16_t i = 0; i < repeat_times; ++i) {
    offset = asc_update_addr_reg_b16(one_repeat_size);
    mask = asc_update_mask_b16(output_length);
    asc_loadalign(src0_reg, src0, offset);
    asc_loadalign(src1_reg, src1, offset);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst, dst_reg, offset, mask);
}
```

### Post Update搬出

调用`asc_storealign_postupdate`的立即数偏移搬出模式。目的地址以引用方式传入，接口在每次搬出后将其向后更新`one_repeat_size`个元素。

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask;
for (uint16_t i = 0; i < repeat_times; ++i) {
    mask = asc_update_mask_b16(output_length);
    asc_loadalign_postupdate(src0_reg, src0, one_repeat_size);
    asc_loadalign_postupdate(src1_reg, src1, one_repeat_size);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign_postupdate(dst, dst_reg, one_repeat_size, mask);
}
```
