# HashTable中使用MTE队列优化insert_or_assign样例

## 概述

本样例实现一个open-addressing HashTable `insert_or_assign` 流程，用于对比两种value写入方式：

| Case | `SCENARIO_NUM` | 实现方式 | 说明 |
|:---|:---:|:---|:---|
| Case 0 | 0 | SIMT group insert_or_assign | 一个SIMT协作组完成hash/probe/CAS后，由组内线程分片写入一条value向量。 |
| Case 1 | 1 | MTE queue insert_or_assign | SIMT线程完成hash/probe/CAS并记录目标bucket，随后由MTE队列批量执行GM->UB->GM搬运。 |

样例不是单纯的value copy microbenchmark：两种case都会执行HashTable插入/更新、线性探测、key写入和最终表内容校验。

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
| `TEST_DIMS` | 16, 32, 64, 128, 256 |
| `BLOCK_DIM` | 64 |
| `THREAD_COUNT` | 512 |
| `GROUP_SIZE` | 16 |

`dim` 是运行时参数，不再固定写死在bucket结构中。HashTable bucket只保存key，value独立存储为 `tableValues[capacity, dim]`，更接近HierarchicalKV中key/value分离的设计。

## 实现说明

HashTable bucket结构：

```cpp
struct alignas(32) Bucket {
    int64_t key;
    int64_t padding[3];
};
```

`key == 0` 表示空槽，`key == -1` 表示槽位被当前写入者锁定。`padding` 用于让bucket满足32B对齐。value不在bucket内联保存，而是按运行时 `dim` 独立分配。

### 冲突处理

冲突后不会直接覆盖。当前样例使用线性探测：

```cpp
bucketIdx = hash(key) & (capacity - 1);
for (uint32_t probe = 0; probe < capacity; ++probe) {
    if 当前bucket为空，插入;
    if 当前bucket的key相同，更新;
    bucketIdx = (bucketIdx + 1) & (capacity - 1);
}
```

默认benchmark使用确定性乱序唯一key，避免连续key按bucket顺序插入导致结果过于理想化。当前乘法hash和2的幂容量组合下，默认key集基本不会产生冲突，测试重点仍然是value搬运路径。

### Case 0：SIMT group写value

每个SIMT group处理一条key。lane 0执行hash/probe/CAS，抢到槽位后，组内线程按元素分片写入value：

```cpp
for (uint32_t j = laneId; j < dim; j += GROUP_SIZE) {
    tableValues[bucketIdx * dim + j] = values[idx * dim + j];
}
```

该方式把value搬运也放在SIMT线程上，value越长，SIMT线程用于GM读写的指令越多。

### Case 1：MTE队列写value

Case 1分为两个kernel：

1. `insert_or_assign_probe_kernel`：SIMT线程执行hash/probe/CAS，输出每条key对应的bucket index。
2. `write_values_mte_queue_kernel`：使用 `TQueBind<VECIN, VECOUT>` 作为MTE搬运队列，按batch把连续输入value搬入UB，再写回HashTable value区域。

MTE写回阶段按批次执行：

1. MTE2把多条value从GM搬入UB。
2. MTE3把UB中的value搬到HashTable value区域。

这对应PR124的核心优化方向：SIMT负责HashTable控制流，MTE负责长value向量搬运。本样例采用两阶段实现，避免把完整工程中的ring queue并发协议直接暴露给教学样例。

### 校验方式

样例没有实现对外的device `find()` API。benchmark结束后会把 `table` 和 `tableValues` 拷回host，然后用host侧 `FindValueInTable()` 按相同hash + linear probing查找key，并逐元素比较value。

## DIM选择依据

开源推荐框架中embedding维度通常由模型配置指定，并没有唯一固定值：

- NVIDIA Merlin HugeCTR示例和API文档中可见 `embedding_vec_size=16` 与 `embedding_vec_size=128` 的配置。
- DeepCTR的 `SparseFeat` 暴露 `embedding_dim` 参数，Quick Start示例使用 `embedding_dim=4`。
- HugeCTR文档也说明 `embedding_vec_size` 是需要用户指定的整数参数，并且部分多模态示例会使用更大的预训练embedding。

本样例选择 `16/32/64/128/256` 五组，覆盖小型CTR embedding到较长向量搬运的常见性能区间，同时保持每条value大小为32B对齐倍数，便于MTE搬运。

参考：

- https://nvidia-merlin.github.io/HugeCTR/main/api/python_interface.html
- https://nvidia-merlin.github.io/HugeCTR/main/api/hugectr_layer_book.html
- https://deepctr-doc.readthedocs.io/en/latest/Quick-Start.html
- https://deepctr-doc.readthedocs.io/en/latest/Features.html

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

- `Dim`
- `Avg Latency(us)`
- `Throughput(Mops/s)`
- `Effective Value BW(GB/s)`
- `Verification PASSED`

## 远端实测结果

在 `root@121.41.199.170:20002`，CANN 9.0.0环境下执行 `./scripts/run_compare.sh`：

| Dim | SIMT耗时(us) | MTE耗时(us) | SIMT吞吐(Mops/s) | MTE吞吐(Mops/s) | SIMT有效带宽(GB/s) | MTE有效带宽(GB/s) |
|---:|---:|---:|---:|---:|---:|---:|
| 16 | 165.521 | 73.266 | 791.876 | 1788.983 | 50.680 | 114.495 |
| 32 | 166.084 | 75.716 | 789.193 | 1731.100 | 101.017 | 221.581 |
| 64 | 204.870 | 82.316 | 639.780 | 1592.300 | 163.784 | 407.629 |
| 128 | 315.485 | 96.680 | 415.462 | 1355.734 | 212.716 | 694.136 |
| 256 | 575.273 | 217.996 | 227.843 | 601.258 | 233.311 | 615.689 |

两种实现、五组dim均 `Verification PASSED`。默认配置下，value越长，SIMT直接写value的耗时增长更明显；MTE队列版本在五组dim上均有收益。
