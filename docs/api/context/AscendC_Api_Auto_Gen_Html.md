# Ascend C API 参考 —— 静态文档构建指南

## 目录

- [1. 文件清单](#1-文件清单)
- [2. 环境准备](#2-环境准备)
- [3. 构建步骤](#3-构建步骤)
- [4. 功能说明](#4-功能说明)
- [5. 故障排除](#5-故障排除)

---

## 1. 文件清单

构建本项目需要以下文件，缺一不可。

```
context/
├── conf.py                          # Sphinx 主配置
├── _toc.yml                         # 目录树定义
├── index.rst                        # 首页
├── *.md                             # 源文档（约 1709 个）
│
├── _templates/
│   ├── layout.html                  # 自定义布局（注入版本数据 + 下载按钮）
│   ├── version-filter.html          # 版本筛选下拉框
│   └── download-button.html         # 浮动下载按钮
│
├── _static/
│   ├── css/
│   │   └── custom.css               # 自定义样式
│   ├── js/
│   │   └── version-filter.js        # 版本筛选核心逻辑
│   ├── version_filter.json          # 950 产品支持映射表
│   └── switcher.json                # 主题版本切换器配置
│
├── extract_product_support.py       # 【工具】扫描源文档生成 version_filter.json
├── generate_zips.py                 # 【工具】构建后生成下载 zip 包
│
├── figures/                         # 图片资源（约 761 个 PNG/JPG/SVG）
├── c_api/                           # 子目录源文件（通过 include 嵌入）
└── tensor_api/                      # 子目录源文件（通过 include 嵌入）
```

### 各文件作用详解

| 文件 | 类别 | 说明 |
|---|---|---|
| `conf.py` | 构建配置 | Sphinx 入口配置。定义主题 (`pydata_sphinx_theme`)、插件 (`myst_parser`, `sphinx_external_toc`)、导航栏布局、版本数据注入、构建后钩子（自动调用 `generate_zips.py`） |
| `_toc.yml` | 构建配置 | 全局目录树（YAML 格式），定义所有页面的层级导航结构。`sphinx_external_toc` 扩展读取此文件生成侧边栏和导航栏 |
| `index.rst` | 内容 | 站点首页 |
| `*.md` | 内容 | 核心内容文件。每文件对应一个独立页面。文件中 `## 产品支持情况` 章节的 HTML 表格用于判断 950 版本支持情况 |
| `figures/` | 资源 | 文档引用的图片，构建时自动拷贝到 `_build/html/_images/` |
| `c_api/` `tensor_api/` | 子文档 | 被根级 `.md` 文件通过 `include` 或 `toctree` 指令嵌入的章节文件，不单独生成 HTML 页面 |
| `_templates/layout.html` | 模板 | 继承 PyData 主题的 `!layout.html`，在 `<head>` 注入 `VERSION_950_HIDDEN` 全局变量，在 `</body>` 前插入下载按钮 |
| `_templates/version-filter.html` | 模板 | 版本下拉框组件，渲染于导航栏右侧和侧边栏（PyData 主题自动渲染两处） |
| `_templates/download-button.html` | 模板 | 右下角浮动下载按钮，默认指向 `download_all.zip` |
| `_static/css/custom.css` | 样式 | 导航栏弹性布局、侧边栏美化、内容表格/代码块样式、下载按钮浮动样式 |
| `_static/js/version-filter.js` | 逻辑 | 版本筛选全部前端逻辑：侧边栏过滤、产品支持表格行过滤、下拉切换、下载按钮同步、localStorage 持久化 |
| `_static/version_filter.json` | 数据 | 预计算的 1710 条 URL 编码 key → `true`/`false` 映射，由 `extract_product_support.py` 生成 |
| `_static/switcher.json` | 配置 | PyData 主题版本切换器数据 |
| `extract_product_support.py` | 工具 | 批量解析所有根级 `.md` 文件中的 `## 产品支持情况` 表格，判断 Ascend 950PR/950DT 是否支持，输出 `version_filter.json` |
| `generate_zips.py` | 工具 | 读取 `version_filter.json`，生成 `download_all.zip`（全量源文件）和 `download_950.zip`（仅 950 支持源文件） |

---

## 2. 环境准备

### 2.1 Python 版本要求

- **Python 3.9+**（推荐 3.10 或更高）

### 2.2 依赖安装

#### Linux

```bash
# 创建虚拟环境（推荐）
python3 -m venv venv
source venv/bin/activate

# 安装依赖
pip install sphinx==9.1.0
pip install myst-parser==5.0.0
pip install sphinx-external-toc==1.1.0
pip install pydata-sphinx-theme==0.17.1
```

或使用一条命令：

```bash
pip install sphinx==9.1.0 myst-parser==5.0.0 sphinx-external-toc==1.1.0 pydata-sphinx-theme==0.17.1
```

#### Windows (PowerShell)

```powershell
# 创建虚拟环境（推荐）
python -m venv venv
.\venv\Scripts\Activate.ps1

# 安装依赖
pip install sphinx==9.1.0 myst-parser==5.0.0 sphinx-external-toc==1.1.0 pydata-sphinx-theme==0.17.1
```

> **注意：** 如果遇到 PowerShell 执行策略限制，先运行：
> ```powershell
> Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
> ```

### 2.3 验证安装

```bash
python -c "import sphinx; print(sphinx.__version__)"
# 应输出: 9.1.0
```

---

## 3. 构建步骤

> **前提：** 以下命令均在项目根目录 `context/` 下执行。
>
> **重要：** 每次源文件（`.md`/`.rst`）变更后，建议按以下完整流程重新构建。

### 3.1 完整构建流程（三步）

```bash
# ─── 步骤 1: 扫描产品支持情况，生成 version_filter.json ───
#
# 说明：解析所有根级 .md 文件中 "## 产品支持情况" 下的 HTML 表格，
#       判断 Ascend 950PR/Ascend 950DT 行是否标记为 √，
#       输出到 _static/version_filter.json
#       约需 2-5 分钟（取决于文件数量）

python extract_product_support.py

# ─── 步骤 2: Sphinx 构建静态站点 ───
#
# 说明：将 .md/.rst 源文件编译为 HTML，
#       应用 PyData 主题、自定义模板和样式，
#       输出到 _build/html/
#       约需 5-10 分钟（取决于文件数量）

# Linux
sphinx-build -b html -j auto . _build/html

# Windows (如果 sphinx-build 不在 PATH 中)
python -m sphinx -b html -j auto . _build/html

# ─── 步骤 3: 生成下载 zip 包（自动执行） ───
#
# conf.py 中已配置 build-finished 钩子，Sphinx 构建完成后会自动调用 generate_zips.py。
# 无需手动执行此步骤。
#
# 如需手动重新生成 zip，可单独运行：
#   python generate_zips.py
```

### 3.2 各平台完整命令

#### Linux

```bash
cd /path/to/context

# 完整构建（三合一脚本）
python extract_product_support.py && \
sphinx-build -b html -j auto . _build/html
```

#### Windows (PowerShell)

```powershell
cd C:\path\to\context

# 完整构建
python extract_product_support.py
if ($?) { python -m sphinx -b html -j auto . _build/html }
```

### 3.3 只重新生成 zip 包（不重建 HTML）

如果只修改了 `generate_zips.py` 或想刷新下载包，无需重新构建整个站点：

```bash
python generate_zips.py
```

### 3.4 构建输出说明

构建成功后，所有产物位于 `_build/html/`：

```
_build/html/
├── index.html                      # 首页
├── *.html                          # 所有内容页面（约 1616 个）
├── download_all.zip                # 全量源文件下载（.md + .rst）
├── download_950.zip                # 仅 950 支持的源文件下载
├── _static/
│   ├── css/                        # 主题样式 + custom.css
│   ├── js/                         # 主题脚本 + version-filter.js
│   ├── version_filter.json         # 950 支持映射表
│   └── switcher.json               # 版本切换器配置
├── _images/                        # 图片资源
└── _sources/                       # 源文件文本副本
```

---

## 4. 功能说明

### 4.1 版本筛选

站点提供两个版本选项：

| 版本 | 显示范围 | 侧边栏 | 页面内产品支持表 |
|---|---|---|---|
| **全量** | 全部页面 | 显示全部导航项 | 显示全部产品行 |
| **950** | 仅 Ascend 950PR/950DT 支持的页面 | 隐藏不支持页面的导航项 | 仅显示 `Ascend 950PR/Ascend 950DT` 行 |

**切换方式：**

- 点击导航栏右侧或侧边栏的版本下拉框
- 选择 "全量" 或 "950"
- 选择状态通过 `localStorage` 持久化，刷新页面后保持选择

**筛选机制：**

```
源文档 "产品支持情况" 表格
        │
        v
extract_product_support.py  【构建时运行一次】
        │ 解析每个 .md → 判断 950 行是否有 √
        v
_static/version_filter.json  【自动生成】
        │
        v
conf.py  【构建时读取】
        │ 提取 unsupported 条目 → VERSION_950_HIDDEN 变量
        v
每页 HTML <script>  【构建时注入】
        │
        v
version-filter.js  【浏览器运行时】
        ├── 侧边栏过滤: 隐藏不支持页面的导航项
        ├── 表格过滤: 隐藏非 950 的产品行
        └── 下载同步: 切换下载包链接
```

### 4.2 源文件下载

页面右下角有浮动下载按钮：

| 当前版本 | 下载链接 | 内容 |
|---|---|---|
| 全量 | `download_all.zip` | 全部 1565 个源文件（`.md` + `.rst`） |
| 950 | `download_950.zip` | 仅 950 支持的源文件（约 1565 个） |

下载可离线使用——zip 内为原始 Markdown/RST 源文件。

### 4.3 离线使用

`_build/html/` 目录为完全静态站点，无需服务器即可浏览：

```bash
# Linux
xdg-open _build/html/index.html

# Windows
start _build/html/index.html
```

所有资源使用相对路径，支持 `file://` 协议直接打开。

### 4.4 定制版本

**添加新版本（例如 "910"）：**

1. 修改 `conf.py`，在 `html_context['versions']` 中添加新条目：
   ```python
   html_context = {
       'versions': [
           ('全量', '全量 (当前版本)'),
           ('950', '950'),
           ('910', '910'),          # ← 新增
       ],
   }
   ```

2. 修改 `_static/switcher.json`，添加对应条目：
   ```json
   {"name": "910", "version": "910", "url": "#"}
   ```

3. 修改 `extract_product_support.py`，增加对新产品的支持判断逻辑。

4. 修改 `generate_zips.py`，增加对应的 zip 生成逻辑。

5. 重新执行三步构建流程。

---

## 5. 故障排除

### 5.1 `ModuleNotFoundError: No module named 'sphinx'`

**原因：** Sphinx 未安装或虚拟环境未激活。

**解决：**
```bash
pip install sphinx==9.1.0 myst-parser==5.0.0 sphinx-external-toc==1.1.0 pydata-sphinx-theme==0.17.1
```

### 5.2 `sphinx-build: command not found`（Linux）/ 不识别（Windows）

**原因：** `sphinx-build` 不在 PATH 中。

**解决：** 使用 Python 模块方式运行：
```bash
python -m sphinx -b html . _build/html
```

### 5.3 `WARNING: local id not found in doc ...`

**原因：** 源文档中的交叉引用目标不存在。这是预存的内容问题，不影响页面显示。

**解决：** 可忽略。如果数量较多想隐藏，去掉 `sphinx-build` 命令的 `-W` 参数（当前未使用）。

### 5.4 `FileNotFoundError: _static/version_filter.json`

**原因：** 未先运行 `extract_product_support.py`。

**解决：** 先执行 `python extract_product_support.py`，再执行 `sphinx-build`。

### 5.5 下载按钮不显示或下载链接 404

**原因：** `generate_zips.py` 未执行或执行失败。

**解决：**
```bash
# 手动运行并检查输出
python generate_zips.py

# 验证产物
ls -lh _build/html/download_*.zip    # Linux
dir _build\html\download_*.zip       # Windows
```

### 5.6 页面空白或样式错乱

**原因：** 静态资源路径问题，通常出现在移动构建目录后。

**解决：** 不要移动 `_build/html/` 内的子目录结构。整个 `_build/html/` 目录应当保持完整。如果必须移动，将整个 `_build/html/` 目录复制到目标位置。

### 5.7 构建时内存不足

**原因：** 源文件数量较多（1700+），并行构建消耗内存。

**解决：** 减少并行度或使用单线程：
```bash
# 单线程构建（更慢但更省内存）
sphinx-build -b html . _build/html

# 或限制并行数
sphinx-build -b html -j 2 . _build/html
```
