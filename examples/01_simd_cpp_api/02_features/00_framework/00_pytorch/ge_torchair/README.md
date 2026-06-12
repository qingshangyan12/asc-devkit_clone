# PyTorch自定义算子通过TorchAir接入GE图样例

## 概述

本样例基于AddCustom算子展示PyTorch自定义算子如何通过TorchAir接入GE图执行。PyTorch侧注册`ascendc_ops::ascendc_add`的schema和Meta实现，使`torch.compile`能够捕获该自定义算子节点；TorchAir converter再将该节点映射为GE `AddCustom`节点，最终由GE执行入口下发Ascend C kernel。

本样例的主链路不是PyTorch eager直调，而是PyTorch图模式接入GE：

```text
torch.compile(model, backend=torchair.get_npu_backend(...))
  -> Dynamo/FX捕获torch.ops.ascendc_ops.ascendc_add
  -> Meta实现提供输出shape/dtype推导
  -> TorchAir converter映射为GE AddCustom
  -> GE AddCustom::Execute调用launch_add_custom
  -> add_custom<<<...>>>(...)
```

## 为什么要通过TorchAir接入GE图执行

如果只希望在PyTorch中直接调用一个Ascend C核函数，可以使用`pybind`或`torch_library`样例中的方式：Python侧调用自定义接口，C++侧拿到当前NPU stream后直接通过`<<<>>>`下发kernel。这类方式适合单算子功能验证或eager执行，不需要把自定义算子交给GE图引擎调度。

当自定义算子需要参与PyTorch图模式执行时，就需要一条从PyTorch图到GE图的转换链路。TorchAir承担的就是PyTorch到GE的lowering工作：`torch.compile`先捕获`torch.ops.ascendc_ops.ascendc_add`，再由TorchAir converter把该PyTorch自定义算子转换为GE `AddCustom`节点。这样自定义算子就不再是Python测试脚本里的单独直调kernel，而是GE图中的一个节点。

通过TorchAir接入GE图执行的典型场景包括：

- 需要验证自定义算子在`torch.compile`图模式下能被捕获、转换并执行。
- 上层模型希望整体下沉到GE执行，自定义算子需要作为图节点参与编译和调度。
- 需要让PyTorch侧提供schema和Meta推导，让图捕获阶段知道自定义算子的输入、输出、shape和dtype。
- 需要让GE侧提供`AddCustom`原型、shape/dtype推导和`Execute()`实现，让TorchAir转换出的GE节点能被GE识别并执行。

本样例展示的是PyTorch + TorchAir + GE的完整接入关系；如果目标只是PyTorch eager直调，请优先参考同目录下的`pybind`或`torch_library`样例。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── ge_torchair
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── add_custom.asc          // Ascend C kernel + PyTorch schema/Meta + GE AddCustom
│   ├── add_custom_test.py      // TorchAir图模式验证脚本
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：

  Add计算公式为：

  ```
  z = x + y
  ```

- 样例规格：
  <table border="2" align="center">
  <caption>表1：AddCustom样例规格描述</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">AddCustom</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

- PyTorch自定义算子注册：

  本样例在`add_custom.asc`中使用`TORCH_LIBRARY_FRAGMENT`注册`ascendc_ops::ascendc_add`的schema，使Python侧可以通过`torch.ops.ascendc_ops.ascendc_add`引用该自定义算子。

  ```cpp
  TORCH_LIBRARY_FRAGMENT(ascendc_ops, m)
  {
      m.def("ascendc_add(Tensor x, Tensor y) -> Tensor");
  }
  ```

  同时通过`TORCH_LIBRARY_IMPL(ascendc_ops, Meta, m)`注册Meta实现，用于在`torch.compile`捕获图时提供输出shape和dtype推导。本样例固定支持`float`、`[8, 2048]`、`ND`，不支持广播和动态shape。

- TorchAir converter：

  `add_custom_test.py`通过`torch.ops.load_library`加载编译生成的`libascendc_ops.so`，再注册TorchAir converter，将PyTorch自定义算子映射为GE `AddCustom`节点。

  ```python
  @torchair.register_fx_node_ge_converter(torch.ops.ascendc_ops.ascendc_add.default)
  def convert_ascendc_add(x, y, z=None, meta_outputs=None):
      return torchair.ge.custom_op(
          "AddCustom",
          inputs={"x": x, "y": y},
          outputs=["z"],
      )
  ```

- GE自定义算子执行实现：

  本样例在`add_custom.asc`中注册GE `AddCustom`原型和执行实现。GE图执行到`AddCustom`节点时进入`AddCustom::Execute()`，申请输出Tensor，并调用`ascendc_ops::launch_add_custom(...)`。

  ```cpp
  ascendc_ops::launch_add_custom(
      static_cast<float *>(const_cast<void *>(x->GetAddr())),
      static_cast<float *>(const_cast<void *>(y->GetAddr())),
      static_cast<float *>(z->GetAddr()),
      ctx->GetStream());
  ```

  `launch_add_custom(...)`内部使用GE提供的stream调用Ascend C kernel：

  ```cpp
  add_custom<BLOCK_LENGTH><<<BLOCK_NUM, nullptr, stream>>>(x, y, z);
  ```

- Ascend C核函数实现：

  本样例使用8个核完成计算，每个核处理2048个`float`元素。每个核按`block_idx * blockLength`计算当前核负责的GM偏移，计算流程为GM输入搬运到UB，调用`AscendC::Add`完成向量加法，再将结果从UB搬回GM输出。

## 编译运行

- 安装PyTorch、Ascend Extension for PyTorch插件以及TorchAir

  请参考[pytorch: Ascend Extension for PyTorch](https://gitcode.com/Ascend/pytorch)开源代码仓、[Ascend Extension for PyTorch昇腾社区](https://hiascend.com/document/redirect/Pytorch-index)和TorchAir配套安装说明，选取支持的`Python`版本配套发行版，完成`torch`、`torch-npu`和`torchair`的安装。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例根目录下执行如下步骤，运行该样例。

  ```bash
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..; make -j;    # 编译工程
  cd ..
  python3 add_custom_test.py    # 执行样例
  ```

- 编译选项说明

  | 选项　　　　　 | 可选值　　　　　　　　　　　| 说明　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品 与 Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明TorchAir图模式执行结果与CPU标准加法结果对齐。

  ```bash
  Ran 1 test in **s
  OK
  ```

## 动态库内容说明

编译生成的`build/libascendc_ops.so`同时包含以下内容：

| 来源文件 | 编入动态库的内容 | 作用 |
| --- | --- | --- |
| `add_custom.asc` | PyTorch schema、Meta实现、GE `AddCustom`原型注册、GE `AddCustom`执行实现、`add_custom` kernel和`launch_add_custom(...)` | 让PyTorch识别自定义算子，让TorchAir转出的GE `AddCustom`节点能被GE识别并执行，并在执行阶段下发Ascend C kernel |

Python测试脚本通过`torch.ops.load_library("libascendc_ops.so")`加载该动态库后，PyTorch注册和GE注册都会生效。

## 约束说明

- 本样例只覆盖PyTorch通过TorchAir进入GE图，不涉及GE原生C++构图。
- PyTorch eager不作为样例主链路和验收项。
- 本样例固定支持`float`、`[8, 2048]`、`ND`，不支持广播和动态shape。
