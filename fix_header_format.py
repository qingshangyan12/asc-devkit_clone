#!/usr/bin/env python3
"""
修正已添加头文件路径的格式：
1. 功能说明标题后空一行
2. 头文件路径后空一行再跟内容
3. 删除多余空行
"""
import os
import re

DOCS_ROOT = r"C:\JC\asc-devkit\docs\api\SIMD-API\C-API"

def fix_header_format(content):
    """修正头文件路径的格式"""
    
    # 匹配模式: ## 功能说明(可选锚点)后紧跟头文件路径（无空行）
    # 需要改成: ## 功能说明\n\n头文件路径：...\n\n内容
    
    # 模式1: ## 功能说明\n头文件路径：...（中间无空行）
    pattern1 = r'(## 功能说明[^\n]*)\n(头文件路径：[^\n]+)\n\n*'
    replacement1 = r'\1\n\n\2\n\n'
    content = re.sub(pattern1, replacement1, content)
    
    # 模式2: 头文件路径后有多余空行（多于1个空行）
    # 头文件路径行后应该只有一个空行
    pattern2 = r'(头文件路径：[^\n]+)\n\n\n+'
    replacement2 = r'\1\n\n'
    content = re.sub(pattern2, replacement2, content)
    
    # 模式3: 头文件路径后直接跟内容（无空行）
    pattern3 = r'(头文件路径：[^\n]+)\n(?!\n)(?!$)'
    replacement3 = r'\1\n\n'
    content = re.sub(pattern3, replacement3, content)
    
    return content

def main():
    print("修正头文件路径格式...")
    
    count = 0
    for root, dirs, files in os.walk(DOCS_ROOT):
        for f in files:
            if not f.endswith('.md'):
                continue
            filepath = os.path.join(root, f)
            
            try:
                with open(filepath, 'r', encoding='utf-8', errors='ignore') as fp:
                    content = fp.read()
            except:
                continue
            
            if '头文件路径' not in content:
                continue
            
            new_content = fix_header_format(content)
            
            if new_content != content:
                try:
                    with open(filepath, 'w', encoding='utf-8') as fp:
                        fp.write(new_content)
                    rel = os.path.relpath(filepath, DOCS_ROOT)
                    print(f"  [修正] {rel}")
                    count += 1
                except Exception as e:
                    print(f"  [错误] {filepath}: {e}")
    
    print(f"\n修正完成，共处理 {count} 个文件")

if __name__ == '__main__':
    main()