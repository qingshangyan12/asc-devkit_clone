#!/usr/bin/env python3
"""
修正常规头文件映射，使用基于目录的正确映射。
"""
import os
import re

DOCS_ROOT = r"C:\JC\asc-devkit\docs\api\SIMD-API\C-API"

# 目录到头文件的映射
DIR_HEADER_MAP = {
    'vector_compute': 'c_api/vector_compute/vector_compute.h',
    'vector_datamove': 'c_api/vector_datamove/vector_datamove.h',
    'sync': 'c_api/sync/sync.h',
    'sys_var': 'c_api/sys_var/sys_var.h',
    'scalar_compute': 'c_api/scalar_compute/scalar_compute.h',
    'cube_compute': 'c_api/cube_compute/cube_compute.h',
    'cube_datamove': 'c_api/cube_datamove/cube_datamove.h',
    'cache_ctrl': 'c_api/cache_ctrl/cache_ctrl.h',
    'misc': 'c_api/misc/misc.h',
    'simd_atomic': 'c_api/atomic/atomic.h',
}

# reg子目录映射
REG_DIR_MAP = {
    'reg_vector': 'c_api/reg_compute/reg_vector.h',
    'reg_load': 'c_api/reg_compute/reg_load.h',
    'reg_store': 'c_api/reg_compute/reg_store.h',
}

# reg_vector中属于类型转换的函数（→ reg_convert.h）
CONVERSION_PATTERNS = [
    r'float2', r'half2', r'int\d+2', r'uint\d+2', r'bfloat\d+2',
    r'e\d+m\d+2', r'hif8\d*2', r'int4x\d+2', r'e\d+m\d+x\d+2',
    r'float2e\d+m\d+', r'float2hif8', r'float2int', r'float2bfloat',
    r'deq', r'vdeq', r'truncate',
]

def is_conversion_function(filename):
    """判断文件名是否对应类型转换函数"""
    basename = os.path.splitext(filename)[0]
    for pattern in CONVERSION_PATTERNS:
        if re.search(pattern, basename):
            return True
    return False

def get_correct_header(doc_path):
    """根据文档路径确定正确的头文件"""
    rel_path = os.path.relpath(doc_path, DOCS_ROOT)
    parts = rel_path.replace('\\', '/').split('/')
    
    # reg目录特殊处理
    if parts[0] == 'reg':
        if len(parts) >= 2 and parts[1] in REG_DIR_MAP:
            sub_dir = parts[1]
            filename = os.path.basename(doc_path)
            
            if sub_dir == 'reg_vector' and is_conversion_function(filename):
                return 'c_api/reg_compute/reg_convert.h'
            return REG_DIR_MAP[sub_dir]
        return None
    
    # 其他目录
    if parts[0] in DIR_HEADER_MAP:
        return DIR_HEADER_MAP[parts[0]]
    
    return None

def fix_doc_header(doc_path):
    """修正文档中的头文件路径"""
    correct_header = get_correct_header(doc_path)
    if not correct_header:
        return False
    
    try:
        with open(doc_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
    except:
        return False
    
    # 查找当前的头文件路径
    pattern = r'头文件路径：`"([^"]+)"`'
    match = re.search(pattern, content)
    if not match:
        return False
    
    current_header = match.group(1)
    
    if current_header == correct_header:
        return False  # 已经正确
    
    # 替换
    old_str = f'头文件路径：`"{current_header}"`'
    new_str = f'头文件路径：`"{correct_header}"`'
    new_content = content.replace(old_str, new_str)
    
    try:
        with open(doc_path, 'w', encoding='utf-8') as f:
            f.write(new_content)
        return True
    except:
        return False

def main():
    print("修正错误的头文件映射...")
    
    count = 0
    for root, dirs, files in os.walk(DOCS_ROOT):
        for f in files:
            if not f.endswith('.md'):
                continue
            filepath = os.path.join(root, f)
            
            if fix_doc_header(filepath):
                rel = os.path.relpath(filepath, DOCS_ROOT)
                correct = get_correct_header(filepath)
                print(f"  [修正] {rel} → {correct}")
                count += 1
    
    print(f"\n修正完成，共处理 {count} 个文件")

if __name__ == '__main__':
    main()