# ACLGraph样例

## 概述

本样例以Add算子为例，介绍如何通过ACL Model RI接口捕获Ascend C `<<<>>>`核函数调用，并将一段运行时任务保存为可重复执行的runtime model instance。

> **什么是ACLGraph？** <br>
> 本样例中的ACLGraph指通过 `aclmdlRICaptureBegin` 和 `aclmdlRICaptureEnd` 捕获stream上的运行时任务，生成 `aclmdlRI` 执行实例。与每次逐条提交拷贝和核函数任务不同，capture方式会先记录一段固定的任务流程，再通过 `aclmdlRIExecuteAsync` 整体重复执行，适用于任务拓扑固定、需要多次执行的场景。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```text
├── 04_aclgraph
│   ├── CMakeLists.txt          // 编译工程：通过SCENARIO_NUM选择单流或双流场景
│   ├── add.asc                 // Ascend C核函数实现和ACLGraph capture调用样例
│   └── README.md               // 样例说明文档
```

## 场景详细说明

本样例通过编译参数 `SCENARIO_NUM` 选择不同的ACLGraph capture场景。所有场景使用相同的Add算子规格：输入和输出形状均为 `[8, 2048]`，数据类型为 `float`，核函数名为 `add_custom`。

| `SCENARIO_NUM` | 场景 | capture拓扑 | 说明 |
| --- | --- | --- | --- |
| 1 | 单流 | H2D拷贝、核函数启动、D2H拷贝均提交到同一个stream | 展示最基础的线性任务捕获和重复执行流程 |
| 2 | 双流 | `mainStream` 和 `computeStream` 通过event建立依赖 | 展示跨stream任务和event同步关系的捕获方式 |

**场景1：单流capture**
- 输入：x [8, 2048] float类型；y [8, 2048] float类型
- 输出：z [8, 2048] float类型
- 实现：在 `mainStream` 上依次提交x/y的Host到Device拷贝、`add_custom`核函数启动和z的Device到Host拷贝。
- 说明：`aclmdlRICaptureEnd` 生成的 `aclmdlRI` 保存一条线性任务链，后续通过 `aclmdlRIExecuteAsync` 整体重放。

**场景2：双流capture**
- 输入：x [8, 2048] float类型；y [8, 2048] float类型
- 输出：z [8, 2048] float类型
- 实现：`mainStream` 负责x的Host到Device拷贝和z的Device到Host拷贝；`computeStream` 等待x拷贝完成后执行y拷贝和 `add_custom` 核函数。两个stream之间通过 `aclrtRecordEvent` 和 `aclrtStreamWaitEvent` 建立依赖。
- 说明：`aclmdlRI` 不只记录单个stream上的任务顺序，也记录跨stream的event依赖，适合展示多stream任务拓扑的capture方式。

## 样例实现

### 算子实现

算子实现的核心内容在 `add.asc` 中，Add的计算公式为：

$$
z = x + y
$$

- x：输入，形状为[8, 2048]，数据类型为float；
- y：输入，形状为[8, 2048]，数据类型为float；
- z：输出，形状为[8, 2048]，数据类型为float；

核函数中每个block处理2048个float元素，8个block共同完成全部输入数据的计算。每个block先将x和y从GM搬运到UB，调用 `AscendC::Add` 完成向量加法，再将结果写回GM。

### ACLGraph捕获流程

Host侧首先调用 `aclmdlRICaptureBegin` 进入capture状态，然后提交需要记录的运行时任务，最后调用 `aclmdlRICaptureEnd` 结束capture并获取 `aclmdlRI`。

```cpp
aclmdlRICaptureBegin(mainStream, ACL_MODEL_RI_CAPTURE_MODE_GLOBAL);

// 提交数据拷贝、核函数启动、event同步等任务

aclmdlRICaptureEnd(mainStream, &modelRI);
```

生成 `modelRI` 后，通过 `aclmdlRIExecuteAsync` 重复执行捕获到的任务流程。

```cpp
for (int i = 0; i < 5; ++i) {
    aclmdlRIExecuteAsync(modelRI, mainStream);
    aclrtSynchronizeStream(mainStream);
}
```

样例还调用 `aclmdlRIDebugJsonPrint` 将捕获到的任务信息输出为JSON文件，单流场景输出 `modelRI_single_stream.json`，双流场景输出 `modelRI_double_stream.json`。

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
  SCENARIO_NUM=1
  mkdir -p build && cd build;
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;
  ./demo
  ```

  将 `SCENARIO_NUM` 设置为2可编译运行双流capture场景。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品 与 Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1、2 | 场景编号：1表示单流capture，2表示双流capture |

- 执行结果

  成功时可观察到如下输出：

  ```bash
  Output: 0 0.3 0.6 0.9 1.2 ...
  Golden: 0 0.3 0.6 0.9 1.2 ...
  test pass!
  ```
