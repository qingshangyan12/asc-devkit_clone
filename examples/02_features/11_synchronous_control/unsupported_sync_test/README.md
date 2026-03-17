# SetFlag/WaitFlag不支持同步场景样例

## 概述
本样例用于展示AIC和AIV之间不支持的同步场景。在硬件架构中，AIC和AIV之间不存在SetFlag/WaitFlag同步机制。本样例展示了AIC和AIV之间不支持的同步通路。另外，此README会介绍核内同步和自动同步机制，供开发者学习参考。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── unsupported_sync_test
│   ├── CMakeLists.txt              // 编译工程文件
│   └── unsupported_sync_test.asc   // Ascend C算子实现 & 调用样例
```

## 算子描述

本样例实现了一个简单的Add算子（dst = src0 + src1），在此基础上，调用了SetFlag/WaitFlag接口进行AIC和AIV之间同步。SetFlag/WaitFlag接口用于核内同步，AIC和AIV之间的同步属于核间同步，所以SetFlag/WaitFlag接口不支持AIC和AIV之间的同步。

### 一、AIC和AIV之间不支持的同步情况样例
为了展示AIC和AIV核间调用SetFlag/WaitFlag的情况，本样例设计了表1中的7种场景，场景0为核内调用SetFlag/WaitFlag场景，场景1-6为AIC和AIV核间调用SetFlag/WaitFlag场景。

**表1：AIC和AIV之间不支持的同步**

| 场景编号<br>SCENARIONUM | 源流水 | 目标流水 |
|:--------:|:------:|:--------:|
| 0 | PIPE_MTE2 | PIPE_MTE3 |
| 1 | PIPE_M | PIPE_V |
| 2 | PIPE_V | PIPE_M |
| 3 | PIPE_V | PIPE_FIX |
| 4 | PIPE_FIX | PIPE_V |
| 5 | PIPE_V | PIPE_MTE1 |
| 6 | PIPE_MTE1 | PIPE_V |

**注意：** 由于AIC和AIV之间不支持SetFlag/WaitFlag同步，场景1-6的同步操作实际上不起作用。执行结果可能因为SetFlag或WaitFlag没有正确插入恰好通过，也可能因为数据依赖未满足而失败。本样例仅供参考。

### 二、核内同步概述

核内同步分为多流水同步和单流水同步：
- **多流水同步**：同一核内具有数据依赖的不同类型流水间的同步。
  通过SetFlag/WaitFlag接口进行不同流水的同步控制。
- **单流水同步**：同一核内具有数据依赖的相同类型流水间的同步。
  通过PipeBarrier接口进行相同流水指间的同步控制，该接口不支持PIPE_S的单流水同步。
  通过DataSyncBarrier接口进行PIPE_S单流水的同步。

表2、表3分别为AIC和AIV支持的核内同步情况，不同的流水间的同步需要调用的接口不同。
<br>

**表2：AIC支持的同步**

| 源流水\目标流水 | PIPE_S | PIPE_M | PIPE_MTE1 | PIPE_MTE2 | PIPE_MTE3 | PIPE_FIX |
|:---------------:|:------:|:------:|:---------:|:---------:|:---------:|:--------:|
| PIPE_S          | DataSyncBarrier | AscendC待支持 | AscendC待支持  | SetFlag/WaitFlag | SetFlag/WaitFlag | AscendC待支持  |
| PIPE_M          | SetFlag/WaitFlag | PipeBarrier | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag |
| PIPE_MTE1       | AscendC待支持  | SetFlag/WaitFlag | PipeBarrier | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag |
| PIPE_MTE2       | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag | PipeBarrier | SetFlag/WaitFlag | SetFlag/WaitFlag |
| PIPE_MTE3       | SetFlag/WaitFlag | AscendC待支持 | SetFlag/WaitFlag | SetFlag/WaitFlag | PipeBarrier | AscendC待支持  |
| PIPE_FIX        | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag | PipeBarrier |


**表3：AIV支持的同步**

| 源流水\目标流水 | PIPE_S | PIPE_V | PIPE_MTE2 | PIPE_MTE3 |
|:---------------:|:------:|:------:|:---------:|:---------:|
| PIPE_S          | DataSyncBarrier | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag |
| PIPE_V          | SetFlag/WaitFlag | PipeBarrier | SetFlag/WaitFlag | SetFlag/WaitFlag |
| PIPE_MTE2       | SetFlag/WaitFlag | SetFlag/WaitFlag | PipeBarrier | SetFlag/WaitFlag |
| PIPE_MTE3       | SetFlag/WaitFlag | SetFlag/WaitFlag | SetFlag/WaitFlag | PipeBarrier |

### 三、自动同步机制

 在静态Tensor编程模型下，所有自动同步均失效，开发者必须手动插入所有同步。自动同步只在使用TPipe/TQue编程方式时生效。具体内容见表3。<br><br>
**表3：核内自动同步情况**
| 编程方式 | 是否开启自动同步编译选项 |单流水 | 多流水 |
|:------:|:------:|:------:|:------:|
| TPipe/TQue编程方式 | 是 | PIPE_V由编译器自动完成同步，其他流水在搬运地址有重叠的情况下需要开发者手动插入同步。 | PIPE_S分别于PIPE_V、PIPE_MTE2、PIPE_MTE3之间的双向同步（6种），由编译器自动同步，此时编译器占用事件ID7。<br> 其余非PIPE_S的流水线由AscendC框架通过EnQue/DeQue、AllocTensor/FreeTensor等接口自动同步。 |
| 静态Tensor编程方式 | 否 | 需要手动同步 | 需要手动同步 |

根据表1、表2、表3，在TPipe/TQue编程模型下，AIC和AIV不同流水线的同步情况可总结为表4、表5。<br><br>
**表4：在TPipe/TQue编程模型下，不同流水的同步情况（AIC）**

| 源流水\目标流水 | PIPE_S | PIPE_M | PIPE_MTE1 | PIPE_MTE2 | PIPE_MTE3 | PIPE_FIX |
|:---------------:|:------:|:------:|:---------:|:---------:|:---------:|:--------:|
| PIPE_S          | 需要手动插入单流水同步 | NA | AscendC待支持 | 编译器自动同步 | 编译器自动同步 | NA |
| PIPE_M          | NA | 需要手动插入单流水同步 | AscendC框架自动同步 | AscendC框架自动同步 | AscendC待支持 | AscendC框架自动同步 |
| PIPE_MTE1       | AscendC待支持 | AscendC框架自动同步 | 需要手动插入单流水同步 | AscendC框架自动同步 | AscendC框架自动同步 | AscendC框架自动同步 |
| PIPE_MTE2       | 编译器自动同步 | AscendC框架自动同步 | AscendC框架自动同步 | 需要手动插入单流水同步 | AscendC框架自动同步 | AscendC框架自动同步 |
| PIPE_MTE3       | 编译器自动同步 | AscendC待支持 | AscendC框架自动同步 | AscendC框架自动同步 | 需要手动插入单流水同步 | AscendC待支持 |
| PIPE_FIX        | NA | AscendC框架自动同步 | AscendC框架自动同步 | AscendC框架自动同步 | AscendC框架自动同步 | 需要手动插入单流水同步 |


**表5：在TPipe/TQue编程模型下，不同流水的同步情况（AIV）**

| 源流水\目标流水 | PIPE_S | PIPE_V | PIPE_MTE2 | PIPE_MTE3 |
|:---------------:|:------:|:------:|:---------:|:---------:|
| PIPE_S          | 需要手动插入单流水同步 | 编译器自动同步 | 编译器自动同步 | 编译器自动同步 |
| PIPE_V          | 编译器自动同步 | 编译器自动同步 | AscendC框架自动同步 | AscendC框架自动同步 |
| PIPE_MTE2       | 编译器自动同步 | AscendC框架自动同步 | 需要手动插入单流水同步 | AscendC框架自动同步 |
| PIPE_MTE3       | 编译器自动同步 | AscendC框架自动同步 | AscendC框架自动同步 | 需要手动插入单流水同步 |


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
   SCENARIONUM=0                    # 场景编号，取值范围0-6。
   mkdir -p build && cd build;      # 创建并进入build目录
   cmake .. -DSCENARIO_NUM=$SCENARIONUM;make -j;  # 编译工程
   ./demo                           # 执行编译生成的可执行程序
   ```
   当SCENARIONUM为0时，执行结果如下，说明精度对比成功。
   ```bash
   test pass!
   ```
   当SCENARIONUM为1-6时，执行结果如下：
   ```bash
   Assertion `false' SetFlag/WaitFlag From XX to XX is not supported!
   ```
