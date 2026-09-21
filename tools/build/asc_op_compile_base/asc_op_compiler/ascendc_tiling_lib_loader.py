#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
"""
AscendC tiling library loading
"""

import ctypes
import glob
import os
from pathlib import Path
from asc_op_compile_base.asc_op_compiler.op_tiling import (
    _ASCEND_OPP_PATH_ENV,
    _ASCEND_OPP_PATH_DEFAULT,
    so_arch_path2,
    op_impl_path,
)
import asc_op_compile_base.common.context.op_context as op_context
from asc_op_compile_base.common.utils.log_utils import LogUtil, AscendCLogLevel
from asc_op_compile_base.common.utils import log

_ASCEND_CUSTOM_OPP_PATH_ENV = "ASCEND_CUSTOM_OPP_PATH"
_TILING_SO_PATH = "op_impl/ai_core/tbe/op_tiling/liboptiling.so"
opp_dir = os.environ.get(_ASCEND_OPP_PATH_ENV, _ASCEND_OPP_PATH_DEFAULT)
config_default = os.path.join(opp_dir, "vendors", "config.ini")


def get_default_optiling_pathlist():
    vendor_list = []
    full_path_list = []
    if not os.path.exists(config_default):
        return full_path_list
    with open(config_default) as f:
        vdr_lst = f.readline().split("=")[1].split(",")
        for vdr in vdr_lst:
            _vendor = vdr.strip()
            if _vendor not in vendor_list:
                vendor_list.append(_vendor)
                full_path = os.path.join(opp_dir, "vendors", _vendor)
                full_path_list.append(full_path)
    return full_path_list


def get_custom_opp_pathlist():
    vendor_list = []
    custom_opp_dir = None
    if op_context.get_context() is None:
        log.warn("op_context.get_context() is None!")
    else:
        custom_opp_dir = op_context.get_context().get_addition("custom_opp_path")
        log.info("op_context get custom_opp_dir: %s" % custom_opp_dir)
    if custom_opp_dir is None:
        custom_opp_dir = os.environ.get(_ASCEND_CUSTOM_OPP_PATH_ENV)
        log.info("get custom_opp_dir from env path: %s" % custom_opp_dir)
    if custom_opp_dir is None:
        return vendor_list
    _path_lst = str(custom_opp_dir).split(":")
    for _path in _path_lst:
        local_path = _path.strip()
        if len(local_path) != 0 and local_path not in vendor_list:
            vendor_list.append(local_path)
    return vendor_list


def load_op_host_tiling_lib():
    LogUtil.print_compile_log("", "load op host tiling lib.", AscendCLogLevel.LOG_INFO)
    builtin_op_host_tiling_prefix = (
        os.environ.get(_ASCEND_OPP_PATH_ENV, _ASCEND_OPP_PATH_DEFAULT)
        + "/"
        + op_impl_path
        + "/ai_core/tbe/op_host/"
    )
    builtin_op_host_tiling_pattern = builtin_op_host_tiling_prefix + "**/*.so"
    builtin_op_host_tiling_file_path = glob.glob(
        builtin_op_host_tiling_pattern, recursive=True
    )
    builtin_op_host_tiling_file_path.sort(
        key=lambda x: os.path.basename(x) == "libophost_legacy.so"
    )
    import platform

    archlinux = platform.machine()
    if "x86" in archlinux:
        incompatiable_arch_name = "aarch64"
    else:
        incompatiable_arch_name = "x86_64"
    for tiling_path in builtin_op_host_tiling_file_path:
        if incompatiable_arch_name in tiling_path[len(builtin_op_host_tiling_prefix) :]:
            continue
        try:
            if os.path.exists(tiling_path):
                lib_tiling_builtin = ctypes.CDLL(tiling_path)
                tiling_path_str = str(tiling_path)
                lib_tiling_builtin.TbeLoadSoAndSaveToRegistry(
                    tiling_path_str.encode("utf-8")
                )
        except OSError as e:
            LogUtil.print_compile_log(
                "",
                f"An OSError occurred: {e}, \
tiling_path: {tiling_path}",
                AscendCLogLevel.LOG_ERROR,
            )
            raise Exception(f"An OSError occurred, tiling_path: {tiling_path}") from e

        except Exception as e:
            LogUtil.print_compile_log(
                "",
                f"An Unknown error occurred: {e}, \
tiling_path: {tiling_path}",
                AscendCLogLevel.LOG_ERROR,
            )
            raise Exception(
                f"An Unknown error occurred, tiling_path: {tiling_path}"
            ) from e
    return


def load_build_in_lib():
    LogUtil.print_compile_log("", "load build in tiling lib.", AscendCLogLevel.LOG_INFO)
    # 1. load env tiling so
    tiling_so = os.environ.get("ASCEND_OPP_TILING_SO_PATH")
    if tiling_so is not None:
        if not isinstance(tiling_so, str) or not os.path.exists(tiling_so):
            LogUtil.print_compile_log(
                "",
                f"ASCEND_OPP_TILING_SO_PATH {tiling_so} invalid",
                AscendCLogLevel.LOG_ERROR,
            )
            return False
        tiling_so_path = Path(tiling_so)
        try:
            ctlib_opp_tiling = ctypes.CDLL(tiling_so_path)
        except AttributeError as e:
            # ascend c static load builtin opmaster ct so fail
            LogUtil.print_compile_log(
                "",
                f"An AttributeError occurred: {e}, when load tiling so {tiling_so_path}",
                AscendCLogLevel.LOG_ERROR,
            )
            return False
        return True

    # 2. loat built-in tiling so in op_host dir
    load_op_host_tiling_lib()
    opp_path = Path(os.environ.get(_ASCEND_OPP_PATH_ENV, _ASCEND_OPP_PATH_DEFAULT))

    builtin_optiling_lib_path2 = opp_path.joinpath(so_arch_path2)
    parent_dir = Path(os.path.dirname(builtin_optiling_lib_path2))
    builtin_optiling_rtlib_path = parent_dir.joinpath("libopmaster_rt.so")
    builtin_optiling_ctlib_path = parent_dir.joinpath("libopmaster_ct.so")
    # 3. builtin optiling 2.0 regist compile time
    try:
        if os.path.exists(builtin_optiling_ctlib_path):
            ctlib_optiling_builtin = ctypes.CDLL(builtin_optiling_ctlib_path)
            builtin_optiling_ctlib_path_str = str(builtin_optiling_ctlib_path)
            ctlib_optiling_builtin.TbeLoadSoAndSaveToRegistry(
                builtin_optiling_ctlib_path_str.encode("utf_8")
            )
    except AttributeError as e:
        # ascend c static load builtin opmaster ct so fail
        LogUtil.print_compile_log(
            "",
            f"An AttributeError occurred: {e}, when load tiling so {builtin_optiling_ctlib_path}",
            AscendCLogLevel.LOG_ERROR,
        )
        return False

    # 4. builtin optiling 2.0 regist runtime
    try:
        if os.path.exists(builtin_optiling_rtlib_path):
            lib_optiling_builtin = ctypes.CDLL(builtin_optiling_rtlib_path)
            builtin_optiling_lib_path_str = str(builtin_optiling_rtlib_path)
            lib_optiling_builtin.TbeLoadSoAndSaveToRegistry(
                builtin_optiling_lib_path_str.encode("utf_8")
            )
        elif os.path.exists(builtin_optiling_lib_path2):
            lib_optiling_builtin = ctypes.CDLL(builtin_optiling_lib_path2)
            builtin_optiling_lib_path2_str = str(builtin_optiling_lib_path2)
            lib_optiling_builtin.TbeLoadSoAndSaveToRegistry(
                builtin_optiling_lib_path2_str.encode("utf_8")
            )
    except AttributeError as e:
        # ascend c static load builtin opmaster rt so fail, undefined symbol, then use 1.0 way
        LogUtil.print_compile_log(
            "",
            f"An AttributeError occurred: {e}, when load tiling so "
            f"{builtin_optiling_rtlib_path} or {builtin_optiling_lib_path2}",
            AscendCLogLevel.LOG_ERROR,
        )
        return False
    return True


def load_lib():
    libregister = ctypes.CDLL("libregister.so")

    # 1. custom optiling 2.0 regist
    default_lst = get_default_optiling_pathlist()
    custom_opp_list = get_custom_opp_pathlist()
    join_list = custom_opp_list + default_lst
    for _path in join_list:
        try:
            custom_opp_so_path = os.path.join(_path, _TILING_SO_PATH)
            if not os.path.exists(custom_opp_so_path):
                LogUtil.print_compile_log(
                    "", f"{custom_opp_so_path} not exists", AscendCLogLevel.LOG_INFO
                )
            else:
                lib_optiling = ctypes.CDLL(custom_opp_so_path)
                custom_opp_so_path_str = str(custom_opp_so_path)
                lib_optiling.TbeLoadSoAndSaveToRegistry(
                    custom_opp_so_path_str.encode("utf_8")
                )
        except OSError as e:
            # Custom op tiling lib may not exists
            LogUtil.print_compile_log(
                "",
                f"An OSError occurred: {e}, when load tiling so {custom_opp_so_path}",
                AscendCLogLevel.LOG_ERROR,
            )
            pass

    # load build in tiling so
    load_build_in_lib()

    return libregister


def load_custom_lib(custom_op_tiling_path):
    try:
        if not os.path.exists(custom_op_tiling_path):
            LogUtil.print_compile_log(
                "", f"{custom_op_tiling_path} not exists", AscendCLogLevel.LOG_INFO
            )
            return False
        else:
            lib_optiling = ctypes.CDLL(custom_op_tiling_path)
            custom_op_tiling_path_str = str(custom_op_tiling_path)
            lib_optiling.TbeLoadSoAndSaveToRegistry(
                custom_op_tiling_path_str.encode("utf_8")
            )
            return True
    except OSError as e:
        # Custom op tiling lib may not exists
        LogUtil.print_compile_log(
            "",
            f"An OSError occurred: {e}, \
tiling_path: {custom_op_tiling_path}",
            AscendCLogLevel.LOG_ERROR,
        )
        raise Exception(
            f"An OSError occurred, tiling_path: {custom_op_tiling_path}"
        ) from e
    except Exception as e:
        # Custom op tiling lib may not exists
        LogUtil.print_compile_log(
            "",
            f"An Unknown error occurred: {e}, \
tiling_path: {custom_op_tiling_path}",
            AscendCLogLevel.LOG_ERROR,
        )
        raise Exception(
            f"An Unknown error occurred, tiling_path: {custom_op_tiling_path}"
        ) from e
    return False
