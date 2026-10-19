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
#       下载地址（URL）、SHA256 与离线存储路径均固化硬编码在本脚本内，取值来源为 cmake 仓
#       third_party/ 下对应的 xxx.cmake 声明；三方件版本升级时需同步更新此处的固化值。
#
# 用法：
#   sh scripts/download_third_party_offline.sh [--3rd-dir <路径>] [--cmake-dir <路径>] [-h]
#
# 参数：
#   --3rd-dir   离线缓存目录，默认 <仓根>/build（与 build.sh 的 CANN_3RD_LIB_PATH 默认值一致，
#               可被 --cann_3rd_lib_path 覆盖，需保持两者一致）
#   --cmake-dir cmake 仓路径（仅用于归档 json 补丁，下载任务本身不依赖 cmake 仓），默认依次探测：
#               $CANN_CMAKE_DIR 环境变量 -> <仓根>/../cmake -> <仓根>/build/_deps/cann-cmake-src
#
# 固化清单（URL / SHA256 来源：cmake 仓 third_party/xxx.cmake）：
#   1. 各文件按 cmake 文件中声明的离线分支路径存储，构建时自动命中本地缓存，不再联网：
#        boost_1_87_0.tar.gz               -> <3rd-dir>/          (boost.cmake DOWNLOAD_URL)
#        googletest-1.14.0.tar.gz          -> <3rd-dir>/          (gtest.cmake GTEST_PROJECT_URL 下载分支)
#        json-3.12.0.tar.gz                -> <3rd-dir>/          (json.cmake REQ_URL 下载分支)
#        makeself-release-2.5.0.tar.gz     -> <3rd-dir>/          (makeself-fetch.cmake REQ_URL)
#        makeself-2.5.0.patch              -> <3rd-dir>/patch/    (makeself-fetch.cmake PATCH_REQ_URL)
#        json_3.12.0_change_version.patch  -> <3rd-dir>/patch/    (从 cmake 仓复制归档)
#   2. json 补丁无下载 URL（json.cmake 直接从 cmake 仓 patch/ 目录应用），此处复制仅为离线归档；
#      未找到 cmake 仓时仅告警，不影响其余下载。
#   3. 已存在且 SHA256 校验通过的文件自动跳过；校验失败则重新下载。

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"

# ---------------- 固化的三方件下载清单（来源：cmake 仓 third_party/*.cmake，版本升级时同步更新） ----------------
# boost: boost.cmake
BOOST_URL="https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/boost/boost_1_87_0.tar.gz"
BOOST_REL="boost_1_87_0.tar.gz"
BOOST_SHA="f55c340aa49763b1925ccf02b2e83f35fdcf634c9d5164a2acb87540173c741d"

# gtest: gtest.cmake（下载分支）
GTEST_URL="https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/googletest/googletest-1.14.0.tar.gz"
GTEST_REL="googletest-1.14.0.tar.gz"
GTEST_SHA="8ad598c73ad796e0d8280b082cebd82a630d73e73cd3c70057938a6501bba5d7"

# json: json.cmake（下载分支）；补丁取自 cmake 仓 patch/ 目录，无下载 URL
JSON_URL="https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/json/json-3.12.0.tar.gz"
JSON_REL="json-3.12.0.tar.gz"
JSON_SHA="4b92eb0c06d10683f7447ce9406cb97cd4b453be18d7279320f7b2f025c10187"
JSON_PATCH_REL="patch/json_3.12.0_change_version.patch"

# makeself: makeself-fetch.cmake（tar 包与补丁各自独立 URL）
MAKESELF_TAR_URL="https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/makeself/makeself-release-2.5.0.tar.gz"
MAKESELF_TAR_REL="makeself-release-2.5.0.tar.gz"
MAKESELF_TAR_SHA="705d0376db9109a8ef1d4f3876c9997ee6bed454a23619e1dbc03d25033e46ea"
MAKESELF_PATCH_URL="https://cann-3rd.obs.cn-north-4.myhuaweicloud.com/makeself/fix/makeself-2.5.0.patch"
MAKESELF_PATCH_REL="patch/makeself-2.5.0.patch"
MAKESELF_PATCH_SHA="0ecac3fe200221353322626900d5257a59d98347f83653b98456e08475994486"

usage() {
    echo "用法: $0 [--3rd-dir <路径>] [--cmake-dir <路径>] [-h]"
    echo "  --3rd-dir    离线缓存目录（默认 ${REPO_ROOT}/build，与 CANN_3RD_LIB_PATH 一致）"
    echo "  --cmake-dir  cmake 仓路径（仅用于归档 json 补丁，下载不再依赖）"
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

echo "[INFO] 离线缓存目录(CANN_3RD_LIB_PATH): ${THIRD_DIR}"
mkdir -p "${THIRD_DIR}/patch"

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

    if [ -f "${target}" ] && [ "$(sha256_of "${target}")" = "${sha}" ]; then
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

    local actual
    actual="$(sha256_of "${tmp}")"
    if [ "${actual}" != "${sha}" ]; then
        rm -f "${tmp}"
        echo "[失败] ${comp}: SHA256 不匹配 (期望 ${sha}, 实际 ${actual})"
        return 1
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

# ---------------- json 补丁归档（cmake 仓自带，无下载 URL） ----------------
find_cmake_dir() {
    local candidate
    local candidates=()
    [ -n "$CMAKE_DIR" ] && candidates+=("$CMAKE_DIR")
    candidates+=("${REPO_ROOT}/../cmake" "${REPO_ROOT}/build/_deps/cann-cmake-src")
    for candidate in "${candidates[@]}"; do
        if [ -f "${candidate}/third_party/${JSON_PATCH_REL}" ]; then
            echo "$(cd "${candidate}" && pwd)"
            return 0
        fi
    done
    return 1
}

if CMAKE_REPO="$(find_cmake_dir)"; then
    cp -f "${CMAKE_REPO}/third_party/${JSON_PATCH_REL}" "${THIRD_DIR}/${JSON_PATCH_REL}"
    echo "[归档] json 补丁: ${JSON_PATCH_REL} 已从 cmake 仓复制至离线目录（构建时由 cmake 仓自带路径应用）"
else
    echo "[警告] 未找到 cmake 仓（已探测: --cmake-dir / \$CANN_CMAKE_DIR / ${REPO_ROOT}/../cmake / ${REPO_ROOT}/build/_deps/cann-cmake-src），"
    echo "       跳过 json 补丁归档；json 构建补丁由 cmake 仓 patch/ 目录自带，不影响其余离线包下载。"
fi

# ---------------- 汇总 ----------------
echo ""
echo "=============== 离线缓存汇总 (${THIRD_DIR}) ==============="
for rel in "${BOOST_REL}" "${GTEST_REL}" "${JSON_REL}" "${MAKESELF_TAR_REL}" "${MAKESELF_PATCH_REL}" "${JSON_PATCH_REL}"; do
    if [ -f "${THIRD_DIR}/${rel}" ]; then
        echo "  [OK] ${rel}"
    else
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
