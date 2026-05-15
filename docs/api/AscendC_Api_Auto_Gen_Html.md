# Ascend C API 参考 —— 静态网页自动生成指南

## 目录结构

```
docs/api/
  conf.py                          Sphinx 配置（主题、扩展、构建钩子）
  index.rst                        首页入口
  README.md                        侧边栏目录的权威定义（约 2200 行）
  _toc.yml                         运行时 TOC 文件（Sphinx 侧边栏数据源）
  _toc.yml.new                     _gen_toc.py 生成（复制为 _toc.yml 后使用）
  AscendC_Api_Auto_Gen_Html.md     本指南

  _gen_toc.py                      从 README.md 生成 TOC 文件（×3 输出）
  _verify_toc.py                   验证 _toc.yml 引用的源文件是否存在
  _verify_html.py                  构建后校验（残留 .md 链接、空段、转义符、图片引用）
  generate_zips.py                 构建后处理：修复 .md 链接 + 生成下载 zip

  _static/
    css/custom.css                 自定义样式
    js/toc-data.js                 内嵌 TOC 数据（JS 变量，避免 fetch，支持 file://）
    js/toc-toggle.js               侧边栏 SPA 导航（分区拦截、AJAX 加载、动态重建目录）
    js/version-filter.js           版本筛选（全量 / 950 切换，侧边栏重建后重新筛选）
    toc_data.json                  TOC 数据 JSON（HTTP 下 fetch 备用，file:// 不依赖）
    switcher.json                  版本切换器菜单
    version_filter.json            每页面是否支持 950 版本的映射表

  _templates/
    layout.html                    基础布局（注入变量 + cache-busting + 下载按钮）
    download-button.html           浮动下载按钮
    version-filter.html            版本筛选下拉组件

  附录/                            附录 API 源文件（.md）
  AI-CPU-API/                      AI CPU API 源文件（.md）
  SIMD-API/                        SIMD API 源文件（.md）
  SIMT-API/                        SIMT API 源文件（.md）
  Utils-API/                       Utils API 源文件（.md）
  tensor_api/                      Tensor API 源文件（.md）
  c_api/                           C API 源文件（.md）
  context/                         独立 Sphinx 子项目（单独构建）
  figures/                         文档插图（PNG）

  _build/html/                     构建输出（每个页面一个 .html 文件）
```

## 前置依赖

| 包 | 用途 |
|---|---|
| `sphinx` | 文档构建引擎 |
| `myst-parser` | 解析 Markdown → Sphinx AST |
| `sphinx-external-toc` | 读取 `_toc.yml` 生成侧边栏目录 |
| `pydata-sphinx-theme` | 页面主题（导航栏、侧边栏、搜索、深浅色切换） |
| `pyyaml` | `_gen_toc.py` 写 YAML 时使用 |

安装：

```bash
pip install sphinx myst-parser sphinx-external-toc pydata-sphinx-theme pyyaml
```

---

## 构建步骤

> 以下所有命令均在 `docs/api/` 目录下执行。

### 第一步：生成 TOC 文件

`_toc.yml` 是侧边栏的目录定义，由 `_gen_toc.py` 从 `README.md` 解析生成。

```bash
python _gen_toc.py
```

脚本行为：

1. 逐行解析 `README.md` 中的 `- [标题](链接)` 条目
2. 根据 4 空格缩进还原树形层级
3. 为每个条目标注独立编号（每节从 1 开始）
4. 自动清除 PyYAML 引入的反斜杠转义符（如 `\_` → `_`）
5. 跳过 .png / .jpg / .gif / .svg / .pdf 等图片链接

输出三个文件：

| 文件 | 大小 | 用途 |
|---|---|---|
| `_toc.yml.new` | ~330 KB | Sphinx 侧边栏目录定义（~2050 条目，6 个一级分区） |
| `_static/toc_data.json` | ~200 KB | 紧凑 TOC JSON（HTTP fetch 备用） |
| `_static/js/toc-data.js` | ~200 KB | 内嵌 TOC 数据（`var TOC_DATA_EMBEDDED = [...]`，优先使用） |

### 第二步：激活 TOC 文件

```bash
# Windows PowerShell
Copy-Item -LiteralPath "_toc.yml.new" -Destination "_toc.yml" -Force
```

```bash
# Linux / macOS
cp -f _toc.yml.new _toc.yml
```

> `_toc.yml.new` 是纯生成产物，`_toc.yml` 是运行文件。分离两者可在重新生成时先对比再覆盖。

### 第三步：（可选）验证 TOC 完整性

```bash
python _verify_toc.py
```

验证 `_toc.yml` 中引用的 `.md` 源文件是否存在，输出缺失文件列表。

> `conf.py` 中 `external_toc_exclude_missing = True`，缺失文件会被静默跳过不中断构建。此步骤帮助提前发现路径错误。

### 第四步：清理缓存并构建

**重要：TOC 变更后必须清理 Sphinx 缓存（doctree），否则侧边栏不会更新。**

```bash
# Windows PowerShell
Remove-Item -Recurse -Force "_build" -ErrorAction SilentlyContinue
sphinx-build -b html . _build/html
```

```bash
# Linux / macOS
rm -rf _build
sphinx-build -b html . _build/html
```

参数说明：

| 参数 | 含义 |
|---|---|
| `-b html` | 构建类型 html（每个页面输出为 `.html` 文件，支持 file:// 协议浏览） |
| `.` | 源文件目录 |
| `_build/html` | 输出目录 |

构建产生约 900+ 条 warning，均为源文件内容问题（缺失图片、内部交叉引用失效），不影响功能。

### 第五步：自动后处理

`conf.py` 中注册了 `on_build_finished` 钩子，构建完成后自动依次运行：

1. **`generate_zips.py`** — 修复 .md 链接 + 生成下载 zip
2. **`_verify_html.py`** — 校验构建产物完整性

**`generate_zips.py`** 完成两件事：

1. **修复 .md → .html 链接**：源文件中的 `<a href="../foo.md">` 经 myst-parser 保留原样，脚本将 `href="*.md"` 替换为 `href="*.html"`。约 3600+ 处替换，覆盖 500+ 个文件。

2. **生成下载 zip**：
   - `_build/html/download_all.zip` — 全部源文件（~2050 个 .md/.rst）
   - `_build/html/download_950.zip` — 仅 950 版本支持的文件（~1200 个），产品支持表格裁剪为 950 行

**`_verify_html.py`** 校验四项：

- 残留 `.md` 链接（验证 generate_zips.py 生效）
- 引用指向缺失图片
- 空 `<section>` 元素（可能表示 MyST 解析问题）
- 可见文本中出现反斜杠转义符 `\`

日志示例：

```
[md] 3637 .md→.html link(s) fixed in 502 file(s)
[zip] Building download_all.zip (2054 .md/.rst) ...
[zip] download_all.zip  (15.3 MB)
[zip] 950-supported source files: 1234
[zip] Building download_950.zip (950-only, tables filtered) ...
[zip] download_950.zip  (8.7 MB)
[zip] Done.
[verify] All checks passed.
```

> 如果后处理未自动执行，可手动运行：`python generate_zips.py` 和 `python _verify_html.py`

---

## 一键构建脚本

### Windows（PowerShell）

```powershell
# 保存到 docs/api/build.ps1，执行：.\build.ps1

$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

Write-Host "==> 1/3 生成 TOC ..." -ForegroundColor Cyan
python _gen_toc.py
if ($LASTEXITCODE -ne 0) { throw "_gen_toc.py failed" }

Write-Host "==> 2/3 激活 TOC & 清理缓存 ..." -ForegroundColor Cyan
Copy-Item -LiteralPath "_toc.yml.new" -Destination "_toc.yml" -Force
Remove-Item -Recurse -Force "_build" -ErrorAction SilentlyContinue

Write-Host "==> 3/3 构建 HTML（自动后处理） ..." -ForegroundColor Cyan
sphinx-build -b html . _build/html
if ($LASTEXITCODE -ne 0 -and $LASTEXITCODE -ne 1) { throw "sphinx-build failed" }

Write-Host "Done! 打开 _build/html/index.html 查看。" -ForegroundColor Green
```

### Linux / macOS（Bash）

```bash
#!/usr/bin/env bash
# 保存到 docs/api/build.sh，执行：bash build.sh

set -euo pipefail
cd "$(dirname "$0")"

echo -e "\033[36m==> 1/3 生成 TOC ...\033[0m"
python3 _gen_toc.py

echo -e "\033[36m==> 2/3 激活 TOC & 清理缓存 ...\033[0m"
cp -f _toc.yml.new _toc.yml
rm -rf _build

echo -e "\033[36m==> 3/3 构建 HTML（自动后处理） ...\033[0m"
sphinx-build -b html . _build/html

echo -e "\033[32mDone! 打开 _build/html/index.html 查看。\033[0m"
```

> 一键脚本已简化为 3 步：`generate_zips.py` 和 `_verify_html.py` 在 `conf.py` 的 `on_build_finished` 钩子中自动运行。

---

## 关键配置说明

### conf.py 核心项

| 配置项 | 值 | 说明 |
|---|---|---|
| `extensions` | `myst_parser`, `sphinx_external_toc` | Markdown 解析 + 外部 TOC |
| `myst_enable_extensions` | `['colon_fence', 'deflist', …]` | 启用 11 个常用 MyST 扩展（见下方） |
| `html_theme` | `pydata_sphinx_theme` | 页面主题 |
| `html_static_path` | `['_static']` | 静态资源目录 |
| `templates_path` | `['_templates']` | Jinja2 模板目录 |
| `html_theme_options.show_nav_level` | `2` | 原生 toctree 侧边栏深度（外部 TOC 实际控制深度） |
| `html_theme_options.navigation_depth` | `6` | 顶部导航栏深度 |
| `html_theme_options.collapse_navigation` | `True` | 侧边栏默认折叠 |
| `external_toc_exclude_missing` | `True` | TOC 中缺失的源文件不报错 |
| `exclude_patterns` | `['_build', 'README.md.orig', 'figures', '*.py', 'context', '.git']` | 排除目录/文件 |
| `source_suffix` | `{'.rst': ..., '.md': ...}` | 支持 .rst 和 .md 源文件 |
| `html_context.build_hash` | 8 位 MD5（每次构建变化） | 静态资源 cache-busting |
| `html_context.version_filter_hidden` | JSON 数组 | 950 版本不支持的页面路径 |

### MyST 扩展

```python
myst_enable_extensions = [
    'colon_fence',     # ::: 代码块
    'deflist',         # 定义列表
    'dollarmath',      # $LaTeX$ 数学公式
    'fieldlist',       # 字段列表
    'html_image',      # <img> 原样保留
    'linkify',         # URL 自动转链接
    'replacements',    # 字符替换 (c) → ©
    'smartquotes',     # 智能引号
    'strikethrough',   # ~~删除线~~
    'substitution',    # {{ 替换 }}
    'tasklist',        # - [ ] 任务列表
]
```

### JS / CSS 加载方式

`html_js_files` 和 `html_css_files` 在 `conf.py` 中设为空列表 `[]`。自定义 JS 和 CSS 改为在 `_templates/layout.html` 中手动引用，并附带 `?v={{ build_hash }}` 缓存破坏参数，避免浏览器缓存旧版本。

加载顺序（`layout.html` 中按以下顺序注入）：

```
custom.css  →  toc-data.js  →  version-filter.js  →  toc-toggle.js
 (样式)       (定义 TOC_DATA_EMBEDDED)   (定义 window._versionFilter)   (依赖前两者)
```

### 模板文件作用

| 文件 | 作用 |
|---|---|
| `_templates/layout.html` | 继承 pydata 主题布局。注入 `VERSION_950_HIDDEN`、以 `?v=` cache-busting 加载 custom.css / toc-data.js / version-filter.js / toc-toggle.js、引入 `download-button.html` |
| `_templates/version-filter.html` | 版本筛选下拉按钮 + 菜单（全量 / 950 切换） |
| `_templates/download-button.html` | 页面右下角浮动下载按钮（链接 `download_all.zip`） |

### 自定义 JS 功能

| 文件 | 功能 |
|---|---|
| `toc-data.js` | 声明 `var TOC_DATA_EMBEDDED = [...]`，完整 TOC 树。每页内嵌，file:// 下无需 fetch。 |
| `version-filter.js` | 版本筛选。监听点击切换 全量/950，筛选侧边栏条目和产品支持表格。暴露 `window._versionFilter.reapply()` 供 toc-toggle 调用。URL 编码自动解码对比。 |
| `toc-toggle.js` | SPA 分区切换。拦截分区标题点击 → AJAX 获取页面 → 替换内容区 → 重建侧边栏 → 更新 URL（history.pushState）。file:// 下禁用 AJAX 降级为整页刷新。支持 `<details>` 折叠展开、ARIA 无障碍、scroll-to-top。 |

### README.md 的 TOC 格式

`README.md` 是侧边栏顺序的**唯一权威来源**，格式如下：

```markdown
- [一级标题](路径/一级页面.md)
    - [二级标题](路径/二级页面.md)
        - [三级标题](路径/三级页面.md)
```

规则：

- 条目格式：`- [标题](链接)`，以 `.md` 结尾（构建后处理自动修正为 `.html`）
- 每级缩进 4 空格
- 图片链接（`.png`、`.jpg` 等）自动跳过
- 条目顺序即侧边栏顺序，不受文件系统扫描影响
- 编号独立：每个一级分区下从 1 开始编号
- **`_gen_toc.py` 使用相对路径**（`Path(__file__).resolve().parent`），可在任意环境运行

---

## 常见问题

### TOC 变更后侧边栏未更新

**必须先清理 Sphinx 缓存再重新构建**。doctree 缓存会保留旧 TOC 结构：

```bash
# Windows
Remove-Item -Recurse -Force "_build" -ErrorAction SilentlyContinue

# Linux
rm -rf _build
```

### 侧边栏标题出现反斜杠 `\`

如果看到 `operator\[\]`、`asc\_vf\_call` 等，重新运行 `_gen_toc.py`。脚本内 `_unescape_yaml()` 会自动清除 PyYAML 产生的多余转义符。

### 侧边栏链接 404

源文件 `.md` 中 `<a href="../foo.md">` 交叉引用在构建后仍以 `.md` 结尾。`generate_zips.py` 在第五步自动修复。运行 `python _verify_html.py` 可检测是否还有残留。

### 分区标题点击后侧边栏不刷新

1. 确认 `_static/js/toc-data.js` 存在（`_gen_toc.py` 生成）
2. 确认 `layout.html` 中 JS 按顺序加载：toc-data → version-filter → toc-toggle
3. 打开浏览器开发者工具，检查控制台是否有 JS 错误

### file:// 协议下点击分区标题无效

`toc-toggle.js` 检测 `window.location.protocol === 'file:'` 时禁用 AJAX，改用整页跳转。如需 AJAX 体验，请通过 HTTP 服务器打开：

```bash
# 在 _build/html/ 目录下
python -m http.server 8080
```

### 子章节链接点击后父节点折叠

已修复：`handleTocClick` 使用 `closest('li')` 检查 `has-children`，防止祖先节点拦截事件 + `stopImmediatePropagation`。

### 版本筛选在分区切换后失效

`toc-toggle.js` 在 `rebuildSidebar()` 和 popstate 后调用 `window._versionFilter.reapply()`，确保新建侧边栏条目应用当前筛选状态。`reapply()` 通过 `decodeURIComponent` 处理 URL 编码路径。

### 浏览器缓存旧版本 JS/CSS 导致异常

每次构建会产生新的 `build_hash`（MD5 时间戳前 8 位），JS/CSS 引用格式为 `toc-toggle.js?v=a1b2c3d4`，浏览器会重新加载。手动清除缓存也有效。

---

## 构建产物

构建完成后，入口文件为：

```
_build/html/index.html
```

整个 `_build/html/` 目录为完整静态网站，可直接部署到任意静态服务器（Nginx、Apache、GitHub Pages 等）。
