# HashTable中使用MTE队列优化insert_or_assign样例

## 概述

本样例以HashTable `insert_or_assign` 操作为例，介绍SIMT与SIMD C API混合编程场景下，使用MTE搬运长value向量的性能调优方法。样例实现一个C++容器风格的open-addressing HashTable，对比SIMT group直接写value，以及SIMT完成HashTable控制流后由MTE执行value搬运的性能差异。

**优化路径**：

| Case | SCENARIO_NUM | 实现方式 | 说明 |
|:---|:---:|:---|:---|
| Case 0 | 0 | SIMT group直接写value | 一个SIMT协作组完成hash/probe/CAS后，由组内线程分片写入一条value向量，作为baseline。 |
| Case 1 | 1 | 两阶段MTE搬运value | SIMT线程完成hash/probe/CAS并将目标bucket写入GM任务列表，第二个kernel使用C API MTE搬运接口批量执行GM->UB->GM搬运。 |
| Case 2 | 2 | MTETaskQueue搬运value | 单kernel内使用轻量MTETaskQueue，SIMT按batch生成UB任务，MTE立即消费任务并搬运value。 |

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | > CANN 9.0.0 |

## 目录结构介绍

```
├── hash_table_mte_value_move
│   ├── CMakeLists.txt                // 编译工程文件
│   ├── hash_table_mte_value_move.asc // Ascend C样例实现
│   ├── README.md                     // 样例说明文档
│   └── scripts
│       └── run_compare.sh            // 编译并使用msprof op采集三种Case性能数据
```

## 样例描述

**样例功能**：

样例实现一个固定容量open-addressing HashTable，支持 `insert_or_assign` 语义：key已存在时更新value，key不存在时插入key/value。Hash冲突通过线性探测解决，bucket中只保存key，value独立存储为 `values[capacity, dim]`，其中 `dim` 为运行时参数。

HashTable插入流程如下：

$$
bucket = hash(key) \& (capacity - 1)
$$

$$
bucket = (bucket + 1) \& (capacity - 1), \quad \text{when collision occurs}
$$

**样例规格**：

<table>
<tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">HashTable insert_or_assign</td></tr>
<tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">keys</td><td align="center">[128 * 1024]</td><td align="center">int64</td><td align="center">ND</td></tr>
<tr><td align="center">values</td><td align="center">[128 * 1024, dim]</td><td align="center">float32</td><td align="center">ND</td></tr>
<tr><td rowspan="2" align="center">样例输出</td><td align="center">table keys</td><td align="center">[256 * 1024]</td><td align="center">int64</td><td align="center">ND</td></tr>
<tr><td align="center">table values</td><td align="center">[256 * 1024, dim]</td><td align="center">float32</td><td align="center">ND</td></tr>
<tr><td rowspan="4" align="center">核函数名</td><td align="center">SCENARIO_NUM=0</td><td colspan="3" align="center"><code>insert_or_assign_group_store_kernel</code></td></tr>
<tr><td rowspan="2" align="center">SCENARIO_NUM=1</td><td colspan="3" align="center"><code>insert_or_assign_probe_kernel</code></td></tr>
<tr><td colspan="3" align="center"><code>write_values_mte_queue_kernel</code></td></tr>
<tr><td align="center">SCENARIO_NUM=2</td><td colspan="3" align="center"><code>insert_or_assign_mte_task_queue_kernel</code></td></tr>
</table>

样例参数如下：

| 参数 | 值 |
|:---|---:|
| `HASH_TABLE_CAPACITY` | 256 * 1024 |
| `KEY_NUM` | 128 * 1024 |
| 负载因子 | 0.5 |
| `TEST_DIMS` | 16, 32, 64, 128, 256 |
| `BLOCK_DIM` | 64 |
| `THREAD_COUNT` | 512 |
| `GROUP_SIZE` | 16 |

## 样例实现

### 性能指标说明

| 字段名 | 字段含义 |
|:---|:---|
| Task Duration(μs) | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。 |
| aiv_time(μs) | Task 在 AI Vector Core 上的执行时间。 |
| aiv_vec_time(μs) | vec类型指令耗时。SIMT VF调用中的线程计算也体现在该类统计中。 |
| aiv_vec_ratio | vec类型指令cycle数在total cycle数中的占用比。 |
| aiv_scalar_time(μs) | scalar类型指令耗时。 |
| aiv_scalar_ratio | scalar类型指令cycle数在total cycle数中的占用比。 |
| aiv_mte2_time(μs) | mte2类型指令耗时，主要对应GM到UB的搬运。 |
| aiv_mte2_ratio | mte2类型指令cycle数在total cycle数中的占用比。 |
| aiv_mte3_time(μs) | mte3类型指令耗时，主要对应UB到GM的搬运。 |
| aiv_mte3_ratio | mte3类型指令cycle数在total cycle数中的占用比。 |

### Case 0：SIMT group直接写value

**实现方式**：参考 `insert_or_assign_group_store_vf()` 函数实现。

该实现中，每个SIMT group处理一条key。lane 0执行hash、线性探测和CAS抢占bucket，成功后组内线程按 `GROUP_SIZE` 分片写入value向量。value搬运由SIMT线程直接访问GM完成。

**关键代码**：

```cpp
__global__ __vector__ void insert_or_assign_group_store_kernel(__gm__ Bucket* table,
    __gm__ float* tableValues, __gm__ const int64_t* keys, __gm__ const float* values, uint32_t keyNum,
    uint32_t capacity, uint32_t dim)
{
    asc_vf_call<insert_or_assign_group_store_vf>(
        dim3(THREAD_COUNT), table, tableValues, keys, values, keyNum, capacity, dim, block_idx, BLOCK_DIM);
}

__simt_vf__ inline void insert_or_assign_group_store_vf(__gm__ Bucket* table,
    __gm__ float* tableValues, __gm__ const int64_t* keys, __gm__ const float* values, uint32_t keyNum,
    uint32_t capacity, uint32_t dim, uint32_t blockIndex, uint32_t blockNum)
{
    const uint32_t tid = static_cast<uint32_t>(threadIdx.x);
    const uint32_t laneId = tid % GROUP_SIZE;
    // lane 0完成hash/probe/CAS，组内线程通过asc_shfl获取bucketIdx。
    if (success == 1) {
        uint32_t dstBase = bucketIdx * dim;
        uint32_t srcBase = idx * dim;
        for (uint32_t j = laneId; j < dim; j += GROUP_SIZE) {
            tableValues[dstBase + j] = values[srcBase + j];
        }
        asc_threadfence();
        if (laneId == 0) {
            (void)asc_atomic_exch(&table[bucketIdx].key, key);
        }
    }
}
```

**性能数据**：

| Case | 实现方式 | dim | 核数 | Task Duration(μs) |
|:---|:---|---:|:---:|---:|
| 0 | SIMT group直接写value | 16 | 64 | 135.866 |
| 0 | SIMT group直接写value | 32 | 64 | 145.045 |
| 0 | SIMT group直接写value | 64 | 64 | 178.050 |
| 0 | SIMT group直接写value | 128 | 64 | 261.511 |
| 0 | SIMT group直接写value | 256 | 64 | 449.114 |

**性能数据分析**：

- 该Case把HashTable控制流和value搬运都放在SIMT线程中完成，value越长，SIMT线程执行GM读写的指令越多。
- 当 `dim` 从16增长到256时，`Task Duration` 从 **135.866μs** 增长到 **449.114μs**，耗时随value向量长度明显增长。
- 该实现适合作为baseline，用于观察长value向量直接由SIMT线程写GM时的成本。

**原理说明**：

HashTable插入本身包含hash、线性探测、CAS、冲突处理等离散控制流，适合使用SIMT表达。但value向量通常是连续内存，尤其在embedding hash table场景中，value可能包含几十到几百个float元素。若由SIMT线程逐元素访问GM，搬运粒度和搬运流水利用率不如MTE连续搬运。

本样例后续Case将HashTable控制流与value搬运拆成两个阶段：SIMT负责离散控制流，MTE负责连续value向量搬运。

**下一步优化方向**：

当前Case作为SIMT group直接写value的基准Case，用于展示HashTable插入时value搬运和控制流耦合在同一段SIMT线程中的性能数据。下一步将把value写入从SIMT线程中拆出来，改由MTE执行批量GM->UB->GM搬运。

### Case 1：两阶段MTE搬运value

**实现方式**：参考 `insert_or_assign_probe_vf()` 和 `write_values_mte_queue_kernel()` 函数实现。

该实现分为两个kernel：第一个kernel使用SIMT线程执行hash/probe/CAS，并输出每条key对应的bucket index；第二个kernel使用C API `asc_copy_gm2ub_align` 和 `asc_copy_ub2gm_align`，按batch把输入value搬入UB，再搬到HashTable的value区域。

**关键代码**：

```cpp
__global__ __vector__ void insert_or_assign_probe_kernel(
    __gm__ Bucket* table, __gm__ const int64_t* keys, __gm__ uint32_t* bucketIndices, uint32_t keyNum, uint32_t capacity)
{
    asc_vf_call<insert_or_assign_probe_vf>(
        dim3(THREAD_COUNT), table, keys, bucketIndices, keyNum, capacity, block_idx, BLOCK_DIM);
}

__global__ __vector__ void write_values_mte_queue_kernel(
    __gm__ float* tableValues, __gm__ const float* values, __gm__ const uint32_t* bucketIndices, uint32_t dim)
{
    asc_init();
    __ubuf__ float valueLocal[MTE_LOCAL_VALUE_COUNT];
    for (uint32_t baseRow = coreStart; baseRow < coreEnd; baseRow += MTE_BATCH_SIZE) {
        uint32_t batchCount = (MTE_BATCH_SIZE < remain) ? MTE_BATCH_SIZE : remain;
        uint32_t copyInBytes = batchCount * dim * sizeof(float);
        asc_copy_gm2ub_align(
            valueLocal, const_cast<__gm__ float*>(values + baseRow * dim), 1, copyInBytes, 0, 0, true,
            valuesCacheMode, 0, 0);
        asc_sync_pipe(pipe_t::PIPE_ALL);

        uint32_t copyOutBytes = dim * sizeof(float);
        for (uint32_t i = 0; i < batchCount; ++i) {
            uint32_t bucketIdx = bucketIndices[baseRow + i];
            asc_copy_ub2gm_align(
                tableValues + bucketIdx * dim, valueLocal + i * dim, 1, copyOutBytes, tableValuesCacheMode, 0, 0);
        }
        asc_sync_pipe(pipe_t::PIPE_ALL);
    }
}
```

**性能数据**：

| Case | 实现方式 | dim | 核数 | probe Task Duration(μs) | MTE copy Task Duration(μs) | 总耗时(μs) |
|:---|:---|---:|:---:|---:|---:|---:|
| 1 | 两阶段MTE搬运value | 16 | 64 | 29.371 | 98.564 | 127.935 |
| 1 | 两阶段MTE搬运value | 32 | 64 | 28.875 | 81.088 | 109.963 |
| 1 | 两阶段MTE搬运value | 64 | 64 | 28.888 | 84.814 | 113.702 |
| 1 | 两阶段MTE搬运value | 128 | 64 | 28.715 | 96.500 | 125.215 |
| 1 | 两阶段MTE搬运value | 256 | 64 | 28.611 | 164.350 | 192.961 |

**性能数据分析**：

- `insert_or_assign_probe_kernel` 耗时基本稳定在 **29μs** 左右，说明HashTable控制流开销与value维度基本无关。
- `write_values_mte_queue_kernel` 随 `dim` 增长而增长，但增长速度明显低于Case 0中SIMT直接写value的增长速度。
- 当 `dim=16` 时，MTE版本总耗时 **127.935μs**，相比Case 0的 **135.866μs** 收益较小；当 `dim=256` 时，MTE版本总耗时 **192.961μs**，相比Case 0的 **449.114μs** 明显降低。

**原理说明**：

HashTable的hash/probe/CAS是离散访存和控制流密集型逻辑，适合由SIMT线程完成；value向量是连续内存搬运，适合由MTE完成。Case 1把两类任务拆开：SIMT只负责确定目标bucket，MTE按batch搬运连续value。这样可以减少SIMT线程逐元素GM读写的指令数量，并提升长value向量搬运效率。

本样例选择 `16/32/64/128/256` 五组dim，覆盖推荐系统中常见embedding维度区间。value越长，SIMT直接写value的搬运成本越高，MTE搬运收益越明显。

**下一步优化方向**：

当前Case已经把SIMT控制流和MTE搬运拆开，但仍存在两点额外成本：一是 `insert_or_assign_probe_kernel` 和 `write_values_mte_queue_kernel` 之间存在kernel边界；二是目标bucket列表需要先写入GM，再由第二个kernel读回。下一步将引入轻量MTETaskQueue，在同一个kernel内按batch生成任务并立即消费，减少GM任务列表中转。

### Case 2：MTETaskQueue搬运value

**实现方式**：参考 `MTETaskQueue` 和 `insert_or_assign_mte_task_queue_kernel()` 函数实现。

该实现仍然由SIMT线程执行hash/probe/CAS，但不再把完整 `bucketIndices` 写入GM。每个Vector Core按batch处理本核负责的key，SIMT VF将当前batch的bucket index写入UB中的任务数组，随后同一个kernel内由MTE消费这批任务并搬运value。

**关键代码**：

```cpp
class MTETaskQueue {
public:
    __aicore__ inline void AssignTask(__gm__ Bucket* table, __gm__ const int64_t* keys, uint32_t batchStart,
        uint32_t batchCount, uint32_t capacity)
    {
        asc_vf_call<insert_or_assign_task_queue_probe_vf>(
            dim3(THREAD_COUNT), table, keys, bucketTasks_, batchStart, batchCount, capacity);
        asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    }

    __aicore__ inline void RunTask(uint32_t batchStart, uint32_t batchCount)
    {
        asc_copy_gm2ub_align(valueLocal_, const_cast<__gm__ float*>(values_ + batchStart * dim_), 1,
            batchCount * dim_ * sizeof(float), 0, 0, true, valuesCacheMode_, 0, 0);
        asc_sync_pipe(pipe_t::PIPE_ALL);

        for (uint32_t i = 0; i < batchCount; ++i) {
            uint32_t bucketIdx = bucketTasks_[i];
            asc_copy_ub2gm_align(tableValues_ + bucketIdx * dim_, valueLocal_ + i * dim_, 1,
                dim_ * sizeof(float), tableValuesCacheMode_, 0, 0);
        }
        asc_sync_pipe(pipe_t::PIPE_ALL);
    }
};

__global__ __vector__ void insert_or_assign_mte_task_queue_kernel(
    __gm__ Bucket* table, __gm__ float* tableValues, __gm__ const int64_t* keys, __gm__ const float* values,
    uint32_t keyNum, uint32_t capacity, uint32_t dim)
{
    asc_init();
    __ubuf__ float valueLocal[MTE_LOCAL_VALUE_COUNT];
    __ubuf__ uint32_t bucketTasks[MTE_TASK_QUEUE_CAPACITY];

    MTETaskQueue taskQueue;
    taskQueue.Init(tableValues, values, valueLocal, bucketTasks, dim);

    for (uint32_t baseRow = coreStart; baseRow < coreEnd; baseRow += MTE_TASK_QUEUE_CAPACITY) {
        uint32_t batchCount = ...;
        taskQueue.AssignTask(table, keys, baseRow, batchCount, capacity);
        taskQueue.RunTask(baseRow, batchCount);
    }
}
```

**性能数据**：

| Case | 实现方式 | dim | 核数 | Task Duration(μs) |
|:---|:---|---:|:---:|---:|
| 2 | MTETaskQueue搬运value | 16 | 64 | 126.706 |
| 2 | MTETaskQueue搬运value | 32 | 64 | 112.016 |
| 2 | MTETaskQueue搬运value | 64 | 64 | 113.558 |
| 2 | MTETaskQueue搬运value | 128 | 64 | 131.578 |
| 2 | MTETaskQueue搬运value | 256 | 64 | 213.315 |

**性能数据分析**：

- 相比Case 0，Case 2在五组dim下均有收益；`dim=256` 时从 **449.114μs** 降到 **213.315μs**，加速比为 **2.11x**。
- 相比Case 1，Case 2减少了一个kernel launch和GM任务列表中转，在 `dim=16`、`dim=64` 上略快或基本持平。
- 在 `dim=128`、`dim=256` 上，当前样例版MTETaskQueue慢于Case 1。主要原因是该实现为教学样例中的轻量队列，每个batch都会调用一次SIMT VF并执行一次UB同步；当value较长时，这些batch级控制开销抵消了减少GM任务列表中转带来的收益。

**原理说明**：

PR124中的 `MTETaskQueue` 目标是在真实insert流程内形成SIMT生产任务、MTE消费任务的流水。本样例的Case 2保留这个核心结构，但为了便于阅读和验证，没有引入完整ring queue协议，而是使用UB数组承载当前batch任务。因此该Case更适合说明“任务队列化搬运”的编程形态，不代表完整PR124实现的性能上限。

## 性能对比总结

### Ascend 950PR性能对比

下表为本样例在Ascend 950系列产品上运行的性能数据对比：

| dim | Case 0 SIMT直接写value(μs) | Case 1两阶段MTE总耗时(μs) | Case 1加速比 | Case 2 MTETaskQueue(μs) | Case 2加速比 |
|---:|---:|---:|---:|---:|---:|
| 16 | 135.866 | 127.935 | 1.06x | 126.706 | 1.07x |
| 32 | 145.045 | 109.963 | 1.32x | 112.016 | 1.29x |
| 64 | 178.050 | 113.702 | 1.57x | 113.558 | 1.57x |
| 128 | 261.511 | 125.215 | 2.09x | 131.578 | 1.99x |
| 256 | 449.114 | 192.961 | 2.33x | 213.315 | 2.11x |

### 优化要点总结

| 优化手段 | 核心原理 | 样例体现 |
|:---|:---|:---|
| SIMT处理HashTable控制流 | hash/probe/CAS包含离散访存和分支判断，适合SIMT线程表达 | 三种Case都使用SIMT完成HashTable插入或探测逻辑 |
| MTE处理连续value搬运 | value向量是连续内存，使用MTE执行GM->UB->GM批量搬运，可减少SIMT线程逐元素访问GM的成本 | Case 1在 `dim=128` 时达到2.09x加速，在 `dim=256` 时达到2.33x加速 |
| 任务队列化搬运 | SIMT生成搬运任务，MTE按任务消费，可减少完整任务列表写GM再读回的中转 | Case 2在 `dim=16/64` 与Case 1接近或略优，但当前轻量队列在长dim下受batch级SIMT调用和同步开销影响 |
| 按运行时dim覆盖不同embedding长度 | embedding维度通常由推荐模型配置指定，短value场景搬运成本较小，长value场景更容易体现MTE收益 | `dim=16` 收益较小，`dim>=64` 后收益明显增大 |

---

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 切换Case

  在cmake编译时通过 `-DSCENARIO_NUM=N` 指定要编译的Case，各Case说明：
  - `0`: SIMT group直接写value
  - `1`: 两阶段MTE搬运value
  - `2`: MTETaskQueue搬运value

  示例：
  ```bash
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=2
  ```

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  SCENARIO_NUM=2                                                       # 选择执行场景
  mkdir -p build && cd build;                                          # 创建并进入 build 目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # 编译工程
  ./hash_table_mte_value_move 128                                      # 执行单组dim并验证结果
  ```

  也可以直接运行对比脚本，编译三种Case并分别采集 `16/32/64/128/256` 五组dim的性能数据。
  ```bash
  ./scripts/run_compare.sh
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |:---|:---|:---|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，对应 Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `0`、`1`、`2` | Case编号：0=SIMT group直接写value，1=两阶段MTE搬运value，2=MTETaskQueue搬运value |

- 执行结果

  执行结果如下，说明HashTable插入和value校验成功。
  ```bash
  Verification PASSED
  ```

### 性能分析

使用 `msprof op` 工具获取详细性能数据。

```bash
msprof op ./hash_table_mte_value_move 128
```

两阶段MTE搬运value的Case包含 `insert_or_assign_probe_kernel` 和 `write_values_mte_queue_kernel` 两个kernel，需要指定 `--launch-count=2`，确保同一份结果中采集两个kernel。

```bash
msprof op --launch-count=2 ./hash_table_mte_value_move 128
```

MTETaskQueue搬运value的Case是单kernel，使用默认 `launch-count=1` 即可。

当前目录下会生成 `OPPROF_` 前缀的文件夹，保存当前kernel的性能分析数据。

```bash
OPPROF_xxxx_XXXXXXXX
├── ArithmeticUtilization.csv
├── L2Cache.csv
├── Memory.csv
├── MemoryL0.csv
├── MemoryUB.csv
├── OpBasicInfo.csv
├── PipeUtilization.csv
└── ResourceConflictRatio.csv
```

查看具体的性能分析结果：
```bash
# 查看Task Duration等基础信息
cat ./OPPROF_*/OpBasicInfo.csv

# 查看Vector/MTE等流水耗时和占比
cat ./OPPROF_*/PipeUtilization.csv
```
