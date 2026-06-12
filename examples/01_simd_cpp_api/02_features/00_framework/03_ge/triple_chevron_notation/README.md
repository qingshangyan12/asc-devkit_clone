# GE原生构图接入`<<<>>>`样例

## 概述

本样例基于AddCustom算子展示如何把Ascend C `<<<>>>`核函数调用接入GE原生图执行链路。

如果只想验证一个Ascend C核函数是否能启动，或者只在主机侧手动管理输入、输出和stream，直接调用`<<<>>>`即可，不需要GE入图。当自定义算子需要作为计算图的一部分参与执行，例如使用GE图引擎接口构建Graph、上层框架或图编译流程会把模型下沉到GE执行、需要GE在图编译阶段识别自定义算子的输入输出和shape/dtype时，就需要把自定义算子接入GE图。

本样例展示的核心链路为：

```text
add_custom.asc中的Ascend C kernel
  -> AddCustom::Execute中直接调用add_custom<<<...>>>(...)
  -> REG_AUTO_MAPPING_OP(AddCustom)注册GE执行实现
  -> add_custom_proto.h通过REG_OP(AddCustom)声明算子原型
  -> graph.cpp创建Data(x)、Data(y)、AddCustom图节点
  -> Session::RunGraph执行整张GE图
```

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── triple_chevron_notation
│   ├── CMakeLists.txt          // 编译GE custom op和在线构图执行程序
│   ├── add_custom.asc          // Ascend C kernel + GE执行实现 + 直接<<<>>>调用
│   ├── add_custom_proto.h      // AddCustom算子原型，供GE构图侧创建ge::op::AddCustom
│   ├── graph.cpp               // GE在线构图和Session::RunGraph执行验证
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

- 自定义算子原型：

  GE构图时需要根据算子原型创建算子实例。官方“使用图引擎接口全新构建Graph”文档中说明，构建Graph前需要了解算子原型，算子原型描述输入、输出、属性等信息，并通过`REG_OP`、`INPUT`、`OUTPUT`、`ATTR`、`OP_END_FACTORY_REG`等接口注册。

  本样例在`add_custom_proto.h`中通过`REG_OP(AddCustom)`声明自定义算子原型，`graph.cpp`依赖该原型创建`ge::op::AddCustom`节点。

  ```cpp
  REG_OP(AddCustom)
      .INPUT(x, TensorType({DT_FLOAT}))
      .INPUT(y, TensorType({DT_FLOAT}))
      .OUTPUT(z, TensorType({DT_FLOAT}))
      .OP_END_FACTORY_REG(AddCustom);
  ```

- GE在线构图：

  本样例在`graph.cpp`中创建`Data("x")`和`Data("y")`作为图输入，创建`ge::op::AddCustom("add_custom")`作为自定义算子节点，并通过`Session::RunGraph`执行整张GE图。

  ```cpp
  auto add = ge::op::AddCustom("add_custom").set_input_x(x).set_input_y(y);
  session.RunGraph(GRAPH_ID, inputs, outputs);
  ```

- GE自定义算子执行实现：

  本样例在`add_custom.asc`中实现`ge::AddCustom`。`AddCustom::InferShape()`和`AddCustom::InferDataType()`用于让GE在图编译阶段得到输出shape和dtype；`AddCustom::Execute()`是GE执行到该节点时进入用户代码的位置。

  `AddCustom::Execute()`内部直接使用GE提供的stream调用Ascend C核函数：

  ```cpp
  add_custom<BLOCK_LENGTH><<<BLOCK_NUM, nullptr, ctx->GetStream()>>>(
      (float *)const_cast<void *>(x->GetAddr()),
      (float *)const_cast<void *>(y->GetAddr()),
      (float *)z->GetAddr());
  ```

  `REG_AUTO_MAPPING_OP(AddCustom)`用于把GE图中的`AddCustom`节点映射到这里的用户实现。因此，本样例中的`<<<>>>`调用不是由测试程序直接发起，而是由GE图节点的执行入口发起。

- Ascend C核函数实现：

  本样例使用8个核完成计算，每个核处理2048个`float`元素。每个核按`block_idx * blockLength`计算当前核负责的GM偏移，计算流程为GM输入搬运到UB，调用`AscendC::Add`完成向量加法，再将结果从UB搬回GM输出。

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

  看到`AddCustom GE triple-chevron sample success.`且`./demo`返回码为0，说明GE图执行成功。该样例的运行入口是`Session::RunGraph`，可用于确认自定义算子是作为GE图节点执行，而不是由测试程序直接调用`add_custom<<<...>>>`。

## 相关文档

- [什么是算子原型](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/programug/graphdevg/atlasag_25_0008.html)
- [通过算子原型构建Graph](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/programug/graphdevg/atlasag_25_0009.html)
