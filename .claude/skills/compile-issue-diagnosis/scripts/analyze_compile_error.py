#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
编译错误分析脚本

功能：
1. 分析编译错误类型
2. 定位根本原因
3. 提供修复建议
"""

import re
import sys
import argparse
from pathlib import Path
from typing import Dict, List, Optional
from collections import defaultdict


class CompileErrorAnalyzer:
    """编译错误分析器"""

    def __init__(self, log_file: str):
        self.log_file = Path(log_file)
        self.errors = []
        self.knowledge_base = self._load_knowledge_base()

    def analyze(self) -> Dict:
        """分析编译错误"""
        print(f"[√] 分析编译日志: {self.log_file}")

        # 读取日志
        with open(self.log_file, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()

        # 提取错误
        errors = self._extract_errors(lines)

        if not errors:
            return {'status': 'success', 'message': '未发现编译错误'}

        # 分析每个错误
        analyzed_errors = []
        for error in errors[:5]:  # 只分析前5个错误
            analysis = self._analyze_error(error)
            analyzed_errors.append(analysis)

        return {
            'status': 'failed',
            'errors_count': len(errors),
            'analyzed_errors': analyzed_errors
        }

    def _extract_errors(self, lines: List[str]) -> List[Dict]:
        """提取错误信息"""
        errors = []
        current_error = None

        for i, line in enumerate(lines):
            # 检测错误开始
            if re.search(r'\berror:', line, re.IGNORECASE):
                if current_error:
                    errors.append(current_error)

                current_error = {
                    'line_number': i + 1,
                    'content': line.strip(),
                    'type': self._classify_error(line),
                    'details': []
                }

            elif current_error:
                # 收集错误详情（后续几行）
                if line.strip() and not line.startswith('  '):
                    current_error['details'].append(line.strip())

        if current_error:
            errors.append(current_error)

        return errors

    def _classify_error(self, error_line: str) -> str:
        """分类错误类型"""
        if 'undefined reference' in error_line:
            return '链接错误-符号未定义'
        elif 'fatal error' in error_line:
            return '致命错误-文件缺失'
        elif 'No such file or directory' in error_line:
            return '文件不存在'
        elif 'cannot convert' in error_line or 'invalid conversion' in error_line:
            return '类型转换错误'
        elif 'expected' in error_line:
            return '语法错误'
        elif 'conflicting' in error_line:
            return '冲突声明'
        elif 'template' in error_line.lower():
            return '模板错误'
        else:
            return '其他错误'

    def _analyze_error(self, error: Dict) -> Dict:
        """分析单个错误"""
        error_type = error['type']
        error_content = error['content']

        # 从知识库查找解决方案
        solution = self._find_solution(error_type, error_content)

        analysis = {
            'error_type': error_type,
            'error_content': error_content[:200],
            'root_cause': solution.get('root_cause', '未知'),
            'suggestions': solution.get('suggestions', []),
            'examples': solution.get('examples', [])
        }

        return analysis

    def _find_solution(self, error_type: str, error_content: str) -> Dict:
        """从知识库查找解决方案"""
        # 提取关键信息
        symbol_match = re.search(r"undefined reference to '([^']+)'", error_content)
        file_match = re.search(r"fatal error: ([^:]+): No such file", error_content)
        type_match = re.search(r"cannot convert '([^']+)' to '([^']+)'", error_content)

        # 链接错误-符号未定义
        if error_type == '链接错误-符号未定义' and symbol_match:
            symbol = symbol_match.group(1)
            return {
                'root_cause': f"符号 '{symbol}' 未定义，可能缺少链接库或函数实现",
                'suggestions': [
                    f"查找符号 '{symbol}' 的定义",
                    "添加缺失的链接库",
                    "检查函数实现是否存在",
                    "验证链接顺序"
                ],
                'examples': [
                    f"grep -r '{symbol}' --include='*.cpp' --include='*.c'",
                    "target_link_libraries(target PRIVATE libname)",
                    "LOCAL_SHARED_LIBRARIES += libname"
                ]
            }

        # 致命错误-文件缺失
        if error_type == '致命错误-文件缺失' and file_match:
            missing_file = file_match.group(1)
            return {
                'root_cause': f"文件 '{missing_file}' 不存在或路径错误",
                'suggestions': [
                    f"检查文件 '{missing_file}' 是否存在",
                    "验证头文件搜索路径",
                    "确认模块是否已编译",
                    "检查 include 路径配置"
                ],
                'examples': [
                    f"find . -name '{missing_file}'",
                    "include_directories(path/to/include)",
                    "LOCAL_C_INCLUDES += path/to/include"
                ]
            }

        # 类型转换错误
        if error_type == '类型转换错误' and type_match:
            from_type = type_match.group(1)
            to_type = type_match.group(2)
            return {
                'root_cause': f"无法将类型 '{from_type}' 转换为 '{to_type}'",
                'suggestions': [
                    "检查类型定义",
                    "使用显式类型转换",
                    "修改变量类型",
                    "使用模板或重载函数"
                ],
                'examples': [
                    f"static_cast<{to_type}>(value)",
                    f"reinterpret_cast<{to_type}>(value)",
                    f"const_cast<{to_type}>(value)"
                ]
            }

        # 默认解决方案
        return {
            'root_cause': f"{error_type.split('-')[0]}，需要进一步分析",
            'suggestions': [
                "查看完整的错误上下文",
                "检查相关代码和配置",
                "参考编译文档",
                "咨询团队成员"
            ],
            'examples': [
                "查看完整编译日志",
                "检查源文件",
                "验证编译配置"
            ]
        }

    def _load_knowledge_base(self) -> Dict:
        """加载知识库"""
        return {
            'common_errors': {
                'undefined reference': {
                    'root_cause': '符号未定义，缺少链接库或实现',
                    'solutions': [
                        {
                            'description': '添加缺失的链接库',
                            'cmake': 'target_link_libraries(target PRIVATE libname)',
                            'android_mk': 'LOCAL_SHARED_LIBRARIES += libname'
                        },
                        {
                            'description': '检查函数实现',
                            'command': 'grep -r "symbol_name" --include="*.cpp"'
                        }
                    ]
                },
                'fatal error: No such file': {
                    'root_cause': '头文件或源文件不存在',
                    'solutions': [
                        {
                            'description': '验证文件路径',
                            'command': 'find . -name "missing_file.h"'
                        },
                        {
                            'description': '添加 include 路径',
                            'cmake': 'include_directories(path/to/include)',
                            'android_mk': 'LOCAL_C_INCLUDES += path/to/include'
                        }
                    ]
                }
            }
        }


def print_analysis(analysis: Dict):
    """打印分析结果"""
    print(f"\n{'='*60}")
    print("编译错误分析报告")
    print(f"{'='*60}")

    if analysis['status'] == 'success':
        print(f"\n[√] {analysis['message']}")
        return

    print(f"\n发现错误: {analysis['errors_count']} 个")
    print(f"分析错误: {len(analysis['analyzed_errors'])} 个\n")

    for i, error in enumerate(analysis['analyzed_errors'], 1):
        print(f"[{i}] {error['error_type']}")
        print(f"    错误内容: {error['error_content'][:100]}...")
        print(f"    根本原因: {error['root_cause']}")

        if error['suggestions']:
            print(f"    修复建议:")
            for suggestion in error['suggestions']:
                print(f"      - {suggestion}")

        if error['examples']:
            print(f"    示例:")
            for example in error['examples']:
                print(f"      {example}")

        print()


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='编译错误分析工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例用法：
  # 分析编译日志
  python analyze_compile_error.py -f compile.log

  # 只分析特定类型错误
  python analyze_compile_error.py -f compile.log --error-type 链接错误

  # 强制生成修复建议
  python analyze_compile_error.py -f compile.log --suggest
        """
    )

    parser.add_argument('-f', '--file', required=True, help='编译日志文件路径')
    parser.add_argument('--error-type', help='指定错误类型进行深入分析')
    parser.add_argument('--suggest', action='store_true', help='强制生成修复建议')

    args = parser.parse_args()

    # 分析错误
    analyzer = CompileErrorAnalyzer(args.file)
    analysis = analyzer.analyze()

    # 打印结果
    print_analysis(analysis)

    return 0 if analysis['status'] == 'success' else 1


if __name__ == '__main__':
    sys.exit(main())
