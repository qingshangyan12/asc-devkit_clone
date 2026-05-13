# SIMT编程模式实现一维Gather算子样例

## 概述

样例基于Ascend C SIMT编程方式实现简单场景（固定shape的）一维Gather算子，从一维输入中采集指定索引的元素，展示简化场景离散内存访问类算子的开发方法。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```
├── gather_1d
│   ├── gather_1d.asc          # SIMT实现一维gather调用样例
│   ├── CMakeLists.txt
|   └── README.md
```

## 算子描述

- 算子功能：  
  gather_1d算子实现了从长度为100000的一维输入向量中，根据index中每个元素获取1个数据的功能。算子输出output第i个元素计算公式为：

  ```
  output[i] = input[index[i]]
  ```

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">gather_1d</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">100000</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">index</td><td align="center">12288</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">12288</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">gather_1d_custom</td></tr>
  </table>

- 数据切分：  
  * Block数量：根据index长度和AI Vector Core数量自动计算，当前样例运行为6个block
  * 每个 block 线程数：2048线程
  * 总线程数：6 * 2048 = 12288线程
  * 单线程处理：通过grid-stride循环处理一个或多个元素
  * 结果预期：输出第i个元素等于输入的第index[i]个元素

- 算子实现：  
  gather_1d算子的实现流程为从输入input（Global Memory）中获取指定索引的数据。Kernel中单线程根据index[i]读取1个元素，并写入output[i]。

- 调用实现：  
  使用内核调用符<<<>>>启动Kernel。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量   
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  ./gather_1d                   # 执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```
  [INFO] Block number is 6.
  [INFO] Thread number in a block is 2048.
  Output: 15.6 132 248.4 364.8 481.2 597.6 714 830.4 946.8 1063.2 1179.6 1296 1412.4 1528.8 1645.2 1761.6 1878 1994.4 2110.8 2227.2 ...
  Golden: 15.6 132 248.4 364.8 481.2 597.6 714 830.4 946.8 1063.2 1179.6 1296 1412.4 1528.8 1645.2 1761.6 1878 1994.4 2110.8 2227.2 ...
  [Success] Case accuracy is verification passed.
  ```
