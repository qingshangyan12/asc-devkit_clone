#!/usr/bin/env python3
"""按资源组发送 issue 邮件。"""

from __future__ import annotations

import argparse
import json
import re
import smtplib
import ssl
import sys
import unicodedata
from collections import defaultdict
from email.message import EmailMessage
from email.utils import formataddr
from pathlib import Path

from openpyxl import load_workbook


ACCOUNT_MD_FILE = "gitcode_accounts_new.md"
PREVIEW_FILE = "resource_group_email_preview.json"

SENDER_NAME = "王旭"
SENDER_EMAIL = "305055963@qq.com"
SMTP_HOST = "smtp.qq.com"
SMTP_PORT = 465
SMTP_PASSWORD_ENV = "QQ_SMTP_PASSWORD"

TEST_TO = "wangxu243@h-partners.com"
DRY_RUN = True
SEND_CC_IN_TEST = True

OWNER_COLUMN = "责任人"
RESOURCE_GROUP_COLUMN = "资源组"
GITCODE_COLUMN = "gitcode"
NAME_COLUMN = "姓名"
EMAIL_COLUMN = "邮箱"
PL_COLUMN = "PL"

INVALID_VALUES = {
    "",
    "-",
    "nan",
    "none",
    "null",
    "未分配",
    "无",
}

EMAIL_RE = re.compile(r"^[^@\s]+@[^@\s]+\.[^@\s]+$")
FORMATTED_ACCOUNT_RE = re.compile(r"^(.+?)\(([^()]+)\)$")
INVALID_ACCOUNT_CHARS_RE = re.compile(r"[\s@|]")


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


def add_unique(items: list[str], seen: set[str], value: object) -> None:
    """按小写值去重，同时保留首次出现时的原始写法。"""
    text = normalize_cell(value)
    if is_invalid_value(text):
        return
    key = text.lower()
    if key not in seen:
        seen.add(key)
        items.append(text)


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


def pl_email(pl_value: object, account_info: dict[str, dict[str, str]], name_to_account: dict[str, str]) -> str | None:
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
        for sheet in workbook.worksheets:
            rows = sheet.iter_rows(values_only=True)
            try:
                header = [normalize_cell(cell) for cell in next(rows)]
            except StopIteration:
                continue
            if OWNER_COLUMN not in header or RESOURCE_GROUP_COLUMN not in header:
                continue
            owner_index = header.index(OWNER_COLUMN)
            resource_index = header.index(RESOURCE_GROUP_COLUMN)

            for row in rows:
                if owner_index >= len(row) or resource_index >= len(row):
                    continue
                resource_group = normalize_cell(row[resource_index])
                if is_invalid_value(resource_group):
                    continue

                owner_account = resolve_account(row[owner_index], name_to_account)
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


def build_message(resource_group: str, to_addrs: list[str], cc_addrs: list[str]) -> EmailMessage:
    """构造邮件；正文先留空。"""
    message = EmailMessage()
    message["From"] = formataddr((SENDER_NAME, SENDER_EMAIL))
    message["To"] = ", ".join(to_addrs)
    if cc_addrs:
        message["Cc"] = ", ".join(cc_addrs)
    message["Subject"] = f"{resource_group} issue 跟进"
    message.set_content("")
    return message


def send_messages(groups: dict[str, dict[str, list[str]]]) -> list[dict[str, object]]:
    """按资源组发送邮件；默认 DRY_RUN 只生成预览。"""
    previews: list[dict[str, object]] = []
    smtp_password = ""
    if not DRY_RUN:
        import os

        smtp_password = os.environ.get(SMTP_PASSWORD_ENV, "")
        if not smtp_password:
            raise RuntimeError(f"请先设置环境变量 {SMTP_PASSWORD_ENV}")

    smtp = None
    try:
        if not DRY_RUN:
            smtp = smtplib.SMTP_SSL(
                SMTP_HOST,
                SMTP_PORT,
                context=ssl.create_default_context(),
            )
            smtp.login(SENDER_EMAIL, smtp_password)

        for resource_group in sorted(groups):
            group = groups[resource_group]
            real_to_addrs = group["owner_emails"]

            # 正式发送时使用责任人邮箱：
            # to_addrs = real_to_addrs
            to_addrs = [TEST_TO]

            cc_addrs = group["pl_emails"] if SEND_CC_IN_TEST else []
            message = build_message(resource_group, to_addrs, cc_addrs)
            all_recipients = to_addrs + cc_addrs

            preview = {
                "resource_group": resource_group,
                "real_to_addrs": real_to_addrs,
                "test_to_addrs": to_addrs,
                "cc_addrs": cc_addrs,
                "owner_accounts": group["owner_accounts"],
                "pl_values": group["pl_values"],
                "sent": not DRY_RUN,
            }
            previews.append(preview)

            if smtp:
                smtp.send_message(message, to_addrs=all_recipients)
    finally:
        if smtp:
            smtp.quit()

    return previews


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "按资源组发送邮件：责任人作为收件人，PL 作为抄送；"
            "默认测试模式只生成预览，实际收件人使用 wangxu243@h-partners.com。"
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
    previews = send_messages(groups)

    preview_path = script_dir / PREVIEW_FILE
    preview_path.write_text(
        json.dumps(previews, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )

    mode = "预览" if DRY_RUN else "发送"
    print(f"{mode}资源组邮件数: {len(previews)}")
    print(f"已写入 {preview_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
