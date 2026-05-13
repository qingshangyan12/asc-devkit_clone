# Cache访问优化样例

## 概述

本样例展示如何使用 `asc_ldcg`、`asc_ldca` 和 `asc_stcg` 数据缓存加载函数来优化查表计算性能。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- \> CANN 9.0.0

## 目录结构

```
├── 02_simt_data_cache_hint
│   ├── CMakeLists.txt              # cmake编译文件
│   ├── simt_data_cache_hint.asc    # Ascend C算子实现 & 调用样例
│   └── README.md
```

## 算子描述

### 算子功能

使用查表法计算sin值，通过线性插值提高精度。


### 算子规格

| 算子类型（OpType） | sin_table_lookup                                       |
|--------------|--------------------------------------------------------|
| 输入           | input: (65536,), float                                 |
| 输入           | sin_table: (8192,), float                              |
| 输出           | output: (65536,), float                                |
| 核函数名         | sin_table_lookup_baseline / sin_table_lookup_optimized |

### 算法实现

1. **Host侧数据准备**：
   - 生成均匀分布在 `[0, 16π]` 区间的输入数据（256KB）
   - 生成 `[0, π]` 的sin查找表（32KB）

2. **Kernel侧计算**：
   ```c++
   index_float = x / PI * table_length
   n = floor(index_float)
   frac = index_float - n
   low_val = table[n]
   high_val = table[n+1]
   y = low_val + frac * (high_val - low_val)
   ```

3. **优化对比**：
   - **基线版本**：直接读取表数据，直接写入输出，数据同时竞争Dcache空间
   - **优化版本**：使用 `asc_ldcg` 加载输入数据，使用 `asc_ldca` 加载输入表数据，使用 `asc_stcg` 写入结果

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
  msprof op ./simt_data_cache_hint_baseline  # 执行基线样例
  msprof op ./simt_data_cache_hint_optimized  # 执行优化样例
  ```

执行结果如下：
```
Usage: ./simt_data_cache_hint_baseline (compile-time SCENARIO_NUM=0)
Running case 0: baseline...
=== Running Kernel ===
[Success] Case accuracy is verification passed.
```
和
```
Usage: ./simt_data_cache_hint_optimized (compile-time SCENARIO_NUM=1)
Running case 1: optimized...
=== Running Kernel ===
[Success] Case accuracy is verification passed.
```

**注意**：通过编译时定义SCENARIO_NUM宏来选择运行基线版本（0）或优化版本（1）。

## 性能分析

### 实现对比

| Case   | 核函数                        | 内存访问方式                                      | 缓存策略                                |
|--------|----------------------------|---------------------------------------------|-------------------------------------|
| Case 0 | sin_table_lookup_baseline  | 所有数据使用普通加载/存储指令                             | 输入、输出和sin表数据共同竞争DCache空间            |
| Case 1 | sin_table_lookup_optimized | 输入：`asc_ldcg`，sin表：`asc_ldca`，输出：`asc_stcg` | sin表数据优先缓存在DCache，减少输入/输出对DCache的占用 |

### 性能数据

| Case   | Task Duration(us) | DCache Read GM | DCache Read Vector | DCache Write Vector |
|--------|-------------------|----------------|--------------------|---------------------|
| Case 0 | 56.82              | 5064            | 2048                | 6144                 |
| Case 1 | 50.895              | 3531            | 2048                | 6144                 |

**性能提升**：
- **执行时间**：从56.82us降至50.895us，**提升约10.4%**
- **DCache Read GM**：从5064降至3531，**减少约30.2%**（减少1533次Global Memory访问）

### 优化原理

**问题**：在baseline版本中，所有内存访问（输入数据、sin表和输出数据）会同时竞争DCache空间。当访问大量输入和输出数据时，sin查找表的数据会被换出DCache，导致后续查表时需要重新从Global Memory加载，增加访存延迟。

**解决方案**：使用数据缓存提示函数优化访存策略：
- **`asc_ldcg`**：用于加载输入数据。输入数据每个元素仅访问一次，使用`asc_ldcg`加载数据，这些数据不需要占用Cache空间
- **`asc_ldca`**：用于加载sin表数据。sin表是热点数据，需要频繁访问，使用`asc_ldca`加载数据，优先为这些数据分配Cache空间，确保常驻Cache
- **`asc_stcg`**：用于写入输出数据。输出数据写入后不需要立即占用Cache空间，使用`asc_stcg`缓存数据，这些数据直接写入Global Memory，不进攻Cache缓存

**效果**：通过区分不同数据的缓存策略，确保频繁访问的sin查找表常驻DCache，减少Global Memory访问次数（从5064降至3531），整体性能提升约10.4%。

### 性能数据分析

**1. DCache Read GM减少约30.2%（从5064降至3531）**

这是性能提升的关键因素。减少的1533次Global Memory访问主要来自：

- **输入数据访问优化**：使用`asc_ldcg`加载输入数据，减少输入数据对DCache空间的占用
- **sin表访问优化**：使用`asc_ldca`加载sin表，确保sin表优先缓存在DCache中
- **Cache命中率提升**：sin表数据常驻DCache，避免被输入/输出数据挤出，减少了从Global Memory重新加载sin表的次数

**总结**：优化通过减少DCache Read GM次数（降低Global Memory访问延迟），在不增加其他开销的前提下实现约10.4%的性能提升。
