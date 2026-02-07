# 调用模式0的CrossCoreSetFlag指令，完成多个AI Core中所有AIC之间的同步控制
## 概述
本样例介绍了如何使用CrossCoreSetFlag指令，完成多个AI Core中所有AIC之间的同步控制。

本样例通过L1搬运到GM上时开启原子累加，等到所有AIC搬运到GM完成后，再搬运到AIC检查累加结果的计算逻辑，来证明同步的可靠性。如果不插入同步，用户会在检查的过程中发现每个AIC的累加结果不符合预期，且每次的结果都不一致。如果同步插入正确，那么每个AIC搬运的结果都是确定性且正确的。

各指令参数配置原因及执行指令前后的表现，均配合示意图进行了说明。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── cross_core_set_flag_mode0_aic
│   ├── CMakeLists.txt                              // 编译工程文件
│   └── cross_core_set_flag_mode0_aic.asc           // Ascend C算子实现 & 调用样例
```

## 算子描述
算子完整的计算逻辑如下图所述：
本算子设置BLOCK_DIM为8，AIC:AIV配比为2，即总共起了8个AIC和16个AIV，AIC的BlockIdx范围为0~7。总共分2块GM，一块用于存储输入数据(initialDataGm)，一块用于存储所有AIC的累加结果(atomicResultGm)。
(1) 每个AIC均从initialDataGm上搬运全为1的数据到L1上，即为PIPE_MTE2的流水操作。
(2) 将从initialDataGm上搬运来的数据从AIC直接搬运到atomicResultGm上执行累加计算，即为PIPE_MTE3的流水操作。atomicResultGm上的结果预期为8个AIC全部搬运完后的累加结果。此时调用CrossCoreSetFlag接口，保证所有AIC都执行完PIPE_MTE3的搬运指令后，每个AIC再继续执行后续指令。
(3) 上述同步完成之后，此时atomicResultGm已经是8个AIC的累加结果。从atomicResultGm搬运到AIC上，用DumpTensor进行打印数据，检查累加结果是否符合预期。

注意：
如果第2步的同步插入不正确，那么第3步中的从atomicResultGm往AIC的搬运即无法保证时序，可能在其他AIC未完成累加时就开始搬运，导致DumpTensor的结果不准确。用户可以注释掉下述代码段中的CrossCoreSetFlag和CrossCoreWaitFlag调用，会发现累加结果不符合预期，并且每次运行结果都不一致。



        // Process代码段：
        AscendC::DataCopy(atomicResultGm, xLocal, this->blockLength);   // DataCopy属于PIPE_MTE3流水操作

        AscendC::CrossCoreSetFlag<0, PIPE_MTE3>(0);  // 当本AIC完成前置PIPE_MTE3(DataCopy)流水操作后，通知其他AIC核，本AIC已经完成
        AscendC::CrossCoreWaitFlag(0);               // 阻塞本AIC继续往下执行指令，直到其他AIC全部都完成PIPE_MTE3流水操作，才解除阻塞往下执行。



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
  # 在build目录执行以下内容
  ./demo                         # 执行样例
  ```
  执行结果如下，说明精度对比成功。并且每个AIC DumpTensor打印出的数据均为8。
  ```bash
  [Success] Case accuracy is verification passed.
  ```