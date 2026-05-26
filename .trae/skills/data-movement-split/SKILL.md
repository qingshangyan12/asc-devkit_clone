---
name: "data-movement-split"
description: "数据搬运类接口资料按照数据通路拆分重组。触发词：数据搬运资料回合。适用于整理数据搬运类（DataCopy等）API文档时按通路（如GM->UB、GM->L1）解耦资料。"
---

# 数据搬运资料回合

## 技能用途

将数据搬运类接口的 Markdown 文档，按照数据通路（Data Path）进行拆分和解耦。

### 背景

Memory 数据搬运中的很多接口都有多个重载（overload），支持不同的数据通路。例如 `DataCopy` 接口同时支持：

- **GM → VECIN（GM → UB）**
- **GM → A1、B1（GM → L1 Buffer）**
- **VECIN → VECOUT（UB → VECOUT）**
- 等等

按照通路拆分资料，即把原来混杂在同一个接口文档中的多个通路信息解耦出来，例如：
- 某个章节专门讲支持 **GM → UB** 的接口
- 另一个章节专门讲支持 **GM → L1 Buffer** 的接口

## 参考文件

- `docs/api/SIMD-API/通用说明和约束.md` — 通用的 TPoisition 与物理内存映射关系。
- 各数据搬运接口的原始 Markdown 文档（如 `基础数据搬运.md` 等）,存储的windows路径为"C:\Users\guore\Desktop\蓝区样例开发\资料回合\缓存控制\asc-devkit_cache\docs\api\SIMD-API\基础API\数据搬运_top5"。

## 拆分的核心步骤

### 1. 识别数据通路

从接口文档的"支持的通路和数据类型"章节中，提取出所有数据通路。通路的典型格式为：

```
GM -> VECIN（GM -> UB ）
GM -> A1、B1（GM -> L1 Buffer ）
GM -> VECOUT（GM -> UB ）
VECIN -> VECOUT（UB -> VECOUT）
VECIN -> GM（UB -> GM）
VECCALC -> VECOUT（UB -> VECOUT）
```

通路由"源 TPosition → 目的 TPosition"组成，括号内为物理存储说明。

### 2. 按通路分组

将同一类通路归为一组。常见分组维度：

| 分组 | 通路示例 | 说明 |
|------|---------|------|
| **GM → Local Memory** | GM → VECIN、GM → A1/B1、GM → VECOUT | 从Global Memory搬运到各Local Memory |
| **Local Memory → Local Memory** | VECIN → VECOUT、VECCALC → VECOUT | Local Memory之间的搬运 |
| **Local Memory → GM** | VECIN → GM、VECOUT → GM | 从Local Memory写回Global Memory |

### 3. 每个通路收集的信息

对于每个通路，需要从原始文档中提取并整理以下信息：

- **数据通路名称**（源 → 目的）
- **物理存储说明**（如 GM → UB、GM → L1 Buffer）
- **支持的产品型号**（哪些芯片支持该通路）
- **源操作数数据类型**（该通路支持哪些数据类型）
- **目的操作数数据类型**（该通路支持哪些数据类型）
- **特殊约束**（如对齐要求、数量限制等，如有则提取）

### 4. 输出拆分后的资料结构

以 `DataCopy` 接口为例，拆分后各章节的内容结构：

```
## DataCopy（GM → Local Memory）

### 支持的通路和数据类型
（列出所有 GM→Local Memory 的通路及其支持的产品和数据类型）

### 函数原型
（仅列出 GM→Local Memory 相关的原型）

### 参数说明
（通用参数说明）

### 调用示例
（仅涉及 GM→Local Memory 的示例）
```

## 使用流程

1. **读取原始文档**：使用 `Read` 工具读取数据搬运接口的原始 Markdown 文件
2. **读取通用说明**：参考 `通用说明和约束.md` 中的 TPosition 映射关系
3. **识别并提取所有通路**：从"支持的通路和数据类型"章节中提取
4. **按通路分组整理**：将接口信息按通路分组
5. **输出拆分方案**：向用户展示拆分后的结构方案
6. **确认并执行**：用户确认后，生成拆分后的文档内容

## 注意事项

- 通路信息跨多个产品型号时，注意不同产品的差异（如某些型号不支持特定通路）
- 数据类型用标准写法（如 `int8_t`），按规范顺序排列
- 保留原始文档中的约束说明和对齐要求
- 函数原型中与通路无关的部分（如公共模板参数）保留在通用部分
