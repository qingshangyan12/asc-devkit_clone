#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------
# Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ----------------------------------------------------------------------------

import os
import stat
import sys
import json
import operator

from tbe.common.utils import log as logger

from static_compile.gen_simplified_key import gen_simplified_key, load_json


# 运行时禁止生成__pycache__/文件夹
sys.dont_write_bytecode = True


class ShapeCompileRes:
    def __init__(self, shape_config_name, object_name, object_path):
        self.shape_config_name = shape_config_name
        self.object_name = object_name
        self.object_path = object_path


def _get_obj_list(object_dir) -> list:
    obj_list = {}
    for root, _, files in os.walk(object_dir):
        for file in files:
            if not file.endswith('.json'):
                continue
            path = os.path.join(root, file)
            obj_name = file[:-5]
            if obj_name in obj_list:
                raise Exception(f"error: {root}/{obj_name}.json filename dup with {obj_list[obj_name]}/{obj_name}")
            obj_list[obj_name] = root
    return obj_list


def _get_shape_compile_res(shape_config_dir, object_dir, obj_list, shape_file):
    content = load_json(os.path.join(shape_config_dir, shape_file))
    bin_file_name = content.get('bin_filename')
    if bin_file_name is None:
        bin_file_name = shape_file[:-5]
    if bin_file_name in obj_list:
        if not os.path.exists(os.path.join(obj_list[bin_file_name], '{}.o'.format(bin_file_name))):
            raise Exception(f"error: this shape {shape_file} cannot find {bin_file_name}.o"
                            f" in {obj_list[bin_file_name]}")
        return ShapeCompileRes(shape_file, bin_file_name, obj_list[bin_file_name])
    else:
        logger.warn(f"this shape {shape_file} cannot find {bin_file_name}.json in {object_dir}")
        return None


def _get_shape_compile_res_list(shape_config_dir, object_dir) -> list:
    obj_list = _get_obj_list(object_dir)
    shape_compile_res_list = []
    for _, _, files in os.walk(shape_config_dir):
        for shape_file in files:
            if not shape_file.endswith('.json'):
                continue
            shape_compile_res = _get_shape_compile_res(shape_config_dir, object_dir, obj_list, shape_file)
            if shape_compile_res is not None:
                shape_compile_res_list.append(shape_compile_res)
    return shape_compile_res_list


def _get_core_type_enum(core_type: str):
    if core_type == 'MIX':
        return 0
    elif core_type == 'AiCore':
        return 1
    elif core_type == 'VectorCore':
        return 2
    elif core_type == 'MIX_AICORE':
        return 3
    elif core_type == 'MIX_VECTOR_CORE' or core_type == 'MIX_AIV':
        return 4
    return None


def _gen_one_obj(obj_cfg, simplified_key_arr: list, object_name, bin_path, depend_idx_arr):
    one_obj = {}
    core_type = obj_cfg.get('coreType')
    if core_type is None:
        raise Exception(f"error: object json {object_name} has no coreType")
    core_type_enum = _get_core_type_enum(core_type)
    if core_type_enum is not None:
        one_obj['coreType'] = core_type_enum
    one_obj['simplifiedKey'] = simplified_key_arr[0]
    one_obj['simplifiedKeyWithPlatform'] = simplified_key_arr[1]
    one_obj['binPath'] = bin_path
    block_dim = obj_cfg.get('blockDim')
    if block_dim is None:
        raise Exception(f"error: object json {object_name} has no blockDim")

    bin_desc = {}
    bin_desc['blockDim'] = block_dim
    kernel_name = obj_cfg.get('kernelName')
    if kernel_name is None:
        raise Exception(f"error: object json {object_name} has no kernelName")
    bin_desc['kernelName'] = kernel_name
    workspace = obj_cfg.get('workspace')
    if workspace is not None:
        sz = workspace.get('size')
        if sz is None:
            raise Exception(f"error: object json {object_name} has workspace but no size")
        bin_desc['workspace'] = sz
    else:
        bin_desc['workspace'] = []
    one_obj['binDesc'] = bin_desc
    return one_obj


def _check_simplified_key_not_dup(\
    simplified_key_json_match, static_list, shape_config_name, simplified_key_with_platform, shape_config_dir):
    for static_one_obj in static_list:
        if static_one_obj.get('simplifiedKeyWithPlatform') == simplified_key_with_platform:
            json_matched = simplified_key_json_match.get(simplified_key_with_platform, "")
            with open(os.path.join(shape_config_dir, json_matched), encoding='utf-8') as f:
                js = json.load(f)
                json_matched_orginal_path = js['origin_path']
                with open(os.path.join(shape_config_dir, shape_config_name), encoding='utf-8') as g:
                    js = json.load(g)
                    json_new_origin_path = js['origin_path']
                    logger.warn(f"the simplifiedKeyWithPlatform {simplified_key_with_platform} is duplicated, "
                                f"shape cfg is {json_new_origin_path} and {json_matched_orginal_path}")
            return False
    return True


def _check_depend_idx_arr_same(op_type, shape_config_name, op_desc, depend_idx_arr):
    value_depend_index = op_desc.get('valueDependIndex')
    if len(depend_idx_arr) != 0:
        if value_depend_index is None:
            raise Exception(f"error: the op type {op_type} has not value depend index yet,"
                            f"but new shape {shape_config_name} has")
        if not operator.eq(depend_idx_arr, value_depend_index):
            raise Exception(f"error: the depend index of {op_type} is {value_depend_index},"
                            f"not equal to shape {shape_config_name} : {depend_idx_arr}")
    else:
        if value_depend_index is not None:
            raise Exception(f"error: the op type {op_type} has value depend index yet,"
                            f"but new shape {shape_config_name} has not")


def _gen_input_depend_indexes(shape_config: str):
    shape_js = load_json(shape_config)
    inputs = shape_js.get("inputs")
    max_idx = 0
    depend_idx = []
    for desc in inputs:
        if desc is None:
            pass
        elif isinstance(desc, list):
            for single_desc in desc:
                index = single_desc.get("index")
                if index is None:
                    raise Exception(f"error: lack of input index, which is {single_desc}")
                if index < max_idx:
                    raise Exception(f"error: input tensors are not in a ASC sequence")
                max_idx = index
                if single_desc.get("value_depend"):
                    raise Exception(f"error: dynamic input not support value depend mode")
                break
        else:
            index = desc.get("index")
            if index is None:
                raise Exception(f"error: lack of input index, which is {desc}")
            if index < max_idx:
                raise Exception(f"error: input tensors are not in a ASC sequence")
            max_idx = index
            if desc.get("value_depend"):
                depend_idx.append(index)
    return depend_idx


def _patch_up_support_info(shape_cfg, obj_cfg, object_json_path):
    if obj_cfg.get('supportInfo') is not None:
        return
    info = {}
    cp_map = {'impl_mode': 'implMode', 'deterministic':'deterministic', 'inputs':'inputs', 'outputs':'outputs',\
              'attrs':'attrs'}
    for cp_item in cp_map.keys():
        if shape_cfg.get(cp_item):
            info[cp_map.get(cp_item)] = shape_cfg.get(cp_item)

    obj_cfg['supportInfo'] = info
    with open(object_json_path, 'w') as file_path:
        json.dump(obj_cfg, file_path, sort_keys=True, indent=2, separators=(',', ':'))


def gen_binary_info_config(package_name, soc_version, shape_config_dir, object_dir, binary_info_config_path, enable_sk):
    """
    :param soc_version: 芯片名代号
    :param shape_config_dir: dump出的shape的文件夹
    :param object_dir: aoe生成的.o/.json目录
    :param binary_info_config_path: 生成的binary_info_config.json的path
    :return: 无
    """
    shape_compile_res_list = _get_shape_compile_res_list(shape_config_dir, object_dir)
    if len(shape_compile_res_list) == 0:
        logger.warn(f"static compilation tool failed to compile all the operator jsons, "
                    f"please check ascend/log/debug/plog/plog-pid-*.log")
        return False
    simplified_key_json_match = {}
    ops_desc = {}
    for shape_res in shape_compile_res_list:
        shape_config_path = os.path.join(shape_config_dir, shape_res.shape_config_name)
        object_json_path = os.path.join(shape_res.object_path, '{}.json'.format(shape_res.object_name))
        shape_cfg = load_json(shape_config_path)
        obj_cfg = load_json(object_json_path)

        op_type = shape_cfg.get('op_type')
        if op_type is None:
            raise Exception(f"error: cannot find op type in shape config: {shape_res.shape_config_name}")
        simplified_key, simplified_key_with_platform = gen_simplified_key(shape_config_path, enable_sk, soc_version)
        depend_idx_arr = _gen_input_depend_indexes(shape_config_path)
        bin_path = os.path.join(package_name, soc_version, op_type, '{}.o'.format(shape_res.object_name))

        op_desc = ops_desc.get(op_type)
        if op_desc is not None:
            #has
            static_list = op_desc.get('staticList')
            ret = _check_simplified_key_not_dup(simplified_key_json_match, static_list, shape_res.shape_config_name, \
                simplified_key_with_platform, shape_config_dir)
            if not ret:
                continue
            _check_depend_idx_arr_same(op_type, shape_res.shape_config_name, op_desc, depend_idx_arr)
            one_obj = _gen_one_obj(obj_cfg, [simplified_key, simplified_key_with_platform], \
                shape_res.object_name, bin_path, depend_idx_arr)
            static_list.append(one_obj)
        else:
            #has not
            op_desc = {}
            op_desc['simplifiedKeyMode'] = 0
            if len(depend_idx_arr) != 0:
                op_desc['valueDependIndex'] = depend_idx_arr
            one_obj = _gen_one_obj(obj_cfg, [simplified_key, simplified_key_with_platform], \
                shape_res.object_name, bin_path, depend_idx_arr)
            op_desc['staticList'] = [one_obj]
            ops_desc[op_type] = op_desc
        simplified_key_json_match[simplified_key_with_platform] = shape_res.shape_config_name
        # aoe mode will not generate support info, use input json to patch up
        _patch_up_support_info(shape_cfg, obj_cfg, object_json_path)
    wr_flag = os.O_WRONLY | os.O_CREAT
    wr_mode = stat.S_IWUSR | stat.S_IRUSR
    output_config_rpath = os.path.realpath(binary_info_config_path)
    with open(output_config_rpath, 'w') as file_path:
        json.dump(ops_desc, file_path, sort_keys=True, indent=2, separators=(',', ':'))
    return True


if __name__ == "__main__":
    SHAPE_CONFIG_DIR_TEST = './path_demo'
    OBJECT_DIR_TEST = './tmp'
    BINARY_INFO_CONFIG_TEST = './binary_config.json'
    SOC_VERSION_TEST = 'ascend910b'
    PACKAGE_NAME = '123pkg'
    gen_binary_info_config(PACKAGE_NAME, SOC_VERSION_TEST, SHAPE_CONFIG_DIR_TEST,
        OBJECT_DIR_TEST, BINARY_INFO_CONFIG_TEST)
