#!/usr/bin/env bash

set -uo pipefail
# CANN安装目录，可通过ASCEND_HOME_PATH指定。
CANN_INSTALL_PATH="${ASCEND_HOME_PATH:-/home/grj/Ascend/cann}"
if [[ -f "${CANN_INSTALL_PATH}/set_env.sh" ]]; then
    source "${CANN_INSTALL_PATH}/set_env.sh"
fi

NPU_ARCH="dav-3510"
TIMEOUT_SECONDS=60

# 测试集为本次根据AtomGit机器人检视意见修改的3份调用示例，路径相对于本脚本所在目录。
API_DOCS=(
    "load/asc_loadalign_brc_elem.md"
    "store/asc_storealign_pack_quarter.md"
    "store/asc_storealign_pack_quarter_postupdate.md"
)

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPORT_FILE="${SCRIPT_DIR}/调用示例批量执行报告.md"
SUMMARY_FILE="${SCRIPT_DIR}/调用示例汇总表.md"

TEMP_ROOT="$(mktemp -d)"

cleanup()
{
    rm -rf "${TEMP_ROOT}"
}
trap cleanup EXIT

write_line()
{
    printf '%s\n' "$1" >> "${REPORT_FILE}"
}

refresh_summary_table()
{
    local status_file="$1"
    local api_doc_list_file="$2"
    python3 - "${SCRIPT_DIR}" "${SUMMARY_FILE}" "${status_file}" "${api_doc_list_file}" <<'PY'
from pathlib import Path
import re
import sys

script_dir = Path(sys.argv[1])
summary_file = Path(sys.argv[2])
status_file = Path(sys.argv[3])
api_doc_list_file = Path(sys.argv[4])

executed_docs = set()
if api_doc_list_file.exists():
    executed_docs = {line.strip() for line in api_doc_list_file.read_text(encoding="utf-8").splitlines() if line.strip()}

status = {}
if summary_file.exists():
    for line in summary_file.read_text(encoding="utf-8").splitlines():
        match = re.search(
            r"\|\s*\d+\s*\|\s*\[[^\]]+\]\(([^)]+)\)\s*\|\s*[^|]+\|\s*([^|]+)\|\s*([^|]+)\|\s*([^|]+)\|",
            line,
        )
        if not match:
            continue
        result = match.group(2).strip()
        if result == "未执行":
            continue
        status[match.group(1)] = {
            "result": result,
            "exit_code": match.group(3).strip(),
            "elapsed": match.group(4).strip(),
        }

if status_file.exists():
    for line in status_file.read_text(encoding="utf-8").splitlines():
        parts = line.split("\t")
        if len(parts) != 4:
            continue
        status[parts[0]] = {
            "result": parts[1],
            "exit_code": parts[2],
            "elapsed": parts[3],
        }

def extract_main_example(text):
    for match in re.finditer(r"```([^\n`]*)\n(.*?)\n```", text, re.S):
        lang = match.group(1).strip().lower()
        code = match.group(2)
        if lang in ("c", "cpp", "c++") and "__simd_vf__" in code:
            return code
    return text

def has_vf_tail_loop(text):
    code = extract_main_example(text)
    vf_pos = code.find("__simd_vf__")
    if vf_pos < 0:
        return False
    end = len(code)
    for marker in ("\n__global__", "\nint main"):
        marker_pos = code.find(marker, vf_pos + 1)
        if marker_pos >= 0:
            end = min(end, marker_pos)
    vf_code = code[vf_pos:end]
    has_loop = re.search(r"\bfor\s*\(", vf_code) is not None
    has_tail_control = re.search(
        r"\b(?:asc_update_mask_b(?:8|16|32)|remaining|valid_count|valid_elements|current_count)\b",
        vf_code,
    ) is not None
    return has_loop and has_tail_control

existing_targets = []
if summary_file.exists():
    for line in summary_file.read_text(encoding="utf-8").splitlines():
        match = re.search(r"\|\s*\d+\s*\|\s*\[([^\]]+)\]\(([^)]+)\)", line)
        if match:
            existing_targets.append(match.group(2))

if existing_targets:
    target_rels = existing_targets
else:
    target_rels = []
    for path in sorted(script_dir.rglob("*.md")):
        rel = path.relative_to(script_dir).as_posix()
        if rel in executed_docs or rel == "vf_call/asc_vf_call.md":
            continue
        if rel in {"调用示例批量执行报告.md", "调用示例汇总表.md"}:
            continue
        text = path.read_text(encoding="utf-8")
        if "__simd_vf__" not in text:
            continue
        target_rels.append(rel)

target_docs = []
for rel in target_rels:
    doc_path = script_dir / rel
    if doc_path.exists():
        target_docs.append((rel, has_vf_tail_loop(doc_path.read_text(encoding="utf-8"))))
    else:
        target_docs.append((rel, False))

not_adapted = sum(1 for _, adapted in target_docs if not adapted)
not_executed = sum(1 for rel, _ in target_docs if rel not in status)
failed = sum(1 for rel, _ in target_docs if rel in status and status[rel]["result"] != "通过")
passed = sum(1 for rel, _ in target_docs if rel in status and status[rel]["result"] == "通过")

lines = [
    "# 调用示例汇总表",
    "",
    "- 说明：本表跟踪尚未纳入批量执行清单的SIMD VF调用示例，当前口径排除不含独立“调用示例”小节的说明/总览文件、已在脚本`API_DOCS`中验证的样例和`vf_call/asc_vf_call.md`说明页。",
    f"- 样例总数：{len(target_docs)}",
    f"- 未适配尾块处理：{not_adapted}",
    f"- 未执行：{not_executed}",
    f"- 执行通过：{passed}",
    f"- 执行未通过：{failed}",
    "",
    "| # | 接口文档 | 尾块处理状态 | 执行状态 | 退出码 | 耗时 |",
    "|---|---|---|---|---|---|",
]

for index, (rel, adapted) in enumerate(target_docs, start=1):
    item = status.get(rel)
    result = item["result"] if item else "未执行"
    exit_code = item["exit_code"] if item else "-"
    elapsed = item["elapsed"] if item else "-"
    for_status = "已适配" if adapted else "未适配"
    lines.append(f"| {index} | [{rel}]({rel}) | {for_status} | {result} | {exit_code} | {elapsed} |")

summary_file.write_text("\n".join(lines) + "\n", encoding="utf-8")
PY
}

extract_example()
{
    local doc_file="$1"
    local output_file="$2"
    python3 - "$doc_file" "$output_file" <<'PY'
from pathlib import Path
import re
import sys

doc_file = Path(sys.argv[1])
output_file = Path(sys.argv[2])
text = doc_file.read_text(encoding="utf-8")

for match in re.finditer(r"^[ \t]*```([^\n`]*)\n(.*?)^[ \t]*```", text, re.S | re.M):
    lang = match.group(1).strip().lower()
    code = match.group(2)
    if lang not in ("c", "cpp", "c++"):
        continue
    if "#include" in code and "int main()" in code:
        output_file.write_text(code.rstrip() + "\n", encoding="utf-8")
        sys.exit(0)

sys.stderr.write("未找到包含#include和int main()的c/cpp调用示例代码块\n")
sys.exit(2)
PY
}

summarize_log()
{
    local log_file="$1"
    if [[ ! -s "${log_file}" ]]; then
        printf '无输出\n'
        return
    fi
    tail -n 80 "${log_file}" | sed 's/[[:space:]]\+$//'
}

get_bisheng_extra_flags()
{
    local api_doc="$1"
    case "${api_doc}" in
        "reg_arith/asc_ln.md" | \
        "reg_arith/asc_log.md" | \
        "reg_arith/asc_sqrt.md" | \
        "reg_arith/asc_exp.md" | \
        "reg_fused/asc_exp_sub.md")
            printf '%s' "-lm"
            ;;
        *)
            printf '%s' ""
            ;;
    esac
}

if ! command -v bisheng >/dev/null 2>&1; then
    echo "未找到bisheng命令，请检查CANN环境是否配置成功。" >&2
    exit 1
fi

if ! command -v timeout >/dev/null 2>&1; then
    echo "未找到timeout命令，无法限制样例最大执行时间。" >&2
    exit 1
fi

total=${#API_DOCS[@]}
passed=0
failed=0
timeout_count=0
extract_failed=0
missing_file=0

api_doc_list_file="${TEMP_ROOT}/api_docs.txt"
status_file="${TEMP_ROOT}/样例状态.tsv"
result_failed_rows="${TEMP_ROOT}/结果_失败优先.md"
result_passed_rows="${TEMP_ROOT}/结果_通过.md"
result_other_rows="${TEMP_ROOT}/结果_其他.md"

printf '%s\n' "${API_DOCS[@]}" > "${api_doc_list_file}"
: > "${status_file}"
: > "${result_failed_rows}"
: > "${result_passed_rows}"
: > "${result_other_rows}"
refresh_summary_table "${status_file}" "${api_doc_list_file}"

: > "${REPORT_FILE}"
write_line "# 调用示例批量执行报告"
write_line ""
write_line "- 执行时间：$(date '+%Y-%m-%d %H:%M:%S')"
write_line "- CANN安装目录：\`${CANN_INSTALL_PATH}\`"
write_line "- NPU架构：\`${NPU_ARCH}\`"
write_line "- 单样例最大执行时间：${TIMEOUT_SECONDS}秒"
write_line "- 临时目录：\`${TEMP_ROOT}\`"
write_line ""
write_line "## 执行结果"
write_line ""
write_line "失败、超时、示例提取失败和文档不存在的样例优先展示。"
write_line ""
write_line "| # | 接口文档 | 结果 | 退出码 | 耗时 |"
write_line "|---|---|---|---|---|"

detail_file="${TEMP_ROOT}/失败详情.md"
: > "${detail_file}"

index=0
for api_doc in "${API_DOCS[@]}"; do
    index=$((index + 1))
    api_name="${api_doc##*/}"
    api_name="${api_name%.md}"
    printf '[%s/%s] 当前测试接口：%s（%s）\n' "${index}" "${total}" "${api_name}" "${api_doc}"

    doc_file="${SCRIPT_DIR}/${api_doc}"
    sample_dir="${TEMP_ROOT}/case_${index}"
    example_file="${sample_dir}/example.asc"
    log_file="${sample_dir}/output.log"
    mkdir -p "${sample_dir}"

    result="通过"
    exit_code=0
    elapsed_text="0s"

    start_time=$(date +%s)
    if [[ ! -f "${doc_file}" ]]; then
        result="文档不存在"
        exit_code=127
        missing_file=$((missing_file + 1))
        printf '文档不存在：%s\n' "${doc_file}" > "${log_file}"
    elif ! extract_example "${doc_file}" "${example_file}" > "${log_file}" 2>&1; then
        result="示例提取失败"
        exit_code=2
        extract_failed=$((extract_failed + 1))
    else
        extra_flags="$(get_bisheng_extra_flags "${api_doc}")"
        (
            cd "${sample_dir}" &&
            timeout "${TIMEOUT_SECONDS}s" bash -c "bisheng example.asc -o main --npu-arch=${NPU_ARCH} ${extra_flags} && ./main"
        ) > "${log_file}" 2>&1
        exit_code=$?
        if [[ ${exit_code} -eq 0 ]]; then
            passed=$((passed + 1))
        elif [[ ${exit_code} -eq 124 ]]; then
            result="超时"
            timeout_count=$((timeout_count + 1))
        else
            result="失败"
            failed=$((failed + 1))
        fi
    fi

    end_time=$(date +%s)
    elapsed_text="$((end_time - start_time))s"

    if [[ "${result}" != "通过" ]]; then
        if [[ "${result}" == "文档不存在" ]]; then
            link_text="\`${api_doc}\`"
        else
            link_text="[${api_doc}](${api_doc})"
        fi
        {
            printf '### %s\n\n' "${api_doc}"
            printf -- '- 结果：%s\n' "${result}"
            printf -- '- 退出码：%s\n' "${exit_code}"
            printf -- '- 耗时：%s\n\n' "${elapsed_text}"
            printf '```text\n'
            summarize_log "${log_file}"
            printf '```\n\n'
        } >> "${detail_file}"
    else
        link_text="[${api_doc}](${api_doc})"
    fi

    row="| ${index} | ${link_text} | ${result} | ${exit_code} | ${elapsed_text} |"
    if [[ "${result}" == "通过" ]]; then
        printf '%s\n' "${row}" >> "${result_passed_rows}"
    elif [[ "${result}" == "失败" || "${result}" == "超时" || "${result}" == "示例提取失败" || "${result}" == "文档不存在" ]]; then
        printf '%s\n' "${row}" >> "${result_failed_rows}"
    else
        printf '%s\n' "${row}" >> "${result_other_rows}"
    fi
    printf '%s\t%s\t%s\t%s\n' "${api_doc}" "${result}" "${exit_code}" "${elapsed_text}" >> "${status_file}"
    refresh_summary_table "${status_file}" "${api_doc_list_file}"

    rm -f "${sample_dir}/main" "${example_file}"
    : > "${example_file}"
done

cat "${result_failed_rows}" >> "${REPORT_FILE}"
cat "${result_other_rows}" >> "${REPORT_FILE}"
cat "${result_passed_rows}" >> "${REPORT_FILE}"

write_line ""
write_line "## 汇总"
write_line ""
write_line "- 总数：${total}"
write_line "- 通过：${passed}"
write_line "- 失败：${failed}"
write_line "- 超时：${timeout_count}"
write_line "- 示例提取失败：${extract_failed}"
write_line "- 文档不存在：${missing_file}"

if [[ -s "${detail_file}" ]]; then
    write_line ""
    write_line "## 失败详情"
    write_line ""
    cat "${detail_file}" >> "${REPORT_FILE}"
fi

echo "执行完成，报告已生成：${REPORT_FILE}"
