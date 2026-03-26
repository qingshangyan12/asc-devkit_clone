# CrossCoreSetFlag和CrossCoreWaitFlag，纯Vector计算场景样例
## 概述

本样例详细说明：在仅包含 Vector 计算的应用场景下，如何使用 [CrossCoreSetFlag和CrossCoreWaitFlag](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta1/API/ascendcopapi/atlasascendc_api_07_0273.html)的模式 0、模式 1 实现 AIV 间的同步。

模式0（AIV全核同步）的应用场景：在对多个 AI Core 中所有 AIV 完成矢量计算后的结果进行累加时，需确保累加操作前，所有 AI Core 内所有 AIV 的矢量计算均已完成，因此需对多 AI Core 下的所有 AIV 执行同步控制。

模式1的应用场景：在对单个 AI Core 中所有 AIV 完成矢量计算后的结果进行累加时，需确保累加操作前，单个AI Core 内所有 AIV 的矢量计算均已完成，因此需对单 AI Core 下的所有 AIV 执行同步控制。
<table border="1" align="center">
  <tr bgcolor="lightgray">
    <td>同步控制模式</td>
    <td align="center">说明</td>
  </tr>
  <tr>
    <td rowspan="2">mode 0</td>
    <td>对于AIC场景，同步所有的AIC核，直到所有的AIC核都执行到CrossCoreSetFlag时，CrossCoreWaitFlag后续的指令才会执行。</td>
  </tr>
  <tr>
    <td>对于AIV场景，同步所有的AIV核，直到所有的AIV核都执行到CrossCoreSetFlag时，CrossCoreWaitFlag后续的指令才会执行。</td>
  </tr>
  <tr>
    <td>mode 1</td>
    <td>单个AI Core内部，AIV核之间的同步控制。如果两个AIV核都运行了CrossCoreSetFlag，CrossCoreWaitFlag后续的指令才会执行。</td>
  </tr>
  <tr>
    <td rowspan="2">mode 2</td>
    <td>在AIC核执行CrossCoreSetFlag之后， 两个AIV上CrossCoreWaitFlag后续的指令才会继续执行。</td>
  </tr>
  <tr>
    <td>两个AIV都执行CrossCoreSetFlag后，AIC上CrossCoreWaitFlag后续的指令才能执行。</td>
  </tr>
</table>

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── cross_core_set_flag_vector
│   ├── CMakeLists.txt                              // 编译工程文件
│   └── cross_core_set_flag_vector.asc               // Ascend C算子实现 & 调用样例
```

## 算子描述

### 模式0和模式1的对比
本算子设置NUM\_BLOCKS为8（8个AI Core），每一个AI Core内AIC:AIV配比为2，即本算子总共起了8个AIC和16个AIV，AIV的BlockIdx范围为0\~15。
本样例中模式0和模式1计算逻辑几乎相同，区别仅在于参与同步的AIV数目：模式0时，全部的16个AIV参与计算；模式1时，仅有2个AIV（BlockIdx=2和3）参与计算。

### 模式0的计算逻辑
<p align="center">
  <img src="img/纯aiv_模式0示意图.png" width="100%">
</p>

<p align="center">
图1：纯AIV场景，模式0计算逻辑示意图
</p>
本算子用到的GM分为2块，一块用于存储输入数据(initialDataGm)，一块用于存储所有AIV的累加结果(atomicResultGm)。

如图1所示，模式0的计算逻辑如下：
(1) 每个AIV均从initialDataGm上搬运全为1的数据到UB上，即为PIPE\_MTE2的流水操作。

(2) 每个AIV对UB上的数据进行矢量计算：通过Muls指令乘以每个核对应的BlockIdx，即为PIPE\_V的流水操作。

(3) 步骤3对应的代码片如下所示，atomicResultGm上的结果预期为16个AIV全部搬运完后的累加结果。此时调用CrossCoreSetFlag和CrossCoreWaitFlag接口，保证所有AIV都执行完PIPE\_MTE3的搬运指令后，每个AIV再继续执行后续指令。
```
         // UB 到 GM 搬运启用原子累加：搬运至 atomicResult 的数据与原值累加后覆盖原值
        AscendC::SetAtomicAdd<float>(); 
        // DataCopy属于PIPE_MTE3流水操作
        AscendC::DataCopy(atomicResultGm, xLocal, this->blockLength);   
        // 当本AIV完成前置PIPE_MTE3(DataCopy)流水操作后，通知其他AIV核，本AIV已经完成
        AscendC::CrossCoreSetFlag<0, PIPE_MTE3>(0);  
        // 阻塞本AIV继续往下执行指令，直到其他AIV全部都完成PIPE_MTE3流水操作，才解除阻塞往下执行。
        AscendC::CrossCoreWaitFlag(0); 
```

(4) 上述同步完成之后，此时atomicResultGm已经是16个AIV中矢量计算结果的累加值。此时从atomicResultGm搬运到某一个AIV上，用DumpTensor进行打印数据，检查累加结果是否符合预期，最终将该AIV上的结果搬出到atomicResultGm。

如果上一步骤的同步插入不正确，那么从atomicResultGm往AIV的搬运的数据可能是部分AIV中矢量计算结果的累加值，导致DumpTensor的结果不准确。

```
        if (AscendC::GetBlockIdx() == 0) {
            AscendC::DataCopy(yLocal, atomicResultGm, this->blockLength);   // PIPE_MTE2
            AscendC::printf("============== In PrintTensor Process AIV %d ==============", AscendC::GetBlockIdx());
            AscendC::DumpTensor(yLocal, AscendC::GetBlockIdx(), this->blockLength);
            AscendC::DataCopy(atomicResultGm, yLocal, this->blockLength);
        }
```

### 注意事项：
（1）本算子总共起了8个AIC和16个AIV，但是本算子中仅包含矢量计算，因此必须通过ASCEND\_IS\_AIV确保整个程序只在AIV中执行，否则会导致程序卡死。

```
if ASCEND_IS_AIV {
        op.Init(x, z, dataLength);
        op.Process();
}
```
 (2) 模式 1 要求参与同步的 AIV 必须属于同一 AI Core，否则程序会卡死。本样例中，参与同步的两个AIV的 GetBlockIdx=2、3 同属第 2 个 AI Core（下标从 1 开始）；若将GetBlockIdx改为 3 和 4（分别属于两个不同的AI Core），程序将卡死。

 (3) 样例采用[静态tensor编程范式](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/opdevg/Ascendcopdevg/atlas_ascendc_10_00019.html)需要手动插入核内同步。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare\&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```
  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```
  - 指定路径install\_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
- 样例执行
  ```bash
  SCENARIO=0
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;             # 编译工程
  # 在build目录执行以下内容
  ./demo                         # 执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

