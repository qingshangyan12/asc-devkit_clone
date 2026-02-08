#!/usr/bin/env python3
"""
YAML 文件语法检查与修复工具

功能：
1. 检测非标准 YAML 格式（如 key:value 缺少空格）
2. 自动修复语法问题
3. 生成修复报告
4. 支持备份原文件
"""

import sys
import re
import os
import argparse
from datetime import datetime
from typing import List, Tuple


def check_yaml_syntax(file_path: str) -> List[Tuple[int, str, str]]:
    """
    检查 YAML 文件的语法问题

    Args:
        file_path: YAML 文件路径

    Returns:
        问题列表，每个元素为 (行号, 原始内容, 问题描述)
    """
    issues = []

    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        for line_num, line in enumerate(lines, 1):
            # 跳过空行和注释行
            stripped = line.strip()
            if not stripped or stripped.startswith('#'):
                continue

            # 检查问题1: key:value 格式（冒号后缺少空格）
            # 匹配：行首缩进 + 键名 + 冒号 + 字母/数字（非空格）
            # 排除：URLs、列表项、注释等
            if ':' in line and not line.strip().startswith('-'):
                # 检查是否是键值对格式错误（冒号后紧跟非空格字符）
                parts = line.split(':', 1)
                if len(parts) == 2:
                    key_part = parts[0].strip()
                    value_part = parts[1]
                    # 键名应该是字母开头，可以包含字母、数字、下划线
                    if key_part and re.match(r'^[a-zA-Z_][a-zA-Z0-9_]*$', key_part):
                        # 冒号后如果直接跟非空格字符，可能是格式错误
                        if value_part and not value_part.startswith(' ') and not value_part.startswith('\t'):
                            # 排除一些合法情况
                            if not any(char in value_part[0] for char in ['/', '[', '{', '(', '\n']):
                                issues.append((line_num, line.rstrip(),
                                             f"键值对格式错误：'{key_part}:{value_part[0]}' 应为 '{key_part}: {value_part[0]}'"))

            # 检查问题2: 缩进不一致（使用 tab 混合空格）
            if '\t' in line:
                issues.append((line_num, line.rstrip(),
                             "使用 Tab 字符缩进，建议使用空格"))

            # 检查问题3: 行尾多余空格
            if line.rstrip() != line.rstrip(' '):
                issues.append((line_num, line.rstrip(),
                             "行尾存在多余空格"))

    except FileNotFoundError:
        print(f"错误：文件 {file_path} 不存在")
        sys.exit(1)
    except Exception as e:
        print(f"错误：读取文件失败 - {e}")
        sys.exit(1)

    return issues


def fix_yaml_syntax(file_path: str, issues: List[Tuple[int, str, str]],
                   backup: bool = True) -> str:
    """
    修复 YAML 文件的语法问题

    Args:
        file_path: YAML 文件路径
        issues: 问题列表
        backup: 是否创建备份文件

    Returns:
        修复后的文件路径
    """
    # 创建备份
    if backup:
        backup_path = f"{file_path}.backup_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        try:
            with open(file_path, 'r', encoding='utf-8') as src:
                with open(backup_path, 'w', encoding='utf-8') as dst:
                    dst.write(src.read())
            print(f"[OK] 已创建备份文件：{backup_path}")
        except Exception as e:
            print(f"[WARNING] 创建备份失败 - {e}")
            backup_path = None

    # 读取并修复文件
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        fixed_lines = []
        fix_count = 0

        for line_num, line in enumerate(lines, 1):
            fixed_line = line
            line_stripped = line.strip()

            # 跳过空行和注释
            if not line_stripped or line_stripped.startswith('#'):
                fixed_lines.append(fixed_line)
                continue

            # 修复问题1: key:value -> key: value
            if ':' in line and not line.strip().startswith('-'):
                parts = line.split(':', 1)
                if len(parts) == 2:
                    key_part = parts[0].strip()
                    value_part = parts[1]
                    if key_part and re.match(r'^[a-zA-Z_][a-zA-Z0-9_]*$', key_part):
                        if value_part and not value_part.startswith(' ') and not value_part.startswith('\t'):
                            if not any(char in value_part[0] for char in ['/', '[', '{', '(', '\n']):
                                # 修复：在冒号后添加空格
                                colon_pos = line.find(':')
                                fixed_line = line[:colon_pos+1] + ' ' + line[colon_pos+1:]
                                fix_count += 1

            # 修复问题2: 移除行尾多余空格
            fixed_line = fixed_line.rstrip() + '\n'

            fixed_lines.append(fixed_line)

        # 写回文件
        with open(file_path, 'w', encoding='utf-8') as f:
            f.writelines(fixed_lines)

        print(f"[OK] 修复完成：共修复 {fix_count} 处语法问题")

        # 尝试验证修复后的文件
        try:
            import yaml
            with open(file_path, 'r', encoding='utf-8') as f:
                yaml.safe_load(f)
            print(f"[OK] YAML 语法验证通过")
        except ImportError:
            print(f"[WARNING] 未安装 PyYAML，无法验证修复后的文件")
        except Exception as e:
            print(f"[WARNING] 修复后仍存在语法问题 - {e}")

        return file_path

    except Exception as e:
        print(f"错误：修复失败 - {e}")
        # 如果有备份，恢复备份
        if backup and backup_path and os.path.exists(backup_path):
            try:
                with open(backup_path, 'r', encoding='utf-8') as src:
                    with open(file_path, 'w', encoding='utf-8') as dst:
                        dst.write(src.read())
                print(f"[OK] 已从备份恢复文件")
            except:
                pass
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(
        description='YAML 文件语法检查与修复工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例：
  # 仅检查语法问题
  python fix_non_standard_yaml.py ci_config/rule.yaml

  # 检查并自动修复（创建备份）
  python fix_non_standard_yaml.py ci_config/rule.yaml --fix

  # 修复但不创建备份
  python fix_non_standard_yaml.py ci_config/rule.yaml --fix --no-backup

  # 详细模式
  python fix_non_standard_yaml.py ci_config/rule.yaml --fix --verbose
        """
    )

    parser.add_argument('file_path', help='YAML 文件路径')
    parser.add_argument('--fix', action='store_true', help='自动修复语法问题')
    parser.add_argument('--no-backup', action='store_true',
                       help='不创建备份文件（谨慎使用）')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='详细模式，显示所有问题')

    args = parser.parse_args()

    # 检查文件
    if not os.path.exists(args.file_path):
        print(f"错误：文件 {args.file_path} 不存在")
        sys.exit(1)

    print(f"\n检查文件：{args.file_path}")
    print("=" * 80)

    # 检查语法问题
    issues = check_yaml_syntax(args.file_path)

    if not issues:
        print("[OK] 未发现语法问题")
        sys.exit(0)

    print(f"\n发现 {len(issues)} 个潜在问题：")
    print("-" * 80)

    # 显示问题摘要
    issue_types = {}
    for line_num, line, desc in issues:
        issue_type = desc.split('：')[0] if '：' in desc else desc
        issue_types[issue_type] = issue_types.get(issue_type, 0) + 1

    for issue_type, count in issue_types.items():
        print(f"  - {issue_type}: {count} 处")

    # 详细模式：显示所有问题
    if args.verbose:
        print("\n详细问题列表：")
        print("-" * 80)
        for line_num, line, desc in issues:
            print(f"\n第 {line_num} 行:")
            print(f"  内容: {line}")
            print(f"  问题: {desc}")

    # 修复模式
    if args.fix:
        print("\n" + "=" * 80)
        confirm = input(f"确认修复？[y/N]: ") if not args.no_backup else 'y'

        if confirm.lower() == 'y':
            fix_yaml_syntax(args.file_path, issues, backup=not args.no_backup)
        else:
            print("已取消修复")
    else:
        print("\n提示：使用 --fix 参数自动修复这些问题")


if __name__ == "__main__":
    main()
