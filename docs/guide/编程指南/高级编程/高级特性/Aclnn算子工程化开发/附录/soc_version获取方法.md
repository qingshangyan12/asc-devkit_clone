# soc_version 获取方法

> 类型：参考

**适用读者**：所有使用 Ascend C 进行算子开发的开发者

## 概述

soc_version（SoC 版本号）是 AI 处理器的型号标识，用于指定算子在哪个 AI 处理器型号上编译和运行。在创建算子工程、配置编译选项、注册算子原型时都需要正确设置 soc_version。

## 获取方法

不同产品系列使用不同的命令获取 soc_version，具体方法如下表所示：

| 产品系列 | 查询命令 | 获取字段 | 配置值格式 | 示例 |
|---------|---------|---------|-----------|------|
| Atlas A2 训练系列产品 | `npu-smi info` | Name | Ascend{Name} | Name=910B → Ascend910B |
| Atlas A2 推理系列产品 | `npu-smi info` | Name | Ascend{Name} | Name=310P → Ascend310P |
| Atlas 200I/500 A2 推理产品 | `npu-smi info` | Name | Ascend{Name} | Name=310B → Ascend310B |
| Atlas 推理系列产品 | `npu-smi info` | Name | Ascend{Name} | Name=310 → Ascend310 |
| Atlas 训练系列产品 | `npu-smi info` | Name | Ascend{Name} | Name=910 → Ascend910 |
| Atlas A3 训练系列产品 | `npu-smi info -t board -i id -c chip_id` | Chip Name + NPU Name | {ChipName}_{NPUName} | ChipName=Ascend910, NPUName=1234 → Ascend910_1234 |
| Atlas A3 推理系列产品 | `npu-smi info -t board -i id -c chip_id` | Chip Name + NPU Name | {ChipName}_{NPUName} | ChipName=Ascend310, NPUName=5678 → Ascend310_5678 |
| Ascend 950PR | `npu-smi info -t board -i id` | Chip Name + NPU Name | {ChipName}_{NPUName} | ChipName=Ascend950, NPUName=PR → Ascend950_PR |
| Ascend 950DT | `npu-smi info -t board -i id` | Chip Name + NPU Name | {ChipName}_{NPUName} | ChipName=Ascend950, NPUName=DT → Ascend950_DT |

## 命令参数说明

### 基础命令

```bash
npu-smi info
```

**适用产品**：Atlas A2 系列、Atlas 200I/500 A2、Atlas 推理系列、Atlas 训练系列

**使用方法**：
1. 在安装 AI 处理器的服务器上执行该命令
2. 在输出信息中找到 `Name` 字段
3. 配置值为 `Ascend{Name}`

**示例输出**：
```
NPU ID                         : 0
Chip Name                      : Ascend910B
...
Name                           : 910B
...
```

配置值为：`Ascend910B`

### 高级命令（带 board 参数）

```bash
npu-smi info -t board -i id -c chip_id
```

**适用产品**：Atlas A3 训练系列、Atlas A3 推理系列

**参数说明**：
- `-t board`：查询 board 类型信息
- `-i id`：设备 ID，通过 `npu-smi info -l` 命令查出的 NPU ID
- `-c chip_id`：芯片 ID，通过 `npu-smi info -m` 命令查出的 Chip ID

**使用方法**：
1. 先执行 `npu-smi info -l` 获取设备 ID（NPU ID）
2. 再执行 `npu-smi info -m` 获取芯片 ID（Chip ID）
3. 执行 `npu-smi info -t board -i <设备ID> -c <芯片ID>`
4. 在输出信息中找到 `Chip Name` 和 `NPU Name` 字段
5. 配置值为 `{ChipName}_{NPUName}`

**示例**：
```bash
# 步骤1：获取设备ID
npu-smi info -l
# 输出示例：NPU ID: 0

# 步骤2：获取芯片ID
npu-smi info -m
# 输出示例：Chip ID: 0

# 步骤3：查询 board 信息
npu-smi info -t board -i 0 -c 0
# 输出示例：
# Chip Name: Ascend910
# NPU Name: 1234
```

配置值为：`Ascend910_1234`

### 简化命令（不带 chip_id）

```bash
npu-smi info -t board -i id
```

**适用产品**：Ascend 950PR、Ascend 950DT

**参数说明**：
- `-t board`：查询 board 类型信息
- `-i id`：设备 ID，通过 `npu-smi info -l` 命令查出的 NPU ID

**使用方法**：
1. 先执行 `npu-smi info -l` 获取设备 ID（NPU ID）
2. 执行 `npu-smi info -t board -i <设备ID>`
3. 在输出信息中找到 `Chip Name` 和 `NPU Name` 字段
4. 配置值为 `{ChipName}_{NPUName}`

**示例**：
```bash
# 步骤1：获取设备ID
npu-smi info -l
# 输出示例：NPU ID: 0

# 步骤2：查询 board 信息
npu-smi info -t board -i 0
# 输出示例：
# Chip Name: Ascend950
# NPU Name: PR
```

配置值为：`Ascend950_PR`

## 使用场景

### 1. 创建算子工程

使用 msopgen 工具创建算子工程时，通过 `-c` 参数指定 soc_version：

```bash
msopgen gen -i add_custom.json -c ai_core-Ascend910B -lan cpp -out AddCustom
```

### 2. 配置编译选项

在 CMakePresets.json 中配置 ASCEND_COMPUTE_UNIT：

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

### 3. 注册算子原型

在算子原型定义中使用 AddConfig 注册 AI 处理器型号：

```cpp
this->AICore().AddConfig("Ascend910B");
```

### 4. 设置 Kernel 编译选项

在 op_kernel/CMakeLists.txt 中指定 COMPUTE_UNIT：

```cmake
npu_op_kernel_sources(ascendc_kernels
    OP_TYPE AddCustom
    KERNEL_DIR ./Add
    COMPUTE_UNIT Ascend910B
    KERNEL_FILE add_custom.cpp
)
```

## 注意事项

1. **大小写敏感**：soc_version 配置值区分大小写，必须严格按照格式填写
2. **产品系列匹配**：不同产品系列使用不同的查询命令，请勿混用
3. **多设备场景**：如果服务器安装了多个 AI 处理器，需要为每个设备分别查询 soc_version
4. **同系列通用性**：基于同系列 AI 处理器型号创建的算子工程，其基础功能（算子开发、编译和部署）通用

## 相关文档

- [快速入门](../快速入门.md) - 使用 msopgen 创建算子工程
- [编译组织](../编译与部署/基本流程.md#编译组织) - 手工组织算子工程
- [算子工程编译](../编译与部署/基本流程.md) - 算子工程编译配置
