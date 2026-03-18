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
将执行结果的表格数据呈现出来，以及执行命令，供用户复制。

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

---

## 补充说明：Step 3 代码适配详细示例

以用户新增 `Reciprocal` API 为例，说明手动修改代码的具体步骤。假设 `Reciprocal` 是单目 API，支持 half/float 类型。

### 3.1 修改 saturation_verify.asc

**步骤1：添加 API ID 定义**

在文件开头找到 `#define API_ID_EXP  1` 和 `#define API_ID_MUL  2`，添加：

```cpp
#define API_ID_EXP   1
#define API_ID_MUL   2
#define API_ID_RECIPROCAL   3    // 新增
```

**步骤2：修改 CopyIn() 函数**

在 CopyIn() 函数的 if-else 链中新增单目 API 的输入处理：

```cpp
__aicore__ inline void CopyIn()
{
    if constexpr (INPUT_API_ID_VAL == API_ID_EXP) {
        AscendC::LocalTensor<T> srcLocal = inQueueSrc.AllocTensor<T>();
        AscendC::DataCopy(srcLocal, srcGlobal, INPUT_TENSOR_SIZE);
        inQueueSrc.EnQue(srcLocal);
    } else if constexpr (INPUT_API_ID_VAL == API_ID_MUL) {
        // 双目 API 有两个输入
        AscendC::LocalTensor<T> src1Local = inQueueSrc1.AllocTensor<T>();
        AscendC::LocalTensor<T> src2Local = inQueueSrc2.AllocTensor<T>();
        AscendC::DataCopy(src1Local, src1Global, INPUT_TENSOR_SIZE);
        AscendC::DataCopy(src2Local, src2Global, INPUT_TENSOR_SIZE);
        inQueueSrc1.EnQue(src1Local);
        inQueueSrc2.EnQue(src2Local);
    } else if constexpr (INPUT_API_ID_VAL == API_ID_RECIPROCAL) {   // 新增
        AscendC::LocalTensor<T> srcLocal = inQueueSrc.AllocTensor<T>();
        AscendC::DataCopy(srcLocal, srcGlobal, INPUT_TENSOR_SIZE);
        inQueueSrc.EnQue(srcLocal);
    }
}
```

**步骤3：修改 Compute() 函数**

在 Compute() 函数的 if-else 链中新增 API 调用：

```cpp
__aicore__ inline void Compute()
{
    if constexpr (INPUT_API_ID_VAL == API_ID_EXP) {
        AscendC::LocalTensor<T> srcLocal = inQueueSrc.DeQue<T>();
        AscendC::LocalTensor<T> dstLocal = outQueueDst.AllocTensor<T>();
        AscendC::Exp(dstLocal, srcLocal, INPUT_TENSOR_SIZE);
        outQueueDst.EnQue<T>(dstLocal);
        inQueueSrc.FreeTensor(srcLocal);
    } else if constexpr (INPUT_API_ID_VAL == API_ID_MUL) {
        AscendC::LocalTensor<T> src1Local = inQueueSrc1.DeQue<T>();
        AscendC::LocalTensor<T> src2Local = inQueueSrc2.DeQue<T>();
        AscendC::LocalTensor<T> dstLocal = outQueueDst.AllocTensor<T>();
        AscendC::Mul(dstLocal, src1Local, src2Local, INPUT_TENSOR_SIZE);
        outQueueDst.EnQue<T>(dstLocal);
        inQueueSrc1.FreeTensor(src1Local);
        inQueueSrc2.FreeTensor(src2Local);
    } else if constexpr (INPUT_API_ID_VAL == API_ID_RECIPROCAL) {   // 新增
        AscendC::LocalTensor<T> srcLocal = inQueueSrc.DeQue<T>();
        AscendC::LocalTensor<T> dstLocal = outQueueDst.AllocTensor<T>();
        AscendC::Reciprocal(dstLocal, srcLocal, INPUT_TENSOR_SIZE);  // 调用 API
        outQueueDst.EnQue<T>(dstLocal);
        inQueueSrc.FreeTensor(srcLocal);
    }
}
```

### 3.2 创建 scripts/gen_data_reciprocal.py

新建文件，参考现有的 gen_data_exp.py：

```python
#!/usr/bin python3
import numpy as np
from gen_data_utils import DATA_TYPE_MAP, construct_special_values, save_bin, get_special_names, print_unary_api_table

def compute_reciprocal_golden(input_array):
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.where(input_array == 0, np.inf, np.reciprocal(input_array))
    return result.astype(input_array.dtype)

def gen_reciprocal_data(input_type, input_size):
    data_type = DATA_TYPE_MAP[input_type]
    type_name = np.dtype(data_type).name
    
    special_vals = construct_special_values(data_type)
    inputs = np.array(special_vals, dtype=data_type)
    golden = compute_reciprocal_golden(inputs)
    
    save_bin(inputs, f"./input/input_x.bin")
    save_bin(golden, f"./output/golden.bin")
    
    special_names = get_special_names(data_type)
    print_unary_api_table("reciprocal", type_name, special_names, inputs, golden)
```

### 3.3 修改 scripts/gen_data.py

**步骤1：添加导入**

```python
from gen_data_exp import compute_exp_golden, gen_exp_data
from gen_data_mul import compute_mul_golden, gen_mul_data
from gen_data_reciprocal import compute_reciprocal_golden, gen_reciprocal_data  # 新增
```

**步骤2：添加 API ID 和名称映射**

```python
API_ID_EXP = 1
API_ID_MUL = 2
API_ID_RECIPROCAL = 3  # 新增

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "reciprocal",  # 新增
}
```

**步骤3：修改 argparse 的 choices**

```python
parser.add_argument('-apiId', type=int, default=1, choices=[1, 2, 3],
                    help="API ID: 1=exp, 2=mul, 3=reciprocal")
```

**步骤4：添加分支逻辑**

```python
if args.apiId == API_ID_EXP:
    gen_exp_data(input_type, args.inputSize)
elif args.apiId == API_ID_MUL:
    gen_mul_data(input_type, args.inputSize)
elif args.apiId == API_ID_RECIPROCAL:  # 新增
    gen_reciprocal_data(input_type, args.inputSize)
```

### 3.4 修改 scripts/verify_result.py

**步骤1：添加 API ID 和名称映射**

```python
API_ID_EXP = 1
API_ID_MUL = 2
API_ID_RECIPROCAL = 3  # 新增

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    3: "reciprocal",  # 新增
}
```

**步骤2：修改 verify_result 函数中的调用**

```python
if api_id == API_ID_EXP:
    res = verify_single_api(output_type, input_x, "exp")
elif api_id == API_ID_RECIPROCAL:  # 新增
    res = verify_single_api(output_type, input_x, "reciprocal")
elif api_id == API_ID_MUL:
    res = verify_binary_api(output_type, input_x, input_y, api_name)
```

**步骤3：修改 argparse 的 choices**

```python
parser.add_argument('-api_id', type=int, default=1, choices=[1, 2, 3],
                    help="API ID: 1=exp, 2=mul, 3=reciprocal")
```

### 3.5 修改 test.sh

**步骤1：修改 case 映射**

```bash
case $INPUT_API_ID in
    1|3) EXPECTED_TYPE=1 ;;   # 单目 API: exp(1), reciprocal(3)
    2) EXPECTED_TYPE=2 ;;     # 双目 API: mul(2)
    *) echo "Error: Unknown API_ID=$INPUT_API_ID"; exit 1 ;;
esac
```

**步骤2：更新 echo 提示**

```bash
echo "INPUT_API_ID:   $INPUT_API_ID (1: exp, 2: mul, 3: reciprocal)"
```

### 3.6 执行验证

```bash
# 单目 API
INPUT_API_ID=3 INPUT_API_TYPE=1 bash test.sh
```

执行完成后会输出饱和/非饱和模式对比表格。

---

## 附录：执行结果示例

### 单目 API（如 Exp, Ln, Abs, Reciprocal）输出示例

```
====================================================================================================================
API: exp | Data Type: float16 | Verification Results
====================================================================================================================
type     | src                        | dst (API Output, Non-Sat) | dst (API Output, Saturation)
--------------------------------------------------------------------------------------------------------------------
float16  | min   -65504.000000        | 0.000000                 | 0.000000
float16  | max   65504.000000         | inf                      | 65504.000000
float16  | +0    0.000000             | 1.000000                 | 1.000000
float16  | -0    -0.000000            | 1.000000                 | 1.000000
float16  | NaN   nan                  | nan                      | nan
float16  | +INF  inf                  | inf                      | 65504.000000
float16  | -INF  -inf                 | 0.000000                 | 0.000000
--------------------------------------------------------------------------------------------------------------------
Total: 7, Matched: 7, Error Ratio: 0.0000
====================================================================================================================

Test PASSED!
```

### 双目 API（如 Mul）输出示例

```
============================================================================================================================
API: mul | Data Type: float16 | Verification Results
============================================================================================================================
type     | src0                       | src1                       | dst (API Output, Non-Sat) | dst (API Output, Saturation)
----------------------------------------------------------------------------------------------------------------------------
float16  | min   -65504.000000        | min   -65504.000000        | inf                      | 65504.000000
float16  | min   -65504.000000        | max   65504.000000         | -inf                     | -65504.000000
float16  | min   -65504.000000        | +0    0.000000            | -0.000000                | -0.000000
float16  | min   -65504.000000        | -0    -0.000000           | 0.000000                 | 0.000000
float16  | min   -65504.000000        | NaN   nan                  | nan                      | nan
float16  | min   -65504.000000        | +INF  inf                  | -inf                     | -65504.000000
float16  | min   -65504.000000        | -INF  -inf                 | inf                      | 65504.000000
...
----------------------------------------------------------------------------------------------------------------------------
Total: 49, Unique: 49, Matched: 49, Error Ratio: 0.0000
============================================================================================================================

Test PASSED!
```

### 表格说明

| 列名 | 说明 |
|------|------|
| `type` | 数据类型（float16/float32/int16/int32） |
| `src` / `src0, src1` | 输入值（单目API一个输入，双目API两个输入） |
| `dst (API Output, Non-Sat)` | 非饱和模式下的API输出结果 |
| `dst (API Output, Saturation)` | 饱和模式下的API输出结果 |
| `Total` | 测试用例总数 |
| `Matched` | 非饱和模式下的API输出结果，与Golden结果匹配的用例数 |
| `Error Ratio` | 错误率（Match/Total） |


对比`dst (API Output, Non-Sat)`和`dst (API Output, Saturation)`两列数据可以观察 API 在两种模式下的行为差异。
