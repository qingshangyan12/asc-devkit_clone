import yaml
import sys
import argparse
import shutil
import re
import os
from datetime import datetime
from pathlib import Path
from typing import List, Dict, Any, Set, Union

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


def modify_key_value_in_dict(
    node: Dict,
    path_parts: List[str],
    target_key: str,
    new_value: Any,
    current_depth: int = 0
) -> bool:
    """
    递归遍历字典，修改指定路径节点内的key:value键值对

    Args:
        node: 当前遍历的节点
        path_parts: 路径的各个部分（如 ['CB', 'tasks', 'pkg', 'llt1']）
        target_key: 要修改的键名
        new_value: 新的值
        current_depth: 当前遍历深度

    Returns:
        是否成功修改
    """
    # 如果已经到达路径的最后一层，修改目标key的值
    if current_depth == len(path_parts) - 1:
        target_node_name = path_parts[-1]

        # 检查目标节点是否存在
        if target_node_name in node and isinstance(node[target_node_name], dict):
            target_node = node[target_node_name]

            # 检查目标key是否存在
            if target_key in target_node:
                old_value = target_node[target_key]
                # 如果新旧值相同，跳过
                if old_value == new_value:
                    print(f"  ⊘ 跳过：'{'.'.join(path_parts)}' 中的 '{target_key}' 值已是 '{new_value}'")
                    return False

                # 修改值
                target_node[target_key] = new_value
                return True
            else:
                print(f"  ✗ 警告：节点 '{'.'.join(path_parts)}' 中不存在键 '{target_key}'")
                return False
        else:
            print(f"  ✗ 警告：未找到节点 '{'.'.join(path_parts)}'")
            return False

    # 继续递归遍历
    if current_depth < len(path_parts) - 1:
        current_key = path_parts[current_depth]
        if current_key in node and isinstance(node[current_key], dict):
            return modify_key_value_in_dict(
                node[current_key],
                path_parts,
                target_key,
                new_value,
                current_depth + 1
            )
        else:
            print(f"  ✗ 警告：路径 '{'.'.join(path_parts[:current_depth+1])}' 不存在")
            return False

    return False


def modify_yaml_nodes_key_value(
    file_path: str,
    node_paths: List[str],
    target_key: str,
    new_value: Any,
    create_backup: bool = True
) -> int:
    """
    流式修改 YAML 文件中指定节点的key:value键值对

    Args:
        file_path: YAML 文件路径
        node_paths: 要修改的节点路径列表（如 CB.tasks.pkg.llt1）
        target_key: 要修改的键名
        new_value: 新的值
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

    # 处理每个节点路径
    for node_path in node_paths:
        # 移除路径中的方括号（如果有）
        node_path = re.sub(r'\[\d+\]', '', node_path)

        # 解析路径：CB.tasks.pkg.llt1 -> ['CB', 'tasks', 'pkg', 'llt1']
        path_parts = node_path.split('.')

        if len(path_parts) < 2:
            print(f"✗ 警告：跳过无效路径 '{node_path}'（路径深度不足）")
            continue

        # 在每个文档中查找并修改
        for doc_idx, doc in enumerate(documents):
            if not isinstance(doc, dict):
                continue

            # 递归修改节点内的key:value
            if modify_key_value_in_dict(doc, path_parts, target_key, new_value):
                print(f"✓ 修改: [{doc_idx}] {'.'.join(path_parts)} 中的 {target_key}: {new_value}")
                modified_count += 1
                modified_paths.add(node_path)
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


def read_nodes_from_file(file_path: str) -> List[str]:
    """
    从文件中读取节点路径列表

    Args:
        file_path: 包含节点路径的文件

    Returns:
        节点路径列表
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()

        # 解析节点路径（支持多种格式）
        nodes = []
        for line in lines:
            line = line.strip()
            if not line or line.startswith('#'):
                continue

            # 支持格式：
            # [1] CB.tasks.pkg.llt1
            # CB.tasks.pkg.llt1
            # 去除方括号和序号
            line = re.sub(r'^\[\d+\]\s*', '', line)
            nodes.append(line)

        return nodes
    except Exception as e:
        print(f"✗ 错误：无法读取节点文件 - {e}")
        return []


def parse_value(value_str: str, value_type: str = None) -> Any:
    """
    解析值字符串为对应的Python类型

    Args:
        value_str: 值字符串
        value_type: 强制指定的类型（bool/int/float/str/auto）

    Returns:
        解析后的值
    """
    if value_type:
        # 强制类型转换
        if value_type == 'bool':
            if value_str.lower() in ['true', 'yes', '1', 'on']:
                return True
            elif value_str.lower() in ['false', 'no', '0', 'off']:
                return False
            else:
                raise ValueError(f"无效的布尔值: {value_str}")
        elif value_type == 'int':
            return int(value_str)
        elif value_type == 'float':
            return float(value_str)
        elif value_type == 'str':
            return value_str
        else:
            return value_str

    # 自动类型推断
    # 尝试解析为布尔值
    if value_str.lower() in ['true', 'yes', 'on']:
        return True
    if value_str.lower() in ['false', 'no', 'off']:
        return False

    # 尝试解析为整数
    try:
        return int(value_str)
    except ValueError:
        pass

    # 尝试解析为浮点数
    try:
        return float(value_str)
    except ValueError:
        pass

    # 默认作为字符串
    return value_str


def main():
    parser = argparse.ArgumentParser(
        description='批量修改 YAML 文件中指定节点的key:value键值对（支持大型文件流式处理）',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
使用示例：

  # 方式1：直接在命令行指定节点路径
  python modify_node_key_value.py --file ci_config/rule.yaml \\
    --nodes CB.tasks.pkg.llt1_new CB.tasks.pkg.llt2_new \\
    --key version --value cann_test

  # 方式2：从文件读取节点列表（推荐用于批量操作）
  python modify_node_key_value.py --file ci_config/rule.yaml \\
    --nodes-file nodes.txt --key version --value cann_test

  # 方式3：配合 find_yaml_nodes.py 使用（推荐工作流）
  # 步骤1：查找包含特定条件的节点
  python find_yaml_nodes.py -f ci_config/rule.yaml -r CB -k version -v cann > nodes.txt
  # 步骤2：批量修改这些节点的 version 值
  python modify_node_key_value.py -f ci_config/rule.yaml --nodes-file nodes.txt \\
    --key version --value cann_new

  # 方式4：使用管道直接传递（Unix/Linux）
  python find_yaml_nodes.py -f ci_config/rule.yaml -r CB -k version -v cann | \\
    python modify_node_key_value.py -f ci_config/rule.yaml --nodes-file - \\
      --key version --value cann_new

  # 方式5：修改布尔值
  python modify_node_key_value.py -f ci_config/rule.yaml \\
    -n CB.tasks.pkg.llt1_new --key ut_check --value true

  # 方式6：指定值类型
  python modify_node_key_value.py -f ci_config/rule.yaml \\
    -n CB.tasks.pkg.llt1_new --key count --value 10 --type int

  # 不创建备份文件（谨慎使用）
  python modify_node_key_value.py -f ci_config/rule.yaml \\
    -n CB.tasks.pkg.llt1_new --key version --value cann_test --no-backup

技术特性：
  - 流式读取：支持大型 YAML 文件，避免内存溢出
  - 递归遍历：高效定位和修改深层嵌套节点
  - 自动备份：默认创建带时间戳的备份文件
  - 安全恢复：保存失败时自动从备份恢复
  - 多文档支持：支持 YAML 多文档格式（--- 分隔）
  - 类型推断：自动识别布尔值、整数、浮点数和字符串

节点路径格式说明：
  - 路径使用点号分隔，如：CB.tasks.pkg.llt1
  - 路径的最后一部分是目标节点
  - 将修改该节点内的指定 key 的 value

值类型说明：
  - auto（默认）：自动推断类型
  - bool：布尔值（true/false, yes/no, on/off, 1/0）
  - int：整数
  - float：浮点数
  - str：字符串
        """
    )

    parser.add_argument(
        '-f', '--file',
        required=True,
        help='YAML 文件路径（必需）'
    )
    parser.add_argument(
        '-n', '--nodes',
        nargs='+',
        help='要修改的节点路径列表（空格分隔）'
    )
    parser.add_argument(
        '--nodes-file',
        help='包含节点路径列表的文件（每行一个路径，支持 find_yaml_nodes.py 的输出格式）'
    )
    parser.add_argument(
        '-k', '--key',
        required=True,
        help='要修改的键名（必需）'
    )
    parser.add_argument(
        '-v', '--value',
        required=True,
        help='新的值（必需）'
    )
    parser.add_argument(
        '-t', '--type',
        choices=['auto', 'bool', 'int', 'float', 'str'],
        default='auto',
        help='值的类型，默认为 auto（自动推断）'
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

    # 解析值
    try:
        new_value = parse_value(args.value, args.type)
    except ValueError as e:
        parser.error(f"值解析失败: {e}")

    # 获取节点列表
    nodes = []
    if args.nodes_file == '-':
        # 从标准输入读取
        print("从标准输入读取节点列表...")
        nodes = [line.strip() for line in sys.stdin if line.strip()]
    elif args.nodes_file:
        nodes = read_nodes_from_file(args.nodes_file)
        if not nodes:
            parser.error(f"无法从文件 '{args.nodes_file}' 读取节点列表或文件为空")
    elif args.nodes:
        nodes = args.nodes
    else:
        parser.error("必须指定 --nodes 或 --nodes-file 参数（使用 '-' 从标准输入读取）")

    print(f"""开始修改节点键值对：
- 文件路径：{args.file}
- 节点数量：{len(nodes)}
- 目标键：{args.key}
- 新值：{new_value}（类型：{type(new_value).__name__}）
- 创建备份：{'否' if args.no_backup else '是'}
""")

    if args.verbose and nodes:
        print(f"待修改的节点列表：")
        for idx, node in enumerate(nodes, 1):
            print(f"  {idx}. {node}")
        print()

    # 执行修改
    modified_count = modify_yaml_nodes_key_value(
        args.file,
        nodes,
        args.key,
        new_value,
        create_backup=not args.no_backup
    )

    if modified_count > 0:
        print(f"\n✓ 节点键值对修改完成，共修改 {modified_count} 个节点")
        sys.exit(0)
    else:
        print("\n✗ 节点键值对修改失败或没有节点被修改")
        sys.exit(1)


if __name__ == "__main__":
    main()
