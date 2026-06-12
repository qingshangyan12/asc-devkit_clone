# PyTorch样例介绍
## 概述
本目录介绍Ascend C自定义算子接入PyTorch框架的不同方式，包括pybind11直调、torch.library直调，以及通过TorchAir将PyTorch自定义算子接入GE图执行。

## 样例列表
| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [pybind](./pybind) |  本样例展示如何使用pybind11注册自定义算子 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [torch_library](./torch_library) |  本样例展示如何使用PyTorch的torch.library机制注册自定义算子 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [ge_torchair](./ge_torchair) | 本样例展示PyTorch自定义算子如何通过TorchAir接入GE图执行 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
