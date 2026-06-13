# Ascend C算子接入PyTorch图模式样例

## 概述

本样例以AddCustom算子为例，介绍Ascend C算子如何接入PyTorch图模式。用户在模型中调用 `torch.ops.ascendc_ops.ascendc_add` 后，`torch.compile` 负责捕获该调用，TorchAir负责将其转换为GE `AddCustom`节点，最终由GE执行入口启动Ascend C kernel。

> **什么是TorchAir图模式接入？** <br>
> TorchAir承担PyTorch图到GE图的转换工作。关于PyTorch侧接口如何进入TorchAir/GE图执行链路，可参考[TorchAir图模式接入概述](https://www.hiascend.com/document/detail/zh/Pytorch/2600/modthirdparty/torchairuseguide/docs/zh/custom_op_graph/overview.md)。

**执行链路**：
- PyTorch侧：提供 `ascendc_ops::ascendc_add` 的schema和Meta实现，使 `torch.compile` 能捕获该调用并推导输出Tensor信息。
- TorchAir侧：将 `torch.ops.ascendc_ops.ascendc_add` 转换为GE `AddCustom`节点。
- GE/Ascend C侧：GE运行到 `AddCustom` 节点时进入执行入口，并启动Ascend C kernel完成计算。

<img src="figures/execution_flow.svg" width="90%">

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```text
├── ge_torchair
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── add_custom_kernel.asc   // Ascend C kernel实现
│   ├── add_custom_pytorch.asc  // PyTorch侧schema和Meta注册
│   ├── add_custom_ge.asc       // GE AddCustom原型注册和执行实现
│   ├── add_custom_test.py      // TorchAir图模式验证脚本
│   ├── figures                 // 图示
│   └── README.md               // 样例说明文档
```

## 样例描述

**样例功能**：

样例实现固定shape为 `[8, 2048]` 的两个Tensor相加。

Add的计算公式为：

$$
z = x + y
$$

- x：输入，形状为 `[8, 2048]`，数据类型为float，格式为ND；
- y：输入，形状为 `[8, 2048]`，数据类型为float，格式为ND；
- z：输出，形状为 `[8, 2048]`，数据类型为float，格式为ND。

## 样例实现

### Ascend C计算实现

`add_custom_kernel.asc` 实现Add计算对应的Ascend C kernel，该文件完成一次 `z = x + y` 计算。

### PyTorch图捕获需要的内容

`add_custom_pytorch.asc` 提供PyTorch侧可见的 `ascendc_ops::ascendc_add` 接口。图捕获阶段主要需要两类信息：接口签名和Meta实现。

接口签名用于让Python侧能够通过 `torch.ops.ascendc_ops.ascendc_add(x, y)` 找到该接口：

```cpp
TORCH_LIBRARY_FRAGMENT(ascendc_ops, m)
{
    m.def("ascendc_add(Tensor x, Tensor y) -> Tensor");
}
```

`torch.compile` 捕获图时不会执行真实NPU计算，但需要推导输出Tensor信息，因此还需要注册Meta实现：

```cpp
TORCH_LIBRARY_IMPL(ascendc_ops, Meta, m)
{
    m.impl("ascendc_add", TORCH_FN(AscendcAddMeta));
}
```

本样例的Meta实现会返回与输入 `x` 同规格的输出Tensor描述，并检查输入是否符合 `[8, 2048]` float Tensor 规格。具体检查逻辑可查看 `add_custom_pytorch.asc`。

Python侧先加载编译生成的动态库，然后在模型中调用该接口并交给 `torch.compile` 捕获：

```python
torch.ops.load_library(LIB_PATH)

class AddCustomModel(torch.nn.Module):
    def forward(self, x, y):
        return torch.ops.ascendc_ops.ascendc_add(x, y)

opt_model = torch.compile(model, fullgraph=True, backend=npu_backend, dynamic=False)
```

动态库加载完成后，PyTorch侧注册的接口才会生效。`torch.compile` 捕获 `AddCustomModel.forward` 中的调用后，后续由TorchAir converter接管，将该节点映射到GE节点。

这部分解决的是“PyTorch如何看见这个Add计算、图捕获阶段如何推导输出信息，以及捕获到的节点如何交给TorchAir继续转换”。

### TorchAir到GE执行需要的内容

`add_custom_test.py` 加载编译产物后，注册TorchAir converter，将 `torch.ops.ascendc_ops.ascendc_add` 转换为GE `AddCustom`节点：

```python
@torchair.register_fx_node_ge_converter(torch.ops.ascendc_ops.ascendc_add.default)
def convert_ascendc_add(x, y, z=None, meta_outputs=None):
    return torchair.ge.custom_op(
        "AddCustom",
        inputs={"x": x, "y": y},
        outputs=["z"],
    )
```

`add_custom_ge.asc` 则提供GE侧需要的 `AddCustom` 原型、shape/dtype推导和执行入口。GE运行到 `AddCustom` 节点时进入执行入口，并启动 `add_custom_kernel.asc` 中的Ascend C kernel。通过算子原型构建GE节点的基础概念可参考[什么是算子原型](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/programug/graphdevg/atlasag_25_0008.html)和[通过算子原型构建Graph](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/programug/graphdevg/atlasag_25_0009.html)。

## 编译运行

- 安装PyTorch、Ascend Extension for PyTorch插件以及TorchAir

  请参考[pytorch: Ascend Extension for PyTorch](https://gitcode.com/Ascend/pytorch)开源代码仓、[Ascend Extension for PyTorch昇腾社区](https://hiascend.com/document/redirect/Pytorch-index)和TorchAir配套安装说明，选取支持的 `Python` 版本配套发行版，完成 `torch`、`torch-npu` 和 `torchair` 的安装。

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

  python3 ../add_custom_test.py    # 执行样例
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：`dav-2201` 对应Atlas A2训练系列产品/Atlas A2推理系列产品与Atlas A3训练系列产品/Atlas A3推理系列产品，`dav-3510` 对应Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明TorchAir图模式执行结果与CPU标准加法结果对齐。

  ```bash
  Ran 1 test in **s
  OK
  ```



## 生成内容说明

执行 `make -j` 后，会在 `build/` 目录下生成 `libascendc_ops.so`。该动态库由 `add_custom_kernel.asc`、`add_custom_pytorch.asc` 和 `add_custom_ge.asc` 共同编译得到，包含以下内容：

- PyTorch侧 `ascendc_ops::ascendc_add` 的schema和Meta实现；
- GE侧 `AddCustom` 原型、shape/dtype推导和执行入口；
- Ascend C Add kernel。

`add_custom_test.py` 默认加载 `build/libascendc_ops.so`。如需加载其他路径的动态库，可以通过 `ASCENDC_OPS_LIB` 指定：

```bash
ASCENDC_OPS_LIB=/path/to/libascendc_ops.so python3 add_custom_test.py
```
