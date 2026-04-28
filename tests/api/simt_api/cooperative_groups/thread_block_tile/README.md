# thread_block_tile 测试用例

## 功能描述

测试 `thread_block_tile<Size, ParentT>` 类的所有公开方法：

- `sync()`: 同步组内线程
- `num_threads()`: 获取组内线程总数
- `thread_rank()`: 获取当前线程在组内的排名
- `size()`: 获取组内线程总数（同num_threads）
- `meta_group_rank()`: 获取该组在父组分区中的排名
- `meta_group_size()`: 获取父组分区产生的组数量

## 测试覆盖

本测试覆盖以下TileSize：
- TileSize = 4
- TileSize = 8
- TileSize = 16

## 运行方法

```bash
mkdir build && cd build
cmake .. && make
./demo
```