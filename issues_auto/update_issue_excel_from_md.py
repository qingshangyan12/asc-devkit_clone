#!/usr/bin/env python3
"""根据 gitcode_accounts.md 更新 issue Excel 中的账号展示和资源组。"""

from __future__ import annotations

import argparse
import copy
import re
import sys
import unicodedata
from pathlib import Path

from openpyxl import load_workbook


ACCOUNT_MD_FILE = "gitcode_accounts.md"
OUTPUT_SUFFIX = "_updated"
PROPOSER_COLUMN = "提出者"
OWNER_COLUMN = "责任人"
RESOURCE_GROUP_COLUMN = "资源组"
PL_COLUMN = "PL"
OPERATOR_TEST_COLUMN = "算子内测"
GITCODE_COLUMN = "gitcode"
NAME_COLUMN = "姓名"
IS_OPERATOR_GROUP_COLUMN = "是否为算子组"

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
FORMATTED_ACCOUNT_RE = re.compile(r"^(.+?)\(([^()]+)\)$")


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


def split_markdown_row(line: str) -> list[str]:
    """按 Markdown 表格分隔符拆分一行。"""
    return [normalize_cell(part) for part in line.strip().strip("|").split("|")]


def is_markdown_separator(row: list[str]) -> bool:
    """识别 Markdown 表头下方的分隔行。"""
    return bool(row) and all(re.fullmatch(r":?-{3,}:?", cell) for cell in row)


def parse_account_value(value: object) -> tuple[str, str]:
    """解析 Excel 单元格，返回可能的 gitcode 和中文名。"""
    text = normalize_cell(value)
    match = FORMATTED_ACCOUNT_RE.fullmatch(text)
    if match:
        name = normalize_cell(match.group(1))
        account = normalize_cell(match.group(2))
        return account, name
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
                for column in (
                    GITCODE_COLUMN,
                    NAME_COLUMN,
                    RESOURCE_GROUP_COLUMN,
                    PL_COLUMN,
                    IS_OPERATOR_GROUP_COLUMN,
                ):
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

            key = account.lower()
            account_info[key] = data
            if not is_invalid_value(name):
                name_to_account.setdefault(name, account)

    return account_info, name_to_account


def format_account(
    value: object,
    account_info: dict[str, dict[str, str]],
    name_to_account: dict[str, str],
) -> str | None:
    """把账号或中文名转换为“中文名(gitcode)”格式，找不到信息时返回 None。"""
    account, name = parse_account_value(value)
    if not account and name:
        account = name_to_account.get(name, "")
    if not is_gitcode_id(account) or has_chinese(account):
        return None

    info = account_info.get(account.lower())
    if not info:
        return None

    chinese_name = info.get(NAME_COLUMN, "")
    if is_invalid_value(chinese_name):
        return account
    return f"{chinese_name}({account})"


def get_account_resource_group(
    value: object,
    account_info: dict[str, dict[str, str]],
    name_to_account: dict[str, str],
) -> str | None:
    """根据责任人单元格找到资源组，找不到时返回 None。"""
    account, name = parse_account_value(value)
    if not account and name:
        account = name_to_account.get(name, "")
    if not is_gitcode_id(account) or has_chinese(account):
        return None

    info = account_info.get(account.lower())
    if not info:
        return None
    resource_group = info.get(RESOURCE_GROUP_COLUMN, "")
    if is_invalid_value(resource_group):
        return None
    return resource_group


def get_account_pl(
    value: object,
    account_info: dict[str, dict[str, str]],
    name_to_account: dict[str, str],
) -> str | None:
    """根据责任人单元格找到 PL，找不到时返回 None。"""
    account, name = parse_account_value(value)
    if not account and name:
        account = name_to_account.get(name, "")
    if not is_gitcode_id(account) or has_chinese(account):
        return None

    info = account_info.get(account.lower())
    if not info:
        return None
    pl = info.get(PL_COLUMN, "")
    if is_invalid_value(pl):
        return None
    return pl


def get_account_operator_test(
    value: object,
    account_info: dict[str, dict[str, str]],
    name_to_account: dict[str, str],
) -> str | None:
    """根据提出者单元格判断算子内测；只有算子组返回“是”，其它返回空。"""
    account, name = parse_account_value(value)
    if not account and name:
        account = name_to_account.get(name, "")
    if not is_gitcode_id(account) or has_chinese(account):
        return ""

    info = account_info.get(account.lower())
    if not info:
        return ""
    value = info.get(IS_OPERATOR_GROUP_COLUMN, "")
    return "是" if normalize_cell(value) == "是" else ""


def copy_cell_style(source, target) -> None:
    """复制单元格样式，不复制值。"""
    if source.has_style:
        target.font = copy.copy(source.font)
        target.fill = copy.copy(source.fill)
        target.border = copy.copy(source.border)
        target.alignment = copy.copy(source.alignment)
        target.protection = copy.copy(source.protection)
        target.number_format = source.number_format


def copy_column_style_from_left(sheet, target_column: int) -> None:
    """让目标列继承左侧列样式，避免新增列颜色和原表不一致。"""
    if target_column <= 1:
        return

    source_column = target_column - 1
    source_dimension = sheet.column_dimensions[
        sheet.cell(row=1, column=source_column).column_letter
    ]
    target_dimension = sheet.column_dimensions[
        sheet.cell(row=1, column=target_column).column_letter
    ]
    target_dimension.width = source_dimension.width

    for row_index in range(1, sheet.max_row + 1):
        copy_cell_style(
            sheet.cell(row=row_index, column=source_column),
            sheet.cell(row=row_index, column=target_column),
        )


def ensure_column(sheet, header: list[str], column_name: str) -> int:
    """确保 sheet 中存在指定列；不存在时追加到表头最后。"""
    if column_name in header:
        return header.index(column_name) + 1

    column_index = len(header) + 1
    sheet.cell(row=1, column=column_index).value = column_name
    header.append(column_name)
    copy_column_style_from_left(sheet, column_index)
    return column_index


def update_excel(
    excel_path: Path, account_md: Path
) -> tuple[Path, int, int, int, int, int]:
    """更新 Excel 并保存为新文件，返回输出路径和更新计数。"""
    account_info, name_to_account = read_account_md(account_md)
    workbook = load_workbook(excel_path)

    account_update_count = 0
    resource_update_count = 0
    pl_update_count = 0
    operator_test_update_count = 0
    sheet_count = 0

    try:
        for sheet in workbook.worksheets:
            rows = sheet.iter_rows()
            try:
                header = [normalize_cell(cell.value) for cell in next(rows)]
            except StopIteration:
                continue

            column_indexes = {
                column: header.index(column) + 1
                for column in (
                    PROPOSER_COLUMN,
                    OWNER_COLUMN,
                    RESOURCE_GROUP_COLUMN,
                    PL_COLUMN,
                    OPERATOR_TEST_COLUMN,
                )
                if column in header
            }
            if PROPOSER_COLUMN not in column_indexes and OWNER_COLUMN not in column_indexes:
                continue

            sheet_count += 1
            if PROPOSER_COLUMN in column_indexes:
                column_indexes[OPERATOR_TEST_COLUMN] = ensure_column(
                    sheet,
                    header,
                    OPERATOR_TEST_COLUMN,
                )
                copy_column_style_from_left(sheet, column_indexes[OPERATOR_TEST_COLUMN])

            for row_index in range(2, sheet.max_row + 1):
                for column in (PROPOSER_COLUMN, OWNER_COLUMN):
                    column_index = column_indexes.get(column)
                    if not column_index:
                        continue
                    cell = sheet.cell(row=row_index, column=column_index)
                    new_value = format_account(cell.value, account_info, name_to_account)
                    if new_value and normalize_cell(cell.value) != new_value:
                        cell.value = new_value
                        account_update_count += 1

                owner_index = column_indexes.get(OWNER_COLUMN)
                proposer_index = column_indexes.get(PROPOSER_COLUMN)
                resource_index = column_indexes.get(RESOURCE_GROUP_COLUMN)
                pl_index = column_indexes.get(PL_COLUMN)
                operator_test_index = column_indexes.get(OPERATOR_TEST_COLUMN)
                if owner_index and resource_index:
                    owner_cell = sheet.cell(row=row_index, column=owner_index)
                    resource_group = get_account_resource_group(
                        owner_cell.value, account_info, name_to_account
                    )
                    if resource_group:
                        resource_cell = sheet.cell(row=row_index, column=resource_index)
                        if normalize_cell(resource_cell.value) != resource_group:
                            resource_cell.value = resource_group
                            resource_update_count += 1
                if owner_index and pl_index:
                    owner_cell = sheet.cell(row=row_index, column=owner_index)
                    pl = get_account_pl(owner_cell.value, account_info, name_to_account)
                    if pl:
                        pl_cell = sheet.cell(row=row_index, column=pl_index)
                        if normalize_cell(pl_cell.value) != pl:
                            pl_cell.value = pl
                            pl_update_count += 1
                if proposer_index and operator_test_index:
                    proposer_cell = sheet.cell(row=row_index, column=proposer_index)
                    operator_test = get_account_operator_test(
                        proposer_cell.value, account_info, name_to_account
                    )
                    operator_test_cell = sheet.cell(
                        row=row_index, column=operator_test_index
                    )
                    if normalize_cell(operator_test_cell.value) != operator_test:
                        operator_test_cell.value = operator_test
                        operator_test_update_count += 1

        output_path = excel_path.with_name(
            f"{excel_path.stem}{OUTPUT_SUFFIX}{excel_path.suffix}"
        )
        workbook.save(output_path)
    finally:
        workbook.close()

    return (
        output_path,
        sheet_count,
        account_update_count,
        resource_update_count,
        pl_update_count,
        operator_test_update_count,
    )


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "根据同级 gitcode_accounts.md 更新 Excel：提出者/责任人改为"
            "“中文名(gitcode)”，根据责任人更新资源组和 PL，"
            "根据提出者更新算子内测。"
        )
    )
    parser.add_argument("excel", help="issue Excel 文件")
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

    (
        output_path,
        sheet_count,
        account_count,
        resource_count,
        pl_count,
        operator_test_count,
    ) = update_excel(excel_path, account_md)
    print(f"已写入 {output_path}")
    print(f"处理 sheet 数: {sheet_count}")
    print(f"更新提出者/责任人单元格: {account_count}")
    print(f"更新资源组单元格: {resource_count}")
    print(f"更新 PL 单元格: {pl_count}")
    print(f"更新算子内测单元格: {operator_test_count}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
