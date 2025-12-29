# FrameworkLaunch样例介绍
## 概述
基于Ascend C的Add算子的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。

## 算子开发样例
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [add_template](./add_template) | 本样例以Add算子为示例，展示了Tiling模板编程。本样例使用自定义算子工程，编译并部署自定义算子包到自定义算子库中，并调用执行自定义算子 |
| [msopgen_custom](./msopgen_custom) | 本样例基于AddCustom算子工程，介绍了msOpGen工具生成简易自定义算子工程和单算子调用 |
| [onnx_plugin](./onnx_plugin) | 本样例介绍通过onnx网络调用的方式调用LeakyReluCustom算子 |
| [pybind](./pybind) | 本样例以Add算子为示例，介绍了使用Pybind方式调用核函数 |
| [static_tensor_programming](./static_tensor_programming) | 本样例介绍基于静态Tensor方式编程的场景下Add算子的实现方法，并提供核函数直调方法 |
| [transformer_buildin](./transformer_buildin) | 本样例展示了如何使用Ascend C自定义算子AddCustom映射到TensorFlow内置算子Add，并通过TensorFlow调用Ascend C算子 |
| [transformer_custom](./transformer_custom) | 本样例展示了如何使用Ascend C自定义算子AddCustom映射到TensorFlow自定义算子AddCustom，并通过TensorFlow调用Ascend C算子 |
| [tiling_sink](./tiling_sink) | 本样例基于Ascend C的自定义Tiling下沉算子及Pytorch调用样例，通过使能Tiling下沉，实现下沉执行优化调度性能 |