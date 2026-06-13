# Ascend C算子接入GE样例

## 概述

本样例基于AddCustom算子展示如何把Ascend C `<<<>>>`核函数调用接入GE（Graph Engine，图引擎）原生图执行链路。

> **什么是GE？** <br>GE是昇腾计算架构中的图执行引擎。与Eager模式（每个算子下发后立即执行）不同，GE图模式将所有算子构造成一张图，以图为单位下发执行。相较于单个计算操作依次下发的方式，图模式具备图的全局视角，能够更有效地简化和优化计算图操作，从而获得更优执行性能。详细内容可参考：[图模式开发指南](https://hiascend.com/document/redirect/CannCommunityGraphguide)。

**执行流程简述**：

1. `add_custom_proto.h` 中 `REG_OP(AddCustom)` 声明算子原型（有哪些输入、输出）
2. `graph.cpp` 根据原型创建 `ge::op::AddCustom` 图节点，并通过 `Session::RunGraph` 执行整张GE图
3. GE运行到 `AddCustom` 节点时，自动调用 `add_custom.asc` 中的 `AddCustom::Execute()`

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── triple_chevron_notation
│   ├── CMakeLists.txt          // 编译工程：产出自定义算子动态库 + 在线构图执行程序
│   ├── add_custom.asc          // Ascend C核函数实现 + GE算子执行实现（InferShape/InferDataType/Execute）
│   ├── add_custom_proto.h      // 算子原型声明（REG_OP），供GE构图侧创建算子节点
│   ├── graph.cpp               // GE在线构图（Data + AddCustom节点）和Session::RunGraph执行验证
│   └── README.md               // 样例说明文档
```

> **文件角色说明**：本样例共4个源文件，分属"GE构图侧"和"算子执行侧"两个世界：
> - **构图侧**（`graph.cpp` + `add_custom_proto.h`）：负责描述"图长什么样"——有哪些算子节点、如何连接
> - **执行侧**（`add_custom.asc`）：负责描述"算子怎么算"——GE运行到该节点时执行什么代码
> - **构建系统**（`CMakeLists.txt`）：将执行侧编译为动态库（供GE加载），将构图侧编译为可执行程序

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

### 第一步：声明算子原型（add_custom_proto.h）

GE构图时需要知道每个算子"长什么样"——有几个输入、几个输出、各是什么数据类型。这些信息通过算子原型（Operator Proto）描述，并用`REG_OP`宏注册。

本样例在 `add_custom_proto.h` 中声明 `AddCustom` 算子原型：两个 float 输入 `x`、`y`，一个 float 输出 `z`。

```cpp
REG_OP(AddCustom)
    .INPUT(x, TensorType({DT_FLOAT}))
    .INPUT(y, TensorType({DT_FLOAT}))
    .OUTPUT(z, TensorType({DT_FLOAT}))
    .OP_END_FACTORY_REG(AddCustom);
```

### 第二步：GE在线构图（graph.cpp）

本样例在 `graph.cpp` 中使用GE API构造一张包含3个节点的计算图：

```cpp
// 1. 创建两个数据输入节点
auto x = ge::op::Data("x");       // 输入 x
auto y = ge::op::Data("y");       // 输入 y

// 2. 创建 AddCustom 算子节点，将 x、y 连接到它的输入
auto add = ge::op::AddCustom("add_custom").set_input_x(x).set_input_y(y);

// 3. 将节点组装为图，并执行
graph->SetInputs({x, y}).SetOutputs({add});
session.RunGraph(GRAPH_ID, inputs, outputs);
```

`Session::RunGraph` 是整个样例的运行入口。GE引擎会按图的拓扑顺序依次执行各节点——运行到 `AddCustom` 节点时，自动调用 `add_custom.asc` 中的 `AddCustom::Execute()`。

### 第三步：实现GE算子执行逻辑（add_custom.asc）

本样例在 `add_custom.asc` 中实现 `ge::AddCustom` 类，继承 `EagerExecuteOp` 和 `ShapeInferOp`，提供三个关键方法：

| 方法 | 调用时机 | 作用 |
|------|---------|------|
| `InferShape()` | 图编译阶段 | 推导输出张量的 shape（本样例中输出与输入 shape 相同） |
| `InferDataType()` | 图编译阶段 | 推导输出张量的数据类型（本样例中输出与输入 dtype 相同） |
| `Execute()` | 图运行阶段，GE执行到该节点时 | 获取输入地址，调用 Ascend C 核函数执行计算 |

`Execute()` 内部通过 **三重尖括号 `<<<>>>`** 语法发起核函数调用：

```cpp
add_custom<BLOCK_LENGTH><<<BLOCK_NUM, nullptr, ctx->GetStream()>>>(
    (float *)const_cast<void *>(x->GetAddr()),
    (float *)const_cast<void *>(y->GetAddr()),
    (float *)z->GetAddr());
```

最后，`REG_AUTO_MAPPING_OP(AddCustom)` 将 GE 图中的 `AddCustom` 节点映射到此处定义的 `ge::AddCustom` 类。因此，`<<<>>>` 调用不是由测试程序直接发起，而是由 GE 图节点的执行入口自动触发。

### 第四步：Ascend C核函数实现（add_custom.asc）

核函数 `add_custom` 使用8个核完成计算，每个核处理2048个 `float` 元素。每个核的计算流程为：

```text
1. 按 block_idx * blockLength 计算当前核负责的 Global Memory 偏移
2. 将 GM 输入数据搬运到 UB（Unified Buffer）
3. 调用 AscendC::Add 完成向量加法
4. 将结果从 UB 搬回 GM 输出
```

## 编译运行

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
  export ASCEND_CUSTOM_OPP_PATH="$(pwd)/output:${ASCEND_CUSTOM_OPP_PATH}"    # 配置自定义算子包路径
  ./demo    # 执行样例
  ```

  > **为什么需要设置 `ASCEND_CUSTOM_OPP_PATH`？** 编译产出的自定义算子动态库（`libadd_custom_op.so`）和算子原型头文件位于 `build/output/` 目录下。GE 运行时需要加载该动态库才能识别 `AddCustom` 算子并调用其执行实现。`ASCEND_CUSTOM_OPP_PATH` 环境变量告诉 GE 到哪里查找自定义算子包。

- 编译选项说明

  | 选项　　　　　 | 可选值　　　　　　　　　　　| 说明　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　 |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品 与 Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  成功时可观察到如下输出：

  ```bash
  Output: 0 0.3 0.6 0.9 ...
  Golden: 0 0.3 0.6 0.9 ...
  AddCustom GE triple-chevron sample success.
  ```


## 相关文档

- [什么是算子原型](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/programug/graphdevg/atlasag_25_0008.html)
- [通过算子原型构建Graph](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/programug/graphdevg/atlasag_25_0009.html)
