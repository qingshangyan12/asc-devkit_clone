# SIMT Grid Config样例

## 概述

本样例基于Ascend C SIMT和SIMD混合编程方式，展示SIMT编程下的Grid/Block配置和vf函数调用优化方法。样例包含两种核函数：空壳核函数（仅做DataCopy搬运）和混合核函数（Gather+Adds融合计算），通过7种不同核数和vf调用次数的组合展示性能优化策略。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- >= CANN 9.0.0

## 目录结构

```text
├── 02_simt_grid_config
│   ├── CMakeLists.txt      // 编译工程文件
│   ├── grid_config.asc     // SIMT Grid Config样例实现
│   └── README.md
```

## 算子描述

- 算子功能：  
  样例提供两种核函数实现：
  1. **空壳核函数（copy_only_kernel）**：simt和simd vf函数为空实现，仅做DataCopy搬运，将input直接搬到output
  2. **混合核函数（gather_adds_kernel）**：实现Gather+Adds融合计算，`output[i][j] = input[index[i]][j] + 1.0`

- 算子规格：  

  <table>
  <tr><th align="left">场景编号(SCENARIO_NUM)</th><th align="left">核数</th><th align="left">每核线程数</th><th align="left">vf调用次数</th><th align="left">总线程数</th><th align="left">核函数类型</th></tr>
  <tr><td align="left">1</td><td align="left">36</td><td align="left">1024</td><td align="left">1</td><td align="left">36864</td><td align="left">copy_only</td></tr>
  <tr><td align="left">2</td><td align="left">64</td><td align="left">1024</td><td align="left">1</td><td align="left">65536</td><td align="left">copy_only</td></tr>
  <tr><td align="left">3</td><td align="left">128</td><td align="left">1024</td><td align="left">1</td><td align="left">131072</td><td align="left">copy_only</td></tr>
  <tr><td align="left">4</td><td align="left">1024</td><td align="left">1024</td><td align="left">1</td><td align="left">1048576</td><td align="left">copy_only</td></tr>
  <tr><td align="left">5</td><td align="left">36</td><td align="left">1024</td><td align="left">2</td><td align="left">36864</td><td align="left">copy_only</td></tr>
  <tr><td align="left">6</td><td align="left">1024</td><td align="left">1024</td><td align="left">2</td><td align="left">1048576</td><td align="left">copy_only</td></tr>
  <tr><td align="left">7</td><td align="left">64</td><td align="left">1024</td><td align="left">14</td><td align="left">65536</td><td align="left">copy_only</td></tr>
  </table>

  输入张量shape为[1024, 256]，数据类型为float；索引长度由场景决定。

- vf函数设计：  
  * **空壳vf函数**：
    - `simt_copy_dummy`：__simt_vf__空实现
    - `simd_copy_dummy`：__simd_vf__空实现
  * **混合vf函数**：
    - `simt_gather`：__simt_vf__函数，从Global Memory按索引采集数据到UB
    - `simd_adds`：__simd_vf__函数，对UB中的数据加上固定偏移值1.0
  * 核函数中通过`asc_vf_call`调用vf函数，支持多次调用以实现流水线优化

- 算子实现：  
  **空壳核函数**实现流程：
  1. 调用空壳vf函数（占位）
  2. 通过DataCopy将input搬运到UB
  3. 通过DataCopy将UB数据搬运到output
  
  **混合核函数**实现流程：
  1. 调用`simt_gather`进行SIMT gather操作
  2. 调用`simd_adds`进行SIMD加法运算
  3. 将结果从UB搬运到Global Memory

- 调用实现：  
  使用内核调用符<<<>>>调用核函数，dynUBufSize配置为0。

## Grid/Block和vf配置说明

本样例采用固定Block配置，动态Grid和vf调用配置：

- **Block配置**：固定为`dim3(1024, 1, 1)`，每核1024个线程
- **Grid配置**：根据场景动态调整核数（36/64/128/1024）
- **vf调用次数**：基础场景为1次，优化场景为2或14次
- **线程映射**：每个vf调用处理部分数据行，通过多次调用实现流水优化

配置特点：
1. **空壳核函数基准测试**：场景1-7使用空壳核函数，仅做DataCopy搬运，用于建立性能基线
2. **多次vf调用优化**：场景5-7使用多次vf调用，展示流水线深度优化效果
3. **固定线程数**：每核1024个线程，便于对比不同核数的性能差异

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
  SCENARIO_NUM=1                 # 选择场景（1-7）
  mkdir -p build && cd build;    # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;  # 编译工程
  ./grid_config                  # 执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `SCENARIO_NUM` | `1`-`7` | 场景编号，默认为1 |