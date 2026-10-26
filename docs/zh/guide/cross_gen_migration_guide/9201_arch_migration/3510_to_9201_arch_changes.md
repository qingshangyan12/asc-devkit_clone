# 3510到9201架构变更

相比3510架构，9201架构中影响算子迁移的主要变更如下。开发者应先定位算子使用的功能，再按对应迁移章节进行适配。

## 核配比与数据搬运

**表1** 核配比与数据搬运变更

| 9201变更 | 产生的影响 | 影响的API接口 | 迁移指导 |
|----------|------------|---------------|----------|
| MIX场景的AIC:AIV配比由1:2变为1:1。 | 每个逻辑AI Core对应一个AIV。原AIV0/AIV1分工、按两个AIV切分数据、工作区寻址和核索引换算需要调整。 | GetBlockIdx、GetSubBlockNum、GetSubBlockIdx；asc_get_sub_block_num、asc_get_sub_block_id | [核索引与任务切分](./3510_to_9201_guide/basic_api_migration.md#core-index-migration) |
| L0C到UB不支持dualdst功能。 | 不能继续将一份Cube输出按原方式分发到两个AIV的UB；需要重新安排输出和Vector处理任务。 | Fixpipe、asc_copy_l0c2ub、Tensor API Copy | [L0C到UB搬运](./3510_to_9201_guide/basic_api_migration.md#l0c-to-ub-migration) |
| UB到L1、L1到UB的数据通路需要适配1:1配比。 | 原由两个AIV分别完成的搬运、偏移和缓冲区轮转需要重新计算。并非这些数据通路整体删除。 | DataCopy、asc_copy_l12ub、asc_copy_ub2l1 | [UB与L1搬运](./3510_to_9201_guide/basic_api_migration.md#ub-l1-migration) |
| 核间同步的参与者变化。 | mode=1/2/4相关场景中不能再等待同一逻辑AI Core内的第二个AIV；通知次数、等待次数和同步ID映射需要重新设计。 | CrossCoreSetFlag、CrossCoreWaitFlag；asc_sync_subblock_arrive/wait、asc_sync_block_arrive/wait、asc_sync_intra_arrive/wait | [核间同步](./3510_to_9201_guide/basic_api_migration.md#cross-core-sync-migration) |

## 存储单元

**表2** 存储单元变更

| 9201变更 | 产生的影响 | 影响的API接口 | 迁移指导 |
|----------|------------|---------------|----------|
| MIX场景SSBuffer由3510的3KB共享空间变为2KB，地址空间连续，为[0, 2KB)。 | 原AIC、AIV0、AIV1各1KB的偏移和总容量假设不再适用，可能越界或覆盖通信数据。 | GetSsbufBaseAddr | [SSBuffer布局](./3510_to_9201_guide/basic_api_migration.md#ssbuffer-migration) |
| UB物理容量由256KB变为384KB。 | 需要重新计算Tiling、缓冲区大小、循环次数、尾块和间接寻址范围。物理容量不能直接等同于算子可用容量。 | GetUBSizeInBytes、GetRuntimeUBSize；使用UB的搬运和计算接口 | [UB容量与Tiling](./3510_to_9201_guide/basic_api_migration.md#ub-tiling-migration) |

## 计算单元与指令

**表3** 计算单元与指令变更

| 9201变更 | 产生的影响 | 影响的API接口 | 迁移指导 |
|----------|------------|---------------|----------|
| 删除原Scalar Store原子配置对应的硬件能力。 | 设置/查询配置接口在9201为空实现，不能再依赖设置状态使后续普通Scalar Store成为原子操作。 | SetStoreAtomicConfig、GetStoreAtomicConfig | [Scalar原子操作](./3510_to_9201_guide/basic_api_migration.md#scalar-atomic-migration) |
| 归约及交织/去交织的原硬件指令删除，增加新版本指令。 | 原接口通过软仿兼容时可能多使用一个寄存器。寄存器压力增大可能引发UB溢出、性能下降或编译失败。 | Reduce、ReduceDataBlock、PairReduceElem、Interleave、DeInterleave，以及对应C API | [Regbase软仿资源开销](./3510_to_9201_guide/reg_api_migration.md#register-pressure-migration) |
| 移位指令删除负移位能力，硬件移位量改为无符号类型。 | 按接口约束使用非负移位量的场景可适配；依赖负移位行为的代码需要重写。b8标量移位需要防止窄化回绕；b64软仿可能额外使用两个寄存器。 | ShiftLeft、ShiftRight、ShiftLefts、ShiftRights，以及对应C API | [移位](./3510_to_9201_guide/reg_api_migration.md#shift-migration) |
| fp16的exp硬件指令实现变化。 | 部分输入的计算结果与3510相比存在1 ULP差异；使用fp16 Exp的相关激活函数也需要精度回归。 | Exp（基础API及Regbase API）、asc_exp、Silu、Swish、Gelu、FasterGelu | [Exp精度](./3510_to_9201_guide/reg_api_migration.md#exp-precision-migration) |

> [!NOTE]
> 归约、交织和移位的软仿开销取决于实际使用的重载、数据类型、编译器优化和寄存器活跃区间。不能将源码调用次数直接换算为性能损失，也不能把“软仿兼容”解释为“性能不变”。
