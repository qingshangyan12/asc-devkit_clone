#!/usr/bin/env python3
"""提取待补充信息的 GitCode 账号，并查询账号详情。"""

from __future__ import annotations

import argparse
import json
import re
import ssl
import sys
import unicodedata
from pathlib import Path
from typing import Any
from urllib.parse import quote
from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen

from openpyxl import load_workbook


# 固定配置：后续只需要替换接口地址，命令行只保留 Excel 文件参数。
FETCH_URL_TEMPLATE = "https://xxxx.com/gitcode={str}&size=20&page=1"
OUTPUT_FILE = "gitcode_account_infos.json"
GROUP_SIZE = 20
REQUEST_TIMEOUT = 30.0
SSL_VERIFY = False
ISSUE_COLUMNS = ("提出者", "责任人")
ACCOUNT_ID_COLUMN = "gitcode_id"
EMAIL_COLUMN = "邮箱"
NAME_COLUMN = "姓名"
EMAIL_DOMAINS = ("huawei.com", "hisilicon.com", "h-partners.com")

INVALID_VALUES = {
    "",
    "-",
    "nan",
    "none",
    "null",
    "未分配",
    "无",
}

INVALID_ACCOUNT_CHARS_RE = re.compile(r"[\s@|]")


def normalize_cell(value: object) -> str:
    """统一清洗 Excel/Markdown 单元格内容，兼容全角空格等字符。"""
    if value is None:
        return ""
    text = unicodedata.normalize("NFKC", str(value))
    return text.strip()


def is_gitcode_id(value: str) -> bool:
    """过滤空值、占位值，以及明显不是 gitcode 账号的内容。"""
    text = normalize_cell(value)
    if text.lower() in INVALID_VALUES:
        return False
    return not INVALID_ACCOUNT_CHARS_RE.search(text)


def build_email_regex(domains: list[str]) -> re.Pattern[str]:
    """邮箱必须符合：姓名拼音 + 可选数字 + 指定企业域名。"""
    domain_pattern = "|".join(re.escape(domain.lower()) for domain in domains)
    return re.compile(rf"^[A-Za-z]+[0-9]*@(?:{domain_pattern})$", re.IGNORECASE)


def has_email(value: str, email_regex: re.Pattern[str]) -> bool:
    """判断 Markdown 邮箱列是否已有符合规则的邮箱。"""
    text = normalize_cell(value)
    if text.lower() in INVALID_VALUES:
        return False
    return bool(email_regex.fullmatch(text))


def has_chinese(value: str) -> bool:
    """判断账号是否包含中文字符。"""
    return any("\u4e00" <= char <= "\u9fff" for char in value)


def add_unique(items: list[str], seen: set[str], value: str) -> None:
    """按账号小写值去重，同时保留首次出现时的原始大小写。"""
    account = normalize_cell(value)
    if not is_gitcode_id(account):
        return
    key = account.lower()
    if key not in seen:
        seen.add(key)
        items.append(account)


def split_markdown_row(line: str) -> list[str]:
    """按 Markdown 表格分隔符拆分一行。"""
    return [normalize_cell(part) for part in line.strip().strip("|").split("|")]


def is_markdown_separator(row: list[str]) -> bool:
    """识别 Markdown 表头下方的分隔行。"""
    return bool(row) and all(re.fullmatch(r":?-{3,}:?", cell) for cell in row)


def read_account_md(
    path: Path, email_regex: re.Pattern[str]
) -> tuple[list[str], set[str], set[str]]:
    """读取 Markdown 表格，返回账号、已有邮箱账号、已登记姓名。"""
    account_ids: list[str] = []
    seen: set[str] = set()
    accounts_with_email: set[str] = set()
    names: set[str] = set()
    headers: list[str] | None = None
    id_index = -1
    email_index = -1
    name_index = -1

    with path.open("r", encoding="utf-8") as file:
        for line in file:
            if not line.lstrip().startswith("|"):
                continue
            row = split_markdown_row(line)
            if is_markdown_separator(row):
                continue
            if headers is None:
                headers = row
                try:
                    id_index = headers.index(ACCOUNT_ID_COLUMN)
                except ValueError as exc:
                    raise ValueError(f"{path} 缺少列: {ACCOUNT_ID_COLUMN}") from exc
                try:
                    email_index = headers.index(EMAIL_COLUMN)
                except ValueError:
                    email_index = -1
                try:
                    name_index = headers.index(NAME_COLUMN)
                except ValueError:
                    name_index = -1
                continue

            if id_index >= len(row):
                continue
            account = normalize_cell(row[id_index])
            add_unique(account_ids, seen, account)
            if name_index >= 0 and name_index < len(row):
                name = normalize_cell(row[name_index])
                if name.lower() not in INVALID_VALUES:
                    names.add(name)
            if (
                email_index >= 0
                and email_index < len(row)
                and has_email(row[email_index], email_regex)
            ):
                accounts_with_email.add(account.lower())

    return account_ids, accounts_with_email, names


def read_issue_xlsx(path: Path) -> list[str]:
    """从 Excel 所有 sheet 的“提出者”和“责任人”列提取账号。"""
    account_ids: list[str] = []
    seen: set[str] = set()
    workbook = load_workbook(path, read_only=True, data_only=True)
    try:
        for sheet in workbook.worksheets:
            rows = sheet.iter_rows(values_only=True)
            try:
                header = [normalize_cell(cell) for cell in next(rows)]
            except StopIteration:
                continue

            column_indexes = [
                header.index(column) for column in ISSUE_COLUMNS if column in header
            ]
            if not column_indexes:
                continue

            for row in rows:
                for index in column_indexes:
                    if index < len(row):
                        add_unique(account_ids, seen, normalize_cell(row[index]))
    finally:
        workbook.close()
    return account_ids


def grouped_account_strings(accounts: list[str], group_size: int) -> list[str]:
    """每 group_size 个账号组成一个逗号分隔字符串。"""
    return [
        ",".join(accounts[index : index + group_size])
        for index in range(0, len(accounts), group_size)
    ]


def fetch_results(
    grouped_accounts: list[str], url_template: str, group_size: int, timeout: float
) -> list[Any]:
    """逐组发 GET 请求，取返回字典中的 results 列表并合并。"""
    merged_results: list[Any] = []
    ssl_context = None if SSL_VERIFY else ssl._create_unverified_context()
    for page, account_string in enumerate(grouped_accounts, start=1):
        url = (
            url_template.replace("{str}", quote(account_string, safe=","))
            .replace("{size}", str(group_size))
            .replace("{page}", str(page))
        )
        request = Request(
            url,
            headers={
                "Accept": "application/json",
                "User-Agent": "Mozilla/5.0",
            },
        )
        try:
            with urlopen(request, timeout=timeout, context=ssl_context) as response:
                payload = json.loads(response.read().decode("utf-8"))
        except HTTPError as exc:
            body = exc.read().decode("utf-8", errors="replace")
            raise RuntimeError(
                f"第 {page} 组请求失败，HTTP {exc.code}: {url}\n返回内容: {body}"
            ) from exc
        except URLError as exc:
            raise RuntimeError(
                f"第 {page} 组请求失败，URL: {url}\n失败原因: {exc.reason}"
            ) from exc
        if not isinstance(payload, dict):
            raise ValueError(f"接口返回不是字典: {url}")
        results = payload.get("results")
        if not isinstance(results, list):
            raise ValueError(f"接口返回缺少列表字段 results: {url}")
        merged_results.extend(results)
    return merged_results


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "从 Excel 的“提出者/责任人”和同级 gitcode_accounts.md 中提取账号，"
            "过滤已有信息账号后，查询接口并输出账号信息 JSON 列表。"
        )
    )
    parser.add_argument("excel", help="issue Excel 文件")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    script_dir = Path(__file__).resolve().parent
    email_regex = build_email_regex(list(EMAIL_DOMAINS))

    issue_file = Path(args.excel)
    if not issue_file.is_file():
        print(f"Excel 文件不存在: {issue_file}", file=sys.stderr)
        return 1

    account_md = script_dir / "gitcode_accounts.md"
    if not account_md.is_file():
        print(f"账号文件不存在: {account_md}", file=sys.stderr)
        return 1

    all_accounts: list[str] = []
    seen: set[str] = set()

    for account in read_issue_xlsx(issue_file):
        add_unique(all_accounts, seen, account)

    md_accounts, md_accounts_with_email, md_names = read_account_md(
        account_md, email_regex
    )
    for account in md_accounts:
        add_unique(all_accounts, seen, account)

    # 剔除已有邮箱账号；中文账号额外按 Markdown 的“姓名”列判断是否已存在。
    result = [
        account
        for account in all_accounts
        if account.lower() not in md_accounts_with_email
        and not (has_chinese(account) and account in md_names)
    ]
    merged_results = fetch_results(
        grouped_account_strings(result, GROUP_SIZE),
        FETCH_URL_TEMPLATE,
        GROUP_SIZE,
        REQUEST_TIMEOUT,
    )
    output_path = script_dir / OUTPUT_FILE
    output_path.write_text(
        json.dumps(merged_results, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )
    print(f"已写入 {output_path}，共 {len(merged_results)} 条账号信息")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
