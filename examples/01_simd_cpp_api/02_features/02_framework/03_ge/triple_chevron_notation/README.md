# GE原生构图接入`<<<>>>`样例

## 概述

本样例基于`AddCustom`算子展示Ascend C `<<<>>>` kernel launch 如何接入GE原生图执行链路。

本样例只演示GE在线构图：

```text
Ascend C kernel
  -> EagerExecuteOp::Execute中直接add_custom<<<...>>>(...)
  -> ShapeInferOp提供输出shape和dtype推导
  -> REG_AUTO_MAPPING_OP(AddCustom)
  -> GE Graph: Data(x), Data(y), AddCustom
  -> Session::RunGraph
```

不涉及PyTorch、TorchAir、TensorFlow、ATC离线编译或OM模型下沉。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 支持的CANN软件版本

- >= CANN 9.0.0

## 目录结构介绍

```text
triple_chevron_notation
├── CMakeLists.txt          // 编译GE custom op和在线构图执行程序
├── README.md
├── add_custom.asc          // Ascend C kernel + EagerExecuteOp::Execute + 直接<<<>>>调用
├── add_custom_proto.h      // AddCustom算子原型，供GE构图侧创建ge::op::AddCustom
└── graph.cpp               // GE在线构图和Session::RunGraph执行验证
```

## 样例描述

样例功能：

```text
z = x + y
```

样例规格：

| 项目 | 内容 |
| --- | --- |
| 算子类型 | `AddCustom` |
| 输入 | `x`, `y` |
| 输出 | `z` |
| Shape | `[8, 2048]` |
| 数据类型 | `float` |
| Format | `ND` |
| kernel名称 | `add_custom` |

Kernel实现参考`examples/01_simd_cpp_api/00_introduction/01_vector/add`：

- 使用8个核完成计算，每个核处理2048个`float`元素。
- 每个核按`block_idx * blockLength`计算当前核负责的GM偏移。
- 计算流程为GM输入搬运到UB，调用`AscendC::Add`完成向量加法，再将结果从UB搬回GM输出。

## GE和入图说明

GE是CANN中的Graph Engine，用于接收计算图、完成图编译和图执行调度。

“入图”表示自定义算子不是单独直调kernel，而是成为GE图中的一个节点。本样例中：

- `add_custom_proto.h`中的`REG_OP(AddCustom)`让构图侧可以创建`ge::op::AddCustom`节点。
- `add_custom.asc`中的`REG_AUTO_MAPPING_OP(AddCustom)`让GE执行`AddCustom`节点时找到用户实现。
- `add_custom.asc`中的`AddCustom::Execute()`是GE执行该节点时进入用户代码的位置。
- `add_custom.asc`中的`AddCustom::InferShape()`和`InferDataType()`让GE编译阶段得到输出shape和dtype。
- `Execute()`内部直接调用`add_custom<<<...>>>(...)`，展示GE节点执行最终如何下发Ascend C kernel。

注意：`.asc`源文件不是GE加载的交付件，CMake编译出的`.so`才是GE加载对象。

## 编译运行

配置CANN环境变量：

```bash
source ${install_path}/cann/set_env.sh
```

在本样例根目录下执行：

```bash
mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
make -j
cd ..
export ASCEND_CUSTOM_OPP_PATH="$(pwd)/output:${ASCEND_CUSTOM_OPP_PATH}"
./build/triple_chevron_graph
```

样例默认通过`ge.exec.deviceId=1`使用1卡执行。如需切换设备，请修改`graph.cpp`中的`ge.exec.deviceId`。

编译选项说明：

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：`dav-2201`对应Atlas A2/A3系列产品，`dav-3510`对应Ascend 950PR/Ascend 950DT |

## 构建产物

```text
output
└── op_graph
    ├── include
    │   └── add_custom_proto.h
    └── lib
        └── linux
            └── x86_64 或 aarch64
                └── libadd_custom_op.so
build
└── triple_chevron_graph
```

## 执行结果

成功时可观察到如下输出：

```text
Output: 0 0.3 0.6 0.9 ...
Golden: 0 0.3 0.6 0.9 ...
AddCustom GE triple-chevron sample success.
```

其中`Output`为GE图执行得到的输出，`Golden`为Host侧根据`z = x + y`计算出的期望结果。

## 常见问题

- `GEInitialize failed`：请确认CANN环境已通过`set_env.sh`配置。
- GE无法识别`AddCustom`：请确认`ASCEND_CUSTOM_OPP_PATH`包含当前样例的`output`目录。
- 编译找不到GE或ACL头文件：请确认`ASCEND_HOME_PATH`指向CANN toolkit目录，或通过`-DASCEND_HOME_PATH_OVERRIDE=<path>`指定。
