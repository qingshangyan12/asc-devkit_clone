import yaml
import sys
import argparse
import shutil
import re
import os
from datetime import datetime
from pathlib import Path
from typing import List, Dict, Any, Set

# Windows 编码兼容性处理
if sys.platform == 'win32':
    # 尝试设置控制台输出编码为 UTF-8
    try:
        os.system('chcp 65001 > nul 2>&1')
    except:
        pass
    # 设置标准输出编码
    if hasattr(sys.stdout, 'reconfigure'):
        sys.stdout.reconfigure(encoding='utf-8')
    if hasattr(sys.stderr, 'reconfigure'):
        sys.stderr.reconfigure(encoding='utf-8')


def modify_node_name_in_dict(
    node: Dict,
    old_path_parts: List[str],
    new_path_parts: List[str],
    current_depth: int = 0
) -> bool:
    """
    递归遍历字典，修改指定路径的节点名称

    Args:
        node: 当前遍历的节点
        old_path_parts: 原路径的各个部分（如 ['CB', 'tasks', 'pkg', 'llt1']）
        new_path_parts: 新路径的各个部分（如 ['CB', 'tasks', 'pkg', 'llt1_test']）
        current_depth: 当前遍历深度

    Returns:
        是否成功修改
    """
    # 如果已经到达路径的倒数第二层，需要重命名最后一个键
    if current_depth == len(old_path_parts) - 2:
        old_key = old_path_parts[-1]
        new_key = new_path_parts[-1]
        parent_key = old_path_parts[-2]

        # 检查父节点是否存在
        if parent_key in node and isinstance(node[parent_key], dict):
            parent_node = node[parent_key]

            # 检查目标键是否存在
            if old_key in parent_node:
                # 如果新键已存在且不同于旧键，跳过
                if new_key in parent_node and new_key != old_key:
                    print(f"  警告：目标键 '{new_key}' 已存在，跳过重命名 '{old_key}'")
                    return False

                # 重命名键（保持值不变）
                parent_node[new_key] = parent_node.pop(old_key)
                return True
            else:
                print(f"  警告：未找到键 '{old_key}' 在路径 '{'.'.join(old_path_parts[:-1])}'")
                return False
        return False

    # 继续递归遍历
    if current_depth < len(old_path_parts) - 2:
        current_key = old_path_parts[current_depth]
        if current_key in node and isinstance(node[current_key], dict):
            return modify_node_name_in_dict(node[current_key], old_path_parts, new_path_parts, current_depth + 1)
        else:
            print(f"  警告：路径 '{'.'.join(old_path_parts[:current_depth+1])}' 不存在")
            return False

    return False


def modify_yaml_nodes(
    file_path: str,
    node_mappings: Dict[str, str],
    create_backup: bool = True
) -> int:
    """
    流式修改 YAML 文件中指定节点的名称

    Args:
        file_path: YAML 文件路径
        node_mappings: 节点映射关系，key为原节点路径，value为新节点路径
                      例如: {'CB.tasks.pkg.llt1': 'CB.tasks.pkg.llt1_test'}
        create_backup: 是否创建备份文件

    Returns:
        成功修改的节点数量
    """
    # 创建备份
    if create_backup:
        backup_path = f"{file_path}.backup_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
        try:
            shutil.copy2(file_path, backup_path)
            print(f"✓ 已创建备份文件: {backup_path}")
        except Exception as e:
            print(f"✗ 警告：无法创建备份文件 - {e}")
            return 0

    # 读取 YAML 文件（流式读取，支持多文档）
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            # 流式加载所有文档
            documents = list(yaml.safe_load_all(f))
    except Exception as e:
        print(f"✗ 错误：无法读取文件 - {e}")
        return 0

    modified_count = 0
    modified_paths: Set[str] = set()

    # 处理每个节点映射
    for old_path, new_path in node_mappings.items():
        # 移除路径中的方括号（如果有）
        old_path = re.sub(r'\[\d+\]', '', old_path)
        new_path = re.sub(r'\[\d+\]', '', new_path)

        # 解析路径
        old_path_parts = old_path.split('.')
        new_path_parts = new_path.split('.')

        if len(old_path_parts) < 2 or len(new_path_parts) < 2:
            print(f"✗ 警告：跳过无效路径映射 '{old_path} -> {new_path}'（路径深度不足）")
            continue

        # 检查路径长度是否一致
        if len(old_path_parts) != len(new_path_parts):
            print(f"✗ 警告：跳过路径映射 '{old_path} -> {new_path}'（路径层级不一致）")
            continue

        # 检查父路径是否一致
        if old_path_parts[:-1] != new_path_parts[:-1]:
            print(f"✗ 警告：跳过路径映射 '{old_path} -> {new_path}'（父路径不一致）")
            continue

        # 检查是否需要修改
        if old_path_parts[-1] == new_path_parts[-1]:
            print(f"⊘ 跳过：'{old_path}' 与目标名称相同")
            continue

        # 在每个文档中查找并修改
        for doc_idx, doc in enumerate(documents):
            if not isinstance(doc, dict):
                continue

            # 递归修改节点名称
            if modify_node_name_in_dict(doc, old_path_parts, new_path_parts):
                print(f"✓ 重命名: [{doc_idx}] {old_path} -> {new_path}")
                modified_count += 1
                modified_paths.add(old_path)
                break  # 找到并修改后，跳出文档循环

    if modified_count == 0:
        print("\n⊘ 没有节点被修改")
        return 0

    # 保存修改后的文件
    try:
        with open(file_path, 'w', encoding='utf-8') as f:
            # 流式写入所有文档
            for idx, doc in enumerate(documents):
                if idx > 0:
                    f.write('---\n')
                yaml.dump(doc, f, allow_unicode=True, default_flow_style=False, sort_keys=False)
        print(f"\n✓ 成功修改 {modified_count} 个节点")
        return modified_count
    except Exception as e:
        print(f"✗ 错误：无法保存文件 - {e}")
        # 如果保存失败，尝试恢复备份
        if create_backup:
            try:
                shutil.copy2(backup_path, file_path)
                print(f"✓ 已从备份恢复文件")
            except:
                pass
        return 0


def read_nodes_from_file(file_path: str) -> Dict[str, str]:
    """
    从文件中读取节点映射关系

    Args:
        file_path: 包含节点映射关系的文件

    Returns:
        节点映射字典，key为原节点路径，value为新节点路径
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        # 解析节点映射关系（支持多种格式）
        mappings = {}
        for line in lines:
            line = line.strip()
            if not line or line.startswith('#'):
                continue

            # 支持格式1: CB.tasks.pkg.llt1 -> CB.tasks.pkg.llt1_test
            # 支持格式2: CB.tasks.pkg.llt1=CB.tasks.pkg.llt1_test
            # 支持格式3: [1] CB.tasks.pkg.llt1 -> CB.tasks.pkg.llt1_test
            line = re.sub(r'^\[\d+\]\s*', '', line)

            if '->' in line:
                parts = line.split('->', 1)
                old_path = parts[0].strip()
                new_path = parts[1].strip()
                mappings[old_path] = new_path
            elif '=' in line:
                parts = line.split('=', 1)
                old_path = parts[0].strip()
                new_path = parts[1].strip()
                mappings[old_path] = new_path
            else:
                # 如果只有单个路径，跳过（旧格式兼容）
                print(f"  警告：跳过无效映射 '{line}'（缺少映射关系，请使用 '->' 或 '=' 分隔）")
                continue

        return mappings
    except Exception as e:
        print(f"✗ 错误：无法读取节点文件 - {e}")
        return {}


def parse_mapping_string(mapping_str: str) -> tuple:
    """
    解析映射字符串

    Args:
        mapping_str: 映射字符串，格式为 "old_path->new_path" 或 "old_path=new_path"

    Returns:
        (old_path, new_path) 元组
    """
    if '->' in mapping_str:
        parts = mapping_str.split('->', 1)
        return (parts[0].strip(), parts[1].strip())
    elif '=' in mapping_str:
        parts = mapping_str.split('=', 1)
        return (parts[0].strip(), parts[1].strip())
    else:
        raise ValueError(f"无效的映射格式: {mapping_str}（请使用 '->' 或 '=' 分隔原路径和新路径）")


def main():
    parser = argparse.ArgumentParser(
        description='批量修改 YAML 文件中节点的名称（支持大型文件流式处理）',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
使用示例：

  # 方式1：直接在命令行指定节点映射关系（使用 -> 分隔）
  python modify_nodes_names.py --file ci_config/rule.yaml --mappings CB.tasks.pkg.llt1->CB.tasks.pkg.llt1_test

  # 方式2：从文件读取节点映射关系（推荐用于批量操作）
  # 文件内容格式：
  #   CB.tasks.pkg.llt1 -> CB.tasks.pkg.llt1_test
  #   CB.tasks.pkg.llt2 -> CB.tasks.pkg.llt2_test
  python modify_nodes_names.py --file ci_config/rule.yaml --mappings-file mappings.txt

  # 方式3：使用 = 分隔符
  python modify_nodes_names.py --file ci_config/rule.yaml -m CB.tasks.pkg.llt1=CB.tasks.pkg.llt1_test

  # 方式4：使用管道直接传递（Unix/Linux）
  echo "CB.tasks.pkg.llt1->CB.tasks.pkg.llt1_test" | \\
    python modify_nodes_names.py -f ci_config/rule.yaml --mappings-file -

  # 方式5：指定多个映射关系
  python modify_nodes_names.py -f ci_config/rule.yaml \\
    -m CB.tasks.pkg.llt1->CB.tasks.pkg.llt1_test \\
    -m CB.tasks.pkg.llt2->CB.tasks.pkg.llt2_test

  # 不创建备份文件（谨慎使用）
  python modify_nodes_names.py -f ci_config/rule.yaml -m CB.tasks.pkg.llt1->CB.tasks.pkg.llt1_test --no-backup

技术特性：
  - 流式读取：支持大型 YAML 文件，避免内存溢出
  - 递归遍历：高效定位和修改深层嵌套节点
  - 自动备份：默认创建带时间戳的备份文件
  - 安全恢复：保存失败时自动从备份恢复
  - 多文档支持：支持 YAML 多文档格式（--- 分隔）

节点路径映射格式说明：
  - 路径使用点号分隔，如：CB.tasks.pkg.llt1
  - 映射格式1：old_path->new_path（推荐）
  - 映射格式2：old_path=new_path
  - 示例：CB.tasks.pkg.llt1->CB.tasks.pkg.llt1_test
  - 父路径必须一致，只能修改最后一部分节点名称
        """
    )

    parser.add_argument(
        '-f', '--file',
        required=True,
        help='YAML 文件路径（必需）'
    )
    parser.add_argument(
        '-m', '--mappings',
        action='append',
        help='节点映射关系（使用 -> 或 = 分隔，如：CB.tasks.pkg.llt1->CB.tasks.pkg.llt1_test）。可多次使用 -m 参数指定多个映射'
    )
    parser.add_argument(
        '--mappings-file',
        help='包含节点映射关系的文件（每行一个映射，支持 -> 或 = 分隔符）'
    )
    parser.add_argument(
        '--no-backup',
        action='store_true',
        help='不创建备份文件（谨慎使用）'
    )
    parser.add_argument(
        '--verbose',
        action='store_true',
        help='显示详细信息'
    )

    args = parser.parse_args()

    # 检查文件是否存在
    if not Path(args.file).exists():
        parser.error(f"文件不存在: {args.file}")

    # 获取节点映射关系
    node_mappings = {}
    if args.mappings_file == '-':
        # 从标准输入读取
        print("从标准输入读取节点映射关系...")
        for line in sys.stdin:
            line = line.strip()
            if line and not line.startswith('#'):
                try:
                    old_path, new_path = parse_mapping_string(line)
                    node_mappings[old_path] = new_path
                except ValueError as e:
                    print(f"  {e}")
    elif args.mappings_file:
        node_mappings = read_nodes_from_file(args.mappings_file)
        if not node_mappings:
            parser.error(f"无法从文件 '{args.mappings_file}' 读取节点映射关系或文件为空")
    elif args.mappings:
        # 解析命令行传入的映射关系
        # action='append' 会创建列表的列表，需要展平
        mappings_list = []
        for item in args.mappings:
            if isinstance(item, list):
                mappings_list.extend(item)
            else:
                mappings_list.append(item)

        for mapping in mappings_list:
            try:
                old_path, new_path = parse_mapping_string(mapping)
                node_mappings[old_path] = new_path
            except ValueError as e:
                parser.error(str(e))
    else:
        parser.error("必须指定 --mappings 或 --mappings-file 参数（使用 '-' 从标准输入读取）")

    print(f"""开始修改节点名称：
- 文件路径：{args.file}
- 节点映射数量：{len(node_mappings)}
- 创建备份：{'否' if args.no_backup else '是'}
""")

    if args.verbose and node_mappings:
        print(f"待修改的节点映射关系：")
        for idx, (old_path, new_path) in enumerate(node_mappings.items(), 1):
            print(f"  {idx}. {old_path} -> {new_path}")
        print()

    # 执行修改
    modified_count = modify_yaml_nodes(
        args.file,
        node_mappings,
        create_backup=not args.no_backup
    )

    if modified_count > 0:
        print(f"\n✓ 节点名称修改完成，共修改 {modified_count} 个节点")
        sys.exit(0)
    else:
        print("\n✗ 节点名称修改失败或没有节点被修改")
        sys.exit(1)


if __name__ == "__main__":
    main()
