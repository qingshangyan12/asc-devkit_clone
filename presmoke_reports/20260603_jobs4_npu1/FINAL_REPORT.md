# Presmoke Overnight Report

- root: `/tmp/presmoke2_overnight_20260603_jobs4_npu1`
- generated_at: `2026-06-03T02:01:21+08:00`

## Primary Full Run

- run: `full_card1`
- card: `1`
- jobs/npu_slots: `4/1`
- timeout: `1200`
- elapsed_sec: `3058`
- planned_results: `172`
- pass/fail/skip: `130/42/0`

## Failures

| example | status | reason | rc | duration_s | failing_step | retry |
|---|---:|---|---:|---:|---|---|
| 01_simd_cpp_api/02_features/00_compilation/parallel_ops_package | FAIL | nonzero rc | 1 | 0.0 | cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/01_invocation/aclop_invocation | FAIL | nonzero rc | 255 | 0.5 | atc --singleop=../add_custom.json --output=. --soc_version=${soc_version} |  |
| 01_simd_cpp_api/02_features/02_framework/01_tensorflow/tensorflow_custom | FAIL | nonzero rc | 1 | 24.0 | python3 ../run_add_custom_tf.py |  |
| 01_simd_cpp_api/02_features/02_framework/02_onnx/onnx_plugin | FAIL | nonzero rc | 255 | 0.8 | atc --model=./leaky_relu.onnx --framework=5 --soc_version=${soc_version} --output=./leaky_relu_custom --input_shape="X:8,16,1024" --input_format=ND |  |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/copy_ub2ub | FAIL | nonzero rc | 2 | 6.7 | make -j |  |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_gm2l1 | FAIL | nonzero rc | 2 | 7.1 | make -j |  |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_l0c2gm | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2gm | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_l0c2l1 | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_l12l0 | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2 | FAIL | nonzero rc | 1 | 0.0 | cmake .. '-DSCENARIO_NUM=$SCENARIO' '-DM_SIZE=$M' '-DK_SIZE=$K' '-DN_SIZE=$N' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag | FAIL | nonzero rc | 1 | 13.7 | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/block_reduce_min_max_sum | FAIL | nonzero rc | 2 | 6.8 | make -j |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/cast | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_compound_compute | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/element_wise_logic | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort | FAIL | nonzero rc | 2 | 6.9 | make -j |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/select | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/transpose | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum | FAIL | nonzero rc | 2 | 6.7 | make -j |  |
| 01_simd_cpp_api/02_features/03_basic_api/04_resource_management/tbufpool_management | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/cross_core_set_wait_flag | FAIL | nonzero rc | 2 | 7.0 | make -j |  |
| 01_simd_cpp_api/02_features/03_basic_api/06_sync_control/sync_all | FAIL | nonzero rc | 1 | 1103.9 | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |  |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/data_movement_with_atomic_operations | FAIL | nonzero rc | 1 | 0.3 | cmake .. '-DSCENARIO_NUM=$SCENARIO_NUM' -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_deterministic_computation | FAIL | nonzero rc | 2 | 7.0 | make -j |  |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_async_iterate | FAIL | nonzero rc | 1 | 1126.7 | python3 ../scripts/verify_result.py output/output.bin output/golden.bin |  |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_fp8 | FAIL | nonzero rc | 2 | 11.0 | make -j |  |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareAB | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/03_libraries/00_matrix/matmul_ibshareB | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/03_libraries/08_tensor_transformation/transdata | FAIL | nonzero rc | 2 | 10.3 | make -j |  |
| 01_simd_cpp_api/03_libraries/12_math/clampmin | FAIL | nonzero rc | 2 | 10.5 | make -j |  |
| 01_simd_cpp_api/03_libraries/12_math/power | FAIL | nonzero rc | 2 | 10.3 | make -j |  |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_basic_api_high_performance | FAIL | nonzero rc | 2 | 6.4 | make -j |  |
| 01_simd_cpp_api/04_best_practices/01_matrix_compute_practices/matmul_high_performance | FAIL | nonzero rc | 2 | 11.7 | make -j |  |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/matmul_gelu_high_performance | FAIL | nonzero rc | 2 | 6.8 | make -j |  |
| 01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/quant_group_matmul_high_performance | FAIL | nonzero rc | 1 | 0.0 | cmake '-DSCENARIO_NUM=$SCENARIO' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |
| 01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy | FAIL | nonzero rc | 1 | 0.3 | cmake '-DSCENARIO_NUM=$SCENARIO_NUM' '-DCOPY_DST=$COPY_DST' .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 |  |

## Artifacts

- all case timings: `/tmp/presmoke2_overnight_20260603_jobs4_npu1/ALL_CASE_TIMINGS.tsv`
- failures tsv: `/tmp/presmoke2_overnight_20260603_jobs4_npu1/FAILURES.tsv`
- primary JSON: `/tmp/presmoke2_overnight_20260603_jobs4_npu1/full_card1/results/report.json`
- primary Markdown: `/tmp/presmoke2_overnight_20260603_jobs4_npu1/full_card1/results/report.md`
