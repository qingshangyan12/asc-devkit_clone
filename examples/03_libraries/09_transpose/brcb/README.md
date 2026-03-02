# Brcb样例
## 概述
本样例介绍了在 uint16_t 数据类型下，目的操作数连续 / 非连续场景中，Brcb指令及相关参数的使用方法。

## 支持的产品
- Ascend 950PR/Ascend 950DT //TBD
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
## 目录结构介绍
```
├── brcb
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── brcb.asc      // Ascend C算子实现 & 调用样例
```

## 算子描述  
   BrcbCustom算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor srcGm存储在srcLocal中，Compute任务负责从srcLocal每一次取输入张量中的8个数填充到dstLocal的8个datablock（32Bytes）中去，每个数分别对应一个datablock，CopyOut任务负责将输出数据从dstLocal搬运至Global Memory上的输出Tensor dstGm。

  本样例中实现的是数据类型为uint16_t，固定shape为输入x[16]，输出y[256]的BrcbCustom算子，算子规格信息如下表1所示：

<table border="2" align="center">
<caption>表1：目的操作数连续算子规格信息</caption>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Brcb</td></tr>
  <tr><td rowspan="3" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">16</td><td align="center">uint16_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">算子输出</td></tr>
  <tr><td align="center">y</td><td align="center">256</td><td align="center">uint16_t</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">brcb_custom</td></tr>
  </table>

  对于不同数据类型，配置参数均以DataBlock为单位，可以共用一套配置，因此下面两个小节的描述不仅仅适用于输入为uint6_t类型，也适用于输入为Brcb的其他任意支持类型。

### 一.目的操作数 迭代内连续且迭代间连续
  配置BrcbRepeatParams结构体的成员使得目的操作数 迭代内连续且迭代间连续时，dstBlkStride和dstRepStride的单位为32字节，参数取值为DataBlock间步长和迭代内DataBlock的数目。

            brcbRepeatParams.dstBlkStride = 1;
            brcbRepeatParams.dstRepStride = 8;

### 二.目的操作数 迭代内非连续但迭代间连续
  配置BrcbRepeatParams结构体的成员使得目的操作数 迭代内非连续但迭代间连续时，dstBlkStride和dstRepStride的单位为32字节，参数取值为迭代次数和DataBlock间步长。

            brcbRepeatParams.dstBlkStride = 2;
            brcbRepeatParams.dstRepStride = 1;


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
  SCENARIO=1
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;             # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO  # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```