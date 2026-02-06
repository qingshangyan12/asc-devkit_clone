#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
YAML 文件结构分析工具

功能：
1. 节点类型分布统计
2. 命名规范检查
3. 配置项完整性分析
4. 内容规则提取
"""

import argparse
import sys
import re
from collections import defaultdict, Counter
from pathlib import Path

try:
    import yaml
except ImportError:
    print("错误：未安装 PyYAML 模块")
    print("请执行: pip3 install PyYAML")
    sys.exit(1)


class YamlStructureAnalyzer:
    """YAML 结构分析器"""

    def __init__(self, yaml_path):
        """
        初始化分析器

        Args:
            yaml_path: YAML 文件路径
        """
        self.yaml_path = Path(yaml_path)
        self.data = None
        self.nodes = []  # 存储所有节点信息
        self.node_types = defaultdict(list)  # 按类型分组的节点

    def load_yaml(self):
        """加载 YAML 文件"""
        try:
            with open(self.yaml_path, 'r', encoding='utf-8') as f:
                self.data = yaml.safe_load(f)
            return True
        except Exception as e:
            print(f"错误：加载 YAML 文件失败: {e}")
            return False

    def extract_nodes(self, data, parent_path=""):
        """
        递归提取所有节点

        Args:
            data: YAML 数据
            parent_path: 父路径
        """
        if isinstance(data, dict):
            for key, value in data.items():
                current_path = f"{parent_path}.{key}" if parent_path else key

                # 如果是字典且有内容，视为一个节点
                if isinstance(value, dict) and value:
                    node_info = {
                        'name': key,
                        'path': current_path,
                        'content': value,
                        'keys': list(value.keys())
                    }
                    self.nodes.append(node_info)

                    # 判断节点类型
                    node_type = self._infer_node_type(key, value)
                    self.node_types[node_type].append(node_info)

                # 递归处理子节点
                self.extract_nodes(value, current_path)

    def _infer_node_type(self, name, content):
        """
        推断节点类型

        Args:
            name: 节点名称
            content: 节点内容

        Returns:
            节点类型
        """
        # 优先从节点名称判断
        name_lower = name.lower()
        for prefix in ['pkg', 'compile', 'test', 'llt', 'deploy']:
            if name_lower.startswith(prefix):
                return prefix

        # 从内容中的 type 字段判断
        if isinstance(content, dict) and 'type' in content:
            return str(content['type'])

        # 默认类型
        return 'misc'

    def analyze_type_distribution(self):
        """分析节点类型分布"""
        print("\n" + "="*60)
        print("节点类型分布统计")
        print("="*60)

        total = len(self.nodes)
        if total == 0:
            print("未找到任何节点")
            return

        # 按类型统计
        type_counts = {k: len(v) for k, v in self.node_types.items()}

        # 排序
        sorted_types = sorted(type_counts.items(), key=lambda x: x[1], reverse=True)

        print(f"\n总计：{total} 个节点\n")
        for node_type, count in sorted_types:
            percentage = (count / total) * 100
            print(f"├─ {node_type}: {count} 个节点 ({percentage:.1f}%)")

        # 详细列表
        print(f"\n{'节点类型':<15} {'数量':<10} {'节点列表'}")
        print("-" * 60)
        for node_type, count in sorted_types:
            node_names = [node['name'] for node in self.node_types[node_type]]
            names_str = ", ".join(node_names[:5])
            if len(node_names) > 5:
                names_str += f" ... (共{len(node_names)}个)"
            print(f"{node_type:<15} {count:<10} {names_str}")

    def check_naming_convention(self):
        """检查命名规范"""
        print("\n" + "="*60)
        print("命名规范检查")
        print("="*60)

        total = len(self.nodes)
        valid_count = 0
        invalid_nodes = []

        for node in self.nodes:
            name = node['name']
            issues = []

            # 检查规则1：必须使用小写字母和下划线
            if not re.match(r'^[a-z][a-z0-9_]*$', name):
                issues.append("未使用小写字母和下划线")

            # 检查规则2：不能以数字开头
            if re.match(r'^\d', name):
                issues.append("以数字开头")

            # 检查规则3：不能使用连字符
            if '-' in name:
                issues.append("使用连字符")

            # 检查规则4：建议使用类型前缀
            has_type_prefix = any(name.startswith(prefix) for prefix in ['pkg_', 'compile_', 'test_', 'llt_', 'deploy_'])
            if not has_type_prefix:
                issues.append("缺少类型前缀（pkg/compile/test/llt/deploy）")

            if not issues:
                valid_count += 1
            else:
                invalid_nodes.append({
                    'name': name,
                    'path': node['path'],
                    'issues': issues
                })

        # 输出结果
        print(f"\n命名规范检查结果：")
        print(f"[√] 符合规范：{valid_count} 个节点 ({(valid_count/total*100):.1f}%)")
        print(f"[X] 不符合规范：{len(invalid_nodes)} 个节点 ({(len(invalid_nodes)/total*100):.1f}%)")

        if invalid_nodes:
            print(f"\n不规范节点列表：")
            for i, node in enumerate(invalid_nodes, 1):
                print(f"\n{i}. {node['name']}")
                print(f"   路径: {node['path']}")
                print(f"   问题:")
                for issue in node['issues']:
                    print(f"   - {issue}")

    def analyze_config_completeness(self):
        """分析配置项完整性"""
        print("\n" + "="*60)
        print("配置项完整性分析")
        print("="*60)

        # 定义每种类型的必需配置项
        required_keys = {
            'pkg': ['type', 'target'],
            'compile': ['type', 'source'],
            'test': ['type', 'case'],
            'llt': ['type', 'module'],
            'deploy': ['type', 'target'],
            'misc': []
        }

        # 通用配置项（建议有）
        common_keys = ['version', 'enabled', 'timeout']

        total_checked = 0
        incomplete_nodes = []

        for node_type, nodes in self.node_types.items():
            required = required_keys.get(node_type, [])
            if not required and node_type != 'misc':
                continue

            for node in nodes:
                total_checked += 1
                content = node['content']
                missing = []

                # 检查必需配置项
                for key in required:
                    if key not in content:
                        missing.append(f"必需项缺失: {key}")

                # 检查通用配置项
                for key in common_keys:
                    if key not in content:
                        missing.append(f"建议项缺失: {key}")

                if missing:
                    incomplete_nodes.append({
                        'name': node['name'],
                        'type': node_type,
                        'missing': missing
                    })

        # 输出结果
        if not incomplete_nodes:
            print(f"\n[√] 所有 {total_checked} 个节点的配置项都是完整的")
        else:
            print(f"\n[X] {len(incomplete_nodes)} 个节点配置不完整（共检查 {total_checked} 个节点）\n")
            for i, node in enumerate(incomplete_nodes, 1):
                print(f"{i}. {node['name']} (类型: {node['type']})")
                for miss in node['missing']:
                    print(f"   - {miss}")

    def extract_mapping_rules(self):
        """提取节点命名和内容的映射规则"""
        print("\n" + "="*60)
        print("节点内容映射规则提取")
        print("="*60)

        for node_type, nodes in self.node_types.items():
            if not nodes:
                continue

            print(f"\n### {node_type} 类型节点规则")
            print("-" * 60)

            # 统计所有配置项的出现频率
            key_counter = Counter()
            for node in nodes:
                key_counter.update(node['keys'])

            # 按频率排序
            total_nodes = len(nodes)
            print(f"\n基于 {total_nodes} 个 {node_type} 节点的统计：\n")

            print(f"{'配置项':<20} {'出现次数':<10} {'占比':<10} {'必需性'}")
            print("-" * 60)

            for key, count in key_counter.most_common():
                percentage = (count / total_nodes) * 100
                necessity = "必需" if count == total_nodes else "可选"
                print(f"{key:<20} {count:<10} {percentage:>6.1f}%    {necessity}")

            # 提取典型模板
            print(f"\n推荐内容模板：")
            template = self._generate_template(nodes, key_counter)
            print(template)

    def _generate_template(self, nodes, key_counter):
        """
        生成节点内容模板

        Args:
            nodes: 节点列表
            key_counter: 配置项统计

        Returns:
            模板字符串
        """
        if not nodes:
            return ""

        # 找一个最有代表性的节点（包含最多高频配置项）
        best_node = max(nodes, key=lambda n: sum(1 for k in n['keys'] if key_counter[k] > len(nodes)/2))

        template_lines = [f"{best_node['name']}_<任务名>:"]
        for key in best_node['keys']:
            value = best_node['content'][key]
            value_str = self._format_value(value, key)
            template_lines.append(f"  {key}: {value_str}")

        return "\n".join(template_lines)

    def _format_value(self, value, key):
        """
        格式化值为模板形式

        Args:
            value: 原始值
            key: 配置项键名

        Returns:
            格式化后的字符串
        """
        if key == 'version':
            return 'cann'
        elif key == 'enabled':
            return 'true'
        elif key == 'timeout':
            return '3600'
        elif isinstance(value, list):
            if value:
                return f"[{value[0]}]"
            return '[]'
        elif isinstance(value, dict):
            return '{...}'
        else:
            return f'<{key}_值>'

    def generate_summary_report(self):
        """生成总结报告"""
        print("\n" + "="*60)
        print("结构分析总结报告")
        print("="*60)

        print(f"\n文件: {self.yaml_path}")
        print(f"总节点数: {len(self.nodes)}")
        print(f"节点类型数: {len(self.node_types)}")

        print(f"\n节点类型分布:")
        for node_type, nodes in sorted(self.node_types.items(), key=lambda x: len(x[1]), reverse=True):
            print(f"  - {node_type}: {len(nodes)} 个")

        # 命名规范概览
        valid_count = sum(1 for node in self.nodes if re.match(r'^[a-z][a-z0-9_]*$', node['name']))
        print(f"\n命名规范合规率: {(valid_count/len(self.nodes)*100):.1f}%")

        print("\n" + "="*60)


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='YAML 文件结构分析工具',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例用法：
  # 完整分析
  python analyze_yaml_structure.py -f ci_config/rule.yaml

  # 仅分析节点类型分布
  python analyze_yaml_structure.py -f ci_config/rule.yaml --type-distribution

  # 仅检查命名规范
  python analyze_yaml_structure.py -f ci_config/rule.yaml --check-naming

  # 仅提取映射规则
  python analyze_yaml_structure.py -f ci_config/rule.yaml --extract-rules
        """
    )

    parser.add_argument('-f', '--file', required=True, help='YAML 文件路径')
    parser.add_argument('--type-distribution', action='store_true', help='仅输出节点类型分布')
    parser.add_argument('--check-naming', action='store_true', help='仅检查命名规范')
    parser.add_argument('--check-config', action='store_true', help='仅检查配置项完整性')
    parser.add_argument('--extract-rules', action='store_true', help='仅提取内容映射规则')
    parser.add_argument('--summary', action='store_true', help='仅输出总结报告')

    args = parser.parse_args()

    # 创建分析器
    analyzer = YamlStructureAnalyzer(args.file)

    # 加载 YAML 文件
    if not analyzer.load_yaml():
        sys.exit(1)

    # 提取节点
    analyzer.extract_nodes(analyzer.data)

    if len(analyzer.nodes) == 0:
        print("警告：未在文件中找到任何节点")
        sys.exit(0)

    # 根据参数执行相应操作
    if args.summary:
        analyzer.generate_summary_report()
    elif args.type_distribution:
        analyzer.analyze_type_distribution()
    elif args.check_naming:
        analyzer.check_naming_convention()
    elif args.check_config:
        analyzer.analyze_config_completeness()
    elif args.extract_rules:
        analyzer.extract_mapping_rules()
    else:
        # 默认执行所有分析
        analyzer.analyze_type_distribution()
        analyzer.check_naming_convention()
        analyzer.analyze_config_completeness()
        analyzer.extract_mapping_rules()
        analyzer.generate_summary_report()

    print("\n分析完成！")


if __name__ == '__main__':
    main()
