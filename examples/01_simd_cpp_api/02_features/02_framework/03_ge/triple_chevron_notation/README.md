# GE原生构图接入`<<<>>>`样例

## 概述

本样例基于`AddCustom`算子展示Ascend C `<<<>>>` kernel launch 如何接入GE原生图执行链路。

```text
Ascend C kernel
  -> EagerExecuteOp::Execute中直接add_custom<<<...>>>(...)
  -> ShapeInferOp提供输出shape和dtype推导
  -> REG_AUTO_MAPPING_OP(AddCustom)
  -> GE Graph: Data(x), Data(y), AddCustom
  -> Session::RunGraph
```

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
├── add_custom.asc          // Ascend C kernel + EagerExecuteOp::Execute + 直接<<<>>>调用
├── add_custom_proto.h      // AddCustom算子原型，供GE构图侧创建ge::op::AddCustom
└── graph.cpp               // GE在线构图和Session::RunGraph执行验证
```

## 样例描述

- 样例功能：

    ```text
    z = x + y
    ```

- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

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
mkdir -p build && cd build;
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..; make -j
export ASCEND_CUSTOM_OPP_PATH="$(pwd)/output:${ASCEND_CUSTOM_OPP_PATH}"
./demo
```

编译选项说明：

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：`dav-2201`对应Atlas A2/A3系列产品，`dav-3510`对应Ascend 950PR/Ascend 950DT |

## 构建产物

```text
build
├── demo
└── output
    └── op_graph
        ├── include
        │   └── add_custom_proto.h
        └── lib
            └── linux
                └── aarch64
                    └── libadd_custom_op.so
```

## 执行结果

成功时可观察到如下输出：

```text
Output: 0 0.3 0.6 0.9 ...
Golden: 0 0.3 0.6 0.9 ...
AddCustom GE triple-chevron sample success.
```
