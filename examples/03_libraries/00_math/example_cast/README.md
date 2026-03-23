# Cast算子功能样例

## 概述

本样例基于Cast接口，实现类型转换功能。本样例展示了Cast接口在不同数据类型转换场景下的使用方法。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── example_cast
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── example_cast.asc        // Ascend C算子实现 & 调用样例
```

## 算子描述

### 算子功能

Cast算子实现类型转换操作：output = cast(input)

对应的数学表达式为：
```
output[i] = cast(input[i])
```

### 算子规格

| 配置参数名 | 取值 | 说明 |
|-----------|------|------|
| SCENARIO_NUM | 1 | float转half |
| SCENARIO_NUM | 2 | half转float |
| SCENARIO_NUM | 3 | float转bfloat16 |
| SCENARIO_NUM | 4 | bfloat16转float |
| SCENARIO_NUM | 5 | float转int32 |
| SCENARIO_NUM | 6 | int32转float |

算子整体规格如下表所示：

| 算子类型(OpType) | Cast |
|------------------|-----------|
| **算子输入** | |
| name | input |
| shape | [SIZE] |
| data type | float/half/bfloat16/int32 |
| format | ND |
| **算子输出** | |
| name | output |
| shape | [SIZE] |
| data type | half/float/bfloat16/int32 |
| format | ND |
| **核函数名** | kernel_cast |

### 算子实现

#### Kernel实现

计算逻辑：Ascend C提供的Cast接口的操作元素为LocalTensor，输入数据需要先搬运进片上存储，然后使用Cast接口完成类型转换，得到最终结果，再搬出到外部存储上。

Cast算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。

- **CopyIn任务**：负责将Global Memory上的输入Tensor input搬运至Local Memory，存储在inputLocal
- **Compute任务**：负责对inputLocal执行类型转换操作，计算结果存储在outputLocal中
- **CopyOut任务**：负责将输出数据从outputLocal搬运至Global Memory上的输出Tensor output

#### 调用实现

使用内核调用符`<<<>>>`调用核函数。

## 测试场景

本样例支持以下6种测试场景：

| scenarioNum | 场景描述 | 输入数据类型 | 输出数据类型 | 特殊配置 |
|------------|----------|------------|------------|----------|
| 1 | float转half | float | half | 无 |
| 2 | half转float | half | float | 无 |
| 3 | float转bfloat16 | float | bfloat16 | 无 |
| 4 | bfloat16转float | bfloat16 | float | 无 |
| 5 | float转int32 | float | int32 | 无 |
| 6 | int32转float | int32 | float | 无 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

### 配置环境变量

请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

- 默认路径，root用户安装CANN软件包
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 默认路径，非root用户安装CANN软件包
  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- 指定路径install_path，安装CANN软件包
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### 样例执行

```bash
# 场景1执行示例（float转half）
SCENARIO=1 SIZE=1024
mkdir -p build && cd build;      # 创建并进入build目录
cmake .. -DSCENARIO_NUM=$SCENARIO -DSIZE=$SIZE;make -j;    # 编译工程
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO -size=$SIZE  # 生成测试输入数据
./demo                           # 执行编译生成的可执行程序，执行样例
python3 ../scripts/verify_result.py output/output.bin output/golden.bin -scenarioNum=$SCENARIO   # 验证输出结果是否正确，确认算法逻辑正确
```

执行结果如下，说明精度对比成功。
```bash
test pass!
```

### 参数说明

- `SCENARIO`：测试场景编号，取值范围1-6
- `SIZE`：向量长度，默认值为1024

## API使用说明

### API概述

Cast接口用于类型转换操作

### API原型

```cpp
template <typename SrcT, typename DstT>
__aicore__ inline void Cast(
    const LocalTensor<DstT>& dstLocal,
    const LocalTensor<SrcT>& srcLocal,
    uint32_t dataSize
);
```

### API参数说明

| 参数名 | 类型 | 输入/输出 | 说明 |
|--------|------|-----------|------|
| dstLocal | LocalTensor<DstT> | 输出 | 目标操作数，用于存放转换结果 |
| srcLocal | LocalTensor<SrcT> | 输入 | 源操作数 |
| dataSize | uint32_t | 输入 | 数据长度 |

### 使用示例

```cpp
// 基本用法
AscendC::Cast(outputLocal, inputLocal, size);
```

### 注意事项

- 输入和输出数据长度必须一致
- 支持多种数据类型转换组合
- 数据长度需要对齐到32字节

## 实现要点

### 数据对齐要求

Cast接口对输入数据的对齐要求如下：

| 数据类型 | 对齐要求 |
|----------|----------|
| half/fp16 | 32字节对齐（16个元素） |
| float/fp32 | 32字节对齐（8个元素） |
| bfloat16 | 32字节对齐（16个元素） |
| int32_t | 32字节对齐（8个元素） |

### 性能优化建议

- 确保数据访问连续，避免跨bank访问
- 合理使用流水线，提高计算效率
- 对于大数据量，考虑分块处理

### 常见问题

**Q1:** 数据长度不是对齐长度怎么办？
**A:** 在计算时使用对齐后的长度，多余部分单独处理

**Q2:** 如何处理精度损失？
**A:** 类型转换时可能会有精度损失，用户需要根据业务需求选择合适的数据类型

## 扩展阅读

- [Cast接口文档](https://www.hiascend.com/document)
- [类型转换样例](https://www.hiascend.com/example)
- [Ascend C编程指南](https://www.hiascend.com/guide)
