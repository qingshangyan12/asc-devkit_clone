# Saturation Verify 算子饱和度验证样例

## 概述

本样例用于验证 Ascend C 矢量计算 API 在处理特殊值（边界值）时的行为特性，包括：

- **非饱和模式（Non-Saturation）**：计算结果超出数据类型表示范围时，返回 INF 等特殊值
- **饱和模式（Saturation）**：计算结果超出数据类型表示范围时，返回数据类型最大值

样例支持单目 API（如 Exp）和双目 API（如 Mul）的特殊值验证，帮助开发者理解硬件行为与 NumPy 等标准库的行为差异。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构

```
saturation_verify
├── scripts
│   ├── gen_data.py         # 生成特殊值测试数据和真值（非饱和模式）
│   └── verify_result.py    # 验证算子输出与真值的差异
├── CMakeLists.txt          # 编译工程文件
├── data_utils.h            # 数据读写工具函数
├── saturation_verify.asc    # Ascend C 算子实现
├── test.sh                 # 测试执行脚本
└── README.md               # 本文档
```

## 参数说明

| 参数 | 说明 | 可选值/默认值 |
|------|------|--------------|
| INPUT_SIZE | 输入数据长度 | 默认值: 128 |
| INPUT_API_ID | 要验证的 API | 1: Exp, 2: Mul |
| INPUT_API_TYPE | API 类型 | 1: Unary（单目）, 2: Binary（双目） |

**注意**：
- 默认数据类型为 half（float16）
- 单目 API：INPUT_SIZE 应设置为特殊值个数（7个：min, max, +0, -0, NaN, +INF, -INF）
- 双目 API：INPUT_SIZE 应设置为特殊值个数的平方（49个：7×7组合）

## 执行样例

### 环境配置

请根据当前环境选择对应的 CANN 环境配置命令：

```bash
# root 用户默认安装
source /usr/local/Ascend/cann/set_env.sh

# 非 root 用户默认安装
source $HOME/Ascend/cann/set_env.sh

# 指定路径安装
source ${install_path}/cann/set_env.sh
```

### 执行命令

```bash
# 进入样例目录
cd saturation_verify

# Exp 算子验证（单目，INPUT_SIZE=7）
INPUT_API_ID=1 INPUT_API_TYPE=1 INPUT_SIZE=7 bash test.sh

# Mul 算子验证（双目，INPUT_SIZE=49）
INPUT_API_ID=2 INPUT_API_TYPE=2 INPUT_SIZE=49 bash test.sh
```

### 执行结果示例

#### Exp 单目 API 验证结果

**gen_data.py 生成的真值表格：**
```
====================================================================================================
API: exp | Data Type: float16
====================================================================================================
src/dst type | src                  | dst (Non-Sat)         
----------------------------------------------------------------------------------------------------
float16      | [min  ]-65500.0     | 0.0                   
float16      | [max  ]65500.0      | inf                   
float16      | [+0   ]0.0          | 1.0                   
float16      | [-0   ]-0.0         | 1.0                   
float16      | [NaN  ]nan          | nan                   
float16      | [+INF ]inf          | inf                   
float16      | [-INF ]-inf         | 0.0                   
====================================================================================================
```

**verify_result.py 验证结果表格：**
```
=========================================================================================================
API: exp | Verification Results
=========================================================================================================
src/dst type | src                  | dst (Non-Sat)          | dst (Saturation)      
---------------------------------------------------------------------------------------------------------
float16      | [min  ]-65500.0     | 0.0                    | 0.0                   
float16      | [max  ]65500.0      | inf                    | 65500.0               
float16      | [+0   ]0.0          | 1.0                    | 1.0                   
float16      | [-0   ]-0.0         | 1.0                    | 1.0                   
float16      | [NaN  ]nan          | nan                    | 0.0                   
float16      | [+INF ]inf          | inf                    | 65500.0               
float16      | [-INF ]-inf         | 0.0                    | 0.0                   
---------------------------------------------------------------------------------------------------------
Total: 7, Matched: 7, Error Ratio: 0.0000
=========================================================================================================

Test PASSED!
```

#### Mul 双目 API 验证结果

```
=============================================================================================================================
API: mul | Verification Results
=============================================================================================================================
src/dst type | src0            | src1            | dst (Non-Sat)          | dst (Saturation)      
-----------------------------------------------------------------------------------------------------------------------------
float16      | [min ]-65500.0 | [min ]-65500.0 | inf                    | inf                   
float16      | [min ]-65500.0 | [max ]65500.0  | -inf                   | -inf                  
float16      | [max ]65500.0  | [max ]65500.0  | inf                    | inf                   
...
float16      | [+INF]inf      | [+INF]inf      | inf                    | 65500.0               
float16      | [+INF]inf      | [-INF]-inf     | -inf                   | -65500.0              
float16      | [-INF]-inf     | [-INF]-inf     | 0.0                    | 0.0                   
-----------------------------------------------------------------------------------------------------------------------------
Total: 49, Matched: 49, Error Ratio: 0.0000
=============================================================================================================================

Test PASSED!
```

## 代码架构说明

### saturation_verify.asc

核心实现分为两个核函数：

1. **saturation_verify_kernel_sat** - 饱和模式
   - 设置 `AscendC::AscendCUtils::SetOverflow(0)`

2. **saturation_verify_kernel_non_sat** - 非饱和模式
   - 设置 `AscendC::AscendCUtils::SetOverflow(1)`

执行流程：
1. 先调用饱和模式kernel，输出到 output_sat.bin
2. 再调用非饱和模式kernel，输出到 output_non_sat.bin

### KernelSaturationVerify 类

核心类方法：
- `Init()` - 初始化 GlobalTensor 和 Queue
- `SetSatMode(isSaturation)` - 设置饱和/非饱和模式
- `Process(isSaturation)` - 执行完整的 CopyIn -> Compute -> CopyOut 流程

### 关键API说明

| 模式 | 设置 |
|------|------|
| 饱和模式 | SetOverflow(0) |
| 非饱和模式 | SetOverflow(1) |

## 新增 API 验证适配指南

本样例设计为可扩展的验证框架，支持新增不同的 API 进行特殊值验证。

### 需要修改的文件

#### 1. saturation_verify.asc

**添加新的 API ID 定义：**
```cpp
#define API_ID_EXP   1
#define API_ID_MUL   2
#define API_ID_NEW   3  // 新增 API ID
```

**在 Compute() 函数中添加新 API 的计算逻辑：**
```cpp
if constexpr (INPUT_API_ID_VAL == API_ID_EXP) {
    AscendC::Exp(dstLocal, srcLocal, INPUT_TENSOR_SIZE);
} else if constexpr (INPUT_API_ID_VAL == API_ID_NEW) {
    AscendC::NewApi(dstLocal, srcLocal, INPUT_TENSOR_SIZE);
}
```

#### 2. scripts/gen_data.py

**添加新 API 的真值计算函数：**
```python
def compute_newapi_golden(input_array):
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.newapi(input_array)  # 替换为实际的 API
    return result.astype(input_array.dtype)
```

**在主函数中添加分支：**
```python
if args.apiId == API_ID_EXP:
    gen_exp_data(args.inputSize)
elif args.apiId == API_ID_NEW:
    gen_newapi_data(args.inputSize)
```

#### 3. scripts/verify_result.py

根据需要添加对应的验证逻辑，通常不需要大幅修改。

#### 4. test.sh

设置对应的环境变量：
```bash
INPUT_API_ID=3 INPUT_API_TYPE=1 INPUT_SIZE=7 bash test.sh
```

### 新增 API 的完整示例流程

假设要新增一个名为 `Sqrt` 的单目 API：

1. **在 saturation_verify.asc 中添加：**
   - 添加 `#define API_ID_SQRT 3`
   - 在 `Compute()` 函数中添加 `AscendC::Sqrt` 调用

2. **在 scripts/gen_data.py 中：**
   - 添加 `compute_sqrt_golden` 函数
   - 在 `main` 中添加 `gen_sqrt_data` 分支

3. **执行验证：**
   ```bash
   INPUT_API_ID=3 INPUT_API_TYPE=1 INPUT_SIZE=7 bash test.sh
   ```
