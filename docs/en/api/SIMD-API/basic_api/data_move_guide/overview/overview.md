# Overview<a name="ZH-CN_TOPIC_0000002565807407"></a>

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-09-23T13:58:17.833Z pushedAt=2026-10-26T07:41:44.257Z nl=2 -->

Data transfer is one of the most fundamental and critical operations in operator development. In the hierarchical storage architecture of the Ascend AI Core, data must flow between storage units at different levels to complete computation tasks. The data movement APIs (**DataCopy**, **DataCopyPad**, **Copy**, **LoadData** (2D matrix transfer), **LoadData** (convolution data movement), **FixPipe**, and so on) expose the capabilities of the underlying hardware transfer units (MTE1/MTE2/MTE3/FixPipe), enabling developers to efficiently and flexibly transfer data between [Global Memory](../../../basic_api/data_structures/GlobalTensor/GlobalTensor_intro.md) and various levels of [Local Memory](../../../basic_api/data_structures/LocalTensor/LocalTensor_intro.md), as well as within Local Memory.

This chapter provides introductory content on data transfer, aiming to help readers build a unified understanding of the following core concepts before delving into the detailed chapters on each data path:

- **Storage hierarchy and data paths:** What physical storage units exist inside the AI Core, between which paths data can flow, and the hardware pipeline types on which transfer depends.
- **Mapping between logical positions (TPosition) and physical memory:** Ascend C hides hardware differences through the TPosition mechanism. Developers need to understand the actual physical storage corresponding to logical positions such as VECIN/VECOUT/A1/B1/CO1.
- **Transfer function classification:** The concepts and applicable scenarios of functions such as basic data transfer, high-dimensional split transfer, slice transfer, in-line format conversion (ND2NZ/NZ2ND/DN2NZ), on-the-fly quantization activation, unaligned transfer, Unified Buffer (UB) internal transfer (Copy), matrix fractal transfer (**LoadData** (2D matrix transfer)/**LoadData** (convolution data movement)), and multi-dimensional data transfer NDDMA.
- **Key parameter concepts:** The physical meanings and configuration methods of parameters such as DataBlock, blockLen, blockCount, srcStride/dstStride, and repeat.
- **Overall constraints:** The address alignment requirements, data volume constraints, and synchronization rules common to all data movement APIs.

- **[Data Transfer Concepts](data_move_concept.md)**  

- **[Data Paths](data_path.md)**  

- **[Overall Constraints](overall_constraints.md)**  
