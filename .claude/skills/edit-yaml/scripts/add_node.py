import yaml
import sys
import argparse
import shutil
import re
import os
from datetime import datetime
from pathlib import Path
from typing import List, Dict, Any, Set, Union, Tuple

# 兼容 Python 3.8 及以下版本
try:
    BooleanOptionalAction = argparse.BooleanOptionalAction
except AttributeError:
    # Python 3.8 及以下版本的兼容实现
    class BooleanOptionalAction(argparse.Action):
        def __init__(self, option_strings, dest, **kwargs):
            self._positive_strings = option_strings
            self._negative_strings = []
            for opt in option_strings:
                if opt.startswith('--'):
                    self._negative_strings.append('--no-' + opt[2:])
            all_option_strings = self._positive_strings + self._negative_strings
            super().__init__(all_option_strings, dest, nargs=0, **kwargs)

        def __call__(self, parser, namespace, values, option_string=None):
            if option_string in self._positive_strings:
                setattr(namespace, self.dest, True)
            else:
                setattr(namespace, self.dest, False)

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


def parse_node_spec(node_spec: str) -> Tuple[str, Dict]:
    """
    解析节点规格字符串，提取节点名称和初始内容

    支持的格式：
    - node_name              （空节点）
    - node_name: {}          （空节点，显式）
    - node_name: {key: value} （YAML格式）
    - node_name: key1:value1,key2:value2 （简单格式）

    Args:
        node_spec: 节点规格字符串

    Returns:
        (节点名称, 初始内容字典)
    """
    if not node_spec:
        return None, None

    # 检查是否包含冒号
    if ':' not in node_spec:
        # 没有冒号，表示只有节点名称，创建空节点
        return node_spec.strip(), None

    # 分割节点名称和内容
    # 找到第一个冒号的位置（但要注意可能是路径的一部分，如 parent.child:value）
    parts = node_spec.split(':', 1)
    if len(parts) != 2:
        return node_spec.strip(), None

    node_name = parts[0].strip()
    content_str = parts[1].strip()

    if not content_str or content_str == '{}':
        # 空内容
        return node_name, None

    # 尝试解析内容
    initial_content = None

    # 方式1：尝试YAML格式解析
    try:
        # 如果内容以 { 开头，尝试作为YAML解析
        if content_str.startswith('{'):
            parsed = yaml.safe_load(content_str)
            if isinstance(parsed, dict):
                initial_content = parsed
            else:
                print(f"⚠ 警告：内容不是字典格式，将作为单一值处理")
                initial_content = {"value": parsed}
        else:
            # 方式2：尝试简单 key:value,key2:value2 格式
            initial_content = {}
            items = content_str.split(',')
            for item in items:
                if ':' not in item:
                    continue
                key, value = item.split(':', 1)
                key = key.strip()
                value = value.strip()

                # 类型推断
                if value.lower() in ['true', 'yes', 'on']:
                    value = True
                elif value.lower() in ['false', 'no', 'off']:
                    value = False
                elif value.isdigit():
                    value = int(value)
                else:
                    # 尝试浮点数
                    try:
                        value = float(value)
                    except:
                        pass  # 保持为字符串

                initial_content[key] = value

            if not initial_content:
                initial_content = None
    except Exception as e:
        print(f"✗ 错误：无法解析内容 '{content_str}' - {e}")
        return node_name, None

    return node_name, initial_content


def add_node_in_dict(
    node: Dict,
    parent_path_parts: List[str],
    new_node_name: str,
    initial_content: Dict = None,
    skip: bool = False,
    current_depth: int = 0
) -> bool:
    """
    递归遍历字典，在指定父节点路径下添加新的子节点

    Args:
        node: 当前遍历的节点
        parent_path_parts: 父节点路径的各个部分（如 ['CB', 'tasks', 'pkg']）
        new_node_name: 要添加的新节点名称（支持嵌套，如 'new_node.sub1.sub2'）
        initial_content: 新节点的初始内容（字典）
        skip: 如果节点已存在，是否跳过（默认False，即覆盖已存在的节点）
        current_depth: 当前遍历深度

    Returns:
        是否成功添加
    """
    # 如果已经到达父节点路径的最后一层，在父节点下添加新子节点
    if current_depth == len(parent_path_parts) - 1:
        parent_node_name = parent_path_parts[-1]

        # 检查父节点是否存在
        if parent_node_name in node and isinstance(node[parent_node_name], dict):
            parent_node = node[parent_node_name]

            # 解析新节点名称（支持嵌套）
            new_node_parts = new_node_name.split('.')

            # 递归创建嵌套节点
            current_parent = parent_node
            created_path_parts = []

            for idx, part in enumerate(new_node_parts):
                created_path_parts.append(part)
                current_path = '.'.join(created_path_parts)

                # 检查该层级节点是否已存在
                if part in current_parent:
                    if isinstance(current_parent[part], dict):
                        # 如果是最后一层且需要覆盖
                        if idx == len(new_node_parts) - 1:
                            if skip:
                                print(f"  ⊘ 跳过：父节点 '{'.'.join(parent_path_parts)}' 中已存在子节点 '{current_path}'")
                                return False
                            else:
                                old_value = current_parent[part]
                                if initial_content and old_value == initial_content:
                                    print(f"  ⊘ 跳过：父节点 '{'.'.join(parent_path_parts)}' 中的子节点 '{current_path}' 内容已相同")
                                    return False
                                current_parent[part] = initial_content if initial_content else {}
                                print(f"  ✓ 覆盖：父节点 '{'.'.join(parent_path_parts)}' 中的子节点 '{current_path}'")
                                return True
                        else:
                            # 继续深入嵌套
                            current_parent = current_parent[part]
                    else:
                        # 已存在但不是字典类型，无法继续嵌套
                        print(f"  ✗ 警告：父节点 '{'.'.join(parent_path_parts)}' 中的 '{current_path}' 不是字典类型，无法添加嵌套节点")
                        return False
                else:
                    # 创建新节点
                    if idx == len(new_node_parts) - 1:
                        # 最后一层，使用提供的初始内容
                        current_parent[part] = initial_content if initial_content else {}
                        content_info = f"，内容：{initial_content}" if initial_content else ""
                        print(f"  ✓ 添加：父节点 '{'.'.join(parent_path_parts)}' 中添加子节点 '{current_path}'{content_info}")
                        return True
                    else:
                        # 中间层，创建空字典以便继续嵌套
                        current_parent[part] = {}
                        current_parent = current_parent[part]
            return False
        else:
            print(f"  ✗ 警告：未找到父节点 '{'.'.join(parent_path_parts)}'")
            return False

    # 继续递归遍历到父节点
    if current_depth < len(parent_path_parts) - 1:
        current_key = parent_path_parts[current_depth]
        if current_key in node and isinstance(node[current_key], dict):
            return add_node_in_dict(
                node[current_key],
                parent_path_parts,
                new_node_name,
                initial_content,
                skip,
                current_depth + 1
            )
        else:
            print(f"  ✗ 警告：路径 '{'.'.join(parent_path_parts[:current_depth+1])}' 不存在")
            return False

    return False


def add_yaml_nodes(
    file_path: str,
    parent_paths: List[str],
    new_node_name: str,
    initial_content: Dict = None,
    skip: bool = False,
    create_backup: bool = True
) -> int:
    """
    流式添加 YAML 文件中指定父节点路径下的子节点

    Args:
        file_path: YAML 文件路径
        parent_paths: 父节点路径列表（如 CB.tasks.pkg）
        new_node_name: 要添加的新节点名称（支持嵌套）
        initial_content: 新节点的初始内容
        skip: 如果节点已存在，是否跳过（默认False，即覆盖已存在的节点）
        create_backup: 是否创建备份文件

    Returns:
        成功添加的节点数量
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

    added_count = 0
    modified_paths: Set[str] = set()

    # 处理每个父节点路径
    for parent_path in parent_paths:
        # 移除路径中的方括号（如果有）
        parent_path = re.sub(r'\[\d+\]', '', parent_path)

        # 解析路径：CB.tasks.pkg -> ['CB', 'tasks', 'pkg']
        path_parts = parent_path.split('.')

        if len(path_parts) < 1:
            print(f"✗ 警告：跳过无效路径 '{parent_path}'（路径深度不足）")
            continue

        # 在每个文档中查找并添加
        for doc_idx, doc in enumerate(documents):
            if not isinstance(doc, dict):
                continue

            # 递归添加子节点
            if add_node_in_dict(doc, path_parts, new_node_name, initial_content, skip):
                added_count += 1
                modified_paths.add(parent_path)
                break  # 找到并添加后，跳出文档循环

    if added_count == 0:
        print("\n⊘ 没有节点被添加")
        return 0

    # 保存修改后的文件
    try:
        with open(file_path, 'w', encoding='utf-8') as f:
            # 流式写入所有文档
            for idx, doc in enumerate(documents):
                if idx > 0:
                    f.write('---\n')
                # 使用展开形式保存YAML，确保嵌套结构始终展开显示
                # default_flow_style=False: 禁用flow样式（{a: b}），使用块样式
                # width=None: 禁用自动折行
                # indent=2: 使用2空格缩进
                yaml.dump(doc, f, allow_unicode=True, default_flow_style=False, sort_keys=False, width=None, indent=2)
        print(f"\n✓ 成功添加 {added_count} 个节点")
        return added_count
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
            # [1] CB.tasks.pkg
            # CB.tasks.pkg
            # 去除方括号和序号
            line = re.sub(r'^\[\d+\]\s*', '', line)
            nodes.append(line)

        return nodes
    except Exception as e:
        print(f"✗ 错误：无法读取节点文件 - {e}")
        return []


def main():
    parser = argparse.ArgumentParser(
        description='批量在 YAML 文件中指定父节点路径下添加新的子节点（支持嵌套节点和初始内容）',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
使用示例：

  # 方式1：直接在命令行指定父节点路径和空节点
  python add_node.py --file ci_config/rule.yaml \\
    --parents CB.tasks.pkg CB.tasks.compile \\
    --node new_task

  # 方式2：添加带初始内容的节点（简单格式）
  python add_node.py -f ci_config/rule.yaml \\
    -p CB.tasks.pkg -n "new_task: version:cann,enabled:true"

  # 方式3：添加带初始内容的节点（YAML格式）
  python add_node.py -f ci_config/rule.yaml \\
    -p CB.tasks.pkg -n "new_task: {version: cann, enabled: true, count: 5}"

  # 方式4：从文件读取父节点路径列表
  python add_node.py -f ci_config/rule.yaml \\
    --parents-file parents.txt -n "new_task: {version: cann}"

  # 方式5：配合 find_yaml_node.py 使用（推荐工作流）
  # 步骤1：查找包含特定条件的父节点
  python find_yaml_node.py -f ci_config/rule.yaml -r CB.tasks -k type -v pkg > parents.txt
  # 步骤2：批量在这些父节点下添加新的子节点
  python add_node.py -f ci_config/rule.yaml --parents-file parents.txt \\
    -n "new_task: {version: cann, enabled: true}"

  # 方式6：使用管道直接传递（Unix/Linux）
  python find_yaml_node.py -f ci_config/rule.yaml -r CB.tasks -k type -v pkg | \\
    python add_node.py -f ci_config/rule.yaml --parents-file - \\
      -n "new_task: {version: cann}"

  # 方式7：添加嵌套节点
  python add_node.py -f ci_config/rule.yaml \\
    -p CB.tasks.pkg -n "new_sub_node.level1.level2: {key: value}"

  # 方式8：跳过已存在的节点（使用 --skip）
  python add_node.py -f ci_config/rule.yaml \\
    -p CB.tasks.pkg -n "existing_task: {version: new}" --skip

  # 不创建备份文件（谨慎使用）
  python add_node.py -f ci_config/rule.yaml \\
    -p CB.tasks.pkg -n new_task --no-backup

技术特性：
  - 流式读取：支持大型 YAML 文件，避免内存溢出
  - 递归遍历：高效定位和修改深层嵌套节点
  - 自动备份：默认创建带时间戳的备份文件
  - 安全恢复：保存失败时自动从备份恢复
  - 多文档支持：支持 YAML 多文档格式（--- 分隔）
  - 嵌套节点：支持一次性创建多级嵌套节点
  - 类型推断：自动识别布尔值、整数、浮点数和字符串
  - 智能处理：默认覆盖已存在的节点，使用 --skip 跳过

节点规格格式说明：
  --node 参数格式：node_name 或 node_name:content

  1. 空节点：
     --node new_task
     --node new_task: {}

  2. 简单格式内容：
     --node "new_task: version:cann,enabled:true,count:5"

  3. YAML格式内容：
     --node "new_task: {version: cann, enabled: true, count: 5}"
     --node "new_task: {version: cann, options: {opt1: val1}}"

  4. 嵌套节点：
     --node "parent.child: {subkey: value}"
     --node "new_sub_node.level1.level2: {key: value}"

父节点路径格式：
  - 路径使用点号分隔，如：CB.tasks.pkg
  - 新节点将添加到这些父节点下

行为说明：
  - 默认情况下，如果节点已存在则覆盖
  - 使用 --skip 参数可跳过已存在的节点
        """
    )

    parser.add_argument(
        '-f', '--file',
        required=True,
        help='YAML 文件路径（必需）'
    )
    parser.add_argument(
        '-p', '--parents',
        nargs='+',
        help='父节点路径列表（空格分隔）'
    )
    parser.add_argument(
        '--parents-file',
        help='包含父节点路径列表的文件（每行一个路径，支持 find_yaml_node.py 的输出格式）'
    )
    parser.add_argument(
        '-n', '--node',
        required=True,
        help='要添加的新节点规格（格式：node_name 或 node_name:content，支持嵌套节点）'
    )
    parser.add_argument(
        '--skip',
        action='store_true',
        default=False,
        help='跳过已存在的节点（默认为覆盖模式，使用此参数跳过已存在的节点）'
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

    # 解析节点规格
    new_node_name, initial_content = parse_node_spec(args.node)
    if not new_node_name:
        parser.error("无法解析节点规格，请检查格式")

    # 获取父节点列表
    parents = []
    if args.parents_file == '-':
        # 从标准输入读取
        print("从标准输入读取父节点路径列表...")
        parents = [line.strip() for line in sys.stdin if line.strip()]
    elif args.parents_file:
        parents = read_nodes_from_file(args.parents_file)
        if not parents:
            parser.error(f"无法从文件 '{args.parents_file}' 读取父节点路径列表或文件为空")
    elif args.parents:
        parents = args.parents
    else:
        parser.error("必须指定 --parents 或 --parents-file 参数（使用 '-' 从标准输入读取）")

    content_info = f"，初始内容：{initial_content}" if initial_content else "（空节点）"
    print(f"""开始添加节点：
- 文件路径：{args.file}
- 父节点数量：{len(parents)}
- 新节点名称：{new_node_name}
- 节点内容：{'有内容' if initial_content else '空节点'}{content_info}
- 跳过已存在：{'是' if args.skip else '否（默认覆盖）'}
- 创建备份：{'否' if args.no_backup else '是'}
""")

    if args.verbose and parents:
        print(f"待添加的父节点列表：")
        for idx, parent in enumerate(parents, 1):
            print(f"  {idx}. {parent}")
        print()

    # 执行添加
    added_count = add_yaml_nodes(
        args.file,
        parents,
        new_node_name,
        initial_content,
        skip=args.skip,
        create_backup=not args.no_backup
    )

    if added_count > 0:
        print(f"\n✓ 节点添加完成，共添加 {added_count} 个节点")
        sys.exit(0)
    else:
        print("\n✗ 节点添加失败或没有节点被添加")
        sys.exit(1)


if __name__ == "__main__":
    main()
