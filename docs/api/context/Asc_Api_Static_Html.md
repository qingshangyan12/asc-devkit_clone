# Ascend C API 静态网页部署说明

将 `docs/api/context/` 下的 Markdown 源文件生成为可浏览的静态 HTML 文档。

---

## 目录结构

```
docs/api/context/
├── conf.py               Sphinx 配置文件
├── _toc.yml              目录结构定义
├── index.rst             首页
├── *.md (~1700 个)       API 文档源文件
├── figures/              图片资源 (761 个)
├── _static/css/          自定义样式
├── tensor_api/           额外文档
├── c_api/                额外文档
└── _build/html/          构建产物（生成后出现）
```

## 环境要求

- Python 3.8 或更高版本
- pip 包管理器

---

## 安装第三方包

在终端中执行：

```bash
pip install sphinx myst-parser sphinx-external-toc sphinx-rtd-theme
```

### 各包作用

| 包名 | 用途 |
|---|---|
| `sphinx` | 核心文档生成引擎 |
| `myst-parser` | 支持 Markdown（.md）文件作为源文件 |
| `sphinx-external-toc` | 支持 `_toc.yml` 外部目录结构 |
| `sphinx-rtd-theme` | Read the Docs 风格的 HTML 主题 |

### 验证安装

```bash
python -m sphinx --version
```

如果输出版本号，说明安装成功。

---

## 构建静态网页

在包含 `conf.py` 的目录下执行：

```bash
cd docs/api/context
sphinx-build -b html . _build/html
```

或者使用 python 模块方式（推荐，避免环境变量问题）：

```bash
cd docs/api/context
python -m sphinx -b html . _build/html
```

> **注意**：源文件约 1700 个，首次构建可能需要 3-10 分钟。

构建成功后，末尾会显示：
```
build succeeded.
The HTML pages are in _build\html.
```

---

## 各平台详细说明

### Windows

**安装依赖：**

```powershell
# 1. 安装 Python（若未安装）
# 从 https://www.python.org/downloads/ 下载安装包
# 安装时勾选 "Add Python to PATH"

# 2. 安装 Sphinx 及相关包
pip install sphinx myst-parser sphinx-external-toc sphinx-rtd-theme

# 3. 验证
python -m sphinx --version
```

**构建：**

```powershell
# 进入 context 目录
cd C:\path\to\asc-devkit\docs\api\context

# 构建
python -m sphinx -b html . _build/html
```

**查看结果：** 用浏览器打开 `_build/html/index.html`

---

### macOS / Linux

**安装依赖：**

```bash
# 1. 确认 Python 已安装
python3 --version

# 2. 安装 Sphinx 及相关包
pip3 install sphinx myst-parser sphinx-external-toc sphinx-rtd-theme

# 3. 验证
python3 -m sphinx --version
```

**构建：**

```bash
# 进入 context 目录
cd /path/to/asc-devkit/docs/api/context

# 构建
python3 -m sphinx -b html . _build/html
```

**查看结果：** 用浏览器打开 `_build/html/index.html`

---

## 常用命令

### 增量构建（修改少量文件后）

```bash
python -m sphinx -b html . _build/html
```

Sphinx 会自动检测变更，只重新生成修改过的页面，速度很快。

### 强制完全重新构建

```bash
python -m sphinx -b html -E . _build/html
```

`-E` 参数忽略缓存，强制重新生成所有页面。

### 清理构建产物

手动删除目录：

```bash
# Windows
rmdir /s _build

# macOS / Linux
rm -rf _build
```

---

## 常见问题

### 构建出现 WARNING，是否正常？

正常。`image not readable` 等 WARNING 表示某些图片文件缺失或路径不正确，不影响 HTML 页面生成和浏览。

### 构建速度太慢

- 首次构建因为文件较多（~1700 个）需要几分钟。
- 后续增量构建只重新生成修改过的文件，会快很多。
- 可使用 `-j auto` 参数启用多核并行构建加速：

```bash
python -m sphinx -b html -j auto . _build/html
```

### 页面标题显示 "Project name not set"

需在 `conf.py` 中添加 `project` 变量：

```python
project = 'Ascend C API 参考'
```

然后重新构建。

---

## 最终结果

构建完成后，`_build/html/` 目录包含完整的静态网站，可直接部署到任意 HTTP 服务器（Nginx、Apache、GitHub Pages 等），或本地浏览器直接打开 `index.html` 浏览。

```
_build/html/
├── index.html              首页
├── genindex.html           索引页
├── search.html             搜索页
├── *.html (~1623 个)       各 API 页面
├── _static/                静态资源（CSS、JS、字体）
├── _images/                图片资源
└── _sources/               源文件副本
```
