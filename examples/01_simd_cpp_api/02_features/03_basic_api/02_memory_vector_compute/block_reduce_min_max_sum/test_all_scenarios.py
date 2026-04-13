#!/usr/bin/python3
# coding=utf-8

import os
import subprocess
import sys
import shutil

def run_command(cmd, description):
    """运行命令并处理错误"""
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"错误: {description}")
        print(result.stderr)
        return False
    return True

def test_scenario(scenario_num, run_mode, npu_arch):
    """测试单个场景的某个模式"""
    print(f"测试 场景{scenario_num} {run_mode} {npu_arch}")
    
    os.environ['SCENARIO_NUM'] = str(scenario_num)
    
    if not os.path.exists('build'):
        os.makedirs('build')
    else:
        shutil.rmtree('build')
        os.makedirs('build')

    if run_mode == 'npu':
        cmake_cmd = f"cd build && cmake .. -DSCENARIO_NUM={scenario_num} -DCMAKE_ASC_ARCHITECTURES={npu_arch} && make -j"
    else:
        cmake_cmd = f"cd build && cmake .. -DSCENARIO_NUM={scenario_num} -DCMAKE_ASC_RUN_MODE={run_mode} -DCMAKE_ASC_ARCHITECTURES={npu_arch} && make -j"
    
    if not run_command(cmake_cmd, f"编译 场景{scenario_num}"):
        return False
    
    gen_data_cmd = f"cd build && python3 ../scripts/gen_data.py -scenarioNum={scenario_num}"
    if not run_command(gen_data_cmd, f"生成数据 场景{scenario_num}"):
        return False
    
    run_cmd = f"cd build && ./demo"
    if not run_command(run_cmd, f"运行 场景{scenario_num}"):
        return False
    
    verify_cmd = f"cd build && python3 ../scripts/verify_result.py output/output.bin output/golden.bin"
    if not run_command(verify_cmd, f"验证 场景{scenario_num}"):
        return False
    
    print(f"通过！！！！")
    return True

def main():
    """主函数：测试所有场景的所有模式"""
    scenarios = [1, 2, 3]
    
    test_configs = []
    
    for scenario in scenarios:
        test_configs.append((scenario, 'npu', 'dav-2201'))
        test_configs.append((scenario, 'cpu', 'dav-2201'))
        test_configs.append((scenario, 'sim', 'dav-2201'))
        test_configs.append((scenario, 'sim', 'dav-3510'))
    
    print(f"总测试数: {len(test_configs)}")
    
    if os.path.exists('build'):
        subprocess.run("rm -rf build", shell=True)
    
    success_count = 0
    fail_count = 0
    failed_tests = []
    
    for scenario, mode, arch in test_configs:
        if test_scenario(scenario, mode, arch):
            success_count += 1
        else:
            fail_count += 1
            failed_tests.append(f"场景{scenario}_{mode}_{arch}")
    
    print(f"\n总结: 总数{len(test_configs)} 成功{success_count} 失败{fail_count}")
    
    if failed_tests:
        print("失败的测试:")
        for test in failed_tests:
            print(f"  {test}")
    
    if fail_count == 0:
        print("所有测试通过！")
        return 0
    else:
        print(f"{fail_count}个测试失败！")
        return 1

if __name__ == "__main__":
    sys.exit(main())
