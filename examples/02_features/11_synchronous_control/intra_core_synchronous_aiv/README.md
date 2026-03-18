# AIV核内全量同步样例

## 概述

Atlas A2/A3系列产品的硬件架构如下图1所示。由于AIV核内部的执行单元采用异步并行的运行方式，在读写同一块存储资源时可能产生数据依赖关系（读后写、写后读、写后写）。为确保数据一致性与计算正确性，需通过核内同步指令来协调不同执行单元的时序。

本样例将介绍所有AIV核内同步（除标量单流水同步以外）使用的场景，学习本样例之前建议开发者先学习核内同步指令[SetFlag/WaitFlag](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/API/ascendcopapi/atlasascendc_api_07_0270.html)和[PipeBarrier](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/850/API/ascendcopapi/atlasascendc_api_07_0271.html)的使用。



## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── intra_core_synchronous_aiv
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── intra_core_synchronous_aiv.asc           // Ascend C算子实现 & 调用样例
```

## 算子描述

### AIV内硬件流水类型及其含义
<table border="1" cellpadding="8" cellspacing="0" style="width: 100%; border-collapse: collapse; font-family: monospace, sans-serif;">
  <!-- 新增表头 -->
  <caption style="font-size: 14px; font-weight: bold; padding-bottom: 8px;">表1：AIV内流水线类型说明表</caption>
  <thead>
    <tr style="background-color: #f2f2f2;">
      <th style="width: 20%; text-align: center;">流水类型</th>
      <th style="width: 80%; text-align: left;">含义</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_S</td>
      <td>标量流水线，使用Tensor GetValue/SetValue函数时为此流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_V</td>
      <td>矢量计算流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_MTE2</td>
      <td>GM->UB数据搬运流水</td>
    </tr>
    <tr>
      <td style="text-align: center; font-weight: bold;">PIPE_MTE3</td>
      <td>UB->GM数据搬运流水</td>
    </tr>
  </tbody>
</table>

### 单流水同步与多流水同步的定义
<table border="1" cellpadding="8" cellspacing="0" style="width: 100%; border-collapse: collapse; font-family: monospace, sans-serif; text-align: center;">
  <thead>
  <caption style="font-size: 14px; font-weight: bold; padding-bottom: 8px;">表2：AIV内的同步类型</caption>
    <tr style="background-color: #f2f2f2;">
      <!-- 左上角斜线表头：源流水 / 目的流水 -->
      <th style="position: relative; height: 60px; width: 120px;">
        <!-- 斜线 -->
        <div style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; 
                    background: linear-gradient(to top right, transparent calc(50% - 1px), #000, transparent calc(50% + 1px));"></div>
        <!-- 文字：左下-源流水 -->
        <span style="position: absolute; bottom: 4px; left: 6px; font-size: 14px;">源流水</span>
        <!-- 文字：右上-目的流水 -->
        <span style="position: absolute; top: 4px; right: 6px; font-size: 14px;">目的流水</span>
      </th>
      <th>PIPE_S</th>
      <th>PIPE_V</th>
      <th>PIPE_MTE2</th>
      <th>PIPE_MTE3</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><b>PIPE_S</b></td>
      <td>DataSyncBarrier</td>
      <td>S_V</td>
      <td>S_MTE2</td>
      <td>S_MTE3</td>
    </tr>
    <tr>
      <td><b>PIPE_V</b></td>
      <td>V_S</td>
      <td>PipeBarrier</td>
      <td>V_MTE2</td>
      <td>V_MTE3</td>
    </tr>
    <tr>
      <td><b>PIPE_MTE2</b></td>
      <td>MTE2_S</td>
      <td>MTE2_V</td>
      <td>PipeBarrier</td>
      <td>MTE2_MTE3</td>
    </tr>
    <tr>
      <td><b>PIPE_MTE3</b></td>
      <td>MTE3_S</td>
      <td>MTE3_V</td>
      <td>MTE3_MTE2</td>
      <td>PipeBarrier</td>
    </tr>
  </tbody>
</table>
如上表所示，对于两条并行流水的同步控制，根据源流水和目的流水类型是否相同分为两种：

(1)多流水同步：通过SetFlag/WaitFlag接口进行**不同流水线间**的同步控制。以SetFlag<MTE2_V>(EVENT_ID0)/WaitFlag<MTE2_V>(EVENT_ID0)为例，介绍这两个接口的功能：

  SetFlag：当前序指令的所有PIPE_MTE2流水上的读写操作都完成之后，当前指令开始执行，并将硬件中与EVENT_ID0对应标志位设置为1。

  WaitFlag：当执行到该指令时，如果发现与EVENT_ID0对应标志位为0，该队列的后续PIPE_V流水的指令将一直被阻塞；如果发现对应标志位为1，则将对应标志位设置为0，同时后续PIPE_V流水的指令开始执行。

(2)单流水同步：通过[PipeBarrier](../pipe_barrier/)完成**同一流水线内**的同步控制，
其中调用PipeBarrier<PIPE_S>()会引发硬件错误。因此当需要做标量流水之间的同步需要使用[DataSyncBarrier](../data_sync_barrier/)。


### 什么时候需要插入多流水同步？

多流水同步中，根据数据依赖和指令执行关系，存在两种依赖关系，即正向同步（循环内依赖）与反向同步（循环间依赖）。以MTE2从GM搬运数据至UB，进行Vector计算单元的Add计算，再搬运回GM的流程为例，两次正向同步和两次反向同步在此种场景的含义如下(以下变量参考[add_custom_v1](../../06_static_tensor_programming/add_custom_v1/))：

（1）正向同步：在本次数据搬入和计算之间，插入MTE2_V（矢量计算流水等待MTE2搬运流水）同步事件，确保数据xLocal、yLocal搬入之后再进行Add计算。在本次数据计算和搬出之间，插入V_MTE3（MTE3搬运流水等待矢量计算流水）同步事件，确保数据计算完成后再对zLocal进行搬出。

（2）反向同步：多次循环间复用了xLocal、yLocal和zLocal。
在上一次的数据计算和本次数据搬入之间，插入V_MTE2（MTE2搬运流水等待矢量计算流水）同步事件，确保上一次循环的xLocal、yLocal求和完成后，再搬入本次循环新的xLocal、yLocal，防止本次的数据会覆盖掉上一次未计算完成的数据；在上一次的数据搬出和本次数据计算之间，插入MTE3_V（矢量计算流水等待MTE3搬运流水）同步事件，确保上一次的数据搬出后，再进行本次数据的计算。防止本次的数据会覆盖掉上一次未搬出的数据。

### 什么时候需要插入单流水同步？
（1）对于PIPE_S和PIPE_V当需要确保两个标量或两个矢量指令执行的先后顺序时，需要在两个指令之间插入单流水同步。标量流水之间的同步参考[DataSyncBarrier](../data_sync_barrier/)。矢量流水之间的同步由编译器自动插入，具体参考[支持自动同步](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta1/opdevg/BishengCompiler/atlas_bisheng_10_0008.html)。

（2）PIPE_MTE2/PIPE_MTE3在搬运地址有重叠的情况下需要开发者插入同步，请参考[PipeBarrier](../pipe_barrier/)。
### 整体计算逻辑

下图1和图2分别展示了算子两个输入参与计算的流程图，可以看出算子有两个输入和两个输出，通过取值为0或1的环境变量SCENARIO_NUM来决定不同的计算分支。

下表3展示了图1、图2中出现的AscendC接口所属的硬件流水类型。
    <table>
      <caption style="font-size: 14px; font-weight: bold; padding-bottom: 8px;">表3：AIV内流水线类型说明表</caption>
        <tr>
            <th>AscendC接口</th>
            <th>所属硬件流水</th>
        </tr>
        <tr><td>LocalTensor.GetValue、LocalTensor.SetValue</td><td>PIPE_S</td></tr>
        <tr><td>Maxs、Mins、Muls、Add</td><td>PIPE_V</td></tr>
         <tr><td>DataCopy(UB,GM)</td><td>PIPE_MTE2</td></tr>
        <tr><td>DataCopy(GM,UB)</td><td>PIPE_MTE3</td></tr>
    </table>

### 所有插入的同步汇总
由图 1、图 2 可分析得到各接口间存在的数据依赖关系（详见代码注释），并依据表 3 在不同接口之间插入相应的多流水同步或单流水同步指令。表 4 和表 5 汇总了本样例中所插入的全部同步指令及其说明，从两表中可以看出，本样例使用了表 2 中除标量单流水同步之外的所有 AIV 核内同步方式。
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        /* 简单样式，可根据需求修改 */
        table {
            width: 100%;
            border-collapse: collapse;
            border: 1px solid #000;
        }
        th, td {
            border: 1px solid #000;
            padding: 8px 12px;
            text-align: left;
        }
        th {
            background-color: #f5f5f5;
            font-weight: bold;
        }
        /* 方案1：固定宽度（推荐，数值可修改） */
        .col-hardevent { width: 90px; } /* HardEvent列宽度 */
        .col-explain { width: calc(100% - 120px); } /* 说明列自适应剩余宽度 */
        /* 方案2：比例宽度（注释掉方案1，解开下面注释即可使用） */
        /* .col-hardevent { width: 30%; } */
        /* .col-explain { width: 70%; } */
    </style>
</head>
<body>
    <table>
    <caption style="font-size: 14px; font-weight: bold; padding-bottom: 8px;">表4：样例中所有多流水同步</caption>
        <thead>
            <tr>
                <th class="col-hardevent">HardEvent</th>
                <th class="col-explain">说明</th>
            </tr>
        </thead>
        <tbody>
            <tr><td class="col-hardevent">S_V</td><td class="col-explain">循环内依赖：先“SetValue(PIPE_S)写src1Local”，后 “Muls(PIPE_V)读src1Local</td></tr>
            <tr><td class="col-hardevent">S_MTE2</td><td class="col-explain">循环间依赖：先“SetValue(PIPE_S)写src1Local”，后 “DataCopy(PIPE_MTE2)写src1Local”</td></tr>
            <tr><td class="col-hardevent">S_MTE3</td><td class="col-explain">循环内依赖：先“SetValue(PIPE_S)写src1Local”，后 “DataCopy(PIPE_MTE3)读src1Local”</td></tr>
            <tr><td class="col-hardevent">V_S</td><td class="col-explain">循环内依赖：先“Muls(PIPE_V)写src1Local”，后 “SetValue(PIPE_S)写src1Local”</td></tr>
            <tr><td class="col-hardevent">V_MTE2</td><td class="col-explain">循环间依赖：先“Mins(PIPE_V)读src0Local”，后 “DataCopy(PIPE_MTE2)写src0Local”</td></tr>
            <tr><td class="col-hardevent">V_MTE3</td><td class="col-explain">循环内依赖：先“Add(PIPE_V)写dst0Local”，后 “DataCopy(PIPE_MTE3)读dst0Local”</td></tr>
            <tr><td class="col-hardevent">MTE2_S</td><td class="col-explain">循环内依赖：先“DataCopy(PIPE_MTE2)写src1Local”，后 “SetValue(PIPE_S)写src1Local”</td></tr>
            <tr><td class="col-hardevent">MTE2_V</td><td class="col-explain">循环内依赖：先“DataCopy(PIPE_MTE2)写src0Local”，后 “Maxs和Mins（PIPE_V）读src0Local”</td></tr>
            <tr><td class="col-hardevent">MTE2_MTE3</td><td class="col-explain">循环内依赖：先“DataCopy(PIPE_MTE2)写src1Local”，后 “DataCopy(PIPE_MTE3)读src1Local”</td></tr>
            <tr><td class="col-hardevent">MTE3_S</td><td class="col-explain">循环内依赖：先“DataCopy(PIPE_MTE3)读src1Local”，后 “SetValue(PIPE_S)写src1Local”</td></tr>
            <tr><td class="col-hardevent">MTE3_V</td><td class="col-explain">循环间依赖：先“DataCopy(PIPE_MTE3)读dst0Local”，后 “Add(PIPE_V)写dst0Local”</td></tr>
            <tr><td class="col-hardevent">MTE3_MTE2</td><td class="col-explain">循环间依赖：先“DataCopy(PIPE_MTE3)读src1Local”，后 “DataCopy(PIPE_MTE2)写src1Local”</td></tr>
        </tbody>
    </table>
图 3 和图 4 分别展示了两个输入在 UB 上的地址重叠情况，以及两个输出在 GM 上的地址重叠情况。由于存在地址重叠，需插入单流水同步指令，以确保各搬运指令按用户预期的顺序执行。
    <table>
    <caption style="font-size: 14px; font-weight: bold; padding-bottom: 8px;">表5：样例中所有单流水同步</caption>
        <thead>
            <tr>
                <th class="col-hardevent">HardEvent</th>
                <th class="col-explain">说明</th>
            </tr>
        </thead>
        <tbody>
            <tr><td class="col-hardevent">PipeBarrier&lt;PIPE_MTE2&gt;()</td><PIPE_MTE2><td class="col-explain">两次DataCopy（MTE2）搬入到UB的地址都会产生重叠，插入同步能保证src1Local不会覆盖。src0Local</td></tr>
            <tr><td class="col-hardevent">PipeBarrier&lt;PIPE_MTE3&gt;()</td><td class="col-explain"> 两次DataCopy（MTE3）搬出到GM的地址都会产生重叠，插入同步能保证dst1Global不会覆盖dst0Global。</td></tr>
        </tbody>
    </table>

</body>
</html>










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
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake ..;make -j;                # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
