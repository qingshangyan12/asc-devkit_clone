#!/usr/bin/env python3
# coding=utf-8

import re
import sys
import argparse

THEORETICAL_PERFORMANCE = {
    ("exp", "half"): 32,
    ("exp", "float"): 32,
    ("mul", "half"): 128,
    ("mul", "float"): 64,
    ("mul", "int16"): 128,
    ("mul", "int32"): 64,
    ("adds", "half"): 128,
    ("adds", "float"): 64,
    ("adds", "int16"): 128,
    ("adds", "int32"): 64,
    ("leakyrelu", "half"): 128,
    ("leakyrelu", "float"): 64,
}

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "adds",
    4: "leakyrelu",
}

DATA_TYPE_MAP = {
    1: "half",
    2: "float",
    3: "int16",
    4: "int32",
}

REPEAT_TIMES = 1000


def parse_log_file(log_file):
    results = []
    content = open(log_file, 'r').read()
    
    pattern = r'\[LOG\]\s*(\w+)\s+instruction\s+executed\s+\d+\s+times,\s+data\s+length:\s+(\d+),\s+cost\s+total\s+cycles:\s+(\d+)'
    
    for match in re.finditer(pattern, content):
        api_name = match.group(1).lower()
        elements = int(match.group(2))
        total_cycles = int(match.group(3))
        
        result = {
            'api_name': api_name,
            'total_elements': elements,
            'total_cycles': total_cycles,
            'size_k': elements // 1024,
            'passed': False,
        }
        
        end_pos = match.end()
        remaining = content[end_pos:end_pos+500]
        
        if re.search(r'Test PASSED', remaining):
            result['passed'] = True
        
        m = re.search(r'Total elements:\s*(\d+)', remaining)
        if m:
            result['total_elements'] = int(m.group(1))
        
        m = re.search(r'Error Ratio:\s*([\d.]+)', remaining)
        if m:
            result['error_ratio'] = float(m.group(1))
        
        results.append(result)
    
    return results


def calculate_performance(result):
    total_cycles = result.get('total_cycles', 0)
    total_elements = result.get('total_elements', 0)
    api_name = result.get('api_name', 'unknown')
    
    dtype = "half"
    if "float32" in str(result.get('total_elements', '')) or "float" in str(result):
        dtype = "float"
    
    t_total = total_cycles
    t_avg = t_total / REPEAT_TIMES
    t_ai = t_avg / 50 * 1800
    throughput = total_elements / t_ai if t_ai > 0 else 0
    
    key = (api_name, dtype)
    theory = THEORETICAL_PERFORMANCE.get(key, 0)
    achievement = (throughput / theory * 100) if theory > 0 else 0
    
    return {
        'api_name': api_name,
        'data_type': dtype,
        'size_k': result.get('size_k', 0),
        'total_elements': total_elements,
        'total_cycles': t_total,
        'avg_cycles_50mhz': t_avg,
        'throughput_1800mhz': throughput,
        'theory_parallelism': theory,
        'achievement_rate': achievement,
        'passed': result.get('passed', False),
    }


def print_summary(results):
    print("\n" + "=" * 120)
    print("Performance Summary Report")
    print("=" * 120)
    print(f"{'API':<12} {'Type':<8} {'Size(K)':<8} {'Elements':<10} {'TotalCycles':<12} {'AvgCycles':<12} {'Throughput':<12} {'Theory':<10} {'Achieve%':<10} {'Status':<8}")
    print("-" * 120)
    
    for r in results:
        calc = calculate_performance(r)
        status = "PASS" if calc['passed'] else "FAIL"
        print(f"{calc['api_name']:<12} {calc['data_type']:<8} {calc['size_k']:<8} "
              f"{calc['total_elements']:<10} {calc['total_cycles']:<12} "
              f"{calc['avg_cycles_50mhz']:<12.3f} {calc['throughput_1800mhz']:<12.2f} "
              f"{calc['theory_parallelism']:<10} {calc['achievement_rate']:<10.2f} {status:<8}")
    
    print("=" * 120)


def main():
    parser = argparse.ArgumentParser(description="Calculate performance statistics from log file")
    parser.add_argument('-log_file', type=str, default='performance_results.log',
                        help="Path to the log file")
    args = parser.parse_args()
    
    results = parse_log_file(args.log_file)
    
    if not results:
        print("[ERROR] No results found in log file")
        sys.exit(1)
    
    print_summary(results)


if __name__ == "__main__":
    main()
