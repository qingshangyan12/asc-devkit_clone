---
name: capi-doc-generator
description: CAPI 接口资料文档生成技能。当用户需要为 CAPI 接口编写 Markdown 文档时使用。CAPI 是基于底层硬件指令的 C 风格 API，接口位于 include/c_api/reg_compute/，实现位于 impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/。文档编写时内容必须严格参考代码实现，可参考对应的 RegAPI 文档但需要到 CAPI 代码仓验证。
---

# CAPI 接口资料文档生成技能

## 1. 概述

本技能帮助开发者编写 CAPI（C 风格 API）接口的 Markdown 资料文档。CAPI 是 AscendC 底层硬件指令封装，与 RegAPI（基础API）相比：
- 无模板参数，所有数据类型通过重载的独立函数声明体现
- 参数类型使用 `vector_*`（如 `vector_half`），mask 使用 `vector_bool`
- 函数名使用 `asc_` 前缀（如 `asc_abs`）
- 每个数据类型组合对应一个独立的函数声明

### 1.1 代码仓路径

| 类别 | 路径 |
|-----|------|
| CAPI 头文件（向量计算） | `include/c_api/reg_compute/reg_vector.h` |
| CAPI 头文件（类型转换） | `include/c_api/reg_compute/reg_convert.h` |
| CAPI 头文件（数据搬入） | `include/c_api/reg_compute/reg_load.h` |
| CAPI 头文件（数据搬出） | `include/c_api/reg_compute/reg_store.h` |
| CAPI 实现（3510架构） | `impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/` |
| RegAPI 参考文档 | `docs/api/SIMD-API/基础API/Reg矢量计算/` |

### 1.2 前置知识

编写 CAPI 文档前，必须学习以下参考文档以理解编程模型和硬件背景：

| 文档 | 路径 | 用途 |
|-----|------|------|
| Reg矢量计算编程指南 | `docs/guide/编程指南/编程模型/AI-Core-SIMD编程/基于指针的C语言编程/Reg矢量计算编程.md` | 理解 VL=256B、寄存器类型、mask 位宽映射、iter_reg 寻址公式、VF 函数模型 |

关键概念速查：
- **VL = 256B**：矢量数据寄存器宽度。b8 类型存储 256 个元素，b16=128，b32=64，b64=32。
- **vector_bool 宽度 = VL/8 = 32B = 256bit**：b8 模式下每 bit 控制 1 个元素；b16 模式下每 2bit 控制 1 个元素（仅 LSB 有效）；b32 模式下每 4bit 控制 1 个元素（仅 LSB 有效）。
- **iter_reg 寻址公式**：offset = Σ(index_i × stride_i)，最多四维。
- **所有 CAPI 使用 `__simd_callee__` 修饰**，这是 Reg 矢量 API 的统一标识，文档中不需额外说明调用域约束。

### 1.2 文档输出位置

CAPI 文档输出到 `docs/` 目录下对应的 CAPI 子目录中。

---

## 2. 文档章节结构

CAPI 接口文档按以下固定顺序组织章节，不使用 HTML 锚点（`<a name="...">`）：

1. **产品支持情况** — 按固定模板编写
2. **功能说明** — 描述接口功能，计算类接口需包含 LaTeX 公式
3. **函数原型** — 列出所有重载的函数声明
4. **参数说明** — 仅一个参数表格（无模板参数说明）
5. **数据类型** — 根据源/目的类型是否一致选择格式
6. **返回值说明** — 固定填"无"
7. **约束说明** — 参考对应 RegAPI 的约束说明，在 CAPI 代码仓验证后填写
8. **关键特性说明** — 可选，仅当对应 RegAPI 有此节时才添加
9. **调用示例** — 固定先填"无"，后续补充

---

## 3. 各章节编写规范

### 3.1 产品支持情况

严格按以下固定模板编写，不可修改产品名称和排列顺序：

```markdown
## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->
```

> 仅 Ascend 950 系列支持 CAPI。所有注释标签 `<!-- npu="..." idN -->` 和 `<!-- end idN -->` 必须保留。

### 3.2 功能说明

**头文件路径**：从 CAPI 头文件中确定正确的包含路径。路径格式为 `"c_api/reg_compute/xxx.h"`。

**功能描述必须从实现代码中确认**：阅读 `impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/` 下的实现文件，查看底层调用的硬件指令，理解接口的实际行为。

格式模板：

```markdown
## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

描述接口功能的详细说明。对于有多种变体的接口，分点说明每个变体的用途和差异。如果底层调用了特定的硬件指令（如 vmov、vdup、vsqz、vxor 等），应在描述中说明。

- 变体描述1

    具体描述内容，包括底层指令和行为细节。

- 变体描述2

    具体描述内容。

    $$公式$$
```

**规则**：
- 头文件路径写在第一行，使用反引号包裹
- 功能描述必须结合实现代码中的硬件指令来解释接口行为
- 可参考对应 RegAPI 文档的功能说明和公式，但必须到 CAPI 实现代码中验证一致性
- 计算类接口必须在功能说明中给出 LaTeX 公式，使用 `$$...$$` 格式
- 多种变体时用无序列表（`-`）区分，每个变体独立描述
- 列表项下缩进4空格写内容
- 如果有 mask 参数，必须在功能说明中描述 mask=0 时的行为（置为0还是保持原值）

### 3.3 函数原型

列出 CAPI 头文件中该接口的所有重载声明。每个重载变体用无序列表项表示，代码用 `cpp` 代码块包裹。

**格式**：当有多个函数原型变体时，用无序列表区分；只有一个时直接用代码块。

```markdown
## 函数原型

- 变体描述1

    ```cpp
    __simd_callee__ inline void asc_xxx(vector_type& dst, vector_type src, vector_bool mask)
    ```

- 变体描述2

    ```cpp
    __simd_callee__ inline void asc_xxx(vector_type& dst, vector_type src0, vector_type src1, vector_bool mask)
    ```
```

**规则**：
- 函数原型必须从 `include/c_api/reg_compute/` 下的头文件中精确复制，不能自行修改
- 多个重载用无序列表组织，列表项标题简要说明该变体的用途
- 每个代码块用 ````cpp` 包裹
- 代码块相对列表项缩进4空格

### 3.4 参数说明

CAPI 没有模板参数，因此**只有一个参数说明表格**（无"模板参数说明"表格）。表格前必须加表头 `**表1**  参数说明`。

表格包含三列：参数名、输入/输出、描述。

```markdown
## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg数据类型定义.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| src | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg数据类型定义.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| mask | 输入 | 源操作数元素操作的有效指示（掩码寄存器）。<br>类型为[vector_bool](../reg数据类型定义.md#掩码寄存器)。 |
```

**规则**：
- **参数描述必须从实现代码中获取**：查看 `impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/` 下的实现文件，理解每个参数的具体含义、取值范围和使用方式。不能只写"类型为xxx"，必须说明参数的业务含义。
  - 例如 `asc_copy` 的 `part` 参数：需从 `asc_copy_impl.h` 中看到它传给 `movvp` 指令，含义是"取 src 前 part 个元素的低 1bit 复制到 dst 掩码寄存器"，需说明 part 的单位和对齐要求。
  - 例如 `asc_update_mask` 的 `value` 参数：需从 `asc_update_mask_impl.h` 中看到 `plt_b8(value, POST_UPDATE)`，说明 value 是引用，调用后自动递减。
- 使用简易表格（Markdown 原生表格，非 HTML）
- 参数顺序与函数签名一致
- `dst` 总是输出，`src` 总是输入
- 矢量数据寄存器类型使用 `[vector_[data_type]](../reg数据类型定义.md#矢量数据寄存器)` 链接，data_type的取值范围通过 `[数据类型](#数据类型)` 交叉引用。如果该参数的具体类型确定（如 `vector_half`），则直接写出具体类型链
- 掩码寄存器类型使用 `[vector_bool](../reg数据类型定义.md#掩码寄存器)` 链接，并在描述中标注"（掩码寄存器）"
- 如果 dst 类型为 vector_bool，则写"目的操作数（掩码寄存器）"
- 标量参数直接描述数据类型
- 多行描述用 `<br>` 分隔
- 表格内如需无序列表，使用 `&bull;` 替代 `-`
- 表格内如需多级无序列表，用 `&nbsp;` + `&bull;` 表示第二级

### 3.5 数据类型

根据源操作数和目的操作数数据类型是否一致，选择不同格式：

#### 格式A：源和目的数据类型完全一致（如 Add、Adds）

不需要表格，直接用文字描述：

```markdown
## 数据类型

目的操作数与源操作数的数据类型需要保持一致。支持的数据类型为：int8_t、int16_t、half、int32_t、float。
```

或当有多个变体时：

```markdown
## 数据类型

目的操作数与源操作数的数据类型需要保持一致。
- 当xxx时，支持的数据类型为：int32_t、uint32_t。
- 当xxx时，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。
```

#### 格式B：源和目的数据类型不完全一致（如类型转换接口）

需要用表格列出数据类型组合，列名用 `src` 和 `dst`。表格前加 `**表2**  数据类型`，不允许同时写文字描述：

```markdown
## 数据类型

**表2**  数据类型

| src | dst |
| --- | --- |
| int16_t | float |
```

**数据类型书写顺序**：所有数据类型按位宽从小到大排列，同一位宽内按以下固定顺序：
bool、int4b_t、int4x2_t、int8_t、uint8_t、fp4x2_e2m1_t、fp4x2_e1m2_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、complex32、int64_t、uint64_t、double、complex64。

此顺序适用于：
- `## 数据类型` 章节中的文字描述和表格行。
- `## 函数原型` 章节中函数声明的排列顺序（位宽小的重载在前）。

**规则**：
- 数据类型必须从 CAPI 头文件中的函数重载声明中提取
- 不可参考 RegAPI 文档的数据类型表（CAPI 可能不支持某些类型组合）
- 表格列的顺序：源操作数在左，目的操作数在右
- 使用简易表格
- **格式A和格式B不能同时出现**：如果源和目的数据类型不一致，只使用表格；如果一致，只使用文字描述

### 3.6 返回值说明

固定填写：

```markdown
## 返回值说明

无
```

### 3.7 约束说明

**步骤**：
1. 找到对应的 RegAPI 文档（`docs/api/SIMD-API/基础API/Reg矢量计算/` 下的同名 `.md` 文件）
2. 查看其"约束说明"章节内容
3. 到 CAPI 代码实现中验证约束是否适用
4. 将适用的约束内容写入 CAPI 文档

**格式**：使用无序列表。

```markdown
## 约束说明

- 约束条件1。
- 约束条件2。
```

如果 RegAPI 的约束说明为"无"，则 CAPI 也填"无"。

**不需要写的约束**：
- `__simd_callee__` 只能在 `__simd_vf__` 中调用 —— 这是所有 CAPI 的统一特征，无需在每个文档中重复说明。
- AIV 模式约束 —— 仅当接口有额外硬件模式限制时才需要说明。

**必须写的约束**：
- 所有带 mask 参数的接口，必须在约束说明中明确 mask=0 时的行为：
  - ZEROING 模式：`dst中未被mask筛选的位置被置为0。`
  - MERGING 模式：`dst中未被mask筛选的位置保持原值。`

### 3.8 关键特性说明

**规则**：
- 仅当对应的 RegAPI 文档中存在"关键特性说明"章节时，CAPI 文档才需要添加此章节
- 内容参考 RegAPI 的关键特性说明，但需在 CAPI 代码仓验证
- 如果 RegAPI 文档没有此章节，则 CAPI 文档也省略此章节

### 3.9 调用示例

固定先填写：

```markdown
## 调用示例

无
```

后续再补充具体调用示例。

---

## 4. 通用格式规范

### 4.1 代码格式

- C/C++ 代码块使用 ````cpp` 包裹，不使用 ` ```c ` 或其他语言标识。
- Python 伪代码使用 ````python` 包裹。
- 功能说明中如需要伪代码辅助描述接口行为，使用 ````python` 标记。

### 4.2 数学公式

- 计算类接口的功能说明中必须包含 LaTeX 公式
- 使用 `$$...$$` 格式（块级公式，独占一行）

### 4.3 标点符号

- 中文描述使用中文标点（`，`、`。`、`：`、`、`），不使用英文标点
- 中文和英文之间不加空格
- 中文和数字之间不加空格
- 英文和数字之间不加空格

### 4.4 表格

- 所有表格使用简易表格（Markdown 原生表格），不使用 HTML 表格
- 表格内如需无序列表，用 `&bull;` 替代 `-`
- 表格内如需多级无序列表，用 `&nbsp;` + `&bull;` 表示第二级

### 4.5 无序列表

- 使用 `-` 作为无序列表标记
- 列表项下的子内容缩进4空格
- 代码块在列表项下需额外缩进4空格

### 4.6 引用链接

- 不使用 HTML 锚点（`<a name="...">`）
- 如需交叉引用，使用 Markdown 链接格式，如 `[数据类型](#数据类型)`
- 引用其他文档时使用相对路径，如 `[MaskMergeMode](../辅助数据类型/MaskMergeMode.md)`

---

## 5. 文档生成流程

### 5.1 确定目标 API

用户指定需要编写文档的 CAPI 接口名称（如 `asc_abs`、`asc_add`）。

### 5.2 收集源码信息

1. 在 `include/c_api/reg_compute/reg_vector.h`（或其他对应的头文件）中搜索该 API 的所有函数声明
2. 记录所有重载的：
   - 完整函数签名
   - 参数类型（`vector_*` 类型）
   - 参数个数和顺序
3. 在 `impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/` 中找到对应的实现文件，查看实现逻辑

### 5.3 参考 RegAPI 文档

1. 在 `docs/api/SIMD-API/基础API/Reg矢量计算/` 下找到同名 RegAPI 文档
2. 提取以下信息作为参考（需在 CAPI 代码仓验证）：
   - 功能说明中的公式和描述逻辑
   - 约束说明
   - 关键特性说明（如果有）

### 5.4 验证与编写

**关键原则：所有内容必须经过 CAPI 代码仓验证，不可直接照搬 RegAPI 文档。**

- **数据类型**：从 CAPI 头文件的函数重载声明中提取，不可使用 RegAPI 文档的数据类型
- **功能说明**：阅读实现文件中的底层硬件指令，理解实际行为后描述。参考 RegAPI 的公式和描述逻辑，但必须确认 CAPI 是否支持相同的计算逻辑
- **参数含义**：从实现文件中理解每个参数传递给底层指令的方式，确定其业务含义、取值范围和约束
- **约束说明**：逐条在 CAPI 实现中验证。RegAPI 约束中涉及模板参数的部分需要调整为 CAPI 的参数名
- **函数签名**：从 CAPI 头文件精确复制

### 5.5 批量验证

生成文档后，需对每个 API 执行以下验证：
1. 确认头文件中存在对应的函数声明（`include/c_api/reg_compute/`）
2. 确认实现文件中存在对应的实现（`impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/`）
   - 宏定义的接口（如 `asc_create_mask` 映射到 `pset_b8`）无独立实现文件
3. 确认函数原型与头文件完全一致
4. 确认数据类型表与实际重载声明匹配

---

## 6. CAPI 与 RegAPI 的关键差异总结

| 差异项 | RegAPI（基础API） | CAPI |
|-------|------------------|------|
| 参数说明 | 有"模板参数说明"和"参数说明"两个表格 | 仅有"参数说明"一个表格 |
| 数据类型 | 使用 `RegTensor<T>` 泛型，支持模板推导 | 每个类型组合有独立的函数重载 |
| 类型名称 | `RegTensor<half>`、`MaskReg` | `vector_half`、`vector_bool` |
| 函数前缀 | 无前缀（如 `Abs`） | `asc_` 前缀（如 `asc_abs`） |
| 头文件路径 | `"basic_api/reg_compute/kernel_reg_compute_xxx.h"` | `"c_api/reg_compute/reg_vector.h"` 等 |
| 命名空间 | `AscendC::Reg::` | 无命名空间（全局函数） |
| 调用示例 | 包含完整的加载-计算-存储流程 | 暂填"无"，后续补充 |
| 关键特性说明 | 部分接口有 | 仅当 RegAPI 有时才添加 |

---

## 7. 示例：asc_copy 完整文档

以下是以 `asc_copy` 为例的完整 CAPI 文档，展示参数描述应达到的详细程度：

```markdown
# asc_copy

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

完成数据在寄存器内的搬运，支持以下三种模式：

- 矢量寄存器间复制：将src中被mask筛选的有效元素复制到dst对应位置，未被mask筛选的位置保持不变。底层调用vmov指令（MERGING模式）。dst和src的数据类型必须一致。

- Mask寄存器间复制：底层调用pmov指令。支持带mask和不带mask两种重载：
  - 带mask：将src中被mask对应位置为1的bit复制到dst。
  - 不带mask：将src的所有bit复制到dst。

- Mask寄存器部分复制：底层调用movvp指令。将src矢量数据寄存器中的低part个元素，按照每个元素的低bit位复制到dst掩码寄存器中。例如当src为vector_uint16_t时，part=4表示取src的前4个uint16_t元素，每个元素的低1bit复制到dst中，共复制4bit到dst。

## 函数原型

- 矢量寄存器间复制

    ```cpp
    __simd_callee__ inline void asc_copy(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_half& dst, vector_half src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_float& dst, vector_float src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
    ```

- Mask寄存器间复制

    ```cpp
    __simd_callee__ inline void asc_copy(vector_bool& dst, vector_bool src, vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_bool& dst, vector_bool src)
    ```

- Mask寄存器部分复制

    ```cpp
    __simd_callee__ inline void asc_copy(vector_bool& dst, vector_uint16_t src, int16_t part)
    __simd_callee__ inline void asc_copy(vector_bool& dst, vector_uint32_t src, int16_t part)
    ```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg数据类型定义.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| src | 输入 | 源操作数（矢量数据寄存器）。<br>类型为[vector_[data_type]](../reg数据类型定义.md#矢量数据寄存器)，data_type的取值范围请参考[数据类型](#数据类型)。 |
| mask | 输入 | 源操作数元素操作的有效指示（掩码寄存器）。<br>类型为[vector_bool](../reg数据类型定义.md#掩码寄存器)。对应位置为1的bit参与操作，为0的不参与。<br>Mask寄存器间复制的部分重载中不存在此参数。 |
| part | 输入 | 指定复制src的低part个元素到dst掩码寄存器中。<br>类型为int16_t。<br>&bull; 对于vector_uint16_t的src，每个元素的低1bit复制到dst中。part的值不应超过dst掩码寄存器能容纳的位数。<br>&bull; 对于vector_uint32_t的src，每个元素的低1bit复制到dst中。<br>例如：part=4时，取src的前4个元素，将每个元素的低1bit依次复制到dst的第0~3bit位置。 |

## 数据类型

目的操作数与源操作数的数据类型需要保持一致。
- 矢量寄存器间复制支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float、bfloat16_t。
- Mask寄存器间复制支持的数据类型为：bool。
- Mask寄存器部分复制支持src的数据类型为：uint16_t、uint32_t。

## 返回值说明

无

## 约束说明

- dst中未被mask筛选的位置保持原值。
- 针对于矢量寄存器复制到掩码寄存器：当数据类型为uint16_t，part∈[0, 15]，当数据类型为uint32_t，part∈[0, 31]。

## 调用示例

无
```

---

## 8. 注意事项

1. **严禁猜测**：所有函数签名、数据类型、功能描述、参数含义必须从代码实现中获取。尤其参数描述不能只写"类型为xxx"，必须从实现文件中理解参数传递给底层指令的方式和业务含义。
2. **先读实现代码再写文档**：查看 `impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl/` 下的实现文件，理解底层硬件指令（vmov/vdup/vbr/vsqz/vusqz/pxor/movvp 等）的行为，才能写出准确的参数说明。
3. **CAPI 可能不支持 RegAPI 的全部功能**：例如 RegAPI 的 `Abs` 支持复数类型，但 CAPI 的 `asc_abs` 可能不支持，必须从代码验证。
4. **函数重载的展示方式**：如果所有重载仅数据类型不同且功能一致，可将它们放在同一个代码块中；如果有功能差异的变体，用无序列表区分并配独立代码块。
5. **表头格式**：参数说明表格必须加 `**表1**  参数说明`，数据类型表格加 `**表2**  数据类型`，返回值说明有表格时加对应编号表头。表头前留一空行。
6. **头文件路径**：向量计算类 API 头文件为 `"c_api/reg_compute/reg_vector.h"`，类型转换类为 `"c_api/reg_compute/reg_convert.h"`，数据搬入为 `"c_api/reg_compute/reg_load.h"`，数据搬出为 `"c_api/reg_compute/reg_store.h"`。
7. **不需要写的约束**：`__simd_callee__` 只能在 `__simd_vf__` 中调用 —— 这是所有 CAPI 的统一特征，无需在每个文档中重复。
8. **批量验证**：生成后确认每个 API 在头文件和实现文件中都存在，宏定义接口（如 `asc_create_mask`）无独立实现文件属正常。
9. **mask 约束必写**：所有带 mask 参数的接口，必须在约束说明中明确 mask=0 时 dst 的行为是置为0还是保持原值。
10. **数据类型格式互斥**：源/目的类型一致时只用文字描述，不一致时只用表格，不允许两者并存。
