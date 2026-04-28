# Vote Functions 测试用例

## 功能描述

测试 Warp Vote 函数：

- `any(int predicate)`: 如果组内任一线程的predicate为真，返回非零值
- `all(int predicate)`: 如果组内所有线程的predicate都为真，返回非零值
- `ballot(int predicate)`: 返回组内所有线程predicate结果的位掩码

## 测试场景

1. **全真测试**: 所有线程predicate为1
   - any() = 1, all() = 1, ballot() = 0xF (TileSize=4)

2. **全假测试**: 所有线程predicate为0
   - any() = 0, all() = 0, ballot() = 0

3. **混合测试**: 偶数rank线程predicate为1
   - any() = 1, all() = 0, ballot() = 0x5

4. **coalesced_group测试**: 部分活跃线程的vote操作

## 运行方法

```bash
mkdir build && cd build
cmake .. && make
./demo
```