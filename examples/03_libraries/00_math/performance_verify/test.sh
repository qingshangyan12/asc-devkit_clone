#!/bin/bash

CONFIG_FILE="./user_input.json"
BUILD_DIR="build"
LOG_FILE="./performance_results.log"

run_single_test() {
    local api_id=$1 api_type=$2 data_type=$3 size_k=$4 dtype_name=$5
    local size_elements=$((size_k * 1024))
    
    echo ""
    echo "========================================="
    echo "Testing: API_ID=$api_id, Type=$dtype_name, Size=${size_k}K (${size_elements} elements)"
    echo "========================================="
    
    rm -rf "$BUILD_DIR" && mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    cmake .. -DINPUT_SIZE=$size_elements -DINPUT_API_ID=$api_id -DINPUT_API_TYPE=$api_type -DINPUT_DATA_TYPE=$data_type > /dev/null 2>&1
    make -j > /dev/null 2>&1
    
    if [ ! -f "./demo" ]; then
        echo "[ERROR] Build failed"
        cd .. && return 1
    fi
    
    python3 ../scripts/gen_data.py -inputSize=$size_elements -apiId=$api_id -inputType=$data_type -apiType=$api_type > /dev/null 2>&1
    
    timeout 60 ./demo 2>&1 | tee -a ../"$LOG_FILE"
    python3 ../scripts/verify_result.py -api_id=$api_id -api_type=$api_type -output_type=$data_type 2>&1 | tee -a ../"$LOG_FILE"
    
    cd ..
    echo "[OK] Completed: ${dtype_name} ${size_k}K"
}

run_all_tests() {
    echo "========================================="
    echo "Performance Verify - Auto Test Runner"
    echo "========================================="
    
    rm -f "$LOG_FILE"
    echo "Performance Test Results - $(date)" > "$LOG_FILE"
    echo "=========================================" >> "$LOG_FILE"
    
    local total=0 passed=0 failed=0
    
    for api_name in exp mul adds leakyrelu; do
        declare -A API_MAP="([exp]='1:1' [mul]='2:2' [adds]='3:3' [leakyrelu]='4:3')"
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
except Exception as e:
    print('', file=sys.stderr)
")
        
        [ -z "$configs" ] && continue
        
        echo "Testing API: $api_name"
        while IFS=: read -r a_id a_type dtype size_k dtype_name; do
            [ -z "$a_id" ] && continue
            total=$((total + 1))
            if run_single_test "$a_id" "$a_type" "$dtype" "$size_k" "$dtype_name"; then
                passed=$((passed + 1))
            else
                failed=$((failed + 1))
            fi
        done <<< "$configs"
    done
    
    echo ""
    echo "========================================="
    echo "Test Summary: Total=$total, Passed=$passed, Failed=$failed"
    echo "========================================="
    
    if [ -f "$LOG_FILE" ] && grep -q "cost total cycles" "$LOG_FILE"; then
        echo ""
        echo "========================================="
        echo "Performance Report"
        echo "========================================="
        python3 scripts/calc_performance.py -log_file "$LOG_FILE"
    fi
}

run_all_tests
