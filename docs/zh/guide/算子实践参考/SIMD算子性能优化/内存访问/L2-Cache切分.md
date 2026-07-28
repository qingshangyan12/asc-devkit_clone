# L2 Cache切分<a name="ZH-CN_TOPIC_0000001893038949"></a>

【优先级】：高

【描述】在部分产品上，L2 Cache读写混合带宽和GM带宽处于不同量级，例如理论或典型带宽约为7TB/s和1.6TB/s。上述数据仅用于说明访问层级之间的带宽差异，不表示所有产品或实际算子的可达带宽。实际性能还受到芯片型号、访存模式、并发流量和工具统计口径等因素影响。

当算子需要多次访问同一批数据时，关键是重复访问发生时目标数据是否仍驻留在L2 Cache中。如果首次访问后，L2 Cache被其他数据填满并发生替换，再次访问就可能出现Cache Miss，需要从GM重新搬运，数据搬入可能成为性能瓶颈。

优化的核心思想是：**尽量缩短一批数据从首次加载到末次使用之间的距离，使其有机会驻留在L2 Cache中**。实现时需要控制任一阶段的活跃工作集。本文和配套样例将单批L2 Cache有效工作集阈值设为100MiB，但该值不是所有产品的固定物理L2 Cache容量。实际算子应结合目标产品、并发业务和Profile结果确定阈值。

## 场景设定

为便于理解，以下正反例基于同一固定场景：

- 使用一份384MiB的GM数据Buffer，每轮在同一Buffer上原地计算。
- 每个元素执行两轮“加1”运算，最终结果为`input + 2`。
- 全量索引范围分为B0至B3四个L2 Batch，每批是一段96MiB的连续索引范围。
- 使用20个Vector Core并行计算，每批数据均分到所有核，每核再按Unified Buffer容量切分为UB Tile。
- 每个L2 Batch与Round组合结束后执行全核同步，保证下一阶段开始前，所有核均已完成当前阶段的UB->GM搬运。

### 数据量和L2 Cache阈值

样例在GM中分配一份名为`data`的数据Buffer，包含201326592个`half`元素，共占384MiB。这里的384MiB就是实际待处理的数据量，也是输入文件和输出文件的大小。

单个L2 Batch的有效工作集阈值为100MiB。L2 Batch数先按总数据量与阈值向上取整，再将384MiB数据等分为4个96MiB L2 Batch：

```text
L2 Batch数       = ceil(总数据量 / L2 Cache有效工作集阈值)
                  = ceil(384MiB / 100MiB)
                  = 4
单个L2 Batch大小 = 384MiB / 4 = 96MiB
```

**图 1**  384MiB数据的L2 Batch切分关系

```text
GM数据：384MiB

                               L2 Cache Batch维度
                  B0             B1             B2             B3
             +--------------+--------------+--------------+--------------+
data 384MiB  | 96MiB        | 96MiB        | 96MiB        | 96MiB        |
             +--------------+--------------+--------------+--------------+

每个L2 Batch = 96MiB <= 100MiB有效阈值
```

因此，`TOTAL_DATA_ELEMENTS`为201326592，`L2_BATCH_ELEMENTS`为50331648。B0至B3分别对应`data`上一段连续且不重叠的索引范围，四段范围恰好覆盖全部数据。

**表 1**  样例数据量口径

| 数据量 | 计算方法 | 结果 |
| ------ | ------ | -----: |
| 总数据量 | 样例固定值 | 384MiB |
| 总元素数 | `384MiB / sizeof(half)` | 201326592 |
| L2 Cache有效工作集阈值 | 样例固定值 | 100MiB |
| L2 Batch数 | `ceil(384MiB / 100MiB)` | 4 |
| 单个L2 Batch数据量 | `384MiB / 4` | 96MiB |
| 单个L2 Batch元素数 | `96MiB / sizeof(half)` | 50331648 |

### 为什么需要切分B0至B3

切分B0至B3的直接原因是待处理数据为384MiB，超过100MiB有效阈值：

```text
待处理数据量           = 384MiB
L2 Cache有效工作集阈值 = 100MiB
L2 Batch数             = ceil(384MiB / 100MiB) = 4
```

B0至B3是`data`上的四个连续索引区间，不是四份新的GM Buffer。按L2 Batch优先执行时，先在`data[B0]`上原地完成第1轮和第2轮计算，再依次处理B1至B3。同一批两轮访问之间没有其他大数据批次插入，所以第2轮更有机会直接命中第1轮刚写回的L2 Cache数据。

### GM到L2 Cache的数据量

每轮读取并写回整份384MiB数据。两轮计算的逻辑搬运量为：

```text
逻辑GM->UB读量 = 384MiB/轮 * 2轮 = 768MiB
逻辑UB->GM写量 = 384MiB/轮 * 2轮 = 768MiB
逻辑搬运总量   = 768MiB + 768MiB = 1536MiB
```

`DataCopy`从GM地址读取数据时会经过L2 Cache，因此Profile中的逻辑GM->UB读量约为768MiB。但是，逻辑GM->UB读量不等于实际从主存储器搬入L2 Cache的数据量：L2 Cache命中时直接从L2 Cache读取，只有Cache Miss才需要从主存储器取数并填充L2 Cache。

仅考虑MTE2读取，在冷Cache、忽略Cache Line粒度和其他系统流量的理想分析中，首轮读取的384MiB需要从主存储器取数。正例在每个96MiB L2 Batch写回后立即开始第2轮，因此实际GM->L2读量可接近384MiB；反例在同一批两次访问之间插入其他三个96MiB L2 Batch，第2轮更可能再次从主存储器读取，实际GM->L2读量可接近768MiB。实际数值取决于命中率、替换策略和同期流量，应通过L2 Cache命中率及主存储器读计数共同确认。

两种实现的公共单批处理逻辑如下。代码展示实际样例采用的32字节DataBlock分核公式，以及尾块长度的计算方式；为突出数据通路，仅省略了流水同步代码。

```cpp
__aicore__ inline void ProcessL2Batch(uint32_t l2BatchIdx)
{
    const uint32_t coreIdx = AscendC::GetBlockIdx();
    const uint32_t extraDataBlockNumBeforeCore =
        coreIdx < CORE_NUM_WITH_ONE_EXTRA_DATA_BLOCK ?
            coreIdx : CORE_NUM_WITH_ONE_EXTRA_DATA_BLOCK;
    const uint32_t coreDataBlockNum = BASE_DATA_BLOCK_NUM_PER_CORE +
        (coreIdx < CORE_NUM_WITH_ONE_EXTRA_DATA_BLOCK ? 1U : 0U);
    const uint32_t coreDataBlockOffset =
        coreIdx * BASE_DATA_BLOCK_NUM_PER_CORE + extraDataBlockNumBeforeCore;
    const uint32_t coreElementNum = coreDataBlockNum * ELEMENTS_PER_DATA_BLOCK;
    const int64_t coreOffsetInData =
        static_cast<int64_t>(l2BatchIdx) * L2_BATCH_ELEMENTS +
        static_cast<int64_t>(coreDataBlockOffset) * ELEMENTS_PER_DATA_BLOCK;
    ProcessCoreRange(coreOffsetInData, coreElementNum);
}

__aicore__ inline void ProcessCoreRange(int64_t coreOffsetInData, uint32_t coreElementNum)
{
    const uint32_t ubTileNum =
        (coreElementNum + UB_TILE_CAPACITY_ELEMENTS - 1) / UB_TILE_CAPACITY_ELEMENTS;
    for (uint32_t ubTileIdx = 0; ubTileIdx < ubTileNum; ++ubTileIdx) {
        const uint32_t processedElementNum = ubTileIdx * UB_TILE_CAPACITY_ELEMENTS;
        const uint32_t remainingElementNum = coreElementNum - processedElementNum;
        const uint32_t tileElementNum = remainingElementNum > UB_TILE_CAPACITY_ELEMENTS ?
                                            UB_TILE_CAPACITY_ELEMENTS : remainingElementNum;
        const int64_t gmElementOffset = coreOffsetInData + processedElementNum;
        AscendC::DataCopy(inputLocal_, dataGm_[gmElementOffset], tileElementNum);
        AscendC::Adds(outputLocal_, inputLocal_, half(1.0), tileElementNum);
        AscendC::DataCopy(dataGm_[gmElementOffset], outputLocal_, tileElementNum);
    }
}
```

前8个核各处理157287个DataBlock，其余12个核各处理157286个DataBlock，保证20个核覆盖完整L2 Batch且互不重叠。每个DataBlock包含16个`half`元素，因此所有`DataCopy`长度均满足32字节对齐要求。完整流水同步实现以配套样例为准。

## 反例：按Round优先遍历

反例的循环层级为`Round -> L2 Batch -> UB Tile`。外层先完成一轮全量数据处理，再开始下一轮：

```cpp
for (uint32_t roundIdx = 0; roundIdx < COMPUTE_ROUND_NUM; ++roundIdx) {
    for (uint32_t l2BatchIdx = 0; l2BatchIdx < L2_BATCH_NUM; ++l2BatchIdx) {
        ProcessL2Batch(l2BatchIdx);
        AscendC::SyncAll(syncGm, syncLocal, VECTOR_CORE_NUM);
    }
}
```

**图 2**  Round优先的执行时间线

```text
R0/B0 -> R0/B1 -> R0/B2 -> R0/B3 -> R1/B0 -> R1/B1 -> R1/B2 -> R1/B3
  |                                  |
  +--------------- B0 --------------+-- 再次访问B0前插入了B1至B3，B0可能已被替换。
```

R0处理B1至B3时，这些L2 Batch可能替换R0阶段写入L2 Cache的B0。R1再次访问B0时，更可能从GM重新搬运。其他L2 Batch也存在相同问题。实际命中情况受L2 Cache容量、替换策略和同期流量影响，应以Profile结果为准。

## 正例：按L2 Batch优先遍历

正例只交换L2 Cache级循环顺序，循环层级变为`L2 Batch -> Round -> UB Tile`。同一批连续完成全部轮次后，再切换到下一批：

```cpp
for (uint32_t l2BatchIdx = 0; l2BatchIdx < L2_BATCH_NUM; ++l2BatchIdx) {
    for (uint32_t roundIdx = 0; roundIdx < COMPUTE_ROUND_NUM; ++roundIdx) {
        ProcessL2Batch(l2BatchIdx);
        AscendC::SyncAll(syncGm, syncLocal, VECTOR_CORE_NUM);
    }
}
```

**图 3**  L2 Batch优先的执行时间线

```text
B0/R0 -> B0/R1 -> B1/R0 -> B1/R1 -> B2/R0 -> B2/R1 -> B3/R0 -> B3/R1
  |         |        |         |        |         |        |         |
  +-- B0 ---+        +-- B1 ---+        +-- B2 ---+        +-- B3 ---+
```

B0在两次访问之间没有其他L2 Batch插入，因此R1更有机会从L2 Cache读取R0的结果。B0处理完毕后再依次处理B1至B3，原理相同。

## 正反例对比总结

**表 2**  正反例对比

| 维度 | 反例 | 正例 |
| ------ | ------ | ------ |
| 循环层级 | `Round -> L2 Batch -> UB Tile` | `L2 Batch -> Round -> UB Tile` |
| 执行时间线 | `R0/B0 -> ... -> R0/B3 -> R1/B0 -> ... -> R1/B3` | `B0/R0 -> B0/R1 -> ... -> B3/R0 -> B3/R1` |
| 同一批两次访问之间 | 插入其他大数据批次 | 不插入其他大数据批次 |
| L2 Cache数据替换风险 | 较高 | 较低 |
| 第2轮L2 Cache命中率 | 预期较低，以实测为准 | 预期较高，以实测为准 |

## 核心原则

尽量保证数据从首次加载到末次使用期间驻留在L2 Cache中。对于任一批数据，其多次访问之间不应插入其他大数据批次。总工作集超过有效阈值时，按能够驻留的粒度分批，采用L2 Batch优先、Round次之、UB Tile最内层的循环层级。

## 适用场景

- 总工作集超过目标算子的L2 Cache有效阈值，且同一批数据需要被多次读取，例如多轮迭代计算、Reduce和卷积等。
- 只读取一次的数据不存在跨轮复用，调整循环顺序通常收益有限。

## 注意事项

1. **有效阈值的确定**：100MiB仅为本文样例值。实际阈值需要计入输入、输出和其他会占用L2 Cache的缓冲区，并通过目标环境实测确定。

2. **批次数与开销的平衡**：批次过大会降低重复访问的命中概率，批次过细会增加同步和换批开销。建议在命中率改善的前提下减少不必要的批次。

3. **与UB Tiling的关系**：L2 Cache级分批控制跨轮复用的工作集，最内层UB Tiling解决Unified Buffer容量问题，两者属于不同层级。

4. **核间同步**：所有核共同处理同一个L2 Batch时，需要在进入下一阶段前完成核间同步。配套样例使用`AscendC::SyncAll`，并为每个核申请32字节GM同步空间。不要使用仅保证单核流水顺序的`PipeBarrier`替代核间同步。

5. **CacheMode配合**：对于只访问一次的数据，可以评估使用`SetL2CacheHint(CACHE_MODE_DISABLE)`避免占用L2 Cache。具体支持范围和约束请参考[设置合理的L2 CacheMode](./设置合理的L2-CacheMode.md)。

## 测试用例设计

测试只保留固定负载下的一组正反例。两者使用相同的一份大小为384MiB的GM数据Buffer、核数、L2 Batch数、Round数、UB Tile处理逻辑和全核同步次数，唯一差异是L2 Cache级循环顺序。L2 Cache有效工作集阈值为100MiB。

### 正反例用例

**表 3**  正反例用例

| 用例 | 循环层级 | 执行时间线 | 数据元素数 | 数据量 | 核数 | L2 Batch数 | Round数 | 验证目的 |
| ------ | ------ | ------ | -------: | -------: | -----: | -------: | -----: | ------ |
| 反例round-first | `Round -> L2 Batch -> UB Tile` | `R0/B0 -> ... -> R0/B3 -> R1/B0 -> ... -> R1/B3` | 201326592 | 384MiB | 20 | 4 | 2 | 验证跨L2 Batch访问导致的L2 Cache局部性下降 |
| 正例L2-batch-first | `L2 Batch -> Round -> UB Tile` | `B0/R0 -> B0/R1 -> ... -> B3/R0 -> B3/R1` | 201326592 | 384MiB | 20 | 4 | 2 | 验证同一L2 Batch连续复用带来的L2 Cache局部性改善 |

每个L2 Batch包含50331648个`half`元素，占96MiB。以32字节DataBlock为最小单位分配给20个Vector Core，前8个核各处理2516592个元素，其余12个核各处理2516576个元素。每个核处理153个完整UB Tile和1个尾部UB Tile，完整Tile容量为16384个元素，尾部Tile分别包含9840或9824个元素，所有`DataCopy`长度均保持32字节对齐。

### 验证检查点

**表 4**  验证检查点

| 检查项 | 验证方法 | 预期结果 |
| ------ | ------ | ------ |
| A/B公平性 | 汇总`GM_to_UB_datas(KB)`逻辑搬运量 | 每组差异不超过1% |
| L2 Cache命中率 | 独立进程采集L2 Cache读命中指标 | 正例中位数通常高于反例，具体字段按架构和工具版本解释 |
| 搬运效率 | 对比`aiv_mte2_time(us)`和有效带宽 | 正例MTE2耗时中位数通常降低，有效带宽通常提升 |
| 端到端耗时 | 正反例各采集多组独立Profile并比较`Task Duration(us)`中位数 | 搬运受限场景下，正例耗时通常低于反例 |

> **说明**：`aiv_gm_to_ub_bw(GB/s)`表示有效带宽，不表示GM读取量。优化后有效带宽可能升高，不能以“带宽更低”作为预期。性能采集应交替安排正反例的先后顺序，并使用多组结果的中位数降低系统波动影响。完整实现和Profile采集方法请参考[add_l2_cache_split样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/05_best_practices/00_vector_compute/add_l2_cache_split)。
