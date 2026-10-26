# 基础API迁移指导

本节针对3510到9201的核配比、数据搬运、同步、原子操作及存储空间变更提供适配方法。Regbase计算接口请参考[Regbase API迁移指导](./reg_api_migration.md)。

**表1** 基础API兼容性问题索引

| 功能分类 | 兼容性问题 | 涉及接口 | 迁移指导 |
|----------|------------|----------|----------|
| 原子操作 | Scalar Store原子配置在9201为空实现 | SetStoreAtomicConfig、GetStoreAtomicConfig | [使用显式Scalar原子操作](#scalar-atomic-migration) |
| 系统变量 | AIC:AIV由1:2改为1:1 | GetBlockIdx、GetSubBlockNum、GetSubBlockIdx | [核索引与任务切分](#core-index-migration) |
| 数据搬运 | L0C到UB不支持dualdst | Fixpipe | [重新分配Cube输出](#l0c-to-ub-migration) |
| 数据搬运 | 两个AIV的搬运分工失效 | DataCopy | [重算UB与L1搬运](#ub-l1-migration) |
| 核间同步 | 第二个AIV不再参与同步 | CrossCoreSetFlag、CrossCoreWaitFlag | [重建通知与等待关系](#cross-core-sync-migration) |
| 存储访问 | SSBuffer容量和布局变化 | GetSsbufBaseAddr | [重排SSBuffer](#ssbuffer-migration) |
| 资源查询 | UB容量及可用预算变化 | GetUBSizeInBytes、GetRuntimeUBSize | [重新计算Tiling](#ub-tiling-migration) |

## Scalar原子操作<a id="scalar-atomic-migration"></a>

**说明**：[SetStoreAtomicConfig](../../../../api/SIMD-API/basic_api/atomic_operations/SetStoreAtomicConfig_ISASI.md)和GetStoreAtomicConfig在9201为空实现。不能继续依赖其设置或查询原子开启位与操作类型，也不能通过保存和恢复该配置保证后续Scalar Store的原子性。

**兼容方案**：

- 将依赖上述配置的Scalar更新改为显式的AtomicAdd、AtomicMin、AtomicMax、AtomicCas或AtomicExch，并核对目标接口支持的数据类型和存储位置。
- 重新检查返回值语义、GM与DCache一致性，以及Scalar与搬运流水之间的数据依赖。显式原子操作不能替代算法所需的同步。
- 本项变更针对Scalar Store原子配置，不能据此将全部搬运原子接口机械替换为Scalar循环。

**迁移示例**：对于原先依赖Store原子加配置的GM标量累加，改为以下显式原子加。`counter`为有效的GM计数器地址。

```cpp
uint32_t increment = 1;
uint32_t oldValue = AscendC::AtomicAdd(counter, increment);
```

原子操作与DataCopy等接口存在数据依赖时，应按[AtomicAdd约束说明](../../../../api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md)处理缓存一致性并插入必要的同步。不要以普通的“读取、相加、写回”代替原子加。

## 核索引与任务切分<a id="core-index-migration"></a>

**说明**：对于启动`numBlocks`个逻辑AI Core的MIX算子，GetBlockIdx的范围发生如下变化。

**表2** MIX场景核索引范围

| 架构 | AIC上的GetBlockIdx | AIV上的GetBlockIdx | 每个逻辑AI Core的AIV数量 |
|------|-------------------|-------------------|-------------------------|
| 3510，1:2 | [0, numBlocks) | [0, 2 × numBlocks) | 2 |
| 9201，1:1 | [0, numBlocks) | [0, numBlocks) | 1 |

因此，原先通过`GetBlockIdx() / 2`计算逻辑AI Core索引，或者通过`GetSubBlockIdx() == 1`执行第二份任务的代码，需要修改。此范围比较针对上述MIX执行配置，不能推广到所有独立AIC/AIV内核。

**兼容方案**：使用[GetSubBlockNum](../../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSubBlockNum_ISASI.md)和[GetSubBlockIdx](../../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSubBlockIdx_ISASI.md)获取当前执行配置的信息，按实际参与的AIV数量切分任务。核数量查询只能解决索引换算，不能自动重写数据流、搬运和同步。

**迁移示例**：以下片段仅在MIX算子的AIV侧执行，`tileElements`表示一个逻辑AI Core负责的元素总数。

迁移前（3510）：

```cpp
int64_t logicalCoreIdx = AscendC::GetBlockIdx() / 2;
int64_t subBlockIdx = AscendC::GetSubBlockIdx();
int64_t begin = subBlockIdx * (tileElements / 2);
```

迁移后（按实际AIV数量切分）：

```cpp
int64_t subBlockNum = AscendC::GetSubBlockNum();
int64_t subBlockIdx = AscendC::GetSubBlockIdx();
int64_t logicalCoreIdx = AscendC::GetBlockIdx() / subBlockNum;
int64_t begin = tileElements * subBlockIdx / subBlockNum;
int64_t end = tileElements * (subBlockIdx + 1) / subBlockNum;
```

示例使用区间`[begin, end)`覆盖尾部元素。实际搬运还需满足对齐要求。若原AIV0负责反量化、AIV1负责输出等不同职责，应在9201的单个AIV内重新编排这两项工作，不能仅替换除数。

## L0C到UB搬运<a id="l0c-to-ub-migration"></a>

**说明**：9201不支持L0C到UB的dualdst功能。使用[Fixpipe](../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)将Cube结果分发给两个AIV的代码，需要按单个AIV重新设计。该变更不表示L0C到UB通路整体删除。

**兼容方案**：

1. 按一个AIV重新确定目的UB布局、输出分块和偏移，使用目标架构支持的单目的搬运配置。
2. 将原来两份Vector后处理安排到同一个AIV中，必要时减小每次搬运的块，分批处理。
3. 重新计算目的步长、量化或格式转换参数、双缓冲大小及同步关系，确认每份输出都只处理一次。

**迁移示例（流程伪代码）**：

```text
3510：AIC计算 -> dualdst搬运到AIV0/AIV1的UB
      -> 两个AIV分别处理输出的一部分 -> AIC等待对应完成通知

9201：AIC计算 -> 单目的搬运到唯一AIV的UB
      -> AIV分批完成全部后处理 -> 通知AIC可复用对应缓冲区
```

仅把dualdst配置关闭而保留原来一半的数据量，会导致输出遗漏；直接把单次数据量翻倍而不重算UB预算，也可能越界。

## UB与L1搬运<a id="ub-l1-migration"></a>

**说明**：原先由AIV0、AIV1分别完成的UB到L1或L1到UB搬运，需要适配1:1。DataCopy接口的影响应结合实际通路和调用参数判断，不能把所有DataCopy调用均归类为功能不兼容。

**兼容方案**：逐项核对以下内容。

- 原以`subBlockIdx * halfTileSize`寻址的源和目的偏移，以及两个AIV写入的L1区间。
- 每次搬运的块数、块大小、源/目的步长和尾块对齐。
- 原在两个UB中分别分配的空间能否在一个AIV上容纳；不足时采用分块搬运和缓冲区复用。
- 消费者何时可以读取L1/UB，生产者何时可以覆盖旧数据。搬运发起不等于数据已经可见。

当单个AIV分批处理原来两个AIV的数据时，应保证各批次的目的区间不重叠，并按[核间同步迁移](#cross-core-sync-migration)重建流水依赖。

## 核间同步<a id="cross-core-sync-migration"></a>

**说明**：CrossCoreSetFlag、CrossCoreWaitFlag及其对应C API中，涉及mode=1/2/4和两个AIV的同步逻辑需要重新检查。9201的一个逻辑AI Core内只有一个AIV，不存在原AIV0与AIV1之间的参与关系。不能将该结论理解为这些同步接口全部失效。

**兼容方案**：

1. 列出每个同步点的生产者、消费者、数据区间和保护的缓冲区。
2. 将AIV0/AIV1合并为单个AIV，删除对不存在参与者的等待；保留AIC与AIV之间仍然需要的数据依赖。
3. 重新计算每轮的通知和等待次数，保证同一缓冲区每次复用前都有对应的完成通知。
4. 根据目标SDK核对同步模式、ID范围及映射。不要沿用3510中为AIV1保留的ID偏移，例如直接使用`syncId + 16`。
5. 覆盖尾块、空任务、提前返回和缓冲区绕回路径，确保这些路径也满足成对同步。

**迁移示例（流程伪代码）**：

```text
3510：AIV0完成第一部分并通知；AIV1完成第二部分并通知；AIC等待两部分完成。
9201：唯一AIV完成两部分并通知；AIC等待这一完整任务完成。
```

原两个AIV使用的独立流水在单个AIV中顺序执行后，还需补充必要的核内流水同步。不能用“删除一次wait”代替完整的数据依赖分析。

## SSBuffer布局<a id="ssbuffer-migration"></a>

**说明**：[GetSsbufBaseAddr](../../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSsbufBaseAddr.md)返回基地址，不会自动调整用户的偏移和数据结构。3510的MIX场景可共享3KB，9201为连续的2KB空间，合法地址范围为`[0, 2 * 1024)`。

**兼容方案**：

- 删除AIC/AIV0/AIV1各1KB的固定分区假设，统一规划AIC与唯一AIV的通信空间。
- 核对`offset + size <= 2 * 1024`、接口访问对齐以及通信结构体大小，预留配套库占用的区域，避免与库内部通信空间冲突。
- 对原来占用超过2KB的用户数据，缩减通信字段、分阶段复用空间，或者移至显式管理的GM工作区，并补全相应同步和缓存一致性处理。
- 不要通过构造更大的指针偏移或沿用旧基地址映射来模拟3KB空间。SDK内部的兼容实现不表示用户可以访问超出硬件范围的地址。

**迁移示例**：将用户通信区的布局常量与目标架构隔离，并在编译期检查边界。`kUserCommOffset`和`kUserCommBytes`由实际通信布局确定。

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9201)
constexpr uint32_t kSsbufLimit = 2 * 1024;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
constexpr uint32_t kSsbufLimit = 3 * 1024;
#endif
static_assert(kUserCommOffset <= kSsbufLimit);
static_assert(kUserCommBytes <= kSsbufLimit - kUserCommOffset);
```

以上容量针对MIX场景。实际可用区间还应扣除库占用，并满足目标接口的对齐约束。

## UB容量与Tiling<a id="ub-tiling-migration"></a>

**说明**：9201的UB物理容量由256KB变为384KB。[GetUBSizeInBytes](../../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetUBSizeInBytes.md)与[GetRuntimeUBSize](../../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetRuntimeUBSize.md)接口保留，但查询口径不同：前者提供架构容量信息，后者用于获取运行时UB空间。算子预算还需考虑执行模式、运行时及栈保留、临时空间和缓冲区数量。

> [!NOTE]
> 3510文档中的非混合场景248KB、SIMD与SIMT混合场景上限216KB描述的是3510的运行时可用空间，不能直接作为9201的返回值。也不能直接以384KB作为9201算子全部可分配的空间。应按配套SDK的查询结果和分配约束确定预算，避免重复扣除已由接口预留的空间。

**兼容方案**：

1. Host侧通过目标产品的平台能力查询获取资源，重新计算Tiling。Device侧使用运行时容量的场景，保证实际访问不超过Host侧已规划和分配的缓冲区。
2. 对输入、输出、中间结果、流水多级缓冲及软仿临时空间统一计入预算，容量增加后不要直接按比例放大tile。
3. 根据实际tile长度计算循环次数和尾块，移除固定迭代次数及按旧UB大小推导的边界。
4. 排查DataCopyGather等间接寻址操作的索引位宽和单位。u16索引能覆盖的字节范围取决于索引单位；不能认为UB扩大后原索引就能覆盖全部空间。超出时分窗口处理，或使用公开接口支持的更宽索引。

**迁移示例**：`tileLength`为已完成UB预算与对齐检查的Tiling结果，`totalLength`为当前任务的元素总数。循环按剩余数据长度处理，避免固定次数和向下取整漏掉尾块。

```cpp
// 前提：tileLength > 0，所有局部缓冲区均可容纳tileLength个元素。
uint64_t offset = 0;
while (offset < totalLength) {
    uint64_t remaining = totalLength - offset;
    uint64_t currentLength = remaining < tileLength ? remaining : tileLength;
    // 按currentLength配置搬运、计算mask及有效输出，处理对齐和尾块。
    offset += currentLength;
}
```

应至少覆盖空输入、单tile、整tile、多tile及尾块，以及SIMD与SIMT混合配置。容量变大不代表所有循环次数都应增加或减少，应根据具体循环处理的维度重新推导。
