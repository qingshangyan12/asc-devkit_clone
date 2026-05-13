# aclnn 领域质检规则 - 规范性检查详细说明

**重要提示：**

- 仅使用本文件中明确定义的规则进行检查
- 规则名称、规则级别必须与本文件完全一致

## 1. 规范性检查

检查文档格式和命名是否符合统一规范。此类检查仅基于文档内容，不需要读取代码。

| 规则名称                               | 描述                                                                                       | 级别 |
| -------------------------------------- | ------------------------------------------------------------------------------------------ | ---- |
| API 文件命名规范                       | 检查 API 文件命名是否遵循 aclnnXxx 前缀、文件名与一级标题一致                              | 严重 |
| API 章节标题命名规范                   | 检查##二级标题是否符合必选章节要求（产品支持情况、功能说明、函数原型、约束说明、调用示例） | 提示 |
| 产品型号命名规范                       | 检查产品名称是否在允许范围内、是否使用 term 标记                                           | 严重 |
| API 函数原型格式规范                   | 检查原型格式（返回类型、参数格式、缩进、末尾符号、语言类型标识、两段式接口参数形式）       | 一般 |
| API 参数表里字段规范                   | 检查参数表格字段名称是否在允许范围内                                                       | 提示 |
| API 参数表里参数名列不规范             | 检查参数名是否与函数原型一致、参数顺序是否一致                                             | 一般 |
| API 参数表里输入/输出列不规范          | 检查输入/输出列的书写方式是否在允许范围内（输入、输出、输入/输出等）                       | 一般 |
| API 参数表里数据类型不规范             | 检查 dtype 是否大写、是否在支持范围内                                                      | 严重 |
| API 参数表里数据格式不规范             | 检查 format 是否大写、FRACTAL_NZ 与 NZ 是否混用、基础类型是否有 format                     | 严重 |
| API 参数表里 shape 不规范              | 检查基础数据类型是否错误描述了 shape                                                       | 严重 |
| API 参数表里非连续 Tensor 支持度不规范 | 检查是否对非 aclTensor 类型错误描述了非连续 Tensor 支持度                                  | 严重 |
| API 参数表里描述不规范                 | 检查参数表不允许出现"None"和"默认值"                                                       | 一般 |
| 返回值/错误码不规范                    | 检查返回值和错误码是否在定义范围内                                                         | 严重 |
| 表格使用规范性                         | 检查两段式接口的参数说明和拦截校验是否使用表格形式                                         | 提示 |

### API 文件命名规范

1. API 命名前缀必须是 aclnnXxx，注意 API 名 (Xxx) 必须为大驼峰格式，（仅 2 个接口例外：aclStft 和 aclRfft1D）
2. 允许多 API 合并命名，采用&符号拼接（举例：aclnnAdd&aclnnInplaceAdd）
3. 要求 API 文件命名、API 文件的 `#一级标题`保持一致。
   - 举例 1：`aclnnAbs.md`与 `# aclnnAbs`
   - 举例 2：`aclnnAcos&aclnnInplaceAcos.md`与 `# aclnnAcos&aclnnInplaceAcos`

### API 章节标题命名规范

1. 仅针对##二级章节标题限制
2. 二级标题**必选章节**必须存在，且名称必须与下述列表完全一致（一字不差）：[`## 产品支持情况`、`## 功能说明`、`## 函数原型`、`## 约束说明`、`## 调用示例`]
3. 除上述第2条必选章节必须存在外，还可增加自定义二级章节标题，但自定义标题不允许与必选标题语义重复（例如：已有必选章节"## 调用示例"就无法自定义"## 调用说明"）

### 产品型号命名规范

1. 只允许写指定范围内容的产品名。（具体范围见下面的例子中的产品列）
2. 需要加 term 标记对，例如 `<term>` 产品名 `</term>`。对标签内末尾存在多余空格无要求。
3. 允许的产品名称列表：[产品名称列表](./aclProductList.md)

### API 函数原型格式规范

**必须逐一检查以下8项，每项错误均需独立报告：**

1. 返回类型必须是 `aclnnStatus`
2. 参数名必须使用小驼峰格式，不带英文或中文符号，如下划线、中横线等（如 `cubeMathType`、`bias`）
3. 单参数应单行呈现
4. 多参数时缩进对齐，`*` 号位置随意但所有参数要保持一致
   例如：
   ```Cpp
   aclnnStatus aclnnAbsGetWorkspaceSize(
      const aclTensor *self, 
      aclTensor       *out, 
            uint64_t        *workspaceSize, （该行缩进未对齐）
      aclOpExecutor  **executor)
   ```
5. 原型末尾不加分号或其他符号
6. API命名规范：每个算子接口可以对应n个接口，但只允许存在如下2种命名样式：{`aclnnXxxGetWorkspaceSize`, `aclnnXxx`}
   错误样例：aclnnXxxGetWorkSpaceSize、aclnnXxxGetWorkspacesize
7. 第 1 段接口（特指aclnnXxxGetWorkspaceSize）最后 2 个参数形式必须为：`aclnnStatus aclnnXxxGetWorkspaceSize(xxx, uint64_t* workspaceSize, aclOpExecutor** executor)`
8. 第 2 段接口（特指aclnnXxx）只允许 4 个参数，参数形式必须为下述两种：
   - `aclnnStatus aclnnXxx(void *workspace, uint64_t workspaceSize, aclOpExecutor *executor, const aclrtStream stream)`
   - `aclnnStatus aclnnXxx(void *workspace, uint64_t workspaceSize, aclOpExecutor *executor, aclrtStream stream)` （aclrtStream stream 的 `const` 可有可无）

### API 参数表里字段规范

1. md 所有表格里的字段名称固定，只允许在以下范围中：
   {`参数名`、`输入/输出`、`描述`、`使用说明`、`数据类型`、`数据格式`、`维度 (shape)`、`非连续 Tensor`}
2. 大小写完全一致。

### API 参数表里参数名列不规范

无论哪一段接口参数必需满足：

1. 支持不带参数类型的参数名，要求与函数原型中的参数命名一致。
2. 支持带参数类型的参数名，要求与函数原型中的参数命名一致。（例如a (aclTensor*)带参数类型，注意括号中英文均可）
3. 参数名顺序是否与函数原型中的参数顺序一致。
4. 带参数类型的参数名，其参数类型要与函数原型中参数类型一致（例外：const带不带皆可，*号位置无所谓）。

### API 参数表里输入/输出列不规范

1. 只允许七种书写方式：{`输入`，`输出`，`输入/输出`，`输入 | 输出`，`输入和输出`，`可选输出`，`可选输入`}，其余情况不支持。

### API 参数表里数据类型不规范

1. 支持文字类描述，保证无语病（例如与xx类型一致、同上等）
2. 支持文字描述里包含 dtype，要求大写，可以是 ACL_XX 或 XX 形式。
3. 支持单纯罗列 dtype 范围，要求中文顿号分隔（>=2个）
4. dtype 范围参见 [aclDataType.md](aclDataType.md)，超出范围不支持。

### API 参数表里数据格式不规范

1. 支持文字类描述，保证无语病（例如与xx格式一致、同上等）
2. 支持文字描述里包含 format，要求大写，可以是 ACL_FORMAT_XX 或 XX 形式。
3. 支持单纯罗列format范围，要求中文顿号分隔（>=2个）
4. format 范围参见 [aclDataFormat.md](aclDataFormat.md)，超出范围不支持。
5. FRACTAL_NZ 和 NZ 格式含义一样，均可
6. 针对基础数据类型（int64_t/float/double/bool等），不存在Tensor，就不会有数据格式的描述，单元格写 `-`（表示不涉及）

### API 参数表里 shape 不规范

1. 支持文字类描述，保证无语病（例如与xx维度一致、与xx shape一致、同上、1~8、[1, 8]、1-8等）
2. 针对基础数据类型（int64_t/float/double/bool等），不存在Tensor，就不存在shape的描述，单元格写 `-`（表示不涉及）

### API 参数表里非连续 Tensor 支持度不规范

针对 `aclTensor*`、`aclTensorList*` 参数类型，才需要判断是否支持非连续 Tensor。否则其他参数类型，不存在非连续判断，单元格写 `-`（表示不涉及）

### API 参数表里描述不规范

参数说明表里所有单元格内容：不允许出现 `None`和 `默认值`。

### 返回值/错误码不规范

返回值和错误码只允许在下述定义范围内: [允许的错误码列表](./aclErrorCode.md)

### 表格使用规范性

接口的每个 `## 函数接口`中的 `参数说明`和 `参数拦截校验`必须是表格形式，支持 md 和 html 语法格式

---
