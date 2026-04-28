# thread_block 测试用例

## 功能描述

测试 `thread_block` 类的所有公开方法：

- `this_thread_block()`: 获取当前线程块组
- `sync()`: 同步线程块内所有线程
- `thread_rank()`: 获取当前线程在块内的线性排名
- `group_index()`: 获取当前块在网格中的索引
- `thread_index()`: 获取当前线程在块内的索引
- `dim_threads()`: 获取线程块的维度
- `num_threads()`: 获取块内线程总数
- `size()`: 获取块内线程总数（同num_threads）
- `group_dim()`: 获取线程块的维度（同dim_threads）

## 运行方法

```bash
mkdir build && cd build
cmake .. && make
./demo
```