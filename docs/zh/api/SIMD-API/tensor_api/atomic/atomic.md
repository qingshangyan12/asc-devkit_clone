# 原子操作

Tensor API提供数据搬运过程中的DMA原子操作配置能力，用于后续目的地址为Global Memory的受支持搬运指令。

- **[set_atomic_add](set_atomic_add.md)**：开启原子累加。
- **[set_atomic_max](set_atomic_max.md)**：开启原子取大。
- **[set_atomic_min](set_atomic_min.md)**：开启原子取小。
- **[disable_dma_atomic](disable_dma_atomic.md)**：关闭DMA原子模式。

各接口的硬件语义继承对应C API。Tensor API通过模板参数指定数据类型，避免直接暴露带类型后缀的C API函数名。

