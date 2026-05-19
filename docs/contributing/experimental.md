# Experimental 特性开发规范

## 1. 什么是 Experimental 特性

Experimental 特性是指尚未稳定、API 可能发生变更的功能。通过以 experimental 形式发布，可以：

- 尽早向社区开放新能力供试用和反馈
- 在真实使用场景中验证 API 设计的合理性
- 必要时对 API 进行 breaking change 而不影响稳定 API 的兼容性承诺

---

## 2. 目录位置

### 2.1 主要位置：tensor_api

`tensor_api` 是本项目主要的 experimental 区域：

```
include/tensor_api/         # Experimental API 声明
impl/tensor_api/             # Experimental API 实现
tests/api/tensor_api/        # Experimental API 测试
```

在 `README.md` 目录说明中标注为 `experimental`。

### 2.2 外部引用路径

外部文档中 experimental 特性的 include 路径使用 `experimental/` 前缀：

```cpp
#include "include/experimental/tensor_api/tensor.h"
```

### 2.3 代码内 Experimental 标注

部分在 `adv_api` 等非 experimental 目录下的实现也标记了 experimental（如 matmul scheduler），通过代码注释声明其状态。

---

## 3. 代码标记规范

### 3.1 头文件标记（建议格式）

在头文件顶部注释中明确标注：

```cpp
/*!
 * \file tensor.h
 * \brief Tensor API (Experimental)
 *
 * \note This API is experimental and may change in future releases.
 */
```

或使用行注释：

```cpp
// NOTE: This API is experimental. Interface may change without notice.
```

### 3.2 Impl 文件标记（matmul scheduler 模式）

```cpp
// MatmulScheduler is considered entirely experimental.
// The interface and behavior may change in future releases.
```

### 3.3 README 目录说明（tensor_api 模式）

在项目 README 的目录树中标注 `experimental`：

```
├── experimental                    # Ascend C TENSOR API实现源代码
```

### 3.4 文档标注

在 API 文档中添加 `\note`：

```cpp
/*!
 * \note This API is experimental. The interface may change without notice.
 * \note For stable alternatives, see ...
 */
```

---

## 4. 质量门槛

Experimental 特性不等于低质量。即使是实验性代码，也必须满足：

| 要求 | 说明 |
|------|------|
| 编译通过 | 所有 Header Checker 模式通过 |
| 功能测试 | 提供基本功能 UT |
| 代码审查 | 通过正常 Code Review 流程 |
| 文档 | 有 API 文档并标注 experimental |
| CMake 集成 | 正确集成到构建系统 |

---

## 5. 转正（Graduation）流程

### 5.1 转正条件

Experimental 特性满足以下条件可申请转正为 stable API：

1. 至少在一个正式版本中作为 experimental 发布使用
2. 经社区反馈确认 API 设计合理，无待解决的设计问题
3. 完整的功能测试和性能测试通过
4. 文档完善（移除 experimental 标注，补充完整参数说明和约束）
5. 无已知的严重 Bug

### 5.2 转正操作步骤

```
□ 提交 Issue 提议转正，说明使用情况和稳定性证据
□ 经社区讨论获得共识
□ 更新代码注释：移除 experimental 标注
□ 更新 API 文档：移除 experimental 警告
□ 更新 README.md：移除 experimental 目录标注
□ 更新 CHANGELOG.md：记录转正
□ 如有 include 路径变更，更新所有引用
```

### 5.3 不转正的情况

如果 experimental 特性经评估不适合转正，可能：

- **合并入已有 stable API：** 功能通过扩展已有 API 实现
- **废弃删除：** 在后续版本中移除，需提前一个版本给出弃用警告

---

## 6. Breaking Change 策略

### 6.1 Experimental 阶段

- 允许不向前兼容的修改
- Breaking change 必须在 CHANGELOG.md 中记录
- 建议在 PR 描述中以 `**BREAKING:**` 前缀标注

### 6.2 转正后

- 不再允许 breaking change（遵循 stable API 兼容性策略）
- 如需修改，走标准的弃用 → 新增 → 移除流程

---

## 7. 两种 Experimental 模式的对比

项目实际存在两种 experimental 形式：

| 维度 | tensor_api 模式 | matmul scheduler 模式 |
|------|----------------|---------------------|
| 位置 | 独立 `tensor_api/` 目录 | `adv_api/detail/matmul/scheduler/` 内部 |
| 标记方式 | README + 目录说明 | 代码注释 |
| 独立性 | 完整的独立 API 体系 | 现有 API 的内部实验性算法 |
| 转正方式 | 独立 API 转正 | 合并入主 API 或删除 |
| 预期生命周期 | 可能成为长期独立 API | 实验验证后合并或废弃 |

---

## 8. 新增 Experimental 特性清单

```
□ 确定 experimental 模式：独立 API（tensor_api）还是内部实验（注释标注）

□ 独立 API 模式：
    □ include/tensor_api/<category>/<api>.h
    □ impl/tensor_api/<category>/<api>_impl.h
    □ tests/api/tensor_api/<category>/test_operator_<api>.cpp
    □ 更新 README.md 标注 experimental
    □ 更新相关 CMakeLists.txt

□ 内部实验模式（含 matmul scheduler 类）：
    □ 在对应 impl 文件中添加 "is considered entirely experimental" 注释
    □ 在对外头文件名或注释中标注 experimental
    □ 添加相应测试
    □ 在 commit message 或 PR 描述中说明为 experimental

□ 两种模式共同要求：
    □ 文档标注 experimental
    □ CHANGELOG 中记录（如对外可见）
    □ 通过 Code Review
```
