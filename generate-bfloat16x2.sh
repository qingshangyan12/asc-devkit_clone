#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 3 ]]; then
    echo "usage: $0 MANIFEST_TSV OUTPUT_DIR SOURCE_COMMON_HEADER" >&2
    exit 2
fi

MANIFEST="$1"
OUTPUT_DIR="$2"
SOURCE_COMMON_HEADER="$3"

mkdir -p "$OUTPUT_DIR/common"
cp "$SOURCE_COMMON_HEADER" "$OUTPUT_DIR/common/asc_runtime.h"
chmod 0644 "$OUTPUT_DIR/common/asc_runtime.h"

normalize_type() {
    local value="$1"
    value="${value//const /}"
    value="${value//inline /}"
    case "$value" in
        half|half2|bfloat16x2_t|float|float2|bfloat16_t|bfloat16x2_t|bool) printf '%s' "$value" ;;
        int) printf 'int32_t' ;;
        "unsigned int") printf 'uint32_t' ;;
        "short int") printf 'int16_t' ;;
        "unsigned short int") printf 'uint16_t' ;;
        "long long int") printf 'int64_t' ;;
        "unsigned long long int") printf 'uint64_t' ;;
        "signed char") printf 'int8_t' ;;
        "unsigned char") printf 'uint8_t' ;;
        *) printf '%s' "$value" ;;
    esac
}

macro_name() {
    local value="$1"
    value="${value//[^A-Za-z0-9]/_}"
    printf '%s' "${value^^}"
}

make_cmake() {
    local api="$1" mode="$2" output_dir="$3"
    local upper mode_line
    upper="$(macro_name "$api")"
    if [[ "$mode" == "simt" ]]; then
        mode_line='set(CMAKE_ASC_ENABLE_SIMT ON)'
    else
        mode_line='# Hybrid mode uses the default SIMD/SIMT build configuration.'
    fi
    mkdir -p "$output_dir/$api/$mode"
    cat > "$output_dir/$api/$mode/CMakeLists.txt" <<EOF
cmake_minimum_required(VERSION 3.16)

set(CMAKE_ASC_ARCHITECTURES "dav-3510" CACHE STRING "NPU architecture")
$mode_line

find_package(ASC REQUIRED)
project(${api}_${mode} LANGUAGES ASC CXX)

add_executable(${api}_${mode}
    ../${api}.asc
)

target_compile_definitions(${api}_${mode} PRIVATE ${upper}_MODE_${mode^^})
target_include_directories(${api}_${mode} PRIVATE \${CMAKE_CURRENT_SOURCE_DIR}/../../common)
EOF
}

make_start() {
    local api="$1" output_dir="$2"
    cat > "$output_dir/$api/start.sh" <<EOF
#!/usr/bin/env bash
set -uo pipefail

ROOT_DIR="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"
RUN_ID="\$(date +%Y%m%d-%H%M%S)-\$\$"
LOG_DIR="\${ROOT_DIR}/logs/run-\${RUN_ID}"
mkdir -p "\${LOG_DIR}"

source /home/lishangfan/patch/pkg/cann/set_env.sh

overall_status=0
for mode in simt hybrid; do
    build_dir="\${ROOT_DIR}/\${mode}/build"
    mkdir -p "\${build_dir}"

    printf 'mode=%s\n' "\${mode}"
    printf 'command=cmake -S %s -B %s\n' "\${ROOT_DIR}/\${mode}" "\${build_dir}"
    cmake -S "\${ROOT_DIR}/\${mode}" -B "\${build_dir}" >"\${LOG_DIR}/\${mode}-configure.log" 2>&1
    configure_status=\$?
    printf 'configure_exit_code=%s\n' "\${configure_status}"
    cat "\${LOG_DIR}/\${mode}-configure.log"

    build_status=99
    run_status=99
    if [[ \${configure_status} -eq 0 ]]; then
        printf 'command=cmake --build %s\n' "\${build_dir}"
        cmake --build "\${build_dir}" -j8 >"\${LOG_DIR}/\${mode}-build.log" 2>&1
        build_status=\$?
        printf 'build_exit_code=%s\n' "\${build_status}"
        cat "\${LOG_DIR}/\${mode}-build.log"
    else
        printf 'build_skipped=true\n'
    fi

    if [[ \${configure_status} -eq 0 && \${build_status} -eq 0 ]]; then
        executable="\${build_dir}/${api}_\${mode}"
        printf 'command=%s\n' "\${executable}"
        "\${executable}" >"\${LOG_DIR}/\${mode}-run.log" 2>&1
        run_status=\$?
        printf 'run_exit_code=%s\n' "\${run_status}"
        cat "\${LOG_DIR}/\${mode}-run.log"
    else
        printf 'run_skipped=true\n'
    fi

    printf 'status=%s mode=%s configure=%s build=%s run=%s log=%s\n' \
        "\$([[ \${configure_status} -eq 0 && \${build_status} -eq 0 && \${run_status} -eq 0 ]] && echo success || echo failed)" \\
        "\${mode}" "\${configure_status}" "\${build_status}" "\${run_status}" "\${LOG_DIR}"

    if [[ \${configure_status} -ne 0 || \${build_status} -ne 0 || \${run_status} -ne 0 ]]; then
        overall_status=1
    fi
done

printf 'logs=%s\n' "\${LOG_DIR}"
exit "\${overall_status}"
EOF
    chmod 0755 "$output_dir/$api/start.sh"
}

initializer_list() {
    local api="$1" type="$2" position="$3"
    case "$type" in
        half)
            case "$api" in
                __half2float|__half22float2)
                    printf 'static_cast<half>(-2.0F), static_cast<half>(-0.5F), static_cast<half>(0.5F), static_cast<half>(2.0F), static_cast<half>(std::numeric_limits<float>::infinity()), static_cast<half>(-std::numeric_limits<float>::infinity()), static_cast<half>(std::numeric_limits<float>::quiet_NaN())'
                    ;;
                __half_as_short|__half_as_ushort)
                    printf 'static_cast<half>(0.0F), static_cast<half>(1.0F), static_cast<half>(-1.0F), static_cast<half>(std::numeric_limits<float>::infinity()), static_cast<half>(-std::numeric_limits<float>::infinity()), static_cast<half>(std::numeric_limits<float>::quiet_NaN())'
                    ;;
                __half2char_rz|__half2uchar_rz)
                    printf 'static_cast<half>(-1.5F), static_cast<half>(-0.5F), static_cast<half>(0.5F), static_cast<half>(1.5F), static_cast<half>(127.5F), static_cast<half>(std::numeric_limits<float>::infinity()), static_cast<half>(std::numeric_limits<float>::quiet_NaN())'
                    ;;
                __half2short_*)
                    printf 'static_cast<half>(-32768.0F), static_cast<half>(-2048.5F), static_cast<half>(-1.5F), static_cast<half>(0.5F), static_cast<half>(1.5F), static_cast<half>(2048.5F), static_cast<half>(std::numeric_limits<float>::infinity()), static_cast<half>(std::numeric_limits<float>::quiet_NaN())'
                    ;;
                __half2int_*|__half2uint_*|__half2ll_*|__half2ull_*)
                    printf 'static_cast<half>(-2.5F), static_cast<half>(-1.5F), static_cast<half>(0.5F), static_cast<half>(1.5F), static_cast<half>(2.5F), static_cast<half>(std::numeric_limits<float>::infinity()), static_cast<half>(-std::numeric_limits<float>::infinity()), static_cast<half>(std::numeric_limits<float>::quiet_NaN())'
                    ;;
                __half2half*)
                    printf 'static_cast<half>(-2.0F), static_cast<half>(-0.5F), static_cast<half>(0.5F), static_cast<half>(2.0F), static_cast<half>(std::numeric_limits<float>::quiet_NaN())'
                    ;;
                *)
                    printf 'static_cast<half>(-1.25F), static_cast<half>(0.5F), static_cast<half>(2.5F), static_cast<half>(-3.75F)'
                    ;;
            esac
            ;;
        half2)
            case "$api" in
                __half22float2|__high2*|__low2*|__lowhigh2highlow)
                    printf 'MakeHalf2(-2.5F, -1.5F), MakeHalf2(-0.5F, 0.5F), MakeHalf2(1.5F, 2.5F), MakeHalf2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN())'
                    ;;
                __h*b*|__heq*|__hne*|__hlt*|__hle*|__hgt*|__hge*)
                    if [[ "$position" == "x" ]]; then
                        printf 'MakeHalf2(-1.0F, 0.0F), MakeHalf2(1.0F, -1.0F), MakeHalf2(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()), MakeHalf2(std::numeric_limits<float>::quiet_NaN(), 0.5F)'
                    else
                        printf 'MakeHalf2(1.0F, 0.0F), MakeHalf2(1.0F, -2.0F), MakeHalf2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()), MakeHalf2(0.5F, std::numeric_limits<float>::quiet_NaN())'
                    fi
                    ;;
                *)
                    if [[ "$position" == "x" ]]; then
                        printf 'MakeHalf2(-1.25F, 0.5F), MakeHalf2(2.5F, -3.75F), MakeHalf2(-2.0F, 1.25F), MakeHalf2(0.75F, -0.25F)'
                    elif [[ "$position" == "y" ]]; then
                        printf 'MakeHalf2(0.5F, -2.0F), MakeHalf2(1.25F, 3.0F), MakeHalf2(1.5F, -1.0F), MakeHalf2(-0.5F, 2.25F)'
                    else
                        printf 'MakeHalf2(-0.5F, 1.0F), MakeHalf2(-2.0F, 0.75F), MakeHalf2(1.0F, -1.5F), MakeHalf2(0.25F, 2.0F)'
                    fi
                    ;;
            esac
            ;;
        float)
            case "$api" in
                __float2half*)
                    if [[ "$api" == *_sat ]]; then
                        printf '0.0F, -0.0F, 65504.0F, 65520.0F, 65536.0F, -65536.0F, std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()'
                    else
                        printf '128.0F, 128.0625F, 128.1875F, -128.1875F, std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()'
                    fi
                    ;;
                __floats2half2_rn)
                    if [[ "$position" == "x" ]]; then
                        printf '128.0F, 128.0625F, 128.1875F, std::numeric_limits<float>::infinity()'
                    else
                        printf '%s' '-128.1875F, 0.5F, 2.5F, std::numeric_limits<float>::quiet_NaN()'
                    fi
                    ;;
                *)
                    printf '-2.5F, -0.5F, 0.5F, 2.5F'
                    ;;
            esac
            ;;
        float2)
            printf 'MakeFloat2(128.0F, 128.0625F), MakeFloat2(128.1875F, -128.1875F), MakeFloat2(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()), MakeFloat2(std::numeric_limits<float>::quiet_NaN(), 65536.0F)'
            ;;
        bfloat16_t)
            bf16_initializer_list "$api"
            ;;
        bfloat16x2_t)
            bfloat16x2_initializer_list "$api" "$type" "$position"
            ;;
        int32_t)
            if [[ "$api" == *_sat ]]; then
                printf '0, 1, -1, 65504, 65535, 2147483647, -2147483648'
            else
                printf '0, 1, -1, 2049, 4097, -4097'
            fi
            ;;
        uint32_t)
            printf '0, 1, 2049, 4097, 65535'
            ;;
        int16_t)
            printf '%s' '-4096, -2049, 0, 2049, 4096'
            ;;
        uint16_t)
            printf '0, 1, 2049, 4097, 65535'
            ;;
        int64_t)
            printf '0LL, 1LL, -1LL, 2049LL, 4097LL, -4097LL'
            ;;
        uint64_t)
            printf '0ULL, 1ULL, 2049ULL, 4097ULL, 65535ULL'
            ;;
        *)
            printf '0, 1, 2, 3'
            ;;
    esac
}

output_conversion_prefix() {
    local type="$1"
    case "$type" in
        bfloat16_t) printf '__bfloat16_as_ushort' ;;
        bfloat16x2_t) printf 'reinterpret_cast<const uint32_t&>' ;;
        uint32_t) printf '' ;;
        bool) printf '' ;;
        *) printf '%s' "$type" ;;
    esac
}

call_prefix() {
    local api="$1" type="$2"
    if [[ "$type" == "bool" ]]; then
        printf 'const bool result = %s(' "$api"
    elif [[ "$type" == "bfloat16x2_t" ]]; then
        printf 'const bfloat16x2_t result = %s(' "$api"
    else
        printf 'dst[idx] = %s(' "$api"
    fi
}

call_suffix() {
    local type="$1"
    if [[ "$type" == "bool" ]]; then
        printf ');\n        dst[idx] = result;\n'
    elif [[ "$type" == "bfloat16x2_t" ]]; then
        printf ');\n        dst[idx] = reinterpret_cast<const uint32_t&>(result);\n'
    else
        printf ');\n'
    fi
}

output_cast_prefix() {
    local type="$1"
    case "$type" in
        bool) printf '' ;;
        *) printf '%s' "$type" ;;
    esac
}

bfloat16x2_initializer_list() {
    local api="$1" type="$2" position="$3"
    if [[ "$api" =~ ^__hb(equ|eq|geu|ge|gtu|gt|leu|le|ltu|lt|neu|ne)x2$ ]] ||
       [[ "$api" =~ ^__h(equ|eq|geu|ge|gtu|gt|leu|le|ltu|lt|neu|ne)x2(_mask)?$ ]]; then
        if [[ "$position" == "x" ]]; then
            printf 'MakeBFloat16x2(-1.0F, 0.0F), MakeBFloat16x2(1.0F, -1.0F), MakeBFloat16x2(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()), MakeBFloat16x2(std::numeric_limits<float>::quiet_NaN(), 0.5F)'
        else
            printf 'MakeBFloat16x2(1.0F, 0.0F), MakeBFloat16x2(1.0F, -2.0F), MakeBFloat16x2(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()), MakeBFloat16x2(0.5F, std::numeric_limits<float>::quiet_NaN())'
        fi
    elif [[ "$position" == "x" ]]; then
        printf 'MakeBFloat16x2(-1.25F, 0.5F), MakeBFloat16x2(2.5F, -3.75F), MakeBFloat16x2(-2.0F, 1.25F), MakeBFloat16x2(0.75F, -0.25F)'
    elif [[ "$position" == "y" ]]; then
        printf 'MakeBFloat16x2(0.5F, -2.0F), MakeBFloat16x2(1.25F, 3.0F), MakeBFloat16x2(1.5F, -1.0F), MakeBFloat16x2(-0.5F, 2.25F)'
    else
        printf 'MakeBFloat16x2(-0.5F, 1.0F), MakeBFloat16x2(-2.0F, 0.75F), MakeBFloat16x2(1.0F, -1.5F), MakeBFloat16x2(0.25F, 2.0F)'
    fi
}

host_vector_type() {
    local type="$1"
    case "$type" in
        bfloat16_t) printf 'uint16_t' ;;
        bfloat16x2_t) printf 'uint32_t' ;;
        bool) printf 'uint8_t' ;;
        *) printf '%s' "$type" ;;
    esac
}

device_pointer_type() {
    local type="$1"
    case "$type" in
        bfloat16_t) printf 'bfloat16_t' ;;
        bfloat16x2_t) printf 'uint32_t' ;;
        *) printf '%s' "$type" ;;
    esac
}

bf16_initializer_list() {
    local api="$1"
    if [[ "$api" == *_sat ]]; then
        printf 'FloatToBFloat16Bits(0.0F), FloatToBFloat16Bits(-0.0F), FloatToBFloat16Bits(65504.0F), FloatToBFloat16Bits(65536.0F), FloatToBFloat16Bits(1.0e38F), FloatToBFloat16Bits(std::numeric_limits<float>::infinity()), FloatToBFloat16Bits(-std::numeric_limits<float>::infinity()), FloatToBFloat16Bits(std::numeric_limits<float>::quiet_NaN())'
    else
        printf 'FloatToBFloat16Bits(0.0F), FloatToBFloat16Bits(-0.0F), FloatToBFloat16Bits(0.5F), FloatToBFloat16Bits(1.5F), FloatToBFloat16Bits(65536.0F), FloatToBFloat16Bits(1.0e38F), FloatToBFloat16Bits(std::numeric_limits<float>::infinity()), FloatToBFloat16Bits(std::numeric_limits<float>::quiet_NaN())'
    fi
}

print_call() {
    local type="$1" name="$2"
    case "$type" in
        half) printf 'PrintHalfVector("%s", %s)' "$name" "$name" ;;
        half2) printf 'PrintHalf2Vector("%s", %s)' "$name" "$name" ;;
        float) printf 'PrintFloatVector("%s", %s)' "$name" "$name" ;;
        float2) printf 'PrintFloat2Vector("%s", %s)' "$name" "$name" ;;
        bfloat16_t) printf 'PrintBFloat16BitsVector("%s", %s)' "$name" "$name" ;;
        bfloat16x2_t) printf 'PrintBFloat16x2Vector("%s", %s)' "$name" "$name" ;;
        bool) printf 'PrintBoolVector("%s", %s)' "$name" "$name" ;;
        *) printf 'PrintIntegerVector("%s", %s)' "$name" "$name" ;;
    esac
}

sentinel_value() {
    local type="$1"
    case "$type" in
        half) printf 'static_cast<half>(-999.0F)' ;;
        half2) printf 'MakeHalf2(-999.0F, -999.0F)' ;;
        float|float2) printf '%s' '-999.0F' ;;
        bfloat16_t) printf 'FloatToBFloat16Bits(-999.0F)' ;;
        bfloat16x2_t) printf 'MakeBFloat16x2(-999.0F, -999.0F)' ;;
        bool) printf '0' ;;
        int8_t|int16_t|int32_t|int64_t) printf '%s' '-99' ;;
        *) printf '99' ;;
    esac
}

output_host_type() {
    local type="$1"
    if [[ "$type" == "bool" ]]; then
        printf 'uint8_t'
    else
        printf '%s' "$type"
    fi
}

while IFS=$'\t' read -r api prototype; do
    [[ -z "$api" || -z "$prototype" ]] && continue

    api_dir="$OUTPUT_DIR/$api"
    mkdir -p "$api_dir"

    return_type="${prototype%%"$api"*}"
    return_type="${return_type//inline /}"
    return_type="${return_type# }"
    return_type="${return_type% }"
    return_type="$(normalize_type "$return_type")"

    params="${prototype#*\(}"
    params="${params%\)}"
    param_names=()
    param_types=()
    if [[ -n "${params// /}" ]]; then
        IFS=',' read -r -a raw_params <<< "$params"
        for raw_param in "${raw_params[@]}"; do
            raw_param="${raw_param# }"
            raw_param="${raw_param% }"
            [[ -z "$raw_param" ]] && continue
            param_name="${raw_param##* }"
            param_type="${raw_param% "${param_name}"}"
            param_type="${param_type# }"
            param_type="${param_type% }"
            param_type="${param_type//const /}"
            param_type="$(normalize_type "$param_type")"
            param_names+=("$param_name")
            param_types+=("$param_type")
        done
    fi

    upper_api="$(macro_name "$api")"
    include_bf16=0
    for type in "$return_type" "${param_types[@]}"; do
        [[ "$type" == "bfloat16_t" || "$type" == "bfloat16x2_t" ]] && include_bf16=1
    done

    {
        printf '#include <limits>\n'
        printf '#include <vector>\n\n'
        printf '#include "simt_api/asc_fp16.h"\n'
        if [[ $include_bf16 -eq 1 ]]; then
            printf '#include "simt_api/asc_bf16.h"\n'
        fi
        printf '#include "simt_api/asc_simt.h"\n'
        printf '#include "asc_runtime.h"\n\n'
        printf '#if defined(%s_MODE_SIMT)\n' "$upper_api"
        printf '__global__ __launch_bounds__(32) void %s_kernel(\n' "$api"
        printf '    %s* dst,' "$(device_pointer_type "$return_type")"
        for i in "${!param_names[@]}"; do
            printf ' const %s* %s,' "$(device_pointer_type "${param_types[$i]}")" "${param_names[$i]}"
        done
        printf ' uint32_t total_length)\n'
        printf '{\n'
        printf '    const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;\n'
        printf '    if (idx < total_length) {\n'
        for i in "${!param_names[@]}"; do
            if [[ "${param_types[$i]}" == "bfloat16x2_t" ]]; then
                printf '        const uint32_t %s_value = %s[idx];\n' "${param_names[$i]}" "${param_names[$i]}"
            fi
        done
        printf '        %s' "$(call_prefix "$api" "$return_type")"
        for i in "${!param_names[@]}"; do
            [[ $i -gt 0 ]] && printf ', '
            if [[ "${param_types[$i]}" == "bfloat16_t" ]]; then
                printf '__ushort_as_bfloat16(%s[idx])' "${param_names[$i]}"
            elif [[ "${param_types[$i]}" == "bfloat16x2_t" ]]; then
                printf 'reinterpret_cast<const bfloat16x2_t&>(%s_value)' "${param_names[$i]}"
            else
                printf '%s[idx]' "${param_names[$i]}"
            fi
        done
        printf '%s' "$(call_suffix "$return_type")"
        printf '    }\n'
        printf '}\n'
        printf '#elif defined(%s_MODE_HYBRID)\n' "$upper_api"
        if [[ "$api" == "__int2half_ru_sat" ]]; then
            printf '#include "cce_aicore_intrinsics.h"\n\n'
        fi
        printf '__simt_vf__ __launch_bounds__(32) inline void %s_vf(\n' "$api"
        printf '    __gm__ %s* dst,' "$(device_pointer_type "$return_type")"
        for i in "${!param_names[@]}"; do
            printf ' __gm__ const %s* %s,' "$(device_pointer_type "${param_types[$i]}")" "${param_names[$i]}"
        done
        printf ' uint32_t total_length)\n'
        printf '{\n'
        printf '    const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;\n'
        printf '    if (idx < total_length) {\n'
        for i in "${!param_names[@]}"; do
            if [[ "${param_types[$i]}" == "bfloat16x2_t" ]]; then
                printf '        const uint32_t %s_value = %s[idx];\n' "${param_names[$i]}" "${param_names[$i]}"
            fi
        done
        printf '        %s' "$(call_prefix "$api" "$return_type")"
        for i in "${!param_names[@]}"; do
            [[ $i -gt 0 ]] && printf ', '
            if [[ "${param_types[$i]}" == "bfloat16_t" ]]; then
                printf '__ushort_as_bfloat16(%s[idx])' "${param_names[$i]}"
            elif [[ "${param_types[$i]}" == "bfloat16x2_t" ]]; then
                printf 'reinterpret_cast<const bfloat16x2_t&>(%s_value)' "${param_names[$i]}"
            else
                printf '%s[idx]' "${param_names[$i]}"
            fi
        done
        printf '%s' "$(call_suffix "$return_type")"
        printf '    }\n'
        printf '}\n\n'
        printf '__global__ __vector__ void %s_entry(\n' "$api"
        printf '    __gm__ %s* dst,' "$(device_pointer_type "$return_type")"
        for i in "${!param_names[@]}"; do
            printf ' __gm__ const %s* %s,' "$(device_pointer_type "${param_types[$i]}")" "${param_names[$i]}"
        done
        printf ' uint32_t total_length)\n'
        printf '{\n'
        if [[ "$api" == "__int2half_ru_sat" ]]; then
            printf '    constexpr uint64_t ctrl60Mask = ~(uint64_t(1) << 60);\n'
            printf '    const uint64_t originalCtrl60 = static_cast<uint64_t>(__cce_scalar::get_ctrl());\n'
            printf '    __cce_scalar::set_ctrl(originalCtrl60 & ctrl60Mask);\n'
        fi
        printf '    asc_vf_call<%s_vf>(dim3(32),' "$api"
        printf ' dst'
        for i in "${!param_names[@]}"; do
            printf ', %s' "${param_names[$i]}"
        done
        printf ', total_length);\n'
        if [[ "$api" == "__int2half_ru_sat" ]]; then
            printf '    __cce_scalar::set_ctrl(originalCtrl60);\n'
        fi
        printf '}\n'
        printf '#endif\n\n'
        printf 'int main()\n{\n'
        for i in "${!param_names[@]}"; do
            printf '    std::vector<%s> %s = {\n        %s\n    };\n' \
                "$(host_vector_type "${param_types[$i]}")" "${param_names[$i]}" \
                "$(initializer_list "$api" "${param_types[$i]}" "${param_names[$i]}")"
        done
        printf '    std::vector<%s> output(x.size(), %s);\n' \
            "$(host_vector_type "$return_type")" "$(sentinel_value "$return_type")"
        printf '    const uint32_t totalLength = static_cast<uint32_t>(x.size());\n'
        printf '    const size_t inputByteSize = totalLength * sizeof(x[0]);\n'
        printf '    const size_t outputByteSize = totalLength * sizeof(output[0]);\n'
        printf '\n'
        printf '    aclrtStream stream = nullptr;\n'
        printf '    %s* outputDevice = nullptr;\n' "$(device_pointer_type "$return_type")"
        for i in "${!param_names[@]}"; do
            printf '    %s* %sDevice = nullptr;\n' "$(device_pointer_type "${param_types[$i]}")" "${param_names[$i]}"
        done
        printf '    bool deviceSet = false;\n'
        printf '    bool success = CheckAcl("aclInit", aclInit(nullptr));\n'
        printf '    if (success) {\n'
        printf '        success = CheckAcl("aclrtSetDevice", aclrtSetDevice(0));\n'
        printf '        deviceSet = success;\n'
        printf '    }\n'
        printf '    if (success) {\n'
        printf '        success = CheckAcl("aclrtCreateStream", aclrtCreateStream(&stream));\n'
        printf '    }\n'
        printf '    if (success) {\n'
        printf '        success = CheckAcl("aclrtMalloc(output)", aclrtMalloc(reinterpret_cast<void**>(&outputDevice), outputByteSize, ACL_MEM_MALLOC_HUGE_FIRST));\n'
        printf '    }\n'
        for i in "${!param_names[@]}"; do
            name="${param_names[$i]}"
            printf '    if (success) {\n'
            printf '        success = CheckAcl("aclrtMalloc(%s)", aclrtMalloc(reinterpret_cast<void**>(&%sDevice), inputByteSize, ACL_MEM_MALLOC_HUGE_FIRST));\n' "$name" "$name"
            printf '    }\n'
            printf '    if (success) {\n'
            printf '        success = CheckAcl("aclrtMemcpy(%s)", aclrtMemcpy(%sDevice, inputByteSize, %s.data(), inputByteSize, ACL_MEMCPY_HOST_TO_DEVICE));\n' "$name" "$name" "$name"
            printf '    }\n'
        done
        printf '    if (success) {\n'
        printf '#if defined(%s_MODE_SIMT)\n' "$upper_api"
        printf '        %s_kernel<<<1, 32, 0, stream>>>(' "$api"
        printf 'outputDevice'
        for i in "${!param_names[@]}"; do
            printf ', %sDevice' "${param_names[$i]}"
        done
        printf ', totalLength);\n'
        printf '#elif defined(%s_MODE_HYBRID)\n' "$upper_api"
        printf '        %s_entry<<<1, 0, stream>>>(' "$api"
        printf 'outputDevice'
        for i in "${!param_names[@]}"; do
            printf ', %sDevice' "${param_names[$i]}"
        done
        printf ', totalLength);\n'
        printf '#endif\n'
        printf '        success = CheckAcl("aclrtSynchronizeStream", aclrtSynchronizeStream(stream));\n'
        printf '    }\n'
        printf '    if (success) {\n'
        printf '        success = CheckAcl("aclrtMemcpy(output)", aclrtMemcpy(output.data(), outputByteSize, outputDevice, outputByteSize, ACL_MEMCPY_DEVICE_TO_HOST));\n'
        printf '    }\n'
        printf '\n'
        printf '    if (success) {\n'
        for i in "${!param_names[@]}"; do
            printf '        %s;\n' "$(print_call "${param_types[$i]}" "${param_names[$i]}")"
        done
        printf '        %s;\n' "$(print_call "$return_type" "output")"
        printf '    }\n'
        printf '\n'
        printf '    if (outputDevice != nullptr) {\n'
        printf '        (void)aclrtFree(outputDevice);\n'
        printf '    }\n'
        for ((i=${#param_names[@]}-1; i>=0; i--)); do
            printf '    if (%sDevice != nullptr) {\n' "${param_names[$i]}"
            printf '        (void)aclrtFree(%sDevice);\n' "${param_names[$i]}"
            printf '    }\n'
        done
        printf '    if (stream != nullptr) {\n'
        printf '        (void)aclrtDestroyStream(stream);\n'
        printf '    }\n'
        printf '    if (deviceSet) {\n'
        printf '        (void)aclrtResetDevice(0);\n'
        printf '    }\n'
        printf '    (void)aclFinalize();\n'
        printf '    return success ? 0 : 1;\n'
        printf '}\n'
    } > "$api_dir/$api.asc"

    make_cmake "$api" simt "$OUTPUT_DIR"
    make_cmake "$api" hybrid "$OUTPUT_DIR"
    make_start "$api" "$OUTPUT_DIR"
done < "$MANIFEST"

printf 'generated_projects=%s\n' "$(find "$OUTPUT_DIR" -mindepth 1 -maxdepth 1 -type d ! -name common | wc -l)"
printf 'generated_asc=%s\n' "$(find "$OUTPUT_DIR" -name '*.asc' | wc -l)"
