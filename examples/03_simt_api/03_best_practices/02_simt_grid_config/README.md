# SIMT Grid Config样例

## 概述

本样例基于Ascend C SIMT和SIMD混合编程方式实现Gather+Adds算子，展示SIMT编程下的Grid/Block配置和vf函数调用优化方法。样例使用vf函数拆分SIMT gather和SIMD adds操作，通过10种不同核数和vf调用次数的组合展示性能优化策略。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- >= CANN 9.0.0

## 目录结构

```text
├── 02_simt_grid_config
│   ├── CMakeLists.txt      // 编译工程文件
│   ├── grid_config.asc     // SIMT Gather+Adds样例实现
│   └── README.md
```

## 算子描述

- 算子功能：  
  Gather+Adds算子实现从二维张量中获取指定索引的数据行，并对每行加上对应的偏移值。算子输出output第i行数据计算公式为：
  
  ```
  output[i][j] = input[index[i]][j] + add_val[i]
  ```

- 算子规格：  

  **基础场景（单次vf调用）**：
  <table>
  <tr><th align="left">场景编号(SCENARIO_NUM)</th><th align="left">核数</th><th align="left">每核线程数</th><th align="left">vf调用次数</th><th align="left">总线程数</th><th align="left">处理数据量</th></tr>
  <tr><td align="left">1</td><td align="left">1</td><td align="left">1024</td><td align="left">1</td><td align="left">1024</td><td align="left">1024×128</td></tr>
  <tr><td align="left">2</td><td align="left">36</td><td align="left">1024</td><td align="left">1</td><td align="left">36864</td><td align="left">36864×128</td></tr>
  <tr><td align="left">3</td><td align="left">72</td><td align="left">1024</td><td align="left">1</td><td align="left">73728</td><td align="left">73728×128</td></tr>
  <tr><td align="left">4</td><td align="left">140</td><td align="left">1024</td><td align="left">1</td><td align="left">143360</td><td align="left">143360×128</td></tr>
  <tr><td align="left">5</td><td align="left">1000</td><td align="left">1024</td><td align="left">1</td><td align="left">1024000</td><td align="left">1024000×128</td></tr>
  </table>

  **优化场景（多次vf调用）**：
  <table>
  <tr><th align="left">场景编号(SCENARIO_NUM)</th><th align="left">核数</th><th align="left">每核线程数</th><th align="left">vf调用次数</th><th align="left">总线程数</th><th align="left">处理数据量</th></tr>
  <tr><td align="left">6</td><td align="left">36</td><td align="left">1024</td><td align="left">2</td><td align="left">36864</td><td align="left">36864×128</td></tr>
  <tr><td align="left">7</td><td align="left">72</td><td align="left">1024</td><td align="left">2</td><td align="left">73728</td><td align="left">73728×128</td></tr>
  <tr><td align="left">8</td><td align="left">72</td><td align="left">1024</td><td align="left">14</td><td align="left">73728</td><td align="left">73728×128</td></tr>
  <tr><td align="left">9</td><td align="left">140</td><td align="left">1024</td><td align="left">2</td><td align="left">143360</td><td align="left">143360×128</td></tr>
  <tr><td align="left">10</td><td align="left">1000</td><td align="left">1024</td><td align="left">2</td><td align="left">1024000</td><td align="left">1024000×128</td></tr>
  </table>

  输入张量shape为[1050000, 128]，数据类型为float；索引和add_val长度由场景决定，核函数名为`gather_adds_kernel`。

- vf函数设计：  
  * `simt_gather`：__simt_vf__函数，从Global Memory按索引采集数据到UB
  * `simd_adds`：__simd_vf__函数，对UB中的数据进行SIMD加法运算
  * 核函数中通过`asc_vf_call`调用vf函数，支持多次调用以实现流水线优化

- 算子实现：  
  Gather+Adds算子实现流程：
  1. 每个block根据vf调用次数分批次处理数据
  2. 每批次调用`simt_gather`进行SIMT gather操作
  3. 调用`simd_adds`进行SIMD加法运算
  4. 将结果从UB搬运到Global Memory

- 调用实现：  
  使用内核调用符<<<>>>调用核函数，通过`asc_vf_call`调用vf函数。

## Grid/Block和vf配置说明

本样例采用固定Block配置，动态Grid和vf调用配置：

- **Block配置**：固定为`dim3(1024, 1, 1)`，每核1024个线程
- **Grid配置**：根据场景动态调整核数（1/36/72/140/1000）
- **vf调用次数**：基础场景为1次，优化场景为2或14次
- **线程映射**：每个vf调用处理部分数据行，通过多次调用实现流水优化

配置特点：
1. **vf函数拆分**：SIMT和SIMD操作分离，便于流水线优化
2. **多次vf调用**：场景8使用14次vf调用，展示流水线深度优化效果
3. **固定线程数**：每核1024个线程，便于对比不同核数的性能差异
4. **基础vs优化对比**：场景1-5为单次vf调用的基础版本，场景6-10为多次vf调用的优化版本

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
  SCENARIO_NUM=1                 # 选择场景（1-10）
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
  | `SCENARIO_NUM` | `1`-`10` | 场景编号，默认为1 |