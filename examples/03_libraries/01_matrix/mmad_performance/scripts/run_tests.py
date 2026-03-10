import csv
import subprocess
import sys
import os
import re
import glob
import ntpath  # 跨平台路径处理

# ===================== 全局配置（写死SOC_VERSION） =====================
SOC_VERSION = "Ascend910B1"

# ===================== 解析gen_data.py输出的耗时数据 =====================
def parse_duration_output(target_col="aic_mac_time(us)"):
    try:
        prof_dirs = glob.glob("./PROF_*")
        if not prof_dirs:
            raise FileNotFoundError("未找到PROF开头的文件夹")
        
        prof_dirs.sort(reverse=True)
        latest_prof_dir = prof_dirs[0]
        print(f"找到最新的PROF文件夹: {latest_prof_dir}")
        
        op_summary_files = glob.glob(os.path.join(latest_prof_dir, "mindstudio_profiler_output/op_summary_*.csv"))
        if not op_summary_files:
            raise FileNotFoundError(f"在{latest_prof_dir}中未找到op_summary_开头的CSV文件")
        
        op_summary_file = op_summary_files[0]
        print(f"读取耗时数据文件: {op_summary_file}")
        
        target_time = None
        with open(op_summary_file, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            if len(lines) < 2:
                raise ValueError("op_summary CSV文件无有效数据行")
            
            header_line = lines[0].strip()
            headers = [h.strip() for h in header_line.split(',')]
            if target_col not in headers:
                raise KeyError(f"CSV文件中未找到目标列: {target_col}")

            col_index = headers.index(target_col)

            last_line = lines[-1].strip()
            last_line_data = [d.strip() for d in last_line.split(',')]
            if len(last_line_data) <= col_index:
                raise IndexError(f"最后一行数据中无{target_col}列值")
            
            target_time = last_line_data[col_index]
            print(f"解析到{target_col}: {target_time}")
        
        return target_time if target_time else None
    
    except Exception as e:
        print(f"❌ 解析耗时数据失败: {str(e)}", file=sys.stderr)
        return None

# ===================== 执行单个测试用例 =====================
def run_test_case(case_row, row_index, all_cases, target_col):
    scenario = case_row['scenario'].strip()
    scenario_desc = case_row['scenario_desc'].strip()
    m = case_row['m'].strip()
    n = case_row['n'].strip()
    k = case_row['k'].strip()
    
    print("\n========================================")
    print(f"执行测试用例: {scenario_desc}")
    print(f"参数: Scenario={scenario}, M={m}, N={n}, K={k}")
    print(f"SOC_VERSION: {SOC_VERSION}")
    print("========================================")
    
    try:
        cmd = [
            "msprof", "bash", "./run.sh",
            "-r", "npu",
            "-v", SOC_VERSION,
            "-b", "Release",
            "-c", "OFF",
            "-d", "cube",
            scenario, m, n, k
        ]
        print(f"执行命令: {' '.join(cmd)}")
        result2 = subprocess.run(cmd, check=True, capture_output=True, text=True)
        gen_output = result2.stdout.strip()
        print(f"gen_data.py 输出:\n{gen_output}")
        
        mmad_time = parse_duration_output(target_col)
        if mmad_time is None:
            print(f"⚠️ 警告：未解析到{target_col}数据！")
        else:
            all_cases[row_index]['duration(us)'] = mmad_time

        print(f"✅ 用例 [{scenario_desc} (m={m},n={n},k={k})] 执行成功\n")
        return True
    
    except subprocess.CalledProcessError as e:
        print(f"❌ 用例 [{scenario_desc} (m={m},n={n},k={k})] 执行失败")
        print(f"错误信息: {e.stderr}")
        print("终止脚本执行\n")
        sys.exit(1)

# ===================== 读取CSV并遍历所有行执行用例 =====================
def load_and_run_cases(case_file_path, target_col):
    if not os.path.exists(case_file_path):
        print(f"错误：找不到测试用例文件 {case_file_path}!")
        sys.exit(1)
    
    all_cases = []
    with open(case_file_path, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        fieldnames = reader.fieldnames
        for row in reader:
            all_cases.append(row)
    
    print("\n---------- 开始执行所有测试用例 ----------")
    for idx, case in enumerate(all_cases):
        run_test_case(case, idx, all_cases, target_col)

    with open(case_file_path, 'w', encoding='utf-8', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(all_cases)
    print(f"✅ 所有测试结果已回填到 {case_file_path}")

# ===================== 主函数 =====================
if __name__ == "__main__":
    # 支持命令行传参，示例：python your_script.py ./cases/mmad_datasize_uint8.csv
    if len(sys.argv) > 1:
        case_file_path = sys.argv[1]
        target_col = sys.argv[2]
    else:
        # 默认路径
        case_file_path = "./cases/mmad_datasize.csv"
        target_col = "aic_mac_time(us)"
    load_and_run_cases(case_file_path, target_col)