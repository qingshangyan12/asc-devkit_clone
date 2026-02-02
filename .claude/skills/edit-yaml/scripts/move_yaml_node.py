#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
YAML节点批量移动工具

功能：
1. 将查找的节点批量移动到新的父节点路径下
2. 支持从文件或命令行参数读取节点路径列表
3. 支持管道操作，可直接配合 find_yaml_node.py 使用
4. 保持原有节点内容和结构
5. 自动备份和错误恢复

使用示例：
# 方式1：配合 find_yaml_node.py 使用（推荐）
python find_yaml_node.py -f ci_config/rule.yaml -r CB -k version -v cann > nodes.txt
python move_yaml_node.py -f ci_config/rule.yaml --nodes-file nodes.txt --target CB.tasks.pkg

# 方式2：直接指定节点路径
python move_yaml_node.py -f ci_config/rule.yaml --nodes CB.tasks.llt1 CB.tasks.llt2 -t CB.tasks.pkg

# 方式3：使用管道
python find_yaml_node.py -f ci_config/rule.yaml -r CB -k version -v cann | \\
  python move_yaml_node.py -f ci_config/rule.yaml --nodes-file - -t CB.tasks.pkg
"""

import argparse
import yaml
import sys
import os
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List, Set, Tuple
import copy


class YAMLNodeMover:
    """YAML节点移动器"""

    def __init__(self, file_path: str, target_parent: str, suffix: str = None, addmode: bool = False, create_backup: bool = True):
        """
        初始化节点移动器

        Args:
            file_path: YAML文件路径
            target_parent: 目标父节点路径（如 CB.tasks.pkg）
            suffix: 任务名后缀（可选）
            addmode: 后缀添加模式（True=直接添加，False=替换最后一个"_"后的部分）
            create_backup: 是否创建备份文件
        """
        self.file_path = file_path
        self.target_parent = target_parent
        self.suffix = suffix
        self.addmode = addmode
        self.create_backup = create_backup
        self.backup_path = None
        self.data = None
        self.moved_count = 0
        self.failed_nodes = []

    def _create_backup(self) -> bool:
        """创建备份文件"""
        if not self.create_backup:
            return True

        try:
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            self.backup_path = f"{self.file_path}.backup_{timestamp}"

            with open(self.file_path, 'r', encoding='utf-8') as src:
                with open(self.backup_path, 'w', encoding='utf-8') as dst:
                    dst.write(src.read())

            print(f"[OK] 已创建备份文件: {self.backup_path}")
            return True
        except Exception as e:
            print(f"[ERROR] 创建备份文件失败: {e}")
            return False

    def _restore_from_backup(self) -> bool:
        """从备份文件恢复"""
        if not self.backup_path or not os.path.exists(self.backup_path):
            return False

        try:
            with open(self.backup_path, 'r', encoding='utf-8') as src:
                with open(self.file_path, 'w', encoding='utf-8') as dst:
                    dst.write(src.read())
            print(f"[OK] 已从备份文件恢复: {self.backup_path}")
            return True
        except Exception as e:
            print(f"[ERROR] 从备份文件恢复失败: {e}")
            return False

    def load_yaml(self) -> bool:
        """加载YAML文件"""
        try:
            with open(self.file_path, 'r', encoding='utf-8') as f:
                self.data = yaml.safe_load(f)

            if self.data is None:
                self.data = {}

            return True
        except Exception as e:
            print(f"[ERROR] 加载YAML文件失败: {e}")
            return False

    def _parse_path(self, path: str) -> List[str]:
        """解析节点路径为键列表"""
        if not path:
            return []
        return path.split('.')

    def _get_node_by_path(self, data: Any, path_parts: List[str]) -> Tuple[Any, List[str]]:
        """
        根据路径获取节点

        Args:
            data: 数据对象
            path_parts: 路径部分列表

        Returns:
            (节点对象, 剩余路径)
        """
        if not path_parts:
            return data, []

        current = data
        for i, key in enumerate(path_parts):
            if isinstance(current, dict):
                if key in current:
                    if i == len(path_parts) - 1:
                        # 找到目标节点
                        return current[key], []
                    current = current[key]
                else:
                    # 路径不存在，返回当前节点和剩余路径
                    return current, path_parts[i:]
            elif isinstance(current, list):
                try:
                    idx = int(key)
                    if 0 <= idx < len(current):
                        if i == len(path_parts) - 1:
                            return current[idx], []
                        current = current[idx]
                    else:
                        return current, path_parts[i:]
                except ValueError:
                    return current, path_parts[i:]
            else:
                return current, path_parts[i:]

        return current, []

    def _set_node_by_path(self, data: Dict, path_parts: List[str], value: Any) -> bool:
        """
        根据路径设置节点值

        Args:
            data: 数据对象
            path_parts: 路径部分列表
            value: 要设置的值

        Returns:
            是否设置成功
        """
        if not path_parts:
            return False

        current = data
        for i, key in enumerate(path_parts[:-1]):
            if key not in current:
                # 创建中间节点
                current[key] = {}
            current = current[key]

            if not isinstance(current, dict):
                print(f"[ERROR] 路径 {'.'.join(path_parts[:i+1])} 不是字典类型，无法创建子节点")
                return False

        # 设置最终值
        current[path_parts[-1]] = value
        return True

    def _delete_node_by_path(self, data: Dict, path_parts: List[str]) -> bool:
        """
        根据路径删除节点

        Args:
            data: 数据对象
            path_parts: 路径部分列表

        Returns:
            是否删除成功
        """
        if not path_parts:
            return False

        current = data
        for i, key in enumerate(path_parts[:-1]):
            if isinstance(current, dict) and key in current:
                current = current[key]
            else:
                print(f"[ERROR] 路径 {'.'.join(path_parts[:i+1])} 不存在")
                return False

        # 删除最终节点
        if isinstance(current, dict) and path_parts[-1] in current:
            del current[path_parts[-1]]
            return True
        else:
            print(f"[ERROR] 节点 {path_parts[-1]} 不存在")
            return False

    def _rename_node(self, data: Dict, path_parts: List[str], old_name: str, new_name: str) -> bool:
        """
        在指定路径下重命名节点（原地重命名）

        Args:
            data: 数据对象
            path_parts: 父节点路径部分列表
            old_name: 旧节点名称
            new_name: 新节点名称

        Returns:
            是否重命名成功
        """
        if not path_parts:
            return False

        # 找到父节点
        current = data
        for key in path_parts:
            if isinstance(current, dict) and key in current:
                current = current[key]
            else:
                print(f"[ERROR] 父路径 {'.'.join(path_parts)} 不存在")
                return False

        # 检查父节点是否为字典
        if not isinstance(current, dict):
            print(f"[ERROR] 父节点不是字典类型")
            return False

        # 检查旧节点是否存在
        if old_name not in current:
            print(f"[ERROR] 节点 {old_name} 不存在")
            return False

        # 检查新节点名是否已存在
        if new_name in current:
            print(f"[WARNING] 目标节点名 {new_name} 已存在，将被覆盖")
            # 删除已存在的节点（覆盖）
            del current[new_name]

        # 重命名节点
        current[new_name] = current[old_name]
        del current[old_name]

        return True

    def _ensure_target_parent(self, data: Dict) -> bool:
        """确保目标父节点路径存在"""
        path_parts = self._parse_path(self.target_parent)

        current = data
        for key in path_parts:
            if key not in current:
                current[key] = {}
            elif not isinstance(current[key], dict):
                print(f"[ERROR] 目标路径 {self.target_parent} 中的 {key} 不是字典类型")
                return False
            current = current[key]

        return True

    def move_node(self, node_path: str) -> bool:
        """
        移动单个节点到目标父路径

        Args:
            node_path: 源节点路径

        Returns:
            是否移动成功
        """
        source_parts = self._parse_path(node_path)

        if not source_parts:
            print(f"[ERROR] 无效的源节点路径: {node_path}")
            return False

        # 获取源节点名称
        source_name = source_parts[-1]

        # 应用后缀逻辑
        dest_name = source_name
        if self.suffix:
            if self.addmode:
                # 添加模式：先增加下划线，再追加新后缀
                dest_name = source_name + '_' + self.suffix
            else:
                # 替换模式：替换最后一个"_"后的部分
                last_underscore_pos = source_name.rfind('_')
                if last_underscore_pos > 0:
                    # 保留基础名称（最后一个"_"之前的部分）
                    base_name = source_name[:last_underscore_pos]
                    # 替换为新后缀
                    dest_name = base_name + '_' + self.suffix
                else:
                    # 如果没有下划线，添加下划线和新后缀
                    dest_name = source_name + '_' + self.suffix

        print(f"  正在移动: {node_path} -> {self.target_parent}.{dest_name}")

        # 找到源节点的父节点
        parent_parts = source_parts[:-1]
        source_parent, remaining = self._get_node_by_path(self.data, parent_parts)

        if remaining:
            print(f"    [ERROR] 源节点路径不存在（剩余路径: {'.'.join(remaining)}）")
            self.failed_nodes.append(node_path)
            return False

        # 检查源节点是否存在
        if not isinstance(source_parent, dict) or source_name not in source_parent:
            print(f"    [ERROR] 源节点 {source_name} 不存在")
            self.failed_nodes.append(node_path)
            return False

        # 步骤1：如果需要修改节点名，先在原位置重命名
        current_node_name = source_name
        if dest_name != source_name:
            print(f"    [步骤1] 在原位置重命名: {source_name} -> {dest_name}")
            if not self._rename_node(self.data, parent_parts, source_name, dest_name):
                print(f"    [ERROR] 原位置重命名失败")
                self.failed_nodes.append(node_path)
                return False
            current_node_name = dest_name
            print(f"    [OK] 原位置重命名成功")

        # 步骤2：复制重命名后的节点内容
        node_content = copy.deepcopy(source_parent[current_node_name])

        # 步骤3：确保目标父路径存在
        if not self._ensure_target_parent(self.data):
            print(f"    [ERROR] 无法创建目标父路径")
            self.failed_nodes.append(node_path)
            return False

        # 步骤4：构建目标路径并检查是否已存在同名节点
        target_parts = self._parse_path(self.target_parent) + [dest_name]
        target_parent_obj, _ = self._get_node_by_path(self.data, self._parse_path(self.target_parent))

        # 检查目标位置是否已存在同名节点
        if isinstance(target_parent_obj, dict) and dest_name in target_parent_obj:
            print(f"    [WARNING] 目标位置 {self.target_parent}.{dest_name} 已存在同名节点，将被覆盖")

        # 步骤5：移动到目标位置（覆盖已存在的同名节点）
        if not self._set_node_by_path(self.data, target_parts, node_content):
            print(f"    [ERROR] 无法设置到目标位置")
            self.failed_nodes.append(node_path)
            return False

        # 步骤6：从源位置删除（使用修改后的路径）
        delete_parts = parent_parts + [current_node_name]
        if not self._delete_node_by_path(self.data, delete_parts):
            print(f"    [ERROR] 无法从源位置删除")
            self.failed_nodes.append(node_path)
            return False

        print(f"    [OK] 成功移动: {node_path} -> {self.target_parent}.{dest_name}")
        self.moved_count += 1
        return True

    def move_nodes(self, node_paths: List[str]) -> bool:
        """
        批量移动节点

        Args:
            node_paths: 节点路径列表

        Returns:
            是否全部移动成功
        """
        # 创建备份
        if not self._create_backup():
            return False

        # 加载YAML文件
        if not self.load_yaml():
            return False

        print(f"\n开始移动节点：")
        print(f"- 文件路径: {self.file_path}")
        print(f"- 目标父路径: {self.target_parent}")
        print(f"- 节点数量: {len(node_paths)}")
        print()

        # 移动每个节点
        all_success = True
        for node_path in node_paths:
            if not self.move_node(node_path.strip()):
                all_success = False

        # 保存文件
        if self.moved_count > 0:
            if self.save_yaml():
                print(f"\n[OK] 节点移动完成，共移动 {self.moved_count} 个节点")

                if self.failed_nodes:
                    print(f"\n[WARNING] 以下 {len(self.failed_nodes)} 个节点移动失败：")
                    for node in self.failed_nodes:
                        print(f"  - {node}")

                return all_success
            else:
                print("\n[ERROR] 保存文件失败")
                self._restore_from_backup()
                return False
        else:
            print("\n[ERROR] 没有节点被移动")
            return False

    def save_yaml(self) -> bool:
        """保存YAML文件"""
        try:
            with open(self.file_path, 'w', encoding='utf-8') as f:
                yaml.dump(self.data, f, allow_unicode=True, default_flow_style=False, sort_keys=False)
            return True
        except Exception as e:
            print(f"[ERROR] 保存YAML文件失败: {e}")
            return False


def read_nodes_from_file(file_path: str) -> List[str]:
    """
    从文件读取节点路径列表

    Args:
        file_path: 文件路径（- 表示标准输入）

    Returns:
        节点路径列表
    """
    nodes = []

    try:
        if file_path == '-':
            # 从标准输入读取
            for line in sys.stdin:
                line = line.strip()
                if line and not line.startswith('#'):
                    nodes.append(line)
        else:
            # 从文件读取
            with open(file_path, 'r', encoding='utf-8') as f:
                for line in f:
                    line = line.strip()
                    if line and not line.startswith('#'):
                        nodes.append(line)
    except Exception as e:
        print(f"[ERROR] 读取节点文件失败: {e}")
        return []

    return nodes


def main():
    """主函数"""
    parser = argparse.ArgumentParser(
        description='批量移动YAML节点到指定路径',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog='''
使用示例：
  # 配合 find_yaml_node.py 使用（推荐）
  python find_yaml_node.py -f ci_config/rule.yaml -r CB -k version -v cann > nodes.txt
  python move_yaml_node.py -f ci_config/rule.yaml --nodes-file nodes.txt -t CB.tasks.pkg

  # 直接指定节点路径
  python move_yaml_node.py -f ci_config/rule.yaml --nodes CB.tasks.llt1 CB.tasks.llt2 -t CB.tasks.pkg

  # 使用管道
  python find_yaml_node.py -f ci_config/rule.yaml -r CB -k version -v cann | \\
    python move_yaml_node.py -f ci_config/rule.yaml --nodes-file - -t CB.tasks.pkg

  # 移动时替换后缀（默认模式）
  python move_yaml_node.py -f ci_config/rule.yaml -n CB.tasks.llt1_test -t CB.tasks.pkg --suffix prod
  # 结果：CB.tasks.llt1_test -> CB.tasks.pkg.llt1_prod

  # 移动时添加后缀
  python move_yaml_node.py -f ci_config/rule.yaml -n CB.tasks.llt1 -t CB.tasks.pkg --suffix test --addmode
  # 结果：CB.tasks.llt1 -> CB.tasks.pkg.llt1_test

  # 批量移动并替换后缀
  python find_yaml_node.py -f ci_config/rule.yaml -r CB.tasks -k version -v cann > nodes.txt
  python move_yaml_node.py -f ci_config/rule.yaml --nodes-file nodes.txt -t CB.tasks.new_pkg --suffix pkg
        '''
    )

    parser.add_argument('--file', '-f', required=True, help='YAML文件路径（必需）')
    parser.add_argument('--nodes', '-n', nargs='+', help='要移动的节点路径列表（空格分隔）')
    parser.add_argument('--nodes-file', help='包含节点路径列表的文件（每行一个路径，- 表示标准输入）')
    parser.add_argument('--target', '-t', required=True, help='目标父节点路径（必需，如 CB.tasks.pkg）')
    parser.add_argument('--suffix', help='指定任务名后缀（将应用于移动后的节点名称）')
    parser.add_argument('--addmode', action='store_true', help='后缀添加模式（true=直接添加后缀，false=替换最后一个"_"后的部分，默认为false）')
    parser.add_argument('--no-backup', action='store_true', help='不创建备份文件（谨慎使用）')

    args = parser.parse_args()

    # 检查文件是否存在
    if not os.path.exists(args.file):
        print(f"[ERROR] 文件不存在: {args.file}")
        return 1

    # 获取节点路径列表
    node_paths = []

    if args.nodes:
        node_paths.extend(args.nodes)

    if args.nodes_file:
        nodes_from_file = read_nodes_from_file(args.nodes_file)
        node_paths.extend(nodes_from_file)

    if not node_paths:
        print("[ERROR] 未指定要移动的节点")
        print("请使用 --nodes 或 --nodes-file 参数指定节点路径")
        return 1

    # 创建节点移动器
    mover = YAMLNodeMover(
        file_path=args.file,
        target_parent=args.target,
        suffix=args.suffix,
        addmode=args.addmode,
        create_backup=not args.no_backup
    )

    # 移动节点
    success = mover.move_nodes(node_paths)

    return 0 if success else 1


if __name__ == '__main__':
    sys.exit(main())
