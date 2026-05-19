# Experimental特性开发规范

## 什么是Experimental特性

Experimental特性是指尚未稳定、API可能发生变更的功能。通过以experimental形式发布，可以：

- 尽早向社区开放新能力供试用和反馈
- 在真实使用场景中验证API设计的合理性
- 必要时对API进行breaking change而不影响稳定API的兼容性承诺

---

## 目录位置

### 主要位置：tensor_api

`tensor_api` 是本项目主要的experimental区域：

```cpp
include/tensor_api/         # Experimental API 声明
impl/tensor_api/             # Experimental API 实现
tests/api/tensor_api/        # Experimental API 测试
```

在 `README.md` 目录说明中标注为 `experimental`。

### 外部引用路径

外部引用experimental特性使用 `tensor_api/` 前缀：

```cpp
#include "include/tensor_api/tensor.h"
```

### 代码内Experimental标注

部分在 `adv_api` 等非experimental目录下的实现也标记了experimental（如matmul scheduler），通过代码注释声明其状态。

---

## 代码标记规范

### 头文件标记（建议格式）

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

### Impl文件标记（matmul scheduler模式）

```cpp
// MatmulScheduler is considered entirely experimental.
// The interface and behavior may change in future releases.
```

### README目录说明（tensor_api模式）

在项目README的目录树中标注 `experimental`：

```cpp
├── experimental                    # Ascend C TENSOR API实现源代码
```

### 文档标注

在API文档中添加 `\note`：

```cpp
/*!
 * \note This API is experimental. The interface may change without notice.
 * \note For stable alternatives, see ...
 */
```

---

## 质量门槛

Experimental特性不等于低质量。即使是实验性代码，也必须满足：

| 要求 | 说明 |
|------|------|
| 编译通过 | 头文件可独立编译 |
| 功能测试 | 提供基本功能UT |
| 代码审查 | 通过正常Code Review流程 |
| 文档 | 有API文档并标注experimental |
| CMake集成 | 正确集成到构建系统 |

---

## 转正（Graduation）流程

### 转正条件

Experimental特性满足以下条件可申请转正为stable API：

1. 至少在一个正式版本中作为experimental发布使用
2. 经社区反馈确认API设计合理，无待解决的设计问题
3. 完整的功能测试和性能测试通过
4. 文档完善（移除experimental标注，补充完整参数说明和约束）
5. 无已知的严重Bug

### 转正操作步骤

```text
□ 提交 Issue 提议转正，说明使用情况和稳定性证据
□ 经社区讨论获得共识
□ 更新代码注释：移除 experimental 标注
□ 更新 API 文档：移除 experimental 警告
□ 更新 README.md：移除 experimental 目录标注
□ 更新 CHANGELOG.md：记录转正
□ 如有 include 路径变更，更新所有引用
```

### 不转正的情况

如果experimental特性经评估不适合转正，可能：

- **合并入已有stable API：** 功能通过扩展已有API实现
- **废弃删除：** 在后续版本中移除，需提前一个版本给出弃用警告

---

## Breaking Change策略

### Experimental阶段

- 允许不向前兼容的修改
- Breaking change必须在CHANGELOG.md中记录
- 建议在PR描述中以 `**BREAKING:**` 前缀标注

### 转正后

- 不再允许breaking change（遵循stable API兼容性策略）
- 如需修改，走标准的弃用 → 新增 → 移除流程

---

## 两种Experimental模式的对比

项目实际存在两种experimental形式：

| 维度 | tensor_api模式 | matmul scheduler模式 |
|------|----------------|---------------------|
| 位置 | 独立 `tensor_api/` 目录 | `adv_api/detail/matmul/scheduler/` 内部 |
| 标记方式 | README + 目录说明 | 代码注释 |
| 独立性 | 完整的独立API体系 | 现有API的内部实验性算法 |
| 转正方式 | 独立API转正 | 合并入主API或删除 |
| 预期生命周期 | 可能成为长期独立API | 实验验证后合并或废弃 |

---

## 新增Experimental特性清单

```text
□ 确定 experimental 模式：独立 API（tensor_api）还是内部实验（注释标注）

□ 独立 API 模式：
    □ include/tensor_api/<category>/<api>.h
    □ impl/tensor_api/<category>/<api>_impl.h
    □ tests/api/tensor_api/<category>/test_tensor_api_<api>.cpp
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
