#!/usr/bin/env python3
"""
Markdown文件清理脚本
功能：
1. 清理冗余锚点标签（如 <a name="xxx"></a>, <a id="xxx"></a>）
2. 将HTML表格转换为Markdown简易表格
3. 其他内容保持不变
"""

import re
import os
import sys
from pathlib import Path
from html.parser import HTMLParser


class TableParser(HTMLParser):
    def __init__(self, referenced_anchors=None):
        super().__init__()
        self.in_table = False
        self.in_thead = False
        self.in_tbody = False
        self.in_tr = False
        self.in_th = False
        self.in_td = False
        self.rows = []
        self.current_row = []
        self.current_cell = ""
        self.headers = []
        self.is_header_row = False
        self.referenced_anchors = referenced_anchors or set()
        self.in_link = False
        self.link_href = ""
        self.link_text = ""

    def handle_starttag(self, tag, attrs):
        if tag == "table":
            self.in_table = True
            self.rows = []
            self.headers = []
        elif tag == "thead":
            self.in_thead = True
        elif tag == "tbody":
            self.in_tbody = True
        elif tag == "tr":
            self.in_tr = True
            self.current_row = []
            self.is_header_row = self.in_thead
        elif tag == "th":
            self.in_th = True
            self.current_cell = ""
        elif tag == "td":
            self.in_td = True
            self.current_cell = ""
        elif tag == "a":
            if self.in_th or self.in_td:
                self.in_link = True
                self.link_text = ""
                for attr_name, attr_value in attrs:
                    if attr_name.lower() == "href":
                        self.link_href = attr_value
                    elif attr_name.lower() in ("name", "id"):
                        pass
        elif tag in ("p", "br"):
            if self.in_th or self.in_td:
                self.current_cell += "\n"

    def handle_endtag(self, tag):
        if tag == "table":
            self.in_table = False
        elif tag == "thead":
            self.in_thead = False
        elif tag == "tbody":
            self.in_tbody = False
        elif tag == "tr":
            self.in_tr = False
            if self.current_row:
                if self.is_header_row:
                    self.headers = self.current_row
                else:
                    self.rows.append(self.current_row)
            self.current_row = []
        elif tag == "th":
            self.in_th = False
            cell_content = self.current_cell.strip()
            cell_content = clean_anchor_tags(cell_content, self.referenced_anchors)
            cell_content = cell_content.replace("\n", "<br>")
            self.current_row.append(cell_content)
            self.current_cell = ""
        elif tag == "td":
            self.in_td = False
            cell_content = self.current_cell.strip()
            cell_content = clean_anchor_tags(cell_content, self.referenced_anchors)
            cell_content = cell_content.replace("\n", "<br>")
            self.current_row.append(cell_content)
            self.current_cell = ""
        elif tag == "a":
            if self.in_link and (self.in_th or self.in_td):
                if self.link_href and self.link_text:
                    self.current_cell += f"[{self.link_text}]({self.link_href})"
                elif self.link_text:
                    self.current_cell += self.link_text
                self.in_link = False
                self.link_href = ""
                self.link_text = ""

    def handle_data(self, data):
        if self.in_th or self.in_td:
            if self.in_link:
                self.link_text += data
            else:
                self.current_cell += data

    def get_markdown_table(self):
        if not self.headers and not self.rows:
            return ""
        all_rows = []
        if self.headers:
            all_rows.append(self.headers)
        all_rows.extend(self.rows)
        if not all_rows:
            return ""
        num_cols = max(len(row) for row in all_rows)
        normalized_rows = []
        for row in all_rows:
            padded_row = row + [""] * (num_cols - len(row))
            normalized_rows.append(padded_row)
        md_lines = []
        header_row = normalized_rows[0]
        md_lines.append("| " + " | ".join(header_row) + " |")
        separator = "| " + " | ".join(["---" for _ in range(num_cols)]) + " |"
        md_lines.append(separator)
        for row in normalized_rows[1:]:
            md_lines.append("| " + " | ".join(row) + " |")
        return "\n".join(md_lines)


def extract_referenced_anchors(content):
    referenced = set()
    link_pattern = re.compile(r'\]\(#([^)]+)\)')
    for match in link_pattern.finditer(content):
        referenced.add(match.group(1))
    return referenced

def clean_anchor_tags(content, referenced_anchors=None):
    if referenced_anchors is None:
        referenced_anchors = extract_referenced_anchors(content)
    anchor_pattern = re.compile(
        r'<a\s+[^>]*(?:name|id)="([^"]+)"[^>]*>\s*</a\s*>',
        re.IGNORECASE
    )
    def replace_anchor(match):
        anchor_name = match.group(1)
        if anchor_name in referenced_anchors:
            return match.group(0)
        return ""
    content = anchor_pattern.sub(replace_anchor, content)
    span_pattern = re.compile(
        r'<span\s+id="([^"]+)"[^>]*>\s*</span\s*>',
        re.IGNORECASE
    )
    def replace_span(match):
        span_id = match.group(1)
        if span_id in referenced_anchors:
            return match.group(0)
        return ""
    content = span_pattern.sub(replace_span, content)
    content = re.sub(r'\n\s*\n\s*\n', '\n\n', content)
    return content


def convert_html_tables_to_markdown(content, referenced_anchors=None):
    table_pattern = re.compile(
        r'<a\s+[^>]*name="[^"]*"[^>]*></a\s*>\s*\n?\s*<table[^>]*>.*?</table\s*>',
        re.DOTALL | re.IGNORECASE
    )
    simple_table_pattern = re.compile(
        r'<table[^>]*>.*?</table\s*>',
        re.DOTALL | re.IGNORECASE
    )
    def convert_table(match):
        table_html = match.group(0)
        anchor_match = re.match(r'^\s*(<a\s+[^>]*name="([^"]+)"[^>]*></a\s*>)\s*', table_html)
        preserved_anchor = ""
        if anchor_match:
            anchor_name = anchor_match.group(2)
            if anchor_name in (referenced_anchors or set()):
                preserved_anchor = anchor_match.group(1) + "\n"
            table_html = re.sub(r'^\s*<a\s+[^>]*name="[^"]*"[^>]*></a\s*>\s*\n?\s*', '', table_html)
        parser = TableParser(referenced_anchors)
        try:
            parser.feed(table_html)
            md_table = parser.get_markdown_table()
            result = preserved_anchor + md_table if md_table else preserved_anchor + table_html
            return result
        except Exception:
            return match.group(0)
    content = table_pattern.sub(convert_table, content)
    content = simple_table_pattern.sub(convert_table, content)
    return content


def clean_md_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()
    original_content = content
    referenced_anchors = extract_referenced_anchors(content)
    content = clean_anchor_tags(content, referenced_anchors)
    content = convert_html_tables_to_markdown(content, referenced_anchors)
    content = re.sub(r'\n{3,}', '\n\n', content)
    content = re.sub(r'[ \t]+\n', '\n', content)
    if content != original_content:
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
        return True
    return False


def process_files(paths):
    processed_count = 0
    changed_count = 0
    for path in paths:
        path_obj = Path(path)
        if path_obj.is_file() and path_obj.suffix.lower() == '.md':
            processed_count += 1
            if clean_md_file(str(path_obj)):
                changed_count += 1
                print(f"已处理: {path_obj}")
        elif path_obj.is_dir():
            for md_file in path_obj.rglob('*.md'):
                processed_count += 1
                if clean_md_file(str(md_file)):
                    changed_count += 1
                    print(f"已处理: {md_file}")
    print(f"\n处理完成: 共处理 {processed_count} 个文件，修改了 {changed_count} 个文件")


def main():
    if len(sys.argv) < 2:
        print("使用方法: python clean_md_file.py <文件或目录路径> [更多路径...]")
        print("示例:")
        print("  python clean_md_file.py docs/readme.md")
        print("  python clean_md_file.py docs/")
        print("  python clean_md_file.py file1.md file2.md docs/")
        sys.exit(1)
    paths = sys.argv[1:]
    process_files(paths)


if __name__ == "__main__":
    main()