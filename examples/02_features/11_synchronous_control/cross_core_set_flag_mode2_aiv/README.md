# 调用模式2的CrossCoreSetFlag指令，完成单个AI Core中AIV与AIC之间(AIV Set, AIC Wait)的同步控制
## 概述
本样例介绍了如何使用CrossCoreSetFlag指令，完成单个AI Core中AIV与AIC之间(AIV Set, AIC Wait)的同步控制。

本样例通过UB搬运到GM上时开启原子累加，等到所有AIV搬运到GM完成后，再搬运到AIC检查累加结果的计算逻辑，来证明同步的可靠性。如果不插入同步，用户会在检查的过程中发现AIC的搬运结果不符合预期，且每次的结果都不一致。如果同步插入正确，那么每个AIC搬运的结果都是确定性且正确的。

各指令参数配置原因及执行指令前后的表现，均配合示意图进行了说明。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── cross_core_set_flag_mode2_aiv
│   ├── CMakeLists.txt                              // 编译工程文件
│   └── cross_core_set_flag_mode2_aiv.asc           // Ascend C算子实现 & 调用样例
```

## 算子描述
算子完整的计算逻辑如下图所述：
本算子设置BLOCK_DIM为1，AIC:AIV配比为2，即总共起了1个AIC和2个AIV，AIV的BlockIdx范围为0~1。总共分2块GM，一块用于存储输入数据(initialDataGm)，一块用于存储AIV的累加计算结果(atomicResultGm)。
(1) 每个AIV均从initialDataGm上搬运全为1的数据到UB上，即为PIPE_MTE2的流水操作。
(2) 每个AIV将刚才的搬运数据，通过Adds指令加上每个核对应的BlockIdx，即为PIPE_V的流水操作。
(3) 每个AIV将Adds计算完的数据从UB搬运到atomicResultGm上执行累加计算，即为PIPE_MTE3的流水操作。atomicResultGm上的结果预期为AIV 0和AIV 1全部搬运完后的累加结果。此时调用CrossCoreSetFlag接口，保证所有AIV都执行完PIPE_MTE3的搬运指令后，AIC再继续执行后续指令。
(4) 上述同步完成之后，此时atomicResultGm已经是AIV 0和AIV 1的累加结果。从atomicResultGm搬运到AIC上，用DumpTensor进行打印数据，检查累加结果是否符合预期。

注意：
如果第3步的同步插入不正确，那么第4步中的从atomicResultGm往AIC的搬运即无法保证时序，可能在其他AIV未完成累加时就开始搬运，导致DumpTensor的结果不准确。用户可以注释掉下述代码段中的CrossCoreSetFlag和CrossCoreWaitFlag调用，会发现累加结果不符合预期，并且每次运行结果都不一致。



        // AIV侧：
        AscendC::CrossCoreSetFlag<2, PIPE_MTE3>(0);  // 当本AIV完成前置PIPE_MTE3(DataCopy)流水操作后，通知AIC核，本AIV已经完成搬运任务

        // AIC侧：
        AscendC::CrossCoreWaitFlag(0);               // 阻塞本AIC继续往下执行指令，直到对应所有AIV都完成PIPE_MTE3流水操作，才解除阻塞往下执行。



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
  执行结果如下，说明精度对比成功。并且AIC DumpTensor打印出的数据均为3。
  ```bash
  [Success] Case accuracy is verification passed.
  ```