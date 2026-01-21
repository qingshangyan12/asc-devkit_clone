# FrameworkLaunch样例介绍

## 概述
基于Ascend C自定义算子工程，介绍了Ascend C算子实现、编译部署、单算子调用等一系列开发流程。

## 算子开发样例
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [aclop_invocation](./aclop_invocation) | 本样例基于示例自定义算子工程，介绍了aclnn`OpType`单算子API的方式执行固定shape算子。 |
| [aclnn_invocation](./aclnn_invocation) | 本样例基于示例自定义算子工程，介绍了`aclopExecuteV2`单算子模型的方式执行固定shape算子。 |
| [custom_op](./custom_op) | 本样例以简单的自定义算子为示例，展示了其编译、打包成自定义算子包，并部署到CANN环境中的流程。 |
| [tiling_template_programming](./tiling_template_programming) | 本样例基于示例自定义算子工程，使用Tiling模板编程进行单算子API方式的算子执行，以有效减少多TilingKey的复杂度。 |
| [msopgen_custom](./msopgen_custom) | 本样例基于AddCustom算子工程，介绍了msOpGen工具生成简易自定义算子工程和单算子调用 |
| [onnx_plugin](./onnx_plugin) | 本样例介绍通过onnx网络调用的方式调用LeakyReluCustom算子 |
| [pybind](./pybind) | 本样例以Add算子为示例，介绍了使用Pybind方式调用核函数 |
| [static_tensor_programming](./static_tensor_programming) | 本样例介绍基于静态Tensor方式编程的场景下Add算子的实现方法，并提供核函数直调方法 |
| [transformer_buildin](./transformer_buildin) | 本样例展示了如何使用Ascend C自定义算子AddCustom映射到TensorFlow内置算子Add，并通过TensorFlow调用Ascend C算子 |
| [transformer_custom](./transformer_custom) | 本样例展示了如何使用Ascend C自定义算子AddCustom映射到TensorFlow自定义算子AddCustom，并通过TensorFlow调用Ascend C算子 |
| [tiling_sink_programming](./tiling_sink_programming) | 本样例基于示例自定义算子工程，介绍了PyTorch图模式下调用自定义算子，并通过使能Tiling下沉到device侧执行，优化调度性能。 |
| [standard_cpp_tiling_struct](./standard_cpp_tiling_struct) | 本样例以Matmul算子为示例，展示了标准C++语法定义Tiling结构体样例。本样例使用自定义算子工程，编译并部署自定义算子包到自定义算子库中，并调用执行自定义算子 |
| [static_library](./static_library) | 本样例基于AddCustom算子工程和MatmulCustom算子工程，介绍了自定义算子工程静态库的集成和使用。 |