# PR文档翻译Skill

## 功能描述
检测用户本地仓库的md文档，自动翻译成英文文档。支持：
- 非docs目录下的所有md文档
- docs目录下，排除列表外的md文档

## 触发条件
- 用户指定本地仓库发生变化
- md文档不存在对应的英文翻译文档（`_en.md`），或存在文档变更

## 执行流程

### 1. 扫描所有md文档并统计

扫描用户本地仓库的所有`.md`文档（排除.git目录），**必须先统计排除路径文件数量，再计算实际需翻译数量**。

**扫描流程：**

1. **统计总md文件数量**
   ```bash
   find . -name "*.md" -not -path "./.git/*" -type f | wc -l
   ```

2. **读取排除列表配置**
   ```bash
   current_path=$(pwd) 
   cat ${current_path}\exclude_docs.json
   ```

3. **统计排除路径文件数量**（避免误导用户）
   ```bash
   # 统计每个排除路径的文件数量
   find . -name "*.md" -not -path "./.git/*" -path "*/docs/dflow_api/*" | wc -l
   find . -name "*.md" -not -path "./.git/*" -path "*/docs/graph_engine_api/*" | wc -l
   # 或统一统计所有排除路径
   find . -name "*.md" -not -path "./.git/*" \
     -not -path "*/docs/dflow_api/*" \
     -not -path "*/docs/graph_engine_api/*" \
     -not -path "*/docs/llm_datadist_api/*" | wc -l
   ```

4. **计算实际需要翻译的文件数量**
   ```
   实际需翻译数量 = 总文件数 - 排除路径文件数
   ```

**统计报告格式：**
向用户输出清晰的统计信息：
```
扫描结果：
- 总md文件：1495个
- 排除路径（跳过）：1345个
  - docs/dflow_api：401个
  - docs/graph_engine_api：843个  
  - docs/llm_datadist_api：101个
- 实际需翻译：150个
```

### 2. 读取排除列表配置

读取配置文件：`${current_path}\exclude_docs.json`

### 3. 判断文档是否需要翻译

对每个md文档按以下流程判断：

**步骤1：提取仓库名并匹配排除配置**
1. 获取本地仓库远程地址
2. 从远程地址提取仓库名（去除.git后缀）
3. 在配置中查找repoName是否包含仓库名（关键字匹配）
   - 匹配成功 → 该仓库有docs路径排除配置
   - 不匹配 → 需要翻译所有docs文档

**步骤2：判断文档是否需要翻译**
1. 非docs目录文档 → 需要翻译
2. docs目录文档 → 检查是否在排除路径中
   - 在excludeDocsPaths列表中 → 跳过
   - 不在排除路径列表中 → 需要翻译
3. 已存在翻译文件 → 检查是否有变更
   - 有变更 → 需要增量翻译
   - 无变更 → 跳过
4. 不存在翻译文件 → 需要完整翻译

> 详细判断流程和示例见 [Q1: 如何判断文档是否需要翻译](#q1-如何判断文档是否需要翻译)

**重要提示：**
- `excludeDocsPaths`配置的含义是：这些docs子路径已经在公开仓库维护，不需要在本仓库翻译
- 其他docs路径的文档仍然需要翻译
- 仓库名匹配采用**关键字匹配**（如`ge_en_test`匹配`ge`）

### 4. 确定翻译文件存放路径

**非docs目录文档：**
- 存放规则：与原文件同级目录
- 文件命名：`<原文件名>_en.md`
- 示例：`README.md` → `README_en.md`

**docs目录文档（非排除列表）：**
- **docs根目录下的README.md**：存放在同级目录，命名为`README_en.md`
  - 示例：`docs/README.md` → `docs/README_en.md`
- **docs其他文档**：存放在`docs/en/`目录下，保持原有目录层级
  - 文件命名：`<原文件名>_en.md`
  - 示例：`docs/api/quantize.md` → `docs/en/api/quantize_en.md`
  - 示例：`docs/api/README.md` → `docs/en/api/README_en.md`

**PR模板文件（特殊规则）：**
- 识别规则：文件名包含`PULL_REQUEST_TEMPLATE`且后缀为语言代码（如`.zh-CN.md`）
- 存放规则：与原文件同级目录（`.gitcode/`目录）
- 文件命名：将语言代码替换为对应英文代码，不使用`_en`后缀
- 示例：
  - `.gitcode/PULL_REQUEST_TEMPLATE.zh-CN.md` → `.gitcode/PULL_REQUEST_TEMPLATE.en-US.md`
  - 其他平台模板：`ISSUE_TEMPLATE.zh-CN.md` → `ISSUE_TEMPLATE.en-US.md`

> 详细路径说明和示例见 [Q2: 如何确定翻译文件存放路径](#q2-如何确定翻译文件存放路径)

### 5. 检查是否存在翻译文件

根据文档路径类型检查对应的翻译文件是否存在。

> 详细检查方法和示例见 [Q4: 如何检查是否存在翻译文件](#q4-如何检查是否存在翻译文件)

### 6. 检查文档变更

如果翻译文件已存在，检查原文档是否有变更。

> 详细检查方法见 [Q5: 如何检测文档变更](#q5-如何检测文档变更)

### 7. 读取翻译标准（必须执行）

> **⚠️ 重要警告：翻译前必须先读取翻译标准！遗漏此步骤会导致翻译不符合规范，需要返工修正！**

**必须在翻译任何内容之前**，读取`https://developers.google.com/style`英文风格指南的关键内容。

#### 翻译标准核心要点（必须遵守）

**一、语态规范**

| 规范 | 要求 | 示例 |
|------|------|------|
| 主动语态优先 | 面向用户的资料以主动语态为主 | ❌ "Designed for..." → ✅ "This guide provides..." |
| 操作类用祈使句 | 操作步骤省去you，直接使用动词开头 | ❌ "You can enter the password" → ✅ "Enter the password" |
| 被动语态例外 | 动作执行者未知/无关、错误提示中避免责备用户时可用被动 | "The dialog box is displayed" |

**二、时态规范**

| 场景 | 使用时态 | 示例 |
|------|---------|------|
| 陈述规律/原理/机制 | 一般现在时 | "A ping command sends packets to test connectivity." |
| 操作后的瞬时结果 | 一般现在时 | ❌ "The dialog box will appear" → ✅ "The dialog box appears" |
| 需间隔较长时间的结果 | 将来时 | "The system will restart after installation." |
| 已完成的动作 | 现在完成时 | "You have successfully logged in." |

**三、词汇规范**

| 禁止使用 | 正确用法 |
|---------|---------|
| etc. | and so on（需限定范围） |
| e.g. | for example |
| i.e. | that is |
| via | through / by / using |
| can't, it's, don't | cannot, it is, do not |
| you're, they're | you are, they are |
| won't, shouldn't | will not, should not |

**四、句子和段落规范**

| 规范 | 要求 |
|------|------|
| 重要信息置前 | 将关键信息放在句首或段落开头 |
| 避免超长句子 | 每句不超过25个单词 |
| 使用并行结构 | 相似描述使用统一句式 |
| 避免双重否定 | 使用直接陈述 |

**五、好的英文风格必须满足**

- 使用前后一致的术语
- 使用简单词汇
- 定义缩略语（首次出现时定义）
- 尽量使用主动语态
- 尽量使用一般现在时态
- 使用并行结构
- 使用第二人称（操作类用祈使句）
- 清晰、正确地组织信息

**六、好的英文风格必须避免**

- 虚悬前置词
- 无谓重复或累赘
- 外来语（etc、e.g.、i.e.、via）
- 过时词汇（thus、hereinafter、hence）
- 口语词汇（figure out）
- 词汇简缩（char、config）
- 缩略词（can't、it's）

### 8. 执行翻译

**不存在翻译文件（完整翻译）：**
1. 使用read工具读取中文文档完整内容
2. 参考翻译标准进行完整翻译
3. 根据文档路径类型确定存放位置并创建翻译文件

**已存在翻译文件且有变更（增量翻译）：**
1. 使用bash工具获取中文文档的变更内容（diff）
2. 分析变更点
3. 将变更点翻译成英文
4. 使用edit工具更新对应的英文文档

> 详细处理方法见 [Q8: 如何处理已存在的翻译文件](#q8-如何处理已存在的翻译文件)

### 9. 使用todowrite跟踪进度

创建任务列表跟踪翻译进度，**必须包含统计步骤**（避免误导用户）。

**示例（正确统计流程）：**
```
[
  {"content": "扫描所有md文档并统计总数", "status": "in_progress", "priority": "high"},
  {"content": "统计排除路径文件数量（docs/dflow_api、docs/graph_engine_api等）", "status": "pending", "priority": "high"},
  {"content": "计算实际需翻译数量（总数-排除数）", "status": "pending", "priority": "high"},
  {"content": "读取翻译标准文档（必须执行）", "status": "pending", "priority": "high"},
  {"content": "翻译根目录核心文档（README/CONTRIBUTING/SECURITY等）", "status": "pending", "priority": "high"},
  {"content": "翻译 docs/architecture/ 目录（非排除路径）", "status": "pending", "priority": "high"},
  {"content": "翻译 docs/build.md 等核心文档（非排除路径）", "status": "pending", "priority": "high"},
  {"content": "提交翻译变更到远程仓库", "status": "pending", "priority": "high"}
]
```

**统计报告示例：**
扫描完成后，向用户输出清晰的统计信息：
```
扫描结果：
- 总md文件：1495个
- 排除路径（跳过翻译）：1345个
  - docs/dflow_api：401个
  - docs/graph_engine_api：843个  
  - docs/llm_datadist_api：101个
- 实际需翻译：150个
- 已存在翻译文件：3个（README_en.md等）
- 待翻译文件：147个
```

### 10. 提交变更到远程仓库

翻译完成后，将所有变更的中英文md文档提交到远程仓库。

**提交流程：**
1. 查看所有变更的md文档
2. 添加变更文件到暂存区
3. 创建提交记录
4. 推送到远程仓库

> 详细提交方法见 [Q9: 如何提交翻译变更到远程仓库](#q9-如何提交翻译变更到远程仓库)

## 配置文件说明

### 排除文档配置
- **路径**：`${current_path}\exclude_docs.json`
- **作用**：定义公开产品文档仓库及其需要排除翻译的 docs 子路径
- **格式**：JSON格式，包含 mapping 数组，每个元素包含：
  - `repoName`: 仓库名（用于匹配）
  - `publicRepo`: 公开仓库完整地址
  - `excludeDocsPaths`: 需要跳过翻译的 docs 子路径列表
- **匹配规则**：
  1. 获取本地仓库远程地址
  2. 从远程地址提取仓库名
  3. 在配置中查找 repoName 是否匹配
  4. 如果匹配成功，该仓库 docs 目录下 excludeDocsPaths 中列出的子路径跳过翻译

> 详细检查方法见 [Q3: 如何检查排除列表](#q3-如何检查排除列表)

### 翻译标准文档
- **主文档**：https://developers.google.com/style

## 目录结构示例

```
仓库根目录/
├── README.md                  # 原始中文文档（非docs）
├── README_en.md               # 翻译后英文文档（同级目录）
├── CONTRIBUTING.md            # 原始中文文档（非docs）
├── CONTRIBUTING_en.md         # 翻译后英文文档（同级目录）
├── docs/                      # docs目录
│   ├── README.md              # 原始文档（docs根目录下的README.md）
│   ├── README_en.md           # 翻译后英文文档（同级目录）
│   ├── api/
│   │   ├── README.md          # 原始文档（docs子目录下的README.md）
│   │   └── quantize.md        # 原始文档
│   ├── build.md               # 原始文档
│   ├── guide/
│   │   └── install.md         # 原始文档
│   ├── public/                # 排除列表（公开产品文档，不翻译）
│   │   └── guide.md
│   └── en/                    # 翻译文件统一存放目录（保持原有目录层级）
│       ├── api/
│       │   ├── README_en.md   # 翻译后英文文档
│       │   └── quantize_en.md # 翻译后英文文档
│       ├── build_en.md        # 翻译后英文文档
│       └── guide/
│           └── install_en.md  # 翻译后英文文档
├── examples/
│   └── README.md              # 原始中文文档（非docs）
│   └── README_en.md           # 翻译后英文文档（同级目录）
└── npu_ops/
    └── README.md              # 原始中文文档（非docs）
    └── README_en.md           # 翻译后英文文档（同级目录）
```

## 工具使用建议

### 并行操作提高效率
- 并行读取多个相关文件（中文原文、英文版本、风格指南等）
- 并行执行多个bash命令获取不同信息

## 注意事项

1. **扫描范围与统计（重要）**：
   - 扫描所有md文档，包括docs目录和非docs目录
   - **必须先统计排除路径文件数量**，再计算实际需翻译数量
   - 向用户输出清晰的统计信息（总数、排除数、实际需翻译数）
   - 避免误导用户认为需要翻译所有文件
2. **排除列表判断**：docs目录下的文档需要检查是否在排除列表中
3. **必须先读取翻译标准**：翻译任何内容前，必须先读取英文风格指南
4. **文件存放路径**：非docs目录与原文件同级；docs根目录下的README.md与原文件同级；docs目录其他文档存放在`docs/en/`下保持原有层级
5. **文件命名规则**：翻译文件统一命名为 `<原文件名>_en.md`
6. **增量更新**：已存在翻译文件时，仅翻译变更部分，避免重复翻译
7. **格式一致性**：保持Markdown格式、表格结构、链接引用等一致
8. **提交变更**：翻译完成后必须将所有变更的中英文md文档提交到远程仓库

## 常见问题处理

### Q1: 如何判断文档是否需要翻译

**判断流程：**

```
1. 文档路径是否以docs/开头？
   → 否：需要翻译（非docs目录文档）
   → 是：继续检查排除列表

2. 从本地仓库远程地址提取仓库名
   → 示例：https://gitcode.com/sophia1213/ge_en_test.git → 提取仓库名 "ge"

3. 在exclude_docs.json中查找repoName是否匹配
   → 不匹配：需要翻译（该仓库不在排除列表中）
   → 匹配：继续检查文档路径是否在excludeDocsPaths中

4. 文档路径是否在excludeDocsPaths列表中？
   → 是：跳过（该路径的文档在公开仓库维护，不需要翻译）
   → 否：需要翻译

5. 是否存在对应的翻译文件？
   → 否：需要完整翻译
   → 是：检查是否有变更

6. 文档是否有变更？
   → 是：需要增量翻译更新
   → 否：跳过
```

**检查命令：**
```bash
# 1. 统计总md文档数量（排除.git目录）
find . -name "*.md" -not -path "./.git/*" -type f | wc -l

# 2. 统计排除路径文件数量（重要：避免误导用户）
# 统计单个排除路径
find . -name "*.md" -not -path "./.git/*" -path "*/docs/dflow_api/*" | wc -l

# 统计所有排除路径
find . -name "*.md" -not -path "./.git/*" \
  -not -path "*/docs/dflow_api/*" \
  -not -path "*/docs/graph_engine_api/*" \
  -not -path "*/docs/llm_datadist_api/*" | wc -l

# 3. 检查文档是否在docs目录
echo $filepath | grep "^./docs/"

# 4. 获取本地仓库远程地址
git remote -v

# 5. 读取排除列表配置
current_path=$(pwd) 
cat ${current_path}\exclude_docs.json
```

**示例场景：**

| 本地仓库远程地址 | 提取仓库名 | 配置中repoName匹配 | docs路径 | excludeDocsPaths | 是否翻译 |
|-----------------|-----------|------------------|---------|-----------------|---------|
| `https://gitcode.com/sophia1213/ge.git` | ge | 匹配 | `docs/dflow_api/guide.md` | `["docs/dflow_api", "docs/graph_engine_api"]` | 跳过（在排除路径） |
| `https://gitcode.com/sophia1213/ge.git` | ge | 匹配 | `docs/architecture/feature.md` | `["docs/dflow_api", "docs/graph_engine_api"]` | 翻译（不在排除路径） |
| `https://gitcode.com/user/pypto.git` | pypto | 匹配 | `docs/api/guide.md` | `["docs"]` | 跳过（整个docs目录排除） |
| `https://gitcode.com/user/custom.git` | custom | 不匹配 | `docs/any.md` | - | 翻译（不在排除列表） |

### Q2: 如何确定翻译文件存放路径

**规则：**
- **非docs目录**：翻译文件与原文件在同一目录
- **docs根目录下的README.md**：翻译文件与原文件在同一目录
- **docs目录下的其他文档**：翻译文件存放在`docs/en/`目录下，保持原有目录层级结构
- **PR模板文件**：翻译文件与原文件在同一目录，语言代码替换（如`.zh-CN.md` → `.en-US.md`）

**示例：**

| 文档类型 | 原文档路径 | 翻译后英文文档路径 |
|---------|-----------|------------------|
| 非docs目录 | `README.md` | `README_en.md` |
| 非docs目录 | `examples/README.md` | `examples/README_en.md` |
| docs根目录README.md | `docs/README.md` | `docs/README_en.md` |
| docs子目录README.md | `docs/api/README.md` | `docs/en/api/README_en.md` |
| docs目录其他文档 | `docs/api/quantize.md` | `docs/en/api/quantize_en.md` |
| docs目录其他文档 | `docs/guide/install.md` | `docs/en/guide/install_en.md` |
| docs目录其他文档 | `docs/build.md` | `docs/en/build_en.md` |
| PR模板文件 | `.gitcode/PULL_REQUEST_TEMPLATE.zh-CN.md` | `.gitcode/PULL_REQUEST_TEMPLATE.en-US.md` |
| Issue模板 | `.gitcode/ISSUE_TEMPLATE.zh-CN.md` | `.gitcode/ISSUE_TEMPLATE.en-US.md` |

**为什么docs根目录下的README.md存放在同级目录？**
- docs/README.md是docs目录的入口文档，需要与原文在同一目录便于访问
- docs子目录下的README.md和其他文档统一存放在docs/en/目录下，便于统一管理
- 保持目录结构清晰，避免混淆原文和译文

### Q3: 如何检查排除列表

**检查步骤：**

1. **读取排除列表配置文件**
```bash
current_path=$(pwd) 
cat ${current_path}\exclude_docs.json
```

2. **获取本地仓库的远程地址并提取仓库名**
```bash
# 获取远程地址
git remote -v

# 提取仓库名示例：
# https://gitcode.com/sophia1213/ge_en_test.git → ge
# https://gitcode.com/cann/pypto.git → pypto
# https://gitcode.com/user/custom_repo.git → custom_repo
```

3. **匹配逻辑**
   - 在配置文件的 mapping 数组中查找 repoName 字段
   - 如果 repoName 与本地仓库名匹配：
     - 检查文档路径是否在 excludeDocsPaths 列表中
     - 在排除路径：跳过翻译
     - 不在排除路径：需要翻译
   - 如果 repoName 不匹配：需要翻译（该仓库的所有文档）

**配置文件格式：**
```json
{
    "mapping": [
        {
            "repoName": "ge",
            "publicRepo": "https://gitcode.com/cann/ge",
            "excludeDocsPaths": ["docs/dflow_api", "docs/graph_engine_api", "docs/llm_datadist_api"]
        },
        {
            "repoName": "pypto",
            "publicRepo": "https://gitcode.com/cann/pypto",
            "excludeDocsPaths": ["docs"]
        }
    ]
}
```

**匹配示例：**

| 本地仓库名 | 配置repoName | excludeDocsPaths | docs文档路径 | 是否跳过 |
|-----------|-------------|-----------------|------------|---------|
| ge | ge | `["docs/dflow_api"]` | `docs/dflow_api/cpp/AddInvokedClosure.md` | 跳过 |
| ge | ge | `["docs/dflow_api"]` | `docs/architecture/architecture.md` | 翻译 |
| pypto | pypto | `["docs"]` | `docs/any/path.md` | 跳过（整个docs排除） |
| custom | 不匹配 | - | `docs/any.md` | 翻译 |

**代码实现示例：**
```bash
# 从远程地址提取仓库名
remote_url=$(git remote get-url origin)
repo_name=$(basename "$remote_url" .git)

# 检查是否在排除列表
# 需要解析JSON配置并匹配repoName
```

### Q4: 如何检查是否存在翻译文件

**非docs目录文档：**
```bash
# 检查同级目录下是否存在_en.md文件
ls README_en.md
ls examples/README_en.md
ls npu_ops/guide_en.md
```

**docs根目录下的README.md文档：**
```bash
# 检查同级目录下是否存在_en.md文件
ls docs/README_en.md
```

**docs目录下的其他文档（包括子目录README.md）：**
```bash
# 检查docs/en/目录下是否存在翻译文件（保持原有目录层级）
ls docs/en/api/README_en.md
ls docs/en/api/quantize_en.md
ls docs/en/guide/install_en.md
ls docs/en/build_en.md
```

**检查逻辑：**
```bash
# 非docs目录：检查同级目录
ls <文件所在目录>/<文件名（不含扩展名）>_en.md

# docs根目录下的README.md：检查同级目录
ls docs/README_en.md

# docs目录下的其他文档（包括子目录README.md）：检查docs/en/目录下的对应目录层级
ls docs/en/<原有目录路径>/<文件名（不含扩展名）>_en.md
```

### Q5: 如何检测文档变更

**方式1：与上一次提交比较**
```bash
git diff HEAD~1 -- README.md
git diff HEAD~1 -- docs/api/quantize.md
```

**方式2：检查工作区状态**
```bash
git status README.md
git status docs/api/quantize.md
```

**方式3：获取变更统计**
```bash
git diff HEAD~1 --stat -- README.md
git diff HEAD~1 --stat -- docs/api/quantize.md
```

### Q6: 翻译时保留中文链接

对于引用其他中文文档的链接，保持原链接路径不变，待对应文档翻译后再更新。

### Q7: 为什么必须先读取翻译标准

**不读取翻译标准会导致以下问题：**

1. **被动语态滥用**：中文常见"面向..."、"设计用于..."等表达，直接翻译为"Designed for..."是被动语态，应改为主动语态"This guide provides..."

2. **外来语使用**：中文常用"等"、"例如"、"即"，直接翻译为etc、e.g.、i.e.是外来语，应改为and so on、for example、that is

3. **缩写形式**：翻译中使用can't、it's等缩写，应改为cannot、it is

4. **时态错误**：操作步骤后的结果使用将来时"will appear"，应改为一般现在时"appears"

**后果：** 翻译完成后发现不符合规范，需要返工修正，浪费时间。因此必须在翻译前先读取标准。

### Q8: 如何处理已存在的翻译文件

**处理流程：**
1. 检查原文档是否有变更
2. 如有变更，分析变更内容
3. 仅翻译变更部分
4. 根据文档路径类型，更新对应的翻译文件：
   - **非docs目录**：使用edit工具更新同级目录的翻译文件
   - **docs目录**：使用edit工具更新docs/en/目录下对应目录层级的翻译文件

**增量翻译示例：**
```bash
# 1. 获取变更内容
git diff HEAD~1 -- README.md

# 2. 分析变更点
# 3. 翻译变更部分
# 4. 使用edit工具更新翻译文件
```

### Q9: 如何提交翻译变更到远程仓库

**提交流程：**

1. **查看所有变更的md文档**
```bash
# 查看所有变更文件
git status

# 查看变更的md文档（包括中文原文和英文翻译）
git status -- "*.md"
```

2. **添加变更文件到暂存区**
```bash
# 方式1：添加所有md文档变更
git add "*.md"

# 方式2：添加特定文件
git add README.md README_en.md
git add docs/api/quantize.md docs/en/api/quantize_en.md
```

3. **创建提交记录**
```bash
# 创建提交（推荐提交信息格式）
git commit -m "docs: add/update English translation for markdown files"

# 或更详细的提交信息
git commit -m "docs: translate README.md to README_en.md

- Add README_en.md (English translation)
- Update docs/api/quantize.md and docs/en/api/quantize_en.md"
```

4. **推送到远程仓库**
```bash
# 推送到远程仓库
git push origin <branch_name>

# 如果是新分支，使用 -u 参数
git push -u origin <branch_name>
```

**完整提交示例：**
```bash
# 1. 查看变更
git status

# 2. 添加所有md文件变更
git add "*.md"

# 3. 创建提交
git commit -m "docs: add English translation for markdown files"

# 4. 推送到远程
git push origin main
```

**注意事项：**
- 提交前确保所有翻译文件都已创建或更新
- 提交信息应清晰说明翻译的内容
- 中文原文和英文翻译应一起提交，保持同步
- 如有多个翻译文件，建议批量提交
