# Saturation Verify Skill

## 概述

本技能用于自动完成 saturation_verify 样例的代码适配，根据用户提供的 API 信息自动生成测试代码并执行验证。

## 输入要求

用户需要提供以下信息：
1. **API 函数原型**：Ascend C API 的函数签名
2. **支持的数据类型**：如 half, float, int16, int32 等
3. **功能说明**：API 的数学含义

## 处理流程

### 1. 解析 API 信息

根据用户提供的函数原型，提取：
- API 名称（如 Exp, Mul, Sqrt 等）
- 输入参数数量（单目/双目）
- 数据类型

### 2. 代码适配

自动修改以下文件：

#### saturation_verify.asc

1. 添加 API ID 定义
2. 在 Compute() 函数中添加 API 调用

#### scripts/gen_data.py

1. 添加真值计算函数
2. 在 main() 中添加分支

#### scripts/verify_result.py

通常无需修改

### 3. 执行验证

运行 test.sh 并输出结果表格

## 使用示例

### 示例 1：新增 Exp API

**用户输入：**
```
API: Exp
函数原型: AscendC::Exp(dst, src, count)
支持类型: half, float
功能: 计算指数 e^x
```

**自动执行流程：**

1. 添加 `#define API_ID_EXP 1`
2. 在 Compute() 中添加 `AscendC::Exp(dstLocal, srcLocal, INPUT_TENSOR_SIZE);`
3. 添加 `compute_exp_golden()` 函数
4. 执行验证

**输出结果：**
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

### 示例 2：新增 Mul API

**用户输入：**
```
API: Mul
函数原型: AscendC::Mul(dst, src0, src1, count)
支持类型: half, float, int16, int32
功能: 元素级乘法
```

**输出结果：**
```
=============================================================================================================================
API: mul | Verification Results
=============================================================================================================================
src/dst type | src0            | src1            | dst (Non-Sat)          | dst (Saturation)      
-----------------------------------------------------------------------------------------------------------------------------
float16      | [min ]-65500.0 | [min ]-65500.0 | inf                    | inf                   
float16      | [min ]-65500.0 | [max ]65500.0  | -inf                   | -inf                  
...
Total: 49, Matched: 49, Error Ratio: 0.0000
=============================================================================================================================

Test PASSED!
```

## 实现代码

### 自动适配脚本逻辑

```python
def adapt_api(api_name, api_proto, supported_types, description):
    """
    自动适配新的 API 到 saturation_verify 样例
    
    参数:
        api_name: API 名称 (如 "Exp", "Mul", "Sqrt")
        api_proto: 函数原型 (如 "AscendC::Exp(dst, src, count)")
        supported_types: 支持的数据类型列表 (如 ["half", "float"])
        description: 功能描述
    """
    
    # 1. 确定 API 类型（单目/双目）
    api_type = detect_api_type(api_proto)  # 1: Unary, 2: Binary
    
    # 2. 分配 API ID
    api_id = allocate_api_id(api_name)
    
    # 3. 修改 saturation_verify.asc
    modify_asc_file(api_name, api_proto, api_id)
    
    # 4. 修改 gen_data.py
    modify_gen_data(api_name, api_proto, api_id, supported_types)
    
    # 5. 执行测试
    run_test(api_id, api_type)
```

### 关键函数说明

| 函数 | 说明 |
|------|------|
| detect_api_type() | 根据函数原型判断单目/双目 |
| allocate_api_id() | 分配新的 API ID |
| modify_asc_file() | 修改 .asc 文件添加 API 调用 |
| modify_gen_data() | 修改 gen_data.py 添加真值计算 |
| run_test() | 执行测试并输出结果 |

## 注意事项

1. **INPUT_SIZE 设置**：
   - 单目 API：INPUT_SIZE = 7（7个特殊值）
   - 双目 API：INPUT_SIZE = 49（7×7组合）

2. **数据类型**：当前仅支持 half（float16）

3. **API 支持**：
   - 单目：Exp, Sqrt, Ln, Abs, Reciprocal 等
   - 双目：Mul, Add, Sub, Div, Max, Min 等
