#!/usr/bin/env bash
set -euo pipefail
test_dir=$(cd "$(dirname "$0")" && pwd)
sample=${1:?sample name}
scenario=${2:-1}
archive_root=${CUBE_ARCHIVE_ROOT:?set CUBE_ARCHIVE_ROOT to extracted test-cases}
sample_dir="$archive_root/$sample"
if [[ "$sample" == "mmad_mx" || "$sample" == load_data_2dmx_l12l0 ]]; then
    python3 -c 'import en_dtypes, ml_dtypes; assert hasattr(en_dtypes, "float4_e1m2")'
fi
if [[ "$sample" == load_data_l12l0 || "$sample" == "mmad_with_sparse" ]]; then
    echo "SKIP: $sample is restricted to dav-2201 by the supplied CMakeLists.txt"
    exit 77
fi
out=${CUBE_TEST_OUTPUT:?set CUBE_TEST_OUTPUT to a build/output directory}/"${sample}_${scenario}"
mkdir -p "$out/input" "$out/output"
cmake -S "$test_dir" -B "$out/build" -DSAMPLE_DIR="$sample_dir" -DSCENARIO_NUM="$scenario" -DCUBE_FIX_BIAS="${CUBE_FIX_BIAS:-OFF}" -DCUBE_TENSOR_BASELINE="${CUBE_TENSOR_BASELINE:-OFF}" > "$out/configure.log" 2>&1
cmake --build "$out/build" -j2 > "$out/build.log" 2>&1
repo=$(cd "$test_dir/../../../.." && pwd)
grep -Fq "$repo/include/kernel_operator.h" "$out/build/CMakeFiles/demo.dir/${sample}.asc.o.d"
cd "$out"
if grep -q 'add_argument.*scenarioNum' "$sample_dir/scripts/gen_data.py"; then
    python3 "$sample_dir/scripts/gen_data.py" -scenarioNum "$scenario" > generate.log 2>&1
else
    python3 "$sample_dir/scripts/gen_data.py" "scenarioNum=$scenario" > generate.log 2>&1
fi
timeout --kill-after=5s 60s "$out/build/demo" > run.log 2>&1
if grep -q 'add_argument.*scenarioNum' "$sample_dir/scripts/verify_result.py"; then
    python3 "$sample_dir/scripts/verify_result.py" -scenarioNum "$scenario" output/output.bin output/golden.bin > verify.log 2>&1
elif grep -q 'sys.argv\[3\]' "$sample_dir/scripts/verify_result.py"; then
    python3 "$sample_dir/scripts/verify_result.py" output/output.bin output/golden.bin "$scenario" > verify.log 2>&1
else
    python3 "$sample_dir/scripts/verify_result.py" output/output.bin output/golden.bin > verify.log 2>&1
fi
echo "PASS $sample scenario $scenario"
