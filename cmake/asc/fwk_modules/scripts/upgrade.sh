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
targetdir=/usr/local/Ascend/opp
target_custom=0

sourcedir=$PWD/packages
vendordir=vendors/$vendor_name

log() {
    cur_date=$(date +"%Y-%m-%d %H:%M:%S")
    echo "[ops_custom] [$cur_date] "$1
}

update_load_priority() {
    local config_file="$1"
    local line_count=0
    local grep_status=0
    local priority_line=""
    local priority_value=""
    local item=""
    local existing=""
    local duplicate=0
    local new_value=""
    local -a items=()
    local -a new_items=("${vendor_name}")

    line_count=$(grep -cE '^[[:space:]]*load_priority[[:space:]]*=' "${config_file}" 2>/dev/null)
    grep_status=$?
    if [ "${grep_status}" -gt 1 ]; then
        log "[ERROR] read ${config_file} failed"
        return 1
    fi
    if [ "${line_count}" -gt 1 ]; then
        log "[ERROR] multiple load_priority entries found in ${config_file}"
        return 1
    fi

    if [ "${line_count}" -eq 1 ]; then
        priority_line=$(grep -m1 -E '^[[:space:]]*load_priority[[:space:]]*=' "${config_file}")
        priority_value=${priority_line#*=}
        IFS=',' read -r -a items <<< "${priority_value}"
        for item in "${items[@]}"; do
            item=$(printf '%s' "${item}" | tr -d '[:space:]')
            [ -z "${item}" ] && continue
            [ "${item}" = "${vendor_name}" ] && continue

            duplicate=0
            for existing in "${new_items[@]}"; do
                if [ "${item}" = "${existing}" ]; then
                    duplicate=1
                    break
                fi
            done
            [ "${duplicate}" -eq 0 ] && new_items+=("${item}")
        done
    fi

    new_value=$(IFS=,; echo "${new_items[*]}")
    if [ "${line_count}" -eq 0 ]; then
        printf 'load_priority=%s\n' "${new_value}" >> "${config_file}"
    else
        sed -i -E "0,/^[[:space:]]*load_priority[[:space:]]*=.*/s//load_priority=${new_value}/" "${config_file}"
    fi
    if [ $? -ne 0 ] || ! grep -Fxq "load_priority=${new_value}" "${config_file}"; then
        log "[ERROR] update load_priority in ${config_file} failed"
        return 1
    fi

    return 0
}

if [[ "x${ASCEND_OPP_PATH}" == "x" ]];then
    log "[ERROR] env ASCEND_OPP_PATH no exist"
    exit 1
fi

targetdir=${ASCEND_OPP_PATH}

if [ ! -d $targetdir ];then
    log "[ERROR] $targetdir no exist"
    exit 1
fi

if [ ! -x $targetdir ] || [ ! -w $targetdir ] || [ ! -r $targetdir ];then
    log "[WARNING] The directory $targetdir does not have sufficient permissions. \
    Please check and modify the folder permissions (e.g., using chmod)."
fi

upgrade()
{
    if [ ! -d ${sourcedir}/$vendordir/$1 ]; then
        log "[INFO] no need to upgrade ops $1 files"
        return 0
    fi

    if [ ! -d ${targetdir}/$vendordir/$1 ];then
        log "[INFO] create ${targetdir}/$vendordir/$1."
        mkdir -p ${targetdir}/$vendordir/$1
        if [ $? -ne 0 ];then
            log "[ERROR] create ${targetdir}/$vendordir/$1 failed"
            return 1
        fi
    else
        vendor_installed_dir=$(ls "$targetdir/vendors" 2> /dev/null)
        for i in $vendor_installed_dir;do
            vendor_installed_file=$(ls "$vendor_installed_dir/$vendor_name/$i" 2> /dev/null)
            if [ "$i" = "$vendor_name" ] && [ "$vendor_installed_file" != "" ]; then
                echo "[INFO]: $vendor_name custom opp package has been installed on the path $vendor_installed_dir, \
                you want to Overlay Installation , please enter:[o]; \
                or replace directory installation , please enter: [r]; \
                or not install , please enter:[n]."
            fi
	          while true
            do
                read mrn
                if [ "$mrn" = o ]; then
                    break
                elif [ "$mrn" = r ]; then
                    [ -n "$vendor_installed_file"] && rm -rf "$vendor_installed_file"
                    break
                elif [ "$mrn" = n ]; then
                    return 0
                else
                    log "[WARNING]: Input error, please input m or r or n to choose!"
                fi
            done
        done
        log "[INFO] replace old ops $1 files ......"
    fi

    log "copy new ops $1 files ......"
    cp -rf ${sourcedir}/$vendordir/$1/* $targetdir/$vendordir/$1/
    if [ $? -ne 0 ];then
        log "[ERROR] copy new $1 files failed"
        return 1
    fi

    return 0
}

upgrade_file()
{
    if [ ! -e ${sourcedir}/$vendordir/$1 ]; then
        log "[INFO] no need to upgrade ops $1 file"
        return 0
    fi

    log "copy new $1 files ......"
    cp -f ${sourcedir}/$vendordir/$1 $targetdir/$vendordir/$1
    if [ $? -ne 0 ];then
        log "[ERROR] copy new $1 file failed"
        return 1
    fi

    return 0
}

log "[INFO] copy uninstall sh success"

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
upgrade op_impl
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO] upgrade op api"
upgrade op_api
if [ $? -ne 0 ];then
    exit 1
fi

log "[INFO] upgrade version.info"
upgrade_file version.info
if [ $? -ne 0 ];then
    exit 1
fi

config_file=${targetdir}/vendors/config.ini
if [ ! -f "${config_file}" ]; then
    touch "${config_file}"
    if [ $? -ne 0 ]; then
        log "[ERROR] create ${config_file} failed"
        exit 1
    fi
fi
update_load_priority "${config_file}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "SUCCESS"
exit 0
