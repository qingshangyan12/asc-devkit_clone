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
