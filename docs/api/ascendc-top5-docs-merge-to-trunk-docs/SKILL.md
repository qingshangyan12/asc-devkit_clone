---
name: ascendc-top5-docs-merge-to-trunk-docs
description: 将 TOP5（产品化文档）与 Trunk（主线文档）两个来源的 API 文档合并为一份统一的 `xxx-new.md`，以 Trunk 为骨架，融合 TOP5 的内容增强。
---

# TOP5资料反合Trunk主线

## 输入文件

| 文件 | 路径模式 | 角色 |
|------|---------|------|
| TOP5 源文件 | `docs/api/TOP5/{ApiName}.md` | 产品化文档，包含 A2/A3 系列产品的接口描述、内部交叉引用链接 |
| Trunk 源文件 | `docs/api/SIMD-API/基础API/Memory矢量计算/{分类}/{ApiName}.md` | 主线文档，包含 950/全产品线内容、产品专属重载（如复数计算）、config 参数、完整样例代码 |

## 输出文件

`docs/api/SIMD-API/基础API/Memory矢量计算/{分类}/{ApiName}-new.md`（与 Trunk 源文件同目录，文件名加 `-new` 后缀）

---

## 合并规则

### 1. 标题与锚点（Anchor）

- **标题 anchor 沿用 Trunk**：所有 `## ` 级别标题后的 `<a name="...">` 使用 Trunk 源文件的值。
- **`# ` 一级标题 anchor 也沿用 Trunk**。
- TOP5 独有的新章节（Trunk 中不存在的），其 anchor 保留 TOP5 原值。
- **例外**：`## 数据类型` 标题后**不添加** `<a name="...">` 锚点。

### 2. 产品支持情况

- **整节保留 Trunk 的内容**（包含全部产品线的完整表格），不做任何修改。
- 不采用 TOP5 的精简版产品表格。

### 3. 功能说明

- 采用 TOP5 的**功能描述文本**（"属于单目/双目矢量类计算接口..."等描述）。
- **公式渲染**：将 Trunk/TOP5 中的图片公式 `![](...)` 替换为 LaTeX 行内公式，如 `$dst_i = |src_i|$`、`$dst_i = e^{src_i}$`。
- **头文件引用**：采用 TOP5 的头文件路径，格式调整为：`头文件引用路径为："xxx.h"`。
  - 双目接口：`basic_api/kernel_operator_vec_binary_intf.h`
  - 标量接口：`basic_api/kernel_operator_vec_binary_scalar_intf.h`
  - 单目接口：`basic_api/kernel_operator_vec_unary_intf.h`

### 4. 函数原型

#### 4.1 总体策略：共享优先，差异隔离

**核心原则**：比较 Trunk 和 TOP5 的函数重载列表，提取两者**完全相同**的重载放在最外层（不包裹任何 `<cann-filter>`），仅将**产品专属**的重载用 `<cann-filter>` 包裹。

这样做的好处：
- 避免大量重复代码（当 950 和 A2/A3 的某些重载签名完全一致时）
- 读者一眼就能看到哪些接口是通用的，哪些是产品专属的

#### 4.2 情况一：部分函数原型仅支持特定产品型号（参考 Abs-new.md）

大部分重载签名在 950（Trunk）和 910b,A3（TOP5）之间完全一致，但存在少量**产品专属重载**（如 950 的复数重载）。采用**共享优先**结构：

- 共享重载放在最外层，不包裹任何 `<cann-filter>`
- 产品专属重载用 `<cann-filter>` 包裹，紧跟在对应的共享重载后面

```markdown
- tensor前n个数据连续计算

    ```cpp
    // 950 和 A2/A3 签名完全一致，放在最外层（使用 TOP5 的签名）
    ```

<cann-filter npu-type="950">

- tensor前n个复数计算

    ```cpp
    // 该函数原型，仅支持Ascend 950PR/Ascend 950DT系列产品
    template <typename T, typename U>
    __aicore__ inline void Abs(const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t& count)
    ```

</cann-filter>

- tensor高维切分计算
    - mask逐bit模式 / mask连续模式

        ```cpp
        // 950 和 A2/A3 签名完全一致，放在最外层（使用 TOP5 的签名）
        ```
```

**特殊子情况**：当所有重载签名完全相同（即没有产品专属重载，如 Add/Sub/Mul/Div/Max/Min 等只有 `isSetMask`，没有 `config`），所有函数原型放在最外层，不包裹任何 `<cann-filter>`。

#### 4.3 情况二：所有函数原型区分产品型号（参考 Exp-new.md）

当 950 的所有重载签名都包含产品专属参数（如 `config`），与 910b,A3 的签名**全部不同**时，所有函数原型按产品分别用 `<cann-filter>` 包裹：

```markdown
<cann-filter npu-type="950">
以下原型仅支持Ascend 950PR/Ascend 950DT系列产品

- tensor前n个数据连续计算

    ```cpp
    // 950 签名（含 config 模板参数）
    ```

- tensor高维切分计算
    - mask逐bit模式 / mask连续模式

        ```cpp
        // 950 签名（含 config 模板参数）
        ```

</cann-filter>

以下原型支持

<cann-filter npu-type="A3">
Atlas A3 训练系列产品/Atlas A3 推理系列产品
</cann-filter>

<cann-filter npu-type="910b">
Atlas A2 训练系列产品/Atlas A2 推理系列产品
</cann-filter>

<cann-filter npu-type="310b">
Atlas 200I/500 A2 推理产品
</cann-filter>

<cann-filter npu-type="310p">
Atlas 推理系列产品
</cann-filter>

<cann-filter npu-type="910">
Atlas 训练系列产品
</cann-filter>

<cann-filter npu-type="x90">
Kirin X90
</cann-filter>

<cann-filter npu-type="9030">
Kirin 9030
</cann-filter>

- tensor前n个数据连续计算

    ```cpp
    // A2/A3 签名（含 isSetMask，无 config）
    ```

- tensor高维切分计算
    - mask逐bit模式 / mask连续模式

        ```cpp
        // A2/A3 签名（含 isSetMask，无 config）
        ```

</cann-filter>
```

**关键规则**：
- **情况一**中，共享重载不包裹 `<cann-filter>`，产品专属重载用 `<cann-filter>` 包裹。
- **情况二**中，所有函数原型按产品用 `<cann-filter>` 包裹，`npu-type="950"` 块在前，`npu-type="910b,A3"` 块在后。
- `npu-type="950"` 块：使用 Trunk 的函数签名（含 `config` 模板参数或复数模板参数）。
- `npu-type="910b,A3"` 块：使用 TOP5 的函数签名（含 `isSetMask` 模板参数，无 `config`）。
- 代码块添加 `cpp` 语言标注。

### 5. 参数说明

#### 5.1 表格格式转换

将 Trunk 的 **HTML 表格**（`<table>`）转换为 **Markdown 简易表格**（`| ... |`）。

以下情况**不转换**，保持 HTML `<table>` 格式：
- 产品支持情况的 HTML 表格 — 整节保留 Trunk 原样。

#### 5.2 模板参数说明表（表 1）— 统一使用 Markdown 简易表格

**模板参数在表格中的顺序，要跟所有模板参数同时出现的函数原型（比如tensor高维切分计算函数原型）中的顺序保持一致**

**所有表 1 均使用 Markdown 简易表格**，包括含 config 等带 `<pre>` 代码块的参数行。不使用 HTML `<table>` 格式。

| 参数 | 处理方式 |
|------|---------|
| T | 简化为 `操作数数据类型。`（不在此行列举具体类型，因为数据类型详情已移至「数据类型」章节按产品拆分展示） |
| isSetMask | 采用 TOP5 的描述，保留 TOP5 的交叉引用链接（`SetVectorMask.md`、`掩码.md`） |
| Trunk 独有参数（如 `config`、`U`） | 保留，用 `<cann-filter npu-type="950">` 包裹整行内容 |
| 参数行顺序 | T → 产品专属参数（用 cann-filter 包裹）→ isSetMask |

**cann-filter 行内写法**（Markdown 表格中，无 `<pre>` 代码块的简单参数）：
```markdown
| <cann-filter npu-type="950">U | 源操作数数据类型。<br>Ascend 950PR/Ascend 950DT，支持的数据类型为：complex32、complex64；...</cann-filter> |
```

**cann-filter 行内写法**（Markdown 表格中，含 `<pre>` 代码块的 config 参数）：
```markdown
| <cann-filter npu-type="950">config | 该参数仅支持Ascend 950PR/Ascend 950DT。<br>用于配置Subnormal计算模式，XxxConfig类型，定义如下：<br><pre class="screen" codetype="Cpp">enum class XxxAlgo {&#10;    INTRINSIC = 0,&#10;    PRECISION_1ULP_FTZ_TRUE,&#10;    PRECISION_1ULP_FTZ_FALSE,&#10;};&#10;struct XxxConfig {&#10;    XxxAlgo algo = XxxAlgo::INTRINSIC;&#10;};</pre>通过XxxConfig结构体的参数algo来配置...。algo取值如下：<br>&bull; XxxAlgo::INTRINSIC、XxxAlgo::PRECISION_1ULP_FTZ_TRUE，...。<br>&bull; XxxAlgo::PRECISION_1ULP_FTZ_FALSE，...。<br>该参数的默认值DEFAULT_XXX_CONFIG的取值如下：<br><pre class="screen" codetype="Cpp">constexpr XxxConfig DEFAULT_XXX_CONFIG = { XxxAlgo::INTRINSIC };</pre></cann-filter> |
```

**要点**：
- `<cann-filter>` 标签从参数名列开始包裹，到描述列末尾结束，跨越整行。
- **整行必须写在同一物理行**（Markdown 表格行不能跨行），否则会破坏表格结构。
- `<pre>` 内的换行用 `&#10;`（HTML 换行实体）代替物理换行符，确保 `<pre>` 代码块渲染时正确换行。
- 段落之间用 `<br>` 分隔，列表项用 `&bull;` 前缀 + `<br>` 分隔。
- 不使用 `<p>`、`<ul><li>` 等 HTML 块级标签（Markdown 表格行内不支持块级元素换行）。

#### 5.3 参数说明表（表 2）— Markdown 格式

各参数描述优先采用 **TOP5 的文本和内部交叉引用链接**：

| 参数 | 链接来源 |
|------|---------|
| dst/src | TOP5 的 `Unified-Buffer介绍.md` 链接（不使用 Trunk 的 `LocalTensor.md` 或内联"32字节对齐"文本） |
| count | TOP5 的 `连续计算.md` 链接 |
| mask | TOP5 的 `掩码.md` 链接（简化描述，不保留 Trunk 的详细逐bit/连续模式范围表） |
| repeatTime | TOP5 的 `高维切分.md` 链接（不使用 Trunk 的外部 gitcode URL） |
| repeatParams | TOP5 的 `UnaryRepeatParams.md` 链接和 `高维切分.md` 链接 |

#### 5.4 多段落单元格

Markdown 表格中同一单元格的多个段落用 `<br>` 分隔，列表项用 `&bull;` 前缀。

### 6. 章节合并与排序

按 Trunk 的章节顺序排列，TOP5 独有的章节按逻辑位置插入：

```
产品支持情况        ← Trunk（完整版）
功能说明            ← TOP5 文本 + 公式/头文件改造
函数原型            ← 合并，共享优先 + cann-filter 隔离产品专属重载
参数说明            ← 合并，表格格式按 <pre> 有无决定 HTML/Markdown
数据类型            ← 合并，根据产品支持类型是否相同决定是否按产品 cann-filter 拆分
返回值说明          ← Trunk 独有章节
约束说明            ← 合并，产品专属约束用 cann-filter 包裹
调用示例            ← 合并，共享代码不包裹 filter，产品专属样例用 cann-filter
```

**丢弃的章节**：
- TOP5 的 `API映射关系` 章节（built-in 接口映射）— 不保留。
- Trunk 的 `更多样例可参考[LINK](更多样例-x.md)。` 行 — 不保留。

### 6.1 数据类型章节

- `## 数据类型` 标题后**不添加** `<a name="...">` 锚点。
- 严格从 Trunk 的 T 参数描述中提取各产品的数据类型列表。
- 根据各产品支持的数据类型是否相同，选择不同写法：

#### 写法一：所有产品支持的数据类型完全相同

直接写一句话描述，不使用 `<cann-filter>` 拆分。参考 Exp-new.md：

```markdown
## 数据类型

支持的数据类型为：half、float。
```

#### 写法二：不同产品支持的数据类型不同

按产品分别用 `<cann-filter>` 包裹，每个产品一个块。参考 Abs-new.md：

```markdown
## 数据类型

<cann-filter npu-type="950">

- 针对Ascend 950PR/Ascend 950DT系列产品
    支持的数据类型为：int8_t、int16_t、half、int32_t、float、int64_t。

</cann-filter>

<cann-filter npu-type="A3">

- 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：
    支持的数据类型为：half、float。

</cann-filter>

...（其余产品以此类推）
```

**产品列表顺序**（与产品支持情况表格一致）：950 → A3 → 910b → 310b → 310p → 910 → x90 → 9030

**npu-type 取值对照**：

| 产品 | npu-type 值 |
|------|------------|
| Ascend 950PR/Ascend 950DT | `950` |
| Atlas A3 训练/推理系列产品 | `A3` |
| Atlas A2 训练/推理系列产品 | `910b` |
| Atlas 200I/500 A2 推理产品 | `310b` |
| Atlas 推理系列产品 | `310p` |
| Atlas 训练系列产品 | `910` |
| Kirin X90 | `x90` |
| Kirin 9030 | `9030` |

### 7. 约束说明

- 保留 TOP5 的约束条目（如 NOP 约束、支持数据类型声明）。
- 保留 Trunk 的约束条目（地址对齐、地址重叠）。
- **产品专属约束**（如"针对Ascend 950PR/Ascend 950DT，int8_t和int64_t数据类型仅支持tensor前n个数据计算接口"）用 `<cann-filter npu-type="950">` 包裹。
- 丢弃 TOP5 中标注"**待补充**"的约束条目。
- 丢弃 **不区分产品版本** 的 **支持数据类型为：** 的约束条目。
- 链接统一使用 TOP5 的内部引用（`Unified-Buffer介绍.md`）。

### 8. 调用示例

#### 8.1 总体策略：共享优先

与函数原型相同，采用**共享代码不包裹 filter，产品专属样例用 cann-filter 隔离**的策略。

#### 8.2 样例结构

```markdown
- tensor高维切分计算样例-mask连续模式

    ```cpp
    // 通用样例代码（不分产品）
    ```

    结果示例如下：

    ```bash
    // 结果
    ```

- tensor高维切分计算样例-mask逐bit模式

    ```cpp
    // 通用样例代码
    ```

- tensor前n个数据计算接口样例

    ```cpp
    // 950 和 A2/A3 代码完全一致时，不包裹 filter
    AscendC::Abs(dstLocal, srcLocal, 512);
    ```

<cann-filter npu-type="950">

针对Ascend 950PR/Ascend 950DT系列产品：

- tensor前n个复数计算接口样例

    ```cpp
    // 仅 950 支持的样例
    ```

    结果示例如下：

    ```bash
    // 结果也放在 filter 内
    ```

</cann-filter>
```

#### 8.3 关键规则

- **通用样例**（不分产品的 mask 连续/逐bit 模式、950/A2/A3 代码完全一致的前n个数据样例）不包裹 cann-filter。
- **产品专属样例**用 `<cann-filter>` 包裹，**整个样例（含代码块和结果示例）都放在 filter 内**。
  - 但当同一类示例拆分为多个产品块（如 910b,A3 和 950）时，若结果数据各产品通用，则"结果示例如下"放在所有 cann-filter 块之后。与 cann-filter 同级缩进（4 空格），表示结果各产品通用。
- `<cann-filter>` 标签前后需有空行，标签内部不使用子列表（`- `），代码块保持 4 空格缩进。
- 代码块添加 `cpp`/`bash` 语言标注。
- **cann-filter 块顺序**：当确实需要拆分同一类样例时，`npu-type="910b,A3"` 在前，`npu-type="950"` 在后。
- **950 块样例只保留 config 版本调用**（针对有 config 参数的接口）：只展示 `static constexpr XxxConfig config = {...}; AscendC::Xxx<T, config>(...)` ，config 声明在前、调用在后。
- 不保留 Trunk 的 `更多样例可参考[LINK](更多样例-x.md)。` 引用行。
- 不保留"本样例的srcLocal和dstLocal均为xxx类型。"独立行。

---

## 执行检查清单

完成合并后，逐项检查：

1. [ ] 标题 anchor 全部沿用 Trunk（新章节保留 TOP5 的），但 `## 数据类型` 标题后不添加锚点
2. [ ] 产品支持情况表格完整保留 Trunk 版本
3. [ ] 公式已替换为 LaTeX 行内公式
4. [ ] 头文件引用路径已包含
5. [ ] 函数原型策略正确：情况一（部分产品专属重载，参考 Abs-new.md）共享优先 + cann-filter 隔离；情况二（所有原型区分产品，参考 Exp-new.md）全部按产品 cann-filter 包裹
6. [ ] 参数表格格式正确：表 1 统一使用 Markdown 简易表格（含 config 时用 `<pre>` + `<br>` 行内写法），表 2 使用 Markdown
7. [ ] 模板参数表（表 1）中 T 描述简化为 `操作数数据类型。`（类型详情在数据类型章节展示），产品专属参数用 cann-filter 包裹
8. [ ] 参数描述链接已统一为 TOP5 的内部引用
9. [ ] Trunk 独有的模板参数（如 config、U）已保留并用 cann-filter 包裹
10. [ ] 章节顺序正确（Trunk 顺序 + TOP5 新章节按逻辑位置插入）
11. [ ] API映射关系章节已丢弃
12. [ ] 数据类型章节：标题后无锚点；数据类型从 Trunk 的 T 参数描述中提取；所有产品类型相同时一句话描述，不同时按产品 cann-filter 拆分
13. [ ] 约束说明无"待补充"条目，产品专属约束用 cann-filter 包裹
14. [ ] 调用示例采用共享优先策略：代码完全一致的样例不包裹 filter，产品专属样例用 cann-filter 包裹（含结果示例）
15. [ ] 所有代码块已添加语言标注（`cpp`/`bash`）
16. [ ] 无 `<cann-filter>` 标签与子列表嵌套导致的渲染问题
17. [ ] 不保留 `更多样例可参考[LINK]` 引用行
18. [ ] 约束说明中丢弃不区分产品版本的"支持数据类型为：xxx"约束条目
19. [ ] 模板参数表（表 1）中参数顺序，要跟所有模板参数同时出现的函数原型（比如tensor高维切分计算函数原型）中的顺序保持一致。有的是：T → 产品专属参数（cann-filter）→ isSetMask；有的是：T → isSetMask → 产品专属参数（cann-filter）；
