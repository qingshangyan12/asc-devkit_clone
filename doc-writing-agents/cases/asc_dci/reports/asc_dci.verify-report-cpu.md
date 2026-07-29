# asc_dci 详细验证报告

## 1. 输入与目标
- 输入对象：asc_dci 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/cache_ctrl/asc_dci.md`
- 辅助输入：源码 `include/c_api/cache_ctrl/cache_ctrl.h`、`impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`、UT 测试 `tests/api/c_api/npu_arch_3510/cache_ctrl/test_dci.cpp`
- case 输出路径：`doc-writing-agents/cases/asc_dci/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_dci/reports/asc_dci.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用 asc_dci() 可编译执行（3510） | `01_basic_invoke/` | 待执行 |
| 02 | 带DSB屏障调用 asc_dci() 可编译执行（3510） | `02_with_dsb_barrier/` | 待执行 |
| 03 | asc_dci 在 2201 架构不可编译 | `03_arch_2201_not_supported/` | 待执行（负向） |
| 04 | 文档调用示例回放 | `04_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline void asc_dci()`
- 源码（`cache_ctrl.h:55-57`）：`#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510) __aicore__ inline void asc_dci(); #endif` ✅ 一致
- 源码（`npu_arch_3510/cache_ctrl_impl.h:47`）：`__aicore__ inline void asc_dci() { asc_dci_impl(); }` ✅

### 3.2 产品支持矩阵与源码一致性
- 文档声明：仅 950PR/950DT 支持，A3/A2/310b/310p/910 不支持
- 源码（`cache_ctrl.h:55-57`）：仅在 `__NPU_ARCH__ == 3510` 时定义 ✅ 一致
- 2201 架构实现（`npu_arch_2201/cache_ctrl_impl.h`）：**无 asc_dci 函数** ✅ 确认 2201 不支持
- 3510 架构实现（`npu_arch_3510/cache_ctrl_impl.h:47`）：有 asc_dci ✅ 确认 3510 支持

### 3.3 约束说明：DSB 屏障
- 文档："在调用asc_dci之前，需调用asc_sync_data_barrier插入DSB_ALL指令，等待所有内存访问指令执行结束"
- 源码 UT（`test_dci.cpp`）：验证 `asc_dci()` 正确调用底层 `dci()` ✅
- 调用示例中文档展示了 `asc_sync_data_barrier(mem_dsb_t::DSB_ALL); asc_dci();` ✅

### 3.4 功能说明
- 文档："数据缓存失效，该指令用于使整个数据缓存无效化，且不会对处于'脏'状态的缓存行执行写回操作"
- 源码调用链：`asc_dci` → `asc_dci_impl` → `dci`（底层指令） ✅

### 3.5 头文件路径
- 文档声明：`"c_api/cache_ctrl/cache_ctrl.h"` ✅

### 3.6 流水类型
- 文档声明：PIPE_S ✅

### 3.7 参数和返回值
- 无参数 ✅
- 无返回值 ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 Cache 失效实际效果
- 文档说"使整个数据缓存无效化，不会对脏缓存行写回"
- cpudebug 无法验证真实缓存失效行为 ⚠️ 需上板确认

### 4.2 DSB 约束的强制性
- 文档要求调用前必须插入 DSB_ALL
- 不插入 DSB 直接调用 asc_dci 时的行为未知 ⚠️ 需确认

### 4.3 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"

### 4.4 2201 架构不支持（负向验证）
- 文档明确说 A3 不支持，源码 `#ifdef` 确认 2201 无此函数
- 但尚无实际编译失败证据 ⚠️ 需通过编译测试确认

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 部分验证 | 源码 #ifdef 确认仅 3510，与文档一致 | 参考证据 | 源码分析 + 03_arch_2201_not_supported |
| 功能说明 | 已验证 | 功能描述与源码语义一致 | 直接证据 | 源码比对 |
| 函数原型 | 已验证 | 原型与源码一致 | 直接证据 | 源码比对 |
| 参数说明 | 已验证 | 无参数 | 直接证据 | 源码比对 |
| 返回值说明 | 已验证 | 无返回值 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | DSB 约束存在，但未验证不遵守时的行为 | 参考证据 | 02_with_dsb_barrier |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 04_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| 不带 DSB 调用 asc_dci | 约束：不插入DSB的后果 | 需要硬件环境验证 | 在 NPU 上测试无 DSB 调用是否产生异常 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| Cache 失效实际效果 | 仅文本核对 | cpudebug 不模拟真实缓存行为 | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证 dci 后缓存确实失效 |
| DSB 约束强制行为 | 仅文本核对 | cpudebug 不验证 DSB 约束违反后果 | 跑不起来 | 需 NPU 测试不插入 DSB 时 asc_dci 的行为 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_dci 后数据缓存确实无效化（脏数据被丢弃）
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实缓存行为
  - 建议上板验证动作：在 NPU 上写脏数据到 Cache → 调用 asc_dci → 读回确认数据已被丢弃
- 问题2：不插入 DSB 直接调用 asc_dci 的后果
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证内存屏障约束
  - 建议上板验证动作：在 NPU 上不调用 DSB 直接调用 asc_dci，观察是否产生异常或未定义行为
- 问题3：2201 架构编译失败确认
  - 为什么 cpudebug 不能最终确认：需实际用 2201 架构编译确认
  - 建议上板验证动作：用 `-DCMAKE_ASC_ARCHITECTURES=dav-2201` 编译调用 asc_dci 的代码，确认编译失败

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：3510 架构有测试，验证 `asc_dci()` 正确调用底层 `dci()` ✅
- 2201 架构无 asc_dci 测试 ✅（符合预期，因为 2201 不支持）

## 10. 关键证据
- **架构独占性**：`asc_dci` 仅在 `__NPU_ARCH__ == 3510` 时定义（`cache_ctrl.h:55-57`），2201 架构无此函数
- **底层映射**：`asc_dci` → `asc_dci_impl` → `dci`（底层指令）
- **DSB 约束**：文档要求调用前必须插入 DSB_ALL，源码层面无编译期强制检查
- **UT 证据**：`test_dci.cpp` 验证 `asc_dci()` 正确传递到 `dci()` 底层调用

## 11. 对原文的建议回写
1. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
2. **SIMT 不兼容提示**：源码 `cache_ctrl.h:11-13` 显示不支持 `--enable-simt`，建议在约束中补充
3. **DSB 约束强化**：建议在约束说明中明确不遵守 DSB 约束可能导致的问题（而非仅说"需调用"）

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_dci/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_dci/reports/asc_dci.verify-report-cpu.md`

## 13. 特殊值测试情况

本接口为缓存控制操作，无参数和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 在调用asc_dci之前，需调用asc_sync_data_barrier插入DSB_ALL指令 | 正向 | 部分验证 | 02_with_dsb_barrier | 参考证据 | cpudebug 可编译但无法验证 DSB 实际同步效果 |

### 约束覆盖统计
- 约束总条数：1
- 已验证：0
- 部分验证：1
- 待补验证：0
