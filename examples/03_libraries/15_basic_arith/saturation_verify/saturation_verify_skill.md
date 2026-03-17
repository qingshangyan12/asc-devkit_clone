# Saturation Verify Skill

## 概述

本技能用于自动完成 saturation_verify 样例的代码适配，根据用户提供的 API 信息自动生成测试代码并执行验证。

技能会自动：
1. 解析user_input.md用户提供的 API 信息，如果没有提供完整，会发出告警
2. 自动适配代码（修改 .asc 文件和 Python 脚本）
3. 执行饱和/非饱和模式验证
4. 输出对比结果表格

---

## Step 1：提问用户是否完成user_input.md的信息输入填写，YES or NO
如果YES继续往下执行，如果NO提示填写完成后再继续

## Step 2: 收集 API 信息

解析user_input.md用户提供的 API 信息（所有用户填写:后面的内容），打印如下结果信息

| 信息 | 说明 | 示例 |
|------|------|------|
| **API 名称** | 函数名称 | `Exp`, `Sqrt`, `Mul` |
| **函数原型** | Ascend C API 签名 | `AscendC::Exp(dst, src, count)` |
| **单目API或双目API** | API的输入参数是一个还是两个 | 单目如`AscendC::API(dst, src, count)` 或 双目如`AscendC::API(dst, src0, src1, count)` |
| **支持的数据类型** | half/float/int16/int32 | `half, float` |
| **功能说明** | 数学含义 | 例如Exp是计算指数 `dst_i = e ^ src_i` |
---

## Step 3: 代码自动适配

根据用户提供的 API 信息，技能会自动完成以下修改：

### 2.1 修改 saturation_verify.asc

```cpp
// 1. 添加 API ID 定义
#define API_ID_{API_NAME}  3

// 2. 在 Compute() 函数中添加 API 调用
// 如果是单目
if constexpr (INPUT_API_ID_VAL == API_ID_{API_NAME}) {
    AscendC::{API_NAME}(dstLocal, srcLocal, INPUT_TENSOR_SIZE);
}
// 如果是双目
if constexpr (INPUT_API_ID_VAL == API_ID_{API_NAME}) {
    AscendC::{API_NAME}(dstLocal, src0Local, src1Local, INPUT_TENSOR_SIZE);
}
```

### 2.2 创建 scripts/gen_data_{api_name}.py

```python
# scripts/gen_data_{api_name}.py
import numpy as np
from gen_data_utils import DATA_TYPE_MAP, construct_special_values, save_bin, get_special_names, print_unary_api_table

def compute_{api_name}_golden(input_array):
    ...

def gen_{api_name}_data(input_type, input_size):
    ...
```

### 2.3 修改 gen_data.py 主入口

添加导入和分支：
```python
from gen_data_{api_name} import compute_{api_name}_golden, gen_{api_name}_data

if args.apiId == API_ID_{api_name}:
    gen_{api_name}_data(input_type, args.inputSize)
```
---

## Step 4: 执行验证

### 执行命令

```bash
# 单目 API
INPUT_API_ID=xxx INPUT_API_TYPE=1 bash test.sh

# 双目 API  
INPUT_API_ID=xxx INPUT_API_TYPE=2 bash test.sh
```

## 文件结构

```
saturation_verify/
├── scripts/
│   ├── gen_data.py           # 主入口
│   ├── gen_data_utils.py      # 公共函数
│   ├── gen_data_exp.py       # Exp 专用
│   ├── gen_data_mul.py       # Mul 专用
│   └── verify_result.py       # 验证脚本
├── saturation_verify.asc      # 算子实现
├── CMakeLists.txt            # 编译配置
├── test.sh                   # 测试脚本
└── README.md                 # 使用文档
```
