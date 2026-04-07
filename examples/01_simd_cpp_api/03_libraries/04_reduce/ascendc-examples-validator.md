v---
name: ascendc-examples-validator
description: 样例检查技能。用于检查 README.md 文档规范性、.asc 文件注释规范性，验证 README 与代码实现一致性。包含：多场景样例检查、专业术语定义验证、数据流路径描述验证、图表资源引用检查等。
---

# Ascend C 样例检查技能

## 技能描述

本技能用于检查单个样例目录的 README.md 文档规范性和 .asc 文件注释规范性，以及验证 README 与代码实现的一致性。

## 触发时机

当用户需要检查某个样例的完整性和规范性时使用本技能：
- 验证 README 描述与代码实现是否一致
- 检查 README 文档是否符合描述规范
- 检查 .asc 文件代码注释是否符合规范
- 发现文档、注释与代码不一致的问题
- **检查多场景样例的场景说明完整性**
- **验证专业术语定义的清晰度**
- **检查图表资源引用的完整性**
- **验证数据流路径描述的完整性**

## README 描述规范

检查 README.md 是否符合以下规范要求：

### 1. Shape 格式规范
- **要求**：shape 格式必须使用 `[xx,xx]` 形式
- **禁止**：不能使用 `*`、`()` 等其他格式
- **示例**：
  - ✅ 正确：`[1024]`、`[512, 512, 1024]`、`[N, C, H, W]`
  - ❌ 错误：`1024*1024`、`(1024, 1024)`、`1024, 1024`

### 2. API 使用背景描述
- **要求**：描述 API 使用的背景场景，再详细描述 API 内容
- **示例**：
  - ✅ 正确："在卷积场景下实现了 Padding 功能，使用 Pad API 对输入 Tensor 进行边界填充..."
  - ❌ 错误："本样例实现了 Padding 功能"

### 3. 概述详细程度
- **要求**：概述部分描述需要详细，不能一句话简单了事，应包含使用场景、API、功能说明
- **示例**：
  - ✅ 正确1："本样例在卷积场景下，基于DataCopy实现数据随路量化激活搬运，支持在数据搬运过程中通过Scalar量化将int32_t类型转换为half类型，并完成L0C Buffer到Global Memory通路NZ到ND格式转换，同时支持Relu激活。"
  - ✅ 正确2："本样例在矩阵乘法场景下，基于MatMul实现矩阵乘法运算，支持int8_t类型矩阵相乘得到int32_t类型结果，通过双缓冲和流水线优化提高计算效率。"
  - ✅ 正确3："本样例在归约场景下，基于BlockReduceMax实现最大值归约，对输入Tensor的每个datablock内所有元素求最大值。"
  - ❌ 错误："本样例实现了加法功能"
  - ❌ 错误："本样例基于DataCopy实现数据搬运。"

### 4. 模糊概念清晰化
- **要求**：模糊概念应描述清楚，指明具体含义
- **示例**：
  - ✅ 正确："非对齐指的是 Tensor 的首地址不满足 32 字节对齐要求"
  - ❌ 错误："本样例处理非对齐情况"

### 5. Tensor 大小描述术语
- **要求**：描述 Tensor 大小时使用"数据量"或"元素个数"，禁止使用"长度"
- **示例**：
  - ✅ 正确："数据量为 1024"、"元素个数为 1024"
  - ❌ 错误："长度为 1024"

### 6. Shape 描述准确性
- **要求**：shape 描述要准确，维度数量必须匹配
- **示例**：
  - ✅ 正确：四维 Tensor 描述为 `[N, C, H, W]` 或 `[yy, yy, yy, yy]`
  - ❌ 错误：四维 Tensor 描述为 `[N, C, H]`

### 7. 概念术语规范
- **要求**：弱化"算子"概念，统一使用"样例"描述
- **示例**：
  - ✅ 正确："本样例实现了..."、"样例功能..."
  - ❌ 错误："本算子实现了..."、"算子功能..."

### 8. 软仿接口处理
- **要求**：软仿接口需要提供数据差的理由、数据、实现原理，或直接删除
- **检查项**：
  - 如果保留软仿接口，是否说明了数据差的原因
  - 是否提供了实现原理说明

### 9. 配合使用接口
- **要求**：需要配合使用的接口应写在同一个样例中，描述中强调配合使用
- **示例**：
  - ✅ 正确："本样例展示了 SetFlag 和 WaitFlag 的配合使用，用于实现事件同步机制"
  - ❌ 错误：将配合使用的接口分成两个独立样例

### 10. 多场景样例场景说明
- **要求**：当样例支持多种场景（如多种数据类型、多种配置组合）时，应提供场景对照表
- **检查项**：
  - 是否有场景编号（如 scenarioNum）与具体场景的对照表
  - 表格是否清晰说明每种场景的参数配置（数据类型、转置选项等）
  - 场景数量与代码实现是否一致
- **示例**：
  - ✅ 正确：提供表格说明 scenarioNum=1~13 分别对应的数据类型、A/B矩阵转置配置
  - ❌ 错误：只说"支持多种场景"但未列出具体组合

### 11. 专业术语定义
- **要求**：样例中使用专业术语（如 fractalShape、fractalNum、CeilAlign 等）时，应在 README 中给出明确定义
- **检查项**：
  - 术语是否有清晰的数学或逻辑定义
  - 定义是否与代码实现一致
  - 是否提供了计算公式或示例
- **示例**：
  - ✅ 正确："fractalShape: a 矩阵和 b 矩阵的 shape 为 [16, 32/sizeof(T)]，其中 T 表示输入数据类型"
  - ✅ 正确："CeilAlign: 向上对齐操作，例如 CeilAlign(M=30, 16)=32"
  - ❌ 错误：直接使用术语而不解释其含义

### 12. 数据流路径描述
- **要求**：涉及多级存储单元（GM、L1、L0A/L0B、L0C 等）数据搬运的样例，应清晰描述数据流路径
- **检查项**：
  - 是否说明了数据在各存储单元间的搬运路径（如 GM→L1→L0A→L0C→GM）
  - 是否说明了各阶段数据排布格式的变化（如 ND→NZ→ZZ）
  - 是否配合示意图说明
- **示例**：
  - ✅ 正确："数据搬运过程包括：GM-->L1、L1-->L0A/L0B、L0C-->GM，其中 A、B 矩阵在不同存储单元的数据排布格式不同"
  - ❌ 错误：只描述单个 API 调用，未说明整体数据流

### 13. 表格标题规范
- **要求**：README 中的表格应有标题（caption），清晰说明表格内容
- **格式**：
  ```html
  <table border="2" align="center">
  <caption>表1：场景编号与参数对照表</caption>
    <tr>...</tr>
  </table>
  ```
- **示例**：
  - ✅ 正确：表格有 `<caption>表1：scenarioNum 不同取值的含义</caption>`
  - ❌ 错误：表格无标题，读者不清楚表格用途

### 14. 图表资源引用完整性
- **要求**：img 目录下的图片文件应在 README 中被正确引用
- **检查项**：
  - 扫描 img/ 目录下的所有图片文件
  - 验证每张图片是否在 README 中有对应的引用（`<img src="img/...">`）
  - 验证每张引用的图片文件是否存在
- **示例**：
  - ✅ 正确：README 中引用 `<img src="img/A矩阵不转置_F16_GM_L1.png">`，且该文件存在于 img 目录
  - ❌ 错误：img 目录有图片但 README 未引用，或引用了不存在的图片

### 15. 对齐要求说明
- **要求**：涉及硬件对齐要求的样例，应详细说明各存储单元、各轴的对齐要求
- **检查项**：
  - 是否说明了 L1、L0A、L0B、L0C 等存储单元的对齐要求
  - 是否说明了不同数据类型下的对齐差异
  - 是否提供了对齐参数的计算方法
- **示例**：
  - ✅ 正确："在 L1 上 A 矩阵的 K 轴向 fS[0] * fractalNum 对齐、M 轴向 fS[1] 对齐"
  - ❌ 错误：未说明对齐要求，导致用户理解困难

## .asc 文件注释规范

检查 `.asc` 文件是否符合以下注释规范要求：

### 1. 文件头部版权声明
- **要求**：每个 .asc 文件必须包含完整的版权声明
- **格式**：
  ```cpp
  /**
  * Copyright (c) 2025 Huawei Technologies Co., Ltd.
  * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
  * CANN Open Software License Agreement Version 2.0 (the "License").
  * Please refer to the License for details. You may not use this file except in compliance with the License.
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
  * See LICENSE in the root of the software repository for the full text of the License.
  */
  ```

### 2. 文件描述注释
- **要求**：版权声明后必须包含文件描述注释
- **格式**：
  ```cpp
  /* !
   * \file filename.asc
   * \brief 简要描述文件功能
   */
  ```

### 3. 类注释
- **要求**：每个类应有简短的功能说明
- **格式**：
  ```cpp
  /**
   * @brief 类功能简要说明
   */
  class KernelClassName {
  ```

### 4. 主要 API 注释
- **要求**：仅为样例主要介绍的 API 调用添加注释说明
- **示例**：
  ```cpp
  // repeat = 1, 128 elements one repeat, 128 elements total
  // srcBlkStride = 1, no gap between blocks in one repeat
  // dstRepStride = 1, srcRepStride = 8, no gap between repeats
  AscendC::BlockReduceMax<half>(dstLocal, srcLocal, repeat, mask, 1, 1, 8);
  ```
- **注意**：不需要为 CopyIn、CopyOut、main 函数、核函数等添加注释

## 使用方法

### 输入参数

指定要检查的样例目录路径，例如：
- `examples/02_features/16_scalar_computation/cast`
- `examples/01_utilities/00_printf`

### 检查步骤

#### 步骤 1：读取 README.md

从指定目录读取 `README.md` 文件，提取以下信息：

1. **样例功能描述**
   - 从"概述"或"样例功能"部分提取
   - 检查描述是否详细（规范 3）
   - 检查是否使用"样例"而非"算子"（规范 8）

2. **样例规格表格**
   - 输入：name, shape, data type, format
   - 输出：name, shape, data type, format
   - 核函数名
   - 检查 shape 格式是否为 `[xx,xx]`（规范 1、7）

3. **固定 shape 值**
   - 如 M=512, N=1024, K=512

  4. **目录结构中的文件列表**
    - 检查列出的文件是否存在

  5. **术语使用**
     - 检查是否使用了"长度"描述 Tensor 大小（规范 5）
     - 检查模糊概念是否清晰化（规范 4）

  7. **多场景样例检查**（如适用）
     - 检查是否有场景编号对照表（规范 11）
     - 验证场景数量与代码实现一致

  8. **专业术语定义**（如适用）
     - 检查专业术语是否有明确定义（规范 12）
     - 验证定义与代码实现一致

  9. **数据流路径描述**（如适用）
     - 检查是否描述了完整的数据流路径（规范 13）
     - 检查是否说明了数据排布格式变化

  10. **表格规范**
      - 检查所有表格是否有 `<caption>` 标题（规范 14）

  11. **图表资源检查**（如适用）
      - 扫描 img/ 目录下的图片文件
      - 验证图片引用完整性（规范 15）

#### 步骤 3：从代码提取规格

在指定目录中查找：
- `.asc` 文件（主实现文件）
- `CMakeLists.txt`
- `scripts/` 目录下的脚本

#### 步骤 2.5：检查 .asc 文件注释规范

检查 `.asc` 文件是否符合注释规范：

1. **文件头部版权声明**
   - 检查是否包含完整的版权声明块

2. **文件描述注释**
   - 检查是否有 `\file` 和 `\brief` 注释

3. **类注释**
   - 检查每个类是否有 `@brief` 注释

4. **主要 API 注释**
   - 检查样例主要介绍的 API 调用是否有注释说明
   - 不需要检查 CopyIn、CopyOut、main 函数、核函数等注释

#### 步骤 3：从代码提取规格

从 `.asc` 文件中提取：

1. **核函数名**
   ```cpp
   __global__ __vector__ void kernel_cast(GM_ADDR x, GM_ADDR z)
   ```
   提取：`kernel_cast`

2. **输入输出参数**
   - 从核函数签名获取参数数量和名称

3. **Buffer size 和数据类型**
   ```cpp
   size_t inputByteSize = 1024 * sizeof(bfloat16_t);
   size_t outputByteSize = 1024 * sizeof(float);
   ```
   提取：shape=1024, dtype=bfloat16_t (输入), dtype=float (输出)

4. **使用的 API**
   - 如 `Cast`, `Duplicate`, `Matmul`, `DataCopy`, `printf`
   - 检查配合使用的 API 是否在同一个样例中（规范 10）

5. **固定 shape 值**
   ```cpp
   int M = 512;
   int N = 1024;
   int K = 512;
   ```

#### 步骤 4：对比验证

对比 README 和代码提取的信息：

| 检查项 | README | 代码 | 结果 |
|--------|--------|------|------|
| 核函数名 | kernel_cast | kernel_cast | ✅ |
| 输入 shape | [1024] | 1024 | ✅ |
| 输入 dtype | bfloat16_t | bfloat16_t | ✅ |
| 输出 shape | [1024] | 1024 | ✅ |
| 输出 dtype | float | float | ✅ |
| API 调用 | Cast, Duplicate | Cast, Duplicate | ✅ |

#### 步骤 5：输出结果

- ✅ **通过**：README 与代码完全一致，符合所有规范，.asc 文件注释规范符合要求
- ⚠️ **警告**：存在轻微不一致（如描述不够详细、部分注释缺失）
- ❌ **错误**：存在严重不一致（如规格不匹配、文件不存在、违反规范、缺少关键注释）

## 检查示例

### 示例：cast 样例

**目录：** `examples/02_features/16_scalar_computation/cast`

**README 提取：**
- 核函数名：kernel_cast
- 输入：x, shape=[1024], dtype=bfloat16_t, format=ND
- 输出：z, shape=[1024], dtype=float, format=ND
- 功能：将输入 bfloat16 类型 Tensor 的第 0 个元素转换为 float 类型，并 Duplicate 成 Shape 为 1024 的 Tensor

**代码提取（cast.asc）：**
```cpp
__global__ __vector__ void kernel_cast(GM_ADDR x, GM_ADDR z)  // 核函数名：kernel_cast
{
    uint32_t calCount = 1024;  // shape：1024
    KernelCast<bfloat16_t> op;  // 输入类型：bfloat16_t
    // ...
}

int32_t main(int32_t argc, char *argv[]) {
    size_t inputByteSize = 1024 * sizeof(bfloat16_t);  // 输入：1024, bfloat16_t
    size_t outputByteSize = 1024 * sizeof(float);      // 输出：1024, float
    // ...
}
```

**对比结果：**
- 核函数名：✅ 匹配
- 输入 shape：✅ 匹配 (1024)
- 输入 dtype：✅ 匹配 (bfloat16_t)
- 输出 shape：✅ 匹配 (1024)
- 输出 dtype：✅ 匹配 (float)
- API 调用：✅ 匹配 (Cast, Duplicate)

**注释规范检查：**
- ✅ 文件头部版权声明完整
- ✅ 文件描述注释存在
- ✅ 类注释完整
- ✅ 主要 API 注释充分

**最终结果：** ✅ 通过

### 示例：mmad 多场景样例

**目录：** `examples/03_libraries/01_matrix/mmad_s8_f16_f32_with_A_B_transpose_option`

**README 提取：**
- 场景数量：13 种（3 种数据类型 × 4 种转置组合 + 1 个特殊场景）
- 场景对照表：✅ 有（表1：scenarioNum 不同取值的含义）
- 数据流路径：GM→L1→L0A/L0B→L0C→GM
- 专业术语定义：fractalShape、fractalNum、CeilAlign 等
- 图表资源：21 张示意图

**代码提取（mmad_s8_f16_f32_with_A_B_transpose_option.asc）：**
```cpp
// 场景定义：13 种场景通过 scenarioNum 区分
if constexpr (scenarioNum == 1) {
    KernelMmad<int8_t, int32_t, false, false> op;  // S8, A不转置, B不转置
} else if constexpr (scenarioNum == 2) {
    KernelMmad<int8_t, int32_t, false, true> op;   // S8, A不转置, B转置
}
// ... 13 个分支
```

**多场景规范检查：**
- ✅ 场景对照表完整（表1 包含 13 种场景的参数配置）
- ✅ 场景数量与代码一致（13 个 if-else 分支）
- ✅ 专业术语有定义（fractalShape、fractalNum、CeilAlign 等）
- ✅ 数据流路径清晰（GM→L1→L0A/L0B→L0C→GM）
- ✅ 各阶段数据排布变化有说明（ND→NZ→ZZ→ZN）

**图表资源检查：**
- ✅ img/ 目录 21 张图片全部在 README 中被引用
- ✅ 每张图片都有对应的图号和说明

**表格规范检查：**
- ✅ 所有 6 个表格都有 `<caption>` 标题
- ✅ 表格内容与代码实现一致

**注释规范检查：**
- ✅ 文件头部版权声明完整
- ✅ 场景说明注释清晰（A/B 矩阵转置含义）
- ✅ 模板参数注释充分（isL1NZ 含义说明）
- ✅ 成员变量初始化注释详细

**最终结果：** ✅ 通过

## 常见问题类型

### 规范违反问题

1. **Shape 格式错误**
   - README 使用 `1024*1024` 而非 `[1024, 1024]`

2. **概述过于简单**
   - 概述只有一句话，未详细说明功能和使用场景

3. **使用"算子"而非"样例"**
   - 文档中频繁使用"算子"概念

4. **使用"长度"描述 Tensor**
   - 应使用"数据量"或"元素个数"

5. **多场景样例缺少场景对照表**
   - 支持 scenarioNum 等多场景的样例未提供场景参数对照表
   - 场景数量与代码中的 if-else 分支不一致

6. **专业术语未定义**
   - 使用 fractalShape、fractalNum、CeilAlign 等术语但未给出定义
   - 定义与代码实现不一致

7. **数据流路径描述不完整**
   - 只描述单个 API，未说明 GM→L1→L0→GM 的完整路径
   - 未说明各阶段数据排布格式变化

8. **表格缺少标题**
   - README 中的表格没有 `<caption>` 标签
   - 无法清晰理解表格用途

9. **图表资源未引用或引用无效**
   - img/ 目录存在图片但 README 未引用
   - README 引用了不存在的图片文件

10. **对齐要求说明缺失**
   - 涉及硬件对齐的样例未说明对齐要求
   - 不同数据类型的对齐差异未说明

### .asc 文件注释规范问题

### .asc 文件注释规范问题

1. **缺少版权声明**
   - .asc 文件头部缺少完整的版权声明

2. **缺少文件描述注释**
   - 缺少 `\file` 和 `\brief` 注释

3. **缺少类注释**
   - 类定义前缺少 `@brief` 注释

4. **缺少主要 API 注释**
   - 样例主要介绍的 API 调用缺少注释说明

5. **缺少成员变量注释**
   - 重要的私有成员变量缺少用途说明注释

### 代码一致性问题

1. **样例规格不匹配**
   - README 描述 shape=[1024]，代码中是 512
   - README 描述 dtype=float16，代码中是 float

2. **核函数名不一致**
   - README 描述核函数名为 `kernel_cast`，代码中是 `cast_kernel`

3. **文件路径错误**
   - README 描述文件在 `scripts/gen_data.py`，实际在 `scripts/generate_data.py`

4. **功能描述不准确**
   - README 描述使用 Cast API，代码中实际使用的是其他 API

5. **文件不存在**
   - README 中列出的文件在目录中找不到

## 注意事项

1. **类型映射**
   - README 中的 `float16` 可能对应代码中的 `half`
   - 需要建立类型映射关系

2. **多文件样例**
   - 某些样例可能有多个 .asc 文件
   - 优先检查包含 `__global__` 的主文件

3. **模板类**
   - 代码可能使用模板类，需要从模板参数提取类型

4. **部分匹配**
   - README 可能只描述主要功能，不必与代码逐行对应

5. **规范优先级**
    - 代码一致性检查、规范描述检查和注释规范检查同等重要
    - 三者都通过才算检查通过

6. **注释检查范围**
     - 检查所有 .asc 文件的注释完整性
     - 包括版权声明、文件描述、类/函数/变量注释
     - 行内注释关注关键代码行

7. **多场景样例检查**
     - 识别场景控制变量（如 scenarioNum、模板参数组合）
     - 验证 README 中场景描述与代码分支一致
     - 检查场景对照表是否覆盖所有组合

8. **图表资源检查**
     - 扫描 img/ 目录获取图片列表
     - 使用正则匹配 README 中的 `<img src="img/...">` 引用
     - 比对图片文件与引用的一致性

9. **专业术语检查**
     - 识别 README 中的大驼峰/下划线命名的专业术语
     - 检查是否有"定义"、"含义"等解释性描述
     - 验证术语定义与代码变量名/注释一致

10. **数据流检查**
    - 识别涉及 GM、L1、L0A、L0B、L0C、UB 等存储单元的样例
    - 检查是否描述了完整的搬运路径
    - 检查是否说明了各阶段的排布格式变化（ND/NZ/ZZ/ZN 等）
