#!/usr/bin/env python3
"""从 Excel 和 Markdown 提取英文 GitCode 账号，并用接口信息生成新 Markdown。"""

from __future__ import annotations

import argparse
import json
import re
import ssl
import sys
import unicodedata
from pathlib import Path
from typing import Any
from urllib.error import HTTPError, URLError
from urllib.parse import quote
from urllib.request import Request, urlopen

from openpyxl import load_workbook


FETCH_URL_TEMPLATE = "https://xxxx.com/gitcode={str}&size={size}&page=1"
OUTPUT_FILE = "merged_gitcode_accounts.json"
MD_FILE = "gitcode_accounts.md"
OUTPUT_MD_FILE = "gitcode_accounts_new.md"
GROUP_SIZE = 20
REQUEST_TIMEOUT = 30.0
SSL_VERIFY = False
ISSUE_COLUMNS = ("提出者", "责任人")
ACCOUNT_ID_COLUMN = "gitcode_id"
OUTPUT_ACCOUNT_COLUMN = "gitcode"
EMAIL_COLUMN = "邮箱"
NAME_COLUMN = "姓名"
RESOURCE_GROUP_COLUMN = "资源组"
PL_COLUMN = "PL"
OUTPUT_MD_HEADERS = (
    OUTPUT_ACCOUNT_COLUMN,
    NAME_COLUMN,
    EMAIL_COLUMN,
    RESOURCE_GROUP_COLUMN,
    PL_COLUMN,
)

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
    return unicodedata.normalize("NFKC", str(value)).strip()


def normalize_output_value(value: object) -> str:
    """写入 Markdown 前清洗字段，空值统一写成 -。"""
    text = normalize_cell(value).replace("|", "/")
    return "-" if is_invalid_value(text) else text


def is_invalid_value(value: str) -> bool:
    """判断是否为空值或占位值。"""
    return normalize_cell(value).lower() in INVALID_VALUES


def has_chinese(value: str) -> bool:
    """判断账号是否包含中文字符。"""
    return any("\u4e00" <= char <= "\u9fff" for char in value)


def is_gitcode_id(value: str) -> bool:
    """过滤空值、占位值，以及明显不是 gitcode 账号的内容。"""
    text = normalize_cell(value)
    if is_invalid_value(text):
        return False
    return not INVALID_ACCOUNT_CHARS_RE.search(text)


def add_unique(items: list[str], seen: set[str], value: str) -> None:
    """按账号小写值去重，同时保留首次出现时的原始大小写。"""
    account = normalize_cell(value)
    if not is_gitcode_id(account) or has_chinese(account):
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


def read_account_md(path: Path) -> list[str]:
    """读取 Markdown 表格中的 gitcode_id。"""
    account_ids: list[str] = []
    headers: list[str] | None = None
    id_index = -1

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
                continue

            if id_index >= len(row):
                continue
            account = normalize_cell(row[id_index])
            if not is_gitcode_id(account) or has_chinese(account):
                continue
            account_ids.append(account)

    return account_ids


def get_resource_group(info: dict[str, Any]) -> object:
    """优先从 user_ship.projec 取资源组，取不到再用 unaffiliated_detail.lowest_department。"""
    user_ship = info.get("user_ship")
    if isinstance(user_ship, dict):
        resource_group = user_ship.get("projec")
        if is_invalid_value(normalize_cell(resource_group)):
            resource_group = user_ship.get("project")
        if not is_invalid_value(normalize_cell(resource_group)):
            return resource_group

    unaffiliated_detail = info.get("unaffiliated_detail")
    if isinstance(unaffiliated_detail, dict):
        return unaffiliated_detail.get("lowest_department")
    return None


def get_pl(info: dict[str, Any]) -> object:
    """优先从 user_ship.manager_chinese_name 取 PL，取不到再用 user_ship.manager。"""
    user_ship = info.get("user_ship")
    if isinstance(user_ship, dict):
        pl = user_ship.get("manager_chinese_name")
        if not is_invalid_value(normalize_cell(pl)):
            return pl
        return user_ship.get("manager")
    return None


def markdown_line(values: list[str]) -> str:
    """生成一行 Markdown 表格。"""
    return "| " + " | ".join(values) + " |"


def write_output_md(path: Path, rows: list[dict[str, str]]) -> None:
    """写入新的账号 Markdown 文件。"""
    rows = sorted(rows, key=output_row_sort_key)
    lines = [
        markdown_line(list(OUTPUT_MD_HEADERS)),
        markdown_line([":---"] * len(OUTPUT_MD_HEADERS)),
    ]
    for row in rows:
        lines.append(markdown_line([row.get(header, "-") for header in OUTPUT_MD_HEADERS]))
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def output_row_sort_key(row: dict[str, str]) -> tuple[int, str, str]:
    """新 Markdown 按资源组排序；无资源组的账号放在最后。"""
    resource_group = normalize_cell(row.get(RESOURCE_GROUP_COLUMN))
    account = normalize_cell(row.get(OUTPUT_ACCOUNT_COLUMN)).lower()
    is_empty_group = is_invalid_value(resource_group)
    return (1 if is_empty_group else 0, resource_group, account)


def merge_output_row(base: dict[str, str], incoming: dict[str, str]) -> None:
    """合并新 Markdown 行，新数据中有值的字段覆盖旧值。"""
    for header in OUTPUT_MD_HEADERS:
        value = normalize_output_value(incoming.get(header))
        if not is_invalid_value(value):
            base[header] = value


def account_info_to_output_row(info: dict[str, Any]) -> dict[str, str]:
    """按新 Markdown 表头生成行数据。"""
    return {
        OUTPUT_ACCOUNT_COLUMN: normalize_output_value(info.get("gitcode_account")),
        NAME_COLUMN: normalize_output_value(info.get("chinese_name")),
        EMAIL_COLUMN: normalize_output_value(info.get("email")),
        RESOURCE_GROUP_COLUMN: normalize_output_value(get_resource_group(info)),
        PL_COLUMN: normalize_output_value(get_pl(info)),
    }


def build_output_md_rows(account_infos: list[Any]) -> list[dict[str, str]]:
    """把接口结果转换为去重后的新 Markdown 行。"""
    rows: list[dict[str, str]] = []
    account_to_index: dict[str, int] = {}
    for info in account_infos:
        if not isinstance(info, dict):
            continue
        row = account_info_to_output_row(info)
        account = normalize_cell(row.get(OUTPUT_ACCOUNT_COLUMN))
        if not is_gitcode_id(account) or has_chinese(account):
            continue

        key = account.lower()
        if key in account_to_index:
            merge_output_row(rows[account_to_index[key]], row)
        else:
            account_to_index[key] = len(rows)
            rows.append(row)
    return rows


def missing_field_report(account_infos: list[Any], limit: int = 10) -> dict[str, list[str]]:
    """统计接口结果中缺少关键字段的账号，便于定位数据问题。"""
    report = {
        "missing_email": [],
        "missing_chinese_name": [],
        "missing_resource_group": [],
        "missing_pl": [],
    }
    for info in account_infos:
        if not isinstance(info, dict):
            continue
        account = normalize_output_value(info.get("gitcode_account"))
        if is_invalid_value(normalize_cell(info.get("email"))):
            report["missing_email"].append(account)
        if is_invalid_value(normalize_cell(info.get("chinese_name"))):
            report["missing_chinese_name"].append(account)
        if is_invalid_value(normalize_cell(get_resource_group(info))):
            report["missing_resource_group"].append(account)
        if is_invalid_value(normalize_cell(get_pl(info))):
            report["missing_pl"].append(account)
    return {key: values[:limit] for key, values in report.items()}


def read_issue_xlsx(path: Path) -> list[str]:
    """从 Excel 提取账号；中文值直接忽略。"""
    account_ids: list[str] = []
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
            for row in rows:
                for index in column_indexes:
                    if index >= len(row):
                        continue
                    value = normalize_cell(row[index])
                    if is_gitcode_id(value) and not has_chinese(value):
                        account_ids.append(value)
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
            "只从 Excel 的“提出者/责任人”和同级 gitcode_accounts.md 提取账号，"
            "Excel 中的中文值直接忽略，其他信息全部来自接口结果，并生成新的 Markdown。"
        )
    )
    parser.add_argument("excel", help="issue Excel 文件")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    script_dir = Path(__file__).resolve().parent

    issue_file = Path(args.excel)
    if not issue_file.is_file():
        print(f"Excel 文件不存在: {issue_file}", file=sys.stderr)
        return 1

    account_md = script_dir / MD_FILE
    if not account_md.is_file():
        print(f"账号文件不存在: {account_md}", file=sys.stderr)
        return 1

    md_accounts = read_account_md(account_md)
    merged_accounts: list[str] = []
    seen: set[str] = set()
    for account in read_issue_xlsx(issue_file) + md_accounts:
        add_unique(merged_accounts, seen, account)

    merged_results = fetch_results(
        grouped_account_strings(merged_accounts, GROUP_SIZE),
        FETCH_URL_TEMPLATE,
        GROUP_SIZE,
        REQUEST_TIMEOUT,
    )

    output_path = script_dir / OUTPUT_FILE
    output_path.write_text(
        json.dumps(merged_results, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )
    output_md_path = script_dir / OUTPUT_MD_FILE
    output_rows = build_output_md_rows(merged_results)
    write_output_md(output_md_path, output_rows)
    report = missing_field_report(merged_results)
    print(f"已写入 {output_path}，共 {len(merged_results)} 条账号信息")
    print(f"已写入 {output_md_path}，共 {len(output_rows)} 个账号")
    print(f"字段缺失样例: {json.dumps(report, ensure_ascii=False)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
