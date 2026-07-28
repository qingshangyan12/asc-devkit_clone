# dav-3510 基础 API 与 C API CCE 指令索引

## 用途

本目录为后续 agent 提供静态源码索引：在 `dav-3510` 下定位基础 API 与 C API 使用同一 CCE 指令的实现入口。它用于缩小阅读范围和建立迁移候选，**不构成 API 可直接替换或数值语义完全等价的结论**。

## 文件

| 文件 | 作用 |
| --- | --- |
| `cce_instruction_index.json` | 机器可读的 API/CCE 对应索引。优先作为 agent 输入。 |
| `README.md` | 索引范围、字段定义与使用顺序。 |
| `appendix.md` | 软实现、疑问、文档/实现矛盾及未建立一对一映射的情况。 |

## 静态分析范围

| 优先级 | 基础 API 源码 | 处理方式 |
| --- | --- | --- |
| 高 | `impl/basic_api/reg_compute/dav_3510` | 主要索引来源。此目录大量 Reg API 直接调用 CCE 指令。 |
| 低 | `impl/basic_api/dav_3510` | 仅记录可静态确认的直接指令入口；大量代码带 CPU 调试分支、对象/队列编排或兼容层，不将其自动等同于 C API。 |
| 对照 | `impl/c_api/instr_impl/npu_arch_3510` | C API 包装层及实际 CCE 调用来源。 |

未纳入 `dav-2201` 或其他架构路径；也未把主机 ACL、样例、Python 脚本或仅声明文件作为实现证据。

## JSON 字段

每个 `mappings` 条目包含：

- `basic_api`：基础 API 的公开操作族和实现文件；
- `c_api`：候选 C API 操作族和实现文件；
- `shared_cce`：两侧静态可见的共同 CCE 指令；
- `confidence`：`exact`、`family`、`composite` 或 `candidate`；
- `notes`：必须在迁移前核对的参数、dtype、mask、SPR、布局或同步条件。

置信度解释：

| 值 | 含义 |
| --- | --- |
| `exact` | 双方包装层对同一单个 CCE 指令的直接调用已静态确认。仍需核对重载、dtype、mask和模式位。 |
| `family` | 双方使用同一 CCE 指令族，但具体重载、变体或参数组合尚需按调用点确认。 |
| `composite` | 基础 API 或 C API 在同一指令族上叠加了多条指令、特判或格式转换，不能视为一对一。 |
| `candidate` | 仅在低优先级目录发现候选直接调用，尚未完成参数级审计。 |

## 建议读取流程

1. 用任务涉及的基础 API 名称或 CCE 指令名检索 `cce_instruction_index.json`。
2. 仅当 `confidence=exact` 时，先阅读双方列出的实现文件和公共 API 文档，确认目标 dtype/架构/重载。
3. 对 `family`、`composite`、`candidate` 条目，先阅读 `appendix.md` 对应限制；不得据此直接改写代码。
4. 若索引未命中，先在高优先级目录中检索 CCE 指令，再读取低优先级 `impl/basic_api/dav_3510`。不要从 2201 兼容/软实现反推 3510 行为。

## 覆盖统计

对两个高优先级实现树的 CCE 调用标识符做静态交集（排除预处理宏 `defined`）得到 **115** 个共享指令名。JSON 的 `all_shared_cce_names` 保留完整名称集合；`mappings` 和 `shared_cce_groups` 按 API 操作族聚合，避免因 dtype、B64、双寄存器、后更新、对齐/非对齐等重载造成重复条目。

本索引只做文本和源码路径核对，未编译、未运行、未反汇编。
