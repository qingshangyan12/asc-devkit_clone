#!/usr/bin/python3
# coding=utf-8

# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import os
import sys
import logging

logging.basicConfig(level=logging.INFO, stream=sys.stdout)


def file_filter(path):
    """过滤不需要触发测试的文件"""
    key_words = ['.md', '.json', '.ini', '.yaml', '.txt', 'examples', 'docs']
    for key_word in key_words:
        if key_word in path:
            return False
    return True


class ChangedFileParser:
    """解析修改的文件，返回需要执行的测试目标"""

    def __init__(self, base_path):
        self.base_path = base_path
        self.test_targets = set()

        # SOC 版本映射：目录名 -> 测试目标后缀列表
        self.soc_version_map = {
            'dav_c100': ['ascend910'],
            'dav_c220': ['ascend910B1_AIC', 'ascend910B1_AIV'],
            'dav_c310': ['ascend950pr_9599_AIC', 'ascend950pr_9599_AIV'],
            'dav_l300': ['ascend310p'],
            'dav_l311': ['ascend310B1'],
            'dav_m200': ['ascend610'],
            'dav_m510': ['ascend950pr_9599_AIC', 'ascend950pr_9599_AIV_BASIC',
                        'ascend950pr_9599_AIV_FRAMEWORK', 'ascend950pr_9599_AIV_MICRO',
                        'ascend950pr_9599_AIV_SIMT'],
        }

        # 基础 API SOC 映射（测试目录名 -> 测试目标后缀列表）
        self.basic_api_soc_map = {
            'ascendc_case_ascend910': ['ascend910'],
            'ascendc_case_ascend310p': ['ascend310p'],
            'ascendc_case_ascend610': ['ascend610'],
            'ascendc_case_ascend610lite': ['ascend610Lite'],
            'ascendc_case_ascend310310B1': ['ascend310B1'],
            'ascendc_case_ascend910B1': ['ascend910B1_AIC', 'ascend910B1_AIV', 'ascend910B1_AIV_MSTX'],
            'ascendc_case_ascend950pr_9599': ['ascend950pr_9599_AIC', 'ascend950pr_9599_AIV'],
            'ascendc_case_mc62cm12aa': ['mc62cm12aa_AIC', 'mc62cm12aa_BASIC',
                                      'mc62cm12aa_FRAMEWORK', 'mc62cm12aa_MICRO', 'mc62cm12aa_SIMT'],
        }

    def match_impl(self, parts):
        """匹配 impl 目录下的文件"""
        if len(parts) < 2:
            return

        impl_type = parts[1]

        # 基础 API 实现
        if impl_type == 'basic_api':
            if len(parts) >= 3 and parts[2] in self.soc_version_map:
                for soc in self.soc_version_map[parts[2]]:
                    self.test_targets.add(f'ascendc_ut_basic_api_{soc}')
        # 高级 API kernel 实现
        elif impl_type == 'adv_api' and len(parts) >= 3:
            if parts[2] == 'detail':
                # 所有 detail 下的库都触发 kernel 测试
                for soc in ['ascend310p', 'ascend610', 'ascend310B1', 'ascend910B1_AIC', 'ascend910B1_AIV',
                           'ascend950pr_9599_AIC', 'ascend950pr_9599_AIV']:
                    self.test_targets.add(f'ascendc_ut_adv_api_kernel_{soc}')
            elif parts[2] == 'tiling':
                # tiling 实现
                for soc in ['ascend310p', 'ascend610', 'ascend310B1', 'ascend910B1_AIC', 'ascend910B1_AIV',
                           'ascend950pr_9599_AIC', 'ascend950pr_9599_AIV']:
                    self.test_targets.add(f'ascendc_ut_adv_api_tiling_{soc}')
        # C API 实现
        elif impl_type == 'c_api':
            for soc in ['ascend910B1_AIC', 'ascend910B1_AIV', 'ascend950pr_9599_AIC', 'ascend950pr_9599_AIV']:
                self.test_targets.add(f'ascendc_ut_c_api_{soc}')
        # 工具类实现
        elif impl_type == 'utils':
            for soc in ['ascend910', 'ascend310p', 'ascend610', 'ascend310B1', 'ascend610Lite',
                'ascend910B1_AIC', 'ascend910B1_AIV', 'ascend950pr_9599_AIC', 'ascend950pr_9599_AIV_BASIC',
                'ascend950pr_9599_AIV_FRAMEWORK', 'ascend950pr_9599_AIV_MICRO', 'ascend950pr_9599_AIV_SIMT',
                'mc62cm12aa_AIC', 'mc62cm12aa_BASIC', 'mc62cm12aa_FRAMEWORK', 'mc62cm12aa_MICRO', 'mc62cm12aa_SIMT']:
                self.test_targets.add(f'ascendc_ut_tiling_utils_{soc}')
        # Micro API 实现
        elif impl_type == 'micro_api':
            self.test_targets.add('ascendc_ut_micro_api_ascend950pr_9599')
        # Simt API 实现
        elif impl_type == 'simt_api':
            self.test_targets.add('ascendc_ut_simt_api_ascend950pr_9599')

    @classmethod
    def match_tests(self, parts):
        """匹配 tests 目录下的文件"""
        if len(parts) < 2:
            return
        test_type = parts[1]

        # API 测试
        api_type = parts[2]

        # 基础 API 测试
        if api_type == 'basic_api':
            if len(parts) >= 4 and parts[3] in self.basic_api_soc_map:
                for soc in self.basic_api_soc_map[parts[3]]:
                    self.test_targets.add(f'ascendc_ut_basic_api_{soc}')
        # 高级 API kernel 测试
        elif api_type == 'adv_api' and len(parts) >= 4:
            if parts[3] in ['activation', 'math', 'matmul', 'normalization', 'conv', 'conv_backprop',
                            'filter', 'index', 'pad', 'quantization', 'reduce', 'select', 'sort',
                            'transpose', 'utils']:
                for soc in ['ascend310p', 'ascend610', 'ascend310B1', 'ascend910B1_AIC', 'ascend910B1_AIV',
                    'ascend950pr_9599_AIC', 'ascend950pr_9599_AIV']:
                    self.test_targets.add(f'ascendc_ut_adv_api_kernel_{soc}')
            elif parts[3] == 'tiling':
                for soc in ['ascend310p', 'ascend610', 'ascend310B1', 'ascend910B1_AIC', 'ascend910B1_AIV',
                    'ascend950pr_9599_AIC', 'ascend950pr_9599_AIV']:
                    self.test_targets.add(f'ascendc_ut_adv_api_tiling_{soc}')
        # C API 测试
        elif api_type == 'c_api':
            for soc in ['ascend910B1_AIC', 'ascend910B1_AIV', 'ascend950pr_9599_AIC', 'ascend950pr_9599_AIV']:
                self.test_targets.add(f'ascendc_ut_c_api_{soc}')
        # Tensor API 测试
        elif api_type == 'tensor_api':
            for soc in ['NPU_ARCH_2201_AIC', 'NPU_ARCH_2201_AIV']:
                self.test_targets.add(f'ascendc_ut_tensor_api_{soc}')
        # 工具类测试
        elif api_type == 'utils':
            for soc in ['ascend910', 'ascend310p', 'ascend610', 'ascend310B1', 'ascend610Lite',
                'ascend910B1_AIC', 'ascend910B1_AIV', 'ascend950pr_9599_AIC',
                'ascend950pr_9599_AIV_BASIC', 'ascend950pr_9599_AIV_FRAMEWORK',
                'ascend950pr_9599_AIV_MICRO', 'ascend950pr_9599_AIV_SIMT',
                'mc62cm12aa_AIC', 'mc62cm12aa_BASIC', 'mc62cm12aa_FRAMEWORK',
                'mc62cm12aa_MICRO', 'mc62cm12aa_SIMT']:
                self.test_targets.add(f'ascendc_ut_tiling_utils_{soc}')
        # Micro API 测试
        elif api_type == 'micro_api':
            self.test_targets.add('ascendc_ut_micro_api_ascend950pr_9599')
        # Simt API 测试
        elif api_type == 'simt_api':
            self.test_targets.add('ascendc_ut_simt_api_ascend950pr_9599')

    @classmethod
    def match_tools(self, parts):
        """匹配 tools 目录下的文件"""
        # 工具代码修改触发相关测试
        self.test_targets.add('ascendc_ut_aclrtc')
        self.test_targets.add('ascendc_ut_asc_compile_base')
        self.test_targets.add('ascendc_ut_asc_runtime')
        self.test_targets.add('ascendc_ut_elf_tool')
        self.test_targets.add('ascendc_ut_pack_kernel')
        self.test_targets.add('ascendc_ut_tpl_tiling_debug')
        self.test_targets.add('ascendc_ut_tpl_tiling_release')
        self.test_targets.add('ascendc_ut_aicpu_api')
        self.test_targets.add('ascendc_run_all_header_checks')

    def parse_file(self, file_path):
        """解析单个文件路径，添加对应的测试目标"""
        if not os.path.exists(file_path):
            return

        if not file_filter(file_path):
            return

        # 转换为相对路径
        try:
            rel_path = os.path.relpath(file_path, self.base_path)
        except ValueError:
            return

        parts = rel_path.split(os.sep)

        # 匹配实现代码
        if parts[0] == 'impl':
            self.match_impl(parts)
        # 匹配测试代码
        elif parts[0] == 'tests':
            self.match_tests(parts)
        # 匹配工具代码
        elif parts[0] == 'tools':
            self.match_tools(parts)

    def get_test_targets(self):
        """获取测试目标列表"""
        return sorted(list(self.test_targets))


def main():
    if len(sys.argv) < 3:
        logging.error("Usage: python3 parse_changed_files.py <changed_files>... <base_path>")
        sys.exit(1)

    changed_files_arg = sys.argv[1]
    base_path = sys.argv[2]

    # 读取修改的文件列表
    if os.path.isfile(changed_files_arg) and changed_files_arg.endswith('.txt'):
        with open(changed_files_arg, 'r') as f:
            changed_files = [line.strip() for line in f.readlines() if line.strip()]
    elif ',' in changed_files_arg:
        changed_files = [f.strip() for f in changed_files_arg.split(',') if f.strip()]
    else:
        changed_files = [changed_files_arg.strip()]

    if not changed_files:
        logging.warning("No changed files provided")
        sys.exit(0)

    parser = ChangedFileParser(base_path)

    for file_path in changed_files:
        parser.parse_file(file_path)

    test_targets = parser.get_test_targets()

    if not test_targets:
        logging.warning("No test targets found for changed files")
        sys.exit(0)

    # 输出空格分隔的测试目标
    output_traget = ' '.join(test_targets)
    logging.info(output_traget)


if __name__ == '__main__':
    main()
