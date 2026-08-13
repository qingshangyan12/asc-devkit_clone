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
FETCH_URL_TEMPLATE = "https://xxxx.com/gitcode={str}&size={size}&page=1"
OUTPUT_FILE = "gitcode_account_infos.json"
MD_FILE = "gitcode_accounts.md"
GROUP_SIZE = 20
REQUEST_TIMEOUT = 30.0
SSL_VERIFY = False
ISSUE_COLUMNS = ("提出者", "责任人")
ACCOUNT_ID_COLUMN = "gitcode_id"
EMAIL_COLUMN = "邮箱"
ROLE_COLUMN = "角色"
NAME_COLUMN = "姓名"
RESOURCE_GROUP_COLUMN = "资源组"
MD_HEADERS = (
    ACCOUNT_ID_COLUMN,
    EMAIL_COLUMN,
    ROLE_COLUMN,
    NAME_COLUMN,
    RESOURCE_GROUP_COLUMN,
)
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


def is_invalid_value(value: str) -> bool:
    """判断是否为空值或占位值。"""
    return normalize_cell(value).lower() in INVALID_VALUES


def normalize_output_value(value: object) -> str:
    """写入 Markdown 前清洗字段，空值统一写成 -。"""
    text = normalize_cell(value).replace("|", "/")
    return "-" if is_invalid_value(text) else text


def is_gitcode_id(value: str) -> bool:
    """过滤空值、占位值，以及明显不是 gitcode 账号的内容。"""
    text = normalize_cell(value)
    if is_invalid_value(text):
        return False
    return not INVALID_ACCOUNT_CHARS_RE.search(text)


def build_email_regex(domains: list[str]) -> re.Pattern[str]:
    """邮箱必须符合：姓名拼音 + 可选数字 + 指定企业域名。"""
    domain_pattern = "|".join(re.escape(domain.lower()) for domain in domains)
    return re.compile(rf"^[A-Za-z]+[0-9]*@(?:{domain_pattern})$", re.IGNORECASE)


def has_email(value: str, email_regex: re.Pattern[str]) -> bool:
    """判断 Markdown 邮箱列是否已有符合规则的邮箱。"""
    text = normalize_cell(value)
    if is_invalid_value(text):
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
                if not is_invalid_value(name):
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


def read_markdown_rows(path: Path) -> list[dict[str, str]]:
    """读取现有 Markdown 表格，返回按表头组织的行数据。"""
    rows: list[dict[str, str]] = []
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
                continue
            rows.append(
                {
                    header: normalize_output_value(row[index])
                    for index, header in enumerate(headers)
                    if index < len(row)
                }
            )
    return rows


def account_info_to_md_row(info: dict[str, Any]) -> dict[str, str]:
    """按接口字段映射生成 Markdown 行。"""
    unaffiliated_detail = info.get("unaffiliated_detail")
    if not isinstance(unaffiliated_detail, dict):
        unaffiliated_detail = {}
    return {
        ACCOUNT_ID_COLUMN: normalize_output_value(info.get("gitcode_account")),
        EMAIL_COLUMN: normalize_output_value(info.get("email")),
        ROLE_COLUMN: "-",
        NAME_COLUMN: normalize_output_value(info.get("chinese_name")),
        RESOURCE_GROUP_COLUMN: normalize_output_value(
            unaffiliated_detail.get("lowest_department")
        ),
    }


def markdown_line(values: list[str]) -> str:
    """生成一行 Markdown 表格。"""
    return "| " + " | ".join(values) + " |"


def write_account_md(path: Path, rows: list[dict[str, str]]) -> None:
    """把账号信息写回 Markdown 表格。"""
    lines = [
        markdown_line(list(MD_HEADERS)),
        markdown_line([":---"] * len(MD_HEADERS)),
    ]
    for row in rows:
        lines.append(markdown_line([row.get(header, "-") for header in MD_HEADERS]))
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def account_sort_key(row: dict[str, str]) -> tuple[int, str, str]:
    """按资源组排序；无资源组的账号放在最后。"""
    resource_group = normalize_cell(row.get(RESOURCE_GROUP_COLUMN))
    account = normalize_cell(row.get(ACCOUNT_ID_COLUMN)).lower()
    is_empty_group = is_invalid_value(resource_group)
    return (1 if is_empty_group else 0, resource_group, account)


def sort_account_rows(rows: list[dict[str, str]]) -> list[dict[str, str]]:
    """按资源组排序；同一资源组内按 gitcode_id 排序。"""
    return sorted(rows, key=account_sort_key)


def update_account_md(path: Path, account_infos: list[Any]) -> tuple[int, int]:
    """用接口返回的账号信息更新 Markdown，返回新增和更新数量。"""
    rows = read_markdown_rows(path)
    account_to_index: dict[str, int] = {}

    # 先建立现有 gitcode_id 到行号的索引，用于判断是更新还是追加。
    for index, row in enumerate(rows):
        account = normalize_cell(row.get(ACCOUNT_ID_COLUMN))
        if is_gitcode_id(account) and not has_chinese(account):
            account_to_index.setdefault(account.lower(), index)

    added_count = 0
    updated_count = 0

    # 先按接口返回结果更新 Markdown 数据；不在 Markdown 中的账号追加到末尾。
    for info in account_infos:
        if not isinstance(info, dict):
            continue
        new_row = account_info_to_md_row(info)
        account = new_row[ACCOUNT_ID_COLUMN]
        if not is_gitcode_id(account) or has_chinese(account):
            continue
        key = account.lower()
        if key in account_to_index:
            rows[account_to_index[key]].update(new_row)
            updated_count += 1
        else:
            account_to_index[key] = len(rows)
            rows.append(new_row)
            added_count += 1

    # 数据更新完成后再统一排序，保证同一资源组的账号聚在一起。
    rows = sort_account_rows(rows)
    write_account_md(path, rows)
    return added_count, updated_count


def missing_field_report(account_infos: list[Any], limit: int = 10) -> dict[str, list[str]]:
    """统计接口结果中缺少关键字段的账号，便于定位数据问题。"""
    report = {
        "missing_email": [],
        "missing_chinese_name": [],
        "missing_resource_group": [],
    }
    for info in account_infos:
        if not isinstance(info, dict):
            continue
        account = normalize_output_value(info.get("gitcode_account"))
        unaffiliated_detail = info.get("unaffiliated_detail")
        if not isinstance(unaffiliated_detail, dict):
            unaffiliated_detail = {}
        if is_invalid_value(normalize_cell(info.get("email"))):
            report["missing_email"].append(account)
        if is_invalid_value(normalize_cell(info.get("chinese_name"))):
            report["missing_chinese_name"].append(account)
        if is_invalid_value(
            normalize_cell(unaffiliated_detail.get("lowest_department"))
        ):
            report["missing_resource_group"].append(account)
    return {key: values[:limit] for key, values in report.items()}


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
            "过滤已有信息账号后，查询接口并更新 Markdown。"
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

    account_md = script_dir / MD_FILE
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

    # 每 20 个账号一组请求接口，接口返回的 results 会被合并成一个列表。
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
    added_count, updated_count = update_account_md(account_md, merged_results)
    report = missing_field_report(merged_results)
    print(f"接口返回 {len(merged_results)} 条账号信息")
    print(f"已写入接口原始结果 {output_path}")
    print(f"已更新 {account_md}，新增 {added_count} 个账号，更新 {updated_count} 个账号")
    print(f"字段缺失样例: {json.dumps(report, ensure_ascii=False)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
