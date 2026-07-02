#!/usr/bin/env python3
"""
批量为 C-API 目录下的 Markdown 文档功能说明添加头文件路径。
根据文档中函数原型，在 include/c_api 目录下查找对应的头文件。
"""
import os
import re
import glob

# 路径配置
DOCS_ROOT = r"C:\JC\asc-devkit\docs\api\SIMD-API\C-API"
INCLUDE_ROOT = r"C:\JC\asc-devkit\include\c_api"

def build_function_header_map():
    """扫描 include/c_api 下所有 .h 文件，建立 函数名 → 头文件相对路径 的映射"""
    func_map = {}
    
    for root, dirs, files in os.walk(INCLUDE_ROOT):
        for f in files:
            if f.endswith('.h'):
                full_path = os.path.join(root, f)
                rel_path = os.path.relpath(full_path, INCLUDE_ROOT).replace('\\', '/')
                header_include = f"c_api/{rel_path}"
                
                try:
                    with open(full_path, 'r', encoding='utf-8', errors='ignore') as hf:
                        content = hf.read()
                except:
                    continue
                
                # 匹配函数声明：__aicore__ 或 __simd_callee__ 开头的函数
                # 匹配模式: __aicore__ inline void func_name( 或 __simd_callee__ inline void func_name(
                patterns = [
                    r'__aicore__\s+inline\s+\w+\s+(asc_\w+)\s*\(',
                    r'__simd_callee__\s+inline\s+\w+\s+(asc_\w+)\s*\(',
                    r'__simd_callee__\s+inline\s+\w+\s+(asc_\w+)\s*\(',
                ]
                
                for pattern in patterns:
                    matches = re.findall(pattern, content)
                    for func_name in matches:
                        if func_name not in func_map:
                            func_map[func_name] = header_include
                        # If already mapped, keep the first one (or we could prefer specific headers)
    
    return func_map

def extract_function_names_from_doc(doc_path):
    """从文档中提取函数名"""
    try:
        with open(doc_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except:
        return []
    
    func_names = set()
    
    # 从标题提取函数名（如 # asc_abs）
    title_match = re.match(r'^#\s+(asc_\w+)', content, re.MULTILINE)
    if title_match:
        func_names.add(title_match.group(1))
    
    # 从函数原型代码块中提取函数名
    # 匹配 ```cpp ... ``` 块中的函数声明
    code_blocks = re.findall(r'```cpp\s*\n(.*?)```', content, re.DOTALL)
    for block in code_blocks:
        # 匹配函数声明
        found = re.findall(r'(?:__aicore__|__simd_callee__)\s+inline\s+\w+\s+(asc_\w+)\s*\(', block)
        func_names.update(found)
        # 也匹配不带 __aicore__/__simd_callee__ 的声明（如纯原型）
        found2 = re.findall(r'\b(asc_\w+)\s*\(', block)
        func_names.update(found2)
    
    return list(func_names)

def get_header_for_doc(doc_path, func_map):
    """根据文档中的函数名查找对应的头文件"""
    func_names = extract_function_names_from_doc(doc_path)
    
    headers = set()
    for name in func_names:
        if name in func_map:
            headers.add(func_map[name])
    
    # 过滤掉一些特殊的函数名
    valid_headers = [h for h in headers if h != 'c_api/asc_simd.h']
    
    if valid_headers:
        # 如果有多个，选最常见的
        return list(valid_headers)[0]
    return None

def has_header_path(content):
    """检查是否已经包含头文件路径"""
    return bool(re.search(r'头文件路径', content))

def add_header_path_to_doc(doc_path, header_path):
    """在文档的功能说明部分添加头文件路径"""
    try:
        with open(doc_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except:
        print(f"  [错误] 无法读取: {doc_path}")
        return False
    
    if has_header_path(content):
        print(f"  [跳过] 已包含头文件路径")
        return False
    
    # 找到功能说明部分
    # 模式: ## 功能说明 后面跟的内容（直到下一个 ## 标题）
    pattern = r'(## 功能说明[^\n]*\n)'
    match = re.search(pattern, content)
    if not match:
        print(f"  [警告] 未找到 '## 功能说明' 部分")
        return False
    
    # 在功能说明标题后插入头文件路径
    header_line = f'头文件路径：`"{header_path}"`。\n\n'
    
    insert_pos = match.end()
    new_content = content[:insert_pos] + header_line + content[insert_pos:]
    
    # 写入
    try:
        with open(doc_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        print(f"  [完成] 添加头文件: {header_path}")
        return True
    except Exception as e:
        print(f"  [错误] 写入失败: {e}")
        return False

def main():
    print("=" * 60)
    print("步骤1: 构建函数到头文件的映射...")
    func_map = build_function_header_map()
    print(f"  共找到 {len(func_map)} 个函数映射")
    
    # 打印一些样例映射
    sample_funcs = ['asc_abs', 'asc_add', 'asc_sync', 'asc_get_block_idx', 'asc_load']
    for f in sample_funcs:
        if f in func_map:
            print(f"  {f} → {func_map[f]}")
    
    print()
    print("步骤2: 处理所有 Markdown 文档...")
    
    # 收集所有 .md 文件
    md_files = []
    for root, dirs, files in os.walk(DOCS_ROOT):
        for f in files:
            if f.endswith('.md'):
                md_files.append(os.path.join(root, f))
    
    print(f"  共找到 {len(md_files)} 个 Markdown 文件")
    print()
    
    # 统计
    processed = 0
    skipped = 0
    not_found = 0
    errors = 0
    
    for doc_path in sorted(md_files):
        rel_doc = os.path.relpath(doc_path, DOCS_ROOT)
        print(f"处理: {rel_doc}")
        
        header = get_header_for_doc(doc_path, func_map)
        
        if header:
            result = add_header_path_to_doc(doc_path, header)
            if result:
                processed += 1
            else:
                skipped += 1
        else:
            print(f"  [未找到] 无法确定头文件")
            not_found += 1
    
    print()
    print("=" * 60)
    print(f"处理完成:")
    print(f"  成功添加: {processed}")
    print(f"  已跳过(已有): {skipped}")
    print(f"  未找到头文件: {not_found}")
    print(f"  总计: {len(md_files)}")

if __name__ == '__main__':
    main()