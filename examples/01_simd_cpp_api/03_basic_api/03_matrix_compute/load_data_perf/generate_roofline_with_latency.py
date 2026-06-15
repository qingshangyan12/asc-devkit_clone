#!/usr/bin/env python3
"""
生成 MTE1 Bandwidth Roofline 图（带延迟模型）
理论带宽: 256 Byte/cycle
固定延迟: 30 cycles
L0 最大容量: 64 KB

使用方法:
  python3 generate_roofline_with_latency.py --csv perf_data_20260613_162817/perf_result_scenario1.csv
  python3 generate_roofline_with_latency.py perf_data_20260613_162817/perf_result_scenario1.csv
"""

import csv
import sys
import argparse
from pathlib import Path
import math

def read_csv_data(csv_file):
    """读取 CSV 数据"""
    data = []
    with open(csv_file, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append({
                'test_id': int(row['Test_ID']),
                'm': int(row['M']),
                'k': int(row['K']),
                'n': int(row['N']),
                'shape': row['Shape'],
                'time_us': float(row['AIC_MTE1_Time(us)']),
                'cycle': float(row['Cycle']),
                'bandwidth_gbps': float(row['Bandwidth(GB/s)'])
            })
    return data

def calculate_theoretical_bandwidth(data_size_bytes, peak_bw_bytes_per_cycle=256, latency_cycles=30, frequency_mhz=1800):
    """
    计算理论带宽（考虑固定延迟）
    
    参数：
        data_size_bytes: 数据量（字节）
        peak_bw_bytes_per_cycle: 峰值带宽（字节/周期）= 256 Byte/cycle
        latency_cycles: 固定延迟（周期）= 30 cycles
        frequency_mhz: 主频（MHz）= 1800 MHz
    
    返回：
        bandwidth_gbps: 理论带宽（GB/s）
        time_us: 理论时间（微秒）
        time_cycles: 理论周期数
    """
    # 计算理论时间（cycles）
    # Time = Fixed_Latency + DataSize / PeakBandwidth
    transfer_cycles = data_size_bytes / peak_bw_bytes_per_cycle
    total_cycles = latency_cycles + transfer_cycles
    
    # 转换为微秒
    time_us = total_cycles / frequency_mhz
    
    # 计算带宽（GB/s）
    # Bandwidth = DataSize / Time
    # Bandwidth_GBps = DataSize_bytes / Time_us / 1e3
    bandwidth_gbps = data_size_bytes / time_us / 1e3
    
    return bandwidth_gbps, time_us, total_cycles

def calculate_data_size(m, k):
    """计算数据量（字节）"""
    # 场景1：Load2DA，搬运 A 矩阵，bfloat16 (2 bytes)
    return m * k * 2

def generate_roofline_with_latency(data, output_file):
    """生成带延迟模型的 Roofline 图（ASCII版本）"""
    
    # 计算每个测试的数据量
    for item in data:
        item['data_size'] = calculate_data_size(item['m'], item['k'])
        item['data_size_kb'] = item['data_size'] / 1024
    
    # 理论参数
    peak_bw_bytes_per_cycle = 256  # Byte/cycle
    latency_cycles = 30  # cycles
    frequency_mhz = 1800  # MHz
    l0_max_size_kb = 64  # L0最大容量 64KB
    
    # 计算峰值带宽（GB/s）
    peak_bw_gbps = peak_bw_bytes_per_cycle * frequency_mhz / 1e3  # 256 * 1800 / 1e3 = 460.8 GB/s
    
    # 计算理论曲线数据
    # 横轴限制在64KB（L0最大容量）
    max_data_kb = l0_max_size_kb  # 64KB
    min_data_kb = 0.1  # 最小值
    
    # 生成理论曲线的数据点（0到64KB）
    theory_data_sizes = []
    theory_bandwidths = []
    
    # 从小数据到64KB，生成理论曲线
    num_points = 100
    for i in range(num_points):
        data_size_kb = min_data_kb + (max_data_kb - min_data_kb) * i / (num_points - 1)
        data_size = data_size_kb * 1024
        
        bw, time, cycles = calculate_theoretical_bandwidth(
            data_size, peak_bw_bytes_per_cycle, latency_cycles, frequency_mhz)
        theory_data_sizes.append(data_size_kb)  # KB
        theory_bandwidths.append(bw)
    
    # 生成 ASCII 图
    lines = []
    lines.append("=" * 80)
    lines.append("MTE1 Bandwidth Roofline Model (L0 Size Limited)")
    lines.append("=" * 80)
    lines.append("")
    lines.append("硬件限制说明：")
    lines.append(f"  L0 Buffer 最大容量: {l0_max_size_kb} KB")
    lines.append(f"  横轴范围: 0 - {l0_max_size_kb} KB (限制在L0容量内)")
    lines.append("")
    lines.append("Roofline 模型参数：")
    lines.append(f"  峰值带宽: {peak_bw_bytes_per_cycle} Byte/cycle")
    lines.append(f"  主频: {frequency_mhz} MHz")
    lines.append(f"  峰值带宽上限: {peak_bw_gbps:.2f} GB/s")
    lines.append(f"  固定延迟: {latency_cycles} cycles = {latency_cycles/frequency_mhz:.4f} us")
    lines.append("")
    lines.append("理论公式：")
    lines.append("  Time(cycles) = Fixed_Latency + DataSize / PeakBW")
    lines.append(f"  Time(cycles) = {latency_cycles} + DataSize / {peak_bw_bytes_per_cycle}")
    lines.append("  Bandwidth(GB/s) = DataSize / Time(us) / 1e3")
    lines.append("")
    lines.append("图表说明：")
    lines.append("  X轴: Data Size (KB) - 数据搬运量（限制在64KB内）")
    lines.append("  Y轴: Bandwidth (GB/s) - 实际带宽")
    lines.append("  实线: 理论带宽曲线（考虑延迟）")
    lines.append("  虚线: 峰值带宽上限（无延迟）")
    lines.append("  *号: 实际测试数据点")
    lines.append("")
    
    # 绘制图表框架
    chart_width = 70
    chart_height = 25
    
    # 找出最大带宽用于缩放（考虑理论曲线的最大值）
    max_bandwidth = max(max(theory_bandwidths), max(item['bandwidth_gbps'] for item in data))
    
    # Y轴缩放（带宽）
    bw_per_line = max_bandwidth / chart_height
    
    # X轴缩放（数据量），限制在64KB
    kb_per_char = max_data_kb / chart_width  # 64KB / chart_width
    
    # 创建图表网格
    grid = [[' ' for _ in range(chart_width)] for _ in range(chart_height)]
    
    # 绘制峰值带宽线（水平虚线）
    peak_y = int(chart_height - peak_bw_gbps / bw_per_line)
    if 0 <= peak_y < chart_height:
        for x in range(chart_width):
            if x % 3 == 0:
                grid[peak_y][x] = ':'
    
    # 绘制理论带宽曲线（实线）
    for i in range(len(theory_data_sizes)):
        data_kb = theory_data_sizes[i]
        bw_gbps = theory_bandwidths[i]
        
        x_pos = int(data_kb / kb_per_char)
        y_pos = int(chart_height - bw_gbps / bw_per_line)
        
        x_pos = max(0, min(chart_width - 1, x_pos))
        y_pos = max(0, min(chart_height - 1, y_pos))
        
        # 绘制曲线点
        if grid[y_pos][x_pos] == ' ':
            grid[y_pos][x_pos] = '-'  # 理论曲线
    
    # 绘制实际测试数据点
    for item in data:
        x_pos = int(item['data_size_kb'] / kb_per_char)
        y_pos = int(chart_height - item['bandwidth_gbps'] / bw_per_line)
        
        x_pos = max(0, min(chart_width - 1, x_pos))
        y_pos = max(0, min(chart_height - 1, y_pos))
        
        # 标记数据点
        grid[y_pos][x_pos] = '*'
    
    # 添加坐标轴标签
    lines.append("Bandwidth (GB/s)")
    for i, row in enumerate(grid):
        # Y轴标签
        bw_value = (chart_height - i) * bw_per_line
        
        if i == peak_y:
            label = f"{peak_bw_gbps:6.1f} ::"
        elif i % 5 == 0:
            label = f"{bw_value:6.1f} |"
        else:
            label = "       |"
        
        line = label + ''.join(row)
        lines.append(line)
    
    # X轴
    lines.append("       +" + "-" * chart_width)
    lines.append("        Data Size (KB)")
    
    # X轴刻度（0到64KB）
    x_labels = []
    for i in range(0, chart_width + 1, 14):
        data_kb = int(i * kb_per_char)
        x_labels.append(f"{data_kb:3d}")
    lines.append("       " + ' '.join(x_labels))
    
    lines.append("")
    lines.append("图例说明：")
    lines.append(f"  :  峰值带宽上限（{peak_bw_gbps:.1f} GB/s，无延迟）")
    lines.append(f"  -  理论带宽曲线（考虑{latency_cycles} cycle延迟）")
    lines.append("  *  实际测试数据点")
    lines.append(f"  L0 Buffer限制: {l0_max_size_kb} KB")
    lines.append("")
    
    # 数据点详细说明
    lines.append("=" * 80)
    lines.append("实际测试数据详细分析")
    lines.append("=" * 80)
    
    for item in data:
        data_kb = item['data_size_kb']
        
        # 计算理论值
        theory_bw, theory_time, theory_cycles = calculate_theoretical_bandwidth(
            item['data_size'], peak_bw_bytes_per_cycle, latency_cycles, frequency_mhz)
        
        # 计算差异
        bw_diff = item['bandwidth_gbps'] - theory_bw
        time_diff_us = item['time_us'] - theory_time
        time_diff_cycles = item['cycle'] - theory_cycles
        
        lines.append(f"\nTest {item['test_id']}: Shape [{item['m']}, {item['k']}, {item['n']}]")
        lines.append("-" * 80)
        lines.append(f"  数据量: {data_kb:.2f} KB ({item['data_size']} bytes)")
        lines.append("")
        lines.append(f"  实际测量:")
        lines.append(f"    时间: {item['time_us']:.4f} us = {item['cycle']:.2f} cycles")
        lines.append(f"    带宽: {item['bandwidth_gbps']:.3f} GB/s")
        lines.append("")
        lines.append(f"  理论计算:")
        lines.append(f"    时间: {theory_time:.4f} us = {theory_cycles:.2f} cycles")
        lines.append(f"         = {latency_cycles} + {item['data_size']}/{peak_bw_bytes_per_cycle:.0f}")
        lines.append(f"         = {latency_cycles} + {item['data_size']/peak_bw_bytes_per_cycle:.2f}")
        lines.append(f"    带宽: {theory_bw:.3f} GB/s")
        lines.append("")
        lines.append(f"  性能对比:")
        lines.append(f"    时间差异: {time_diff_us:.4f} us = {time_diff_cycles:.2f} cycles")
        lines.append(f"    带宽差异: {bw_diff:.3f} GB/s ({(bw_diff/theory_bw)*100:+.1f}%)")
        
        # 性能评级
        if abs(bw_diff) < 5:
            rating = "✓ 理论相符"
        elif bw_diff > 5:
            rating = "✓ 优于理论"
        else:
            rating = "⚠ 低于理论"
        lines.append(f"    评级: {rating}")
    
    # 性能总结
    lines.append("")
    lines.append("=" * 80)
    lines.append("性能分析总结")
    lines.append("=" * 80)
    
    avg_bandwidth = sum(item['bandwidth_gbps'] for item in data) / len(data)
    avg_theory_bw = sum(calculate_theoretical_bandwidth(
        item['data_size'], peak_bw_bytes_per_cycle, latency_cycles, frequency_mhz)[0] 
        for item in data) / len(data)
    
    lines.append(f"  平均实际带宽: {avg_bandwidth:.3f} GB/s")
    lines.append(f"  平均理论带宽: {avg_theory_bw:.3f} GB/s")
    lines.append(f"  平均差异: {avg_bandwidth - avg_theory_bw:.3f} GB/s ({((avg_bandwidth - avg_theory_bw)/avg_theory_bw)*100:+.1f}%)")
    lines.append("")
    
    # 延迟影响分析
    lines.append("延迟影响分析：")
    lines.append("")
    lines.append(f"  L0 Buffer最大容量: {l0_max_size_kb} KB = {l0_max_size_kb * 1024} bytes")
    lines.append("")
    
    for item in data:
        transfer_cycles = item['data_size'] / peak_bw_bytes_per_cycle
        latency_ratio = latency_cycles / (latency_cycles + transfer_cycles) * 100
        
        # 计算L0利用率
        l0_utilization = item['data_size_kb'] / l0_max_size_kb * 100
        
        lines.append(f"  Shape [{item['m']},{item['k']},{item['n']}]:")
        lines.append(f"    数据量: {item['data_size_kb']:.2f} KB ({l0_utilization:.1f}% of L0)")
        lines.append(f"    延迟占比: {latency_ratio:.1f}%")
        lines.append(f"    数据搬运: {transfer_cycles:.1f} cycles")
        lines.append(f"    总时间: {latency_cycles + transfer_cycles:.1f} cycles")
    
    lines.append("")
    
    # 计算64KB数据量的理论值（作为L0上限参考）
    l0_max_bytes = l0_max_size_kb * 1024
    l0_bw, l0_time, l0_cycles = calculate_theoretical_bandwidth(
        l0_max_bytes, peak_bw_bytes_per_cycle, latency_cycles, frequency_mhz)
    
    lines.append(f"  L0满载时（{l0_max_size_kb} KB）理论性能：")
    lines.append(f"    时间: {l0_time:.4f} us = {l0_cycles:.2f} cycles")
    lines.append(f"    带宽: {l0_bw:.3f} GB/s")
    lines.append(f"    延迟占比: {latency_cycles/l0_cycles*100:.1f}%")
    
    lines.append("")
    lines.append("=" * 80)
    
    # 写入文件
    with open(output_file, 'w') as f:
        f.write('\n'.join(lines))
    
    return '\n'.join(lines)

def generate_matplotlib_roofline_with_latency(data, output_file):
    """生成 Matplotlib Roofline 图（带延迟模型，限制在64KB）"""
    try:
        import matplotlib.pyplot as plt
        import numpy as np
        
        # 计算数据量
        for item in data:
            item['data_size'] = calculate_data_size(item['m'], item['k'])
            item['data_size_kb'] = item['data_size'] / 1024
        
        # 理论参数
        peak_bw_bytes_per_cycle = 256
        latency_cycles = 30
        frequency_mhz = 1800
        peak_bw_gbps = peak_bw_bytes_per_cycle * frequency_mhz / 1e3
        l0_max_kb = 64  # L0最大容量
        
        # 生成理论曲线（0到64KB）
        theory_sizes = np.linspace(0.1 * 1024, l0_max_kb * 1024, 100)
        theory_bandwidths = []
        theory_times = []
        
        for size in theory_sizes:
            bw, time, cycles = calculate_theoretical_bandwidth(
                size, peak_bw_bytes_per_cycle, latency_cycles, frequency_mhz)
            theory_bandwidths.append(bw)
            theory_times.append(time)
        
        theory_sizes_kb = theory_sizes / 1024
        
        # 创建图表
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
        
        # === 左图：带宽 Roofline ===
        # 绘制峰值带宽线（无延迟）
        ax1.axhline(y=peak_bw_gbps, color='red', linestyle='--', linewidth=2,
                   label=f'Peak BW (no latency): {peak_bw_gbps:.1f} GB/s')
        
        # 绘制理论带宽曲线（带延迟）
        ax1.plot(theory_sizes_kb, theory_bandwidths, 'b-', linewidth=3,
                label=f'Theoretical BW (with {latency_cycles}c latency)')
        
        # 绘制L0容量限制线
        ax1.axvline(x=l0_max_kb, color='purple', linestyle=':', linewidth=2,
                   label=f'L0 Max Size: {l0_max_kb} KB')
        
        # 绘制实际测试数据点
        colors = ['#2ecc71', '#f39c12', '#e74c3c', '#9b59b6', '#3498db', '#1abc9c', '#e67e22']
        markers = ['o', 's', 'D', '^', 'v', '<', '>']
        
        for i, item in enumerate(data):
            # 使用循环索引，避免超出列表范围
            color_idx = i % len(colors)
            marker_idx = i % len(markers)
            
            ax1.scatter(item['data_size_kb'], item['bandwidth_gbps'],
                       c=colors[color_idx], marker=markers[marker_idx], s=250,
                       edgecolors='black', linewidths=2.5, zorder=5,
                       label=f'Shape [{item["m"]},{item["k"]},{item["n"]}]')
            
            # 添加标注
            ax1.annotate(f'{item["bandwidth_gbps"]:.1f} GB/s',
                        xy=(item['data_size_kb'], item['bandwidth_gbps']),
                        xytext=(15, 15), textcoords='offset points',
                        fontsize=11, fontweight='bold',
                        bbox=dict(boxstyle='round,pad=0.5', facecolor='yellow', alpha=0.7))
        
        # 设置坐标轴（限制在64KB）
        ax1.set_xlabel('Data Size (KB) [L0 Max: 64KB]', fontsize=14, fontweight='bold')
        ax1.set_ylabel('Bandwidth (GB/s)', fontsize=14, fontweight='bold')
        ax1.set_title('MTE1 Bandwidth Roofline Model\n(L1 → L0A, Limited by L0 Size)', 
                     fontsize=16, fontweight='bold')
        
        ax1.grid(True, linestyle='--', alpha=0.4)
        ax1.legend(loc='lower right', fontsize=10)
        
        ax1.set_xlim(0, l0_max_kb * 1.1)
        ax1.set_ylim(0, peak_bw_gbps * 1.2)
        
        # 添加L0容量区域说明
        ax1.fill_between([l0_max_kb, l0_max_kb * 1.1],
                        [0, 0],
                        [peak_bw_gbps * 1.2, peak_bw_gbps * 1.2],
                        color='purple', alpha=0.1)
        
        ax1.text(l0_max_kb * 1.05, peak_bw_gbps * 0.5,
               'L0 Limit', fontsize=11, ha='center', rotation=90,
               color='purple', fontweight='bold')
        
        # === 右图：时间分析 ===
        # 绘制理论时间曲线
        ax2.plot(theory_sizes_kb, theory_times, 'b-', linewidth=3,
                label=f'Theoretical Time\n= {latency_cycles}c + Size/{peak_bw_bytes_per_cycle}')
        
        # 绘制延迟基线
        latency_us = latency_cycles / frequency_mhz
        ax2.axhline(y=latency_us, color='orange', linestyle='--', linewidth=2,
                   label=f'Fixed Latency: {latency_cycles} cycles = {latency_us:.3f} us')
        
        # 绘制L0容量限制线
        ax2.axvline(x=l0_max_kb, color='purple', linestyle=':', linewidth=2,
                   label=f'L0 Max Size: {l0_max_kb} KB')
        
        # 绘制实际时间数据点
        for i, item in enumerate(data):
            # 使用循环索引，避免超出列表范围
            color_idx = i % len(colors)
            marker_idx = i % len(markers)
            
            ax2.scatter(item['data_size_kb'], item['time_us'],
                       c=colors[color_idx], marker=markers[marker_idx], s=250,
                       edgecolors='black', linewidths=2.5, zorder=5)
            
            # 计算理论时间
            theory_bw, theory_time, theory_cycles = calculate_theoretical_bandwidth(
                item['data_size'], peak_bw_bytes_per_cycle, latency_cycles, frequency_mhz)
            
            # 添加标注
            ax2.annotate(f'Measured: {item["time_us"]:.3f} us\nTheory: {theory_time:.3f} us',
                        xy=(item['data_size_kb'], item['time_us']),
                        xytext=(15, 15), textcoords='offset points',
                        fontsize=10, fontweight='bold',
                        bbox=dict(boxstyle='round,pad=0.5', facecolor='lightgreen', alpha=0.7))
        
        # 设置坐标轴（限制在64KB）
        ax2.set_xlabel('Data Size (KB) [L0 Max: 64KB]', fontsize=14, fontweight='bold')
        ax2.set_ylabel('Time (us)', fontsize=14, fontweight='bold')
        ax2.set_title('MTE1 Transfer Time Analysis\n(Fixed Latency + Data Transfer)', 
                     fontsize=16, fontweight='bold')
        
        ax2.grid(True, linestyle='--', alpha=0.4)
        ax2.legend(loc='upper left', fontsize=9)
        
        ax2.set_xlim(0, l0_max_kb * 1.1)
        
        # 添加L0容量区域说明
        ax2.fill_between([l0_max_kb, l0_max_kb * 1.1],
                        [0, 0],
                        [max(theory_times) * 1.2, max(theory_times) * 1.2],
                        color='purple', alpha=0.1)
        
        ax2.text(l0_max_kb * 1.05, max(theory_times) * 0.5,
               'L0 Limit', fontsize=11, ha='center', rotation=90,
               color='purple', fontweight='bold')
        
        # 添加模型公式说明
        formula_text = f"Model Parameters:\n"
        formula_text += f"• Peak BW: {peak_bw_bytes_per_cycle} Byte/cycle\n"
        formula_text += f"• Frequency: {frequency_mhz} MHz\n"
        formula_text += f"• Fixed Latency: {latency_cycles} cycles\n"
        formula_text += f"• L0 Max Size: {l0_max_kb} KB\n"
        formula_text += f"• Formula: Time = {latency_cycles} + Size/{peak_bw_bytes_per_cycle}"
        
        fig.text(0.5, 0.02, formula_text, ha='center', fontsize=12,
                bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.8))
        
        plt.tight_layout(rect=(0, 0.08, 1, 1))
        
        # 保存图表
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f"Matplotlib 图表已保存: {output_file}")
        
        png_file = output_file.replace('.pdf', '.png')
        plt.savefig(png_file, dpi=300, bbox_inches='tight')
        print(f"PNG 版本已保存: {png_file}")
        
        return True
        
    except ImportError:
        print("警告: matplotlib 未安装，跳过图表生成")
        return False
    except Exception as e:
        print(f"警告: matplotlib 生成失败 ({e})")
        return False

def find_latest_perf_data():
    """查找最新的 perf_data 目录中的 CSV 文件"""
    perf_dirs = sorted(Path('.').glob('perf_data_*'), reverse=True)
    
    if not perf_dirs:
        return None
    
    latest_dir = perf_dirs[0]
    csv_file = latest_dir / 'perf_result_scenario1.csv'
    
    if csv_file.exists():
        return str(csv_file)
    
    return None

def main():
    # 解析命令行参数
    parser = argparse.ArgumentParser(
        description='生成 MTE1 Bandwidth Roofline 图（带延迟模型）',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""示例用法:
  # 指定 CSV 文件
  python3 generate_roofline_with_latency.py --csv perf_data_20260613_162817/perf_result_scenario1.csv
  
  # 或直接传入路径
  python3 generate_roofline_with_latency.py perf_data_20260613_162817/perf_result_scenario1.csv
  
  # 不指定参数，自动查找最新数据
  python3 generate_roofline_with_latency.py
  
输出文件:
  - mte1_bandwidth_roofline_with_latency.txt (ASCII详细分析)
  - mte1_bandwidth_roofline_with_latency.png (PNG图表)
  - mte1_bandwidth_roofline_with_latency.pdf (PDF矢量图)
""")
    
    parser.add_argument('--csv', '-c', type=str, 
                       help='CSV 文件路径（如果不指定，自动查找最新数据）')
    parser.add_argument('--peak-bw', type=int, default=256,
                       help='峰值带宽（Byte/cycle），默认: 256')
    parser.add_argument('--latency', type=int, default=30,
                       help='固定延迟（cycles），默认: 30')
    parser.add_argument('--frequency', type=int, default=1800,
                       help='主频（MHz），默认: 1800')
    parser.add_argument('--l0-size', type=int, default=64,
                       help='L0最大容量（KB），默认: 64')
    parser.add_argument('--output', '-o', type=str,
                       help='输出文件前缀，默认: mte1_bandwidth_roofline_with_latency')
    
    # 支持直接传入 CSV 文件作为第一个参数
    if len(sys.argv) > 1 and not sys.argv[1].startswith('-'):
        args = parser.parse_args(['--csv', sys.argv[1]])
    else:
        args = parser.parse_args()
    
    # 确定 CSV 文件路径
    csv_file = args.csv
    
    if not csv_file:
        # 自动查找最新数据
        csv_file = find_latest_perf_data()
        if not csv_file:
            print("错误: 未找到 perf_data 目录或 CSV 文件")
            print("提示: 请先运行性能测试脚本，或使用 --csv 参数指定文件")
            parser.print_help()
            sys.exit(1)
        print(f"自动检测到最新数据: {csv_file}")
    
    if not Path(csv_file).exists():
        print(f"错误: CSV 文件不存在: {csv_file}")
        sys.exit(1)
    
    # 获取参数
    peak_bw = args.peak_bw
    latency = args.latency
    frequency = args.frequency
    l0_size = args.l0_size
    
    # 确定输出文件前缀
    if args.output:
        output_prefix = args.output
    else:
        # 根据输入 CSV 文件路径生成输出前缀
        csv_path = Path(csv_file)
        parent_dir = csv_path.parent.name  # perf_data_20260613_162817
        output_prefix = f"{parent_dir}_roofline"
    
    # 读取数据
    print(f"读取数据: {csv_file}")
    data = read_csv_data(csv_file)
    print(f"找到 {len(data)} 条测试数据")
    print()
    
    # 显示理论模型参数
    print("=" * 80)
    print("Roofline 模型参数")
    print("=" * 80)
    print(f"  理论峰值带宽: {peak_bw} Byte/cycle")
    print(f"  主频: {frequency} MHz")
    print(f"  峰值带宽: {peak_bw} × {frequency} / 1e3 = {peak_bw * frequency / 1e3:.2f} GB/s")
    print(f"  固定延迟: {latency} cycles = {latency / frequency:.4f} us")
    print(f"  L0 最大容量: {l0_size} KB")
    print()
    print("理论公式:")
    print(f"  Time(cycles) = {latency} + DataSize(bytes) / {peak_bw}")
    print("  Bandwidth(GB/s) = DataSize(bytes) / Time(us) / 1e3")
    print()
    
    # 生成 ASCII Roofline 图
    ascii_output = f"{output_prefix}.txt"
    print("生成 ASCII Roofline 图...")
    
    # 传递自定义参数到生成函数（需要修改函数签名）
    # 这里简化处理，直接使用默认值（后续可扩展）
    ascii_chart = generate_roofline_with_latency(data, ascii_output)
    print(ascii_chart)
    
    # 尝试生成 Matplotlib 图
    matplotlib_output = f"{output_prefix}.pdf"
    print("\n生成 Matplotlib Roofline 图...")
    generate_matplotlib_roofline_with_latency(data, matplotlib_output)
    
    print("\n" + "=" * 80)
    print("Roofline 图生成完成！")
    print("=" * 80)
    print(f"  输入文件: {csv_file}")
    print(f"  ASCII 版本: {ascii_output}")
    # print(f"  PDF 版本: {matplotlib_output}")
    print(f"  PNG 版本: {matplotlib_output.replace('.pdf', '.png')}")
    print()
    print("使用方法:")
    print(f"  查看 ASCII 报告: cat {ascii_output}")
    print(f"  查看图片: 打开 {matplotlib_output.replace('.pdf', '.png')}")
    print()
    print("自定义参数:")
    print("  如需调整理论参数，可使用:")
    print(f"  python3 {sys.argv[0]} --csv {csv_file} --peak-bw {peak_bw} --latency {latency} --frequency {frequency} --l0-size {l0_size}")

if __name__ == "__main__":
    main()