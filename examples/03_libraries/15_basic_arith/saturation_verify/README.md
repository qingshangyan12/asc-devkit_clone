# Saturation Verify 算子饱和度验证样例

## 概述

本样例用于验证 Ascend C 矢量计算 API 在处理特殊值（边界值）时的行为特性，包括：

- **非饱和模式（Non-Saturation）**：计算结果超出数据类型表示范围时，返回饱和值（如 INF）
- **饱和模式（Saturation）**：计算结果超出数据类型表示范围时，返回数据类型最大值

样例支持单目 API（如 Exp）和双目 API（如 Mul）的特殊值验证，帮助开发者理解硬件行为与 NumPy 等标准库的行为差异。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构

```
saturation_verify
├── scripts
│   ├── gen_data.py         # 生成特殊值测试数据和真值
│   └── verify_result.py    # 验证算子输出与真值的差异
├── CMakeLists.txt          # 编译工程文件
├── data_utils.h            # 数据读写工具函数
├── saturation_verify.asc   # Ascend C 算子实现
├── test.sh                 # 测试执行脚本
└── README.md               # 本文档
```

## 参数说明

| 参数 | 说明 | 可选值 |
|------|------|--------|
| INPUT_TYPE | 输入数据类型 | 1: half, 2: float, 3: int16, 4: int32 |
| INPUT_SIZE | 输入数据长度 | 任意正整数 |
| INPUT_API_ID | 要验证的 API | 1: Exp（单目）, 2: Mul（双目） |

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

# Exp 算子验证（float16）
INPUT_TYPE=1 INPUT_SIZE=224 INPUT_API_ID=1 bash test.sh

# Exp 算子验证（float32）
INPUT_TYPE=2 INPUT_SIZE=224 INPUT_API_ID=1 bash test.sh

# Mul 算子验证（float16，7x7=49 个组合）
INPUT_TYPE=1 INPUT_SIZE=49 INPUT_API_ID=2 bash test.sh

# Mul 算子验证（float32，7x7=49 个组合）
INPUT_TYPE=2 INPUT_SIZE=49 INPUT_API_ID=2 bash test.sh
```

### 执行结果示例

#### Exp 单目 API 验证结果

```
================================================================================
API: exp | Data Type: float16 | Single Input Special Values
================================================================================
Input Value               | API Output (Non-Sat)     
------------------------------------------------------------
[min  ] -65500.0             | 0.0                      
[max  ] 65500.0              | inf                      
[+0   ] 0.0                  | 1.0                      
[-0   ] -0.0                 | 1.0                      
[NaN  ] nan                  | nan                      
[+INF ] inf                  | inf                      
[-INF ] -inf                 | 0.0                      
================================================================================

================================================================================
Single API (exp) Verification Results - Non-Saturation Mode
================================================================================
Input                | Golden               | Actual               | Match 
---------------------------------------------------------------------------
[min  ] -65500.0     | 0.0                | 0.0                | YES
[max  ] 65500.0      | inf                | inf                | YES
[+0   ] 0.0          | 1.0                | 1.0                | YES
[-0   ] -0.0         | 1.0                | 1.0                | YES
[NaN  ] nan          | nan                | nan                | YES
[+INF ] inf          | inf                | inf                | YES
[-INF ] -inf         | 0.0                | 0.0                | YES
---------------------------------------------------------------------------
Total: 7, Matched: 7, Error Ratio: 0.0000
================================================================================

Test PASSED!
```

#### Mul 双目 API 验证结果（部分）

```
================================================================================
Binary API (mul) Verification Results - Non-Saturation Mode
================================================================================
Input A         | Input B         | Golden               | Actual               | Match 
-----------------------------------------------------------------------------------------------
[min ] -3.4028235e+38 | [min ] -3.4028235e+38 | inf                | inf                | YES
[min ] -3.4028235e+38 | [max ] 3.4028235e+38 | -inf               | -inf               | YES
...
[+INF] inf      | [+INF] inf      | inf                | inf                | YES
[+INF] inf      | [-INF] -inf     | -inf               | -inf               | YES
[-INF] -inf     | [-INF] -inf     | inf                | 0.0                | NO
-----------------------------------------------------------------------------------------------
Total: 49, Matched: 48, Error Ratio: 0.0204
================================================================================
```

**说明**：
- Mul 算子在处理 INF × INF、(-INF) × (-INF) 等组合时，Ascend C 硬件行为与 NumPy 存在差异
- 差异主要集中在无穷大值的乘法组合，这是硬件实现的特性，并非算子 bug

## 新增 API 验证适配指南

本样例设计为可扩展的验证框架，支持新增不同的 API 进行特殊值验证。

### 需要修改的文件

#### 1. saturation_verify.asc

**添加新的 API ID 定义：**
```cpp
#define API_ID_EXP  1
#define API_ID_MUL  2
#define API_ID_NEW  3  // 新增 API ID
```

**在 ComputeSingle 或 ComputeBinary 函数中添加新 API 的计算逻辑：**

```cpp
if constexpr (INPUT_API == API_ID_EXP) {
    AscendC::Exp(dstLocal, srcLocal, INPUT_TENSOR_SIZE);
} else if constexpr (INPUT_API == API_ID_NEW) {  // 新增 API
    AscendC::NewApi(dstLocal, srcLocal, INPUT_TENSOR_SIZE);
}
```

**如果新增的是双目 API，需要：**
- 修改 `InitBinary` 函数支持三个输入参数
- 修改 `ProcessBinary` 函数
- 修改 `ComputeBinary` 函数添加新 API 调用
- 修改 `main` 函数中的内存分配和拷贝逻辑

#### 2. scripts/gen_data.py

**添加新 API 的真值计算函数：**

```python
def compute_newapi_golden(input_array):
    """计算新 API 的真值"""
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.newapi(input_array)  # 替换为实际的 API
    return result.astype(input_array.dtype)
```

**在主函数中添加分支：**

```python
def main():
    # ... existing code ...
    if args.apiId == API_ID_EXP:
        gen_exp_data(args.inputType, args.inputSize)
    elif args.apiId == API_ID_MUL:
        gen_mul_data(args.inputType, args.inputSize)
    elif args.apiId == API_ID_NEW:  # 新增 API
        gen_newapi_data(args.inputType, args.inputSize)
```

#### 3. scripts/verify_result.py

**如果新增的 API 有特殊的验证逻辑，修改验证函数：**

```python
def verify_result(output_type, output, golden, api_id, input_x=None, input_y=None):
    if api_id == API_ID_EXP:
        # ... existing code ...
    elif api_id == API_ID_MUL:
        # ... existing code ...
    elif api_id == API_ID_NEW:  # 新增 API
        # 添加新 API 的验证逻辑
```

#### 4. CMakeLists.txt

通常不需要修改，除非新增的 API 需要额外的编译选项。

#### 5. test.sh

通常不需要修改，脚本已经支持通过环境变量指定不同的 API。

### 新增 API 的完整示例流程

假设要新增一个名为 `Sqrt` 的单目 API：

1. **在 saturation_verify.asc 中添加：**
   - 添加 `#define API_ID_SQRT 3`
   - 在 `ComputeSingle` 函数中添加 `AscendC::Sqrt` 调用

2. **在 scripts/gen_data.py 中：**
   - 添加 `compute_sqrt_golden` 函数
   - 在 `main` 中添加 `gen_sqrt_data` 分支

3. **执行验证：**
   ```bash
   INPUT_TYPE=1 INPUT_SIZE=7 INPUT_API_ID=3 bash test.sh
   ```

### INPUT_SIZE 参数说明

- **单目 API（如 Exp、Sqrt）**：输入为 N 个特殊值，INPUT_SIZE = N
  - 浮点类型：N = 7（min, max, +0, -0, NaN, +INF, -INF）
  - 整数类型：N = 4（min, max, +0, -0）

- **双目 API（如 Mul）**：输入为 M × M 个组合，INPUT_SIZE = M × M
  - 浮点类型：M = 7，INPUT_SIZE = 49
  - 整数类型：M = 4，INPUT_SIZE = 16
