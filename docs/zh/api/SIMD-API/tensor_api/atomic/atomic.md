# 原子操作

DMA（Direct Memory Access，直接内存访问单元）原子操作用于在数据搬运过程中对Global Memory上的已有数据执行原子读改写，再写入搬运结果。Tensor API提供以下接口配置后续目的地址为Global Memory的受支持搬运指令：

- **[set_atomic_add](set_atomic_add.md)**：将待搬运数据与Global Memory上的已有数据求和，并将结果写入Global Memory。
- **[set_atomic_max](set_atomic_max.md)**：比较待搬运数据与Global Memory上的已有数据，将两者中的最大值写入Global Memory。
- **[set_atomic_min](set_atomic_min.md)**：比较待搬运数据与Global Memory上的已有数据，将两者中的最小值写入Global Memory。
- **[disable_dma_atomic](disable_dma_atomic.md)**：关闭DMA原子模式，避免原子配置影响后续搬运。

`set_atomic_add`、`set_atomic_max`和`set_atomic_min`通过模板参数指定参与原子操作的数据类型。
