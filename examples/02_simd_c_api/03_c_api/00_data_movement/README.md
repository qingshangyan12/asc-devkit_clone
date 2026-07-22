# Data Movement API样例介绍

## 概述

本目录展示Ascend C API中UB与向量寄存器之间的数据搬运接口。样例覆盖按元素或DataBlock索引读取、对齐读写，以及MaskReg加载和存储等场景。

## 样例列表

| 目录名称 | 功能描述 |
| --- | --- |
| [reg_load_gather](./reg_load_gather) | 展示`asc_gather`和`asc_gather_datablock`的索引读取方式。 |
| [reg_load_store_align](./reg_load_store_align) | 展示偏移、PostUpdate、AddrReg、DataBlock、广播和上采样等对齐读写方式。 |
| [reg_load_store_mask](./reg_load_store_mask) | 展示MaskReg的加载、存储和选择操作。 |
