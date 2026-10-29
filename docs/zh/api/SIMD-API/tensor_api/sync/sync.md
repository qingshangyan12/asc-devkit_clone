# 同步控制

Tensor API提供三类同步控制能力：

- **[block、intra、inter和subblock同步](block_sync.md)**：函数式接口，用于核间或subblock之间的到达/等待同步。
- **[mutex](mutex.md)**：对象式流水锁，用于核内异步流水之间的资源交接。
- **[lock_guard](lock_guard.md)**：作用域锁保护对象，析构时自动释放引用的mutex。
- **[sync_data_barrier](data_barrier.md)**：数据访问屏障，等待前序内存访问完成。
- **[sync_pipe](sync_pipe.md)**：流水同步，等待指定流水或全部流水的前序指令完成。
