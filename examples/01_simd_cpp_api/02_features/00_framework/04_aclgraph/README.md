# ACLGraph样例

## 概述

当一段固定的数据拷贝和计算流程需要重复执行时，主机侧默认需要在每次执行前重新提交数据拷贝、核函数启动和同步等运行时任务；当重复次数较多时，这部分任务下发开销会影响整体执行效率。

ACLGraph 可以先捕获指定流上的一组运行时任务，再将这些任务作为一个整体重复执行。具体流程是：通过 `aclmdlRICaptureBegin` 和 `aclmdlRICaptureEnd` 生成 `aclmdlRI` 执行实例；后续通过 `aclmdlRIExecuteAsync` 执行该实例，无需再次逐条提交相同任务。关于ACLGraph的更多内容，可参考[ACLGraph文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/programug/acldevg/runtime_doc_dev_0045.html)。

本样例以Add算子为例，展示如何捕获Ascend C `<<<>>>` 核函数调用及其前后的数据拷贝任务，并分别演示单流线性任务捕获、双流事件依赖捕获，以及图重放发生AI Core异常时的Exception Dump。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.1.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── 04_aclgraph
│   ├── figures                        // 图示
│   │   ├── single_stream_capture.png  // 单流捕获流程图
│   │   └── double_stream_capture.png  // 双流捕获流程图
│   ├── CMakeLists.txt                 // 编译工程
│   ├── acl.json                       // aclInit配置文件，用于使能Exception Dump（仅场景3使用）
│   ├── add.asc                        // Ascend C核函数实现和ACLGraph捕获调用样例
│   └── README.md                      // 样例说明文档
```

## 场景详细说明

本样例通过CMake编译参数 `SCENARIO_NUM` 选择不同的ACLGraph捕获场景，`SCENARIO_NUM` 不同取值对应的含义如下表所示。所有场景使用相同的Add算子规格：输入和输出形状均为 `[8, 2048]`，数据类型为 `float`，核函数名为 `add_custom`。

<table>
<caption style="font-weight: normal;">
         <span style="font-weight: bold; font-size: 1.2em;">📌 表1：SCENARIO_NUM不同取值的含义</span></caption>
<tr><td rowspan="1" align="center">SCENARIO_NUM</td><td align="center">场景</td><td align="center">任务组织方式</td><td align="center">说明</td></tr>
<tr><td align="center">1</td><td align="center">单流</td><td align="center">全部任务提交到 <code>mainStream</code></td><td align="center">展示最基础的线性任务捕获和重复执行流程</td></tr>
<tr><td align="center">2</td><td align="center">双流</td><td align="center"><code>mainStream</code> 与 <code>computeStream</code> 通过事件同步</td><td align="center">展示跨流任务和事件同步关系的捕获方式</td></tr>
<tr><td align="center">3</td><td align="center">单流 + Exception Dump</td><td align="center">全部任务提交到 <code>mainStream</code>，核函数内触发AI Core异常</td><td align="center">展示 <code>&lt;&lt;&lt;&gt;&gt;&gt;</code> 场景下自行实现异常回调并落盘Tensor数据的方法</td></tr>
</table>

**场景1：单流捕获**
- 输入：x [8, 2048] float类型；y [8, 2048] float类型
- 输出：z [8, 2048] float类型
- 捕获内容：在 `mainStream` 上依次提交x、y输入数据拷贝、`add_custom` 核函数启动和z输出数据拷贝。
- 说明：`aclmdlRICaptureEnd` 生成的 `aclmdlRI` 保存一条线性任务链，后续通过 `aclmdlRIExecuteAsync` 整体重放。该场景用于展示最基础的“捕获一次，多次执行”流程。

<p align="center">
  <img src="figures/single_stream_capture.png" width="800">
</p>

**场景2：双流捕获**
- 输入：x [8, 2048] float类型；y [8, 2048] float类型
- 输出：z [8, 2048] float类型
- 捕获内容：`mainStream` 负责x输入数据拷贝和z输出数据拷贝；`computeStream` 等待x拷贝完成后执行y输入数据拷贝和 `add_custom` 核函数。两个流之间通过 `aclrtRecordEvent` 和 `aclrtStreamWaitEvent` 建立依赖。
- 说明：`aclmdlRI` 不只记录单个流上的任务顺序，也记录跨流的事件依赖，适合展示多流任务关系的捕获方式。

<p align="center">
  <img src="figures/double_stream_capture.png" width="800">
</p>

**场景3：单流捕获 + Exception Dump**
- 输入：x [8, 2048] float类型；y [8, 2048] float类型
- 输出：z [8, 2048] float类型
- 捕获内容：任务组织方式与场景1完全相同，仅把核函数替换为 `add_custom_fault`，该核函数在计算完成后于指定block上执行 `trap` 指令触发AI Core异常。
- 说明：`<<<>>>` 直调场景下没有算子描述信息，CANN无法自动Dump出核函数的Tensor数据，需要用户自行注册异常回调并落盘。本场景演示该回调的实现方法。

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

本样例中的ACLGraph可以理解为“捕获一次，重复执行”。主机侧首先调用 `aclmdlRICaptureBegin` 进入捕获状态，然后提交需要记录的运行时任务，最后调用 `aclmdlRICaptureEnd` 结束捕获并获取 `aclmdlRI`。

捕获流程如下：

1. 开始捕获：调用 `aclmdlRICaptureBegin`，开始捕获指定流上的运行时任务。
2. 任务提交：提交输入数据拷贝、核函数启动、输出数据拷贝，以及双流场景中的事件同步任务。
3. 结束捕获：调用 `aclmdlRICaptureEnd`，结束捕获并生成 `aclmdlRI`。
4. 重复执行：调用 `aclmdlRIExecuteAsync`，重复执行已经捕获的任务流程。
5. 资源释放：调用 `aclmdlRIDestroy`，释放不再使用的 `aclmdlRI` 执行实例。

```cpp
aclmdlRICaptureBegin(mainStream, ACL_MODEL_RI_CAPTURE_MODE_GLOBAL);

// 提交输入数据拷贝、核函数启动、输出数据拷贝、事件同步等运行时任务

aclmdlRICaptureEnd(mainStream, &modelRI);
```

生成 `modelRI` 后，通过 `aclmdlRIExecuteAsync` 重复执行捕获到的任务流程。

```cpp
for (int i = 0; i < 5; ++i) {
    aclmdlRIExecuteAsync(modelRI, mainStream);
    aclrtSynchronizeStream(mainStream);
}
```

样例还调用 `aclmdlRIDebugJsonPrint` 将捕获到的任务信息输出为JSON文件，单流场景输出 `modelRI_single_stream.json`，双流场景输出 `modelRI_double_stream.json`，Exception Dump场景输出 `modelRI_exception_dump.json`。

### Exception Dump实现（场景3）

`<<<>>>` 直调核函数时，CANN侧没有该核函数的算子描述信息，因此Exception Dump无法自动感知核函数有哪些Tensor、shape和数据类型，需要用户自行注册异常回调补齐这部分信息。本场景组合使用以下两类接口：

- `aclrtExceptionInfoCallbackRegister`：注册异常回调。AI Core异常发生后，Runtime在内部线程中调用该回调，并传入 `aclrtExceptionInfo`。
- `acldumpSaveExceptionInfo`：在回调中把用户构造的Tensor数据主动落盘到Exception Dump路径下。

**使能Exception Dump**

Exception Dump需要通过 `aclInit` 的配置文件使能，本样例的 `acl.json` 内容如下，编译时会自动拷贝到 `build` 目录下。

```json
{
    "dump": {
        "dump_path": "./",
        "dump_scene": "aic_err_brief_dump"
    }
}
```

**注册异常回调**

异常回调的注册不是Stream任务，不会被捕获，但必须在图重放之前完成。同时建议把流设置为遇错即停，避免后续任务继续下发掩盖异常现场。

```cpp
aclInit("./acl.json");
aclrtSetDevice(deviceId);
aclrtExceptionInfoCallbackRegister(ExceptionCallback);
aclrtSetStreamFailureMode(mainStream, ACL_STOP_ON_FAILURE);
```

**在回调中获取异常信息并落盘**

回调中先通过 `aclrtGetXxxFromExceptionInfo` 系列接口获取定位信息，再通过 `aclrtGetArgsFromExceptionInfo` 拿到核函数args在Device上的地址。`<<<>>>` 调用的args按核函数形参顺序打包，本样例中依次为x、y、z的GM地址，拷回Host后即可构造 `acldumpTensorInfo` 并落盘。

```cpp
void ExceptionCallback(aclrtExceptionInfo* exceptionInfo)
{
    // 1. 获取定位信息：deviceId、streamId、taskId、errorCode。
    // 2. 通过aclrtGetFuncHandleFromExceptionInfo和aclrtGetFunctionName获取出错的核函数名。
    // 3. 通过aclrtGetArgsFromExceptionInfo获取args地址，拷回Host后解析出x、y、z的GM地址。
    // 4. 构造acldumpTensorInfo，调用acldumpSaveExceptionInfo落盘。
    acldumpSaveExceptionInfo("aclgraph_add_exception", userTag, tensors, argNum);
}
```

> [!NOTE]说明
> 异常回调运行在Runtime内部线程中，无法访问业务函数的局部变量，因此样例在捕获开始前把Dump所需的Tensor描述记录到全局变量中。回调中不要调用 `aclrtSynchronizeStream` 等阻塞接口，也不要继续向出错的流下发任务。

> [!CAUTION]注意
> `acldumpTensorInfo` 的 `dataType` 字段使用Dump文件的 `OutputDataType` 枚举，取值与 `aclDataType` 不同，float在该枚举中的取值为1。若误填 `aclDataType::ACL_FLOAT`（取值为0），接口仍会返回成功且数据正常落盘，但Dump文件中记录的数据类型会变成 `DT_UNDEFINED`，导致后续解析工具无法正确识别数据类型。

落盘结果保存在 `<dump_path>/extra-info/data-dump/<deviceId>/` 目录下，文件名为 `aclgraph_add_exception.custom.{timestamp}`，可通过 `acldumpGetExceptionInfoPath` 查询该根路径。

## 编译运行

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例根目录下执行如下步骤，运行该样例。

  ```bash
  SCENARIO_NUM=1  # 设置场景编号
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;
  ./demo
  ```

  `SCENARIO_NUM=1` 表示编译单流捕获场景，`SCENARIO_NUM=2` 表示编译双流捕获场景，`SCENARIO_NUM=3` 表示编译Exception Dump场景。不指定 `SCENARIO_NUM` 时，默认编译单流捕获场景。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品 与 Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`（默认）、`2`、`3` | 捕获场景编号：1表示单流捕获，2表示双流捕获，3表示单流捕获 + Exception Dump |

- 执行结果

  场景1和场景2成功时可观察到如下输出：

  ```bash
  Output: 0 0.3 0.6 0.9 1.2 ...
  Golden: 0 0.3 0.6 0.9 1.2 ...
  test pass!
  ```

  场景3会主动触发AI Core异常，核函数必定执行失败，因此不校验计算结果。成功时可观察到如下输出：

  ```bash
  [Host] exception dump path: .../extra-info/data-dump/0/
  [Host] executing captured graph, an AI Core exception is expected on block 4.
  [Exception] deviceId=0, streamId=..., taskId=..., errorCode=...
  [Exception] kernel name: ...add_custom_fault...
  [Exception] x=0x..., y=0x..., z=0x...
  [Exception] acldumpSaveExceptionInfo ret=0
  [Host] aclrtSynchronizeStream failed as expected, ret=...
  [Host] recent error message: ...
  exception dump scenario finished, check the dump files under <dump_path>/extra-info/data-dump/<deviceId>/.
  ```

  > [!NOTE]说明
  > 若日志中出现 `exception dump is not enabled`，说明 `acl.json` 未被正确加载，请确认该文件与 `demo` 在同一目录下。
