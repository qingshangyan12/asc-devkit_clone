# coalesced_group 测试用例

## 功能描述

测试 `coalesced_group` 类的所有公开方法：

- `coalesced_threads()`: 获取当前活跃线程组成的合并组
- `sync()`: 同步组内线程
- `num_threads()`: 获取组内线程总数
- `thread_rank()`: 获取当前线程在组内的排名
- `size()`: 获取组内线程总数（同num_threads）
- `meta_group_size()`: 获取父组分区产生的组数量（对于coalesced_threads()返回1）
- `meta_group_rank()`: 获取该组在父组分区中的排名（对于coalesced_threads()返回0）

## 测试场景

1. **全warp测试**: 所有线程都活跃时，测试coalesced_threads()
2. **部分warp测试**: 仅偶数线程活跃时，测试coalesced_threads()
3. **同步测试**: 测试sync()方法

## 运行方法

```bash
mkdir build && cd build
cmake .. && make
./demo
```