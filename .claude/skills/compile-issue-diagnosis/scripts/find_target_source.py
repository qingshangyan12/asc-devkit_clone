#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Target 源码查找脚本

功能：
1. 根据 target 名称查找对应的源代码文件
2. 分析 target 的编译逻辑
3. 查找依赖的配置文件
"""

import os
import re
import sys
import argparse
from pathlib import Path
from typing import Dict, List, Optional


class TargetSourceFinder:
    """Target 源码查找器"""

    def __init__(self, target: str, repo_path: str = None):
        self.target = target
        self.repo_path = Path(repo_path) if repo_path else Path.cwd()
        self.source_files = []
        self.build_config = None

    def find(self) -> Dict:
        """查找 target 对应的源代码"""
        print(f"[√] 查找 Target: {self.target}")

        result = {
            'target': self.target,
            'source_files': [],
            'build_config': None,
            'dependencies': []
        }

        # 步骤1：查找对应的源文件
        source_files = self._find_source_files()
        result['source_files'] = source_files

        # 步骤2：查找编译配置文件
        build_config = self._find_build_config()
        result['build_config'] = build_config

        # 步骤3：分析依赖关系
        if build_config:
            dependencies = self._extract_dependencies(build_config)
            result['dependencies'] = dependencies

        return result

    def _find_source_files(self) -> List[Dict]:
        """查找源文件"""
        print(f"  [*] 查找源文件...")

        # 提取 target 的基础名称
        base_name = self._extract_base_name()
        if not base_name:
            print(f"  [!] 无法提取 target 名称")
            return []

        source_files = []

        # 查找所有相关的源文件
        search_patterns = [
            f"**/{base_name}*.cpp",
            f"**/{base_name}*.cc",
            f"**/{base_name}*.c",
            f"**/lib{base_name}/**/*.cpp",
        ]

        for pattern in search_patterns:
            matches = list(self.repo_path.rglob(pattern))
            for match in matches:
                if match.is_file():
                    rel_path = match.relative_to(self.repo_path)
                    source_files.append({
                        'path': str(rel_path),
                        'absolute_path': str(match),
                        'language': self._detect_language(match)
                    })

        # 去重
        seen = set()
        unique_files = []
        for f in source_files:
            path_key = f['path']
            if path_key not in seen:
                seen.add(path_key)
                unique_files.append(f)

        if unique_files:
            print(f"  [√] 找到 {len(unique_files)} 个源文件")
            for f in unique_files[:5]:  # 只显示前5个
                print(f"      - {f['path']}")
        else:
            print(f"  [!] 未找到源文件")

        return unique_files

    def _extract_base_name(self) -> Optional[str]:
        """提取 target 的基础名称"""
        # libbase.so -> base
        # libc++.so -> c++
        # libutils.so -> utils
        match = re.match(r'lib(.+?)\.so', self.target)
        if match:
            return match.group(1)

        # base.a -> base
        match = re.match(r'(.+?)\.a', self.target)
        if match:
            return match.group(1)

        # 直接使用 target 名称
        return self.target.replace('.so', '').replace('.a', '')

    def _detect_language(self, file_path: Path) -> str:
        """检测编程语言"""
        suffix = file_path.suffix.lower()
        lang_map = {
            '.cpp': 'C++',
            '.cc': 'C++',
            '.cxx': 'C++',
            '.c': 'C',
            '.h': 'C/C++ Header',
            '.hpp': 'C++ Header',
        }
        return lang_map.get(suffix, 'Unknown')

    def _find_build_config(self) -> Optional[Dict]:
        """查找编译配置文件"""
        print(f"  [*] 查找编译配置...")

        base_name = self._extract_base_name()
        if not base_name:
            return None

        # 查找常见的编译配置文件
        config_files = []

        # Android.mk
        android_mks = list(self.repo_path.rglob('Android.mk'))
        for mk in android_mks:
            content = mk.read_text(encoding='utf-8', errors='ignore')
            if base_name in content or f'lib{base_name}' in content:
                config_files.append({
                    'type': 'Android.mk',
                    'path': str(mk.relative_to(self.repo_path)),
                    'content': self._extract_relevant_config(content, base_name)
                })

        # CMakeLists.txt
        cmake_files = list(self.repo_path.rglob('CMakeLists.txt'))
        for cmake in cmake_files:
            content = cmake.read_text(encoding='utf-8', errors='ignore')
            if base_name in content or f'lib{base_name}' in content:
                config_files.append({
                    'type': 'CMakeLists.txt',
                    'path': str(cmake.relative_to(self.repo_path)),
                    'content': self._extract_relevant_config(content, base_name)
                })

        # Makefile
        makefiles = list(self.repo_path.rglob('Makefile'))
        for mk in makefiles:
            content = mk.read_text(encoding='utf-8', errors='ignore')
            if base_name in content:
                config_files.append({
                    'type': 'Makefile',
                    'path': str(mk.relative_to(self.repo_path)),
                    'content': self._extract_relevant_config(content, base_name)
                })

        if config_files:
            print(f"  [√] 找到 {len(config_files)} 个配置文件")
            for cfg in config_files:
                print(f"      - {cfg['type']}: {cfg['path']}")
            return config_files[0] if config_files else None
        else:
            print(f"  [!] 未找到编译配置文件")
            return None

    def _extract_relevant_config(self, content: str, base_name: str) -> List[str]:
        """提取相关的配置内容"""
        lines = content.split('\n')
        relevant_lines = []

        # 查找包含 target 名称的行及其上下文
        for i, line in enumerate(lines):
            if base_name in line or f'lib{base_name}' in line:
                # 添加当前行和前后2行
                start = max(0, i - 2)
                end = min(len(lines), i + 3)
                relevant_lines.extend(lines[start:end])
                relevant_lines.append('...')  # 分隔符

        return relevant_lines[:20]  # 限制行数

    def _extract_dependencies(self, config: Dict) -> List[str]:
        """从配置文件中提取依赖"""
        content = '\n'.join(config.get('content', []))
        dependencies = []

        # 提取常见的依赖模式
        patterns = [
            r'SHARED_LIBRARIES\s*\+=\s*(.+)',  # Android.mk
            r'target_link_libraries\s*\(\s*\w+\s+(.+)\)',  # CMake
            r'LDLIBS\s*\+=\s*(.+)',  # Makefile
        ]

        for pattern in patterns:
            matches = re.findall(pattern, content)
            for match in matches:
                # 分割并清理库名称
                libs = re.findall(r'lib(\w+)', match)
                dependencies.extend(libs)

        # 去重
        return list(set(dependencies))


def print_result(result: Dict):
    """打印查找结果"""
    print(f"\n{'='*60}")
    print(f"Target: {result['target']}")
    print(f"{'='*60}")

    print(f"\n源文件 ({len(result['source_files'])} 个):")
    if result['source_files']:
        for f in result['source_files'][:10]:
            print(f"  [{f['language']}] {f['path']}")
        if len(result['source_files']) > 10:
            print(f"  ... 还有 {len(result['source_files']) - 10} 个文件")
    else:
        print("  未找到源文件")

    if result['build_config']:
        print(f"\n编译配置:")
        cfg = result['build_config']
        print(f"  类型: {cfg['type']}")
        print(f"  文件: {cfg['path']}")
        if cfg.get('content'):
            print(f"  相关配置:")
            for line in cfg['content'][:10]:
                print(f"    {line}")

    if result['dependencies']:
        print(f"\n依赖库:")
        for dep in result['dependencies']:
            print(f"  - lib{dep}")


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='Target 源码查找工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例用法：
  # 查找 libbase.so 的源文件
  python find_target_source.py -t libbase.so

  # 指定代码仓路径
  python find_target_source.py -t libbase.so -r /path/to/repo

  # 指定分支
  python find_target_source.py -t libbase.so -b master
        """
    )

    parser.add_argument('-t', '--target', required=True, help='Target 名称')
    parser.add_argument('-r', '--repo', help='代码仓路径（默认自动查找）')
    parser.add_argument('-b', '--branch', help='分支名称（默认 master）')

    args = parser.parse_args()

    # 查找源文件
    finder = TargetSourceFinder(args.target, args.repo)
    result = finder.find()

    # 打印结果
    print_result(result)

    return 0 if result['source_files'] else 1


if __name__ == '__main__':
    sys.exit(main())
