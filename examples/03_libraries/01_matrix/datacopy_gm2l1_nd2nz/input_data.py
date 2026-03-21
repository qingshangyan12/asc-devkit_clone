import os
import numpy as np
import sys
import argparse

def generate_input_data(scenario_num, matrix_row, matrix_col, input_type):
    if scenario_num == 7:
        # scenario_num=7: 20行32列矩阵，前8行和后8行每行前24个为1，后8个为0
        input_x = np.zeros((20, 32), dtype=input_type)
        # 前8行：前24个为1，后8个为0
        input_x[:8, :24] = 1
        # 后8行：前24个为1，后8个为0
        input_x[12:20, :24] = 1
    else:
        input_x = np.arange(1, matrix_row * matrix_col + 1).reshape(matrix_row, matrix_col)
        input_x = input_x.astype(input_type)
        if scenario_num == 4 or scenario_num == 5 or scenario_num == 6:
            # 列非对齐情况，每行最后两列设为0
            input_x[:, -8:] = 0
    print(input_x)

# 调用函数，指定 scenario_num=7, matrix_row=20, matrix_col=32, input_type='half'
if __name__ == "__main__":
    generate_input_data(7, 20, 32, 'half')