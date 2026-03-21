#!/usr/bin/env python3
# coding=utf-8
"""
Ascend C API 性能统计模块
功能：解析测试日志，计算性能指标，生成性能报告
"""

import re
import sys
import json
import argparse
import os

from gen_data_utils import API_NAME_MAP, THEORETICAL_PERFORMANCE, REPEAT_TIMES


def load_theory_from_config(config_file='testcases.json'):
    """从配置文件加载理论性能数据"""
    if not os.path.exists(config_file):
        return {}
    try:
        with open(config_file) as f:
            data = json.load(f)
        theory = data.get('theory', {})
        result = {}
        for api_name, types in theory.items():
            for dtype, value in types.items():
                result[(api_name.lower(), dtype.lower())] = value
        return result
    except:
        return {}


def parse_log_file(log_file):
    """
    解析日志文件，提取性能测试结果
    
    Returns:
        list: 每个测试 case 的结果字典列表
    """
    results = []
    content = open(log_file, 'r').read()
    
    # 先提取所有 [DataType] 行及其值，建立位置索引
    dtype_pattern = r'\[DataType\]\s+(float16|float32|int16|int32)'
    dtype_positions = []
    for m in re.finditer(dtype_pattern, content):
        dtype_positions.append((m.start(), m.group(1)))
    
    # 匹配性能日志格式: [LOG] Exp instruction executed 1000 times, data length: 4096, data type: float16, cost total cycles: 4445
    log_pattern = r'\[LOG\]\s*(\w+)\s+instruction\s+executed\s+\d+\s+times,\s+data\s+length:\s+(\d+),\s+data\s+type:\s+(\w+),\s+cost\s+total\s+cycles:\s+(\d+)'
    
    for match in re.finditer(log_pattern, content):
        api_name = match.group(1).lower()
        elements = int(match.group(2))
        dtype_raw = match.group(3)
        total_cycles = int(match.group(4))
        log_end_pos = match.end()
        
        # 转换 dtype 格式
        dtype_map = {'float16': 'half', 'float32': 'float', 'int16': 'int16', 'int32': 'int32'}
        current_dtype = dtype_map.get(dtype_raw, dtype_raw)
        
        result = {
            'api_name': api_name,
            'total_elements': elements,
            'total_cycles': total_cycles,
            'size_k': elements // 1024,
            'passed': False,
            'data_type': current_dtype,
        }
        
        # 在日志后续内容中查找验证结果
        remaining = content[log_end_pos:log_end_pos+5000]
        
        # 检查验证是否通过
        if re.search(r'Precision Verification:.*✓', remaining):
            result['passed'] = True
        
        # 提取匹配数量
        m = re.search(r'Matched:\s*(\d+)', remaining)
        if m:
            result['matched'] = int(m.group(1))
        
        # 提取错误率
        m = re.search(r'Error Ratio:\s*([\d.]+)', remaining)
        if m:
            result['error_ratio'] = float(m.group(1))
        
        results.append(result)
    
    return results


def calculate_performance(result, dtype="half", theory_config=None):
    """
    计算性能指标
    
    Args:
        result: 解析出的测试结果
        dtype: 数据类型
        theory_config: 理论性能配置
    
    Returns:
        dict: 包含各项性能指标的字典
    """
    if theory_config is None:
        theory_config = {}
    
    total_cycles = result.get('total_cycles', 0)
    total_elements = result.get('total_elements', 0)
    api_name = result.get('api_name', 'unknown')
    
    # 性能计算公式
    t_total = total_cycles                                          # 总周期 (50MHz)
    t_avg = t_total / REPEAT_TIMES                                  # 平均周期
    t_ai = t_avg / 50 * 1800                                       # 换算到 AI Core 频率
    throughput = total_elements / t_ai if t_ai > 0 else 0           # 有效并行度
    
    # 获取理论并行度
    key = (api_name, dtype)
    theory = theory_config.get(key) or THEORETICAL_PERFORMANCE.get(key, 0)
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


def generate_markdown_table(results, theory_config=None):
    """生成 Markdown 格式的性能报告表格"""
    lines = []
    lines.append("# Ascend C API Performance Report")
    lines.append("")
    lines.append("| API | Type | Size(K) | Elements | TotalCycles | AvgCycles | Actual | Theory | Achieve% | Status |")
    lines.append("| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |")
    
    for r in results:
        dtype = r.get('data_type', 'half')
        calc = calculate_performance(r, dtype=dtype, theory_config=theory_config)
        
        # 达标判断：仅根据 Achieve >= 70% 判断
        is_pass = calc['achievement_rate'] >= 70
        status = "PASS (Achieve≥70%)" if is_pass else "FAIL (Achieve<70%)"
        
        lines.append(f"| {calc['api_name']} | {calc['data_type']} | {calc['size_k']} | "
                   f"{calc['total_elements']} | {calc['total_cycles']} | "
                   f"{calc['avg_cycles_50mhz']:.3f} | {calc['throughput_1800mhz']:.2f} | "
                   f"{calc['theory_parallelism']} | {calc['achievement_rate']:.2f}% | {status} |")
    
    return '\n'.join(lines)


def print_summary(results, theory_config=None):
    """打印性能汇总表格到终端"""
    print("\n" + "=" * 130)
    print("Performance Summary Report")
    print("=" * 130)
    print(f"{'API':<12} {'Type':<8} {'Size(K)':<8} {'Elements':<10} {'TotalCycles':<12} {'AvgCycles':<12} {'Actual':<12} {'Theory':<10} {'Achieve%':<10} {'Status':<15}")
    print("-" * 130)
    
    for r in results:
        dtype = r.get('data_type', 'half')
        calc = calculate_performance(r, dtype=dtype, theory_config=theory_config)
        
        # 达标判断：仅根据 Achieve >= 70% 判断
        is_pass = calc['achievement_rate'] >= 70
        if is_pass:
            status = "\033[32mPASS (Achieve≥70%)\033[0m"
        else:
            status = "\033[31mFAIL (Achieve<70%)\033[0m"
        
        print(f"{calc['api_name']:<12} {calc['data_type']:<8} {calc['size_k']:<8} "
              f"{calc['total_elements']:<10} {calc['total_cycles']:<12} "
              f"{calc['avg_cycles_50mhz']:<12.3f} {calc['throughput_1800mhz']:<12.2f} "
              f"{calc['theory_parallelism']:<10} {calc['achievement_rate']:<10.2f} {status}")
    
    print("=" * 130)


def main():
    parser = argparse.ArgumentParser(description="Calculate performance statistics")
    parser.add_argument('-log_file', type=str, default='performance_results.log')
    parser.add_argument('-result_md', type=str, default='result.md', help="Output markdown file")
    args = parser.parse_args()
    
    # 加载理论性能配置
    theory_config = load_theory_from_config()
    if theory_config:
        print("[INFO] Loaded theory performance from testcases.json")
    
    # 解析日志文件
    results = parse_log_file(args.log_file)
    
    if not results:
        print("[ERROR] No results found in log file")
        sys.exit(1)
    
    # 打印终端汇总
    print_summary(results, theory_config)
    
    # 保存到 Markdown 文件
    md_content = generate_markdown_table(results, theory_config)
    with open(args.result_md, 'w') as f:
        f.write(md_content)
    print(f"\n[INFO] Result saved to: {args.result_md}")


if __name__ == "__main__":
    main()
