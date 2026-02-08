#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
YAML 节点构造工具

功能：
根据任务名自动生成符合规范的 YAML 节点内容，并可添加到 YAML 文件中
"""

import argparse
import sys
import re
import subprocess
from pathlib import Path

try:
    import yaml
except ImportError:
    print("错误：未安装 PyYAML 模块")
    print("请执行: pip3 install PyYAML")
    sys.exit(1)


class NodeConstructor:
    """YAML 节点构造器"""

    # 节点类型定义
    NODE_TYPES = {
        'pkg': '打包任务',
        'compile': '编译任务',
        'test': '测试任务',
        'llt': '链接测试任务',
        'deploy': '部署任务',
        'misc': '其他任务'
    }

    # 通用配置项
    COMMON_CONFIG = {
        'version': 'cann',
        'enabled': True,
        'timeout': 3600
    }

    # 各类型节点的默认模板
    TYPE_TEMPLATES = {
        'pkg': {
            'timeout': 3600,
            'type': 'pkg',
            'target': ['<target_name>.so', '<target_name>.a']
        },
        'compile': {
            'timeout': 7200,
            'type': 'compile',
            'source': ['src/<module_name>/*.cpp'],
            'include': ['include/'],
            'flags': ['-O2', '-Wall']
        },
        'test': {
            'timeout': 1800,
            'type': 'test',
            'case': ['test_<module>_001'],
            'dependency': ['<compile_task>'],
            'ut_check': True
        },
        'llt': {
            'timeout': 5400,
            'type': 'llt',
            'module': ['<module_name>'],
            'coverage': {'line': 80, 'branch': 70}
        },
        'deploy': {
            'timeout': 2400,
            'type': 'deploy',
            'target': {
                'host': '<host_ip>',
                'path': '<deploy_path>'
            },
            'pre_check': ['disk_space_check'],
            'post_check': ['service_status_check']
        }
    }

    def __init__(self):
        """初始化构造器"""
        self.node_type = None
        self.node_name = None
        self.suffix = None
        self.full_name = None

    def parse_node_name(self, name):
        """
        解析节点名称

        Args:
            name: 节点名称

        Returns:
            bool: 解析是否成功
        """
        # 检查命名规范
        if not re.match(r'^[a-z][a-z0-9_]*$', name):
            print(f"错误：节点名称不符合规范")
            print(f"规范：必须使用小写字母和下划线，不能以数字开头")
            return False

        # 提取类型前缀
        for prefix in ['pkg_', 'compile_', 'test_', 'llt_', 'deploy_']:
            if name.startswith(prefix):
                self.node_type = prefix[:-1]  # 去掉下划线
                self.node_name = name[len(prefix):]
                self.suffix = None
                self.full_name = name
                return True

        # 没有类型前缀，视为 misc 类型
        self.node_type = 'misc'
        self.node_name = name
        self.suffix = None
        self.full_name = name

        print(f"警告：节点名称缺少类型前缀（pkg/compile/test/llt/deploy），归类为 misc 类型")
        return True

    def extract_suffix(self):
        """
        从节点名称中提取后缀信息

        Returns:
            dict: 后缀信息
        """
        suffix_info = {
            'version': None,
            'env': None,
            'is_test': False,
            'is_prod': False
        }

        if not self.node_name:
            return suffix_info

        # 检查版本后缀
        version_match = re.search(r'_v(\d+)', self.node_name)
        if version_match:
            suffix_info['version'] = version_match.group(1)

        # 检查环境后缀
        if '_prod' in self.node_name:
            suffix_info['env'] = 'production'
            suffix_info['is_prod'] = True
        elif '_test' in self.node_name:
            suffix_info['env'] = 'testing'
            suffix_info['is_test'] = True

        # 检查 smoke 测试
        if 'smoke' in self.node_name:
            suffix_info['is_smoke'] = True

        # 检查性能测试
        if 'perf' in self.node_name or 'performance' in self.node_name:
            suffix_info['is_performance'] = True

        return suffix_info

    def generate_node_content(self):
        """
        生成节点内容

        Returns:
            dict: 节点内容
        """
        # 获取后缀信息
        suffix_info = self.extract_suffix()

        # 基础内容：通用配置项
        content = self.COMMON_CONFIG.copy()

        # 根据类型添加特定配置
        if self.node_type in self.TYPE_TEMPLATES:
            template = self.TYPE_TEMPLATES[self.node_type]
            # 深度复制模板
            for key, value in template.items():
                if isinstance(value, dict):
                    content[key] = value.copy()
                elif isinstance(value, list):
                    content[key] = value.copy()
                else:
                    content[key] = value

        # 根据后缀调整内容
        self._apply_suffix_rules(content, suffix_info)

        # 替换占位符
        content = self._replace_placeholders(content)

        return content

    def _apply_suffix_rules(self, content, suffix_info):
        """
        应用后缀规则调整内容

        Args:
            content: 节点内容
            suffix_info: 后缀信息
        """
        # 性能测试：增加超时时间
        if suffix_info.get('is_performance'):
            content['timeout'] = 7200
            if self.node_type == 'test':
                content['performance_test'] = True

        # Smoke 测试
        if suffix_info.get('is_smoke'):
            content['smoke_test'] = True

        # 生产环境
        if suffix_info.get('is_prod'):
            content['env'] = 'production'

        # 测试环境
        if suffix_info.get('is_test'):
            content['env'] = 'testing'

        # 版本信息
        if suffix_info.get('version'):
            content['build_version'] = suffix_info['version']

    def _replace_placeholders(self, content):
        """
        替换内容中的占位符

        Args:
            content: 节点内容

        Returns:
            dict: 替换后的内容
        """
        def replace_value(value):
            if isinstance(value, str):
                # 替换各种占位符
                value = value.replace('<target_name>', self.node_name)
                value = value.replace('<module_name>', self.node_name)
                value = value.replace('<compile_task>', f'compile_{self.node_name}')
                value = value.replace('<host_ip>', '192.168.1.100')
                value = value.replace('<deploy_path>', '/opt/cann/deploy')
                return value
            elif isinstance(value, dict):
                return {k: replace_value(v) for k, v in value.items()}
            elif isinstance(value, list):
                return [replace_value(item) for item in value]
            return value

        return {k: replace_value(v) for k, v in content.items()}

    def format_node_yaml(self, content):
        """
        将节点内容格式化为 YAML 字符串（用于显示）

        Args:
            content: 节点内容

        Returns:
            str: YAML 格式字符串
        """
        lines = [f"{self.full_name}:"]
        for key, value in content.items():
            if isinstance(value, bool):
                value = 'true' if value else 'false'
                lines.append(f"  {key}: {value}")
            elif isinstance(value, dict):
                lines.append(f"  {key}:")
                for k, v in value.items():
                    if isinstance(v, bool):
                        v = 'true' if v else 'false'
                    lines.append(f"    {k}: {v}")
            elif isinstance(value, list):
                lines.append(f"  {key}:")
                for item in value:
                    lines.append(f"    - {item}")
            else:
                lines.append(f"  {key}: {value}")

        return "\n".join(lines)

    def format_node_for_add(self, content):
        """
        将节点内容格式化为 add_node.py 可接受的格式

        Args:
            content: 节点内容

        Returns:
            str: 节点规格字符串
        """
        # 使用 YAML 格式：node_name: {key1: value1, key2: value2}
        items = []
        for key, value in content.items():
            if isinstance(value, bool):
                value = 'true' if value else 'false'
            elif isinstance(value, dict):
                # 嵌套字典
                dict_items = [f"{k}: {v}" for k, v in value.items()]
                value = "{" + ", ".join(dict_items) + "}"
            elif isinstance(value, list):
                # 列表
                list_items = [str(item) for item in value]
                value = "[" + ", ".join(list_items) + "]"
            else:
                value = str(value)
            items.append(f"{key}: {value}")

        node_spec = f"{self.full_name}: {{{', '.join(items)}}}"
        return node_spec

    def get_parent_path(self):
        """
        获取节点应该添加到的父路径

        Returns:
            str: 父路径
        """
        return f"CB.tasks.{self.node_type}"

    def validate_hierarchy(self, parent_path=None):
        """
        验证节点名称与层级的对应关系

        规则：节点名称的前缀必须与层级类型一致

        Args:
            parent_path: 指定的父路径（可选）

        Returns:
            tuple: (is_valid, error_message)
        """
        # 如果没有指定parent_path，使用默认推断的路径
        if not parent_path:
            parent_path = self.get_parent_path()

        # 从父路径中提取层级类型
        # CB.tasks.<层级类型> -> 提取 <层级类型>
        parts = parent_path.split('.')
        if len(parts) >= 3 and parts[0] == 'CB' and parts[1] == 'tasks':
            hierarchy_type = parts[2]  # pkg, compile, test, llt, deploy, misc
        else:
            return False, f"父路径格式不正确：{parent_path}，应为 CB.tasks.<层级类型>"

        # 验证节点名称前缀是否与层级类型匹配
        node_prefix = self.node_type  # 从节点名称中提取的类型

        if node_prefix == 'misc':
            # misc类型的节点可以在任何层级下
            return True, None

        if node_prefix != hierarchy_type:
            # 提供清晰的错误信息
            error_msg = (
                f"节点名称与层级不匹配！\n"
                f"  节点名称：{self.full_name}（前缀：{node_prefix}）\n"
                f"  父路径：{parent_path}（层级类型：{hierarchy_type}）\n\n"
                f"规则：节点名称必须以所在层级类型开头\n"
                f"  正确示例：CB.tasks.{node_prefix}.{node_prefix}_xxx\n"
                f"  当前路径：CB.tasks.{hierarchy_type}.{node_prefix}_xxx\n\n"
                f"建议：\n"
            )

            # 根据层级类型提供具体建议
            if hierarchy_type in ['pkg', 'compile', 'test', 'llt', 'deploy']:
                error_msg += f"  1. 将节点放到 CB.tasks.{node_prefix} 层级下\n"
                error_msg += f"  2. 或修改节点名称为 {hierarchy_type}_{self.node_name}"
            else:
                error_msg += f"  1. 将节点放到 CB.tasks.{node_prefix} 层级下\n"
                error_msg += f"  2. 或修改节点名称以匹配层级类型"

            return False, error_msg

        return True, None

    def add_to_yaml(self, yaml_file, content):
        """
        将节点添加到 YAML 文件

        Args:
            yaml_file: YAML 文件路径
            content: 节点内容

        Returns:
            bool: 是否成功
        """
        # 构建 add_node 参数
        parent_path = self.get_parent_path()
        node_spec = self.format_node_for_add(content)

        print(f"\n准备添加节点到 YAML 文件...")
        print(f"文件: {yaml_file}")
        print(f"父路径: {parent_path}")
        print(f"节点名称: {self.full_name}")
        print(f"\n节点内容：")
        print(self.format_node_yaml(content))
        print(f"\n{'='*60}")

        # 验证层级对应关系
        is_valid, error_msg = self.validate_hierarchy(parent_path)
        if not is_valid:
            print(f"\n[✗] 层级验证失败！")
            print(f"\n{error_msg}")
            print(f"\n请修正后再执行添加操作。")
            print(f"\n提示：使用 --parent 参数可以指定其他父路径")
            print(f"示例：python create_node.py -n {self.full_name} -f {yaml_file} -p CB.tasks.{self.node_type}")
            return False

        print(f"\n[√] 层级验证通过")
        print(f"  节点名称前缀：{self.node_type}")
        print(f"  父层级类型：{parent_path.split('.')[2] if len(parent_path.split('.')) >= 3 else '未知'}")
        print(f"  ✓ 匹配正确")

        # 调用 edit-yaml 的 add_node.py
        script_path = Path(__file__).parent.parent.parent / 'edit-yaml' / 'scripts' / 'add_node.py'

        if not script_path.exists():
            print(f"错误：找不到 add_node.py 脚本")
            print(f"预期路径：{script_path}")
            return False

        cmd = [
            'python',
            str(script_path),
            '-f', yaml_file,
            '-p', parent_path,
            '-n', node_spec
        ]

        try:
            print(f"\n执行命令：")
            print(f"python {script_path.name} -f {yaml_file} -p {parent_path} -n \"{node_spec}\"")
            print(f"\n{'='*60}\n")

            result = subprocess.run(cmd, capture_output=False, text=True)

            if result.returncode == 0:
                print(f"\n[√] 节点添加成功！")
                return True
            else:
                print(f"\n[X] 节点添加失败")
                return False

        except Exception as e:
            print(f"错误：执行 add_node.py 失败: {e}")
            return False


def print_node_info(constructor, content):
    """
    打印节点信息

    Args:
        constructor: 节点构造器
        content: 节点内容
    """
    print(f"\n{'='*60}")
    print(f"节点构造结果")
    print(f"{'='*60}\n")
    print(f"节点类型: {constructor.node_type} ({constructor.NODE_TYPES.get(constructor.node_type, '未知')})")
    print(f"节点名称: {constructor.full_name}")
    print(f"父路径:   {constructor.get_parent_path()}")
    print(f"\n节点内容：")
    print(constructor.format_node_yaml(content))


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='YAML 节点构造工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例用法：
  # 构造节点（仅显示，不添加到文件）
  python create_node.py -n pkg_common_utils

  # 构造节点并添加到 YAML 文件
  python create_node.py -n pkg_common_utils -f ci_config/rule.yaml

  # 指定父路径
  python create_node.py -n compile_core_module -f ci_config/rule.yaml -p CB.tasks.compile

节点命名规范：
  - 格式：<类型>_<名称>_<可选后缀>
  - 类型：pkg, compile, test, llt, deploy
  - 必须使用小写字母和下划线
  - 不能以数字开头
        """
    )

    parser.add_argument('-n', '--name', required=True, help='节点名称（必需）')
    parser.add_argument('-f', '--file', help='YAML 文件路径（可选，指定则添加到文件）')
    parser.add_argument('-p', '--parent', help='父路径（可选，默认自动推断）')
    parser.add_argument('--dry-run', action='store_true', help='仅显示节点内容，不执行添加')
    parser.add_argument('-v', '--verbose', action='store_true', help='显示详细信息')

    args = parser.parse_args()

    # 创建构造器
    constructor = NodeConstructor()

    # 解析节点名称
    if not constructor.parse_node_name(args.name):
        sys.exit(1)

    # 生成节点内容
    content = constructor.generate_node_content()

    # 打印节点信息
    print_node_info(constructor, content)

    # 如果指定了文件且不是 dry-run，则添加到文件
    if args.file and not args.dry_run:
        constructor.add_to_yaml(args.file, content)
    elif args.dry_run or not args.file:
        print(f"\n[提示] 这是预览模式，节点未添加到文件")
        print(f"如需添加到文件，请使用 -f 参数")


if __name__ == '__main__':
    main()
