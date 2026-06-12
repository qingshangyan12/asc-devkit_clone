# Framework样例介绍

## 概述

本样例介绍了PyTorch、TensorFlow、ONNX框架以及GE原生构图场景的自定义算子实现方法。

## 如何选择样例

| 使用场景 | 推荐样例 | 适用说明 |
| -------- | -------- | -------- |
| 在PyTorch中直接调用Ascend C自定义算子，并希望以Python模块形式导入 | [00_pytorch/pybind](./00_pytorch/pybind) | 适合使用pybind11把C++接口封装成Python模块，由Python脚本直接调用自定义算子 |
| 在PyTorch中直接调用Ascend C自定义算子，并希望通过`torch.ops`接口调用 | [00_pytorch/torch_library](./00_pytorch/torch_library) | 适合使用PyTorch `torch.library`注册自定义算子，走PyTorch eager直调链路 |
| 在PyTorch图模式中使用自定义算子，并希望通过TorchAir接入GE图执行 | [00_pytorch/ge_torchair](./00_pytorch/ge_torchair) | 适合`torch.compile`捕获PyTorch自定义算子，再由TorchAir转换成GE自定义算子节点执行 |
| 在TensorFlow网络中调用Ascend C自定义算子 | [01_tensorflow](./01_tensorflow) | 适合TensorFlow框架集成场景，可根据需要选择映射到TensorFlow内置算子或自定义算子 |
| 在ONNX网络中调用Ascend C自定义算子 | [02_onnx](./02_onnx) | 适合通过ONNX网络和插件机制调用自定义算子 |
| 不经过PyTorch/TensorFlow/ONNX，直接使用GE接口构建并执行Graph | [03_ge](./03_ge) | 适合GE原生构图场景，展示自定义算子作为GE图节点执行，并在`EagerExecuteOp::Execute`中下发Ascend C kernel |

## 样例列表

| 目录名称  | 功能描述 |
| --------- | --------- |
| [00_pytorch](./00_pytorch) | 样例介绍PyTorch框架的自定义算子实现方法 |
| [01_tensorflow](./01_tensorflow) | 样例介绍TensorFlow框架的自定义算子实现方法 |
| [02_onnx](./02_onnx) | 样例介绍ONNX框架的自定义算子实现方法 |
| [03_ge](./03_ge) | 样例介绍GE原生构图场景的自定义算子实现方法 |
