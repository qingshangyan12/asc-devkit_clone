# Reg与UB数据搬运

Reg与UB数据搬运在AIV的Vector Function（`__simd_vf__`标记的函数）内完成。加载接口从UB读取数据到矢量数据寄存器、掩码寄存器或非对齐寄存器，存储接口将寄存器结果写回UB。GM不能直接访问SIMD Register File，数据需要先通过GM到UB通路进入UB。

DAV_3510的矢量数据寄存器长度（VL）为256字节，掩码寄存器长度为32字节。单个矢量寄存器可容纳的元素个数由数据位宽决定，例如b16为128个元素、b32为64个元素。

## 接口选择

### UB到Reg

| 场景 | 代表接口 | 说明 |
| --- | --- | --- |
| 连续对齐加载 | [asc_loadalign](../reg_compute/load/asc_loadalign.md) | 从32字节对齐地址加载一个VL，可使用立即数或地址寄存器偏移。 |
| 对齐加载并更新地址 | [asc_loadalign_postupdate](../reg_compute/load/asc_loadalign_postupdate.md) | 加载后自动更新源地址或地址寄存器。 |
| 按DataBlock步长加载 | [asc_loadalign_datablock_strided](../reg_compute/load/asc_loadalign_datablock_strided.md) | 按步长加载8个DataBlock。 |
| DataBlock或元素广播 | [asc_loadalign_brc_datablock](../reg_compute/load/asc_loadalign_brc_datablock.md)、[asc_loadalign_brc_elem](../reg_compute/load/asc_loadalign_brc_elem.md) | 把一个DataBlock或一个元素广播到矢量寄存器。 |
| 解交织、上下采样或解包加载 | [Reg数据搬入概述](../reg_compute/load/reg_load_overview.md#矢量数据寄存器搬入) | 加载时改变寄存器内排布。 |
| 连续非对齐易用加载 | [asc_load](../reg_compute/load/asc_load.md) | 源地址只需满足数据类型对齐。 |
| 连续非对齐高性能加载 | [asc_loadunalign_pre](../reg_compute/load/asc_loadunalign_pre.md)、[asc_loadunalign](../reg_compute/load/asc_loadunalign.md)、[asc_loadunalign_postupdate](../reg_compute/load/asc_loadunalign_postupdate.md) | 使用`vector_load_unalign`保存跨32字节边界的状态。 |
| 掩码寄存器加载 | [Reg数据搬入概述](../reg_compute/load/reg_load_overview.md#掩码寄存器搬入) | 支持直接、压缩或采样方式加载掩码。 |

### Reg到UB

| 场景 | 代表接口 | 说明 |
| --- | --- | --- |
| 连续或按步长对齐存储 | [asc_storealign](../reg_compute/store/asc_storealign.md) | 按`mask`写入一个VL，或按DataBlock步长写入。 |
| 对齐存储并更新地址 | [asc_storealign_postupdate](../reg_compute/store/asc_storealign_postupdate.md) | 写入后自动更新目的地址或地址寄存器。 |
| 首元素存储 | [asc_storealign_1st](../reg_compute/store/asc_storealign_1st.md) | 仅写入矢量寄存器的第一个元素，地址按数据类型对齐。 |
| 交织或压缩存储 | [Reg数据搬出概述](../reg_compute/store/reg_store_overview.md#矢量数据寄存器搬出) | 支持两个寄存器交织、半宽压缩和Quarter Pack。 |
| 连续非对齐存储 | [asc_storeunalign](../reg_compute/store/asc_storeunalign.md)、[asc_storeunalign_postupdate](../reg_compute/store/asc_storeunalign_postupdate.md) | 使用`vector_store_unalign`暂存不能立即写出的尾块。 |
| 非对齐尾块后处理 | [asc_storeunalign_post](../reg_compute/store/asc_storeunalign_post.md)、[asc_storeunalign_post_postupdate](../reg_compute/store/asc_storeunalign_post_postupdate.md) | 把非对齐寄存器中的最后一个尾块写回UB。 |
| 掩码寄存器存储 | [Reg数据搬出概述](../reg_compute/store/reg_store_overview.md#掩码寄存器搬出) | 支持直接、压缩或非对齐方式存储掩码。 |

寄存器类型及声明方式请参见[寄存器类型](../defs/type/reg_data_types.md)。

## 对齐搬运

`asc_loadalign`和`asc_storealign`通常从32字节对齐地址访问一个VL。调用方可通过以下方式维护循环地址：

- 直接计算每次调用的UB指针。
- 传入相对基地址的立即数偏移。
- 使用`addr_reg`描述Hardware Loop中的动态偏移。
- 使用Post Update接口在加载或存储后自动更新地址。

偏移单位随接口和寄存器类型变化。矢量数据偏移通常以元素为单位，掩码地址偏移可能以字节为单位，非连续模式还可能以DataBlock为单位，应以接口参数表为准。

## 非对齐加载

非对齐加载适用于有效起始地址满足数据类型对齐但不满足32字节对齐的场景：

1. 使用`asc_loadunalign_pre`按有效源地址初始化`vector_load_unalign`，硬件会访问向低地址方向对齐后的32字节块。
2. 使用`asc_loadunalign`或`asc_loadunalign_postupdate`拼接前置数据与后续读取数据，得到从有效地址开始的一个VL。
3. 连续Post Update加载可复用更新后的非对齐寄存器；源地址或偏移被修改后，需要重新执行预处理。

易用接口`asc_load`封装了连续非对齐加载。无论选择哪种形式，向低地址对齐后的实际读取范围也必须位于已分配UB空间内。

## 非对齐存储

非对齐主存储把可直接写入的完整块写入UB，并将末尾不足对齐块的数据暂存在`vector_store_unalign`中。连续存储时必须复用同一个非对齐寄存器；最后一次主存储后，通过匹配的后处理接口写出尾块。

| 主存储 | 后处理 | 地址维护方式 |
| --- | --- | --- |
| `asc_storeunalign` | `asc_storeunalign_post` | 调用方更新目的地址。 |
| `asc_storeunalign_postupdate`立即数模式 | `asc_storeunalign_post_postupdate` | 主接口自动更新目的指针。 |
| `asc_storeunalign_postupdate`地址寄存器模式 | `asc_storeunalign_post` | 主接口自动更新地址寄存器，后处理复用该寄存器。 |

若两次主存储的有效范围不连续，需要先完成前一次尾块的后处理，再开始新的非对齐序列。

## 掩码与有效元素

对齐存储通常使用`vector_bool`掩码控制哪些元素写入UB。尾块处理时可通过`asc_update_mask_b*`按剩余元素数更新掩码。掩码只控制有效元素，不会放宽实际访问地址的对齐和越界约束。

掩码寄存器本身也可在UB与Reg之间搬运。直接加载、存储通常处理32字节掩码；上采样、下采样和压缩模式的访问量、对齐要求及偏移单位不同，需要查阅具体接口文档。

## 地址冲突与同步

- Reg加载、计算和存储仅在Vector Function内使用，所有实际UB访问均不得超过分配范围。
- 多条Reg指令访问重叠UB区域且存在读写冲突时，使用[asc_mem_bar](../reg_compute/reg_sync/asc_mem_bar.md)保证顺序。
- PIPE_MTE2或PIPE_FIX写入UB后由Vector Function读取时，需要先完成对应流水到Vector执行的同步；Vector Function写入UB后由PIPE_MTE3读取时，也需要建立反向依赖。
- Post Update会修改指针或地址寄存器状态，循环展开、分支和提前退出时应保证状态更新次数与实际访问次数一致。
