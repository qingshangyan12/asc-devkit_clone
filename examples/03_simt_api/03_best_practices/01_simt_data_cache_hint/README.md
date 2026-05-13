# SIMT数据缓存提示优化样例

## 概述

本样例展示如何使用 `asc_ldcg` 和 `asc_stcg` 数据缓存提示函数优化查表计算性能。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- \> CANN 9.0.0

## 目录结构

```
├── 01_simt_data_cache_hint
│   ├── CMakeLists.txt              # cmake编译文件
│   ├── simt_data_cache_hint.asc    # Ascend C算子实现 & 调用样例
│   └── README.md
```

## 背景知识

### 数据缓存提示函数

在NPU编程中，合理利用数据缓存可以显著提升性能。Ascend C SIMT提供了以下数据缓存提示函数：

- `asc_ldcg`: 从Global Memory加载数据，提示硬件数据可能被缓存
- `asc_stcg`: 将数据存储到Global Memory，提示硬件数据可能被缓存

这些函数可以在特定场景下优化内存访问性能，特别是对于频繁访问的数据或需要写回的数据。

## 算子描述

### 算子功能

使用查表法计算sin值，通过线性插值提高精度。

### 数据规格

- 输入数据大小：16KB
- Sin查找表大小：16KB

### 算法流程

1. **Host侧数据准备**：
   - 生成均匀分布在 `[0, π]` 区间的输入数据（16KB）
   - 生成 `[0, π]` 的sin查找表（16KB）

2. **Kernel侧计算**：
   ```
   index_float = x / π * table_length
   n = floor(index_float)
   frac = index_float - n
   low_val = table[n]
   high_val = table[n+1]
   y = low_val + frac * (high_val - low_val)
   ```

3. **优化对比**：
   - **基线版本**：直接读取表数据，直接写入输出
   - **优化版本**：使用 `asc_ldcg` 加载表数据，使用 `asc_stcg` 写入结果

### 算子规格

| 项目 | 说明 |
|------|------|
| 算子类型 | sin_table_lookup |
| 输入 | input: (4096,), float; sin_table: (4096,), float |
| 输出 | output: (4096,), float |
| 核函数名 | sin_table_lookup_baseline / sin_table_lookup_optimized |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量   
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
    
- 样例执行
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..; make -j;            # 编译工程
  ./demo                        # 执行样例
  ```

执行结果如下：
```
Input length: 4096
Sin table length: 4096

=== Running Baseline Kernel ===
[Success] All results are correct.

=== Precision Analysis (vs std::sin) ===
Table length: 4096
Table step: 0.000767 rad
Max error: ~0.0000003
Average error: ~0.0000001
Relative max error: ~0.00003%

=== Running Optimized Kernel (with asc_ldcg/asc_stcg) ===
[Success] All results are correct.

=== Precision Analysis (vs std::sin) ===
Table length: 4096
Table step: 0.000767 rad
Max error: ~0.0000003
Average error: ~0.0000001
Relative max error: ~0.00003%
```

### 精度分析

查表法使用线性插值计算sin值，精度分析如下：

| 参数 | 值 |
|------|------|
| 表长度 | 4096 |
| 表步长 | π/4096 ≈ 0.000767 rad |
| 最大误差 | ~3e-7 |
| 平均误差 | ~1e-7 |

线性插值误差公式：`误差 ≈ (步长)^2 × max|sin''(x)| / 2 = (π/4096)^2 × 0.5 ≈ 3×10^-7`