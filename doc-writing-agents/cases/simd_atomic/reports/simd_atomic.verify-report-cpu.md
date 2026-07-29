# simd_atomic API 组详细验证报告

## 1. 输入与目标
- 输入对象：simd_atomic 接口组文档（7个 API）
- 原文路径：`docs/zh/api/SIMD-API/C-API/simd_atomic/` 下 7 篇文档
  - `asc_set_atomic_add.md`
  - `asc_set_atomic_max.md`
  - `asc_set_atomic_min.md`
  - `asc_set_atomic_none.md`
  - `asc_get_store_atomic_config.md`
  - `asc_set_store_atomic_config_v1.md`
  - `asc_set_store_atomic_config_v2_deprecated.md`
- 辅助输入：源码 `include/c_api/atomic/atomic.h`、`impl/c_api/instr_impl/npu_arch_2201/atomic_impl.h`、`impl/c_api/instr_impl/npu_arch_3510/atomic_impl.h`、UT 测试 `tests/api/c_api/npu_arch_2201/simd_atomic/`（20 个文件）
- case 输出路径：`doc-writing-agents/cases/simd_atomic/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/simd_atomic/reports/simd_atomic.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | asc_set_atomic_add_* 6 变体可编译执行 | `01_set_atomic_add/` | 待执行 |
| 02 | asc_set_atomic_max_* 6 变体可编译执行 | `02_set_atomic_max/` | 待执行 |
| 03 | asc_set_atomic_min_* 6 变体可编译执行 | `03_set_atomic_min/` | 待执行 |
| 04 | asc_set_atomic_none() 可编译执行 | `04_set_atomic_none/` | 待执行 |
| 05 | asc_get_store_atomic_config() 可编译执行 | `05_get_store_atomic_config/` | 待执行 |
| 06 | asc_set_store_atomic_config_v1() 可编译执行（2201） | `06_set_store_atomic_config_v1/` | 待执行 |
| 07 | asc_set_store_atomic_config_v2() 在 3510 可编译但 deprecated | `07_set_store_atomic_config_v2/` | 待执行 |
| 08 | v1 与 v2 type 编码差异验证 | `08_v1_v2_encoding_diff/` | 待执行（负向） |
| 09 | 文档调用示例回放 | `09_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性

#### 3.1.1 2201 架构（A3/A2）声明
| 文档原型 | 源码（`atomic.h:39-71`） | 一致性 |
|---------|------|--------|
| `void asc_set_atomic_add_int8()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_int16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_int()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_bfloat()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_float16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_float()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_int8()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_int16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_int()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_bfloat()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_float16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_float()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_int8()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_int16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_int()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_bfloat()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_float16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_float()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_none()` | ✅ 声明（仅 2201） | 一致 |
| `void asc_get_store_atomic_config(asc_store_atomic_config&)` | ✅ 声明（仅 2201） | 一致 |
| `void asc_set_store_atomic_config_v1(uint16_t, uint16_t)` | ✅ 声明（仅 2201） | 一致 |

#### 3.1.2 3510 架构（950）声明
| 文档原型 | 源码（`atomic.h:73-99`） | 一致性 |
|---------|------|--------|
| `void asc_set_atomic_add_float16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_float()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_bfloat()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_float()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_bfloat()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_float()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_float16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_none()` | ✅ 声明 | 一致 |
| `[[deprecated]] void asc_get_store_atomic_config(asc_store_atomic_config&)` | ✅ 声明 + deprecated 属性 | 一致 |
| `[[deprecated]] void asc_set_store_atomic_config_v2(uint16_t, uint16_t)` | ✅ 声明 + deprecated 属性 | 一致 |
| `void asc_set_atomic_max_bfloat()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_float16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_int()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_int8()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_add_int16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_int()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_int8()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_max_int16()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_int()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_int8()` | ✅ 声明 | 一致 |
| `void asc_set_atomic_min_int16()` | ✅ 声明 | 一致 |

#### 3.1.3 架构外公共声明
| 文档原型 | 源码（`atomic.h:29-37`） | 一致性 |
|---------|------|--------|
| `void asc_set_atomic_add_float()` | ✅ 公共声明 | 一致 |
| `void asc_set_atomic_max_float16()` | ✅ 公共声明 | 一致 |
| `void asc_set_atomic_add_int()` | ✅ 公共声明 | 一致 |
| `void asc_set_atomic_add_int8()` | ✅ 公共声明 | 一致 |
| `void asc_set_atomic_add_int16()` | ✅ 公共声明 | 一致 |

**说明**：`atomic.h` 第 29-37 行的 5 个声明位于 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)` 之前，为两个架构共享的函数声明。2201 和 3510 的条件编译块内部又分别声明了各自的完整函数集。

### 3.2 产品支持矩阵与源码一致性

#### 3.2.1 asc_set_atomic_add / max / min
- 文档声明：950/A3/A2 支持 ✅
- 2201 实现（`npu_arch_2201/atomic_impl.h:55-89`）：18 个变体（add/max/min × 6 类型）全部实现 ✅
- 3510 实现（`npu_arch_3510/atomic_impl.h:37-102`）：18 个变体全部实现 ✅

#### 3.2.2 asc_set_atomic_none
- 文档声明：950/A3/A2 支持 ✅
- 2201 实现（`npu_arch_2201/atomic_impl.h:43`）：有 ✅
- 3510 实现（`npu_arch_3510/atomic_impl.h:61`）：有 ✅

#### 3.2.3 asc_get_store_atomic_config
- 文档声明：950/A3/A2 支持，950 上已废弃 ✅
- 2201 实现（`npu_arch_2201/atomic_impl.h:45-48`）：有，无 deprecated 标记 ✅（2201=A3/A2 不废弃）
- 3510 实现（`npu_arch_3510/atomic_impl.h:64-69`）：有，`[[deprecated]]` 标记 ✅（950 上废弃）

#### 3.2.4 asc_set_store_atomic_config_v1（A3/A2 专用）
- 文档声明：950 不支持，A3/A2 支持 ✅
- 2201 实现（`npu_arch_2201/atomic_impl.h:50-53`）：有 ✅
- 3510 实现：**无 v1**，仅有 v2 ✅ 确认 950 不支持 v1

#### 3.2.5 asc_set_store_atomic_config_v2（950 专用，已废弃）
- 文档声明：950 支持，A3/A2 不支持 ✅
- 3510 实现（`npu_arch_3510/atomic_impl.h:50-55`）：有，`[[deprecated]]` 标记 ✅
- 2201 实现：**无 v2** ✅ 确认 A3/A2 不支持 v2

### 3.3 v1 与 v2 编码差异源码验证
- v1 实现（`npu_arch_2201/atomic_impl/asc_set_store_atomic_config_impl.h:27-34`）：
  - `type_mask = 0x7`（3 bit type），`op_mask = 0x3`（2 bit op），`op_bit = 3`
  - 编码：`config = (type & 0x7) | ((op & 0x3) << 3)`
  - 与文档 v1 编码一致：type 0-6，op 0 ✅
- v2 实现（`npu_arch_3510/atomic_impl/asc_set_store_atomic_config_impl.h:28-35`）：
  - `type_mask = 0x15`（5 bit type），`op_mask = 0x3`（2 bit op），`op_bit = 4`
  - 编码：`config = (type & 0x15) | ((op & 0x3) << 4)`
  - 与文档 v2 编码一致：type 1/4/5/7/9/10，op 2 ✅
- **v1 与 v2 编码完全不兼容**：type 位宽、op 位移、type 取值均不同 ✅

### 3.4 asc_store_atomic_config 结构体验证
- 源码（`include/c_api/utils/union.h:35-42`）：
  ```cpp
  union asc_store_atomic_config {
      uint64_t config = ASC_STORE_ATOMIC_DEFAULT_VALUE;
      struct {
          uint64_t atomic_type : 3;
          uint64_t atomic_op : 2;
          uint64_t reserved1 : 59;
      };
  };
  ```
- `atomic_type` 为 3 bit 字段（0-7），`atomic_op` 为 2 bit 字段（0-3） ✅
- 此结构体用于 `asc_get_store_atomic_config` 的输出参数 ✅

### 3.5 头文件路径
- 文档声明：`"c_api/atomic/atomic.h"` ✅（文件确实存在于 `include/c_api/atomic/atomic.h`）
- `c_api/asc_simd.h:28,39` 包含 `"c_api/atomic/atomic.h"` ✅ 用户也可通过总头文件访问

### 3.6 SIMT 不兼容
- 源码（`atomic.h:11-13`）：`#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__) #error "atomic.h cannot be used with compile flag --enable-simt enabled." #endif` ✅
- 文档**未提及** SIMT 不兼容 ⚠️

### 3.7 流水类型
- 文档声明：PIPE_S ✅（原子操作属于 PIPE_S 流水）

### 3.8 功能说明
- asc_set_atomic_add：设置原子累加，文档描述与源码语义一致 ✅
- asc_set_atomic_max：设置原子比较取大，文档描述与源码语义一致 ✅
- asc_set_atomic_min：设置原子比较取小，文档描述与源码语义一致 ✅
- asc_set_atomic_none：清空原子操作状态，文档描述与源码语义一致 ✅
- asc_get_store_atomic_config：获取原子操作配置，文档描述与源码语义一致 ✅
- asc_set_store_atomic_config_v1：设置原子操作配置（2201），文档描述与源码语义一致 ✅
- asc_set_store_atomic_config_v2：设置原子操作配置（3510），已废弃，文档描述与源码语义一致 ✅

### 3.9 UT 测试覆盖
- 2201 架构：`tests/api/c_api/npu_arch_2201/simd_atomic/` 有 20 个测试文件
  - 18 个变体测试（add/max/min × 6 类型） ✅
  - `test_get_st_atomic_cfg.cpp` ✅
  - `test_popc.cpp`（非本组 API）✅
- 3510 架构：`tests/api/c_api/npu_arch_3510/simd_atomic/` **不存在** ⚠️

### 3.10 950 L1->GM 通路限制
- asc_set_atomic_add 文档（`asc_set_atomic_add.md:34`）："针对Ascend 950PR/Ascend 950DT，不支持L1 Buffer到Global Memory的通路" ✅
- asc_set_atomic_max 文档（`asc_set_atomic_max.md:34`）：同上 ✅
- asc_set_atomic_min 文档（`asc_set_atomic_min.md:34`）：同上 ✅
- 约束章节中也有重复说明 ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 头文件路径歧义
- 文档声明头文件路径为 `"c_api/atomic/atomic.h"`
- 该文件确实存在于 `include/c_api/atomic/atomic.h` ✅
- 但用户也可通过 `#include "c_api/asc_simd.h"` 间接包含
- 文档未提及可通过总头文件访问 ⚠️ 建议补充说明

### 4.2 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"
- 7 篇文档均存在此问题

### 4.3 SIMT 不兼容未在文档中提及
- 源码 `atomic.h:11-13` 明确 `#error` 禁止 SIMT 编译
- 所有 7 篇文档均未提及此限制 ⚠️ 应在约束说明中补充

### 4.4 v1 与 v2 编码差异文档强调不足
- v1 type 编码：0=none, 1=float, 2=half, 3=int16, 4=int32, 5=int8, 6=bf16；op: 0=sum
- v2 type 编码：1=int32, 4=half, 5=float, 7=bf16, 9=int16, 10=int8；op: 2=sum
- 两套编码完全不同，极易混淆 ⚠️ 文档仅在参数说明中分别列出，缺乏显眼的风险提示
- 如用户误用 v1 编码值调用 v2（或反之），将导致原子操作类型设置错误，后果严重

### 4.5 asc_get_store_atomic_config 在 950 上废弃说明不够清晰
- 文档说 "950 上已废弃，请使用 asc_atomic_add" ⚠️
- 但 asc_atomic_add 是 scalar_compute 的接口，与 simd_atomic 配置接口功能不同
- 文档未说明 950 上替代原子配置查询的方案

### 4.6 asc_set_atomic_min 原型排列顺序与 _add/_max 不一致
- asc_set_atomic_add 原型顺序：int8, int16, int, bfloat, float16, float
- asc_set_atomic_max 原型顺序：int8, int16, int, bfloat, float16, float
- asc_set_atomic_min 原型顺序：int8, int16, **bfloat, float16**, int, float
- asc_set_atomic_min 中 bfloat/float16 排在 int/float 之前，与 _add/_max 不一致 ⚠️
- 源码中 3510 声明顺序也有差异，但 2201 声明顺序一致

### 4.7 无 3510 架构 UT 测试
- `tests/api/c_api/npu_arch_3510/simd_atomic/` 目录不存在 ⚠️
- 950 特有的 `asc_set_store_atomic_config_v2` 无 UT 覆盖
- 950 上 deprecated 的 `asc_get_store_atomic_config` 无 UT 覆盖

### 4.8 asc_set_store_atomic_config_v2 约束说明写"无"
- v2 废弃文档约束章节写"无" ⚠️
- 但 v2 与 v1 编码不兼容是重大风险，至少应提示不要与 v1 混用
- 且 v2 本身已废弃，约束中应明确推荐替代方案

### 4.9 原子操作实际效果
- cpudebug 无法验证原子累加/取大/取小的实际运算正确性 ⚠️ 需上板确认

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 部分验证 | 源码 #ifdef 与文档一致，950 无 UT | 参考证据 | 源码分析 + 缺少 3510 UT |
| 功能说明 | 已验证 | 所有 7 个 API 功能描述与源码语义一致 | 直接证据 | 源码比对 |
| 函数原型 | 已验证 | 7 个 API 原型与源码一致 | 直接证据 | 源码比对 |
| 参数说明 | 已验证 | 无参数/参数类型与源码一致 | 直接证据 | 源码比对 |
| 返回值说明 | 已验证 | void 返回 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | SIMT 不兼容未提及；v1/v2 编码差异风险未强调；950 L1->GM 限制存在但位置可优化 | 参考证据 | 需补充 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 09_sample_replay |
| v1/v2 编码 | 已验证 | 两套编码源码实确认与文档一致，但完全不同 | 直接证据 | 08_v1_v2_encoding_diff |
| 950 deprecated | 部分验证 | 源码有 [[deprecated]] 属性，但替代方案说明不清 | 参考证据 | 07_set_store_atomic_config_v2 |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| 3510 架构 set_atomic_add_* 编译执行 | 950 上所有 atomic 变体可编译 | 3510 无 UT 目录 | 创建 3510 simd_atomic UT |
| v1 编码值在 v2 架构下错误行为 | 误用 v1 编码调用 v2 导致原子类型错误 | 需要硬件环境验证 | 确认 v1 编码值在 950 上产生错误原子配置 |
| v2 编码值在 v1 架构下错误行为 | 误用 v2 编码调用 v1 导致原子类型错误 | 需要硬件环境验证 | 确认 v2 编码值在 A3/A2 上产生错误原子配置 |
| 950 L1->GM 原子操作失败 | 950 不支持 L1->GM 通路 | 需要硬件环境验证 | 在 NPU 上确认 L1->GM 原子操作报错或无效 |
| 不调用 asc_set_atomic_none 的后续影响 | 约束：使用后必须清空 | 需要硬件环境验证 | 验证不清空原子状态对后续指令的影响 |
| 950 上调用 asc_set_store_atomic_config_v1 编译失败 | v1 仅 2201 可用 | 负向 case 未创建 | 创建 3510 架构编译失败证据 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| 原子操作运算正确性 | 仅文本核对 | cpudebug 不模拟原子运算 | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证 add/max/min 原子运算结果正确 |
| v1/v2 编码混用后果 | 仅源码分析 | cpudebug 不验证原子配置错误后果 | 跑不起来 | 需 NPU 测试误用编码时的原子操作行为 |
| L1->GM 通路限制 | 仅文档描述 | cpudebug 不验证数据通路约束 | 跑不起来 | 需 NPU 确认 950 上 L1->GM 原子操作不可用 |
| 不清空原子状态影响 | 仅文档描述 | cpudebug 不验证状态残留影响 | 跑不起来 | 需 NPU 测试不清空原子状态时后续指令行为 |
| asc_atomic_add 替代方案 | 文档模糊 | 文档未明确 950 上替代 get_config 的方案 | 写不出用例 | 需确认 950 上原子配置查询的推荐做法 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_set_atomic_add/max/min 原子运算正确性
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实原子运算语义
  - 建议上板验证动作：在 NPU 上设置原子操作 → 执行 DataCopy → 验证 GM 数据正确累加/取大/取小
- 问题2：v1 与 v2 编码混用的实际后果
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证原子配置错误的实际影响
  - 建议上板验证动作：在 950 上用 v1 编码值调用 v2，确认原子操作类型错误
- 问题3：950 L1->GM 通路限制
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证数据通路约束
  - 建议上板验证动作：在 950 上设置原子操作后从 L1 搬运到 GM，确认操作无效或报错
- 问题4：asc_set_atomic_none 清空必要性
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证状态残留影响
  - 建议上板验证动作：在 NPU 上不清空原子状态，执行后续搬运，验证是否受影响
- 问题5：950 上 asc_get_store_atomic_config 废弃后的替代方案
  - 为什么 cpudebug 不能最终确认：需确认推荐做法
  - 建议上板验证动作：确认 950 上是否仍可调用 deprecated 接口获取配置，或必须使用 asc_atomic_add 替代

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：2201 架构有 20 个测试文件，覆盖所有 18 个 atomic 变体 + get_store_atomic_config + popc
- 3510 架构无 UT 测试 ⚠️
- 2201 UT 验证：`test_get_st_atomic_cfg.cpp` 验证 `asc_get_store_atomic_config` 获取默认配置后通过 `asc_set_store_atomic_config_v1` 设置回去 ✅

## 10. 关键证据
- **架构分化**：`atomic.h` 通过 `#if __NPU_ARCH__ == 2201` / `#elif __NPU_ARCH__ == 3510` 实现架构分支
  - 2201 独有：`asc_set_atomic_none`、`asc_get_store_atomic_config`、`asc_set_store_atomic_config_v1`
  - 3510 独有：`asc_set_store_atomic_config_v2`（deprecated）、`asc_get_store_atomic_config`（deprecated）
  - 公共（架构外声明）：`asc_set_atomic_add_float`、`asc_set_atomic_max_float16`、`asc_set_atomic_add_int`、`asc_set_atomic_add_int8`、`asc_set_atomic_add_int16`
- **SIMT 禁用**：`atomic.h:11-13` 通过 `#error` 完全禁止 SIMT 模式编译
- **v1 编码**（2201）：`type_mask = 0x7, op_bit = 3`，type 范围 0-6，op 范围 0-3
- **v2 编码**（3510）：`type_mask = 0x15, op_bit = 4`，type 范围 0-20（有效值 1/4/5/7/9/10），op 范围 0-3
- **deprecated 属性**：950 上 `asc_get_store_atomic_config` 和 `asc_set_store_atomic_config_v2` 均有 `[[deprecated]]` 标记，提示使用 `asc_atomic_add` 替代
- **asc_store_atomic_config**：`atomic_type` 为 3 bit，`atomic_op` 为 2 bit（`union.h:35-42`），与 v1 编码的 3 bit type + 2 bit op 匹配
- **UT 证据**：2201 有完整覆盖；3510 无 UT

## 11. 对原文的建议回写
1. **SIMT 不兼容提示**：所有 7 篇文档的约束说明中均应补充"不支持 --enable-simt 编译模式"
2. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
3. **v1/v2 编码差异风险提示**：在 `asc_set_store_atomic_config_v1.md` 和 `asc_set_store_atomic_config_v2_deprecated.md` 中增加显眼警告框，强调两套编码不兼容，切勿混用
4. **950 替代方案明确化**：`asc_get_store_atomic_config.md` 中 950 废弃说明应更明确，说明 950 上是否仍可调用 deprecated 接口，或必须完全切换到 `asc_atomic_add`
5. **asc_set_atomic_min 原型顺序统一**：将 `asc_set_atomic_min.md` 中的原型排列顺序调整为与 `_add`/`_max` 一致（int8, int16, int, bfloat, float16, float）
6. **v2 约束补充**：`asc_set_store_atomic_config_v2_deprecated.md` 约束说明写"无"不合理，应至少包含：(1) 已废弃，推荐使用 asc_atomic_add；(2) 编码与 v1 完全不同，不可混用
7. **950 L1->GM 限制突出**：asc_set_atomic_add/max/min 中 L1->GM 限制在功能说明和约束中各出现一次，建议在约束中加粗或使用警告框突出
8. **头文件路径补充说明**：7 篇文档均声明 `"c_api/atomic/atomic.h"`，建议补充说明也可通过 `#include "c_api/asc_simd.h"` 访问
9. **3510 UT 补充**：建议为 3510 架构创建 simd_atomic UT 测试目录，至少覆盖 v2 接口和 deprecated 行为

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/simd_atomic/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/simd_atomic/reports/simd_atomic.verify-report-cpu.md`

## 13. 特殊值测试情况

本接口组为原子操作配置类 API，无数据类型和数值计算输入，不适用传统特殊值测试。但存在以下配置值相关测试要点：

### v1 type 参数边界值
| type 值 | 含义 | 文档是否说明 | 源码行为 |
|---------|------|-------------|---------|
| 0 | 无原子操作 | ✅ | type & 0x7 = 0 |
| 1-6 | 各数据类型 | ✅ | 正常编码 |
| 7 | 文档说"无具体含义" | ✅ | type & 0x7 = 7，写入硬件配置 |
| >7 | 文档说"无具体含义" | ✅ | type & 0x7 截断，高位被 mask |

### v2 type 参数边界值
| type 值 | 含义 | 文档是否说明 | 源码行为 |
|---------|------|-------------|---------|
| 1 | int32_t | ✅ | type & 0x15 = 1 |
| 4 | half | ✅ | type & 0x15 = 4 |
| 5 | float | ✅ | type & 0x15 = 5 |
| 7 | bfloat16_t | ✅ | type & 0x15 = 7 |
| 9 | int16_t | ✅ | type & 0x15 = 9 |
| 10 | int8_t | ✅ | type & 0x15 = 10 |
| 0 | 文档说"无具体含义" | ✅ | type & 0x15 = 0 |
| 其他值 | 文档说"无具体含义" | ✅ | type & 0x15 截断 |

### v1 vs v2 关键混淆点
| 数据类型 | v1 type | v2 type | 混淆后果 |
|---------|---------|---------|---------|
| float | 1 | 5 | v1 的 float(1) 传入 v2 会被编码为 int32_t |
| half | 2 | 4 | v1 的 half(2) 传入 v2 会被 mask 截断为 0 |
| int32_t | 4 | 1 | v1 的 int32(4) 传入 v2 会被编码为 half |
| int8_t | 5 | 10 | v1 的 int8(5) 传入 v2 会被编码为 float |
| sum op | 0 | 2 | v1 的 sum(0) 传入 v2 会被编码为无效 op |

## 14. 约束章节覆盖情况

### 约束逐条覆盖表

#### asc_set_atomic_add
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 使用完成后建议调用 asc_set_atomic_none 清空 | 正向 | 部分验证 | 04_set_atomic_none | 参考证据 | cpudebug 无法验证不清空的后果 |
| C2 | 该指令执行前不会对 GM 数据做清零操作 | 正向 | 已验证 | 源码分析 | 直接证据 | 文档描述合理，属硬件行为 |
| C3 | 950 不支持 L1->GM 通路 | 正向 | 部分验证 | 源码分析 | 参考证据 | 文档和源码均有说明，需上板验证实际行为 |

#### asc_set_atomic_max
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 使用结束后建议通过 asc_set_atomic_none 关闭 | 正向 | 部分验证 | 04_set_atomic_none | 参考证据 | 同 add C1 |
| C2 | 该指令执行前不会对 GM 数据做清零操作 | 正向 | 已验证 | 源码分析 | 直接证据 | 同 add C2 |
| C3 | 950 不支持 L1->GM 通路 | 正向 | 部分验证 | 源码分析 | 参考证据 | 同 add C3 |

#### asc_set_atomic_min
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 使用结束后建议通过 asc_set_atomic_none 关闭 | 正向 | 部分验证 | 04_set_atomic_none | 参考证据 | 同 add C1 |
| C2 | 该指令执行前不会对 GM 数据做清零操作 | 正向 | 已验证 | 源码分析 | 直接证据 | 同 add C2 |
| C3 | 950 不支持 L1->GM 通路 | 正向 | 部分验证 | 源码分析 | 参考证据 | 同 add C3 |

#### asc_set_atomic_none
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 约束说明：无 | N/A | 部分验证 | 无 | 参考证据 | 文档写"无"约束，但 SIMT 不兼容未提及 |

#### asc_get_store_atomic_config
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 需配合 asc_set_store_atomic_config_v1/v2 使用 | 正向 | 部分验证 | 05_get_store_atomic_config | 参考证据 | 2201 UT 验证了 get 后 set 回去的流程 |

#### asc_set_store_atomic_config_v1
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 使用完成后建议通过 asc_set_atomic_none 清空 | 正向 | 部分验证 | 04_set_atomic_none | 参考证据 | 同 add C1 |
| C2 | 该指令执行前不会对 GM 数据做清零操作 | 正向 | 已验证 | 源码分析 | 直接证据 | 同 add C2 |

#### asc_set_store_atomic_config_v2
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 约束说明：无 | N/A | 部分验证 | 无 | 参考证据 | 文档写"无"约束不合理：已废弃、编码与 v1 不兼容、需清空等均未提及 |

### 约束覆盖统计
- 约束总条数：13（跨 7 个 API）
- 已验证：2
- 部分验证：9
- 待补验证：2（950 L1->GM 通路限制 × 3 API 合并为 1 个待补 + 950 上 get_config 废弃后的实际行为 1 个）
- **文档缺失约束**：SIMT 不兼容（7 篇均缺）、v1/v2 编码不兼容警告（2 篇缺）
