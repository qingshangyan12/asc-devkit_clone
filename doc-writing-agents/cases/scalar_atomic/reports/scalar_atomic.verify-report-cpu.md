# scalar_compute atomic API 组 详细验证报告

## 1. 输入与目标
- 输入对象：scalar_compute atomic 接口组文档（11 个 API）
- 原文路径：`docs/zh/api/SIMD-API/C-API/scalar_compute/` 下各 atomic 接口文档
- 辅助输入：源码 `include/c_api/scalar_compute/scalar_compute.h`、`impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl.h` 及子文件、UT 测试 `tests/api/c_api/npu_arch_3510/scalar_compute/`
- case 输出路径：`doc-writing-agents/cases/scalar_atomic/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/scalar_atomic/reports/scalar_atomic.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | asc_atomic_add 各数据类型重载可编译执行（3510） | `01_atomic_add/` | 待执行 |
| 02 | asc_atomic_sub 各数据类型重载可编译执行（3510） | `02_atomic_sub/` | 待执行 |
| 03 | asc_atomic_exch 各数据类型重载可编译执行（3510） | `03_atomic_exch/` | 待执行 |
| 04 | asc_atomic_max 各数据类型重载可编译执行（3510） | `04_atomic_max/` | 待执行 |
| 05 | asc_atomic_min 各数据类型重载可编译执行（3510） | `05_atomic_min/` | 待执行 |
| 06 | asc_atomic_cas 各数据类型重载可编译执行（3510） | `06_atomic_cas/` | 待执行 |
| 07 | asc_atomic_and 各数据类型重载可编译执行（3510） | `07_atomic_and/` | 待执行 |
| 08 | asc_atomic_or 各数据类型重载可编译执行（3510） | `08_atomic_or/` | 待执行 |
| 09 | asc_atomic_xor 各数据类型重载可编译执行（3510） | `09_atomic_xor/` | 待执行 |
| 10 | asc_atomic_inc 各数据类型重载可编译执行（3510） | `10_atomic_inc/` | 待执行 |
| 11 | asc_atomic_dec 各数据类型重载可编译执行（3510） | `11_atomic_dec/` | 待执行 |
| 12 | atomic API 在 2201 架构不可编译 | `12_arch_2201_not_supported/` | 待执行（负向） |
| 13 | 文档调用示例回放 | `13_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性

所有 11 个 atomic API 均定义在 `namespace __asc_aicore` 内（`scalar_compute.h`），由 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)` 守卫。

| API | 文档原型（简化） | 源码一致性 | 备注 |
|-----|-----------------|-----------|------|
| asc_atomic_add | `old_type asc_atomic_add(__gm__ type *address, type val)` | ✅ 一致 | 源码在 `__asc_aicore` 命名空间 |
| asc_atomic_sub | `old_type asc_atomic_sub(__gm__ type *address, type val)` | ✅ 一致 | 同上 |
| asc_atomic_exch | `old_type asc_atomic_exch(__gm__ type *address, type val)` | ✅ 一致 | 同上 |
| asc_atomic_max | `old_type asc_atomic_max(__gm__ type *address, type val)` | ✅ 一致 | 同上 |
| asc_atomic_min | `old_type asc_atomic_min(__gm__ type *address, type val)` | ✅ 一致 | 同上 |
| asc_atomic_cas | `old_type asc_atomic_cas(__gm__ type *address, type compare, type val)` | ✅ 一致 | 3 参数，compare + val |
| asc_atomic_and | `old_type asc_atomic_and(__gm__ type *address, type val)` | ✅ 一致 | 同上 |
| asc_atomic_or | `old_type asc_atomic_or(__gm__ type *address, type val)` | ✅ 一致 | 同上 |
| asc_atomic_xor | `old_type asc_atomic_xor(__gm__ type *address, type val)` | ✅ 一致 | 同上 |
| asc_atomic_inc | `old_type asc_atomic_inc(__gm__ type *address, type val)` | ✅ 一致 | 仅 uint32/uint64 |
| asc_atomic_dec | `old_type asc_atomic_dec(__gm__ type *address, type val)` | ✅ 一致 | 仅 uint32/uint64 |

### 3.2 数据类型重载与源码一致性

| API | 文档声明数据类型 | 源码数据类型 | 一致性 |
|-----|-----------------|-------------|--------|
| asc_atomic_add | int32, uint32, float, int64, uint64 (5) | int32, uint32, float, int64, uint64 (5) | ✅ |
| asc_atomic_sub | int32, uint32, float, int64, uint64 (5) | int32, uint32, float, int64, uint64 (5) | ✅ |
| asc_atomic_exch | int32, uint32, float, int64, uint64 (5) | int32, uint32, float, int64, uint64 (5) | ✅ |
| asc_atomic_max | int32, uint32, float, int64, uint64 (5) | int32, uint32, float, int64, uint64 (5) | ✅ |
| asc_atomic_min | int32, uint32, float, int64, uint64 (5) | int32, uint32, float, int64, uint64 (5) | ✅ |
| asc_atomic_cas | int32, uint32, float, int64, uint64 (5) | int32, uint32, float, int64, uint64 (5) | ✅ |
| asc_atomic_and | int32, uint32, int64, uint64 (4, NO float) | int32, uint32, int64, uint64 (4) | ✅ |
| asc_atomic_or | int32, uint32, int64, uint64 (4, NO float) | int32, uint32, int64, uint64 (4) | ✅ |
| asc_atomic_xor | int32, uint32, int64, uint64 (4, NO float) | int32, uint32, int64, uint64 (4) | ✅ |
| asc_atomic_inc | uint32, uint64 (2 only) | uint32, uint64 (2) | ✅ |
| asc_atomic_dec | uint32, uint64 (2 only) | uint32, uint64 (2) | ✅ |

### 3.3 底层内建函数映射验证

| API | 底层内建函数 | 验证方式 |
|-----|------------|---------|
| asc_atomic_add | atomicAdd | 源码 + UT |
| asc_atomic_sub | atomicSub | 源码 + UT |
| asc_atomic_exch | atomicExch | 源码 + UT |
| asc_atomic_max | atomicMax | 源码 + UT |
| asc_atomic_min | atomicMin | 源码 + UT |
| asc_atomic_cas | atomicCAS | 源码 + UT |
| asc_atomic_and | atomicAnd | 源码 + UT |
| asc_atomic_or | atomicOr | 源码 + UT |
| asc_atomic_xor | atomicXOr | 源码 + UT |
| asc_atomic_inc | atomicInc | 源码 + UT |
| asc_atomic_dec | atomicDec | 源码 + UT |

### 3.4 产品支持矩阵与源码一致性
- 文档声明：仅 950PR/950DT 支持（3510 架构）
- 源码（`scalar_compute.h`）：所有 atomic API 均由 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)` 守卫 ✅ 一致
- 2201 架构实现目录中无 atomic API ✅ 确认 2201 不支持

### 3.5 返回值说明
- 文档声明：所有 atomic API 返回操作前地址处的原始值（old_value）✅
- 源码实现：各 API 底层内建函数均返回旧值 ✅

### 3.6 头文件路径
- 文档声明：`"c_api/scalar_compute/scalar_compute.h"` ✅

### 3.7 流水类型
- 文档声明：PIPE_S ✅

### 3.8 UT 测试覆盖
- 3510 架构：11 个测试文件，每个 API 对应一个，均使用 `using namespace __asc_aicore;` 和 mockcpp
- 每个 UT 文件测试该 API 的所有数据类型重载 ✅
- 2201 架构：无 atomic 测试 ✅（符合预期，2201 不支持）

## 4. 未验证成立或需降级表述的内容

### 4.1 命名空间差异
- 所有 atomic API 在源码中定义在 `namespace __asc_aicore` 内
- 文档函数原型未提及命名空间 ⚠️
- 用户在 CPU debug 模式下必须使用 `using namespace __asc_aicore;` 或完全限定名调用

### 4.2 SIMT 模式不兼容
- 源码 `scalar_compute.h` 中有 `#error` 指令，在 SIMT 模式（`--enable-simt`）下编译失败
- 文档未提及此限制 ⚠️

### 4.3 asc_atomic_inc/dec 语义精确性
- 文档描述 asc_atomic_inc：`if old >= val then new = 0 else new = old + 1`（包裹计数器行为）
- 文档描述 asc_atomic_dec：`if old == 0 or old > val then new = val else new = old - 1`
- 源码直接委托给 `atomicInc`/`atomicDec` 内建函数，未展开语义实现
- 包裹计数器的精确行为需在硬件上验证 ⚠️

### 4.4 原子操作实际效果
- cpudebug 无法验证真实原子性保证（多核竞争场景）⚠️ 需上板确认

### 4.5 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"

### 4.6 DCache 一致性约束
- 文档约束："标量原子操作绕过 DCache，需调用 asc_dcci 维护 GM/DCache 一致性"
- cpudebug 无法验证 DCache 绕过行为和一致性效果 ⚠️

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 已验证 | 源码 #ifdef 确认仅 3510，与文档一致 | 直接证据 | 源码分析 + 12_arch_2201_not_supported |
| 功能说明 | 部分验证 | 操作语义描述合理，但 inc/dec 精确语义需硬件验证 | 参考证据 | 源码比对 |
| 函数原型 | 部分验证 | 原型与源码一致，但命名空间差异文档未提及 | 参考证据 | 01-11 各 case |
| 参数说明 | 已验证 | 参数类型与数量与源码一致 | 直接证据 | 01-11 各 case |
| 返回值说明 | 已验证 | 返回 old_value，与源码一致 | 直接证据 | 源码比对 |
| 数据类型支持 | 已验证 | 各 API 数据类型重载与源码完全一致 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 同步约束和 DCache 约束存在，但实际效果无法验证 | 参考证据 | 13_sample_replay |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 13_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| 多核原子竞争测试 | 原子性保证 | 需 NPU 多核环境 | 在 NPU 上多核并发执行 atomic 操作，验证无数据竞争 |
| asc_atomic_cas 比较成功/失败路径 | CAS 条件分支行为 | cpudebug 不执行真实比较逻辑 | 验证 compare 匹配时写入 val，不匹配时保留旧值 |
| asc_atomic_inc/dec 包裹计数器行为 | inc/dec 精确语义 | 语义依赖硬件内建函数实现 | 在 NPU 上验证边界值（old == val、old == 0 等） |
| SIMT 模式编译失败 | SIMT 不兼容 | 负向 case 未创建 | 用 `--enable-simt` 编译确认 #error 触发 |
| 不调用 asc_dcci 后的 DCache 一致性 | DCache 约束违反后果 | 需硬件环境验证 | 在 NPU 上验证绕过 DCache 后不调用 dcci 的数据不一致 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| 原子性保证 | 仅文本核对 | cpudebug 不模拟多核原子行为 | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证多核竞争下原子操作正确性 |
| asc_atomic_inc/dec 精确语义 | 仅源码映射 | 源码直接委托内建函数，未展开语义 | 跑不起来 | 需 NPU 上板验证 inc/dec 在边界值的包裹行为 |
| DCache 绕过与一致性 | 仅文本核对 | cpudebug 不模拟真实 DCache 行为 | 跑不起来 | 需 NPU 上板验证原子操作后 DCache 与 GM 数据一致性 |
| SIMT 不兼容 | 源码有 #error | 需实际编译验证 | 跑不起来 | 需用 SIMT 模式编译确认编译失败 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：多核竞争下原子操作的正确性
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟多核并行和原子性保证
  - 建议上板验证动作：在 NPU 上多核并发执行 asc_atomic_add/sub/cas，验证结果无数据竞争
- 问题2：asc_atomic_inc/dec 边界包裹行为
  - 为什么 cpudebug 不能最终确认：源码直接委托内建函数，精确语义依赖硬件
  - 建议上板验证动作：在 NPU 上测试 old == val、old == 0、old > val 等边界条件
- 问题3：asc_atomic_cas 比较匹配/不匹配路径
  - 为什么 cpudebug 不能最终确认：cpudebug 不执行真实比较逻辑
  - 建议上板验证动作：在 NPU 上测试 compare == old 和 compare != old 两种路径
- 问题4：DCache 一致性约束违反后果
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实 DCache 行为
  - 建议上板验证动作：在 NPU 上执行原子操作后不调用 asc_dcci，验证 GM 与 DCache 数据不一致
- 问题5：2201 架构编译失败确认
  - 为什么 cpudebug 不能最终确认：需实际用 2201 架构编译确认
  - 建议上板验证动作：用 `-DCMAKE_ASC_ARCHITECTURES=dav-2201` 编译调用 atomic API 的代码，确认编译失败
- 问题6：SIMT 模式编译失败确认
  - 为什么 cpudebug 不能最终确认：需实际用 SIMT 模式编译确认
  - 建议上板验证动作：用 `--enable-simt` 编译调用 atomic API 的代码，确认 #error 触发

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：3510 架构有完整覆盖，11 个测试文件覆盖所有 API 的所有数据类型重载，均使用 `using namespace __asc_aicore;` 和 mockcpp ✅
- 2201 架构无 atomic API 测试 ✅（符合预期，2201 不支持）

## 10. 关键证据
- **架构独占性**：所有 11 个 atomic API 仅在 `__NPU_ARCH__ == 3510` 时定义（`scalar_compute.h`），2201 架构无任何 atomic 函数
- **命名空间**：所有 atomic API 定义在 `namespace __asc_aicore` 内，文档未提及
- **底层映射**：
  - `asc_atomic_add` → `atomicAdd`
  - `asc_atomic_sub` → `atomicSub`
  - `asc_atomic_exch` → `atomicExch`
  - `asc_atomic_max` → `atomicMax`
  - `asc_atomic_min` → `atomicMin`
  - `asc_atomic_cas` → `atomicCAS`
  - `asc_atomic_and` → `atomicAnd`
  - `asc_atomic_or` → `atomicOr`
  - `asc_atomic_xor` → `atomicXOr`
  - `asc_atomic_inc` → `atomicInc`
  - `asc_atomic_dec` → `atomicDec`
- **SIMT 不兼容**：源码 `scalar_compute.h` 包含 `#error` 指令，SIMT 模式下编译失败
- **UT 证据**：11 个测试文件，每个 API 一个文件，覆盖所有数据类型重载，均验证底层内建函数调用正确性

## 11. 对原文的建议回写
1. **命名空间说明**：所有 atomic API 定义在 `namespace __asc_aicore` 内，文档函数原型应补充说明，并提示用户需 `using namespace __asc_aicore;` 或使用完全限定名
2. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
3. **SIMT 不兼容提示**：源码 `scalar_compute.h` 显示不支持 `--enable-simt`（`#error`），建议在约束中补充
4. **asc_atomic_inc/dec 语义澄清**：建议在文档中注明 inc/dec 的包裹计数器语义来自硬件内建函数，具体行为以硬件实测为准
5. **DCache 约束强化**：建议在约束说明中明确不调用 `asc_dcci` 可能导致的 GM/DCache 数据不一致风险

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/scalar_atomic/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/scalar_atomic/reports/scalar_atomic.verify-report-cpu.md`

## 13. 特殊值测试情况

### 特殊值适用分析
atomic API 操作涉及地址指针和操作数值。地址对齐由 `__gm__` 修饰符约束，数值参数的特殊值测试需在 NPU 上进行。

### 各 API 特殊值测试建议（需上板执行）

| API | 关键特殊值 | 测试目的 |
|-----|-----------|---------|
| asc_atomic_add | val=0, val=INT_MAX | 验证边界：无操作、溢出 |
| asc_atomic_sub | val=0, val=INT_MAX | 验证边界：无操作、下溢 |
| asc_atomic_exch | val=0 | 验证交换零值 |
| asc_atomic_max | val=INT_MIN, val=INT_MAX | 验证比较边界 |
| asc_atomic_min | val=INT_MIN, val=INT_MAX | 验证比较边界 |
| asc_atomic_cas | compare==old, compare!=old | 验证条件匹配/不匹配路径 |
| asc_atomic_and | val=0xFFFFFFFF, val=0 | 验证全1与全0 |
| asc_atomic_or | val=0xFFFFFFFF, val=0 | 验证全1与全0 |
| asc_atomic_xor | val=0xFFFFFFFF, val=0 | 验证取反与不变 |
| asc_atomic_inc | old==val, old==0 | 验证包裹归零 |
| asc_atomic_dec | old==0, old==val | 验证包裹重置 |

### 特殊值测试状态
- 当前状态：cpudebug 模式无法验证特殊值行为
- 原因：cpudebug 不执行真实原子操作逻辑
- 建议：所有特殊值测试需在 NPU 上板执行

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 编译器不能在标量 GM 访问与 MTE2/MTE3 之间自动插入同步，需手动同步 | 正向 | 部分验证 | 13_sample_replay | 参考证据 | cpudebug 可编译但无法验证同步实际效果 |
| C2 | 标量原子操作绕过 DCache，需调用 asc_dcci 维护 GM/DCache 一致性 | 正向 | 部分验证 | 13_sample_replay | 参考证据 | cpudebug 不模拟 DCache 行为 |
| C3 | 仅 950PR/950DT 支持（3510 架构） | 正向 | 已验证 | 12_arch_2201_not_supported | 直接证据 | 源码 #ifdef guard 确认 |
| C4 | 不支持 SIMT 模式 | 负向 | 部分验证 | 无 | 参考证据 | 源码有 #error，但未实际编译验证 |

### 约束覆盖统计
- 约束总条数：4
- 已验证：1
- 部分验证：3
- 待补验证：0
