import json
import re

# 读取文档
file_path = 'D:/2.idea_workspace/branch/CloudIDP_Lite_AssistWriteService/markdown/md/rule-text.md'
with open(file_path, 'r', encoding='utf-8') as f:
    lines = f.readlines()

# 规则检查结果
issues = []

# 规则：检查是否存在多余空格
rule_name = 'punctuation-check'
rule_item = 'StaticCheck_punctuation_check'
rule_type = '规范性'
rule_level = '提示'

for line_num, line in enumerate(lines, start=1):
    line_stripped = line.rstrip()

    # 检查1：行尾多余空格（排除空行和表格行）
    if line != line.rstrip('\\n') and line.strip() != '' and not line.startswith('|'):
        end_spaces = len(line) - len(line.rstrip())
        issues.append({
            'line_number': str(line_num),
            'snippet': line_stripped,
            'description': '行尾存在多余空格',
            'rule_level': rule_level,
            'file_name': 'rule-text.md',
            'location': f'vscode://file/{file_path}:{line_num}'
        })

    # 检查2：中文标点前空格
    if re.search(r'\s+[，。：？！]', line):
        match = re.search(r'(\s+[，。：？！])', line)
        issues.append({
            'line_number': str(line_num),
            'snippet': match.group(1),
            'description': '中文标点（，。：？！）前不允许有空格',
            'rule_level': rule_level,
            'file_name': 'rule-text.md',
            'location': f'vscode://file/{file_path}:{line_num}'
        })

    # 检查3：连续多个空格
    if re.search(r'  +', line):
        match = re.search(r'(  +)', line)
        issues.append({
            'line_number': str(line_num),
            'snippet': match.group(1),
            'description': '存在连续多个空格',
            'rule_level': rule_level,
            'file_name': 'rule-text.md',
            'location': f'vscode://file/{file_path}:{line_num}'
        })

# 格式化输出
result = []
for issue in issues:
    result.append({
        'rule_group': '通用规则',
        'rule_item': rule_item,
        'rule_type': rule_type,
        'rule_name': rule_name,
        'snippet': issue['snippet'],
        'description': issue['description'],
        'rule_level': issue['rule_level'],
        'file_name': issue['file_name'],
        'line_number': issue['line_number'],
        'location': issue['location'],
        'suggestion': '删除多余空格'
    })

# 保存结果
output_file = 'D:/2.idea_workspace/branch/CloudIDP_Lite_AssistWriteService/output/punctuation/markdown-check-result/rule-text_punctuation-check.json'
with open(output_file, 'w', encoding='utf-8') as f:
    json.dump(result, f, ensure_ascii=False, indent=2)

print(f'检查完成，发现 {len(issues)} 个问题')
print(f'结果已保存到: {output_file}')
