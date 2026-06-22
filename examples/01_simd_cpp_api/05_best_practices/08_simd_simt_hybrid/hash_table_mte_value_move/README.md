# HashTable中使用MTE队列优化insert_or_assign样例

## 概述

本样例实现一个真实的open-addressing HashTable `insert_or_assign` 流程，用于对比两种value写入方式：

| Case | `SCENARIO_NUM` | 实现方式 | 说明 |
|:---|:---:|:---|:---|
| Case 0 | 0 | SIMT group insert_or_assign | 参考NVIDIA/原始HierarchicalKV思路，一个SIMT协作组完成hash/probe/CAS后，由组内线程分片写入一条value向量。 |
| Case 1 | 1 | MTE queue insert_or_assign | 参考PR124的优化方向，SIMT线程完成hash/probe/CAS并记录目标bucket，随后由MTE队列批量执行GM->UB->GM搬运。 |

样例不是单纯的value copy microbenchmark：两种case都会执行HashTable插入/更新、线性探测、key锁定和最终表内容校验。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | > CANN 9.0.0 |

## 目录结构介绍

```
├── hash_table_mte_value_move
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── hash_table_mte_value_move.asc
│   └── scripts
│       └── run_compare.sh
```

## 样例参数

| 参数 | 值 |
|:---|---:|
| `HASH_TABLE_CAPACITY` | 256 * 1024 |
| `KEY_NUM` | 128 * 1024 |
| 负载因子 | 0.5 |
| `VALUE_DIM` | 128 |
| 单条value大小 | 512B |
| `BLOCK_DIM` | 64 |
| `THREAD_COUNT` | 512 |
| `GROUP_SIZE` | 16 |

## 实现说明

HashTable bucket结构：

```cpp
struct alignas(32) Bucket {
    int64_t key;
    int64_t padding[3];
    float value[VALUE_DIM];
};
```

`key == 0` 表示空槽，`key == -1` 表示槽位被当前写入者锁定。`padding` 用于让 `value` 起始地址满足32B对齐，匹配MTE搬运要求。Case 0通过CAS抢占空槽或已有key，然后由SIMT group写value；Case 1先通过SIMT阶段确定每条key的目标bucket，再由MTE队列阶段写value。样例使用连续唯一key和乘法hash，在默认容量下不会产生碰撞，便于聚焦value搬运优化。

### Case 0：SIMT group写value

每个SIMT group处理一条key。lane 0执行hash/probe/CAS，抢到槽位后，组内线程按元素分片写入value：

```cpp
for (uint32_t j = laneId; j < VALUE_DIM; j += GROUP_SIZE) {
    bucket->value[j] = values[idx * VALUE_DIM + j];
}
```

该方式把value搬运也放在SIMT线程上，value越长，SIMT线程用于GM读写的指令越多。

### Case 1：MTE队列写value

Case 1分为两个kernel：

1. `insert_or_assign_probe_kernel`：SIMT线程执行hash/probe/CAS，输出每条key对应的bucket index。
2. `write_values_mte_queue_kernel`：使用 `TQueBind<VECIN, VECOUT>` 作为MTE搬运队列，按batch把连续输入value搬入UB，再写回HashTable bucket。

MTE写回阶段按批次执行：

1. MTE2把多条value从GM搬入UB。
2. MTE3把UB中的value搬到HashTable bucket。

这对应PR124的核心优化方向：SIMT负责HashTable控制流，MTE负责长value向量搬运。本样例采用两阶段实现，避免把PR124完整工程中的ring queue并发协议直接暴露给教学样例。

## 编译运行

单独运行SIMT baseline：

```bash
cmake -S . -B build/simt -DSCENARIO_NUM=0
cmake --build build/simt -j
./build/simt/hash_table_mte_value_move
```

单独运行MTE队列优化版：

```bash
cmake -S . -B build/mte -DSCENARIO_NUM=1
cmake --build build/mte -j
./build/mte/hash_table_mte_value_move
```

运行对比脚本：

```bash
./scripts/run_compare.sh
```

输出中会打印：

- `Avg Latency(us)`
- `Throughput(Mops/s)`
- `Effective Value BW(GB/s)`
- `Verification PASSED`

## 与HierarchicalKV实现的关系

- `HierarchicalKV/` 目录是NVIDIA原始项目，可参考其HashTable插入和value更新思路。
- PR124在Ascend实现中引入了UB ring queue和MTE搬运，把原来SIMT线程直接写长value的路径改为任务化搬运。
- 本样例保留HashTable插入/更新关键路径，但为便于教学和稳定编译，使用单bucket数组、线性探测和固定LRU无关的key锁定协议。

## 远端实测结果

在 `root@121.41.199.170:20002`，CANN 9.0.0环境下执行 `./scripts/run_compare.sh`：

| Case | 实现方式 | 平均耗时 | 吞吐 | 有效value带宽 | 校验 |
|:---|:---|---:|---:|---:|:---|
| 0 | SIMT group insert_or_assign | 357.839 us | 366.287 Mops/s | 187.539 GB/s | PASS |
| 1 | MTE queue insert_or_assign | 124.151 us | 1055.747 Mops/s | 540.543 GB/s | PASS |

默认配置下，MTE队列版本相比SIMT group写value版本耗时下降约65.3%。该结果主要反映长value向量写入路径的收益；改变value维度、负载因子、key分布或冲突率后，收益会变化。
