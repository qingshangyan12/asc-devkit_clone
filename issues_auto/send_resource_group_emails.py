#!/usr/bin/env python3
"""按资源组发送 issue 邮件。"""

from __future__ import annotations

import argparse
import copy
import html
import json
import os
import re
import shutil
import smtplib
import sys
import unicodedata
from collections import defaultdict
from email.message import EmailMessage
from pathlib import Path

from openpyxl import Workbook, load_workbook
from openpyxl.styles import Font, PatternFill


ACCOUNT_MD_FILE = "gitcode_accounts.md"
SEND_REPORT_FILE = "resource_group_email_send_report.json"
ATTACHMENT_DIR = "resource_group_email_attachments"
KEEP_INTERMEDIATE_ENV = "KEEP_INTERMEDIATE_FILES"

SENDER_EMAIL = "wangxu243@h-partners.com"
SMTP_HOST = "smtp.huawei.com"
SMTP_USER = "xxxx"
SMTP_PASSWORD = "XXXXX"

# 调试时填测试邮箱列表；设为 None 或空列表时发送给对应资源组的责任人和 PL。
TEST_TO = ["wangxu243@h-partners.com"]
# 正式发送时，每个资源组都会额外发送给这里配置的邮箱。
EXTRA_TO = [
    "gaoyue6@huawei.com",
    "wangxu243@h-partners.com",
]

SUMMARY_GENERATED_TIME = "2026-08-14 08:54:22"
OVERDUE_UNCLOSED_DAYS = 14

PROPOSER_COLUMN = "提出者"
OWNER_COLUMN = "责任人"
TITLE_COLUMN = "标题"
STATUS_COLUMN = "状态"
RESOURCE_GROUP_COLUMN = "资源组"
OPEN_DURATION_COLUMN = "开启时长"
LAST_REPLY_TIME_COLUMN = "最后回复时间"
GITCODE_COLUMN = "gitcode"
NAME_COLUMN = "姓名"
EMAIL_COLUMN = "邮箱"
PL_COLUMN = "PL"
TITLE_COLUMN_WIDTH = 30
HIGHLIGHT_FILL_COLOR = "FCE4D6"
HIGHLIGHT_HTML_BACKGROUND = "#fce4d6"
HYPERLINK_FONT = Font(color="0563C1", underline="single")
DROP_COLUMNS = (STATUS_COLUMN, RESOURCE_GROUP_COLUMN, PL_COLUMN)
DEFAULT_SUMMARY_LINES = (
    f"生成时间: {SUMMARY_GENERATED_TIME}",
    "Issues 总数: 0，其中超期未关闭0个，未回复0个",
)
TARGET_RESOURCE_GROUPS = {
    "向量编程一组",
    "向量编程二组",
    "向量编程三组",
    "矩阵编程一组",
    "矩阵编程二组",
    "矩阵编程三组",
    "编程框架一组",
    "编程框架二组",
    "Pass开发组",
    "工具组",
    "通信编程组",
}

INVALID_VALUES = {
    "",
    "-",
    "nan",
    "none",
    "null",
    "未分配",
    "无",
}
OPEN_STATUS_VALUE = "open"

EMAIL_RE = re.compile(r"^[^@\s]+@[^@\s]+\.[^@\s]+$")
DURATION_PART_RE = re.compile(r"(\d+(?:\.\d+)?)\s*(天|小时|分钟)")
FORMATTED_ACCOUNT_RE = re.compile(r"^(.+?)\(([^()]+)\)$")
INVALID_ACCOUNT_CHARS_RE = re.compile(r"[\s@|]")
INVALID_FILENAME_CHARS_RE = re.compile(r'[\\/:*?"<>|]+')


def keep_intermediate_files() -> bool:
    """通过环境变量控制是否保留中间文件，默认不保留。"""
    value = os.environ.get(KEEP_INTERMEDIATE_ENV, "")
    return value.strip().lower() in {"1", "true", "yes", "y", "on"}


def cleanup_intermediate_files(script_dir: Path) -> None:
    """默认清理邮件附件目录和发送报告，避免留下中间文件。"""
    report_path = script_dir / SEND_REPORT_FILE
    if report_path.exists():
        report_path.unlink()
    shutil.rmtree(script_dir / ATTACHMENT_DIR, ignore_errors=True)


def normalize_cell(value: object) -> str:
    """统一清洗 Excel/Markdown 单元格内容，兼容全角空格等字符。"""
    if value is None:
        return ""
    return unicodedata.normalize("NFKC", str(value)).strip()


def is_invalid_value(value: object) -> bool:
    """判断是否为空值或占位值。"""
    return normalize_cell(value).lower() in INVALID_VALUES


def has_chinese(value: str) -> bool:
    """判断文本是否包含中文字符。"""
    return any("\u4e00" <= char <= "\u9fff" for char in value)


def is_gitcode_id(value: str) -> bool:
    """过滤空值、占位值，以及明显不是 gitcode 账号的内容。"""
    text = normalize_cell(value)
    if is_invalid_value(text):
        return False
    return not INVALID_ACCOUNT_CHARS_RE.search(text)


def is_email(value: object) -> bool:
    """判断是否是邮箱地址。"""
    return bool(EMAIL_RE.fullmatch(normalize_cell(value)))


def duration_to_hours(value: object) -> float:
    """把“开启时长”转换为小时，支持“7天3小时”“10小时”“30分钟”。"""
    text = normalize_cell(value)
    if is_invalid_value(text):
        return 0

    total_hours = 0.0
    for number, unit in DURATION_PART_RE.findall(text):
        amount = float(number)
        if unit == "天":
            total_hours += amount * 24
        elif unit == "小时":
            total_hours += amount
        elif unit == "分钟":
            total_hours += amount / 60
    return total_hours


def is_open_status(value: object) -> bool:
    """判断 issue 状态是否为 open；邮件统计只统计 open 状态。"""
    return normalize_cell(value).lower() == OPEN_STATUS_VALUE


def add_unique(items: list[str], seen: set[str], value: object) -> None:
    """按小写值去重，同时保留首次出现时的原始写法。"""
    text = normalize_cell(value)
    if is_invalid_value(text):
        return
    key = text.lower()
    if key not in seen:
        seen.add(key)
        items.append(text)


def normalize_email_list(values: list[str] | None) -> list[str]:
    """清洗邮箱列表，过滤空值并去重。"""
    if not values:
        return []
    emails: list[str] = []
    seen: set[str] = set()
    for value in values:
        if is_email(value):
            add_unique(emails, seen, value)
    return emails


def split_markdown_row(line: str) -> list[str]:
    """按 Markdown 表格分隔符拆分一行。"""
    return [normalize_cell(part) for part in line.strip().strip("|").split("|")]


def is_markdown_separator(row: list[str]) -> bool:
    """识别 Markdown 表头下方的分隔行。"""
    return bool(row) and all(re.fullmatch(r":?-{3,}:?", cell) for cell in row)


def parse_account_value(value: object) -> tuple[str, str]:
    """解析“中文名(gitcode)”或普通单元格，返回 gitcode 和中文名。"""
    text = normalize_cell(value)
    match = FORMATTED_ACCOUNT_RE.fullmatch(text)
    if match:
        return normalize_cell(match.group(2)), normalize_cell(match.group(1))
    if has_chinese(text):
        return "", text
    return text, ""


def is_same_account_person(left: object, right: object) -> bool:
    """判断两个账号/姓名单元格是否表示同一个人，优先比较 gitcode。"""
    if is_invalid_value(left) or is_invalid_value(right):
        return False
    left_account, left_name = parse_account_value(left)
    right_account, right_name = parse_account_value(right)
    if left_account and right_account:
        return left_account.lower() == right_account.lower()
    if left_name and right_name:
        return left_name == right_name
    return normalize_cell(left) == normalize_cell(right)


def is_overdue_issue(open_duration: object) -> bool:
    """判断 issue 是否超期未关闭；调用处已经过滤 open 状态。"""
    return duration_to_hours(open_duration) >= OVERDUE_UNCLOSED_DAYS * 24


def is_unreplied_issue(last_reply_time: object, proposer: object, owner: object) -> bool:
    """判断 issue 是否未回复；提出者和责任人相同时不算未回复。"""
    return is_invalid_value(last_reply_time) and not is_same_account_person(
        proposer, owner
    )


def should_highlight_issue(
    open_duration: object,
    last_reply_time: object,
    proposer: object,
    owner: object,
) -> bool:
    """判断 issue 行是否需要橙色底色。"""
    return is_overdue_issue(open_duration) or is_unreplied_issue(
        last_reply_time,
        proposer,
        owner,
    )


def header_indexes(header: list[str]) -> dict[str, int]:
    """生成表头到 0 基列号的映射。"""
    return {column_name: index for index, column_name in enumerate(header)}


def row_value(
    row: tuple[object, ...],
    indexes: dict[str, int],
    column_name: str,
) -> object:
    """从 values_only 行里按列名取值，缺列或越界返回空字符串。"""
    index = indexes.get(column_name)
    if index is None or index >= len(row):
        return ""
    return row[index]


def sheet_value(
    sheet,
    row_index: int,
    indexes: dict[str, int],
    column_name: str,
) -> object:
    """从普通 worksheet 里按列名取值，缺列返回空字符串。"""
    index = indexes.get(column_name)
    if index is None:
        return ""
    return sheet.cell(row=row_index, column=index + 1).value


def is_target_open_resource(
    resource_group: str,
    row_resource: object,
    row_status: object,
) -> bool:
    """判断行是否属于当前资源组且状态为 open。"""
    return normalize_cell(row_resource) == resource_group and is_open_status(
        row_status
    )


def read_account_md(path: Path) -> tuple[dict[str, dict[str, str]], dict[str, str]]:
    """读取新 Markdown，返回 gitcode 信息表和“姓名 -> gitcode”映射。"""
    account_info: dict[str, dict[str, str]] = {}
    name_to_account: dict[str, str] = {}
    headers: list[str] | None = None

    with path.open("r", encoding="utf-8") as file:
        for line in file:
            if not line.lstrip().startswith("|"):
                continue
            row = split_markdown_row(line)
            if is_markdown_separator(row):
                continue
            if headers is None:
                headers = row
                for column in (GITCODE_COLUMN, NAME_COLUMN, EMAIL_COLUMN, PL_COLUMN):
                    if column not in headers:
                        raise ValueError(f"{path} 缺少列: {column}")
                continue

            data = {
                header: normalize_cell(row[index])
                for index, header in enumerate(headers)
                if index < len(row)
            }
            account = data.get(GITCODE_COLUMN, "")
            name = data.get(NAME_COLUMN, "")
            if not is_gitcode_id(account) or has_chinese(account):
                continue
            account_info[account.lower()] = data
            if not is_invalid_value(name):
                name_to_account.setdefault(name, account)

    return account_info, name_to_account


def resolve_account(value: object, name_to_account: dict[str, str]) -> str | None:
    """把责任人单元格解析为 gitcode，中文名会按 Markdown 映射。"""
    account, name = parse_account_value(value)
    if not account and name:
        account = name_to_account.get(name, "")
    if not is_gitcode_id(account) or has_chinese(account):
        return None
    return account


def account_email(account: str, account_info: dict[str, dict[str, str]]) -> str | None:
    """根据 gitcode 查询邮箱。"""
    info = account_info.get(account.lower())
    if not info:
        return None
    email = info.get(EMAIL_COLUMN, "")
    return email if is_email(email) else None


def pl_email(
    pl_value: object,
    account_info: dict[str, dict[str, str]],
    name_to_account: dict[str, str],
) -> str | None:
    """根据 PL 字段查询邮箱；PL 可以是邮箱、gitcode、中文名或“中文名(gitcode)”。"""
    text = normalize_cell(pl_value)
    if is_invalid_value(text):
        return None
    if is_email(text):
        return text

    account = resolve_account(text, name_to_account)
    if account:
        return account_email(account, account_info)
    return None


def collect_group_recipients(
    excel_path: Path,
    account_info: dict[str, dict[str, str]],
    name_to_account: dict[str, str],
) -> dict[str, dict[str, list[str]]]:
    """按资源组收集责任人收件人和 PL 抄送人。"""
    groups: dict[str, dict[str, list[str]]] = defaultdict(
        lambda: {
            "owner_accounts": [],
            "owner_emails": [],
            "pl_values": [],
            "pl_emails": [],
        }
    )
    seen: dict[str, dict[str, set[str]]] = defaultdict(
        lambda: {
            "owner_accounts": set(),
            "owner_emails": set(),
            "pl_values": set(),
            "pl_emails": set(),
        }
    )

    workbook = load_workbook(excel_path, read_only=True, data_only=True)
    try:
        sheet = workbook.worksheets[0]
        rows = sheet.iter_rows(values_only=True)
        try:
            header = [normalize_cell(cell) for cell in next(rows)]
        except StopIteration:
            return {}
        indexes = header_indexes(header)
        if (
            OWNER_COLUMN not in header
            or RESOURCE_GROUP_COLUMN not in header
            or STATUS_COLUMN not in header
        ):
            return {}

        for row in rows:
            resource_group = normalize_cell(
                row_value(row, indexes, RESOURCE_GROUP_COLUMN)
            )
            if not is_open_status(row_value(row, indexes, STATUS_COLUMN)):
                continue
            if is_invalid_value(resource_group):
                continue
            if resource_group not in TARGET_RESOURCE_GROUPS:
                continue

            owner_account = resolve_account(
                row_value(row, indexes, OWNER_COLUMN),
                name_to_account,
            )
            if not owner_account:
                continue

            group = groups[resource_group]
            group_seen = seen[resource_group]
            add_unique(
                group["owner_accounts"],
                group_seen["owner_accounts"],
                owner_account,
            )

            email = account_email(owner_account, account_info)
            if email:
                add_unique(group["owner_emails"], group_seen["owner_emails"], email)

            owner_info = account_info.get(owner_account.lower(), {})
            pl_value = owner_info.get(PL_COLUMN, "")
            if not is_invalid_value(pl_value):
                add_unique(group["pl_values"], group_seen["pl_values"], pl_value)
            email = pl_email(pl_value, account_info, name_to_account)
            if email:
                add_unique(group["pl_emails"], group_seen["pl_emails"], email)
    finally:
        workbook.close()

    return dict(groups)


def safe_filename(value: str) -> str:
    """把资源组名称转换为可用文件名。"""
    text = INVALID_FILENAME_CHARS_RE.sub("_", normalize_cell(value))
    return text or "未命名资源组"


def copy_excel_cell(source, target, keep_hyperlink: bool) -> None:
    """复制单元格内容和基础格式；第一列需要保留超链接。"""
    target.value = source.value
    if source.has_style:
        target.font = copy.copy(source.font)
        target.fill = copy.copy(source.fill)
        target.border = copy.copy(source.border)
        target.alignment = copy.copy(source.alignment)
        target.protection = copy.copy(source.protection)
        target.number_format = source.number_format
    if source.comment:
        target.comment = copy.copy(source.comment)
    if keep_hyperlink and source.hyperlink:
        target.hyperlink = source.hyperlink.target
        target.font = copy.copy(HYPERLINK_FONT)
        if source.hyperlink.tooltip:
            target.hyperlink.tooltip = source.hyperlink.tooltip
        if source.hyperlink.display:
            target.hyperlink.display = source.hyperlink.display


def apply_highlight_fill(cell) -> None:
    """给需要提醒的 issue 行设置橙色底色。"""
    cell.fill = PatternFill(
        fill_type="solid",
        fgColor=HIGHLIGHT_FILL_COLOR,
    )


def enable_wrap_text(cell) -> None:
    """设置单元格自动换行，同时保留原有对齐方式。"""
    alignment = copy.copy(cell.alignment)
    alignment.wrap_text = True
    cell.alignment = alignment


def build_resource_group_attachment(
    source_excel: Path, output_dir: Path, resource_group: str
) -> Path | None:
    """为单个资源组生成 Excel 附件，删除状态/资源组/PL 列并保留第一列超链接。"""
    source_workbook = load_workbook(source_excel)
    target_workbook = Workbook()
    target_workbook.remove(target_workbook.active)
    has_rows = False

    try:
        source_sheet = source_workbook.worksheets[0]
        header = [normalize_cell(cell.value) for cell in source_sheet[1]]
        indexes = header_indexes(header)
        if RESOURCE_GROUP_COLUMN not in header or STATUS_COLUMN not in header:
            target_workbook.close()
            return None

        keep_columns = [
            index
            for index, column_name in enumerate(header, start=1)
            if column_name not in DROP_COLUMNS
        ]

        matched_rows: list[tuple[int, bool]] = []
        for row_index in range(2, source_sheet.max_row + 1):
            if not is_target_open_resource(
                resource_group,
                sheet_value(source_sheet, row_index, indexes, RESOURCE_GROUP_COLUMN),
                sheet_value(source_sheet, row_index, indexes, STATUS_COLUMN),
            ):
                continue
            needs_highlight = should_highlight_issue(
                sheet_value(source_sheet, row_index, indexes, OPEN_DURATION_COLUMN),
                sheet_value(source_sheet, row_index, indexes, LAST_REPLY_TIME_COLUMN),
                sheet_value(source_sheet, row_index, indexes, PROPOSER_COLUMN),
                sheet_value(source_sheet, row_index, indexes, OWNER_COLUMN),
            )
            matched_rows.append((row_index, needs_highlight))

        if not matched_rows:
            target_workbook.close()
            return None

        has_rows = True
        target_sheet = target_workbook.create_sheet(source_sheet.title)

        for target_column, source_column in enumerate(keep_columns, start=1):
            source_dimension = source_sheet.column_dimensions[
                source_sheet.cell(row=1, column=source_column).column_letter
            ]
            target_dimension = target_sheet.column_dimensions[
                target_sheet.cell(row=1, column=target_column).column_letter
            ]
            is_title_column = header[source_column - 1] == TITLE_COLUMN
            target_dimension.width = (
                TITLE_COLUMN_WIDTH if is_title_column else source_dimension.width
            )
            copy_excel_cell(
                source_sheet.cell(row=1, column=source_column),
                target_sheet.cell(row=1, column=target_column),
                keep_hyperlink=(target_column == 1),
            )
            if is_title_column:
                enable_wrap_text(target_sheet.cell(row=1, column=target_column))

        for target_row, (source_row, needs_highlight) in enumerate(matched_rows, start=2):
            for target_column, source_column in enumerate(keep_columns, start=1):
                is_title_column = header[source_column - 1] == TITLE_COLUMN
                target_cell = target_sheet.cell(row=target_row, column=target_column)
                copy_excel_cell(
                    source_sheet.cell(row=source_row, column=source_column),
                    target_cell,
                    keep_hyperlink=(target_column == 1),
                )
                if needs_highlight:
                    apply_highlight_fill(target_cell)
                if is_title_column:
                    enable_wrap_text(target_cell)
    finally:
        source_workbook.close()

    if not has_rows:
        target_workbook.close()
        return None

    output_dir.mkdir(parents=True, exist_ok=True)
    attachment_path = output_dir / f"{safe_filename(resource_group)}.xlsx"
    target_workbook.save(attachment_path)
    target_workbook.close()
    return attachment_path


def build_group_attachments(
    source_excel: Path, output_dir: Path, resource_groups: list[str]
) -> dict[str, Path]:
    """按资源组生成附件文件。"""
    attachments: dict[str, Path] = {}
    for resource_group in resource_groups:
        attachment_path = build_resource_group_attachment(
            source_excel, output_dir, resource_group
        )
        if attachment_path:
            attachments[resource_group] = attachment_path
    return attachments


def html_cell(
    value: object,
    link: str | None = None,
    header: bool = False,
    wrap: bool = False,
    highlight: bool = False,
) -> str:
    """生成 HTML 单元格，第一列可带超链接。"""
    tag = "th" if header else "td"
    text = html.escape(normalize_cell(value))
    if link and text:
        text = (
            f'<a href="{html.escape(link, quote=True)}" '
            'style="color:#0969da;text-decoration:none;border-bottom:none;">'
            f"{text}</a>"
        )
    content_style = "line-height:18px;mso-line-height-rule:exactly;text-decoration:none;"
    if highlight:
        content_style += f"background-color:{HIGHLIGHT_HTML_BACKGROUND};"
    text = f'<div style="{content_style}">{text}</div>'
    style = (
        "border:1px solid #d0d7de;padding:6px 8px;text-align:left;"
        "vertical-align:top;line-height:18px;mso-line-height-rule:exactly;"
        "text-decoration:none;"
    )
    if wrap:
        style += "width:30em;max-width:30em;white-space:normal;word-break:break-word;"
    else:
        style += "white-space:nowrap;"
    if header:
        style += "background:#f6f8fa;font-weight:600;"
    elif highlight:
        style += f"background-color:{HIGHLIGHT_HTML_BACKGROUND};"
    return f"<{tag} style=\"{style}\">{text}</{tag}>"


def build_resource_group_preview_html(source_excel: Path, resource_group: str) -> str:
    """生成资源组对应 Excel 内容的 HTML 预览。"""
    workbook = load_workbook(source_excel, data_only=True)
    try:
        sheet = workbook.worksheets[0]
        header = [normalize_cell(cell.value) for cell in sheet[1]]
        indexes = header_indexes(header)
        if RESOURCE_GROUP_COLUMN not in header or STATUS_COLUMN not in header:
            return "<p>未找到资源组或状态列。</p>"

        keep_columns = [
            index
            for index, column_name in enumerate(header, start=1)
            if column_name not in DROP_COLUMNS
        ]
        if not keep_columns:
            return "<p>无可展示列。</p>"

        rows_html = []
        header_cells = [
            html_cell(
                sheet.cell(row=1, column=source_column).value,
                header=True,
                wrap=(header[source_column - 1] == TITLE_COLUMN),
            )
            for source_column in keep_columns
        ]
        rows_html.append("<tr>" + "".join(header_cells) + "</tr>")

        for row_index in range(2, sheet.max_row + 1):
            if not is_target_open_resource(
                resource_group,
                sheet_value(sheet, row_index, indexes, RESOURCE_GROUP_COLUMN),
                sheet_value(sheet, row_index, indexes, STATUS_COLUMN),
            ):
                continue
            needs_highlight = should_highlight_issue(
                sheet_value(sheet, row_index, indexes, OPEN_DURATION_COLUMN),
                sheet_value(sheet, row_index, indexes, LAST_REPLY_TIME_COLUMN),
                sheet_value(sheet, row_index, indexes, PROPOSER_COLUMN),
                sheet_value(sheet, row_index, indexes, OWNER_COLUMN),
            )

            cells = []
            for target_column, source_column in enumerate(keep_columns, start=1):
                source_cell = sheet.cell(row=row_index, column=source_column)
                link = (
                    source_cell.hyperlink.target
                    if target_column == 1 and source_cell.hyperlink
                    else None
                )
                cells.append(
                    html_cell(
                        source_cell.value,
                        link=link,
                        wrap=(header[source_column - 1] == TITLE_COLUMN),
                        highlight=needs_highlight,
                    )
                )
            row_style = (
                f' style="background-color:{HIGHLIGHT_HTML_BACKGROUND};"'
                if needs_highlight
                else ""
            )
            rows_html.append(f"<tr{row_style}>" + "".join(cells) + "</tr>")

        if len(rows_html) == 1:
            return "<p>该资源组无待跟进 issue。</p>"
        return (
            '<table cellspacing="0" cellpadding="0" '
            'style="border-collapse:collapse;border-spacing:0;'
            'font-family:Arial,Microsoft YaHei,sans-serif;font-size:13px;">'
            + "".join(rows_html)
            + "</table>"
        )
    finally:
        workbook.close()


def build_group_previews(source_excel: Path, resource_groups: list[str]) -> dict[str, str]:
    """按资源组生成邮件正文中的 HTML 预览。"""
    return {
        resource_group: build_resource_group_preview_html(source_excel, resource_group)
        for resource_group in resource_groups
    }


def build_summary_lines(source_excel: Path, resource_group: str) -> tuple[str, str]:
    """生成单个资源组的统计信息；只统计 open，最后回复时间为空表示未回复。"""
    workbook = load_workbook(source_excel, read_only=True, data_only=True)
    try:
        sheet = workbook.worksheets[0]
        rows = sheet.iter_rows(values_only=True)
        try:
            header = [normalize_cell(cell) for cell in next(rows)]
        except StopIteration:
            issue_count = 0
            overdue_unclosed_count = 0
            unreplied_count = 0
        else:
            indexes = header_indexes(header)
            issue_count = 0
            overdue_unclosed_count = 0
            unreplied_count = 0
            for row in rows:
                if not any(not is_invalid_value(cell) for cell in row):
                    continue
                if not is_target_open_resource(
                    resource_group,
                    row_value(row, indexes, RESOURCE_GROUP_COLUMN),
                    row_value(row, indexes, STATUS_COLUMN),
                ):
                    continue

                issue_count += 1
                open_duration = row_value(row, indexes, OPEN_DURATION_COLUMN)
                if is_overdue_issue(open_duration):
                    overdue_unclosed_count += 1

                if is_unreplied_issue(
                    row_value(row, indexes, LAST_REPLY_TIME_COLUMN),
                    row_value(row, indexes, PROPOSER_COLUMN),
                    row_value(row, indexes, OWNER_COLUMN),
                ):
                    unreplied_count += 1
    finally:
        workbook.close()

    return (
        f"生成时间: {SUMMARY_GENERATED_TIME}",
        (
            f"Issues 总数: {issue_count}，其中超期未关闭{overdue_unclosed_count}个，"
            f"未回复{unreplied_count}个"
        ),
    )


def build_group_summaries(
    source_excel: Path, resource_groups: list[str]
) -> dict[str, tuple[str, str]]:
    """按资源组生成邮件表格上方的统计信息。"""
    return {
        resource_group: build_summary_lines(source_excel, resource_group)
        for resource_group in resource_groups
    }


def build_owner_issue_counts(
    source_excel: Path,
    resource_group: str,
) -> list[tuple[str, int]]:
    """统计单个资源组下每个责任人的 open issue 数量。"""
    counts: dict[str, int] = defaultdict(int)
    workbook = load_workbook(source_excel, read_only=True, data_only=True)
    try:
        sheet = workbook.worksheets[0]
        rows = sheet.iter_rows(values_only=True)
        try:
            header = [normalize_cell(cell) for cell in next(rows)]
        except StopIteration:
            return []

        indexes = header_indexes(header)
        for row in rows:
            if not any(not is_invalid_value(cell) for cell in row):
                continue
            if not is_target_open_resource(
                resource_group,
                row_value(row, indexes, RESOURCE_GROUP_COLUMN),
                row_value(row, indexes, STATUS_COLUMN),
            ):
                continue

            owner = normalize_cell(row_value(row, indexes, OWNER_COLUMN))
            if is_invalid_value(owner):
                owner = "未分配"
            counts[owner] += 1
    finally:
        workbook.close()

    return sorted(counts.items(), key=lambda item: (-item[1], item[0]))


def build_owner_issue_count_table_html(owner_counts: list[tuple[str, int]]) -> str:
    """生成责任人 issue 数量统计表。"""
    if not owner_counts:
        return ""

    rows = [
        "<tr>"
        + html_cell("责任人", header=True)
        + html_cell("issue数量", header=True)
        + "</tr>"
    ]
    for owner, count in owner_counts:
        rows.append(
            "<tr>"
            + html_cell(owner)
            + html_cell(str(count))
            + "</tr>"
        )

    return (
        '<table cellspacing="0" cellpadding="0" '
        'style="border-collapse:collapse;border-spacing:0;'
        'font-family:Arial,Microsoft YaHei,sans-serif;font-size:13px;'
        'margin-bottom:12px;">'
        + "".join(rows)
        + "</table>"
    )


def build_group_owner_tables(
    source_excel: Path,
    resource_groups: list[str],
) -> dict[str, str]:
    """按资源组生成责任人 issue 数量统计表。"""
    return {
        resource_group: build_owner_issue_count_table_html(
            build_owner_issue_counts(source_excel, resource_group)
        )
        for resource_group in resource_groups
    }


def build_summary_html(summary_lines: tuple[str, str]) -> str:
    """生成邮件统计 HTML：标签加粗，统计数字标红。"""
    generated_line, issue_line = summary_lines
    generated_label, _, generated_value = generated_line.partition(":")
    issue_label, _, issue_value = issue_line.partition(":")
    issue_value_html = re.sub(
        r"\d+",
        lambda match: (
            '<span style="color:#cf222e;font-weight:600;">'
            f"{match.group(0)}</span>"
        ),
        html.escape(issue_value),
    )
    return (
        f"<div><strong>{html.escape(generated_label)}</strong>:"
        f"{html.escape(generated_value)}</div>"
        f"<div><strong>{html.escape(issue_label)}</strong>:"
        f"{issue_value_html}</div>"
    )


def build_message(
    resource_group: str,
    to_addrs: list[str],
    attachment_path: Path | None,
    preview_html: str,
    summary_lines: tuple[str, str],
    owner_table_html: str,
) -> EmailMessage:
    """构造邮件，附件为对应资源组 Excel。"""
    message = EmailMessage()
    message["From"] = SENDER_EMAIL
    message["To"] = ", ".join(to_addrs)
    message["Subject"] = f"{resource_group} asc-devkit仓 Issue 提醒"
    text_body = "\n".join(summary_lines) + "\n\n"
    summary_html = build_summary_html(summary_lines)
    html_body = f"""\
<html>
  <body>
    <div style="margin-bottom:12px;">{summary_html}</div>
    {owner_table_html}
    {preview_html}
  </body>
</html>
"""
    message.set_content(text_body)
    message.add_alternative(html_body, subtype="html")
    if attachment_path:
        message.add_attachment(
            attachment_path.read_bytes(),
            maintype="application",
            subtype="vnd.openxmlformats-officedocument.spreadsheetml.sheet",
            filename=attachment_path.name,
        )
    return message


def send_messages(
    groups: dict[str, dict[str, list[str]]],
    attachments: dict[str, Path],
    previews: dict[str, str],
    summaries: dict[str, tuple[str, str]],
    owner_tables: dict[str, str],
) -> list[dict[str, object]]:
    """按资源组直接发送邮件，并返回发送报告。"""
    reports: list[dict[str, object]] = []

    try:
        smtp = smtplib.SMTP(SMTP_HOST)
        smtp.login(SMTP_USER, SMTP_PASSWORD)

        for resource_group in sorted(groups):
            group = groups[resource_group]
            real_to_addrs: list[str] = []
            real_to_seen: set[str] = set()
            for email in group["owner_emails"] + group["pl_emails"]:
                add_unique(real_to_addrs, real_to_seen, email)

            test_to_addrs = normalize_email_list(TEST_TO)
            extra_to_addrs = normalize_email_list(EXTRA_TO)
            if test_to_addrs:
                to_addrs = test_to_addrs
            else:
                to_addrs = []
                to_seen: set[str] = set()
                for email in real_to_addrs + extra_to_addrs:
                    add_unique(to_addrs, to_seen, email)
            if not to_addrs:
                print(f"{resource_group} 无有效收件人，跳过发送")
                continue

            attachment_path = attachments.get(resource_group)
            message = build_message(
                resource_group,
                to_addrs,
                attachment_path,
                previews.get(resource_group, ""),
                summaries.get(resource_group, DEFAULT_SUMMARY_LINES),
                owner_tables.get(resource_group, ""),
            )

            report = {
                "resource_group": resource_group,
                "real_to_addrs": real_to_addrs,
                "actual_to_addrs": to_addrs,
                "extra_to_addrs": extra_to_addrs,
                "owner_accounts": group["owner_accounts"],
                "owner_emails": group["owner_emails"],
                "pl_emails": group["pl_emails"],
                "pl_values": group["pl_values"],
                "attachment": str(attachment_path) if attachment_path else "",
                "sent": True,
            }
            reports.append(report)

            smtp.sendmail(SENDER_EMAIL, to_addrs, message.as_string())
        print("Email send success")
    except smtplib.SMTPException as exc:
        print(f"Error: unable to send email:{exc}")
        raise
    finally:
        if "smtp" in locals():
            try:
                smtp.quit()
            except smtplib.SMTPException:
                pass

    return reports


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "按资源组发送邮件：责任人和 PL 作为收件人；"
            "每封邮件附带对应资源组 Excel，附件删除状态/资源组/PL 列并保留第一列超链接；"
            "TEST_TO 列表有值时发送到测试邮箱，TEST_TO = None 或空列表时发送给资源组责任人、PL 和 EXTRA_TO。"
        )
    )
    parser.add_argument("excel", help="已更新后的 issue Excel 文件")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    script_dir = Path(__file__).resolve().parent

    excel_path = Path(args.excel)
    if not excel_path.is_file():
        print(f"Excel 文件不存在: {excel_path}", file=sys.stderr)
        return 1

    account_md = script_dir / ACCOUNT_MD_FILE
    if not account_md.is_file():
        print(f"账号文件不存在: {account_md}", file=sys.stderr)
        return 1

    account_info, name_to_account = read_account_md(account_md)
    groups = collect_group_recipients(excel_path, account_info, name_to_account)
    keep_intermediate = keep_intermediate_files()
    reports: list[dict[str, object]] = []
    try:
        attachments = build_group_attachments(
            excel_path,
            script_dir / ATTACHMENT_DIR,
            sorted(groups),
        )
        previews = build_group_previews(excel_path, sorted(groups))
        summaries = build_group_summaries(excel_path, sorted(groups))
        owner_tables = build_group_owner_tables(excel_path, sorted(groups))
        reports = send_messages(
            groups,
            attachments,
            previews,
            summaries,
            owner_tables,
        )

        print(f"发送资源组邮件数: {len(reports)}")
        if keep_intermediate:
            report_path = script_dir / SEND_REPORT_FILE
            report_path.write_text(
                json.dumps(reports, ensure_ascii=False, indent=2) + "\n",
                encoding="utf-8",
            )
            print(f"已生成附件目录 {script_dir / ATTACHMENT_DIR}")
            print(f"已写入 {report_path}")
    finally:
        if not keep_intermediate:
            cleanup_intermediate_files(script_dir)
            print("未保留邮件附件和发送报告")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
