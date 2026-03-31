#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

vendor_name=customize
opmaster_so_name=libcust_opmaster_rt2.0.so
opsproto_so_name=libcust_opsproto_rt2.0.so
opapi_so_name=libcust_opapi.so
targetdir=/usr/local/Ascend/opp
target_custom=0

sourcedir=$PWD/packages
vendordir=vendors/$vendor_name
host_arch=$(uname -m)
opmaster_relpath=$vendordir/op_impl/ai_core/tbe/op_tiling/lib/linux/$host_arch/${opmaster_so_name}
opsproto_relpath=$vendordir/op_proto/lib/linux/$host_arch/${opsproto_so_name}
opapi_relpath=$vendordir/op_api/lib/${opapi_so_name}

QUIET="y"
INSTALL_FOR_ALL="n"
FORCE_INSTALL="n"
VALIDATE_DEBUG="y" # Debug-only switch. Set to "n" or remove in release.

CURR_OPERATE_USER="$(id -nu 2>/dev/null)"
CURR_OPERATE_GROUP="$(id -ng 2>/dev/null)"

while true
do
    case $1 in
    --quiet)
        QUIET="y"
        shift
    ;;
    --install-path=*)
        INSTALL_PATH=$(echo $1 | cut -d"=" -f2-)
        INSTALL_PATH=${INSTALL_PATH%*/}
        shift
    ;;
    --install-for-all)
        INSTALL_FOR_ALL="y"
        shift
    ;;
    --force)
        FORCE_INSTALL="y"
        shift
    ;;
    --*)
        shift
    ;;
    *)
        break
    ;;
    esac
done

log() {
    cur_date=$(date +"%Y-%m-%d %H:%M:%S")
    echo "[ops_custom] [$cur_date] "$1
}

debug_log() {
    if [ "${VALIDATE_DEBUG}" = "y" ]; then
        log "[DEBUG] $1"
    fi
}

resolve_so_path() {
    local so_name="$1"
    local so_path=""
    local search_dir=""

    if [ "${so_name}" = "${opmaster_so_name}" ]; then
        search_dir="${sourcedir}/${vendordir}/op_impl/ai_core/tbe/op_tiling/lib/linux"
        if [ -d "${search_dir}" ]; then
            so_path=$(find "${search_dir}" -type f -name "${so_name}" 2>/dev/null | head -n 1)
        fi
    elif [ "${so_name}" = "${opsproto_so_name}" ]; then
        search_dir="${sourcedir}/${vendordir}/op_proto/lib/linux"
        if [ -d "${search_dir}" ]; then
            so_path=$(find "${search_dir}" -type f -name "${so_name}" 2>/dev/null | head -n 1)
        fi
    elif [ "${so_name}" = "${opapi_so_name}" ]; then
        so_path="${sourcedir}/${opapi_relpath}"
    fi

    echo "${so_path}"
}

normalize_arch() {
    local arch_raw="$1"
    case "${arch_raw}" in
        x86_64|X86_64|Advanced\ Micro\ Devices\ X86-64)
            echo "x86_64"
            ;;
        aarch64|AArch64)
            echo "aarch64"
            ;;
        *)
            echo "${arch_raw}"
            ;;
    esac
}

format_so_arch() {
    local arch_raw="$1"
    case "${arch_raw}" in
        x86_64|X86_64|Advanced\ Micro\ Devices\ X86-64)
            echo "x86_64"
            ;;
        aarch64|AArch64)
            echo "AArch64"
            ;;
        *)
            echo "${arch_raw}"
            ;;
    esac
}

version_gt() {
    local version_a="$1"
    local version_b="$2"
    local max_version

    max_version=$(printf "%s\n%s\n" "${version_a}" "${version_b}" | sort -V | tail -n 1)
    if [ "${version_a}" = "${max_version}" ] && [ "${version_a}" != "${version_b}" ]; then
        return 0
    fi
    return 1
}

validate_so_dlopen() {
    local so_path="$1"
    local so_name="$2"
    local err_msg=""
    local so_dir=""
    local dlopen_lib_path=""
    local file_info=""
    local ldd_not_found=""
    local ldd_output=""

    debug_log "validate_so_dlopen start: so=${so_name}, path=${so_path}, ASCEND_HOME_PATH=${ASCEND_HOME_PATH}"
    so_dir=$(dirname "${so_path}")
    dlopen_lib_path="${so_dir}:${sourcedir}/${vendordir}/op_api/lib"
    if [ -n "${ASCEND_HOME_PATH}" ]; then
        dlopen_lib_path="${dlopen_lib_path}:${ASCEND_HOME_PATH}/lib64"
    fi

    err_msg=$(LD_LIBRARY_PATH="${dlopen_lib_path}:${LD_LIBRARY_PATH}" python3 - "${so_path}" << 'PY'
import ctypes
import os
import sys

path = sys.argv[1]
if not os.path.exists(path):
    print(f"{path}: file not found in package")
    sys.exit(1)
try:
    ctypes.CDLL(path, mode=os.RTLD_NOW)
except Exception as exc:
    print(str(exc))
    sys.exit(1)
PY
    )
    if [ $? -ne 0 ]; then
        err_msg="${err_msg//${so_path}/${so_name}}"
        log "[ERROR] Failed to load ${so_name} wish env ASCEND_HOME_PATH(${ASCEND_HOME_PATH}),error message is ${err_msg}."
        if [ -f "${so_path}" ]; then
            file_info=$(file "${so_path}" 2>/dev/null | cut -d: -f2- | sed 's/^ *//')
            ldd_not_found=$(LD_LIBRARY_PATH="${dlopen_lib_path}:${LD_LIBRARY_PATH}" ldd "${so_path}" 2>/dev/null | grep "not found" | tr '\n' ';')
            if [ -n "${ldd_not_found}" ]; then
                log "[ERROR] Diagnostic: ${so_name} exists, but some dependent libraries are missing: ${ldd_not_found}"
            else
                ldd_output=$(LD_LIBRARY_PATH="${dlopen_lib_path}:${LD_LIBRARY_PATH}" ldd "${so_path}" 2>&1 | head -n 1)
                log "[ERROR] Diagnostic: ${so_name} exists, but dynamic loading failed. This is usually caused by dependent libraries or architecture incompatibility (for example, aarch64 package on x86_64 host)."
                if [ -n "${file_info}" ]; then
                    log "[ERROR] Diagnostic file info (${so_name}): ${file_info}"
                fi
                if [ -n "${ldd_output}" ]; then
                    if echo "${ldd_output}" | grep -q "not a dynamic executable"; then
                        log "[ERROR] Diagnostic ldd output (${so_name}): ${ldd_output}. This usually indicates architecture or binary format incompatibility with current host."
                    else
                        log "[ERROR] Diagnostic ldd output (${so_name}): ${ldd_output}"
                    fi
                fi
            fi
        else
            log "[ERROR] Diagnostic: ${so_name} is missing in package path: ${so_path}"
        fi
        return 1
    fi
    debug_log "validate_so_dlopen success: so=${so_name}"
    return 0
}

validate_so_arch() {
    local so_path="$1"
    local so_name="$2"
    local so_machine_raw=""
    local so_arch_norm=""
    local env_arch=""
    local so_arch_display=""

    debug_log "validate_so_arch start: so=${so_name}, path=${so_path}"
    so_machine_raw=$(readelf -h "${so_path}" 2>/dev/null | awk -F: '/Machine:/{gsub(/^ +/, "", $2); print $2; exit}')
    if [ -z "${so_machine_raw}" ]; then
        log "[ERROR] Failed to parse architecture from '${so_name}'."
        return 1
    fi

    env_arch=$(uname -m)
    so_arch_norm=$(normalize_arch "${so_machine_raw}")
    so_arch_display=$(format_so_arch "${so_machine_raw}")

    if [ "${so_arch_norm}" != "${env_arch}" ]; then
        log "[ERROR] The architecture of the shared library '${so_name}' (${so_arch_display}) is incompatible with the current running environment (${env_arch})."
        return 1
    fi
    debug_log "validate_so_arch success: so=${so_name}, so_arch=${so_arch_display}, env_arch=${env_arch}"
    return 0
}

validate_so_glibc() {
    local so_path="$1"
    local so_name="$2"
    local required_glibc=""
    local env_glibc=""

    debug_log "validate_so_glibc start: so=${so_name}, path=${so_path}"
    required_glibc=$(strings -a "${so_path}" 2>/dev/null | grep -o 'GLIBC_[0-9]\+\.[0-9]\+' | sed 's/GLIBC_//' | sort -V | tail -n 1)
    if [ -z "${required_glibc}" ]; then
        debug_log "validate_so_glibc skip: so=${so_name}, no GLIBC symbol found"
        return 0
    fi

    env_glibc=$(ldd --version 2>/dev/null | head -n 1 | grep -o '[0-9]\+\.[0-9]\+' | head -n 1)
    if [ -z "${env_glibc}" ]; then
        log "[ERROR] Failed to get glibc version from current runtime environment."
        return 1
    fi

    if version_gt "${required_glibc}" "${env_glibc}"; then
        log "[ERROR] The shared library '${so_name}' was linked against a newer version of the GNU C Library (glibc). The current runtime enviroment has an older version of glibs, which does not provide the required symbols."
        return 1
    fi
    debug_log "validate_so_glibc success: so=${so_name}, required_glibc=${required_glibc}, env_glibc=${env_glibc}"
    return 0
}

validate_shared_libraries() {
    local so_name=""
    local so_path=""

    debug_log "validate_shared_libraries enter"
    if [ "${FORCE_INSTALL}" = "y" ]; then
        log "[WARNING] --force is enabled, skip shared library validation."
        debug_log "validate_shared_libraries bypass by --force"
        return 0
    fi

    for so_name in "${opmaster_so_name}" "${opsproto_so_name}" "${opapi_so_name}"; do
        debug_log "validate_shared_libraries checking so=${so_name}"
        so_path=$(resolve_so_path "${so_name}")
        if [ -z "${so_path}" ] || [ ! -f "${so_path}" ]; then
            log "[ERROR] Failed to find shared library '${so_name}' in package path."
            return 1
        fi

        if [ -n "${ASCEND_HOME_PATH}" ]; then
            debug_log "validate_shared_libraries branch=dlopen so=${so_name}"
            validate_so_dlopen "${so_path}" "${so_name}" || return 1
        else
            debug_log "validate_shared_libraries branch=arch+glibc so=${so_name}"
            validate_so_arch "${so_path}" "${so_name}" || return 1
            validate_so_glibc "${so_path}" "${so_name}" || return 1
        fi
        debug_log "validate_shared_libraries checked so=${so_name} success"
    done
    debug_log "validate_shared_libraries success"
    return 0
}

set_install_for_all_mod() {
    local _outvar="$1"
    local _mod="$2"
    local _new_mod

    local _new_mod="${_mod%?}"
    local _new_mod="${_new_mod}${_new_mod#${_new_mod%?}}"

    eval "${_outvar}=\"${_new_mod}\""
}

apply_chmod() {
    local path="$1"
    local mod="$2"
    local install_for_all="$3"

    local own="${CURR_OPERATE_USER}:${CURR_OPERATE_GROUP}"

    if [ "$install_for_all" = "true" ] || [ "$install_for_all" = "y" ]; then
        set_install_for_all_mod "mod" "$mod"
    fi
    chmod -R "$mod" "$path"
    if [ $? -ne 0 ]; then
        log "[ERROR] chmod $path $mod failed."
        return 1
    fi

    chown -f "$own" "$path"
    if [ $? -ne 0 ]; then
        log "[ERROR] chown $path $own failed."
        return 1
    fi

    return 0
}

create_dir() {
    local path="$1"
    local mod="$2"
    local install_for_all="$3"

    if [ "$path" = "" ]; then
        log "[ERROR] dir path is empty"
        return 1
    fi

    if [ ! -d "$path" ]; then
        mkdir -p "$path"
        if [ $? -ne 0 ]; then
            log "[ERROR] create dir $path failed."
            return 1
        fi
    fi

    apply_chmod "$path" "$mod" "$install_for_all"
    if [ $? -ne 0 ]; then
        return 1
    fi

    return 0
}

if [ -n "${INSTALL_PATH}" ]; then
    if [[ ! "${INSTALL_PATH}" = /* ]]; then
        log "[ERROR] use absolute path for --install-path argument"
        exit 1
    fi
    if [ ! -d ${INSTALL_PATH} ]; then
        create_dir "${INSTALL_PATH}" "750" "${INSTALL_FOR_ALL}"
        if [ $? -ne 0 ]; then
            exit 1
        fi
    fi
    targetdir=${INSTALL_PATH}
elif [ -n "${ASCEND_CUSTOM_OPP_PATH}" ]; then
    if [[ "${ASCEND_CUSTOM_OPP_PATH}" == *:* ]]; then
        log "[ERROR] environment variable ASCEND_CUSTOM_OPP_PATH=${ASCEND_CUSTOM_OPP_PATH} is set and \
        has multiple path in it (colon inside), which will cause the custom op installed incorrectly. \
        Please use the --install-path option to specify an installation path instead."
        exit 1
    fi
    if [ ! -d ${ASCEND_CUSTOM_OPP_PATH} ]; then
        create_dir "${INSASCEND_CUSTOM_OPP_PATHTALL_PATH}" "750" "${INSTALL_FOR_ALL}"
    fi
    targetdir=${ASCEND_CUSTOM_OPP_PATH}
else
    if [ "x${ASCEND_OPP_PATH}" == "x" ]; then
        log "[ERROR] env ASCEND_OPP_PATH no exist"
        exit 1
    fi
    targetdir="${ASCEND_OPP_PATH}"
fi

if [ ! -d $targetdir ];then
    log "[ERROR] $targetdir no exist"
    exit 1
fi

if [ ! -x $targetdir ] || [ ! -w $targetdir ] || [ ! -r $targetdir ];then
    log "[WARNING] The directory $targetdir does not have sufficient permissions. \
    Please check and modify the folder permissions (e.g., using chmod), \
    or use the --install-path option to specify an installation path and \
    change the environment variable ASCEND_CUSTOM_OPP_PATH to the specified path."
fi

upgrade()
{
    if [ ! -d ${sourcedir}/$vendordir/$1 ]; then
        log "[INFO] no need to upgrade ops $1 files"
        return 0
    fi

    if [ ! -d ${targetdir}/$vendordir/$1 ];then
        log "[INFO] create ${targetdir}/$vendordir/$1."
        create_dir "${targetdir}/$vendordir/$1" "750" "${INSTALL_FOR_ALL}"
        if [ $? -ne 0 ];then
            return 1
        fi
    else
        has_same_file=-1
        for file_a in ${sourcedir}/$vendordir/$1/*; do
            file_b=${file_a##*/};
            if [ "ls ${targetdir}/$vendordir/$1" = "" ]; then
                log "[INFO] ${targetdir}/$vendordir/$1 is empty !!"
		        return 1
	          fi
            grep -q $file_b <<<`ls ${targetdir}/$vendordir/$1`;
            if [[ $? -eq 0 ]]; then
                echo -n "${file_b} "
                has_same_file=0
            fi
        done
        if [ 0 -eq $has_same_file ]; then
            echo
            if test $QUIET = "n"; then
                echo "[INFO]: has old version in ${targetdir}/$vendordir/$1, \
                you want to Overlay Installation , please enter:[o]; \
                or replace directory installation , please enter: [r]; \
                or not install , please enter:[n]."

                while true
                do
                    read orn
                    if [ "$orn" = n ]; then
                        return 0
                    elif [ "$orn" = o ]; then
                        break;
                    elif [ "$orn" = r ]; then
                        [ -n "${targetdir}/$vendordir/$1/" ] && rm -rf "${targetdir}/$vendordir/$1"/*
                        break;
                    else
                        log "[ERROR] input error, please input again!"
                    fi
                done
            else
                [ -n "${targetdir}/$vendordir/$1/" ] && rm -rf "${targetdir}/$vendordir/$1"/*
            fi
        fi
        log "[INFO] replace or merge old ops $1 files ......"
    fi

    log "[INFO] copy new ops $1 files ......"
    if [ -d ${targetdir}/$vendordir/$1/ ]; then
        chmod -R +w "$targetdir/$vendordir/$1/" >/dev/null 2>&1
    fi
    cp -rf ${sourcedir}/$vendordir/$1/* $targetdir/$vendordir/$1/
    if [ $? -ne 0 ];then
        log "[ERROR] copy new $1 files failed"
        return 1
    fi

    apply_chmod "$targetdir/$vendordir/$1/" "750" "${INSTALL_FOR_ALL}"
    return 0
}
upgrade_proto()
{
    if [ ! -f ${sourcedir}/$vendordir/custom.proto ]; then
        log "[INFO] no need to upgrade custom.proto files"
        return 0
    fi
    if [ ! -d ${targetdir}/$vendordir/framework/caffe ];then
        log "[INFO] create ${targetdir}/$vendordir/framework/caffe."
        create_dir "${targetdir}/$vendordir/framework/caffe" "750" "${INSTALL_FOR_ALL}"
        if [ $? -ne 0 ];then
            return 1
        fi
    else
        if [ -f ${targetdir}/$vendordir/framework/caffe/custom.proto ]; then
            # 有老版本,判断是否要覆盖式安装
            if test $QUIET = "n"; then
                  echo "[INFO] ${targetdir}/$vendordir/framework/caffe has old version"\
                "custom.proto file. Do you want to replace? [y/n] "

                while true
                do
                    read yn
                    if [ "$yn" = n ]; then
                        return 0
                    elif [ "$yn" = y ]; then
                        break;
                    else
                        log "[ERROR] input error, please input again!"
                    fi
                done
            fi
        fi
        log "[INFO] replace old caffe.proto files ......"
    fi
    apply_chmod "$targetdir/$vendordir/framework/caffe/" "750" "${INSTALL_FOR_ALL}"
    cp -rf ${sourcedir}/$vendordir/custom.proto ${targetdir}/$vendordir/framework/caffe/
    if [ $? -ne 0 ];then
        log "[ERROR] copy new custom.proto failed"
        return 1
    fi
	log "[INFO] copy custom.proto success"

    return 0
}

upgrade_file()
{
    if [ ! -e ${sourcedir}/$vendordir/$1 ]; then
        log "[INFO] no need to upgrade ops $1 file"
        return 0
    fi

    log "[INFO] copy new $1 files ......"
    cp -f ${sourcedir}/$vendordir/$1 $targetdir/$vendordir/$1
    if [ $? -ne 0 ];then
        log "[ERROR] copy new $1 file failed"
        return 1
    fi
    apply_chmod "$targetdir/$vendordir/$1" "750" "${INSTALL_FOR_ALL}"
    return 0
}

upgrade_uninstall()
{
    uninstall_file=$(realpath ${sourcedir}/../uninstall.sh)
    dst_path=$targetdir/$vendordir/scripts
    if [ ! -e ${uninstall_file} ]; then
        log "[INFO] no need to upgrade uninstall.sh file"
        return 0
    fi

    log "[INFO] copy new uninstall.sh files ......"
    if [ ! -e ${dst_path} ]; then
        create_dir "${dst_path}" "750" "${INSTALL_FOR_ALL}"
    fi
    if [ ! -w ${dst_path} ]; then
        apply_chmod "$dst_path" "750" "${INSTALL_FOR_ALL}"
    fi
    cp -f ${uninstall_file} $dst_path
    if [ $? -ne 0 ];then
        log "[ERROR] copy new uninstall.sh file failed"
        return 1
    fi
    apply_chmod "$dst_path" "550" "${INSTALL_FOR_ALL}"
    return 0
}

delete_optiling_file()
{
  if [ ! -d ${targetdir}/vendors ];then
    log "[INFO] $1 not exist, no need to uninstall"
    return 0
  fi
  sys_info=$(uname -m)
  if [ ! -d ${sourcedir}/$vendordir/$1/ai_core/tbe/op_tiling/lib/linux/${sys_info} ];then
    rm -rf ${sourcedir}/$vendordir/$1/ai_core/tbe/op_tiling/liboptiling.so
  fi
  return 0
}

if [ ! -d "${targetdir}/vendors" ];then
    log "[INFO] create ${targetdir}/vendors."
    create_dir "${targetdir}/vendors" "750" "${INSTALL_FOR_ALL}"
    if [ $? -ne 0 ];then
        exit 1
    fi
else
    apply_chmod "${targetdir}/vendors" "750" "${INSTALL_FOR_ALL}"
fi

if [ ! -d "${targetdir}/$vendordir" ];then
    log "[INFO] create ${targetdir}/$vendordir."
    create_dir "${targetdir}/$vendordir" "750" "${INSTALL_FOR_ALL}"
    if [ $? -ne 0 ];then
        exit 1
    fi
else
    apply_chmod "${targetdir}/$vendordir" "750" "${INSTALL_FOR_ALL}"
fi

log "[INFO] validate shared libraries before installation"
validate_shared_libraries
if [ $? -ne 0 ]; then
    exit 1
fi

log "[INFO] upgrade framework"
upgrade framework
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO] upgrade op proto"
upgrade op_proto
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO] upgrade op impl"
delete_optiling_file op_impl
upgrade op_impl
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO] upgrade op api"
upgrade op_api
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO] upgrade uninstall"
upgrade_uninstall
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO] upgrade version.info"
upgrade_file version.info
if [ $? -ne 0 ];then
    exit 1
fi

upgrade_proto
if [ $? -ne 0 ];then
    exit 1
fi

# set the set_env.bash
if [ -n "${INSTALL_PATH}" ] && [ -d ${INSTALL_PATH} ]; then
    _ASCEND_CUSTOM_OPP_PATH=${targetdir}/${vendordir}
    bin_path="${_ASCEND_CUSTOM_OPP_PATH}/bin"
    set_env_variable="#!/bin/bash\nexport ASCEND_CUSTOM_OPP_PATH=${_ASCEND_CUSTOM_OPP_PATH}:\${ASCEND_CUSTOM_OPP_PATH}\nexport LD_LIBRARY_PATH=${_ASCEND_CUSTOM_OPP_PATH}/op_api/lib/:\${LD_LIBRARY_PATH}"
    if [ ! -d ${bin_path} ]; then
        create_dir "${bin_path}" "750" "${INSTALL_FOR_ALL}"
        if [ $? -ne 0 ]; then
            exit 1
        fi
    fi
    echo -e ${set_env_variable} > ${bin_path}/set_env.bash
    if [ $? -ne 0 ]; then
        log "[ERROR] write ASCEND_CUSTOM_OPP_PATH to set_env.bash failed"
        exit 1
    else
        log "[INFO] using requirements: when custom module install finished or before you run the custom module, \
        execute the command [ source ${bin_path}/set_env.bash ] to set the environment path"
    fi
else
    _ASCEND_CUSTOM_OPP_PATH=${targetdir}/${vendordir}
    config_file=${targetdir}/vendors/config.ini
    if [ ! -f ${config_file} ]; then
        touch ${config_file}
        apply_chmod "${config_file}" "640" "${INSTALL_FOR_ALL}"
        echo "load_priority=$vendor_name" > ${config_file}
        if [ $? -ne 0 ];then
            log "[ERROR] echo load_priority failed"
            exit 1
        fi
    else
        found_vendors="$(grep -w "load_priority" "$config_file" | cut --only-delimited -d"=" -f2-)"
        found_vendor=$(echo $found_vendors | sed "s/\<$vendor_name\>//g" | tr ',' ' ')
        vendor=$(echo $found_vendor | tr -s ' ' ',')
        if [ "$vendor" != "" ]; then
            sed -i "/load_priority=$found_vendors/s@load_priority=$found_vendors@load_priority=$vendor_name,$vendor@g" "$config_file"
        fi
    fi
    apply_chmod "${config_file}" "750" "${INSTALL_FOR_ALL}"
    log "[INFO] using requirements: when custom module install finished or before you run the custom module, \
        execute the command [ export LD_LIBRARY_PATH=${_ASCEND_CUSTOM_OPP_PATH}/op_api/lib/:\${LD_LIBRARY_PATH} ] to set the environment path"
fi

if [ -d ${targetdir}/$vendordir/op_impl/cpu/aicpu_kernel/impl/ ]; then
    chmod -R 444 ${targetdir}/$vendordir/op_impl/cpu/aicpu_kernel/impl/* >/dev/null 2>&1
fi

echo "SUCCESS"
exit 0
