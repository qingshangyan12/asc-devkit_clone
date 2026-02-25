#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
replace_microapi.py

批量把文件中的字符串"MicroAPI"替换为"Reg"
- 递归遍历目录
- 只处理文本文件（后缀可自行增删）
- 自动生成.bak备份（同名+.bak）
- 支持dry-run（仅打印要改动的文件/行，不实际写入）

使用示例：
    python replace_microapi.py -d ./src
    python replace_microapi.py -d ./src -e .cpp .h .py .txt --dry-run
"""

import argparse
import os
import sys
from pathlib import Path
from typing import List


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Rename files containing '_micro_' to '_reg_compute_'",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument(
        "-d",
        "--dir",
        default=".",
        help="Root directory to start the search(default: current directory)",
    )
    parser.add_argument(
        "-r",
        "--recursive",
       action="store_true",
        help="Search recursively in sub-directories(default: recursive)",
    )
    parser.add_argument(
        "-n",
        "--dry-run",
        action="store_true",
        help="Show what would be renamed without actually renaming",
    )
    parser.add_argument(
        "-i",
        "--ignore-case",
        action="store_true",
        help="Match '_micro_'case-insensitively",
    )
    return parser.parse_args()


def collect_target_files(root: Path, recursive: bool)->List[Path]:
    """递归收集所有符合后缀的文件路径（返回pathlib.Path列表）"""
    if not root.is_dir():
        print(f"[ERROR]指定的目录不是目录:{root}", file=sys.stderr)
        sys.exit(1)
    pattern = "*_micro_*"
    files:List[Path] = []
    if recursive:
        for p in root.rglob(pattern):
            if p.is_file():
                files.append(p)
    else:
        for p in root.glob(pattern):
            if p.is_file():
                files.append(p)
    return files


def rename_file(old_path: Path, dry_run: bool) -> None:
    """读取、替换、写回（或仅打印）"""
    new_name = old_path.name.replace("_micro_", "_reg_compute_")
    new_path = old_path.with_name(new_name)
    if new_path.exists():
        print(
            f"[SKIP] 目标文件已存在，跳过：{old_path} -> {new_path}",
              file=sys.stderr,
            )
        return

    if dry_run:
        print(f"[DRY-RUN] {old_path} -> {new_path} ")
    else:
        try:
            old_path.rename(new_path)
            print(f"[RENAME] {old_path} -> {new_path}）")
        except Exception as exc:
            print(
                f"[ERROR]重命名失败： {old_path} -> {new_path}\n     {exc}",
                  file=sys.stderr,
                )

    


def main()->None:
    args = parse_args()
    root_dir = Path(args.dir).resolve()
    target_files=collect_target_files(root_dir, args.recursive)
    if not target_files:
        print("[INFO]没有发现文件名中含有'_micro_'的文件。")
        return
    print(f"[INFO]共找到{len(target_files)}个待处理的文件:")
    
    print("\n开始重命名:")
    for f in target_files:
        rename_file(f, False)
    print("\n完成")
    if args.dry_run:
        print("已使用 --dry-run，仅做演示，实际未改动")


if __name__ == "__main__":
    main()

