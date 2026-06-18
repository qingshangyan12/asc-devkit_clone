#!/bin/bash
# ============================================================================
# 脚本名称: calc_l2cache_hit.sh
# 功能描述: 读取 msprof 生成的 op_summary_*.csv 文件，根据 NPU 架构选择对应
#           的 L2Cache 命中率公式进行计算。
#
# 架构与公式:
#   Atlas A2/A3 系列 (dav-2201):
#     l2cache_hit_ratio = (r0_hit + r1_hit) / (r0_hit + r1_hit + r0_miss_allocate + r1_miss_allocate)
#   Ascend 950 系列 (dav-3510):
#     l2cache_hit_ratio = hit / (hit + miss + victim)
#
# 使用方法:
#   # 方式一：通过环境变量指定架构
#   export CMAKE_ASC_ARCHITECTURES=dav-2201
#   bash calc_l2cache_hit.sh op_summary_*.csv
#
#   # 方式二：不设置环境变量，脚本根据 CSV 列名自动匹配合适的公式
#   bash calc_l2cache_hit.sh op_summary_*.csv
#
#   # 方式三：通过命令行参数显式指定架构
#   bash calc_l2cache_hit.sh --arch dav-3510 op_summary_*.csv
# ============================================================================

set -euo pipefail

# ---------- 辅助函数 ----------

usage() {
    echo "用法: $0 [--arch <架构>] <csv文件...>"
    echo ""
    echo "参数:"
    echo "  --arch <架构>  手动指定 NPU 架构 (dav-2201 或 dav-3510)"
    echo "                  未指定时优先读取环境变量 CMAKE_ASC_ARCHITECTURES，"
    echo "                  仍未设置则根据 CSV 列名自动匹配。"
    echo "  csv文件         msprof 生成的 op_summary_*.csv 文件路径，支持通配符。"
    echo ""
    echo "示例:"
    echo "  $0 op_summary_*.csv"
    echo "  CMAKE_ASC_ARCHITECTURES=dav-3510 $0 op_summary_*.csv"
    echo "  $0 --arch dav-2201 op_summary_*.csv"
    exit 1
}

die() {
    echo "错误: $*" >&2
    exit 1
}

# ---------- 解析命令行参数 ----------

ARCH=""
CSV_FILES=()

while [[ $# -gt 0 ]]; do
    case "$1" in
        --arch)
            if [[ -z "${2:-}" ]]; then
                die "--arch 需要跟一个架构参数 (dav-2201 或 dav-3510)"
            fi
            ARCH="$2"
            shift 2
            ;;
        --arch=*)
            ARCH="${1#*=}"
            shift
            ;;
        --help|-h)
            usage
            ;;
        *)
            CSV_FILES+=("$1")
            shift
            ;;
    esac
done

# ---------- 确定 NPU 架构 ----------

if [[ -z "$ARCH" ]]; then
    # 尝试从环境变量获取
    if [[ -n "${CMAKE_ASC_ARCHITECTURES:-}" ]]; then
        ARCH="$CMAKE_ASC_ARCHITECTURES"
        echo "从环境变量 CMAKE_ASC_ARCHITECTURES 获取架构: $ARCH"
    else
        echo "未指定架构，将根据 CSV 列名自动匹配..."
    fi
fi

# 规范化架构名称（处理用户提到的简称）
normalize_arch() {
    local input="$1"
    case "$input" in
        dav-2201|dav2201|2201)
            echo "dav-2201"
            ;;
        dav-3510|dav3510|3510|dav510)
            echo "dav-3510"
            ;;
        *)
            echo "$input"
            ;;
    esac
}

if [[ -n "$ARCH" ]]; then
    ARCH=$(normalize_arch "$ARCH")
fi

# ---------- 检查 CSV 文件 ----------

if [[ ${#CSV_FILES[@]} -eq 0 ]]; then
    die "未指定 CSV 文件。用法: $0 [--arch <架构>] <csv文件...>"
fi

# ---------- 检测 Task Type 列索引 ----------

# 从 CSV 表头中找到 Task Type 列的索引（从1开始计数）
get_task_type_idx() {
    local header="$1"
    head -1 "$header" | tr ',' '\n' | awk '{if($0=="Task Type") print NR}'
}

# ---------- 根据列名自动检测架构 ----------

detect_arch_from_header() {
    local header="$1"
    if echo "$header" | grep -q "r0_read_cache_hit"; then
        echo "dav-2201"
    elif echo "$header" | grep -q "read_local_l2_hit"; then
        echo "dav-3510"
    else
        echo ""
    fi
}

# ---------- 计算命中率 ----------

calc_hit_ratio() {
    local arch="$1"
    local task_type_idx="$2"  # Task Type 列索引
    local file="$3"
    local header="$4"

    echo "文件: $file"
    echo "架构: $arch"

    case "$arch" in
        dav-2201)
            echo "公式: l2cache_hit_ratio = (r0_hit + r1_hit) / (r0_hit + r1_hit + r0_miss_allocate + r1_miss_allocate)"
            echo "每行根据 Task Type 自动选择列前缀: AI_VECTOR_CORE → aiv_*, AI_CORE → aic_*"
            echo ""
            printf "%-12s %-12s %-15s %-15s %-15s %-15s %s\n" "行号" "Task Type" "r0_hit" "r1_hit" "r0_miss" "r1_miss" "L2Cache命中率"
            echo "--------------------------------------------------------------------------------------------------------------------"

            awk -F',' -v task_type_idx="$task_type_idx" '
            NR == 1 {
                for (i = 1; i <= NF; i++) {
                    gsub(/^[ \t"]+|[" \t]+$/, "", $i)
                    if ($i == "aiv_r0_read_cache_hit")              aiv_r0_hit_idx = i
                    if ($i == "aiv_r0_read_cache_miss_allocate")    aiv_r0_miss_idx = i
                    if ($i == "aiv_r1_read_cache_hit")              aiv_r1_hit_idx = i
                    if ($i == "aiv_r1_read_cache_miss_allocate")    aiv_r1_miss_idx = i
                    if ($i == "aic_r0_read_cache_hit")              aic_r0_hit_idx = i
                    if ($i == "aic_r0_read_cache_miss_allocate")    aic_r0_miss_idx = i
                    if ($i == "aic_r1_read_cache_hit")              aic_r1_hit_idx = i
                    if ($i == "aic_r1_read_cache_miss_allocate")    aic_r1_miss_idx = i
                }
                next
            }
            {
                gsub(/"/, "", $0)
                # 根据 Task Type 选择使用 aiv_ 还是 aic_ 前缀
                task_type = $task_type_idx
                gsub(/^[ \t]+|[ \t]+$/, "", task_type)
                if (task_type == "AI_VECTOR_CORE") {
                    r0_hit_idx = aiv_r0_hit_idx
                    r1_hit_idx = aiv_r1_hit_idx
                    r0_miss_idx = aiv_r0_miss_idx
                    r1_miss_idx = aiv_r1_miss_idx
                } else if (task_type == "AI_CORE") {
                    r0_hit_idx = aic_r0_hit_idx
                    r1_hit_idx = aic_r1_hit_idx
                    r0_miss_idx = aic_r0_miss_idx
                    r1_miss_idx = aic_r1_miss_idx
                } else {
                    printf "%-12s %-12s (未知 Task Type，跳过)\n", NR-1, task_type
                    next
                }
                hit   = ($r0_hit_idx + 0) + ($r1_hit_idx + 0)
                miss  = ($r0_miss_idx + 0) + ($r1_miss_idx + 0)
                total = hit + miss
                ratio = (total > 0) ? hit / total : 0
                printf "%-12s %-12s %-15s %-15s %-15s %-15s %.4f%%\n",
                       NR-1, task_type, $r0_hit_idx, $r1_hit_idx, $r0_miss_idx, $r1_miss_idx, ratio * 100
            }' "$file"
            ;;

        dav-3510)
            echo "公式: l2cache_hit_ratio = hit / (hit + miss + victim)"
            echo "每行根据 Task Type 自动选择列前缀: AI_VECTOR_CORE → aiv_*, AI_CORE → aic_*"
            echo ""
            printf "%-12s %-12s %-15s %-15s %-15s %s\n" "行号" "Task Type" "hit" "miss" "victim" "L2Cache命中率"
            echo "----------------------------------------------------------------------------------------------"

            awk -F',' -v task_type_idx="$task_type_idx" '
            NR == 1 {
                for (i = 1; i <= NF; i++) {
                    gsub(/^[ \t"]+|[" \t]+$/, "", $i)
                    if ($i == "aiv_read_local_l2_hit")      aiv_hit_idx = i
                    if ($i == "aiv_read_local_l2_miss")     aiv_miss_idx = i
                    if ($i == "aiv_read_local_l2_victim")   aiv_victim_idx = i
                    if ($i == "aic_read_local_l2_hit")      aic_hit_idx = i
                    if ($i == "aic_read_local_l2_miss")     aic_miss_idx = i
                    if ($i == "aic_read_local_l2_victim")   aic_victim_idx = i
                }
                next
            }
            {
                gsub(/"/, "", $0)
                task_type = $task_type_idx
                gsub(/^[ \t]+|[ \t]+$/, "", task_type)
                if (task_type == "AI_VECTOR_CORE") {
                    hit_idx    = aiv_hit_idx
                    miss_idx   = aiv_miss_idx
                    victim_idx = aiv_victim_idx
                } else if (task_type == "AI_CORE") {
                    hit_idx    = aic_hit_idx
                    miss_idx   = aic_miss_idx
                    victim_idx = aic_victim_idx
                } else {
                    printf "%-12s %-12s (未知 Task Type，跳过)\n", NR-1, task_type
                    next
                }
                h  = ($hit_idx + 0)
                m  = ($miss_idx + 0)
                v  = ($victim_idx + 0)
                total = h + m + v
                ratio = (total > 0) ? h / total : 0
                printf "%-12s %-12s %-15s %-15s %-15s %.4f%%\n",
                       NR-1, task_type, $hit_idx, $miss_idx, $victim_idx, ratio * 100
            }' "$file"
            ;;
        *)
            die "不支持的架构: '$arch'，支持: dav-2201, dav-3510"
            ;;
    esac
}

# ---------- 主流程 ----------

for csv_file in "${CSV_FILES[@]}"; do
    if [[ ! -f "$csv_file" ]]; then
        echo "警告: 文件不存在，跳过: $csv_file" >&2
        continue
    fi

    echo ""
    echo "===================================================================================================="

    # 读取 CSV 表头（第一行）
    HEADER=$(head -1 "$csv_file")
    if [[ -z "$HEADER" ]]; then
        echo "警告: 文件为空，跳过: $csv_file" >&2
        continue
    fi

    # 获取 Task Type 列索引（第8列，从1开始计数）
    TASK_TYPE_IDX=$(get_task_type_idx "$csv_file")
    if [[ -z "$TASK_TYPE_IDX" ]]; then
        echo "警告: CSV 文件中未找到 Task Type 列，跳过: $csv_file" >&2
        continue
    fi

    # 确定架构
    CURRENT_ARCH="$ARCH"
    if [[ -z "$CURRENT_ARCH" ]]; then
        CURRENT_ARCH=$(detect_arch_from_header "$HEADER")
        if [[ -z "$CURRENT_ARCH" ]]; then
            echo "警告: 无法从 CSV 列名自动检测架构，且未通过 --arch 或 CMAKE_ASC_ARCHITECTURES 指定" >&2
            echo "请使用 --arch dav-2201 或 --arch dav-3510 指定架构" >&2
            continue
        fi
        echo "根据列名自动匹配架构: $CURRENT_ARCH"
    fi

    calc_hit_ratio "$CURRENT_ARCH" "$TASK_TYPE_IDX" "$csv_file" "$HEADER"
done

echo ""
echo "===================================================================================================="
echo "计算完成!"
