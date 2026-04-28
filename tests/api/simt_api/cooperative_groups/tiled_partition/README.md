# tiled_partition 测试用例

## 功能描述

测试 `tiled_partition()` 函数对不同父组的分区功能：

### 模板版本 (编译时确定TileSize)
```cpp
template <unsigned int Size, typename ParentT>
thread_block_tile<Size, ParentT> tiled_partition(const ParentT& g)
```

### 动态版本 (运行时确定TileSize)
```cpp
thread_group tiled_partition(const thread_group& parent, unsigned int tilesz)
thread_group tiled_partition(const thread_block& parent, unsigned int tilesz)
coalesced_group tiled_partition(const coalesced_group& parent, unsigned int tilesz)
```

## 测试覆盖

1. **thread_block分区**: 使用模板版本，TileSize = 4, 8, 16
2. **coalesced_group分区**: 使用动态版本，TileSize = 4

## 运行方法

```bash
mkdir build && cd build
cmake .. && make
./demo
```