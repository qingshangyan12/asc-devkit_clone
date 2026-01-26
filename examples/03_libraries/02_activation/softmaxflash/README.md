# SoftmaxFlash样例

## 概述

本样例基于Kernel直调算子工程，介绍了调用SoftmaxFlashV2高阶api实现softmaxflash单算子，Softmax增强版本，除了可以对输入tensor做SoftmaxFlash计算，还可以根据上一次Softmax计算的sum和max来更新本次的Softmax计算结果。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── softmaxflash
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── softmaxflash.asc        // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  
  softmaxflash单算子，Softmax增强版本，除了可以对输入tensor做SoftmaxFlash计算，还可以根据上一次Softmax计算的sum和max来更新本次的Softmax计算结果。last轴切轴的情况，每次计算的reduce结果并非是全轴的，需要根据上一次Softmax计算的sum和max来更新本次的Softmax计算结果，可以使用该增强接口。
- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">SoftmaxflashCustom</td></tr>

  <tr><td rowspan="3" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center"> 960*960 </td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="3" align="center">算子输出</td></tr>
  <tr><td align="center">max</td><td align="center"> 960*8 </td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">sum</td><td align="center"> 960*8 </td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">softmaxflash_custom</td></tr>
  </table>

- 算子实现：  
  本样例中实现的是固定shape为输入x [960, 960], 输出max[960, 8], sum[960, 8]的softmaxflash算子。

  - kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用SoftmaxFlashV2高阶API接口完成softmaxflash计算，得到最终结果，再搬出到外部存储上。

    softmaxflash算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor xGm搬运至Local Memory，存储在xLocal，Compute任务负责对xLocal执行softmaxflash计算，计算结果存储在maxLocal和sumLocal中，CopyOut任务负责将输出数据maxLocal和sumLocal搬运至Global Memory上的输出Tensor maxGm和sumGm中。

  - tiling实现

    softmaxflash算子的tiling实现流程如下：首先对shape按照行数进行分核，使用平均分配法先按照核数向上对齐分配，确定主核的计算行数，再确定尾核计算行数，对主核计算的shape调用SoftmaxFlashV2高阶API的tiling函数获取API所需tiling参数，尾核计算所需的高阶API的tiling由kernel侧自行计算。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

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
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```