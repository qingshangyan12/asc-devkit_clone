# Reg数据搬入概述

Reg数据搬入接口用于将Unified Buffer（UB）中的数据搬入矢量数据寄存器、掩码寄存器或非对齐寄存器。接口均仅在AIV上生效，且需在Vector Function（`__simd_vf__`标记的函数）内调用。

## 接口分类与选择

根据数据访问方式、搬入后的数据排布以及是否需要自动更新地址，选择相应接口。需要将读取地址随Hardware Loop的迭代变化时，可选择支持地址寄存器偏移的接口，相关说明请参见[地址寄存器偏移](#地址寄存器偏移)。

**表1**  Reg数据搬入接口分类

| 类别 | 适用场景 | 接口 |
| --- | --- | --- |
| 常规连续搬入 | 将连续数据搬入矢量数据寄存器或掩码寄存器。需要由用户更新源地址时使用`asc_loadalign`；需要接口自动更新源地址时使用`asc_loadalign_postupdate`。 | [asc_loadalign](asc_loadalign.md)<br>[asc_loadalign_postupdate](asc_loadalign_postupdate.md) |
| 非连续DataBlock搬入 | 单次从UB中按配置的DataBlock步长搬入8个非连续DataBlock。 | [asc_loadalign](asc_loadalign.md)<br>[asc_loadalign_postupdate](asc_loadalign_postupdate.md)<br>[asc_loadalign_datablock_strided](asc_loadalign_datablock_strided.md) |
| 掩码寄存器搬入 | 将UB中的数据搬入掩码寄存器，或在搬入过程中执行掩码上下采样。 | [asc_loadalign](asc_loadalign.md)<br>[asc_loadalign_postupdate](asc_loadalign_postupdate.md)<br>[asc_loadalign_mask](asc_loadalign_mask.md)<br>[asc_loadalign_downsample](asc_loadalign_downsample.md)<br>[asc_loadalign_mask_downsample](asc_loadalign_mask_downsample.md)<br>[asc_loadalign_upsample](asc_loadalign_upsample.md)<br>[asc_loadalign_mask_upsample](asc_loadalign_mask_upsample.md)<br>[asc_loadalign_upsample_postupdate](asc_loadalign_upsample_postupdate.md) |
| 广播搬入 | 将一个DataBlock、一个元素，或连续8个元素分别广播到矢量数据寄存器。带`postupdate`后缀的接口在搬入后自动更新源地址。 | [asc_loadalign_brc_datablock](asc_loadalign_brc_datablock.md)<br>[asc_loadalign_brc_datablock_postupdate](asc_loadalign_brc_datablock_postupdate.md)<br>[asc_loadalign_brc_elem](asc_loadalign_brc_elem.md)<br>[asc_loadalign_brc_elem_postupdate](asc_loadalign_brc_elem_postupdate.md)<br>[asc_loadalign_brc_elem2datablock](asc_loadalign_brc_elem2datablock.md)<br>[asc_loadalign_brc_elem2datablock_postupdate](asc_loadalign_brc_elem2datablock_postupdate.md) |
| 排布变换搬入 | 在搬入时完成解交织、下采样、上采样或解包。带`postupdate`后缀的接口在搬入后自动更新源地址。 | [asc_loadalign_deintlv](asc_loadalign_deintlv.md)<br>[asc_loadalign_deintlv_postupdate](asc_loadalign_deintlv_postupdate.md)<br>[asc_loadalign_downsample](asc_loadalign_downsample.md)<br>[asc_loadalign_downsample_postupdate](asc_loadalign_downsample_postupdate.md)<br>[asc_loadalign_upsample](asc_loadalign_upsample.md)<br>[asc_loadalign_upsample_postupdate](asc_loadalign_upsample_postupdate.md)<br>[asc_loadalign_unpack](asc_loadalign_unpack.md)<br>[asc_loadalign_unpack_postupdate](asc_loadalign_unpack_postupdate.md)<br>[asc_loadalign_unpack4](asc_loadalign_unpack4.md)<br>[asc_loadalign_unpack4_postupdate](asc_loadalign_unpack4_postupdate.md) |
| 连续非对齐搬入 | 需要从非32字节对齐的地址开始连续读取数据时，先初始化非对齐寄存器，再执行搬入。需要自动更新源地址和非对齐寄存器时使用`asc_loadunalign_postupdate`。 | [asc_load](asc_load.md)<br>[asc_loadunalign_pre](asc_loadunalign_pre.md)<br>[asc_loadunalign](asc_loadunalign.md)<br>[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md) |

`asc_load`为易用性接口。对性能有要求的连续非对齐搬入场景，可使用`asc_loadunalign`或`asc_loadunalign_postupdate`。

## 通用约束

所有接口的实际读取范围必须位于UB地址空间内且不越界。UB地址与其他指令存在重叠时，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证相关指令串行化。UB总容量及受编译选项、SIMD与SIMT混合编程影响的可用容量，请参见[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。各接口的地址对齐要求如下。

**表2**  Reg数据搬入接口的地址对齐要求

| 实际读取地址对齐要求 | 涉及接口 | 说明 |
| --- | --- | --- |
| 32字节 | [asc_loadalign](asc_loadalign.md)、[asc_loadalign_postupdate](asc_loadalign_postupdate.md)、[asc_loadalign_datablock_strided](asc_loadalign_datablock_strided.md)、[asc_loadalign_mask](asc_loadalign_mask.md)、[asc_loadalign_brc_datablock](asc_loadalign_brc_datablock.md)、[asc_loadalign_brc_datablock_postupdate](asc_loadalign_brc_datablock_postupdate.md)、[asc_loadalign_deintlv](asc_loadalign_deintlv.md)、[asc_loadalign_deintlv_postupdate](asc_loadalign_deintlv_postupdate.md)、[asc_loadalign_downsample](asc_loadalign_downsample.md)、[asc_loadalign_mask_downsample](asc_loadalign_mask_downsample.md)、[asc_loadalign_downsample_postupdate](asc_loadalign_downsample_postupdate.md)、[asc_loadalign_unpack](asc_loadalign_unpack.md)、[asc_loadalign_unpack_postupdate](asc_loadalign_unpack_postupdate.md)、[asc_loadalign_unpack4](asc_loadalign_unpack4.md)、[asc_loadalign_unpack4_postupdate](asc_loadalign_unpack4_postupdate.md) | 非连续对齐搬入中，某个DataBlock的掩码元素均无效时，该DataBlock即使越界也不会报错，具体以接口约束为准。 |
| 由目的操作数决定：矢量数据寄存器为32字节，掩码寄存器为16字节 | [asc_loadalign_upsample](asc_loadalign_upsample.md)<br>[asc_loadalign_upsample_postupdate](asc_loadalign_upsample_postupdate.md) | 掩码寄存器搬入时，地址偏移的单位为字节。 |
| 16字节 | [asc_loadalign_mask_upsample](asc_loadalign_mask_upsample.md) | 接口将读取的掩码bit上采样两倍。 |
| 按dtype对齐 | [asc_loadalign_brc_elem](asc_loadalign_brc_elem.md)<br>[asc_loadalign_brc_elem_postupdate](asc_loadalign_brc_elem_postupdate.md)<br>[asc_load](asc_load.md)<br>[asc_loadunalign_pre](asc_loadunalign_pre.md)<br>[asc_loadunalign](asc_loadunalign.md)<br>[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md) | 非对齐搬入接口会在内部按32字节边界读取前置数据；除传入地址外，对齐后的读取范围也必须在UB地址空间内。 |
| b16为16字节，b32为32字节 | [asc_loadalign_brc_elem2datablock](asc_loadalign_brc_elem2datablock.md)<br>[asc_loadalign_brc_elem2datablock_postupdate](asc_loadalign_brc_elem2datablock_postupdate.md) | 连续读取8个元素，并将每个元素广播到一个DataBlock。 |

对于带`postupdate`后缀的接口，还需按照接口页面的约束检查更新后的源地址或下一次实际读取地址的对齐要求。

## 关键使用说明

### b64数据类型

以下接口支持b64数据类型。

**表3**  支持b64数据类型的接口

| 支持的数据类型 | 接口 |
| --- | --- |
| `int64_t`、`uint64_t` | [asc_loadalign](asc_loadalign.md)<br>[asc_loadalign_postupdate](asc_loadalign_postupdate.md) |
| `int64_t` | [asc_load](asc_load.md)<br>[asc_loadunalign_pre](asc_loadunalign_pre.md)<br>[asc_loadunalign](asc_loadunalign.md)<br>[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md) |

当`asc_loadalign`、`asc_loadunalign_pre`或`asc_loadunalign_postupdate`通过地址寄存器访问b64数据时，地址寄存器更新接口不支持b64模式，应使用[asc_update_addr_reg_b32](../reg_addr_reg/asc_update_addr_reg.md)生成地址寄存器。一个b64元素占用两个b32寻址单元，因此各维偏移量以及`asc_loadunalign_postupdate`的`inc`应设置为期望b64元素偏移量的2倍。

`asc_loadalign`和`asc_loadalign_postupdate`的非连续对齐搬入模式中，处理`int64_t`数据时，掩码创建接口不支持b64模式。应先使用[asc_create_mask_b32](../reg_mask/asc_create_mask.md)创建b32掩码，再使用[asc_unpack_lower](../reg_permute_sel/asc_unpack.md)将其展开为b64数据对应的掩码。展开后的掩码以连续8个bit为一组，仅每组最低位的bit有效，用于控制一个b64元素。

以下示例以`asc_loadalign`为例，展示b64掩码的构造方式以及地址寄存器偏移量的换算方法。

```cpp
vector_int64_t src_reg;
vector_bool mask_b32 = asc_create_mask_b32(PAT_ALL);
vector_bool mask_b64;
asc_unpack_lower(mask_b64, mask_b32);
addr_reg load_offset;
addr_reg store_offset;
for (uint16_t i = 0; i < repeatTimes; ++i) {
    load_offset = asc_update_addr_reg_b32(elements_vl * 2);
    // 搬出到UB后数据类型为B32，偏移不需要*2
    store_offset = asc_update_addr_reg_b32(elements_vl);
    asc_loadalign(src_reg, input, load_offset);
    asc_storealign_pack(output, src_reg, store_offset, mask_b64);
}
```

### 地址寄存器偏移

地址寄存器用于保存相对UB源地址的元素偏移，适用于Hardware Loop中读取位置随循环迭代变化的场景。使用前需通过[asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md)生成地址寄存器。b8、b16、b32模式下，一个寻址单元分别对应1字节、2字节、4字节。

对于b64数据，请参见[b64数据类型](#b64数据类型)中的示例：`load_offset`使用b32寻址单位生成，偏移量为实际b64元素偏移量的2倍。

**表4**  支持地址寄存器偏移的接口

| 使用场景 | 接口 |
| --- | --- |
| 常规对齐搬入 | [asc_loadalign](asc_loadalign.md) |
| 广播搬入 | [asc_loadalign_brc_datablock](asc_loadalign_brc_datablock.md)<br>[asc_loadalign_brc_elem](asc_loadalign_brc_elem.md)<br>[asc_loadalign_brc_elem2datablock](asc_loadalign_brc_elem2datablock.md) |
| 排布变换搬入 | [asc_loadalign_deintlv](asc_loadalign_deintlv.md)<br>[asc_loadalign_downsample](asc_loadalign_downsample.md)<br>[asc_loadalign_unpack](asc_loadalign_unpack.md)<br>[asc_loadalign_unpack4](asc_loadalign_unpack4.md)<br>[asc_loadalign_upsample](asc_loadalign_upsample.md) |
| 非对齐搬入的预处理和自动更新 | [asc_loadunalign_pre](asc_loadunalign_pre.md)<br>[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md) |

`asc_loadunalign_pre`与`asc_loadunalign_postupdate`的地址寄存器偏移模式必须配合使用：每次循环迭代先生成本次迭代的起始偏移，再使用相同的源地址和偏移初始化非对齐寄存器，然后执行首次搬入。后续搬入仅在继续使用接口更新后的偏移时才可复用非对齐寄存器；源地址或偏移被另行修改后，必须重新初始化。

## 非对齐搬运特性

非对齐搬运用于有效起始地址满足dtype对齐、但不满足32字节对齐的连续数据搬入或搬出场景。搬入和搬出均通过[非对齐寄存器](../../defs/type/data_type_definition.md#非对齐寄存器)保存跨32字节边界的数据，以保证连续数据的完整性。

### 非对齐搬入原理

非对齐搬入通过非对齐寄存器缓存起始地址所在32字节块的前置数据，再与后续读取的数据拼接，得到从有效起始地址开始的VL长度数据。

**图1**  非对齐搬入示意图<a id="fig-loadunalign"></a>

![非对齐搬入示意图](../../../../figures/reg_loadunalign.png "非对齐搬入示意图")

设有效起始字节地址为`src_start_addr`，将其向低地址方向按32字节对齐，得到`aligned_src_start_addr`。处理过程如下：

1. 调用[asc_loadunalign_pre](asc_loadunalign_pre.md)，将`[aligned_src_start_addr, aligned_src_start_addr + 32)`范围内的数据缓存至非对齐寄存器。
2. 调用[asc_loadunalign](asc_loadunalign.md)或[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)，将前置缓存与后续读取数据拼接，得到`[src_start_addr, src_start_addr + VL)`范围的数据并写入矢量数据寄存器。`asc_loadunalign_postupdate`还会更新非对齐寄存器，以缓存下一次连续搬入所需的前置数据。

实际访问会覆盖向低地址方向对齐后的32字节范围，因此除有效起始地址满足dtype对齐外，对齐后的读取范围也必须位于UB地址空间内。

**表5**  非对齐搬入接口的配合关系

| 搬入方式 | 调用顺序与缓存复用规则 | 接口 |
| --- | --- | --- |
| 手动更新源地址 | 每次搬入前均调用预处理接口，且预处理接口与紧随其后的搬入接口必须使用相同的源地址。 | [asc_loadunalign_pre](asc_loadunalign_pre.md) -> [asc_loadunalign](asc_loadunalign.md) |
| 连续自动更新 | 首次搬入前调用预处理接口。后续搬入继续使用接口自动更新后的源地址时，可复用更新后的非对齐寄存器；源地址被另行修改后，必须重新预处理。 | [asc_loadunalign_pre](asc_loadunalign_pre.md) -> [asc_loadunalign_postupdate](asc_loadunalign_postupdate.md) |
| 地址寄存器偏移 | 每次循环迭代先生成起始偏移，再使用相同的源地址和偏移预处理。迭代内继续使用接口按`inc`更新后的偏移时，可复用非对齐寄存器；源地址或偏移被另行修改后，必须重新预处理。 | [asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md) -> [asc_loadunalign_pre](asc_loadunalign_pre.md) -> [asc_loadunalign_postupdate](asc_loadunalign_postupdate.md) |

### 非对齐搬出原理

非对齐搬出将矢量数据寄存器中的连续数据写入非32字节对齐的UB地址。每次主搬出会将可以直接写入的主块写入UB，并将末尾不能构成32字节对齐块的数据保存在非对齐寄存器。后续连续搬出时，接口将该尾块与本次主块的起始数据拼接后写入UB，并更新本次尾块。

首次搬出前，非对齐寄存器无需初始化。连续调用时，必须复用同一个非对齐寄存器；若上一次尾块与本次主块不连续，则需要在下一次主搬出前调用后处理接口完成前一次尾块的写入。

**图2**  非对齐搬出示意图（首次搬出）<a id="fig-storeunalign-empty"></a>

![非对齐搬出示意图（首次搬出）](../../../../figures/reg_storeunalign_1.png "非对齐搬出示意图（首次搬出）")

首次搬出时，非对齐寄存器中没有前序尾块数据。主搬出接口将矢量数据寄存器中可直接写入的数据写入UB，并将末尾数据保存至非对齐寄存器。调用后处理接口后，尾块数据被写入UB。

**图3**  非对齐搬出示意图（连续搬出）<a id="fig-storeunalign-cached"></a>

![非对齐搬出示意图（连续搬出）](../../../../figures/reg_storeunalign_2.png "非对齐搬出示意图（连续搬出）")

后续连续搬出时，主搬出接口将非对齐寄存器中的前序尾块与本次矢量数据寄存器的主块数据拼接后写入UB，再将本次尾块更新至非对齐寄存器。连续搬出结束后，调用后处理接口写入最后一次缓存的尾块。

对于需要压缩掩码选中元素并连续搬出的场景，可使用`asc_squeeze_and_storeunalign`。该接口使用AR特殊寄存器记录相对目的地址的累计字节偏移，并将未满32字节的尾块保存在非对齐寄存器。每组连续搬出开始前需清零AR特殊寄存器，连续调用期间保持目的地址和非对齐寄存器不变，最后调用收尾接口写入尾块。

**表6**  非对齐搬出接口的配合关系

| 主搬出方式 | 配套后处理接口 | 说明 |
| --- | --- | --- |
| [asc_storeunalign](../store/asc_storeunalign.md) | [asc_storeunalign_post](../store/asc_storeunalign_post.md) | 用户手动更新目的地址。 |
| [asc_storeunalign_postupdate](../store/asc_storeunalign_postupdate.md)的立即数偏移模式 | [asc_storeunalign_post_postupdate](../store/asc_storeunalign_post_postupdate.md) | 接口自动更新目的地址。 |
| [asc_storeunalign_postupdate](../store/asc_storeunalign_postupdate.md)的地址寄存器偏移模式 | [asc_storeunalign_post](../store/asc_storeunalign_post.md) | 主搬出和后处理接口需复用同一个非对齐寄存器及地址寄存器。 |
| 压缩掩码选中元素并连续搬出 | [asc_squeeze_and_storeunalign_init](../reg_permute_sel/asc_squeeze_and_storeunalign_init.md) -> [asc_squeeze_and_storeunalign](../reg_permute_sel/asc_squeeze_and_storeunalign.md) -> [asc_squeeze_and_storeunalign_finalize](../reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md) | 使用AR特殊寄存器维护累计字节偏移；连续调用主接口时保持目的地址和非对齐寄存器不变。 |

### 非对齐搬入搬出示例

下图以`uint32_t`数据为例，展示连续非对齐搬入后再连续非对齐搬出的过程。搬入前置初始化和搬出后处理均位于循环外：连续搬入时，`asc_loadunalign_postupdate`更新的非对齐寄存器可供下一次搬入使用；连续搬出时，`asc_storeunalign_postupdate`将尾块保存在非对齐寄存器，最后一次循环结束后再统一写回。

**图4**  连续非对齐搬入搬出示例<a id="fig-loadstoreunalign"></a>

![连续非对齐搬入搬出示例](../../../../figures/reg_loadstoreunalign.png "连续非对齐搬入搬出示例")

```cpp
__simd_vf__ inline void LoadStoreUnAlign(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
                                         uint32_t count, uint16_t repeat_times)
{
    vector_load_unalign load_unalign_reg;
    vector_store_unalign store_unalign_reg;
    vector_uint32_t data_reg;
    // 非对齐搬入初始化，只需在循环开始前调用一次。
    asc_loadunalign_pre(load_unalign_reg, src);
    for (uint16_t i = 0; i < repeat_times; ++i) {
        asc_loadunalign_postupdate(data_reg, load_unalign_reg, src, count);
        asc_storeunalign_postupdate(dst, store_unalign_reg, data_reg, count);
    }
    // 非对齐搬出后处理，只需在循环结束后调用一次。
    asc_storeunalign_post_postupdate(dst, store_unalign_reg, 0);
}
```

搬运步骤如下：

1. 循环开始前，`asc_loadunalign_pre`将源地址所在32字节块的前置数据缓存到`load_unalign_reg`，为首次非对齐搬入准备数据。
2. 每次循环中，`asc_loadunalign_postupdate`将`load_unalign_reg`中的前置数据与后续读取数据拼接为一个VL长度的矢量数据寄存器，并更新`load_unalign_reg`和`src`，供下一次连续搬入使用。
3. 随后，`asc_storeunalign_postupdate`将`data_reg`中的主块数据写入UB，并将末尾尾块缓存到`store_unalign_reg`，同时更新`dst`。下一次连续搬出会将该尾块与本次主块数据拼接后写入UB。
4. 循环结束后，`asc_storeunalign_post_postupdate`将`store_unalign_reg`中最后缓存的尾块写入UB，完成连续非对齐搬出。

示例中`count`为每次搬运的元素个数。`src`和`dst`分别由搬入、搬出接口自动更新，循环内需要复用各自的非对齐寄存器。`count`不超过单个矢量数据寄存器可容纳的元素个数，且搬入、搬出地址范围必须位于实际可用UB空间内。
