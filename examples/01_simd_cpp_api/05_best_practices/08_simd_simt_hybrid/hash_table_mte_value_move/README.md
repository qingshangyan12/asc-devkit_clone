# HashTable中使用MTE队列优化insert_or_assign样例

## 概述

本样例实现一个C++容器风格的open-addressing HashTable，用于对比 `insert_or_assign` 中两种value写入方式：

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

`dim` 是运行时参数，不再固定写死在bucket结构中。HashTable bucket只保存key，value独立存储为 `values[capacity, dim]`，更接近HierarchicalKV中key/value分离的设计。

## 实现说明

样例把HashTable封装成接近C++标准库命名习惯的 `unordered_map` 类，公开 `key_type`、`mapped_type`、`size_type`、`bucket_type` 等类型别名，并提供 `insert_or_assign` 作为插入/更新入口：

```cpp
class unordered_map {
public:
    using key_type = int64_t;
    using mapped_type = float;
    using size_type = uint32_t;
    using insert_return_type = std::pair<size_type, bool>;

    insert_return_type insert_or_assign(
        const key_type* firstKey, const key_type* lastKey, const mapped_type* firstMapped);
};
```

`insert_or_assign` 的接口参考C++标准库命名：key已存在时更新value，key不存在时插入新key/value。由于本样例用于固定规模benchmark，当前实现要求一次插入 `KEY_NUM` 条记录。

Device侧ACL内存由 `device_vector<T>` 做RAII封装，避免在benchmark流程中散落 `aclrtMalloc`、`aclrtFree`、`aclrtMemcpy`。`RunOneDim()` 只负责构造输入、调用容器接口和校验结果。

HashTable bucket结构封装为 `unordered_map::bucket_type`：

```cpp
struct alignas(32) bucket_type {
    int64_t key;
    int64_t padding[3];
};
```

`key == 0` 表示空槽，`key == -1` 表示槽位被当前写入者锁定。`padding` 用于让bucket满足32B对齐。value不在bucket内联保存，而是由 `unordered_map` 按运行时 `dim` 独立分配。

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

样例没有实现对外的device `find()` API。benchmark结束后会通过 `unordered_map::copy_to()` 把bucket和值拷回host，然后用host侧 `FindValueInTable()` 按相同hash + linear probing查找key，并逐元素比较value。

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

单独运行SIMT baseline，并用 `msprof op` 采集某一组dim的性能数据：

```bash
cmake -S . -B build/simt -DSCENARIO_NUM=0
cmake --build build/simt -j
mkdir -p build/simt/profile_dim_128
cd build/simt/profile_dim_128
msprof op ../hash_table_mte_value_move 128
```

单独运行MTE队列优化版，并用 `msprof op` 采集某一组dim的性能数据：

```bash
cmake -S . -B build/mte -DSCENARIO_NUM=1
cmake --build build/mte -j
mkdir -p build/mte/profile_dim_128
cd build/mte/profile_dim_128
msprof op --launch-count=2 ../hash_table_mte_value_move 128
```

运行对比脚本：

```bash
./scripts/run_compare.sh
```

脚本会分别编译两种scenario，并对 `16/32/64/128/256` 五组dim逐一执行：

```bash
msprof op --launch-count=<launch_count> <binary> <dim>
```

其中baseline的 `launch_count` 为1，MTE队列优化版的 `launch_count` 为2。MTE队列优化版包含 `insert_or_assign_probe_kernel` 和 `write_values_mte_queue_kernel` 两个kernel，需要采集两次launch才能在同一份 `OPPROF_*` 结果中看到HashTable探测和MTE搬运阶段。

每组case会在如下目录下生成日志和 `OPPROF_*` profiling目录：

```text
build/scenario_0/profile_dim_<dim>/
build/scenario_1/profile_dim_<dim>/
```

可执行文件只打印样例参数和正确性结果，不再在CPU侧统计kernel耗时：

- `Dim`
- `Verification PASSED`

`msprof op` 执行完成后，会在当前运行目录下生成以 `OPPROF_{timestamp}_XXX` 命名的性能数据目录。性能对比以 `msprof` 生成的文件为准，重点查看：

| 文件 | 关注内容 |
|:---|:---|
| `OpBasicInfo.csv` | kernel执行时间、block维度等基础信息 |
| `PipeUtilization.csv` | Vector/MTE等流水耗时和占比 |
| `Memory.csv` | GM/UB等存储访问带宽 |
| `MemoryUB.csv` | Vector/Scalar到UB的访问情况 |
| `ResourceConflictRatio.csv` | UB bank conflict和资源冲突情况 |

其中 `SCENARIO_NUM=0` 是SIMT group直接写value的baseline；`SCENARIO_NUM=1` 是SIMT完成HashTable控制流后，由MTE队列执行value搬运的优化版本。比较两者时，应使用相同dim、相同输入规模、相同 `PROFILE_REPEAT_TIMES` 下的 `msprof` 结果。
