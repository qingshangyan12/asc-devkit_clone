#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software: you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# 功能：离线下载 asc-devkit 依赖的三方件（boost / gtest / json / makeself）及补丁。
#       下载地址（URL）与存储地址（离线缓存路径）均从 cmake 仓 third_party/ 下对应的
#       xxx.cmake 实时索引解析，不硬编码，版本升级后无需修改本脚本。
#
# 用法：
#   sh scripts/download_third_party_offline.sh [--3rd-dir <路径>] [--cmake-dir <路径>] [-h]
#
# 参数：
#   --3rd-dir   离线缓存目录，默认 <仓根>/build（与 build.sh 的 CANN_3RD_LIB_PATH 默认值一致，
#               可被 --cann_3rd_lib_path 覆盖，需保持两者一致）
#   --cmake-dir cmake 仓路径（包含 third_party/xxx.cmake），默认依次探测：
#               $CANN_CMAKE_DIR 环境变量 -> <仓根>/../cmake -> <仓根>/build/_deps/cann-cmake-src
#
# 说明：
#   1. 各文件按 cmake 文件中声明的离线分支路径存储，构建时自动命中本地缓存，不再联网：
#        boost_1_87_0.tar.gz               -> <3rd-dir>/                      (boost.cmake)
#        googletest-1.14.0.tar.gz          -> <3rd-dir>/                      (gtest.cmake)
#        json-3.12.0.tar.gz                -> <3rd-dir>/                      (json.cmake)
#        makeself-release-2.5.0.tar.gz     -> <3rd-dir>/                      (makeself-fetch.cmake)
#        makeself-2.5.0.patch              -> <3rd-dir>/patch/                (makeself-fetch.cmake)
#        json_3.12.0_change_version.patch  -> <3rd-dir>/patch/                (从 cmake 仓复制归档)
#   2. json 补丁无下载 URL（json.cmake 直接从 cmake 仓 patch/ 目录应用），此处复制仅为离线归档。
#   3. 已存在且 SHA256 校验通过的文件自动跳过；校验失败则重新下载。

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
THIRD_PARTY_COMPONENTS="boost gtest json makeself-fetch"

usage() {
    echo "用法: $0 [--3rd-dir <路径>] [--cmake-dir <路径>] [-h]"
    echo "  --3rd-dir    离线缓存目录（默认 ${REPO_ROOT}/build，与 CANN_3RD_LIB_PATH 一致）"
    echo "  --cmake-dir  cmake 仓路径（含 third_party/*.cmake）"
    echo "  -h           显示帮助"
}

# ---------------- 参数解析 ----------------
THIRD_DIR="${REPO_ROOT}/build"
CMAKE_DIR="${CANN_CMAKE_DIR:-}"
while [ $# -gt 0 ]; do
    case "$1" in
        --3rd-dir)
            THIRD_DIR="$2"; shift 2 ;;
        --3rd-dir=*)
            THIRD_DIR="${1#*=}"; shift ;;
        --cmake-dir)
            CMAKE_DIR="$2"; shift 2 ;;
        --cmake-dir=*)
            CMAKE_DIR="${1#*=}"; shift ;;
        -h|--help)
            usage; exit 0 ;;
        *)
            echo "[错误] 未知参数: $1"; usage; exit 1 ;;
    esac
done

# ---------------- 定位 cmake 仓 ----------------
find_cmake_dir() {
    local candidate
    local candidates=()
    [ -n "$CMAKE_DIR" ] && candidates+=("$CMAKE_DIR")
    candidates+=("${REPO_ROOT}/../cmake" "${REPO_ROOT}/build/_deps/cann-cmake-src")
    for candidate in "${candidates[@]}"; do
        if [ -f "${candidate}/third_party/boost.cmake" ]; then
            echo "$(cd "${candidate}" && pwd)"
            return 0
        fi
    done
    return 1
}

if ! TP_DIR="$(find_cmake_dir)"; then
    echo "[错误] 未找到 cmake 仓（需包含 third_party/boost.cmake），已探测:"
    echo "         --cmake-dir 参数 / \$CANN_CMAKE_DIR / ${REPO_ROOT}/../cmake / ${REPO_ROOT}/build/_deps/cann-cmake-src"
    echo "       请通过 --cmake-dir 指定，或将 cmake 仓检出到 asc-devkit 的同级目录。"
    exit 1
fi
TP_DIR="${TP_DIR}/third_party"

# ---------------- cmake 文件索引解析 ----------------
# 提取 set(<VAR> "value") 的 value
get_cmake_var() {
    sed -n "s/^ *set($1 \"\(.*\)\") */\1/p" "$2" | head -1
}

# 提取文件中第 N 个 SHA256= 哈希值
get_sha256() {
    grep -oE 'SHA256=[0-9a-fA-F]{64}' "$1" | sed -n "$2p" | cut -d= -f2
}

# 剥离 ${CANN_3RD_LIB_PATH}/ 前缀，得到相对存储路径
strip_3rd_prefix() {
    echo "$1" | sed 's|^\${CANN_3RD_LIB_PATH}/||'
}

echo "[INFO] cmake 仓三方件目录: ${TP_DIR}"
echo "[INFO] 离线缓存目录(CANN_3RD_LIB_PATH): ${THIRD_DIR}"
mkdir -p "${THIRD_DIR}/patch"

# boost: DOWNLOAD_URL 内含 ${BOOST_FILE} 占位符，需替换
BOOST_FILE="$(get_cmake_var BOOST_FILE "${TP_DIR}/boost.cmake")"
BOOST_URL="$(get_cmake_var DOWNLOAD_URL "${TP_DIR}/boost.cmake")"
BOOST_URL="$(echo "${BOOST_URL}" | sed "s|\${BOOST_FILE}|${BOOST_FILE}|g")"
BOOST_SHA="$(get_sha256 "${TP_DIR}/boost.cmake" 1)"
BOOST_REL="${BOOST_FILE}"

# gtest: 取 https 开头的 GTEST_PROJECT_URL（本地缓存分支为目录路径，跳过）
GTEST_URL="$(sed -n 's/.*set(GTEST_PROJECT_URL "\(https:[^"]*\)").*/\1/p' "${TP_DIR}/gtest.cmake" | head -1)"
GTEST_SHA="$(get_sha256 "${TP_DIR}/gtest.cmake" 1)"
GTEST_REL="$(basename "${GTEST_URL}")"

# json: 取 https 开头的 REQ_URL；补丁从 PATCH_COMMAND 行索引（cmake 仓本地文件）
JSON_URL="$(sed -n 's/.*set(REQ_URL "\(https:[^"]*\)").*/\1/p' "${TP_DIR}/json.cmake" | head -1)"
JSON_SHA="$(get_sha256 "${TP_DIR}/json.cmake" 1)"
JSON_REL="$(basename "${JSON_URL}")"
JSON_PATCH_REL="$(grep -oE 'patch/[A-Za-z0-9._-]+\.patch' "${TP_DIR}/json.cmake" | head -1)"

# makeself-fetch: tar 与 patch 均有独立 URL 与存储路径变量
MAKESELF_TAR_URL="$(get_cmake_var REQ_URL "${TP_DIR}/makeself-fetch.cmake")"
MAKESELF_PATCH_URL="$(get_cmake_var PATCH_REQ_URL "${TP_DIR}/makeself-fetch.cmake")"
MAKESELF_TAR_REL="$(strip_3rd_prefix "$(get_cmake_var MAKESELF_TAR_PATH "${TP_DIR}/makeself-fetch.cmake")")"
MAKESELF_PATCH_REL="$(strip_3rd_prefix "$(get_cmake_var MAKESELF_PATCH "${TP_DIR}/makeself-fetch.cmake")")"
MAKESELF_TAR_SHA="$(get_sha256 "${TP_DIR}/makeself-fetch.cmake" 1)"
MAKESELF_PATCH_SHA="$(get_sha256 "${TP_DIR}/makeself-fetch.cmake" 2)"

# ---------------- 下载与校验 ----------------
sha256_of() {
    if command -v sha256sum >/dev/null 2>&1; then
        sha256sum "$1" | awk '{print $1}'
    else
        shasum -a 256 "$1" | awk '{print $1}'
    fi
}

# fetch_file <URL> <存储相对路径> <SHA256> <组件名>
fetch_file() {
    local url="$1" rel="$2" sha="$3" comp="$4"
    local target="${THIRD_DIR}/${rel}"
    local tmp="${target}.tmp"

    if [ -f "${target}" ] && [ -n "${sha}" ] && [ "$(sha256_of "${target}")" = "${sha}" ]; then
        echo "[SKIP] ${comp}: ${rel} 已存在且校验通过"
        return 0
    fi

    echo "[下载] ${comp}: ${url}"
    echo "       -> ${target}"
    rm -f "${tmp}"
    local ok=0
    if command -v wget >/dev/null 2>&1; then
        wget -q --tries=3 --timeout=120 -O "${tmp}" "${url}" && ok=1
    fi
    if [ "${ok}" -eq 0 ] && command -v curl >/dev/null 2>&1; then
        curl -fsSL --retry 3 --connect-timeout 120 -o "${tmp}" "${url}" && ok=1
    fi
    if [ "${ok}" -eq 0 ] || [ ! -s "${tmp}" ]; then
        rm -f "${tmp}"
        echo "[失败] ${comp}: 下载失败 ${url}"
        return 1
    fi

    if [ -n "${sha}" ]; then
        local actual
        actual="$(sha256_of "${tmp}")"
        if [ "${actual}" != "${sha}" ]; then
            rm -f "${tmp}"
            echo "[失败] ${comp}: SHA256 不匹配 (期望 ${sha}, 实际 ${actual})"
            return 1
        fi
    else
        echo "[警告] ${comp}: 未从 cmake 文件索引到 SHA256，跳过校验"
    fi

    mkdir -p "$(dirname "${target}")"
    mv -f "${tmp}" "${target}"
    echo "[完成] ${comp}: ${rel} (SHA256 校验通过)"
}

FAILED=0

fetch_file "${BOOST_URL}" "${BOOST_REL}" "${BOOST_SHA}" "boost" || FAILED=1
fetch_file "${GTEST_URL}" "${GTEST_REL}" "${GTEST_SHA}" "gtest" || FAILED=1
fetch_file "${JSON_URL}" "${JSON_REL}" "${JSON_SHA}" "json" || FAILED=1
fetch_file "${MAKESELF_TAR_URL}" "${MAKESELF_TAR_REL}" "${MAKESELF_TAR_SHA}" "makeself-fetch" || FAILED=1
fetch_file "${MAKESELF_PATCH_URL}" "${MAKESELF_PATCH_REL}" "${MAKESELF_PATCH_SHA}" "makeself-patch" || FAILED=1

# json 补丁：cmake 仓自带（无下载 URL），复制到离线归档目录
if [ -n "${JSON_PATCH_REL}" ]; then
    if [ -f "${TP_DIR}/${JSON_PATCH_REL}" ]; then
        cp -f "${TP_DIR}/${JSON_PATCH_REL}" "${THIRD_DIR}/${JSON_PATCH_REL}"
        echo "[归档] json 补丁: ${JSON_PATCH_REL} 已从 cmake 仓复制至离线目录（构建时由 cmake 仓自带路径应用）"
    else
        echo "[警告] json 补丁 ${TP_DIR}/${JSON_PATCH_REL} 不存在，跳过归档"
    fi
fi

# ---------------- 汇总 ----------------
echo ""
echo "=============== 离线缓存汇总 (${THIRD_DIR}) ==============="
for rel in "${BOOST_REL}" "${GTEST_REL}" "${JSON_REL}" "${MAKESELF_TAR_REL}" "${MAKESELF_PATCH_REL}" "${JSON_PATCH_REL}"; do
    if [ -n "${rel}" ] && [ -f "${THIRD_DIR}/${rel}" ]; then
        echo "  [OK] ${rel}"
    elif [ -n "${rel}" ]; then
        echo "  [缺失] ${rel}"
    fi
done
echo "==========================================================="

if [ "${FAILED}" -ne 0 ]; then
    echo "[错误] 存在下载失败项，请检查网络或 URL 有效性后重试。"
    exit 1
fi

echo "[提示] 离线编译时使用: sh build.sh --cann_3rd_lib_path=${THIRD_DIR}（或默认 build 目录即本目录）"
exit 0
