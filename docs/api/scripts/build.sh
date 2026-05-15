#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

echo -e "\033[36m==> 1/3 生成 TOC ...\033[0m"
python3 scripts/_gen_toc.py

echo -e "\033[36m==> 2/3 清理缓存 ...\033[0m"
rm -rf _build

echo -e "\033[36m==> 3/3 构建 HTML（自动后处理） ...\033[0m"
sphinx-build -b html -c scripts . _build/html

echo -e "\033[32mDone! 打开 _build/html/index.html 查看。\033[0m"
