import json
import os
import re
import urllib.parse
from pathlib import Path


def extract_950_support(file_path):
    """Returns True if Ascend 950PR/Ascend 950DT is supported, False if not, None if no Product Support section."""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception:
        return None

    if '## 产品支持情况' not in content and '## 产品支持情况' not in content:
        return None

    section_match = re.search(r'## 产品支持情况.*?(?=\n## |\Z)', content, re.DOTALL)
    if not section_match:
        return None

    section = section_match.group(0)

    rows = re.findall(r'<tr[^>]*>(.*?)</tr>', section, re.DOTALL | re.IGNORECASE)

    for row in rows:
        tds = re.findall(r'<td[^>]*>(.*?)</td>', row, re.DOTALL | re.IGNORECASE)
        if len(tds) >= 2:
            td1_text = re.sub(r'<[^>]+>', '', tds[0]).strip()
            td2_text = re.sub(r'<[^>]+>', '', tds[1]).strip()

            if '950PR' in td1_text and '950DT' in td1_text:
                if '√' in td2_text:
                    return True
                else:
                    return False

    return False


def main():
    source_dir = Path(r'C:\Users\10855\.openclaw\workspace\asc-devkit\docs\api\context')
    result = {}
    stats = {'with_table_supported': 0, 'with_table_unsupported': 0, 'without_table': 0, 'errors': 0}

    for file_path in sorted(source_dir.glob('*.md')):
        key = urllib.parse.quote(file_path.stem, safe='') + '.html'
        supported = extract_950_support(file_path)

        if supported is None:
            result[key] = True
            stats['without_table'] += 1
        else:
            result[key] = supported
            if supported:
                stats['with_table_supported'] += 1
            else:
                stats['with_table_unsupported'] += 1

    for file_path in sorted(source_dir.glob('*.rst')):
        key = urllib.parse.quote(file_path.stem, safe='') + '.html'
        result[key] = True

    output_path = source_dir / '_static' / 'version_filter.json'
    sorted_result = {k: result[k] for k in sorted(result.keys())}

    with open(output_path, 'w', encoding='utf-8') as f:
        json.dump(sorted_result, f, ensure_ascii=False)

    total = len(result)
    supported_count = sum(1 for v in result.values() if v)
    unsupported_count = sum(1 for v in result.values() if not v)

    print(f"Total entries: {total}")
    print(f"  Supported (true):  {supported_count}")
    print(f"  Unsupported (false): {unsupported_count}")
    print(f"")
    print(f"By category:")
    print(f"  With table + 950 supported:   {stats['with_table_supported']}")
    print(f"  With table + 950 unsupported: {stats['with_table_unsupported']}")
    print(f"  Without table (default true): {stats['without_table']}")
    print(f"")
    print(f"Output: {output_path}")


if __name__ == '__main__':
    main()
