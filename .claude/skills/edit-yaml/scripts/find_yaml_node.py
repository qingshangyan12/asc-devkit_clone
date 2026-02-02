import yaml
import sys
import argparse
from typing import Dict, Any, List

def find_objects_with_target_kv_in_root(
    file_path: str,
    target_root: str = "CB",          # 支持多级路径，如CB.pkg.compile
    target_key: str = "version",      # 要匹配的键名，默认version
    target_value: str = "cann_eco_debug"  # 要匹配的值，默认cann_eco_debug
) -> List[str]:
    """
    流式查找大型YAML文件中指定多级根节点下包含指定键值对的所有对象的路径

    Args:
        file_path: YAML文件路径
        target_root: 要查找的根节点路径，支持多级（如CB.pkg.compile），默认是CB
        target_key: 要匹配的键名，默认version
        target_value: 要匹配的值，默认cann_eco_debug

    Returns:
        匹配到的所有对象路径列表
    """
    matched_paths = []  # 仅存储路径，不再存储完整对象
    # 解析多级根路径为列表，按.分割
    root_path_parts = target_root.split('.')

    def traverse_node(node: Any, path: str, parent: str = "") -> None:
        """递归遍历节点，查找包含指定键值对的对象并记录路径"""
        # 如果是字典类型，检查是否包含目标键值对
        if isinstance(node, dict):
            # 检查当前节点是否包含目标键且值匹配
            if node.get(target_key) == target_value:
                matched_paths.append(path)  # 只添加路径

            # 递归遍历子节点
            for key, value in node.items():
                new_path = f"{path}.{key}" if parent else key
                traverse_node(value, new_path, key)

        # 如果是列表类型，遍历列表中的每个元素
        elif isinstance(node, list):
            for idx, item in enumerate(node):
                new_path = f"{path}[{idx}]"
                traverse_node(item, new_path, str(idx))

    # 流式读取YAML文件（避免一次性加载大文件）
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            # safe_load_all会按---分隔符流式读取文档
            for doc in yaml.safe_load_all(f):
                if not isinstance(doc, dict):
                    continue  # 非字典类型文档直接跳过

                # 逐级定位多级根节点
                current_node = doc
                current_path = ""
                for idx, part in enumerate(root_path_parts):
                    if part not in current_node:
                        break  # 某一级节点不存在，终止查找
                    # 更新当前节点和路径
                    current_node = current_node[part]
                    current_path = part if idx == 0 else f"{current_path}.{part}"
                else:
                    # 所有层级都匹配成功，开始遍历目标根节点
                    traverse_node(current_node, current_path, current_path)

    except FileNotFoundError:
        print(f"错误：文件 {file_path} 不存在")
        sys.exit(1)
    except yaml.YAMLError as e:
        print(f"错误：YAML解析失败 - {e}")
        sys.exit(1)
    except Exception as e:
        print(f"未知错误：{e}")
        sys.exit(1)

    return matched_paths

def main():
    # 创建参数解析器
    parser = argparse.ArgumentParser(
        description='在 YAML 文件中查找包含特定键值对的对象路径',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
使用示例：
  # 使用默认配置（CB + version: cann_eco_debug）
  python find_yaml_object.py large_file.yaml

  # 使用参数分隔符形式（推荐）
  python find_yaml_object.py --file large_file.yaml
  python find_yaml_object.py -f large_file.yaml --root CB.pkg --key version --value cann
  python find_yaml_object.py -f large_file.yaml -r CB.pkg.compile -k status -v active

  # 使用位置参数（向后兼容）
  python find_yaml_object.py large_file.yaml CB.pkg
  python find_yaml_object.py large_file.yaml CB.pkg.compile status active

  # 混合使用（位置参数和分隔符参数可混用）
  python find_yaml_object.py large_file.yaml CB.pkg --key version --value cann
        """
    )

    # 添加参数定义
    parser.add_argument(
        'file_path',
        nargs='?',  # 可选的位置参数
        help='YAML 文件路径'
    )
    parser.add_argument(
        'target_root',
        nargs='?',  # 可选的位置参数
        help='根节点路径（支持多级，如 CB.pkg.compile）'
    )
    parser.add_argument(
        '-f', '--file',
        dest='file_path_arg',
        help='YAML 文件路径（使用 --file 指定）'
    )
    parser.add_argument(
        '-r', '--root',
        dest='target_root_arg',
        default='CB',
        help='根节点路径，默认为 CB'
    )
    parser.add_argument(
        '-k', '--key',
        dest='target_key_arg',
        default='version',
        help='目标键名，默认为 version'
    )
    parser.add_argument(
        '-v', '--value',
        dest='target_value_arg',
        default='cann_eco_debug',
        help='目标值，默认为 cann_eco_debug'
    )

    # 解析参数
    args = parser.parse_args()

    # 确定最终的参数值（优先使用分隔符参数，其次使用位置参数）
    file_path = args.file_path_arg or args.file_path
    target_root = args.target_root_arg or args.target_root or 'CB'
    target_key = args.target_key_arg
    target_value = args.target_value_arg

    # 校验必需参数
    if not file_path:
        parser.error("必须指定 YAML 文件路径（使用位置参数或 --file/-f 参数）")

    # 执行查找
    print(f"""开始查找：
- 文件路径：{file_path}
- 根节点路径：[{target_root}]
- 目标键值对：{target_key}: {target_value}
""")
    results = find_objects_with_target_kv_in_root(file_path, target_root, target_key, target_value)

    # 输出结果（仅显示对象路径）
    if results:
        print(f"\n找到 {len(results)} 个包含 {target_key}: {target_value} 的对象（根节点路径：{target_root}）：")
        print("-" * 80)
        for idx, path in enumerate(results, 1):
            print(f"[{idx}] {path}")  # 仅输出路径
    else:
        print(f"\n未在根节点路径 [{target_root}] 下找到包含 {target_key}: {target_value} 的对象")

if __name__ == "__main__":
    main()