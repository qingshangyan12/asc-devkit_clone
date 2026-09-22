# Issue1761样例仓多核GM访问排查清单

## 1. 排查范围与方法

- 排查目录：`examples`全目录，包括SIMD C++ API、SIMD C API、SIMT API及混合编程样例。
- 目标架构：DAV 2201，本机设备为Ascend 910B3，`ASCEND_VISIBLE_DEVICES=7`。
- 静态范围：共检查709个`.asc/.cpp/.c/.h/.hpp`源码文件；其中106个文件显式使用`GetBlockIdx()`，2个文件使用`InitOutput`，12个文件包含原子写模式，22个文件包含核间同步原语。
- 重点模式：多核kernel中未按`blockIdx`分片的GM写、多核对同一GM的普通写、原子写后无核间同步的读取，以及在`SyncAll/GroupBarrier`建立先行关系之前发生的同址访问。
- 动态验证：使用CANN 9.2.0、`dav-2201`和`mssanitizer racecheck`；插桩编译参数为`-g --sanitizer`。

> 本清单是针对确定性高风险模式的专项排查结果，不替代对每个样例全部运行时参数组合的racecheck回归。

## 2. 需要处理的问题

| 编号 | 状态 | 风险 | 代码位置 | 问题说明 | 修改建议 |
|---|---|---|---|---|---|
| MC-GM-001 | 已修复 | 高 | [group_barrier.asc:L43](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/group_barrier.asc:43) | 原代码由8个AIV在`SyncAll()`之前对同一`dstGlobal`执行完整`InitOutput`，产生`C(8,2)=28`条WAW。写入值虽然同为0，但没有核间先行关系。 | 仅允许`block0`执行初始化，随后所有核执行`SyncAll()`。当前分支已按此方案修改。 |
| MC-GM-002 | 建议修改 | 中 | [scalar_atomic_operations.asc:L33](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/06_atomic/scalar_atomic_operations/scalar_atomic_operations.asc:33)、[scalar_atomic_operations.asc:L44](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/06_atomic/scalar_atomic_operations/scalar_atomic_operations.asc:44) | 3个核先对`y[0]`执行`AtomicAdd/AtomicCas`，随后只进行核内`S_MTE2`流水同步便读取整个`yGlobal`。当前核的写已完成不代表其他核的原子写已完成，因此读取可能与其他核写入重叠。读入的`yLocal`后续未参与计算。 | 首选删除未使用的`yLocal`分配、`SetFlag/WaitFlag`和`DataCopy`。如果样例确实需要演示写后读取，则在所有核原子操作完成后增加受支持的核间屏障，再限定一个核读取并使用结果。 |

## 3. 修复后仍会报告但属于同步实现的访问

| 编号 | 代码位置 | 访问模式 | 判断 | 建议 |
|---|---|---|---|---|
| MC-GM-SYNC-001 | [group_barrier.asc:L55](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/group_barrier.asc:55)、[group_barrier.asc:L60](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/group_barrier.asc:60) | Arriver原子累加输出，Waiter在`GroupBarrier::Wait()`返回后读取。 | 功能上由GroupBarrier建立跨核先行关系；当前racecheck仍报告原子写与读取之间的RAW。 | 算子代码无需为该RAW改变同步逻辑；建议由mssanitizer确认GroupBarrier建模或提供抑制规则。 |
| MC-GM-SYNC-002 | [kernel_operator_group_barrier_impl.h:L84](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/impl/basic_api/dav_c220/core_mng/roc/kernel_operator_group_barrier_impl.h:84)、[kernel_operator_group_barrier_impl.h:L101](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/impl/basic_api/dav_c220/core_mng/roc/kernel_operator_group_barrier_impl.h:101) | Arriver原子更新`barWorkspace`，Waiter通过`dcci`失效缓存并轮询计数。 | 这是GroupBarrier在DAV 2201上的软件同步实现，轮询读与原子写会被racecheck识别为RAW，但过期值只会使循环继续。 | 保留实现；建议工具侧识别“原子计数+dcci轮询”同步惯用法，并在文档中说明此类报告。 |

## 4. 已核查的同址访问模式

以下代码确实存在多核访问同一GM地址，但具有明确的原子或核间同步语义，本次不判定为缺陷：

| 模式 | 代码位置 | 核查结论 |
|---|---|---|
| 多核原子Add/Max写同一输出 | [data_movement_with_atomic_operations.asc:L41](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/06_atomic/data_movement_with_atomic_operations/data_movement_with_atomic_operations.asc:41)、[data_movement_with_atomic_operations.asc:L75](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/06_atomic/data_movement_with_atomic_operations/data_movement_with_atomic_operations.asc:75) | 同址写是样例目标，使用`SetAtomicAdd/SetAtomicMax`，kernel内没有紧随其后的跨核读取。 |
| 顺序执行的同址原子累加 | [sequential_block_sync.asc:L62](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/sequential_block_sync/sequential_block_sync.asc:62) | `WaitPreBlock`和`NotifyNextBlock`显式规定block顺序，未发现无保护普通同址写。 |
| 原子归约后单核读取 | [cross_core_set_wait_flag.h:L318](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h:318)、[cross_core_set_wait_flag.h:L329](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/cross_core_set_wait_flag.h:329) | 原子累加后通过`CrossCoreSetFlag/CrossCoreWaitFlag`等待所有参与核完成，再由指定核读取。 |
| 标志发布与轮询读取 | [data_sync_barrier.asc:L42](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/data_sync_barrier/data_sync_barrier.asc:42)、[data_sync_barrier.asc:L51](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/data_sync_barrier/data_sync_barrier.asc:51) | 写侧使用`DataSyncBarrier<DDR>`后发布标志，读侧通过Bypass DCache轮询，属于显式软件同步。 |
| IBSet/IBWait保护跨核RAW | [ib_set_wait.asc:L45](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/ib_set_wait/ib_set_wait.asc:45)、[ib_set_wait.asc:L76](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/ib_set_wait/ib_set_wait.asc:76) | `block1`先执行`IBWait`，`block0`完成输出后执行`IBSet`，输出写与跨核读之间有同步。 |
| DCache标志轮询归约 | [gm_by_dcache.asc:L61](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/09_utils/gm_by_dcache/gm_by_dcache.asc:61)、[gm_by_dcache.asc:L82](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/09_utils/gm_by_dcache/gm_by_dcache.asc:82)、[gm_by_dcache.asc:L90](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/09_utils/gm_by_dcache/gm_by_dcache.asc:90) | 数据和标志均按`blockIdx`分片；`block0`通过DCache清理失效轮询所有标志后汇总读取。 |
| `SyncAll`后读取其他核分片 | [sync_all.asc:L54](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/sync_all/sync_all.asc:54)、[sync_all.asc:L58](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/03_basic_api/05_sync_control/sync_all/sync_all.asc:58) | 每个核先写独立workspace分片，`SyncAll()`后读取其他核分片，不存在无保护同址写。 |

## 5. 边界项

[matmul_ibshareAB.asc:L87](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_ibshareAB/matmul_ibshareAB.asc:87)在`SCENARIO_NUM=2`时对未偏移的`cGlobal`执行`InitOutput`，形态与Issue1761相似；但当前Host代码在[matmul_ibshareAB.asc:L211](/mnt/workspace/gitCode/GRJ_XIDUO/asc-devkit_6/examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_ibshareAB/matmul_ibshareAB.asc:211)固定`numBlocks=1`，因此当前不构成多核WAW。

建议在该样例未来启用多block前，将初始化限制为单核并在其后执行核间同步；同时让Host启动核数与tiling中的`usedCoreNum`保持一致。

## 6. DAV 2201验证结果

### group_barrier

- 普通编译：通过。
- NPU运行：通过，6个Waiter均输出`OUTPUT = 24`。
- sanitizer插桩编译：通过。
- racecheck：修复前Issue所述的28条初始化WAW已消失，当前日志中`WAW=0`；仍有36条GroupBarrier软件同步相关RAW，其中24条来自`barWorkspace`原子计数与轮询，12条来自Arriver写输出与Waiter在`Wait()`返回后读取输出，见第3节。

### scalar_atomic_operations

使用本机CANN 9.2.0按`dav-2201`编译时，`AtomicAdd<uint32_t>`链接失败，因此无法在本机执行racecheck。该项当前结论来自源码的核间先行关系分析，建议在与仓库API版本匹配的2201环境或其声明支持的目标架构上补充动态验证。

## 7. 后续建议

1. 合入MC-GM-001修复，并将带`--sanitizer`的racecheck加入该样例回归。
2. 按MC-GM-002删除无效读回，或补齐真正的核间屏障及单核读取逻辑。
3. 为GroupBarrier、DCache轮询等软件同步惯用法建立mssanitizer识别或抑制规则，避免真实问题与预期RAW混在一起。
4. 在新增多核样例评审中固定检查：GM输出是否按核分片、同址写是否为原子操作、写后跨核读是否存在明确的核间先行关系。


 <!-- Issue1761已修复，并完成examples批量排查。

  修改内容：

  - group_barrier改为仅由block0初始化GM，随后执行SyncAll()：group_barrier.asc:L43 (examples/01_simd_cpp_api/03_basic_api/05_sync_control/group_barrier/
    group_barrier.asc:43)

  - 新增排查清单：issue-1761-example-multicore-access-audit.md

  排查覆盖709个源码文件，发现另一处建议修改的问题：scalar_atomic_operations在多核原子写后缺少核间同步便读取同一GM，且读回结果未使用。清单中已提供可跳转代码位置和修改建
  议。

  2201实机验证结果：

  - 普通编译、NPU运行通过，6个Waiter均输出24
  - sanitizer插桩编译通过
  - racecheck结果：原有28条WAW已全部消失，WAW=0
  - 剩余36条RAW均来自GroupBarrier软件同步路径，已在清单中分类说明
  - git diff --check通过

  现有issue-1761-analysis.md中原先存在的未提交修改已保留。本轮修改尚未提交或推送。 -->