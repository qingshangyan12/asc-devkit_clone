# soc_version获取方法

本文说明aclnn自定义算子工程中`soc_version`的获取方法，以及它在工程创建、编译配置和算子原型注册中的使用位置。

## 1. 概述

`soc_version`是AI处理器型号标识，用于指定算子面向哪类AI处理器编译和运行。创建算子工程、配置`CMakePresets.json`、注册算子原型和设置Kernel编译选项时，都需要使用正确的`soc_version`。

## 2. 获取方法

不同产品系列获取`soc_version`的命令和字段不同，分为以下三类。

### 2.1 Atlas A2 训练/推理、Atlas 200I/500 A2 推理、Atlas 推理、Atlas 训练系列产品

针对如下产品：在安装AI处理器的服务器执行`npu-smi info`命令进行查询，获取`Name`信息。实际配置值为`Ascend{Name}`，例如`Name`取值为`xxxyy`，实际配置值为`Ascendxxxyy`。

### 2.2 Atlas A3 训练/推理系列产品

针对Atlas A3 训练系列产品和Atlas A3 推理系列产品，在安装AI处理器的服务器执行`npu-smi info -t board -i id -c chip_id`命令进行查询，获取`Chip Name`和`NPU Name`信息，实际配置值为`Chip Name_NPU Name`。例如`Chip Name`取值为`Ascendxxx`，`NPU Name`取值为`1234`，实际配置值为`Ascendxxx_1234`。其中：

- `id`：设备id，通过`npu-smi info -l`命令查出的`NPU ID`即为设备id。
- `chip_id`：芯片id，通过`npu-smi info -m`命令查出的`Chip ID`即为芯片id。

查询步骤如下：

1. 执行`npu-smi info -l`获取设备ID。
2. 执行`npu-smi info -m`获取芯片ID。
3. 执行`npu-smi info -t board -i <设备ID> -c <芯片ID>`。
4. 在输出中查找`Chip Name`和`NPU Name`字段。
5. 按`Chip Name_NPU Name`格式得到配置值。

### 2.3 Ascend 950PR/Ascend 950DT

针对Ascend 950PR和Ascend 950DT，在安装AI处理器的服务器执行`npu-smi info -t board -i id`命令进行查询，获取`Chip Name`和`NPU Name`信息，实际配置值为`Chip Name_NPU Name`。例如`Chip Name`取值为`Ascendxxx`，`NPU Name`取值为`1234`，实际配置值为`Ascendxxx_1234`。

其中，`id`为设备id，通过`npu-smi info -l`命令查出的`NPU ID`即为设备id。

查询步骤如下：

1. 执行`npu-smi info -l`获取设备ID。
2. 执行`npu-smi info -t board -i <设备ID>`。
3. 在输出中查找`Chip Name`和`NPU Name`字段。
4. 按`Chip Name_NPU Name`格式得到配置值。

## 3. 使用场景

### 3.1创建算子工程

使用`msopgen`创建算子工程时，通过`-c`参数指定`soc_version`：

```bash
msopgen gen -i add_custom.json -c ai_core-Ascend910B -lan cpp -out AddCustom
```

创建算子工程的完整流程见[创建算子工程](../创建算子工程.md)。

### 3.2配置编译选项

在`CMakePresets.json`中通过`ASCEND_COMPUTE_UNIT`配置编译目标：

```json
{
    "cacheVariables": {
        "ASCEND_COMPUTE_UNIT": {
            "type": "STRING",
            "value": "Ascend910B"
        }
    }
}
```

算子工程编译配置见[算子工程编译](../算子包编译/算子工程编译.md)。

### 3.3注册算子原型

在算子原型定义中，通过`AddConfig`注册AI处理器型号：

```cpp
this->AICore().AddConfig("Ascend910B");
```

原型定义中的AI处理器支持范围配置见[算子原型定义](../设计与实现/算子原型定义.md)。

### 3.4设置Kernel编译选项

手工组织算子工程时，可以在`op_kernel/CMakeLists.txt`中通过`COMPUTE_UNIT`指定Kernel编译目标：

```cmake
npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustom
    KERNEL_DIR ./Add
    COMPUTE_UNIT Ascend910B
    KERNEL_FILE add_custom.cpp
)
```

`npu_op_kernel_sources`的参数说明见[算子工程编译拓展](../算子工程编译拓展.md)。

## 4. 注意事项

- `soc_version`配置值区分大小写，需要按实际查询结果和工程配置要求填写。
- 不同产品系列使用的查询命令不同，需要按设备所属产品系列选择命令。
- 服务器上存在多个AI处理器时，需要按设备分别查询`soc_version`。
- 同系列AI处理器型号创建的算子工程，基础开发、编译和部署流程通常一致。
