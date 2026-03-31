---
name: example-scenario-merge
description: 样例多场景合并技能。用于将功能相同但场景不同的多个样例合并为一个多场景样例。
---

# 样例合并技能

## 技能描述

本技能用于将功能相同但场景不同的多个样例目录合并为一个统一的多场景样例目录，通过场景参数区分不同实现方式。

## 触发时机

当用户需要合并功能相似、场景不同的样例时使用本技能：
- 多个样例实现相同的核心功能，但配置或实现方式不同
- 例如：transpose_common 和 transpose_enhanced 可以合并为 transpose，通过场景参数区分
- 例如：add_basic 和 add_optimized 可以合并为 add，通过场景参数区分
- 减少样例数量，提高代码复用性和可维护性

## 合并原则

1. **功能一致性**：合并的样例必须实现相同的核心功能
2. **场景区分**：通过场景参数（如 scenarioNum、模板参数、运行参数）区分不同实现
3. **命名规范**：合并后的目录名使用核心功能名称，去掉场景修饰词
4. **文档更新**：README 中需要提供场景对照表，说明各场景的参数配置
5. **代码整合**：将多个 .asc 文件整合为一个，使用条件编译或模板参数区分场景

## 使用方法

### 输入参数

指定要合并的样例目录列表，例如：
- `examples/02_features/xx/transpose_common`
- `examples/02_features/xx/transpose_enhanced`

合并后：
- `examples/02_features/xx/transpose`

### 合并步骤

#### 步骤 1：分析待合并样例

对每个待合并的样例进行分析：

1. **读取 README.md**
   - 提取样例功能描述
   - 提取输入输出规格（shape、dtype、format）
   - 提取核函数名
   - 提取使用的 API
   - 提取固定参数值

2. **读取 .asc 文件**
   - 提取核函数实现
   - 提取数据类型和配置参数
   - 提取关键 API 调用
   - 提取特殊处理逻辑

3. **对比分析**
   - 验证核心功能是否一致
   - 识别差异点（数据类型、转置选项、优化方式等）
   - 确定场景区分方式

#### 步骤 2：确定场景区分策略

根据差异点选择场景区分方式：

1. **模板参数方式**（推荐）
   ```cpp
   template<typename T, bool useOptimization>
   __global__ __vector__ void kernel_transpose(GM_ADDR x, GM_ADDR z)
   ```

2. **场景参数方式**
   ```cpp
   __global__ __vector__ void kernel_transpose(GM_ADDR x, GM_ADDR z, int32_t scenarioNum)
   {
       if (scenarioNum == 1) {
           // 场景1：普通转置
       } else if (scenarioNum == 2) {
           // 场景2：增强转置
       }
   }
   ```

3. **运行参数方式**
   - 通过命令行参数传递场景信息
   - 在 main 函数中根据参数选择不同实现

#### 步骤 3：创建合并后的目录结构

```
transpose/
├── README.md
├── transpose.asc
├── CMakeLists.txt
├── data_utils.h
├── scripts/
│   ├── gen_data.py
│   └── run.sh
```

#### 步骤 4：合并 README.md

1. **概述部分**
   - 描述核心功能
   - 说明支持多种场景
   - 列出主要差异点

2. **规格表格**
   - 统一输入输出规格
   - 如果不同场景规格不同，在表格中说明

3. **场景对照表**
   ```html
   <table border="2" align="center">
   <caption>表1：场景编号与参数对照表</caption>
   <tr>
     <th>场景编号</th>
     <th>场景名称</th>
     <th>数据类型</th>
     <th>转置选项</th>
     <th>优化方式</th>
   </tr>
   <tr>
     <td>1</td>
     <td>普通转置</td>
     <td>float16</td>
     <td>不转置</td>
     <td>基础实现</td>
   </tr>
   <tr>
     <td>2</td>
     <td>增强转置</td>
     <td>float16</td>
     <td>转置</td>
     <td>优化实现</td>
   </tr>
   </table>
   ```

4. **功能说明**
   - 合并各样例的功能说明
   - 强调场景差异点

5. **图表资源**
   - 合并并重新编号图片资源
   - 更新图片引用

#### 步骤 5：合并 .asc 文件

1. **文件头部**
   - 保留版权声明
   - 更新文件描述

2. **核函数整合**
   - 使用模板参数或场景参数区分场景
   - 提取公共逻辑，避免代码重复

3. **main 函数**
   - 添加场景参数解析
   - 根据场景参数调用不同实现

4. **注释更新**
   - 添加场景说明注释
   - 标注场景差异点

#### 步骤 6：合并脚本文件

1. **gen_data.py**
   - 支持生成不同场景的测试数据
   - 添加场景参数

2. **run.sh**
   - 支持运行不同场景
   - 添加场景参数传递

#### 步骤 7：验证合并结果

1. **编译测试**
   - 确保合并后的代码可以编译
   - 测试所有场景

2. **文档验证**
   - 使用 ascendc-examples-validator 验证 README 规范
   - 验证场景对照表完整性

3. **功能验证**
   - 确保各场景功能与原样例一致
   - 验证输出结果正确性

## 合并示例

### 示例：transpose_common 和 transpose_enhanced 合并为 transpose

**待合并样例：**
- `examples/02_features/xx/transpose_common`（普通转置）
- `examples/02_features/xx/transpose_enhanced`（增强转置）

**合并后：**
- `examples/02_features/xx/transpose`

**分析结果：**
- 核心功能：都是矩阵转置
- 差异点：
  - transpose_common：基础实现，不支持优化
  - transpose_enhanced：优化实现，支持双缓冲和流水线

**场景区分策略：**
使用场景参数方式，scenarioNum=1 表示普通转置，scenarioNum=2 表示增强转置

**合并后的 transpose.asc：**
```cpp
/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * ...
 */

/*!
 * \file transpose.asc
 * \brief 矩阵转置样例，支持普通转置和增强转置两种场景
 */

template<typename T>
__global__ __vector__ void kernel_transpose_common(GM_ADDR x, GM_ADDR z, int32_t M, int32_t N)
{
    // 普通转置实现
}

template<typename T>
__global__ __vector__ void kernel_transpose_enhanced(GM_ADDR x, GM_ADDR z,  int32_t M, int32_t N)
{
    // 增强转置实现（双缓冲 + 流水线）
}

int32_t main(int32_t argc, char *argv[])
{
    // 解析场景参数
    int32_t scenarioNum = 1;
    if (argc > 1) {
        scenarioNum = atoi(argv[1]);
    }

    // 根据场景调用不同核函数
    if (scenarioNum == 1) {
        kernel_transpose_common<half>(...);
    } else if (scenarioNum == 2) {
        kernel_transpose_enhanced<half>(...);
    }
}
```

**合并后的 README.md 场景对照表：**
```html
<table border="2" align="center">
<caption>表1：场景编号与实现方式对照表</caption>
<tr>
  <th>场景编号</th>
  <th>场景名称</th>
  <th>实现方式</th>
  <th>优化特性</th>
</tr>
<tr>
  <td>1</td>
  <td>普通转置</td>
  <td>基础实现</td>
  <td>无优化</td>
</tr>
<tr>
  <td>2</td>
  <td>增强转置</td>
  <td>优化实现</td>
  <td>双缓冲、流水线</td>
</tr>
</table>
```

## 注意事项

1. **命名规范**
   - 合并后的目录名使用核心功能名称，按照用户输入的名称即可
   - 去掉场景修饰词（common、enhanced、basic、optimized 等）
   - 例如：transpose_common + transpose_enhanced → transpose

2. **场景编号**
   - 从 1 开始编号
   - 按照从简单到复杂的顺序排列
   - 在 README 中提供清晰的场景对照表

3. **代码复用**
   - 提取公共逻辑，避免代码重复
   - 使用模板或宏减少重复代码
   - 保持代码可读性

4. **文档完整性**
   - 更新 README 中的所有描述
   - 确保场景对照表清晰准确
   - 更新图片资源引用

5. **向后兼容**
   - 如果原样例被其他样例引用，需要更新引用
   - 考虑保留原样例的兼容性接口

6. **测试验证**
   - 测试所有场景的功能正确性
   - 验证性能差异（如果有优化场景）
   - 确保编译无错误和警告

7. **版本控制**
   - 合并前建议创建分支
   - 提交时明确说明合并内容
   - 保留原样例的历史提交记录
