---
name: "image-filename-converter"
description: "将中文图片文件名转换为英文小写加下划线格式，并更新所有markdown文件中的引用。当用户需要重命名图片文件或修复文档中的图片引用时调用此技能。"
---
<!-- - 图片文件名转换 / 重命名图片
- 中文转英文加下划线 / 英文小写加下划线
- 修复图片引用 / 更新markdown中的图片路径
- 图片文件命名规范化 / 清理图片文件名 -->

# 图片文件名转换工具

将中文（或中英混合）图片文件名转换为英文小写加下划线格式，并同步更新所有 markdown 文件中的引用路径。

## 使用时机

当用户提出以下需求时调用此技能：
- 将图片文件名从中文改为英文小写加下划线格式
- 修复重命名后的图片引用
- 进行文档图片文件命名规范化清理

## 工作流程

### 步骤 1：理解图片含义

阅读 markdown 文件中图片引用周围的文字内容，理解图片所描述的含义，从而确定合适的英文名称。

### 步骤 2：定义映射关系

建立清晰的映射表：
- 原始 alt 信息（中文短语下划线分割）
- 原始文件名
- 新文件名（英文小写加下划线，`.png` 扩展名）

### 步骤 3：更新 Markdown 引用

更新所有 `![](路径/文件名.png "alt信息")` 引用，指向新的文件名。

### 步骤 4：重命名物理文件

使用操作系统重命名命令修改实际图片文件名称。

## 命名规范

| 规则 | 示例 |
|------|------|
| 仅英文小写 | `atlas_a2_a3_architecture` |
| 下划线分隔 | `vector_compute_data_flow` |
| 无特殊字符 | 不允许 `图1-`、`()`、`（` 等 |
| .png 扩展名 | `intra_core_pipeline_timing.png` |
| 描述性且简洁 | `ascend_950pr_950dt_architecture.png` |

## 常见翻译对照

| 中文概念 | 英文翻译 |
|---------|---------|
| 架构图 | architecture |
| 示意图 | diagram |
| 硬件 | hardware |
| 核内 | intra_core |
| 核间 | inter_core |
| 并行 | parallel |
| 流水 | pipeline |
| 执行时序 | execution_timing / timing |
| 数据流 | data_flow |
| 同步 | sync / synchronization |
| 全核 | all_core |
| 场景示例 | scenario_example |
| 关系图 | relationship_diagram |
| 计数器 | counter |
| 设置 | set |
| 复用 | reuse |
| 错误示范 | error_example |

## 操作示例

修改前：
```
![](../../../../figures/架构图.png "Atlas_A2_A3_硬件架构图")
```
修改后：
```
![](../../../../figures/atlas_a2_a3_architecture.png "Atlas_A2_A3_硬件架构图")
```
物理文件：`架构图.png` → `atlas_a2_a3_architecture.png`
