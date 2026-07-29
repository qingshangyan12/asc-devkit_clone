# L2 Cache切分<a name="ZH-CN_TOPIC_0000001893038949"></a>

【优先级】：高

【描述】在部分产品上，L2 Cache读写混合带宽和GM带宽处于不同量级，例如理论或典型带宽约为7TB/s和1.6TB/s。上述数据仅用于说明访问层级之间的带宽差异，不表示所有产品或实际算子的可达带宽。实际性能还受到芯片型号、访存模式、并发流量和工具统计口径等因素影响。

当算子需要多次访问同一批数据时，关键是重复访问发生时目标数据是否仍驻留在L2 Cache中。如果首次访问后，继续顺序访问的大数据量超过L2 Cache有效容量，早先进入L2 Cache的数据可能被后续数据替换；再次访问早先地址时就可能出现Cache Miss，需要从GM重新取数，数据搬入可能成为性能瓶颈。

优化的核心思想是：**不要把超过L2 Cache有效容量的大数据范围作为一次跨轮复用的整体工作集，而是根据L2Cache有效容量按有机会驻留的粒度切分，并尽快完成同一批数据的多次访问**。本文和配套样例将单批L2 Cache有效工作集阈值设为100MB，但该值不是所有产品的固定物理L2 Cache容量。实际算子应结合目标产品、并发业务和Profile结果确定阈值。

## 场景设定

为便于理解，以下正反例基于同一固定场景：

- 使用一份384MB的GM数据Buffer，每轮在同一Buffer上原地计算。
- 每个元素执行两轮“加1”运算，最终结果为`input + 2`。
- 使用20个Vector Core并行计算，每个核处理一段连续且互不重叠的GM地址范围。
- 每个核内部再按Unified Buffer容量切分为UB Tile，UB Tile仅解决UB容量限制，不代表L2 Cache切分粒度。
- 反例不做L2 Cache切分，每轮把384MB数据作为一个整体，从`data[0]`所在地址范围连续扫描到384MB末尾。
- 正例将384MB数据切成B0至B3四个L2 Batch，每批是一段96MB的连续索引范围，并在切换到下一批前完成同一批的两轮计算。
- 每个阶段结束后执行全核同步，保证下一阶段开始前所有核均已完成当前阶段的UB->GM搬运。反例按整份数据每轮同步，正例按每个L2 Batch和每轮组合同步。

## 数据量和L2 Cache阈值

样例在GM中分配一份名为`data`的数据Buffer，包含201326592个`half`元素，共占384MB。这里的384MB就是实际待处理的数据量，也是输入文件和输出文件的大小。

单批L2 Cache有效工作集阈值设为100MB。整份384MB数据超过该阈值，如果不切分，跨轮复用的活跃工作集就是384MB：

```text
不切分活跃工作集 = 384MB
L2 Cache有效工作集阈值 = 100MB
```

正例按阈值向上取整得到4个L2 Batch，再将384MB数据等分为4个96MB L2 Batch：

```text
L2 Batch数       = ceil(总数据量 / L2 Cache有效工作集阈值)
                  = ceil(384MB / 100MB)
                  = 4
单个L2 Batch大小 = 384MB / 4 = 96MB
```

**图 1**  384MB数据的不切分和L2 Batch切分关系

```text
不切分反例：

data 384MB  |---------------------------- 384MB ----------------------------|
              一轮内沿完整GM范围连续扫描，活跃工作集超过100MB有效阈值

L2切分正例：

                               L2 Cache Batch维度
                  B0             B1             B2             B3
             +--------------+--------------+--------------+--------------+
data 384MB  | 96MB        | 96MB        | 96MB        | 96MB        |
             +--------------+--------------+--------------+--------------+

每个L2 Batch = 96MB <= 100MB有效阈值
```

**表 1**  样例数据量口径

| 数据量 | 计算方法 | 结果 |
| ------ | ------ | -----: |
| 总数据量 | 样例固定值 | 384MB |
| 总元素数 | `384MB / sizeof(half)` | 201326592 |
| L2 Cache有效工作集阈值 | 样例固定值 | 100MB |
| 反例活跃工作集 | 不做L2 Cache切分 | 384MB |
| 正例L2 Batch数 | `ceil(384MB / 100MB)` | 4 |
| 正例单个L2 Batch数据量 | `384MB / 4` | 96MB |
| 正例单个L2 Batch元素数 | `96MB / sizeof(half)` | 50331648 |

## 为什么不切分会降低复用概率

不切分反例把384MB作为一个整体。第1轮处理时，20个核分别处理相邻子区间，从全局地址范围看会覆盖`data[0, 384MB)`。每个核内部虽然一次只把一个UB Tile搬入UB，但循环会不断访问新的GM地址段；随着地址从起始位置推进到384MB末尾，L2 Cache持续被后续数据填充。

由于384MB超过100MB有效工作集阈值，第1轮早期访问过的数据在本轮后续扫描过程中就可能被替换。第2轮再次从`data[0]`所在地址范围开始时，早期数据不一定仍在L2 Cache中，更可能发生Cache Miss并从GM重新取数。

L2 Cache切分正例将跨轮复用的工作集限制在96MB。先在`data[B0]`上完成第1轮和第2轮，再依次处理B1至B3。同一批两轮访问之间没有其他大数据范围插入，因此第2轮更有机会直接命中第1轮刚写回的L2 Cache数据。

## GM到L2 Cache的数据量

每轮读取并写回整份384MB数据。两轮计算的逻辑搬运量为：

```text
逻辑GM->UB读量 = 384MB/轮 * 2轮 = 768MB
逻辑UB->GM写量 = 384MB/轮 * 2轮 = 768MB
逻辑搬运总量   = 768MB + 768MB = 1536MB
```

`DataCopy`从GM地址读取数据时会经过L2 Cache，因此Profile中的逻辑GM->UB读量约为768MB。但是，逻辑GM->UB读量不等于实际从主存储器搬入L2 Cache的数据量：L2 Cache命中时直接从L2 Cache读取，只有Cache Miss才需要从主存储器取数并填充L2 Cache。

仅考虑MTE2读取，在冷Cache、忽略Cache Line粒度和其他系统流量的理想分析中，首轮读取的384MB需要从主存储器取数。不切分反例在两轮之间完整扫描384MB范围，第2轮早期地址更可能再次从主存储器读取，实际GM->L2读量可接近768MB。L2切分正例在每个96MB L2 Batch写回后立即开始同一批的第2轮，因此实际GM->L2读量可接近384MB。实际数值取决于命中率、替换策略和同期流量，应通过L2 Cache命中率及主存储器读计数共同确认。

## 公共处理逻辑

两种实现都使用同一份`data` Buffer，并将结果写回搬入时的同一GM地址。差异在于传给公共处理函数的连续数据范围不同：反例传入完整384MB范围，正例传入单个96MB L2 Batch范围。代码展示实际样例采用的32字节DataBlock分核公式以及尾块长度计算方式；为突出数据通路，仅省略了流水同步代码。

```cpp
__aicore__ inline void ProcessWholeData()
{
    ProcessDataRange(0, TOTAL_DATA_ELEMENTS);
}

__aicore__ inline void ProcessL2Batch(uint32_t l2BatchIdx)
{
    ProcessDataRange(l2BatchIdx * L2_BATCH_ELEMENTS, L2_BATCH_ELEMENTS);
}

__aicore__ inline void ProcessDataRange(int64_t rangeOffsetInData, uint32_t rangeElementNum)
{
    const uint32_t coreIdx = AscendC::GetBlockIdx();
    const uint32_t rangeDataBlockNum = rangeElementNum / ELEMENTS_PER_DATA_BLOCK;
    const uint32_t baseDataBlocksPerCore = rangeDataBlockNum / VECTOR_CORE_NUM;
    const uint32_t coresWithExtraDataBlock = rangeDataBlockNum % VECTOR_CORE_NUM;
    const uint32_t extraDataBlockNumBeforeCore =
        coreIdx < coresWithExtraDataBlock ? coreIdx : coresWithExtraDataBlock;
    const uint32_t coreDataBlockNum =
        baseDataBlocksPerCore + (coreIdx < coresWithExtraDataBlock ? 1U : 0U);
    const uint32_t coreDataBlockOffset =
        coreIdx * baseDataBlocksPerCore + extraDataBlockNumBeforeCore;
    const uint32_t coreElementNum = coreDataBlockNum * ELEMENTS_PER_DATA_BLOCK;
    const int64_t coreOffsetInData =
        rangeOffsetInData + static_cast<int64_t>(coreDataBlockOffset) * ELEMENTS_PER_DATA_BLOCK;
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
        AscendC::DataCopy(inputLocal, dataGm[gmElementOffset], tileElementNum);
        AscendC::Adds(outputLocal, inputLocal, half(1.0), tileElementNum);
        AscendC::DataCopy(dataGm[gmElementOffset], outputLocal, tileElementNum);
    }
}
```

对于正例的单个96MB L2 Batch，前8个核各处理157287个DataBlock，其余12个核各处理157286个DataBlock，保证20个核覆盖完整L2 Batch且互不重叠。对于不切分反例的384MB整体范围，20个核覆盖完整`data[0, 384MB)`范围且互不重叠。每个DataBlock包含16个`half`元素，因此所有`DataCopy`长度均满足32字节对齐要求。完整流水同步实现以配套样例为准。

## 反例：不做L2 Cache切分

反例的循环层级为`Round -> Whole Data -> UB Tile`。外层每轮都处理完整384MB范围：

```cpp
for (uint32_t roundIdx = 0; roundIdx < COMPUTE_ROUND_NUM; ++roundIdx) {
    ProcessWholeData();
    AscendC::SyncAll(syncGm, syncLocal, VECTOR_CORE_NUM);
}
```

**图 2**  不切分反例的执行时间线

```text
R0/data[0, 384MB) -> R1/data[0, 384MB)
  |------ 一轮内连续扫描384MB，超过L2 Cache有效阈值 ------|
```

R0从起始地址范围扫描到384MB末尾的过程中，后续数据可能替换早先数据。R1重新访问起始地址范围时，早先数据更可能已经不在L2 Cache中，因此更可能从GM重新搬运。实际命中情况受L2 Cache容量、替换策略和同期流量影响，应以Profile结果为准。

## 正例：按L2 Cache切分

正例的循环层级为`L2 Batch -> Round -> UB Tile`。同一批连续完成全部轮次后，再切换到下一批：

```cpp
for (uint32_t l2BatchIdx = 0; l2BatchIdx < L2_BATCH_NUM; ++l2BatchIdx) {
    for (uint32_t roundIdx = 0; roundIdx < COMPUTE_ROUND_NUM; ++roundIdx) {
        ProcessL2Batch(l2BatchIdx);
        AscendC::SyncAll(syncGm, syncLocal, VECTOR_CORE_NUM);
    }
}
```

**图 3**  L2 Cache切分正例的执行时间线

```text
B0/R0 -> B0/R1 -> B1/R0 -> B1/R1 -> B2/R0 -> B2/R1 -> B3/R0 -> B3/R1
  |         |        |         |        |         |        |         |
  +-- B0 ---+        +-- B1 ---+        +-- B2 ---+        +-- B3 ---+
```

B0在两次访问之间没有其他大数据范围插入，因此R1更有机会从L2 Cache读取R0的结果。B0处理完毕后再依次处理B1至B3，原理相同。

## 正反例对比总结

**表 2**  正反例对比

| 维度 | 反例 | 正例 |
| ------ | ------ | ------ |
| L2 Cache切分 | 不切分 | 切分为4个96MB L2 Batch |
| 循环层级 | `Round -> Whole Data -> UB Tile` | `L2 Batch -> Round -> UB Tile` |
| 跨轮复用工作集 | 384MB | 96MB |
| 执行时间线 | `R0/data[0,384MB) -> R1/data[0,384MB)` | `B0/R0 -> B0/R1 -> ... -> B3/R0 -> B3/R1` |
| 同一批两次访问之间 | 一轮内已连续扫描完整384MB范围 | 不插入其他大数据范围 |
| L2 Cache数据替换风险 | 较高 | 较低 |
| 第2轮L2 Cache命中率 | 预期较低，以实测为准 | 预期较高，以实测为准 |

## 核心原则

尽量保证数据从首次加载到末次使用期间驻留在L2 Cache中。总工作集超过有效阈值且同一批数据需要多次访问时，不应把全量数据作为一个跨轮复用整体；应按能够驻留的粒度切分，采用L2 Batch优先、Round次之、UB Tile最内层的循环层级。

## 适用场景

- 总工作集超过目标算子的L2 Cache有效阈值，且同一批数据需要被多次读取，例如多轮迭代计算、Reduce和卷积等。
- 只读取一次的数据不存在跨轮复用，调整L2 Cache切分顺序通常收益有限。

## 注意事项

1. **有效阈值的确定**：100MB仅为本文样例值。实际阈值需要计入输入、输出和其他会占用L2 Cache的缓冲区，并通过目标环境实测确定。

2. **批次数与开销的平衡**：批次过大会降低重复访问的命中概率，批次过细会增加同步和换批开销。建议在命中率改善的前提下减少不必要的批次。

3. **与UB Tiling的关系**：L2 Cache级分批控制跨轮复用的工作集，最内层UB Tiling解决Unified Buffer容量问题。即使一次只搬运一个UB Tile，连续扫描的总地址范围仍可能填满并替换L2 Cache。

4. **核间同步**：所有核共同处理同一个数据阶段时，需要在进入下一阶段前完成核间同步。配套样例使用`AscendC::SyncAll`，并为每个核申请32字节GM同步空间。不要使用仅保证单核流水顺序的`PipeBarrier`替代核间同步。

5. **CacheMode配合**：对于只访问一次的数据，可以评估使用`SetL2CacheHint(CACHE_MODE_DISABLE)`避免占用L2 Cache。具体支持范围和约束请参考[设置合理的L2 CacheMode](./设置合理的L2-CacheMode.md)。

## 测试用例设计

测试保留固定负载下的一组正反例。两者使用相同的一份大小为384MB的GM数据Buffer、核数、Round数、UB Tile处理逻辑和计算逻辑。唯一核心差异是反例不做L2 Cache切分，正例按100MB有效工作集阈值切分为4个96MB L2 Batch。

### 正反例用例

**表 3**  正反例用例

| 用例 | 循环层级 | 执行时间线 | 数据元素数 | 数据量 | 核数 | L2 Cache切分 | Round数 | 验证目的 |
| ------ | ------ | ------ | -------: | -------: | -----: | ------ | -----: | ------ |
| 反例no-split | `Round -> Whole Data -> UB Tile` | `R0/data[0,384MB) -> R1/data[0,384MB)` | 201326592 | 384MB | 20 | 不切分 | 2 | 验证384MB整体工作集超过L2 Cache有效阈值时的替换风险 |
| 正例L2-split | `L2 Batch -> Round -> UB Tile` | `B0/R0 -> B0/R1 -> ... -> B3/R0 -> B3/R1` | 201326592 | 384MB | 20 | 4个96MB Batch | 2 | 验证同一L2 Batch连续复用带来的L2 Cache局部性改善 |

正例每个L2 Batch包含50331648个`half`元素，占96MB。以32字节DataBlock为最小单位分配给20个Vector Core，前8个核各处理2516592个元素，其余12个核各处理2516576个元素。反例每轮直接处理完整384MB范围，20个核共同覆盖`data[0,384MB)`。所有`DataCopy`长度均保持32字节对齐。

### 验证检查点

**表 4**  验证检查点

| 检查项 | 验证方法 | 预期结果 |
| ------ | ------ | ------ |
| A/B公平性 | 汇总`GM_to_UB_datas(KB)`逻辑搬运量 | 每组差异不超过1% |
| L2 Cache命中率 | 独立进程采集L2 Cache读命中指标 | 正例中位数通常高于反例，具体字段按架构和工具版本解释 |
| 搬运效率 | 对比`aiv_mte2_time(us)`和有效带宽 | 正例MTE2耗时中位数通常降低，有效带宽通常提升 |
| 端到端耗时 | 正反例各采集多组独立Profile并比较`Task Duration(us)`中位数 | 搬运受限场景下，正例耗时通常低于反例 |

> **说明**：`aiv_gm_to_ub_bw(GB/s)`表示有效带宽，不表示GM读取量。优化后有效带宽可能升高，不能以“带宽更低”作为预期。性能采集应交替安排正反例的先后顺序，并使用多组结果的中位数降低系统波动影响。完整实现和Profile采集方法请参考[add_l2_cache_split样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/05_best_practices/00_vector_compute/add_l2_cache_split)。
