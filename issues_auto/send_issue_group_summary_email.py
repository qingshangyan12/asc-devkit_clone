#!/usr/bin/env python3
"""发送 issue 资源组汇总邮件。"""

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
from dataclasses import dataclass
from datetime import datetime
from email.message import EmailMessage
from pathlib import Path

from openpyxl import Workbook, load_workbook
from openpyxl.styles import Font, PatternFill


ACCOUNT_MD_FILE = "gitcode_accounts.md"
ATTACHMENT_DIR = "issue_group_summary_email_attachments"
SEND_REPORT_FILE = "issue_group_summary_email_send_report.json"
ATTACHMENT_FILE = "asc-devkit_issue_group_summary.xlsx"
KEEP_INTERMEDIATE_ENV = "KEEP_INTERMEDIATE_FILES"

SENDER_EMAIL = "wangxu243@h-partners.com"
SMTP_HOST = "smtp.huawei.com"
SMTP_USER_ENV = "SMTP_USER"
SMTP_PASSWORD_ENV = "SMTP_PASSWORD"

# 调试时填测试邮箱列表；设为 None 或空列表时发送给固定收件人和组件 PL。
TEST_TO = ["wangxu243@h-partners.com"]

# 正式发送时固定收件人。魏霞邮箱如果和实际不一致，只改这里。
FIXED_TO_EMAILS = [
    "guoyafeng1@huawei.com",
    "weixia@huawei.com",
]

OVERDUE_UNCLOSED_DAYS = 14
OTHER_RESOURCE_GROUP = "其他组"
SUBJECT = "asc-devkit仓 Issue 汇总提醒"

PROPOSER_COLUMN = "提出者"
OWNER_COLUMN = "责任人"
TITLE_COLUMN = "标题"
STATUS_COLUMN = "状态"
RESOURCE_GROUP_COLUMN = "资源组"
OPEN_DURATION_COLUMN = "开启时长"
LAST_REPLY_TIME_COLUMN = "最后回复时间"
OPERATOR_TEST_COLUMN = "算子内测"
IS_OPERATOR_GROUP_COLUMN = "是否为算子组"
GITCODE_COLUMN = "gitcode"
OLD_GITCODE_COLUMN = "gitcode_id"
NAME_COLUMN = "姓名"
EMAIL_COLUMN = "邮箱"
PL_COLUMN = "PL"
TITLE_COLUMN_WIDTH = 30
HIGHLIGHT_FILL_COLOR = "FCE4D6"
HYPERLINK_FONT = Font(color="0563C1", underline="single")
DROP_COLUMNS = (
    STATUS_COLUMN,
    RESOURCE_GROUP_COLUMN,
    PL_COLUMN,
    IS_OPERATOR_GROUP_COLUMN,
)

SUMMARY_HEADERS = (
    RESOURCE_GROUP_COLUMN,
    PL_COLUMN,
    "issue数量",
    "未回复",
    "超期未关闭",
    OPERATOR_TEST_COLUMN,
)
REQUIRED_COLUMNS = (
    STATUS_COLUMN,
    RESOURCE_GROUP_COLUMN,
    OPEN_DURATION_COLUMN,
    LAST_REPLY_TIME_COLUMN,
    PROPOSER_COLUMN,
    OWNER_COLUMN,
    OPERATOR_TEST_COLUMN,
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
    "AscendC SE组",
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
MARKDOWN_SEPARATOR_RE = re.compile(r":?-{3,}:?")
FILENAME_TIME_RE = re.compile(r"(\d{8})_(\d{6})")


@dataclass
class GroupStats:
    """单个资源组的 issue 统计结果。"""

    issue_count: int = 0
    unreplied_count: int = 0
    overdue_unclosed_count: int = 0
    operator_test_count: int = 0
    unassigned_count: int = 0


@dataclass
class AccountBook:
    """Markdown 账号表中用于找收件人的信息。"""

    name_to_email: dict[str, str]
    account_to_email: dict[str, str]
    account_to_name: dict[str, str]
    email_to_name: dict[str, str]
    group_to_pl_values: dict[str, list[str]]


def keep_intermediate_files() -> bool:
    """通过环境变量控制是否保留附件和发送报告，默认不保留。"""
    value = os.environ.get(KEEP_INTERMEDIATE_ENV, "")
    return value.strip().lower() in {"1", "true", "yes", "y", "on"}


def cleanup_intermediate_files(script_dir: Path) -> None:
    """默认清理汇总附件和发送报告。"""
    report_path = script_dir / SEND_REPORT_FILE
    if report_path.exists():
        report_path.unlink()
    shutil.rmtree(script_dir / ATTACHMENT_DIR, ignore_errors=True)


def normalize_cell(value: object) -> str:
    """统一清洗 Excel/Markdown 单元格内容，兼容全角空格等字符。"""
    if value is None:
        return ""
    return unicodedata.normalize("NFKC", str(value)).strip()


def smtp_credentials() -> tuple[str, str]:
    """从环境变量读取 SMTP 用户名和密码。"""
    user = normalize_cell(os.environ.get(SMTP_USER_ENV, ""))
    password = os.environ.get(SMTP_PASSWORD_ENV, "")
    if not user or not password:
        raise ValueError(
            f"请先设置环境变量 {SMTP_USER_ENV} 和 {SMTP_PASSWORD_ENV}"
        )
    return user, password


def is_invalid_value(value: object) -> bool:
    """判断是否为空值或占位值。"""
    return normalize_cell(value).lower() in INVALID_VALUES


def is_email(value: object) -> bool:
    """判断是否是邮箱地址。"""
    return bool(EMAIL_RE.fullmatch(normalize_cell(value)))


def has_chinese(value: str) -> bool:
    """判断文本是否包含中文字符。"""
    return any("\u4e00" <= char <= "\u9fff" for char in value)


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
    return bool(row) and all(MARKDOWN_SEPARATOR_RE.fullmatch(cell) for cell in row)


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


def parse_account_value(value: object) -> tuple[str, str]:
    """解析“中文名(gitcode)”或普通单元格，返回 gitcode 和中文名。"""
    text = normalize_cell(value)
    match = FORMATTED_ACCOUNT_RE.fullmatch(text)
    if match:
        return normalize_cell(match.group(2)), normalize_cell(match.group(1))
    if has_chinese(text):
        return "", text
    return text, ""


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


def is_unassigned_owner(owner: object) -> bool:
    """判断 issue 是否没有责任人。"""
    return is_invalid_value(owner)


def generated_time_from_excel_name(excel_path: Path) -> str:
    """从 Excel 文件名提取生成时间，格式如 issue_20260813_085422.xlsx。"""
    match = FILENAME_TIME_RE.search(excel_path.stem)
    if not match:
        raise ValueError(
            f"Excel 文件名不合规，缺少 YYYYMMDD_HHMMSS 时间戳: {excel_path.name}"
        )
    try:
        generated_time = datetime.strptime(
            "".join(match.groups()),
            "%Y%m%d%H%M%S",
        )
    except ValueError as exc:
        raise ValueError(
            f"Excel 文件名时间戳非法，应为 YYYYMMDD_HHMMSS: {excel_path.name}"
        ) from exc
    return generated_time.strftime("%Y-%m-%d %H:%M:%S")


def resource_group_bucket(value: object) -> str:
    """目标资源组按原名统计，其他资源组统一归到“其他组”。"""
    resource_group = normalize_cell(value)
    if resource_group in TARGET_RESOURCE_GROUPS:
        return resource_group
    return OTHER_RESOURCE_GROUP


def is_row_in_resource_group_bucket(row_resource: object, resource_group: str) -> bool:
    """判断行资源组归并后是否属于目标 sheet。"""
    return resource_group_bucket(row_resource) == resource_group


def group_sort_key(item: tuple[str, GroupStats]) -> tuple[int, str]:
    """排序时把“其他组”放到最后。"""
    resource_group, _ = item
    return (1 if resource_group == OTHER_RESOURCE_GROUP else 0, resource_group)


def validate_columns(header: list[str]) -> None:
    """检查 Excel 是否包含汇总所需列。"""
    missing_columns = [column for column in REQUIRED_COLUMNS if column not in header]
    if missing_columns:
        raise ValueError("Excel 缺少列: " + ", ".join(missing_columns))


def read_account_md(path: Path) -> AccountBook:
    """读取 Markdown 账号表，用于按姓名、gitcode、资源组查邮箱和 PL。"""
    name_to_email: dict[str, str] = {}
    account_to_email: dict[str, str] = {}
    account_to_name: dict[str, str] = {}
    email_to_name: dict[str, str] = {}
    group_to_pl_values: dict[str, list[str]] = defaultdict(list)
    group_to_pl_seen: dict[str, set[str]] = defaultdict(set)
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
                if EMAIL_COLUMN not in headers or NAME_COLUMN not in headers:
                    raise ValueError(f"{path} 缺少列: {EMAIL_COLUMN} 或 {NAME_COLUMN}")
                if GITCODE_COLUMN not in headers and OLD_GITCODE_COLUMN not in headers:
                    raise ValueError(
                        f"{path} 缺少列: {GITCODE_COLUMN} 或 {OLD_GITCODE_COLUMN}"
                    )
                continue

            data = {
                header: normalize_cell(row[index])
                for index, header in enumerate(headers)
                if index < len(row)
            }
            email = data.get(EMAIL_COLUMN, "")

            account = data.get(GITCODE_COLUMN) or data.get(OLD_GITCODE_COLUMN, "")
            account = normalize_cell(account)
            name = data.get(NAME_COLUMN, "")

            if account and is_email(email):
                account_to_email.setdefault(account.lower(), email)

            if not is_invalid_value(name) and is_email(email):
                name_to_email.setdefault(name, email)
                email_to_name.setdefault(email.lower(), name)

            if account and not is_invalid_value(name):
                account_to_name.setdefault(account.lower(), name)

            resource_group = data.get(RESOURCE_GROUP_COLUMN, "")
            pl_value = data.get(PL_COLUMN, "")
            if not is_invalid_value(resource_group) and not is_invalid_value(pl_value):
                add_unique(
                    group_to_pl_values[resource_group],
                    group_to_pl_seen[resource_group],
                    pl_value,
                )

    return AccountBook(
        name_to_email=name_to_email,
        account_to_email=account_to_email,
        account_to_name=account_to_name,
        email_to_name=email_to_name,
        group_to_pl_values=dict(group_to_pl_values),
    )


def resolve_pl_email(pl_value: str, account_book: AccountBook) -> str | None:
    """PL 可以是邮箱、gitcode、中文名或“中文名(gitcode)”。"""
    text = normalize_cell(pl_value)
    if is_invalid_value(text):
        return None
    if is_email(text):
        return text

    account, name = parse_account_value(text)
    if account:
        email = account_book.account_to_email.get(account.lower())
        if email:
            return email
    if name:
        email = account_book.name_to_email.get(name)
        if email:
            return email
    return account_book.name_to_email.get(text) or account_book.account_to_email.get(
        text.lower()
    )


def resolve_pl_name(pl_value: str, account_book: AccountBook) -> str:
    """把 PL 字段解析成中文名，找不到映射时保留原值。"""
    text = normalize_cell(pl_value)
    if is_invalid_value(text):
        return "-"

    if is_email(text):
        return account_book.email_to_name.get(text.lower(), text)

    account, name = parse_account_value(text)
    if name:
        return name
    if account:
        return account_book.account_to_name.get(account.lower(), account)

    return (
        account_book.account_to_name.get(text.lower())
        or account_book.email_to_name.get(text.lower())
        or text
    )


def resource_group_pl_names(resource_group: str, account_book: AccountBook) -> str:
    """获取资源组对应 PL 中文名；其他组没有唯一 PL，显示为 -。"""
    if resource_group == OTHER_RESOURCE_GROUP:
        return "-"

    names: list[str] = []
    seen: set[str] = set()
    for pl_value in account_book.group_to_pl_values.get(resource_group, []):
        add_unique(names, seen, resolve_pl_name(pl_value, account_book))
    return ",".join(names) if names else "-"


def collect_group_stats(source_excel: Path) -> dict[str, GroupStats]:
    """读取第一个 sheet，按资源组统计 open issue。"""
    stats: dict[str, GroupStats] = defaultdict(GroupStats)
    workbook = load_workbook(source_excel, read_only=True, data_only=True)
    try:
        sheet = workbook.worksheets[0]
        rows = sheet.iter_rows(values_only=True)
        try:
            header = [normalize_cell(cell) for cell in next(rows)]
        except StopIteration:
            return {}

        validate_columns(header)
        indexes = header_indexes(header)

        for row in rows:
            if not any(not is_invalid_value(cell) for cell in row):
                continue
            if not is_open_status(row_value(row, indexes, STATUS_COLUMN)):
                continue

            group_name = resource_group_bucket(
                row_value(row, indexes, RESOURCE_GROUP_COLUMN)
            )
            group_stats = stats[group_name]
            group_stats.issue_count += 1

            owner = row_value(row, indexes, OWNER_COLUMN)
            if is_unassigned_owner(owner):
                group_stats.unassigned_count += 1
            elif is_unreplied_issue(
                row_value(row, indexes, LAST_REPLY_TIME_COLUMN),
                row_value(row, indexes, PROPOSER_COLUMN),
                owner,
            ):
                group_stats.unreplied_count += 1

            if is_overdue_issue(row_value(row, indexes, OPEN_DURATION_COLUMN)):
                group_stats.overdue_unclosed_count += 1

            if normalize_cell(row_value(row, indexes, OPERATOR_TEST_COLUMN)) == "是":
                group_stats.operator_test_count += 1
    finally:
        workbook.close()

    return dict(stats)


def total_stats(group_stats: dict[str, GroupStats]) -> GroupStats:
    """汇总所有资源组的统计结果。"""
    total = GroupStats()
    for stats in group_stats.values():
        total.issue_count += stats.issue_count
        total.unreplied_count += stats.unreplied_count
        total.overdue_unclosed_count += stats.overdue_unclosed_count
        total.operator_test_count += stats.operator_test_count
        total.unassigned_count += stats.unassigned_count
    return total


def build_summary_lines(
    source_excel: Path,
    group_stats: dict[str, GroupStats],
) -> tuple[str, str]:
    """生成顶部两行汇总，统计范围是整个 Excel 的 open issue。"""
    stats = total_stats(group_stats)
    return (
        f"生成时间: {generated_time_from_excel_name(source_excel)}",
        (
            f"Issues 总数: {stats.issue_count}，其中超期未关闭"
            f"{stats.overdue_unclosed_count}个，算子内测{stats.operator_test_count}个，"
            f"未回复{stats.unreplied_count}个，未分配{stats.unassigned_count}个"
        ),
    )


def stats_rows(group_stats: dict[str, GroupStats]) -> list[tuple[str, GroupStats]]:
    """返回已排序的资源组统计行。"""
    return sorted(group_stats.items(), key=group_sort_key)


def html_cell(value: object, header: bool = False) -> str:
    """生成 HTML 单元格。"""
    tag = "th" if header else "td"
    text = html.escape(normalize_cell(value))
    style = (
        "border:1px solid #d0d7de;padding:6px 8px;text-align:left;"
        "vertical-align:top;line-height:18px;mso-line-height-rule:exactly;"
        "white-space:nowrap;"
    )
    if header:
        style += "background:#f6f8fa;font-weight:600;"
    return f"<{tag} style=\"{style}\">{text}</{tag}>"


def build_summary_table_html(
    group_stats: dict[str, GroupStats],
    account_book: AccountBook,
) -> str:
    """生成邮件正文里的资源组汇总表。"""
    rows = [
        "<tr>"
        + "".join(html_cell(header, header=True) for header in SUMMARY_HEADERS)
        + "</tr>"
    ]
    for resource_group, stats in stats_rows(group_stats):
        rows.append(
            "<tr>"
            + html_cell(resource_group)
            + html_cell(resource_group_pl_names(resource_group, account_book))
            + html_cell(str(stats.issue_count))
            + html_cell(str(stats.unreplied_count))
            + html_cell(str(stats.overdue_unclosed_count))
            + html_cell(str(stats.operator_test_count))
            + "</tr>"
        )

    return (
        '<table cellspacing="0" cellpadding="0" '
        'style="border-collapse:collapse;border-spacing:0;'
        'font-family:Arial,Microsoft YaHei,sans-serif;font-size:13px;">'
        + "".join(rows)
        + "</table>"
    )


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


def safe_sheet_title(value: str, used_titles: set[str]) -> str:
    """生成合法且不重复的 Excel sheet 名。"""
    title = re.sub(r"[\[\]:*?/\\]", "_", normalize_cell(value)) or "未命名"
    title = title[:31]
    if title not in used_titles:
        used_titles.add(title)
        return title

    base = title[:28]
    index = 1
    while True:
        candidate = f"{base}_{index}"
        if candidate not in used_titles:
            used_titles.add(candidate)
            return candidate
        index += 1


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


def should_highlight_issue(
    open_duration: object,
    last_reply_time: object,
    proposer: object,
    owner: object,
) -> bool:
    """判断明细行是否需要橙色底色。"""
    if is_overdue_issue(open_duration):
        return True
    if is_unassigned_owner(owner):
        return False
    return is_unreplied_issue(last_reply_time, proposer, owner)


def build_summary_attachment(
    source_excel: Path,
    output_dir: Path,
    group_stats: dict[str, GroupStats],
) -> Path:
    """生成汇总邮件附件：一个 Excel，每个资源组一个明细 sheet。"""
    output_dir.mkdir(parents=True, exist_ok=True)
    attachment_path = output_dir / ATTACHMENT_FILE

    source_workbook = load_workbook(source_excel)
    target_workbook = Workbook()
    target_workbook.remove(target_workbook.active)
    used_titles: set[str] = set()

    try:
        source_sheet = source_workbook.worksheets[0]
        header = [normalize_cell(cell.value) for cell in source_sheet[1]]
        validate_columns(header)
        indexes = header_indexes(header)
        keep_columns = [
            index
            for index, column_name in enumerate(header, start=1)
            if column_name not in DROP_COLUMNS
        ]

        for resource_group, _ in stats_rows(group_stats):
            matched_rows: list[tuple[int, bool]] = []
            for row_index in range(2, source_sheet.max_row + 1):
                if not is_open_status(
                    sheet_value(source_sheet, row_index, indexes, STATUS_COLUMN)
                ):
                    continue
                if not is_row_in_resource_group_bucket(
                    sheet_value(source_sheet, row_index, indexes, RESOURCE_GROUP_COLUMN),
                    resource_group,
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
                continue

            target_sheet = target_workbook.create_sheet(
                safe_sheet_title(resource_group, used_titles)
            )
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
                target_cell = target_sheet.cell(row=1, column=target_column)
                copy_excel_cell(
                    source_sheet.cell(row=1, column=source_column),
                    target_cell,
                    keep_hyperlink=(target_column == 1),
                )
                if is_title_column:
                    enable_wrap_text(target_cell)

            for target_row, (source_row, needs_highlight) in enumerate(
                matched_rows,
                start=2,
            ):
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

    if not target_workbook.worksheets:
        sheet = target_workbook.create_sheet("无待跟进issue")
        sheet.append(("无待跟进 open issue",))

    target_workbook.save(attachment_path)
    target_workbook.close()
    return attachment_path


def collect_pl_emails(
    group_stats: dict[str, GroupStats],
    account_book: AccountBook,
) -> list[str]:
    """根据本次统计到的资源组，从账号表里追加对应组件 PL 邮箱。"""
    emails: list[str] = []
    seen: set[str] = set()
    for resource_group in group_stats:
        if resource_group == OTHER_RESOURCE_GROUP:
            continue
        for pl_value in account_book.group_to_pl_values.get(resource_group, []):
            email = resolve_pl_email(pl_value, account_book)
            if email:
                add_unique(emails, seen, email)
    return emails


def resolve_recipients(
    group_stats: dict[str, GroupStats],
    account_book: AccountBook,
) -> tuple[list[str], list[str], list[str]]:
    """TEST_TO 有值时发测试邮箱；正式发送时发固定收件人和组件 PL。"""
    real_to_addrs: list[str] = []
    real_to_seen: set[str] = set()
    for email in normalize_email_list(FIXED_TO_EMAILS) + collect_pl_emails(
        group_stats,
        account_book,
    ):
        add_unique(real_to_addrs, real_to_seen, email)

    test_to_addrs = normalize_email_list(TEST_TO)
    if test_to_addrs:
        return test_to_addrs, real_to_addrs, test_to_addrs
    return real_to_addrs, real_to_addrs, test_to_addrs


def build_message(
    to_addrs: list[str],
    attachment_path: Path,
    summary_lines: tuple[str, str],
    table_html: str,
) -> EmailMessage:
    """构造汇总邮件。"""
    message = EmailMessage()
    message["From"] = SENDER_EMAIL
    message["To"] = ", ".join(to_addrs)
    message["Subject"] = SUBJECT

    text_body = "\n".join(summary_lines) + "\n\n"
    summary_html = build_summary_html(summary_lines)
    html_body = f"""\
<html>
  <body>
    <div style="margin-bottom:12px;">{summary_html}</div>
    {table_html}
  </body>
</html>
"""
    message.set_content(text_body)
    message.add_alternative(html_body, subtype="html")
    message.add_attachment(
        attachment_path.read_bytes(),
        maintype="application",
        subtype="vnd.openxmlformats-officedocument.spreadsheetml.sheet",
        filename=attachment_path.name,
    )
    return message


def send_message(message: EmailMessage, to_addrs: list[str]) -> None:
    """直接连接 SMTP 并发送邮件。"""
    try:
        smtp = smtplib.SMTP(SMTP_HOST)
        smtp_user, smtp_password = smtp_credentials()
        smtp.login(smtp_user, smtp_password)
        smtp.sendmail(SENDER_EMAIL, to_addrs, message.as_string())
        smtp.quit()
        print("Email send success")
    except smtplib.SMTPException as exc:
        print(f"Error: unable to send email:{exc}")
        raise


def write_send_report(
    script_dir: Path,
    actual_to_addrs: list[str],
    real_to_addrs: list[str],
    test_to_addrs: list[str],
    summary_lines: tuple[str, str],
    group_stats: dict[str, GroupStats],
    account_book: AccountBook,
    attachment_path: Path,
) -> None:
    """保留调试信息，便于核对邮件发送内容。"""
    report = {
        "actual_to_addrs": actual_to_addrs,
        "real_to_addrs": real_to_addrs,
        "test_to_addrs": test_to_addrs,
        "summary_lines": summary_lines,
        "groups": [
            {
                RESOURCE_GROUP_COLUMN: resource_group,
                PL_COLUMN: resource_group_pl_names(resource_group, account_book),
                "issue数量": stats.issue_count,
                "未回复": stats.unreplied_count,
                "超期未关闭": stats.overdue_unclosed_count,
                "未分配": stats.unassigned_count,
                OPERATOR_TEST_COLUMN: stats.operator_test_count,
            }
            for resource_group, stats in stats_rows(group_stats)
        ],
        "attachment": str(attachment_path),
    }
    report_path = script_dir / SEND_REPORT_FILE
    report_path.write_text(
        json.dumps(report, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )
    print(f"已写入 {report_path}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "发送 issue 资源组汇总邮件：顶部统计整个 Excel 的 open issue，"
            "正文和附件按资源组汇总；非目标资源组统一统计为其他组。"
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

    keep_intermediate = keep_intermediate_files()
    try:
        account_book = read_account_md(account_md)
        group_stats = collect_group_stats(excel_path)
        summary_lines = build_summary_lines(excel_path, group_stats)
        attachment_path = build_summary_attachment(
            excel_path,
            script_dir / ATTACHMENT_DIR,
            group_stats,
        )
        table_html = build_summary_table_html(group_stats, account_book)
        actual_to_addrs, real_to_addrs, test_to_addrs = resolve_recipients(
            group_stats,
            account_book,
        )
        if not actual_to_addrs:
            print("无有效收件人，跳过发送", file=sys.stderr)
            return 1

        message = build_message(
            actual_to_addrs,
            attachment_path,
            summary_lines,
            table_html,
        )
        send_message(message, actual_to_addrs)
        print(f"发送汇总邮件收件人: {', '.join(actual_to_addrs)}")
        print(f"正式收件人: {', '.join(real_to_addrs)}")
        print(f"资源组统计行数: {len(group_stats)}")

        if keep_intermediate:
            write_send_report(
                script_dir,
                actual_to_addrs,
                real_to_addrs,
                test_to_addrs,
                summary_lines,
                group_stats,
                account_book,
                attachment_path,
            )
            print(f"已生成附件目录 {script_dir / ATTACHMENT_DIR}")
    finally:
        if not keep_intermediate:
            cleanup_intermediate_files(script_dir)
            print("未保留邮件附件和发送报告")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
