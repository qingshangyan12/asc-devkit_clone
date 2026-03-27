# torch.library注册自定义算子直调样例

## 概述

本样例展示了如何使用PyTorch的torch.library机制注册自定义算子，并通过`<<<>>>`内核调用符调用核函数，以简单的Add算子为例，实现两个向量的逐元素相加。并展示如何在自定义算子中上报算子基础信息，支持在torch采集的profiling数据中展示。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── torch_library
│   ├── CMakeLists.txt           // 编译工程文件
│   ├── add_custom_test.py       // PyTorch调用脚本
│   ├── add_custom.asc           // Ascend C算子实现 & 自torch.library注册
│   └── add_custom_profiling.asc // 开启Profiling采集和分析数据
```

## 算子描述

- 算子功能：

  Add算子实现了两个数据相加，返回相加结果的功能。对应的数学表达式为：

  ```
  z = x + y
  ```

- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">AddCustom</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">8 * 2048</td><td align="center">float16</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">8 * 2048</td><td align="center">float16</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">8 * 2048</td><td align="center">float16</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

- 算子实现：

  Ascend C提供的矢量计算接口`Add`的操作元素都为`LocalTensor`，输入数据需要先搬运进片上存储，然后使用计算接口完成两个输入参数相加，得到最终结果，再搬出到外部存储上。

  Add算子的实现流程分为3个基本任务：`CopyIn`，`Compute`，`CopyOut`。`CopyIn`任务负责将Global Memory上的输入Tensor `xGm`和`yGm`搬运到Local Memory，分别存储在`xLocal`、`yLocal`，`Compute`任务负责对`xLocal`、`yLocal`执行加法操作，计算结果存储在`zLocal`中，`CopyOut`任务负责将输出数据从`zLocal`搬运至Global Memory上的输出Tensor zGm中。

- 自定义算子注册：

  本样例在`add_custom.asc`中定义了一个名为`ascendc_ops`的命名空间，并在其中注册了`ascendc_add`函数。

  PyTorch提供`TORCH_LIBRARY`宏作为自定义算子注册的核心接口，用于创建并初始化自定义算子库，注册后在Python侧可以通过`torch.ops.namespace.op_name`方式进行调用，例如：

  ```c++
  TORCH_LIBRARY(ascendc_ops, m) {
      m.def(ascendc_add"(Tensor x, Tensor y) -> Tensor");
  }
  ```

  `TORCH_LIBRARY_IMPL`用于将算子逻辑绑定到特定的`DispatchKey`（PyTorch设备调度标识）。针对NPU设备，需要将算子实现注册到`PrivateUse1`这一专属的`DispatchKey`上，例如：

  ```c++
  TORCH_LIBRARY_IMPL(ascendc_ops, PrivateUse1, m)
  {
      m.impl("ascendc_add", TORCH_FN(ascendc_ops::ascendc_add));
  }
  ```

  在`ascendc_add`函数中通过`c10_npu::getCurrentNPUStream()`函数获取当前NPU上的流，并通过内核调用符`<<<>>>`调用自定义的Kernel函数`add_custom`，在NPU上执行算子。

- Python测试脚本

  在`add_custom_test.py`调用脚本中，通过`torch.ops.load_library`加载生成的自定义算子库，调用注册的`ascendc_add`函数，并通过对比NPU输出与CPU标准加法结果来验证自定义算子的数值正确性。

## 编译运行

- 安装PyTorch以及Ascend Extension for PyTorch插件

  请参考[pytorch: Ascend Extension for PyTorch](https://gitcode.com/Ascend/pytorch)开源代码仓或[Ascend Extension for PyTorch昇腾社区](https://hiascend.com/document/redirect/Pytorch-index)的安装说明，选取支持的`Python`版本配套发行版，完成`torch`和`torch-npu`的安装。

- 安装前置依赖

  ```bash
  pip3 install expecttest
  ```

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

  在本样例根目录下执行如下步骤，运行该样例。

  ```bash
  mkdir -p build; cd build
  cmake ..; make -j
  python3 ../add_custom_test.py
  ```

  执行结果如下，说明精度对比成功。

  ```bash
  Ran 1 test in **s
  OK
  ```

## 自定义算子Profiling扩展

- 上报Shape信息
  
  Profiling提供`Shape`信息上报的接口`aclprofStr2Id`、`aclprofRangePushEx`、`aclprofRangePop`，其中`aclprofStr2Id`用于将算子名、算子类型转化为`hashId`，`aclprofRangePushEx`和`aclprofRangePop`分别在`<<<>>>`前后调用，上报`Shape`信息。

  算子信息上报需要构造`aclprofEventAttributes`结构体，包含版本、大小、类型和tensor信息，其中`messageType`固定`MESSAGE_TYPE_TENSOR_INFO`为0，`aclprofTensorInfo`包含`opNameId`、`opTypeId`、`resv`、`tensorNum`、`kernelType`、`blockNums`、`stream`、`tensors`字段。`opNameId`、`opTypeId`字段通过`aclprofStr2Id`接口转化算子名、算子类型获取，每包数据`tensorNum`不超过5，超过5请分成多包数据上报，`tensors`为`aclprofTensor`结构，从算子的`at::Tensor`获取。

- 样例执行

  在本样例根目录下修改`add_custom_test.py`，替换为加载`libascendc_ops_profiling.so`，执行如下步骤，运行该样例。

  ```bash
  mkdir -p build; cd build
  cmake ..; make -j
  msprof --application="python3 ../add_custom_test.py" --output="../result"
  ```

- Shape信息展示

  打开`PROF_000001_*/mindstudio_profiler_output/op_summary_*.csv`，查看Shape信息，样例中Shape信息写入如下字段。
    
  <table>
    <tr>
      <td align="center">...</td>
      <td align="center">Op Name</td>
      <td align="center">Op Type</td>
      <td align="center">Input Shapes</td>
      <td align="center">Input Data Types</td>
      <td align="center">Input Formats</td>
      <td align="center">Output Shapes</td>
      <td align="center">Output Data Types</td>
      <td align="center">Output Formats</td>
      <td align="center">...</td>
    </tr>
    <tr>
      <td align="center">...</td>
      <td align="center">_Z10add_customPhS_S_j</td>
      <td align="center">_Z10add_customPhS_S_j</td>
      <td align="center">"8,2048;8:2048"</td>
      <td align="center">FLOAT16;FLOAT16</td>
      <td align="center">ND;ND</td>
      <td align="center">"8,2048"</td>
      <td align="center">FLOAT16</td>
      <td align="center">ND</td>
      <td align="center">...</td>
    </tr>
  </table>