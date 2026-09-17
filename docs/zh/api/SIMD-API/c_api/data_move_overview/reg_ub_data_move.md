# Reg与UB数据搬运

Reg与UB数据搬运在AIV的Vector Function（`__simd_vf__`标记的函数）内完成。加载接口从UB读取数据到矢量数据寄存器、掩码寄存器或非对齐寄存器；存储接口将寄存器结果写回UB。GM不能直接访问Reg，必须先经过GM到UB通路。

## 接口选择

| 场景 | 推荐入口 | 代表接口 |
| --- | --- | --- |
| 对齐加载 | [Reg数据搬入概述](../reg_compute/load/reg_load_overview.md) | [asc_loadalign](../reg_compute/load/asc_loadalign.md)、[asc_loadalign_postupdate](../reg_compute/load/asc_loadalign_postupdate.md) |
| 非对齐加载 | [Reg数据搬入概述](../reg_compute/load/reg_load_overview.md#非对齐搬入) | [asc_load](../reg_compute/load/asc_load.md)、[asc_loadunalign_pre](../reg_compute/load/asc_loadunalign_pre.md)、[asc_loadunalign](../reg_compute/load/asc_loadunalign.md) |
| 对齐存储 | [Reg数据搬出概述](../reg_compute/store/reg_store_overview.md) | [asc_storealign](../reg_compute/store/asc_storealign.md)、[asc_storealign_postupdate](../reg_compute/store/asc_storealign_postupdate.md) |
| 非对齐存储 | [Reg数据搬出概述](../reg_compute/store/reg_store_overview.md#非对齐搬出) | [asc_storeunalign](../reg_compute/store/asc_storeunalign.md)、[asc_storeunalign_post](../reg_compute/store/asc_storeunalign_post.md) |

寄存器类型与基础API类型的对应关系请参见[寄存器类型](../defs/type/reg_data_types.md)。

## 使用要点

- DAV_3510的矢量数据寄存器长度为256字节，掩码寄存器长度为32字节。单次有效元素数取决于数据类型和掩码。
- 对齐接口通常要求32字节对齐，但广播、首元素、压缩掩码等模式可能使用更小的对齐粒度，应以接口表为准。
- 连续非对齐加载可使用易用接口`asc_load`，也可使用`vector_load_unalign`配合预处理和加载接口。非对齐存储使用`vector_store_unalign`保存尾块，主存储与后处理必须复用同一对象。
- 地址寄存器偏移和Post Update接口可减少循环中的标量地址计算，但偏移单位随接口及数据类型变化。
- 多条Reg指令访问重叠UB区域且存在读写冲突时，按接口约束使用`asc_mem_bar`保证顺序。
