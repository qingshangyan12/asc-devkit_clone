---
name: ascendc-top5-docs-merge-to-trunk-docs
description: 将 TOP5（产品化文档）与 Trunk（主线文档）两个来源的 API 文档合并为一份统一的 `xxx-new.md`，以 Trunk 为骨架，融合 TOP5 的内容增强。
---

# TOP5资料反合Trunk主线

## 输入文件

| 文件 | 路径模式 | 角色 |
|------|---------|------|
| TOP5 源文件 | `docs/api/TOP5/{ApiName}.md` | 产品化文档，包含 A2/A3 系列产品的接口描述、内部交叉引用链接 |
| Trunk 源文件 | `docs/api/SIMD-API/基础API/Memory矢量计算/{分类}/{ApiName}.md` | 主线文档，包含 950/全产品线内容、config 参数、完整样例代码 |

## 输出文件

`docs/api/SIMD-API/基础API/Memory矢量计算/{分类}/{ApiName}-new.md`（与 Trunk 源文件同目录，文件名加 `-new` 后缀）

---

## 合并规则

### 1. 标题与锚点（Anchor）

- **标题 anchor 沿用 Trunk**：所有 `## ` 级别标题后的 `<a name="...">` 使用 Trunk 源文件的值。
- **`# ` 一级标题 anchor 也沿用 Trunk**。
- TOP5 独有的新章节（Trunk 中不存在的），其 anchor 保留 TOP5 原值。

### 2. 产品支持情况

- **整节保留 Trunk 的内容**（包含全部产品线的完整表格），不做任何修改。
- 不采用 TOP5 的精简版产品表格。

### 3. 功能说明

- 采用 TOP5 的**功能描述文本**（"属于单目矢量类计算接口..."等描述）。
- **公式渲染**：将 Trunk/TOP5 中的图片公式 `![](...)` 替换为 LaTeX 行内公式，如 `$dst_i = e^{src_i}$`。
- **头文件引用**：采用 TOP5 的头文件路径，格式调整为：`头文件引用路径为："xxx.h"`。

### 4. 函数原型

使用 `<cann-filter>` 按产品类型拆分为两段：

```markdown
<cann-filter npu-type="950">
针对Ascend 950PR/Ascend 950DT系列产品，函数原型如下：

- tensor前n个数据连续计算

    ```cpp
    // 使用 Trunk 的签名（含 config 参数）
    ```

- tensor高维切分计算
    - mask逐bit模式 / mask连续模式

        ```cpp
        // 使用 Trunk 的签名
        ```

</cann-filter>

<cann-filter npu-type="910b,A3">
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品，函数原型如下：

- tensor前n个数据连续计算

    ```cpp
    // 使用 TOP5 的签名（含 isSetMask 参数，无 config）
    ```

- tensor高维切分计算
    - mask逐bit模式 / mask连续模式

        ```cpp
        // 使用 TOP5 的签名
        ```

</cann-filter>
```

**关键规则**：
- `npu-type="950"` 块：使用 Trunk 的函数签名（含 `config` 模板参数）。
- `npu-type="910b,A3"` 块：使用 TOP5 的函数签名（含 `isSetMask` 模板参数，无 `config`）。
- 代码块添加 `cpp` 语言标注。

### 5. 参数说明

#### 5.1 表格格式转换

将 Trunk 的 **HTML 表格**（`<table>`）转换为 **Markdown 简易表格**（`| ... |`）。

以下情况**不转换**，保持 HTML `<table>` 格式：
- 产品支持情况的 HTML 表格 — 整节保留 Trunk 原样。
- **模板参数说明表（表 1）** — 因 `config` 等参数行包含 `<pre>` 多行代码块，Markdown 表格无法承载多行内容（换行会破坏表格行结构），必须使用 HTML `<table>` 格式以确保代码块正确渲染。

#### 5.2 模板参数说明表（表 1）— HTML 格式

**整表使用 HTML `<table>` 格式**，不使用 Markdown 简易表格。

| 参数 | 处理方式 |
|------|---------|
| T | 采用 TOP5 的描述文本 |
| isSetMask | 采用 TOP5 的描述，保留 TOP5 的交叉引用链接（`SetVectorMask.md`、`掩码.md`） |
| Trunk 独有参数（如 `config`） | 保留，用 `<cann-filter npu-type="950">` 包裹 `<td>` 内的全部内容 |

**config 行写法要点**：
- `<cann-filter>` 标签直接包裹在 `<td>` 内所有内容的**外层**。
- 代码片段使用 `<pre class="screen" codetype="Cpp">` 标签（沿用 Trunk 源文件的 `class` 和 `codetype` 属性），`<pre>` 内可正常换行。
- 多段落使用 `<p>` 标签，列表使用 `<ul><li>` 标签。
- 示例结构：
  ```html
  <tr><td class="cellrowborder" valign="top"><p>config</p></td>
  <td class="cellrowborder" valign="top"><cann-filter npu-type="950">
  <p>该参数仅支持Ascend 950PR/Ascend 950DT。</p>
  <p>用于配置...，LnConfig类型，定义如下：</p>
  <pre class="screen" codetype="Cpp">enum class LnAlgo {
      INTRINSIC = 0,
      ...
  };</pre>
  <p>取值说明：</p>
  <ul><li>LnAlgo::INTRINSIC...</li></ul>
  </cann-filter></td></tr>
  ```

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
函数原型            ← 合并，cann-filter 拆分
参数说明            ← 合并，表格转 Markdown
数据类型            ← TOP5 独有章节，插入此处
返回值说明          ← Trunk 独有章节
约束说明            ← 合并
调用示例            ← 合并，cann-filter 拆分
```

**丢弃的章节**：
- TOP5 的 `API映射关系` 章节（built-in 接口映射）— 不保留。

### 7. 约束说明

- 保留 TOP5 的约束条目（如 NOP 约束、支持数据类型声明）。
- 保留 Trunk 的约束条目（地址对齐、地址重叠）。
- 丢弃 TOP5 中标注"**待补充**"的约束条目。
- 链接统一使用 TOP5 的内部引用（`Unified-Buffer介绍.md`）。

### 8. 调用示例

使用 `<cann-filter>` 按产品类型拆分样例：

```markdown
- tensor高维切分计算样例-mask连续模式

    ```cpp
    // 通用样例代码（不区分产品）
    ```

- tensor高维切分计算样例-mask逐bit模式

    ```cpp
    // 通用样例代码
    ```

- tensor前n个数据计算接口样例

    <cann-filter npu-type="910b,A3">

    针对Atlas A2/A3系列产品，示例如下：

    ```cpp
    // TOP5 或通用样例
    ```

    </cann-filter>

    <cann-filter npu-type="950">

    针对Ascend 950PR/Ascend 950DT系列产品，示例如下：

    ```cpp
    // Trunk 的 950 专属样例
    ```

    </cann-filter>
```

**关键规则**：
- 通用样例（不分产品的 mask 连续/逐bit 模式）不包裹 cann-filter。
- 仅产品专属样例用 `<cann-filter>` 包裹。
- `<cann-filter>` 标签前后需有空行，标签内部不使用子列表（`- `），代码块保持 4 空格缩进。
- 代码块添加 `cpp`/`bash` 语言标注。
- **cann-filter 块顺序**：tensor前n个数据计算接口样例中，`npu-type="910b,A3"`（A2/A3 系列）在前，`npu-type="950"`（950 系列）在后。
- **950 块样例只保留 config 版本调用**：不要同时写无 config 的默认调用和有 config 的调用。只展示 `static constexpr XxxConfig config = {...}; AscendC::Xxx<T, config>(...)` 即可，config 声明在前、调用在后。

---

## 执行检查清单

完成合并后，逐项检查：

1. [ ] 标题 anchor 全部沿用 Trunk（新章节保留 TOP5 的）
2. [ ] 产品支持情况表格完整保留 Trunk 版本
3. [ ] 公式已替换为 LaTeX 行内公式
4. [ ] 头文件引用路径已包含
5. [ ] 函数原型按 cann-filter 正确拆分（950 用 Trunk 签名，A2/A3 用 TOP5 签名）
6. [ ] 参数表格已转为正确格式（表 1 用 HTML，表 2 用 Markdown，产品支持情况除外）
7. [ ] 参数描述链接已统一为 TOP5 的内部引用
8. [ ] Trunk 独有的模板参数（如 config）已保留并用 cann-filter 包裹
9. [ ] 章节顺序正确（Trunk 顺序 + TOP5 新章节按逻辑位置插入）
10. [ ] API映射关系章节已丢弃
11. [ ] 约束说明无"待补充"条目
12. [ ] 调用示例中代码块渲染正确，cann-filter 标签不干扰 Markdown 解析
13. [ ] 所有代码块已添加语言标注（`cpp`/`bash`）
14. [ ] 无 `<cann-filter>` 标签与子列表嵌套导致的渲染问题
15. [ ] tensor前n个数据计算接口样例的 cann-filter 块顺序：A2/A3 在前、950 在后，950 块只保留 config 版本调用（不重复调用）
