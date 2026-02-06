#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Git 冲突检测脚本

功能：
1. 检测工作目录中是否存在冲突
2. 列出所有冲突文件
3. 提供冲突处理建议
"""

import argparse
import sys
import subprocess
from pathlib import Path


class ConflictDetector:
    """Git 冲突检测器"""

    def __init__(self):
        self.conflicted_files = []
        self.unmerged_files = []

    def check_conflicts(self):
        """检测是否存在冲突"""
        try:
            # 执行 git status 获取工作目录状态
            result = subprocess.run(
                ['git', 'status', '--porcelain'],
                capture_output=True,
                text=True,
                check=False
            )

            if result.returncode != 0:
                print(f"错误：执行 git status 失败")
                return False

            # 解析 git status 输出
            for line in result.stdout.strip().split('\n'):
                if not line:
                    continue

                status_code = line[:2]
                file_path = line[3:]

                # UU = 两者都修改
                # AA = 两者都添加
                # U  = 有冲突但未标记
                if status_code in ['UU', 'AA', 'U']:
                    self.unmerged_files.append(file_path)

                # DD = 两者都删除
                # DU = 他们删除，我们修改
                # UD = 我们删除，他们修改
                if status_code in ['DD', 'DU', 'UD']:
                    self.conflicted_files.append(file_path)

            return self.has_conflicts()

        except Exception as e:
            print(f"错误：检测冲突失败: {e}")
            return False

    def has_conflicts(self):
        """是否存在冲突"""
        return len(self.conflicted_files) > 0 or len(self.unmerged_files) > 0

    def print_report(self):
        """打印冲突报告"""
        print("\n" + "="*60)
        print("Git 冲突检测报告")
        print("="*60)

        if not self.has_conflicts():
            print("\n✓ 未检测到冲突")
            print("工作目录干净，可以继续操作。")
            return

        print("\n✗ 检测到冲突！")
        print(f"\n冲突文件数：{len(self.conflicted_files) + len(self.unmerged_files)}")

        if self.conflicted_files:
            print("\n【类型1】已删除冲突（DD/DU/UD）：")
            for f in self.conflicted_files:
                print(f"  - {f}")
            print("\n  建议：")
            print("  - 查看远程和本地差异")
            print("  - 与团队成员沟通确认")

        if self.unmerged_files:
            print("\n【类型2】内容冲突（UU/AA/U）：")
            for f in self.unmerged_files:
                print(f"  - {f}")
            print("\n  建议：")
            print("  - 手动编辑文件解决冲突")
            print("  - 删除冲突标记（<<<<<<<, =======, >>>>>>>）")

        print(f"\n{'='*60}")
        print("冲突解决指导：")
        print(f"{'='*60}")
        print("\n1. 打开冲突文件，查找冲突标记")
        print("2. 根据需要保留远程代码、本地代码或手动合并")
        print("3. 删除所有冲突标记（<<<<<<<, =======, >>>>>>>）")
        print("4. 保存文件")
        print("5. 执行：git add <冲突文件>")
        print("6. 再次运行本脚本验证冲突已解决")
        print()

    def get_conflicted_files(self):
        """获取冲突文件列表"""
        return self.conflicted_files + self.unmerged_files

    def list_raw_conflicts(self):
        """列出原始冲突标记"""
        for file_path in self.get_conflicted_files():
            try:
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                    if '<<<<<<<' in content:
                        print(f"\n冲突文件：{file_path}")
                        print("-" * 60)
                        # 提取冲突片段
                        lines = content.split('\n')
                        in_conflict = False
                        conflict_lines = []
                        for i, line in enumerate(lines):
                            if '<<<<<<<' in line:
                                in_conflict = True
                            if in_conflict:
                                conflict_lines.append(line)
                            if '>>>>>>>' in line:
                                in_conflict = False
                                # 显示冲突内容
                                print('\n'.join(conflict_lines))
                                conflict_lines = []
                        print("-" * 60)
            except Exception as e:
                print(f"错误：读取文件 {file_path} 失败: {e}")


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='Git 冲突检测工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例用法：
  # 检测冲突
  python check_conflict.py

  # 检测冲突并显示详细信息
  python check_conflict.py --verbose

  # 仅检测，不输出（用于脚本）
  python check_conflict.py --quiet

退出码：
  0: 无冲突
  1: 存在冲突
        """
    )

    parser.add_argument('--verbose', '-v', action='store_true', help='显示详细信息')
    parser.add_argument('--quiet', '-q', action='store_true', help='安静模式，仅输出结果')
    parser.add_argument('--list', '-l', action='store_true', help='列出原始冲突标记')

    args = parser.parse_args()

    detector = ConflictDetector()
    has_conflicts = detector.check_conflicts()

    if args.quiet:
        # 安静模式，仅返回退出码
        sys.exit(1 if has_conflicts else 0)

    # 打印报告
    detector.print_report()

    # 详细模式：显示原始冲突标记
    if args.verbose and has_conflicts:
        print("\n" + "="*60)
        print("冲突标记详情：")
        print("="*60)
        detector.list_raw_conflicts()

    sys.exit(1 if has_conflicts else 0)


if __name__ == '__main__':
    main()
