#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
脚本用于给指定路径下的头文件添加宏定义保护
支持多个排除路径和正则表达式
支持删除experimental路径和宏定义
支持检查impl目录是否包含了include目录下的文件
支持修复文件注释中的\file标识符
"""

import os
import re
import argparse
from pathlib import Path
from typing import List, Set, Tuple, Pattern, Optional


def parse_arguments():
    """解析命令行参数"""
    parser = argparse.ArgumentParser(
        description='给头文件添加宏定义保护',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
使用示例:
  # 添加宏定义到默认目录
  python process.py -add
  
  # 删除experimental
  python process.py -delete
  
  # 检查impl是否包含include文件
  python process.py -check
  
  # 修复文件注释
  python process.py -fix
  
  # 组合功能
  python process.py -add -delete -check -fix
  
  # 指定路径
  python process.py -impl impl/experimental/ -include include/experimental/ --add-macro
  
  # 排除路径
  python process.py -add -exclude "include/experimental/tensor_api/utils,impl/test" --exclude-regex ".*test.*,.*temp.*"
        """
    )
    parser.add_argument('-impl', '--impl-dir', default='',
                       help='impl目录路径（可选）')
    parser.add_argument('-include', '--include-dir', default='',
                       help='include目录路径（可选）')
    parser.add_argument('-exclude', '--exclude-dirs', default='',
                       help='排除的目录，多个用逗号分隔')
    parser.add_argument('--exclude-regex', default='',
                       help='排除的正则表达式模式，多个用逗号分隔')
    parser.add_argument('-delete', action='store_true',
                       help='删除experimental路径和宏定义')
    parser.add_argument('-check', action='store_true',
                       help='检查impl是否包含include文件')
    parser.add_argument('-add', '--add-macro', action='store_true',
                       help='添加宏定义保护')
    parser.add_argument('-fix', '--fix-file-comment', action='store_true',
                       help='修复文件注释中的\\file标识符')
    
    return parser.parse_args()


def compile_regex_patterns(pattern_strings: str) -> List[Pattern]:
    """编译正则表达式模式"""
    patterns = []
    if not pattern_strings:
        return patterns
    
    for pattern_str in pattern_strings.split(','):
        pattern_str = pattern_str.strip()
        if pattern_str:
            try:
                # 编译正则表达式，忽略大小写
                pattern = re.compile(pattern_str, re.IGNORECASE)
                patterns.append(pattern)
            except re.error as e:
                print(f"警告: 正则表达式 '{pattern_str}' 编译失败: {e}")
    
    return patterns


def should_exclude(filepath: str, exclude_paths: Set[str], 
                  exclude_patterns: List[Pattern]) -> bool:
    """判断文件是否应该被排除"""
    # 1. 检查是否在排除路径中
    for exclude_path in exclude_paths:
        exclude_abs = os.path.abspath(exclude_path)
        file_abs = os.path.abspath(filepath)
        
        # 检查文件是否在排除目录或其子目录中
        if file_abs.startswith(exclude_abs + os.sep) or file_abs == exclude_abs:
            return True
    
    # 2. 检查是否匹配排除的正则表达式
    for pattern in exclude_patterns:
        if pattern.search(filepath):
            return True
    
    return False


def get_all_header_files(directory: str, exclude_paths: Set[str], 
                        exclude_patterns: List[Pattern]) -> List[str]:
    """获取指定目录下的所有头文件"""
    header_files = []
    dir_path = Path(directory).resolve()
    
    if not dir_path.exists():
        return []
    
    # 只处理常见的头文件扩展名
    header_extensions = {'.h', '.hpp', '.hxx', '.hh', '.hpp.inc'}
    
    for root, dirs, files in os.walk(dir_path):
        for file in files:
            file_path = os.path.join(root, file)
            
            # 检查扩展名
            _, ext = os.path.splitext(file)
            if ext.lower() not in header_extensions:
                continue
            
            # 检查是否应该排除
            if should_exclude(file_path, exclude_paths, exclude_patterns):
                continue
            
            header_files.append(file_path)
    
    return header_files


def get_default_dirs() -> Tuple[List[str], List[str]]:
    """获取默认目录列表"""
    # 默认的目录
    default_impl_dirs = []
    default_include_dirs = []
    
    # 检查这些目录是否存在
    potential_dirs = [
        'include/experimental',
        'impl/experimental', 
        'include/tensor_api',
        'impl/tensor_api'
    ]
    
    for dir_path in potential_dirs:
        if os.path.exists(dir_path):
            if 'include' in dir_path:
                default_include_dirs.append(dir_path)
            elif 'impl' in dir_path:
                default_impl_dirs.append(dir_path)
    
    return default_impl_dirs, default_include_dirs


def get_all_header_files_from_dirs(directories: List[str], exclude_paths: Set[str], 
                                  exclude_patterns: List[Pattern]) -> List[str]:
    """从多个目录获取所有头文件"""
    all_files = []
    for directory in directories:
        files = get_all_header_files(directory, exclude_paths, exclude_patterns)
        all_files.extend(files)
    
    return all_files


def read_file_content(filepath: str) -> str:
    """读取文件内容"""
    with open(filepath, 'r', encoding='utf-8') as f:
        return f.read()


def write_file_content(filepath: str, content: str):
    """写入文件内容"""
    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)


def find_copyright_end(content: str) -> int:
    """查找版权注释的结束位置"""
    # 匹配常见的版权注释格式
    patterns = [
        r'/\*[\s\S]*?\*/\s*\n',  # C风格注释 /* ... */
        r'//[\s\S]*?Copyright[\s\S]*?\n\s*\n',  # C++风格注释
        r'/\*\*[\s\S]*?\*/\s*\n',  # 文档注释 /** ... */
    ]
    
    for pattern in patterns:
        match = re.match(pattern, content)
        if match:
            return match.end()
    
    # 如果没有找到版权注释，尝试找文件开头的多行注释
    lines = content.split('\n')
    in_comment = False
    for i, line in enumerate(lines):
        if line.strip().startswith('/*'):
            in_comment = True
        if in_comment and '*/' in line:
            # 返回注释结束后的位置
            return sum(len(l) + 1 for l in lines[:i+1])  # +1 for newline
        if not in_comment and not line.strip().startswith('//') and line.strip():
            # 如果没有注释，找到第一个非空行
            break
    
    return 0


def generate_impl_macros(filepath: str) -> Tuple[str, str]:
    """生成impl文件的宏定义"""
    # 获取相对于当前目录的文件路径
    rel_path = os.path.relpath(filepath)
    
    header_macro = f'''#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \\
    "{rel_path} is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
'''
    
    footer_macro = '''#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
'''
    
    return header_macro, footer_macro


def generate_include_macros() -> Tuple[str, str]:
    """生成include文件的宏定义"""
    header_macro = '''#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
'''
    
    footer_macro = '''#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
'''
    
    return header_macro, footer_macro


def already_has_macros(content: str, file_type: str) -> bool:
    """检查文件是否已经包含宏定义"""
    if file_type == 'impl':
        check_macros = [
            'ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS',
            'UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC'
        ]
    else:  # include
        check_macros = [
            'ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS',
            'UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H'
        ]
    
    for macro in check_macros:
        if macro in content:
            return True
    
    return False


def add_macros_to_file(filepath: str, file_type: str) -> bool:
    """给文件添加宏定义"""
    try:
        content = read_file_content(filepath)
        
        # 检查是否已经有宏定义
        if already_has_macros(content, file_type):
            return False
        
        # 生成宏定义
        if file_type == 'impl':
            header_macro, footer_macro = generate_impl_macros(filepath)
        else:  # include
            header_macro, footer_macro = generate_include_macros()
        
        # 查找版权注释结束位置
        insert_pos = find_copyright_end(content)
        
        if insert_pos == 0:
            # 没有找到版权注释，在文件开头插入
            new_content = header_macro + '\n' + content
        else:
            # 在版权注释后插入
            new_content = content[:insert_pos] + '\n' + header_macro + '\n' + content[insert_pos:]
        
        # 在文件末尾添加结束宏定义
        # 确保文件以换行结束
        if not new_content.endswith('\n'):
            new_content += '\n'
        new_content += '\n' + footer_macro
        
        write_file_content(filepath, new_content)
        return True
        
    except Exception as e:
        return False


def process_files_for_macros(impl_files: List[str], include_files: List[str]) -> Tuple[int, int, int, int]:
    """处理文件列表添加宏定义"""
    impl_success, impl_fail = 0, 0
    include_success, include_fail = 0, 0
    
    for filepath in impl_files:
        if add_macros_to_file(filepath, 'impl'):
            impl_success += 1
        else:
            impl_fail += 1
    
    for filepath in include_files:
        if add_macros_to_file(filepath, 'include'):
            include_success += 1
        else:
            include_fail += 1
    
    return impl_success, impl_fail, include_success, include_fail


def delete_experimental(filepath: str) -> bool:
    """删除文件中的experimental相关路径和宏定义"""
    try:
        content = read_file_content(filepath)
        original_content = content
        
        # 1. 删除路径中的experimental
        # 替换impl/experimental/tensor_api 为 impl/tensor_api
        content = re.sub(r'impl/experimental/tensor_api', 'impl/tensor_api', content)
        # 替换include/experimental/tensor_api 为 include/tensor_api
        content = re.sub(r'include/experimental/tensor_api', 'include/tensor_api', content)
        
        # 2. 删除宏定义中的EXPERIMENTAL
        content = re.sub(r'_EXPERIMENTAL', '', content)
        
        # 如果内容有变化，保存文件
        if content != original_content:
            write_file_content(filepath, content)
            return True
        
        return False
        
    except Exception as e:
        return False


def process_files_for_delete(files: List[str]) -> int:
    """处理文件列表删除experimental"""
    success_count = 0
    
    for filepath in files:
        if delete_experimental(filepath):
            success_count += 1
    
    return success_count


def check_impl_include_issues(impl_files: List[str], include_dirs: List[str]) -> Tuple[int, List[str]]:
    """检查impl文件是否包含了include目录下的文件"""
    if not impl_files or not include_dirs:
        return 0, []
    
    # 获取所有include目录下的文件
    all_include_files = []
    for include_dir in include_dirs:
        for root, dirs, files in os.walk(include_dir):
            for file in files:
                if file.endswith(('.h', '.hpp', '.hxx', '.hh', '.hpp.inc')):
                    all_include_files.append(os.path.join(root, file))
    
    # 提取include文件名（不包含路径）
    include_filenames = {os.path.basename(f) for f in all_include_files}
    
    issue_count = 0
    issues = []
    
    for impl_file in impl_files:
        try:
            content = read_file_content(impl_file)
            
            # 查找所有的#include指令
            includes = re.findall(r'#include\s+["<]([^">]+)[">]', content)
            
            for inc in includes:
                # 检查是否包含include目录下的文件
                inc_basename = os.path.basename(inc)
                
                # 情况1: 直接包含include目录下的文件名
                if inc_basename in include_filenames:
                    issue_count += 1
                    issues.append(f"{impl_file}: 包含了include文件 {inc_basename}")
                
                # 情况2: 包含路径以include开头
                if inc.startswith('include/'):
                    issue_count += 1
                    issues.append(f"{impl_file}: 包含了include路径 {inc}")
                    
        except Exception as e:
            pass
    
    return issue_count, issues


def fix_file_comment(filepath: str) -> bool:
    """修复文件注释中的\file标识符，确保与当前文件名一致"""
    try:
        content = read_file_content(filepath)
        original_content = content
        
        # 获取当前文件名（不包含路径）
        current_filename = os.path.basename(filepath)
        
        # 查找并修复 * \file 注释
        # 匹配模式：* \file 后面跟着文件名
        # 支持多种格式：* \file, * \file, /* \file 等
        # 1. 匹配 /*! 开头的 Doxygen 注释
        pattern1 = r'(/\*![^*]*\*[ \t]*\\file[ \t]+)([^\s\n]+)'
        # 示例: /*! \file filename.h */
        
        # 2. 匹配 /** 开头的 Doxygen 注释
        pattern2 = r'(/\*\*[^*]*\*[ \t]*\\file[ \t]+)([^\s\n]+)'
        # 示例: /** \file filename.h */
        
        # 3. 匹配 /* 开头的普通多行注释
        pattern3 = r'(/\*[^*]*\\file[ \t]+)([^\s\n]+)'
        # 示例: /* \file filename.h */
        
        # 4. 匹配 //! 开头的 Doxygen 单行注释
        pattern4 = r'(//![ \t]*\\file[ \t]+)([^\s\n]+)'
        # 示例: //! \file filename.h
        
        # 5. 匹配 /// 开头的 Doxygen 单行注释
        pattern5 = r'(///[ \t]*\\file[ \t]+)([^\s\n]+)'
        # 示例: /// \file filename.h
        
        # 6. 匹配 // 开头的普通单行注释
        pattern6 = r'(//[ \t]*\\file[ \t]+)([^\s\n]+)'
        # 示例: // \file filename.h
        
        patterns = [pattern1, pattern2, pattern3, pattern4, pattern5, pattern6]
        
        file_fixed = False
        
        print(current_filename)
        for pattern in patterns:
            match = re.search(pattern, content)
            if match:
                # 提取当前注释中的文件名
                comment_filename = match.group(2) if len(match.groups()) >= 2 else match.group(1)
                print(comment_filename)
                # 如果注释中的文件名与当前文件名不一致，则修复
                if comment_filename != current_filename:
                    # 替换文件名
                    start = match.start(2) if len(match.groups()) >= 2 else match.start(1)
                    end = match.end(2) if len(match.groups()) >= 2 else match.end(1)
                    
                    content = content[:start] + current_filename + content[end:]
                    file_fixed = True
                    break
        
        # 如果内容有变化，保存文件
        if file_fixed and content != original_content:
            write_file_content(filepath, content)
            return True
        
        return False
        
    except Exception as e:
        return False


def process_files_for_fix_file_comment(files: List[str]) -> int:
    """处理文件列表修复文件注释"""
    success_count = 0
    
    for filepath in files:
        if fix_file_comment(filepath):
            success_count += 1
    
    return success_count


def main():
    args = parse_arguments()
    
    # 如果没有指定任何功能，提示并退出
    if not args.add_macro and not args.delete and not args.check and not args.fix_file_comment:
        print("错误: 请至少指定一个功能:")
        print("  -add: 添加宏定义")
        print("  -delete: 删除experimental")
        print("  -check: 检查impl是否包含include文件")
        print("  -fix: 修复文件注释中的\\file标识符")
        return
    
    # 解析排除路径
    exclude_paths = {d.strip() for d in args.exclude_dirs.split(',') if d.strip()}
    
    # 编译正则表达式模式
    exclude_patterns = compile_regex_patterns(args.exclude_regex)
    
    # 确定要处理的目录
    impl_dirs = []
    include_dirs = []
    
    # 处理impl目录
    if args.impl_dir:
        impl_dirs = [args.impl_dir]
    else:
        # 获取默认目录
        default_impl_dirs, default_include_dirs = get_default_dirs()
        impl_dirs = default_impl_dirs
    
    # 处理include目录
    if args.include_dir:
        include_dirs = [args.include_dir]
    else:
        # 获取默认目录
        if not impl_dirs:  # 如果impl_dirs还没获取，先获取
            default_impl_dirs, default_include_dirs = get_default_dirs()
        else:
            # 只获取include目录
            _, default_include_dirs = get_default_dirs()
        include_dirs = default_include_dirs
    
    # 检查是否有目录
    if not impl_dirs and not include_dirs:
        print("警告: 没有找到默认目录，且未指定-impl和-include")
        return
    
    # 获取文件列表
    impl_files = get_all_header_files_from_dirs(impl_dirs, exclude_paths, exclude_patterns)
    include_files = get_all_header_files_from_dirs(include_dirs, exclude_paths, exclude_patterns)
    
    # 汇总信息
    summary = []
    summary.append(f"找到 {len(impl_files)} 个 impl 文件")
    summary.append(f"找到 {len(include_files)} 个 include 文件")
    
    # 执行删除experimental功能
    if args.delete:
        all_files = impl_files + include_files
        if all_files:
            delete_success = process_files_for_delete(all_files)
            summary.append(f"删除experimental: 成功修改 {delete_success} 个文件")
        else:
            summary.append("删除experimental: 没有找到文件")
    
    # 执行添加宏定义功能
    if args.add_macro:
        if impl_files or include_files:
            impl_success, impl_fail, include_success, include_fail = process_files_for_macros(impl_files, include_files)
            summary.append("添加宏定义结果:")
            summary.append(f"  Impl 文件: 成功 {impl_success}, 失败 {impl_fail}")
            summary.append(f"  Include 文件: 成功 {include_success}, 失败 {include_fail}")
        else:
            summary.append("添加宏定义: 没有找到文件")
    
    # 执行检查功能
    if args.check:
        if impl_files and include_dirs:
            issue_count, issues = check_impl_include_issues(impl_files, include_dirs)
            
            if issue_count == 0:
                summary.append("检查结果: ✓ 通过 (impl文件中没有发现包含include目录下的文件)")
            else:
                summary.append(f"检查结果: ✗ 失败 (发现 {issue_count} 个问题)")
                for issue in issues:
                    summary.append(f"  - {issue}")
        else:
            summary.append("检查: 缺少impl文件或include目录")
    
    # 执行修复文件注释功能
    if args.fix_file_comment:
        all_files = impl_files + include_files
        if all_files:
            fix_comment_success = process_files_for_fix_file_comment(all_files)
            summary.append(f"修复文件注释: 成功修改 {fix_comment_success} 个文件")
        else:
            summary.append("修复文件注释: 没有找到文件")
    
    # 输出汇总信息
    print("=" * 60)
    print("处理汇总:")
    print("=" * 60)
    for line in summary:
        print(line)
    print("=" * 60)


if __name__ == '__main__':
    main()