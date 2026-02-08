#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
编译日志解析脚本

功能：
1. 解析编译日志，提取错误信息
2. 识别失败的 target 和 .o 文件
3. 提取编译命令和错误上下文
4. 生成诊断报告
"""

import re
import sys
import argparse
from pathlib import Path
from typing import Dict, List, Tuple, Optional


class CompileLogParser:
    """编译日志解析器"""

    def __init__(self, log_file: str):
        self.log_file = Path(log_file)
        self.errors = []
        self.failed_targets = {}
        self.compile_commands = {}

    def parse(self) -> Dict:
        """解析编译日志"""
        if not self.log_file.exists():
            print(f"[错误] 日志文件不存在: {self.log_file}")
            return {}

        print(f"[√] 解析编译日志: {self.log_file}")

        with open(self.log_file, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()

        self._extract_errors(lines)
        self._identify_failed_targets(lines)
        self._extract_compile_commands(lines)

        return self._generate_report()

    def _extract_errors(self, lines: List[str]):
        """提取错误信息"""
        error_patterns = [
            r'error:',  # 通用错误
            r'undefined reference',  # 链接错误
            r'fatal error:',  # 致命错误
            r'No such file or directory',  # 文件不存在
            r'cannot convert',  # 类型转换错误
        ]

        for i, line in enumerate(lines):
            for pattern in error_patterns:
                if re.search(pattern, line, re.IGNORECASE):
                    error_info = {
                        'line_number': i + 1,
                        'content': line.strip(),
                        'type': self._classify_error(line),
                        'context': self._get_context(lines, i),
                    }
                    self.errors.append(error_info)
                    break

    def _classify_error(self, error_line: str) -> str:
        """分类错误类型"""
        if 'undefined reference' in error_line:
            return '链接错误'
        elif 'fatal error' in error_line:
            return '致命错误'
        elif 'No such file' in error_line:
            return '文件不存在'
        elif 'cannot convert' in error_line or 'invalid conversion' in error_line:
            return '类型错误'
        elif 'expected ' in error_line:
            return '语法错误'
        else:
            return '其他错误'

    def _get_context(self, lines: List[str], error_idx: int, context_lines: int = 3) -> List[str]:
        """获取错误上下文"""
        start = max(0, error_idx - context_lines)
        end = min(len(lines), error_idx + context_lines + 1)
        return [lines[i].strip() for i in range(start, end)]

    def _identify_failed_targets(self, lines: List[str]):
        """识别失败的 target"""
        # 匹配常见的编译失败模式
        patterns = [
            r'error:\s+\*\*\*\s+\[(.+?)\]',  # make: *** [target]
            r'FAILED:\s+(.+?)(?:\s|$)',  # ninja: FAILED: target
            r'ld:\s+cannot find\s+(.+?)',  # ld: cannot find
            r'(.+?\.so):\s+error',  # libxxx.so: error
            r'making\s+(.+?)\s+(?:failed|error)',  # making target failed
        ]

        for line in lines:
            for pattern in patterns:
                match = re.search(pattern, line, re.IGNORECASE)
                if match:
                    target = match.group(1)
                    if target not in self.failed_targets:
                        self.failed_targets[target] = {
                            'name': target,
                            'type': self._identify_target_type(target),
                            'errors': []
                        }
                    break

        # 如果没有找到明确的 target，从错误信息中推断
        if not self.failed_targets and self.errors:
            self._infer_target_from_errors(lines)

    def _identify_target_type(self, target: str) -> str:
        """识别 target 类型"""
        if target.endswith('.so'):
            return 'Shared Library'
        elif target.endswith('.a'):
            return 'Static Library'
        elif target.endswith('.exe') or 'test' in target.lower():
            return 'Executable'
        else:
            return 'Unknown'

    def _infer_target_from_errors(self, lines: List[str]):
        """从错误信息推断 target"""
        for error in self.errors:
            content = error['content']

            # 从错误信息中提取 .o 文件
            obj_match = re.search(r'(.+?\.o):', content)
            if obj_match:
                obj_file = obj_match.group(1)
                target_name = self._obj_to_target(obj_file)

                if target_name and target_name not in self.failed_targets:
                    self.failed_targets[target_name] = {
                        'name': target_name,
                        'type': self._identify_target_type(target_name),
                        'obj_file': obj_file,
                        'errors': []
                    }
                break

    def _obj_to_target(self, obj_file: str) -> Optional[str]:
        """从 .o 文件推断 target 名称"""
        # 常见模式：path/to/libxxx/xxx.o -> libxxx.so
        if 'lib' in obj_file:
            parts = obj_file.split('/')
            for part in parts:
                if part.startswith('lib'):
                    # libbase/utils/base_utils.o -> libbase.so
                    lib_name = part.split('_')[0]
                    return f"{lib_name}.so"

        # path/to/xxx/yyy.o -> yyy.so (或其他)
        parts = obj_file.split('/')
        if len(parts) >= 2:
            return parts[-2] + '.so'

        return None

    def _extract_compile_commands(self, lines: List[str]):
        """提取编译命令"""
        for i, line in enumerate(lines):
            # 匹配编译命令模式
            if any(cmd in line for cmd in ['gcc', 'g++', 'clang', 'clang++', 'arm-linux']):
                # 提取 .o 文件名
                obj_match = re.search(r'(\S+\.o)', line)
                if obj_match:
                    obj_file = obj_match.group(1)
                    self.compile_commands[obj_file] = {
                        'command': line.strip(),
                        'line_number': i + 1
                    }

    def _generate_report(self) -> Dict:
        """生成诊断报告"""
        if not self.errors and not self.failed_targets:
            return {
                'status': 'success',
                'message': '未发现编译错误',
                'errors_count': 0
            }

        report = {
            'status': 'failed',
            'errors_count': len(self.errors),
            'failed_targets': list(self.failed_targets.values()),
            'errors': self.errors[:5],  # 只显示前5个错误
            'compile_commands': self.compile_commands
        }

        return report


def print_report(report: Dict, verbose: bool = False):
    """打印诊断报告"""
    if report['status'] == 'success':
        print(f"\n[√] {report['message']}")
        return

    print(f"\n[X] 发现编译错误")
    print(f"\n问题概览:")

    if report.get('failed_targets'):
        for target in report['failed_targets']:
            print(f"  Target: {target['name']} ({target['type']})")
            if 'obj_file' in target:
                print(f"  出错文件: {target['obj_file']}")

    if report.get('errors'):
        print(f"\n错误详情:")
        for i, error in enumerate(report['errors'][:3], 1):
            print(f"\n  [{i}] {error['type']}")
            print(f"      {error['content'][:100]}...")

            if verbose and error.get('context'):
                print(f"      上下文:")
                for ctx_line in error['context'][:3]:
                    print(f"        {ctx_line}")


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='编译日志解析工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例用法：
  # 解析编译日志
  python parse_compile_log.py -f compile.log

  # 解析并显示详细信息
  python parse_compile_log.py -f compile.log -v

  # 分析指定 target
  python parse_compile_log.py -f compile.log -t libbase.so
        """
    )

    parser.add_argument('-f', '--file', required=True, help='编译日志文件路径')
    parser.add_argument('-t', '--target', help='指定分析的 target 名称')
    parser.add_argument('-v', '--verbose', action='store_true', help='显示详细诊断过程')
    parser.add_argument('-o', '--output', help='输出诊断报告到文件')

    args = parser.parse_args()

    # 解析日志
    parser = CompileLogParser(args.file)
    report = parser.parse()

    # 打印报告
    print_report(report, args.verbose)

    # 输出到文件
    if args.output:
        import json
        with open(args.output, 'w', encoding='utf-8') as f:
            json.dump(report, f, indent=2, ensure_ascii=False)
        print(f"\n[√] 报告已保存到: {args.output}")

    # 返回退出码
    sys.exit(0 if report['status'] == 'success' else 1)


if __name__ == '__main__':
    main()
