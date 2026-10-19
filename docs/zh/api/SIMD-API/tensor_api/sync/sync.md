# 同步控制

Tensor API提供两类同步控制能力：

- **[block、intra、inter和subblock同步](block_sync.md)**：函数式接口，用于核间或subblock之间的到达/等待同步。
- **[mutex](mutex.md)**：对象式流水锁，用于核内异步流水之间的资源交接。
- **[lock_guard](lock_guard.md)**：RAII风格锁保护对象，析构时自动释放引用的mutex。

各接口的硬件同步语义继承对应C API，Tensor API层不额外改变同步协议。

