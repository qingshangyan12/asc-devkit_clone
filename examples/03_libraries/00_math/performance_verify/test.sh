#!/bin/bash

CONFIG_FILE="./testcases.json"
BUILD_DIR="build"
LOG_FILE="./performance_results.log"
RESULT_FILE="./result.md"

declare -A API_MAP="([exp]='1:1' [mul]='2:2' [adds]='3:3' [leakyrelu]='4:3')"

run_single_test() {
    local case_num=$1
    local api_id=$2 api_type=$3 data_type=$4 size_k=$5 api_name=$6 dtype_name=$7
    local size_elements=$((size_k * 1024))
    
    echo ""
    echo -e "\033[1;36m┌─────────────────────────────────────────────────────────────────────────────────┐\033[0m"
    echo -e "\033[1;36m│ Case #${case_num}                                                                        │\033[0m"
    echo -e "\033[1;36m│ API: ${api_name} | DataType: ${dtype_name} | DataLength: ${size_k}K (${size_elements} elements)        │\033[0m"
    echo -e "\033[1;36m└─────────────────────────────────────────────────────────────────────────────────┘\033[0m"
    
    rm -rf "$BUILD_DIR" && mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    echo -e "\033[33m  ↳ Building...\033[0m"
    cmake .. -DINPUT_SIZE=$size_elements -DINPUT_API_ID=$api_id -DINPUT_API_TYPE=$api_type -DINPUT_DATA_TYPE=$data_type > /dev/null 2>&1
    make -j > /dev/null 2>&1
    
    if [ ! -f "./demo" ]; then
        echo -e "\033[31m  ✗ Build failed\033[0m"
        cd .. && return 1
    fi
    echo -e "\033[32m  ✓ Build OK\033[0m"
    
    echo -e "\033[33m  ↳ Generating data...\033[0m"
    python3 ../scripts/gen_data.py -inputSize=$size_elements -apiId=$api_id -inputType=$data_type -apiType=$api_type > /dev/null 2>&1
    echo -e "\033[32m  ✓ Data OK\033[0m"
    
    echo -e "\033[33m  ↳ Running kernel...\033[0m"
    echo -e "\033[32m  ✓ Performance Verification\033[0m"
    timeout 60 ./demo 2>&1 | tee -a ../"$LOG_FILE"
    python3 ../scripts/verify_result.py -api_id=$api_id -api_type=$api_type -output_type=$data_type 2>&1 | tee -a ../"$LOG_FILE"
    
    cd ..
}

generate_result_md() {
    python3 scripts/calc_performance.py -log_file "$LOG_FILE" -result_md "$RESULT_FILE" 2>/dev/null
}

run_all_tests() {
    echo -e "\033[1;35m╔══════════════════════════════════════════════════════════════════════════════════╗\033[0m"
    echo -e "\033[1;35m║              Ascend C Performance Verification - Auto Test Runner                ║\033[0m"
    echo -e "\033[1;35m╚══════════════════════════════════════════════════════════════════════════════════╝\033[0m"
    echo ""
    
    rm -f "$LOG_FILE" "$RESULT_FILE"
    echo "Performance Test Results - $(date)" > "$LOG_FILE"
    echo "========================================" >> "$LOG_FILE"
    
    # 获取配置的测试API列表
    TEST_APIS=$(python3 -c "
import json
import sys
with open('$CONFIG_FILE') as f:
    data = json.load(f)
test_apis = data.get('test_apis', ['all'])
if 'all' in test_apis or test_apis == ['all']:
    sys.stdout.write('exp mul adds leakyrelu')
else:
    sys.stdout.write(' '.join(test_apis))
")
    
    local total=0 passed=0 failed=0 case_num=0
    
    for api_name in $TEST_APIS; do
        api_info="${API_MAP[$api_name]}"
        [ -z "$api_info" ] && continue
        
        api_id="${api_info%%:*}"
        api_type="${api_info##*:}"
        
        configs=$(python3 -c "
import json
try:
    with open('$CONFIG_FILE') as f:
        data = json.load(f)
    if '$api_name' in data:
        for dtype, sizes in data['$api_name'].items():
            type_id = {'half': 1, 'float': 2, 'int16': 3, 'int32': 4}.get(dtype)
            if type_id:
                for s in sizes:
                    print(f'$api_id:$api_type:{type_id}:{s}:{dtype}')
except: pass
")
        
        [ -z "$configs" ] && continue
        
        while IFS=: read -r a_id a_type dtype size_k dtype_name; do
            [ -z "$a_id" ] && continue
            case_num=$((case_num + 1))
            total=$((total + 1))
            if run_single_test "$case_num" "$a_id" "$a_type" "$dtype" "$size_k" "$api_name" "$dtype_name"; then
                passed=$((passed + 1))
            else
                failed=$((failed + 1))
            fi
        done <<< "$configs"
    done
    
    echo ""
    echo -e "\033[1;35m╔══════════════════════════════════════════════════════════════════════════════════╗\033[0m"
    echo -e "\033[1;35m║                                   Summary                                     ║\033[0m"
    echo -e "\033[1;35m╚══════════════════════════════════════════════════════════════════════════════════╝\033[0m"
    echo -e "  Total: \033[1;37m${total}\033[0m  |  Passed: \033[1;32m${passed}\033[0m  |  Failed: \033[1;31m${failed}\033[0m"
    echo ""
    
    if [ -f "$LOG_FILE" ] && grep -q "cost total cycles" "$LOG_FILE"; then
        echo -e "\033[1;33m╔══════════════════════════════════════════════════════════════════════════════════╗\033[0m"
        echo -e "\033[1;33m║                              Performance Report                              ║\033[0m"
        echo -e "\033[1;33m╚══════════════════════════════════════════════════════════════════════════════════╝\033[0m"
        generate_result_md
    fi
}

run_all_tests
