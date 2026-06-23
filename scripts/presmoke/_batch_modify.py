#!/usr/bin/env python3
"""Batch modify run.sh scripts to add multi-scenario iteration."""
import re
import os

PRESMOKE_ROOT = os.path.dirname(os.path.abspath(__file__))
CASES_DIR = os.path.join(PRESMOKE_ROOT, 'cases')

# Each entry: (rel_path, scenario_list_fn, gen_data_arg, verify_arg, cmake_var, special_env, binary_name, no_gen_data)
# rel_path: path from cases/ dir to the run.sh
# scenario_list_fn: function body (without the function wrapper), or None for no-scenario skip 
# gen_data_arg: the argument to pass to gen_data.py (e.g. '-scenarioNum=$SCENARIO_NUM'). None means no gen_data call
# verify_arg: the arguments to pass to verify_result.py. None means no verify. ':' means no-op verify
# cmake_var: the cmake variable name (default 'SCENARIO_NUM')
# special_env: dict of extra env vars to pass
# binary_name: the binary to run (default './demo')
# no_gen_data: True if gen_data.py doesn't exist or takes no args

FILES = [
  # ========== 00_data_movement ==========
   ("01_simd_cpp_api/03_basic_api/00_data_movement/copy_in_tensor_api/run.sh",
    'echo "0 1 2 3"',
    '${SCENARIO_NUM}',  # positional arg
    'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),
  
   ("01_simd_cpp_api/03_basic_api/00_data_movement/copy_out_tensor_api/run.sh",
    'echo "0 1 2"',
    None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),  # gen_data takes no args

   ("01_simd_cpp_api/03_basic_api/00_data_movement/copy_ub2ub/run.sh",
    'echo "1 2 3"',
    '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2ub_nddma/run.sh",
   'echo "1 2 3 4 5"',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm/run.sh",
   'if [[ "$ARCH" == "dav-3510" ]]; then echo "1 2 3 4 5 6"; else echo "1 2"; fi',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin -scenarioNum=$SCENARIO_NUM', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_mask/run.sh",
   'echo "1 2"',
   '-scenarioNum=$SCENARIO_NUM', ':', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_unalign/run.sh",
   'echo "1 2"',
   '-scenarioNum=$SCENARIO_NUM', ':', 'SCENARIO_NUM', {}, None, False),

  # ========== 01_memory_vector_compute ==========
   ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/cast/run.sh",
    'echo "0 1"',
    '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/compare/run.sh",
   'if [[ "$ARCH" == "dav-3510" ]]; then echo "1 2 3 4"; else echo "1 2 3"; fi',
   '-scenario_num=$SCENARIO_NUM', '-scenario_num=$SCENARIO_NUM ./output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

   ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_compound_compute/run.sh",
    'echo "1 2"',
    '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_logic/run.sh",
   'if [[ "$ARCH" == "dav-3510" ]]; then echo "1 2 3 4"; else echo "1 4"; fi',
   '-scenarioNum=$SCENARIO_NUM', './output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/gather/run.sh",
   'echo "1 2 3 4"',
   '-scenario_num=$SCENARIO_NUM', '$SCENARIO_NUM ./output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/interleave_pair/run.sh",
   'echo "1 2"',
   '-scenarioNum=$SCENARIO_NUM',
   './output/output_dst0.bin ./output/output_dst1.bin ./output/golden_dst0.bin ./output/golden_dst1.bin',
   'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/mrg_sort/run.sh",
   'echo "1 2 3"',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/reduce_computation/run.sh",
   'echo "1 2 3 4 5 6"',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/reduce_data_block/run.sh",
   'echo "1 2 3"',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM ./output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/reduce_repeat/run.sh",
   'echo "1 2 3 4"',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM ./output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

   ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/select/run.sh",
    'if [[ "$ARCH" == "dav-3510" ]]; then echo "1 2 3 4"; else echo "1 2 3"; fi',
    '-scenario_num=$SCENARIO_NUM', './output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

   ("01_simd_cpp_api/03_basic_api/01_memory_vector_compute/transpose/run.sh",
    'echo "1 2 3"',
    '-scenario_num=$SCENARIO_NUM', './output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  # ========== 02_reg_vector_compute ==========
  ("01_simd_cpp_api/03_basic_api/02_reg_vector_compute/cast/run.sh",
   'echo "1 2"',
   '-scenarioNum=$SCENARIO_NUM', ':', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/02_reg_vector_compute/compare/run.sh",
   'echo "1 2"',
   '-scenarioNum=$SCENARIO_NUM', ':', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/02_reg_vector_compute/data_relayout/run.sh",
   'echo "1 2"',
   '-scenarioNum $SCENARIO_NUM', ':', 'SCENARIO_NUM', {}, None, False),  # space separator

  ("01_simd_cpp_api/03_basic_api/02_reg_vector_compute/subnormal/run.sh",
   'echo "1 2"',
   None, ':', 'SCENARIO_NUM', {}, None, True),  # gen_data no args

  ("01_simd_cpp_api/03_basic_api/02_reg_vector_compute/trait/run.sh",
   'echo "1 2"',
   None, ':', 'SCENARIO_NUM', {}, None, True),  # gen_data no args

  # ========== 03_matrix_compute ==========
  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2gm/run.sh",
   'if [[ "$ARCH" == "dav-3510" ]]; then echo "1 2 3 4 5 6 7"; else echo "1 2 4 5 6 7"; fi',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2l1/run.sh",
   'echo "1 2 3 4"',
   '-scenarioNum=$SCENARIO_NUM', '$SCENARIO_NUM output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2ub/run.sh",
   'echo "1 2 3 4"',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_2dmx_l12l0/run.sh",
   'echo "1 2 3 4 5 6"',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_2dv2_l12l0/run.sh",
   'echo "1 2 3 4 5 6 7"',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_l12l0/run.sh",
   'echo "1 2 3 4 5 6 7 8 9 10 11 12 13 14"',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad/run.sh",
   'if [[ "$ARCH" == "dav-2201" ]]; then echo "1 2 3 4"; else echo "1 2 3"; fi',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_load3dv2/run.sh",
   'echo "1 2 3 4 5"',
   '-scenarioNum=$SCENARIO_NUM', '-scenarioNum=$SCENARIO_NUM output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_mx/run.sh",
   'echo "1 2 3 4"',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  # ========== 04_memory_management ==========
  ("01_simd_cpp_api/03_basic_api/04_memory_management/tbufpool_management/run.sh",
   'echo "1 2 3"',
   '-scenarioNum=$SCENARIO_NUM', './output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  # ========== 05_sync_control ==========
  ("01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag/run.sh",
   'echo "0 1 2"',
   '-scenarioNum $SCENARIO_NUM', './output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, False),  # space separator

  ("01_simd_cpp_api/03_basic_api/05_sync_control/reg_sync/run.sh",
   'echo "1 2"',
   '-scenarioNum=$SCENARIO_NUM', ':', 'SCENARIO_NUM', {}, None, False),

  # ========== 06_atomic ==========
  ("01_simd_cpp_api/03_basic_api/06_atomic/data_movement_with_atomic_operations/run.sh",
   'echo "1 2"',
   '-scenarioNum $SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),  # space separator

  ("01_simd_cpp_api/03_basic_api/06_atomic/scalar_atomic_operations/run.sh",
   'echo "1 2"',
   '-scenarioNum $SCENARIO_NUM', '-scenarioNum $SCENARIO_NUM output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  # ========== 09_utils ==========
  ("01_simd_cpp_api/03_basic_api/09_utils/get_ub_size/run.sh",
   'echo "1 2"',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  # ========== 04_advanced_api/00_matmul ==========
  ("01_simd_cpp_api/04_advanced_api/00_matmul/matmul_fp8/run.sh",
   'echo "0 1 2 3 4"',
   '-scenarioNum=$SCENARIO_NUM', 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, False),

  ("01_simd_cpp_api/04_advanced_api/00_matmul/matmul_ibshareAB/run.sh",
   'echo "1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),  # gen_data no args

  ("01_simd_cpp_api/04_advanced_api/00_matmul/matmul_ibshareB/run.sh",
   'echo "1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),  # gen_data no args

  # ========== 04_advanced_api/08_transpose ==========
  ("01_simd_cpp_api/04_advanced_api/08_transpose/transdata/run.sh",
   'echo "1 2 3 4"',
   None, ':', 'SCENARIO', {}, None, True),  # uses SCENARIO, not SCENARIO_NUM

  # ========== 04_advanced_api/10_math ==========
  ("01_simd_cpp_api/04_advanced_api/10_math/clampmin/run.sh",
   'echo "0 1"',
   None, ':', 'SCENARIO', {}, None, True),  # uses SCENARIO, not SCENARIO_NUM

  ("01_simd_cpp_api/04_advanced_api/10_math/power/run.sh",
   'echo "0 1 2"',
   None, ':', 'SCENARIO', {}, None, True),  # uses SCENARIO, not SCENARIO_NUM

  # ========== 05_best_practices ==========
  ("01_simd_cpp_api/05_best_practices/00_vector_compute/add_high_performance/run.sh",
   'echo "0 1 2 3 4 5 6"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/01_matrix_compute/matmul_basic_api_high_performance/run.sh",
   'echo "1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/01_matrix_compute/matmul_high_performance/run.sh",
   'echo "0 1 2 3 4 5 6 7 8"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/01_matrix_compute/matmul_mxfp4_high_performance/run.sh",
   'echo "1 2"',
   None, './output/output.bin ./output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_eltwise_high_performance/run.sh",
   'echo "0 1 2 3"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_high_performance/run.sh",
   'echo "0 1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {'CMAKE_VF_MODE': 'false'}, None, True),

  ("01_simd_cpp_api/05_best_practices/03_fusion_compute/matmul_gelu_high_performance/run.sh",
   'echo "1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/03_fusion_compute/quant_group_matmul_high_performance/run.sh",
   'echo "0 1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/04_memory_access/bank_conflict_nd2nz/run.sh",
   'echo "1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("01_simd_cpp_api/05_best_practices/04_memory_access/data_copy/run.sh",
   'echo "1 2 3 4 5 6 7 8"',
   '-scenarioNum $SCENARIO_NUM -copyDst $COPY_DST -arch $ASC_ARCH', ':', 'SCENARIO_NUM',
   {'ASC_ARCH': '$ARCH', 'COPY_DST': 'UB'}, None, False),

  # ========== 06_compatibility_guide ==========
  ("01_simd_cpp_api/06_compatibility_guide/pattern_transformation/run.sh",
   'echo "1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  # ========== 03_simt_api ==========
  ("03_simt_api/02_features/01_api_features/01_sync_instruction/memory_fence/run.sh",
   'echo "1 2"',
   None, ':', 'SCENARIO_NUM', {}, None, True),  # no gen_data.py

  ("03_simt_api/03_best_practices/00_memory_optimizations/cache_hint/run.sh",
   'echo "0 1"',
   None, ':', 'SCENARIO_NUM', {}, 'data_cache_hint', True),

  ("03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice/run.sh",
   'echo "0 1 2 3"',
   None, ':', 'SCENARIO_NUM', {}, 'matrix_transpose', True),

  ("03_simt_api/03_best_practices/00_memory_optimizations/short_vector_add/run.sh",
   'echo "1 2"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

   ("03_simt_api/03_best_practices/01_execution_conf_optimizations/grid_dim_config/run.sh",
    'echo "1 2 3 4 5 6 7 8 9 10 11 12 13"',
    None, ':', 'SCENARIO_NUM', {}, 'grid_config', True),

  # ========== 05_simd_simt_hybrid ==========
  ("05_simd_simt_hybrid/02_best_practices/simd_simt_grid_dim_config/run.sh",
   'echo "1"',
   None, ':', 'SCENARIO_NUM', {}, 'grid_config', True),

  ("05_simd_simt_hybrid/02_best_practices/simd_simt_high_performance/run.sh",
   'echo "0 1 2 3"',
   None, 'output/output.bin output/golden.bin', 'SCENARIO_NUM', {}, None, True),

  ("05_simd_simt_hybrid/02_best_practices/simd_simt_integer_fast_div/run.sh",
   'echo "0 1"',
   None, ':', 'SCENARIO_NUM', {}, 'integer_div', True),

  ("05_simd_simt_hybrid/02_best_practices/simd_simt_matrix_transpose/run.sh",
   'echo "0 1"',
   None, ':', 'SCENARIO_NUM', {}, None, True),
]

# Filter out softmaxgrad and swish - they have no SCENARIO_NUM at all
# They are NOT in the FILES list


def generate_script_content(rel_path, scenario_list_fn, gen_data_arg, verify_arg,
                            cmake_var, special_env, binary_name, no_gen_data):
    filepath = os.path.join(CASES_DIR, rel_path)
    with open(filepath) as f:
        content = f.read()

    lines = content.split('\n')
    
    # Find the CASE_REL line
    case_rel = None
    for line in lines:
        m = re.match(r'^CASE_REL=(.+)$', line)
        if m:
            case_rel = m.group(1)
            break
    
    if not case_rel:
        print(f"ERROR: Could not find CASE_REL in {filepath}")
        return None
    
    # Find the source depth from existing file
    source_line = None
    for line in lines:
        if line.startswith('source '):
            source_line = line
            break
    
    # Determine parent dir depth (for _case_entry.sh path)
    # Count ../ in the source line
    depth = source_line.count('../') if source_line else 4
    
    # Build the new content
    new_lines = []
    
    # License header (lines before set -euo pipefail)
    in_license = True
    for line in lines:
        new_lines.append(line)
        if line.strip() == 'set -euo pipefail':
            break
    
    new_lines.append('')
    new_lines.append(f'CASE_REL={case_rel}')
    new_lines.append('SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"')
    new_lines.append(f'source "$SCRIPT_DIR/{"../" * depth}_case_entry.sh"')
    new_lines.append('presmoke_case_init "$CASE_REL"')
    new_lines.append('')
    
    # get_scenario_list
    new_lines.append('get_scenario_list() {')
    for line in scenario_list_fn.split('\n'):
        new_lines.append(f'    {line}')
    new_lines.append('}')
    new_lines.append('')
    
    # get_build_subdir
    new_lines.append('get_build_subdir() {')
    new_lines.append('    local scenario=$1')
    new_lines.append('    echo "${BUILD_DIR}_scenario${scenario}"')
    new_lines.append('}')
    new_lines.append('')
    
    # case_build
    new_lines.append('case_build() {')
    new_lines.append('    for scenario in $(get_scenario_list); do')
    new_lines.append('        local scenario_build_dir')
    new_lines.append('        scenario_build_dir="$(get_build_subdir "$scenario")"')
    new_lines.append('        mkdir -p "$scenario_build_dir"')
    
    # cmake command
    env_prefix = build_env_prefix(special_env, cmake_var)
    if cmake_var == 'SCENARIO':
        cmake_def = f'-DSCENARIO=$SCENARIO'
    else:
        cmake_def = f'-D{cmake_var}=${cmake_var}'
    
    if 'data_copy' in rel_path and 'best_practices' in rel_path:
        # special case for data_copy (best_practices)
        new_lines.append(f'        (cd "$scenario_build_dir" && {env_prefix} bash -lc \'cmake {cmake_def} -DCOPY_DST=$COPY_DST .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG\')')
    else:
        cmake_order = f'{cmake_def} ..'
        # Check if original uses `cmake .. -DSCENARIO_NUM=` order or `cmake -DSCENARIO_NUM= ..`
        # We'll use the more common `cmake -DSCENARIO_NUM=$SCENARIO_NUM ..` pattern
        if cmake_var == 'SCENARIO':
            new_lines.append(f'        (cd "$scenario_build_dir" && {env_prefix} bash -lc \'cmake .. {cmake_def} -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG\')')
        else:
            new_lines.append(f'        (cd "$scenario_build_dir" && {env_prefix} bash -lc \'cmake {cmake_def} .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG\')')
    
    # make
    new_lines.append(f'        (cd "$scenario_build_dir" && {env_prefix} bash -lc \'make -j\')')
    
    # gen_data
    if no_gen_data:
        # Check if original calls gen_data.py
        # Search in original lines for gen_data.py
        pass  # skip gen_data, it's not used
    elif gen_data_arg is not None:
        new_lines.append(f'        (cd "$scenario_build_dir" && {env_prefix} bash -lc \'python3 ../scripts/gen_data.py {gen_data_arg}\')')
    
    new_lines.append('    done')
    new_lines.append('}')
    new_lines.append('')
    
    # case_run
    new_lines.append('case_run() {')
    new_lines.append('    for scenario in $(get_scenario_list); do')
    new_lines.append('        local scenario_build_dir')
    new_lines.append('        scenario_build_dir="$(get_build_subdir "$scenario")"')
    new_lines.append('        mkdir -p "$scenario_build_dir"')
    bin_name = binary_name or './demo'
    new_lines.append(f'        (cd "$scenario_build_dir" && {env_prefix} bash -lc {bin_name})')
    new_lines.append('    done')
    new_lines.append('}')
    new_lines.append('')
    
    # case_verify
    new_lines.append('case_verify() {')
    if verify_arg == ':':
        new_lines.append('    for scenario in $(get_scenario_list); do')
        new_lines.append('        local scenario_build_dir')
        new_lines.append('        scenario_build_dir="$(get_build_subdir "$scenario")"')
        new_lines.append('        mkdir -p "$scenario_build_dir"')
        new_lines.append('        :')
        new_lines.append('    done')
    elif verify_arg is not None:
        new_lines.append('    for scenario in $(get_scenario_list); do')
        new_lines.append('        local scenario_build_dir')
        new_lines.append('        scenario_build_dir="$(get_build_subdir "$scenario")"')
        new_lines.append('        mkdir -p "$scenario_build_dir"')
        new_lines.append(f'        (cd "$scenario_build_dir" && {env_prefix} bash -lc \'python3 ../scripts/verify_result.py {verify_arg}\')')
        new_lines.append('    done')
    else:
        # No verify - use `:` as no-op
        new_lines.append('    for scenario in $(get_scenario_list); do')
        new_lines.append('        local scenario_build_dir')
        new_lines.append('        scenario_build_dir="$(get_build_subdir "$scenario")"')
        new_lines.append('        mkdir -p "$scenario_build_dir"')
        new_lines.append('        :')
        new_lines.append('    done')
    new_lines.append('}')
    new_lines.append('')
    
    # case_clean
    new_lines.append('case_clean() {')
    new_lines.append('    for scenario in $(get_scenario_list); do')
    new_lines.append('        rm -rf "$(get_build_subdir "$scenario")"')
    new_lines.append('    done')
    new_lines.append('    presmoke_default_clean')
    new_lines.append('}')
    new_lines.append('')
    
    new_lines.append('presmoke_case_main "$@"')
    new_lines.append('')
    
    return '\n'.join(new_lines)


def build_env_prefix(special_env, cmake_var):
    parts = []
    for k, v in special_env.items():
        parts.append(f'{k}={v}')
    parts.append(f'{cmake_var}=$scenario')
    parts.append('soc_version=$SOC_VERSION')
    return ' '.join(parts)


def main():
    errors = []
    success = 0
    skipped = []
    
    for entry in FILES:
        rel_path = entry[0]
        filepath = os.path.join(CASES_DIR, rel_path)
        
        if not os.path.exists(filepath):
            skipped.append((rel_path, "file not found"))
            continue
        
        new_content = generate_script_content(*entry)
        if new_content is None:
            errors.append(rel_path)
            continue
        
        with open(filepath, 'w') as f:
            f.write(new_content)
        success += 1
        print(f"  OK: {rel_path}")
    
    print(f"\nDone: {success} modified, {len(errors)} errors, {len(skipped)} skipped")
    if errors:
        for e in errors:
            print(f"  ERROR: {e}")
    if skipped:
        for s, reason in skipped:
            print(f"  SKIPPED: {s} ({reason})")


if __name__ == '__main__':
    main()
