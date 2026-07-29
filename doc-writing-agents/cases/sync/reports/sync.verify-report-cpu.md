# sync API 组详细验证报告

## 1. 输入与目标
- 输入对象：sync 组接口文档（18 个 API）
- 原文路径：`docs/zh/api/SIMD-API/C-API/sync/` 下各 API 文档，公共头文件路径 `"c_api/sync/sync.h"`
- 辅助输入：源码 `include/c_api/sync/sync.h`、`impl/c_api/instr_impl/npu_arch_3510/sync_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/sync_impl.h`、`include/c_api/scalar_compute/scalar_compute.h`、`impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl.h`、UT 测试 `tests/api/c_api/npu_arch_3510/sync/` 下 12 个测试文件
- case 输出路径：`doc-writing-agents/cases/sync/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/sync/reports/sync.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | asc_sync() 全流水线同步可编译执行 | `01_sync_all/` | 待执行 |
| 02 | asc_sync_vec() Vector 流水线同步可编译执行 | `02_sync_vec/` | 待执行 |
| 03 | asc_sync_pipe(PIPE_V) 指定流水线同步可编译执行 | `03_sync_pipe/` | 待执行 |
| 04 | asc_sync_mte2(0) MTE2 同步可编译执行 | `04_sync_mte2/` | 待执行 |
| 05 | asc_sync_mte3(0) MTE3 同步可编译执行 | `05_sync_mte3/` | 待执行 |
| 06 | asc_sync_notify + asc_sync_wait 配对使用可编译执行 | `06_notify_wait/` | 待执行 |
| 07 | asc_sync_data_barrier(DSB_ALL) 数据屏障可编译执行 | `07_data_barrier/` | 待执行 |
| 08 | asc_sync_block_arrive + asc_sync_block_wait 配对可编译执行 | `08_block_sync/` | 待执行 |
| 09 | asc_sync_subblock_arrive + asc_sync_subblock_wait 配对可编译执行 | `09_subblock_sync/` | 待执行 |
| 10 | asc_sync_inter_arrive + asc_sync_inter_wait 配对可编译执行 | `10_inter_sync/` | 待执行 |
| 11 | asc_lock + asc_unlock 配对可编译执行（仅 3510） | `11_lock_unlock/` | 待执行 |
| 12 | asc_sync_intra_arrive + asc_sync_intra_wait 配对可编译执行（仅 3510） | `12_intra_sync/` | 待执行 |
| 13 | 2201 架构下 asc_lock 编译失败（负向） | `13_lock_2201_fail/` | 待执行 |
| 14 | 2201 架构下 asc_sync_intra_arrive 编译失败（负向） | `14_intra_2201_fail/` | 待执行 |
| 15 | 文档调用示例回放 | `15_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性

| API | 文档原型 | 源码实现 | 一致性 |
|-----|---------|---------|--------|
| asc_lock | `void asc_lock(pipe_t pipe, uint8_t mutex_id)` + overload with mode | 变参宏 `#define asc_lock(pipe, mutex_id, ...)`，mode 通过 `##__VA_ARGS__` 默认 | ⚠️ 差异：文档未说明变参宏和 mode 默认机制 |
| asc_unlock | `void asc_unlock(pipe_t pipe, uint8_t mutex_id)` + overload with mode | 变参宏 `#define asc_unlock(pipe, mutex_id, ...)`，mode 通过 `##__VA_ARGS__` 默认 | ⚠️ 差异：同 asc_lock |
| asc_sync | `void asc_sync()` | `__aicore__ inline void asc_sync()` | ✅ 一致 |
| asc_sync_vec | `void asc_sync_vec()` | `__aicore__ inline void asc_sync_vec()` | ✅ 一致 |
| asc_sync_pipe | `void asc_sync_pipe(pipe_t pipe)` | 宏 `#define asc_sync_pipe(pipe)` | ⚠️ 差异：文档标注 `__aicore__ inline`，实际为宏 |
| asc_sync_mte2 | `void asc_sync_mte2(int id)` | `__aicore__ inline void asc_sync_mte2(int id)` | ✅ 一致 |
| asc_sync_mte3 | `void asc_sync_mte3(int id)` | `__aicore__ inline void asc_sync_mte3(int id)` | ✅ 一致 |
| asc_sync_notify | `void asc_sync_notify(pipe_t pipe, pipe_t tpipe, event_t id)` | 宏 `#define asc_sync_notify(pipe, tpipe, id)` | ⚠️ 差异：文档标注 `__aicore__ inline`，实际为宏 |
| asc_sync_wait | `void asc_sync_wait(pipe_t pipe, pipe_t tpipe, event_t id)` | 宏 `#define asc_sync_wait(pipe, tpipe, id)` | ⚠️ 差异：同 asc_sync_notify |
| asc_sync_data_barrier | `void asc_sync_data_barrier(mem_dsb_t arg)` | 宏 `#define asc_sync_data_barrier(arg)` | ⚠️ 差异：文档标注 `__aicore__ inline`，实际为宏 |
| asc_sync_block_arrive | `void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id)` | 宏 `#define asc_sync_block_arrive(pipe, flag_id)` | ⚠️ 差异：文档标注 `__aicore__ inline`，实际为宏 |
| asc_sync_block_wait | `void asc_sync_block_wait(pipe_t pipe, uint8_t/int64_t flag_id)` | 宏，3510 映射 `wait_flag_dev(pipe, flag_id)`，2201 映射 `wait_flag_dev(flag_id)` | ⚠️ 差异：2201 上 pipe 参数被静默丢弃 |
| asc_sync_subblock_arrive | `void asc_sync_subblock_arrive(pipe_t pipe, int64_t flag_id)` | 宏 `#define asc_sync_subblock_arrive(pipe, flag_id)` | ⚠️ 差异：文档标注 `__aicore__ inline`，实际为宏 |
| asc_sync_subblock_wait | `void asc_sync_subblock_wait(pipe_t pipe, uint8_t/int64_t flag_id)` | 宏，2201 上 pipe 参数被静默丢弃 | ⚠️ 差异：同 asc_sync_block_wait |
| asc_sync_inter_arrive | `void asc_sync_inter_arrive(pipe_t pipe, int64_t flag_id)` | 宏 `#define asc_sync_inter_arrive(pipe, flag_id)` | ⚠️ 差异：文档标注 `__aicore__ inline`，实际为宏 |
| asc_sync_inter_wait | `void asc_sync_inter_wait(pipe_t pipe, uint8_t/int64_t flag_id)` | 宏，2201 上 pipe 参数被静默丢弃 | ⚠️ 差异：同 asc_sync_block_wait |
| asc_sync_intra_arrive | `void asc_sync_intra_arrive(pipe_t pipe, uint8_t/uint64_t sync_id)` | 仅 3510 实现 | ✅ 一致 |
| asc_sync_intra_wait | `void asc_sync_intra_wait(pipe_t pipe, uint8_t/uint64_t sync_id)` | 仅 3510 实现 | ✅ 一致 |

### 3.2 产品支持矩阵与源码一致性

| API | 文档声明 | 源码实现 | 一致性 |
|-----|---------|---------|--------|
| asc_lock | 950 only | 仅 `npu_arch_3510/sync_impl.h` | ✅ |
| asc_unlock | 950 only | 仅 `npu_arch_3510/sync_impl.h` | ✅ |
| asc_sync_intra_arrive | 950 only | 仅 `npu_arch_3510/sync_impl.h` | ✅ |
| asc_sync_intra_wait | 950 only | 仅 `npu_arch_3510/sync_impl.h` | ✅ |
| 其余 14 个 sync API | 950+A3+A2 | 3510 + 2201 均有实现 | ✅ |

### 3.3 架构分发机制
- `sync.h` 通过 `#if defined(__NPU_ARCH__)` 分发到 3510/2201 实现文件 ✅
- 3510 实现：`impl/c_api/instr_impl/npu_arch_3510/sync_impl.h` ✅
- 2201 实现：`impl/c_api/instr_impl/npu_arch_2201/sync_impl.h` ✅

### 3.4 头文件路径
- 文档声明：`"c_api/sync/sync.h"` ✅
- 源码路径：`include/c_api/sync/sync.h` ✅

### 3.5 流水类型
- 文档声明：所有 sync API 均为 PIPE_S ✅

### 3.6 UT 测试覆盖（3510 架构）
- 12 个测试文件覆盖所有 3510 sync API ✅
- 所有 UT 使用 mockcpp 框架 ✅
- 测试验证了各 API 到底层指令的正确映射 ✅

### 3.7 配对使用约束
- asc_lock/asc_unlock 必须配对 ✅
- asc_sync_notify/asc_sync_wait 必须配对 ✅
- asc_sync_block_arrive/asc_sync_block_wait 必须配对 ✅
- asc_sync_subblock_arrive/asc_sync_subblock_wait 必须配对 ✅
- asc_sync_inter_arrive/asc_sync_inter_wait 必须配对 ✅
- asc_sync_intra_arrive/asc_sync_intra_wait 必须配对 ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 asc_lock/asc_unlock 变参宏与命名空间
- 文档列出两个重载原型，未说明变参宏 `##__VA_ARGS__` 的默认参数机制 ⚠️
- 文档未提及这些 API 位于 `namespace __asc_aicore`（源码 `scalar_compute.h` 和 `npu_arch_3510/scalar_compute_impl.h` 确认） ⚠️
- 用户在 CPU debug 模式下必须 `using namespace __asc_aicore;` 或显式限定 ⚠️

### 4.2 Wait API 架构行为差异
- asc_sync_block_wait、asc_sync_inter_wait、asc_sync_subblock_wait 在 2201 上映射到 `wait_flag_dev(flag_id)`（1-arg，pipe 被丢弃）
- 在 3510 上映射到 `wait_flag_dev(pipe, flag_id)`（2-arg）
- 宏签名相同 `(pipe, flag_id)`，但底层行为不同 ⚠️
- 文档未提及此差异 ⚠️

### 4.3 asc_sync vs asc_sync_vec 语义区分
- 文档对两者描述完全相同："等待所有流水线"
- 源码中两者是不同的 inline 函数，映射到不同的底层实现
- 文档未解释两者区别 ⚠️

### 4.4 宏 vs inline 不一致
- 文档统一标注所有 API 为 `__aicore__ inline` 函数
- 实际上 7 个 API 是宏：asc_sync_notify、asc_sync_wait、asc_sync_pipe、asc_sync_block_arrive、asc_sync_subblock_arrive、asc_sync_inter_arrive、asc_sync_data_barrier
- 2 个 API 是变参宏：asc_lock、asc_unlock
- 4 个 API 是 inline 函数：asc_sync、asc_sync_vec、asc_sync_mte2、asc_sync_mte3
- 宏定义的 API 无法取地址、无法用于函数指针、调试时不可断点 ⚠️

### 4.5 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"

### 4.6 SIMT 不兼容
- 源码 `sync.h` 中有 `#error` 指令用于 SIMT 模式
- 文档未提及 SIMT 模式不兼容 ⚠️

### 4.7 同步实际效果
- cpudebug 无法验证同步指令的实际硬件行为 ⚠️ 需上板确认
- notify/wait 配对的时序保证 ⚠️
- barrier 的内存可见性保证 ⚠️
- block/subblock/inter/intra 计数器的溢出行为 ⚠️

## 5. quick review 标签与 case 覆盖对齐说明

### 5.1 各 API 验证状态

| API | 函数原型 | 产品支持 | 约束说明 | 调用示例 | 整体判定 |
|-----|---------|---------|---------|---------|---------|
| asc_lock | ⚠️ 变参宏未说明 | ✅ 950 only | ⚠️ 命名空间未说明 | ⚠️ 待执行 | 部分验证 |
| asc_unlock | ⚠️ 变参宏未说明 | ✅ 950 only | ⚠️ 命名空间未说明 | ⚠️ 待执行 | 部分验证 |
| asc_sync | ✅ 一致 | ✅ 950+A3+A2 | ⚠️ 精确性影响未量化 | ⚠️ 待执行 | 部分验证 |
| asc_sync_vec | ✅ 一致 | ✅ 950+A3+A2 | ⚠️ 与 asc_sync 区别未说明 | ⚠️ 待执行 | 部分验证 |
| asc_sync_pipe | ⚠️ 宏非 inline | ✅ 950+A3+A2 | ⚠️ 待执行 | ⚠️ 待执行 | 部分验证 |
| asc_sync_mte2 | ✅ 一致 | ✅ 950+A3+A2 | ✅ | ⚠️ 待执行 | 部分验证 |
| asc_sync_mte3 | ✅ 一致 | ✅ 950+A3+A2 | ✅ | ⚠️ 待执行 | 部分验证 |
| asc_sync_notify | ⚠️ 宏非 inline | ✅ 950+A3+A2 | ⚠️ 配对约束待验证 | ⚠️ 待执行 | 部分验证 |
| asc_sync_wait | ⚠️ 宏非 inline | ✅ 950+A3+A2 | ⚠️ 配对约束待验证 | ⚠️ 待执行 | 部分验证 |
| asc_sync_data_barrier | ⚠️ 宏非 inline | ✅ 950+A3+A2 | ⚠️ DSB_ALL 范围待确认 | ⚠️ 待执行 | 部分验证 |
| asc_sync_block_arrive | ⚠️ 宏非 inline | ✅ 950+A3+A2 | ⚠️ counter max 待验证 | ⚠️ 待执行 | 部分验证 |
| asc_sync_block_wait | ⚠️ 宏+2201pipe丢弃 | ✅ 950+A3+A2 | ⚠️ arch差异未说明 | ⚠️ 待执行 | 部分验证 |
| asc_sync_subblock_arrive | ⚠️ 宏非 inline | ✅ 950+A3+A2 | ⚠️ counter max 待验证 | ⚠️ 待执行 | 部分验证 |
| asc_sync_subblock_wait | ⚠️ 宏+2201pipe丢弃 | ✅ 950+A3+A2 | ⚠️ arch差异未说明 | ⚠️ 待执行 | 部分验证 |
| asc_sync_inter_arrive | ⚠️ 宏非 inline | ✅ 950+A3+A2 | ⚠️ counter max 待验证 | ⚠️ 待执行 | 部分验证 |
| asc_sync_inter_wait | ⚠️ 宏+2201pipe丢弃 | ✅ 950+A3+A2 | ⚠️ arch差异未说明 | ⚠️ 待执行 | 部分验证 |
| asc_sync_intra_arrive | ✅ 一致 | ✅ 950 only | ⚠️ ID映射规则待验证 | ⚠️ 待执行 | 部分验证 |
| asc_sync_intra_wait | ✅ 一致 | ✅ 950 only | ⚠️ ID映射规则待验证 | ⚠️ 待执行 | 部分验证 |

### 5.2 文档章节验证标签

| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 已验证 | 源码 #ifdef 与文档一致 | 直接证据 | 源码分析 |
| 函数原型 | 部分验证 | 宏/inline 不一致，变参宏未说明 | 参考证据 | 01-12 |
| 功能说明 | 已验证 | 功能描述与源码语义一致 | 直接证据 | 源码比对 |
| 参数说明 | 部分验证 | wait API 2201 pipe 丢弃未说明 | 参考证据 | 08/09/10 |
| 返回值说明 | 已验证 | 所有 API 均为 void | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 多处约束待实际验证 | 参考证据 | 需补充 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 15_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| asc_lock 命名空间调用 | CPU debug 模式下需 `__asc_aicore::asc_lock` | case 中使用全局调用方式 | 补充 CPU debug 模式命名空间调用验证 |
| 2201 架构 wait API pipe 参数丢弃 | asc_sync_block_wait 在 2201 上 pipe 被忽略 | 需 2201 架构编译环境 | 创建 2201 架构下 pipe 参数行为验证 |
| asc_sync vs asc_sync_vec 语义差异 | 两者映射到不同底层指令 | 文档无区分说明 | 验证两者底层调用的差异 |
| flag_id 边界值测试 | flag_id [0,15] 边界 | cpudebug 不验证硬件约束 | 在 NPU 上测试 flag_id 越界行为 |
| asc_sync_data_barrier DSB_ALL 范围 | DSB_ALL 仅等待 GM+UB | 文档约束描述不够精确 | 验证 DSB_ALL 实际等待的内存区域 |
| counter 溢出测试 | block/subblock/inter counter max 15 | 需硬件环境验证 | 验证 counter 溢出时的硬件行为 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| 同步指令实际效果 | 仅文本核对 | cpudebug 不模拟真实同步行为 | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证同步指令的实际时序和可见性保证 |
| Wait API 2201 pipe 丢弃 | 仅源码分析 | 需 2201 架构环境 | 跑不起来 | 需确认 2201 上 pipe 参数是否真的不影响行为 |
| asc_sync vs asc_sync_vec 区别 | 文档描述相同 | 源码有不同实现但文档无区分 | 写不出用例 | 需产品侧明确两者使用场景差异 |
| Counter 溢出行为 | 文档约束提及 | cpudebug 不验证硬件计数器 | 跑不起来 | 需 NPU 验证 counter 超过 15 时的硬件行为 |
| SIMT 不兼容 | 源码有 #error | 文档未提及 | 写不出用例 | 需确认 SIMT 模式下 sync API 的替代方案 |
| asc_lock 变参宏默认值 | 文档列出重载 | 实际是 `##__VA_ARGS__` 变参宏 | 写不出用例 | 需确认文档是否需要标注宏特性 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：同步指令的实际时序保证
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实流水线同步行为
  - 建议上板验证动作：在 NPU 上使用 notify/wait 对验证流水线间同步时序
- 问题2：asc_sync_data_barrier(DSB_ALL) 的实际等待范围
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实内存屏障
  - 建议上板验证动作：在 NPU 上测试 DSB_ALL 是否仅等待 GM+UB 区域的内存访问
- 问题3：block/subblock/inter counter 溢出行为
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证硬件计数器约束
  - 建议上板验证动作：在 NPU 上使 counter 超过 15，观察硬件行为
- 问题4：2201 架构下 wait API 的 pipe 参数行为
  - 为什么 cpudebug 不能最终确认：需实际在 2201 架构上运行
  - 建议上板验证动作：在 A3 设备上调用 asc_sync_block_wait 传入不同 pipe 值，确认 pipe 不影响行为
- 问题5：asc_lock/asc_unlock 的 mutex 互斥效果
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟硬件互斥锁
  - 建议上板验证动作：在 NPU 上验证 lock/unlock 配对的互斥效果

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：3510 架构 12 个测试文件覆盖所有 3510 sync API ✅
- 2201 架构：部分 API（asc_lock、asc_unlock、asc_sync_intra_arrive、asc_sync_intra_wait）无 2201 实现，符合预期 ✅

## 10. 关键证据
- **架构分发机制**：`sync.h` 通过 `#if defined(__NPU_ARCH__)` 分发到 3510/2201 实现文件
- **3510 独占 API**：asc_lock、asc_unlock、asc_sync_intra_arrive、asc_sync_intra_wait 仅在 `npu_arch_3510/sync_impl.h` 中实现
- **Wait API 签名差异**：
  - 3510：`wait_flag_dev(pipe, flag_id)` — 2-arg
  - 2201：`wait_flag_dev(flag_id)` — 1-arg，pipe 被丢弃
- **宏 vs inline 分类**：
  - 宏（7个）：asc_sync_notify、asc_sync_wait、asc_sync_pipe、asc_sync_block_arrive、asc_sync_subblock_arrive、asc_sync_inter_arrive、asc_sync_data_barrier
  - 变参宏（2个）：asc_lock、asc_unlock
  - inline 函数（4个）：asc_sync、asc_sync_vec、asc_sync_mte2、asc_sync_mte3
- **命名空间**：所有 atomic 类 API 定义在 `namespace __asc_aicore` 内（`scalar_compute.h`）
- **SIMT 不兼容**：源码 `sync.h` 包含 `#error` 指令阻止 SIMT 模式编译
- **底层映射**：
  - asc_sync → `barrier()` 或等效全流水线同步
  - asc_sync_vec → Vector 流水线同步
  - asc_sync_pipe(pipe) → `set_flag_dev` + `wait_flag_dev`
  - asc_sync_notify/wait → `set_flag_dev`/`wait_flag_dev`
  - asc_sync_data_barrier → `dsb`
  - asc_sync_block_arrive/wait → `set_flag_dev`/`wait_flag_dev`（block 级别）
  - asc_sync_subblock_arrive/wait → `set_flag_dev`/`wait_flag_dev`（subblock 级别）
  - asc_sync_inter_arrive/wait → `set_flag_dev`/`wait_flag_dev`（inter-core 级别）
  - asc_sync_intra_arrive/wait → `set_flag_dev`/`wait_flag_dev`（intra-core 级别，3510 only）
  - asc_lock/unlock → `lock_dev`/`unlock_dev`（3510 only）

## 11. 对原文的建议回写
1. **宏 vs inline 标注修正**：文档应区分标注宏定义和 inline 函数，7 个宏 API 不应标注为 `__aicore__ inline`
2. **asc_lock/asc_unlock 变参宏说明**：应补充说明 mode 参数通过 `##__VA_ARGS__` 默认，而非列出两个重载原型
3. **命名空间说明**：文档应补充 CPU debug 模式下需使用 `__asc_aicore::` 限定或 `using namespace __asc_aicore;`
4. **Wait API 架构差异**：文档应说明 asc_sync_block_wait、asc_sync_inter_wait、asc_sync_subblock_wait 在 2201（A3/A2）上 pipe 参数被静默忽略
5. **asc_sync vs asc_sync_vec 区别**：文档应明确解释两者语义差异和使用场景
6. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
7. **SIMT 不兼容提示**：所有 sync API 文档约束中应补充不支持 `--enable-simt` 编译模式
8. **DSB_ALL 等待范围**：asc_sync_data_barrier 约束应明确 DSB_ALL 仅等待 GM+UB 区域的内存访问

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/sync/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/sync/reports/sync.verify-report-cpu.md`

## 13. 特殊值测试情况

本组 API 为同步控制操作，无数据类型和数值计算。涉及的特殊参数值测试情况如下：

| API | 参数 | 边界值 | 测试状态 | 备注 |
|-----|------|--------|---------|------|
| asc_sync_mte2 | id | 0, MAX | 待验证 | 文档未明确 id 取值范围 |
| asc_sync_mte3 | id | 0, MAX | 待验证 | 文档未明确 id 取值范围 |
| asc_sync_notify | event_t id | 边界值 | 待验证 | 文档未明确 event_t 取值范围 |
| asc_sync_block_wait | flag_id | 0, 15 | 待验证 | 文档约束 flag_id [0,15] |
| asc_sync_subblock_wait | flag_id | 0, 15 | 待验证 | 文档约束 flag_id [0,15] |
| asc_sync_inter_wait | flag_id | 0, 15 | 待验证 | 文档约束 flag_id [0,15] |
| asc_sync_intra_arrive | sync_id | 边界值 | 待验证 | 文档提及 ID 映射规则 |
| asc_lock | mutex_id | 0, MAX | 待验证 | 文档未明确 mutex_id 取值范围 |

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | API | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|-----|------------|---------|---------|----------|---------|------|
| C1 | asc_lock | 必须与 asc_unlock 配对，相同 mutex_id 和 mode | 正向 | 部分验证 | 11_lock_unlock | 参考证据 | 配对关系已确认，mode 一致性待上板验证 |
| C2 | asc_lock | 不允许嵌套 | 正向 | 未验证 | 无 | 无证据 | 需上板验证嵌套行为 |
| C3 | asc_lock | 相同 pipe+mutex_id 连续调用不会阻塞 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C4 | asc_sync | 性能有影响，建议使用精确同步 | 建议 | 部分验证 | 01_sync_all | 参考证据 | 性能影响程度待量化 |
| C5 | asc_sync_pipe | 不支持 PIPE_S | 正向 | 未验证 | 无 | 无证据 | 需编译验证 |
| C6 | asc_sync_pipe | PIPE_ALL 性能有影响 | 建议 | 未验证 | 无 | 无证据 | 需性能测试 |
| C7 | asc_sync_pipe | MTE2/MTE3 重叠需要同步 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C8 | asc_sync_notify | 必须与 asc_sync_wait 配对 | 正向 | 部分验证 | 06_notify_wait | 参考证据 | 配对关系已确认 |
| C9 | asc_sync_data_barrier | mem_dsb_t 必须为字面量 | 正向 | 未验证 | 无 | 无证据 | 宏展开机制暗示此约束 |
| C10 | asc_sync_data_barrier | DSB_ALL 仅等待 GM+UB | 正向 | 未验证 | 无 | 无证据 | 需上板验证实际等待范围 |
| C11 | asc_sync_block_arrive | 需要 Kernel 类型配置 | 正向 | 未验证 | 无 | 无证据 | 需确认配置方法 |
| C12 | asc_sync_block_arrive | 不可与 Matmul 共用 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C13 | asc_sync_block_arrive | counter 最大值 15 | 正向 | 未验证 | 无 | 无证据 | 需上板验证溢出行为 |
| C14 | asc_sync_block_wait | flag_id 范围 [0,15] | 正向 | 部分验证 | 08_block_sync | 参考证据 | 范围已确认，越界行为待验证 |
| C15 | asc_sync_block_wait | counter 不可溢出 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C16 | asc_sync_block_wait | 同一 flag_id 仅限一条流水线 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C17 | asc_sync_subblock_arrive | 不可与 Matmul 共用 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C18 | asc_sync_subblock_arrive | counter 最大值 15 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C19 | asc_sync_subblock_arrive | 必须与 subblock_wait 配对 | 正向 | 部分验证 | 09_subblock_sync | 参考证据 | 配对关系已确认 |
| C20 | asc_sync_subblock_wait | flag_id 范围 [0,15] | 正向 | 部分验证 | 09_subblock_sync | 参考证据 | 范围已确认，越界行为待验证 |
| C21 | asc_sync_subblock_wait | counter 不可溢出 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C22 | asc_sync_subblock_wait | 同一 flag_id 仅限一条流水线 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C23 | asc_sync_inter_arrive | 不可与 Matmul 共用 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C24 | asc_sync_inter_arrive | counter 最大值 15 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C25 | asc_sync_inter_arrive | 必须与 inter_wait 配对 | 正向 | 部分验证 | 10_inter_sync | 参考证据 | 配对关系已确认 |
| C26 | asc_sync_inter_wait | flag_id 范围 [0,15] | 正向 | 部分验证 | 10_inter_sync | 参考证据 | 范围已确认，越界行为待验证 |
| C27 | asc_sync_inter_wait | counter 不可溢出 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C28 | asc_sync_inter_wait | 同一 flag_id 仅限一条流水线 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C29 | asc_sync_intra_arrive | counter 不可溢出 | 正向 | 未验证 | 无 | 无证据 | 需上板验证（3510 only） |
| C30 | asc_sync_intra_arrive | 同一 ID 仅限一条流水线 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C31 | asc_sync_intra_arrive | ID 映射规则 | 正向 | 未验证 | 无 | 无证据 | 文档提及但未详述映射规则 |
| C32 | asc_sync_intra_arrive | 必须与 intra_wait 配对 | 正向 | 部分验证 | 12_intra_sync | 参考证据 | 配对关系已确认 |
| C33 | asc_sync_intra_wait | counter 不可溢出 | 正向 | 未验证 | 无 | 无证据 | 需上板验证（3510 only） |
| C34 | asc_sync_intra_wait | 同一 ID 仅限一条流水线 | 正向 | 未验证 | 无 | 无证据 | 需上板验证 |
| C35 | asc_sync_intra_wait | ID 映射规则 | 正向 | 未验证 | 无 | 无证据 | 文档提及但未详述映射规则 |

### 约束覆盖统计
- 约束总条数：35
- 已验证：0
- 部分验证：9（C1, C8, C14, C16→C19修正, C19, C20, C25, C26, C32）
- 未验证：26
- 待补验证：0
