#！/usr/bin/env python3
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
import patchlib
import shutil
import sys



#--------------------------------------------------------------
# 配置：默认要处理的文件后缀（可自行增删）
#--------------------------------------------------------------
DEFAULT_EXTS = [".c", ".cpp", ".h", ".py"]


def parse_args():
    parser = argparse.ArgumentParser(
        description="批量把MicroAPI替换为Reg",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument(
        "-d", "--dir",
        nargs="+",
        required=True,
        help="要搜索的根目录（可一次写多个）",
    )
    parser.add_argument(
        "-e", "--ext",
        nargs="+",
        default=DEFAULT_EXTS,
        help="要处理的文件后缀（包括点号），如.c .h .py",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="仅显示将要修改的文件/行，不实际写入",
    )
    parser.add_argument(
        "--bak-suffix",
        default=".bak",
        help="备份文件的后缀名（默认.bak）",
    )
    parser.add_argument(
        "--encoding",
        default="utf-8",
        help="文件读取/写入时的字符编码",
    )
    return parser.parse_args()


def collect_files(root_dirs, exts):
    """递归收集所有符合后缀的文件路径（返回pathlib.Path列表）"""
    files = []
    for root in root_dirs:
        root_path=pathlib.Path(root).resolve()
        if not root_path.is_dir():
            print(f"[WARN]目录不存在或不可访问：{root_path}", file=sys.stderr)
            continue
        for p in root_path.rglob("*"):
            if p.is_file() and p.suffix.lower() in exts:
                files.append(p)
    return files


def backup_file(path, bak_suffix):
    """生成备份（同名+bak_suffix），覆盖同名旧备份"""
    bak_path = path.with_name(path.name + bak_suffix)
    shutil.copy2(path, bak_path)
    return bak_path


def replace_in_file(path, old, new, *, dry_run, bak_suffix, encoding):
    """读取、替换、写回（或仅打印）"""
    try:
        text = path.read_text(encoding=encoding)
    except Exception as e:
        print(f"[ERROR]读取文件失败 {path}: {e}", file=sys.stderr)
        return
    
    if old not in text:
        return # 没有匹配的内容，直接跳过
    
    new_text = text.replace(old, new)
    count = text.count(old)

    if dry_run:
        print(f"[DRY-RUN] {path} -> 将替换 {count} 处")
        # 简单打印前后差异（只展示第一处出现的位置）
        idx = text.find(old)
        before = text[max(0, idx - 30): idx + 30].replace("\n", "->")
        after = new_text[max(0, idx - 30): idx + 30].replace("\n", "->")
        print(f"    ...{before}....")
        print(f"=>    ...{after}....")
        return

    # 正式写回前先备份
    backup_file(path, bak_suffix)

    try:
        path.write_text(new_text, encoding=encoding)
        print(f"[OK] {path} -> 替换 {count} 处（已备份 {path.name + bak_suffix}）")
    except Exception as e:
        print(f"[ERROR]写回文件失败 {path}: {e}", file=sys.stderr)


def main():
    args = parse_args()
    exts = [e.lower() if e.startswith(".") else f".{e.lower()}" for e in args.ext]

    files = collect_files(args.dir, exts)
    if not files:
        print("[INFO]未找到匹配的文件")
        return
    
    print(f"[INFO]共发现{len(files)}个文件，将把\"MicroAPI\"替换为\"Reg\"...")

    for f in files:
        replace_in_file(
            f,
            old="MicroAPI",
            new="Reg",
            dry_run=args.dry_run,
            bak_suffix=args.bak_suffix,
            encoding=args.encoding,
        )
    
    print("[DONE]所有文件处理完毕")
    if args.dry_run:
        print("[NOTE]由于使用了 --dry-run，实际文件未被修改，备份也未生成")


if __name__ == "__main__":
    main()

