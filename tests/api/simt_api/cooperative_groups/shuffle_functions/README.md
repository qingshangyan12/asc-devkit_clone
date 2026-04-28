# Shuffle Functions 测试用例

## 功能描述

测试 Warp Shuffle 函数：

- `shfl(T var, unsigned int src_rank)`: 从指定rank的线程获取值
- `shfl_up(T var, int delta)`: 从向上delta个rank的线程获取值
- `shfl_down(T var, int delta)`: 从向下delta个rank的线程获取值
- `shfl_xor(T var, int laneMask)`: 从rank XOR laneMask的线程获取值

## 支持的数据类型

- int32_t, uint32_t
- int64_t, uint64_t
- float
- half, half2

## 测试覆盖

1. **thread_block_tile shuffle**: TileSize=4，测试int32_t和float
2. **coalesced_group shuffle**: 测试部分活跃线程场景

## 运行方法

```bash
mkdir build && cd build
cmake .. && make
./demo
```