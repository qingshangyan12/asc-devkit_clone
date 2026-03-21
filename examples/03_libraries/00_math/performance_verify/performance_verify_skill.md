# Ascend C API 性能验证 SKILL

## 核心理念

本 SKILL 的核心是基于用户指定扩展的 API 信息，自动化完成输入信息解析、代码适配、测试用例执行与结果展示。

本 SKILL 根据[具体场景分类](#场景分类)，执行对应的工作流程

## 场景分类

根据 API 类型，将工作流程分为以下五类场景：

| 场景 | API 类型 | API TYPE 值 | 示例 |
|------|----------|-------------|------|
| 场景一 | 已支持的 API | - | Exp, Mul, Adds, LeakyRelu |
| 场景二 | 暂未支持的单目 API | API_TYPE_UNARY = 1 | Abs, Sqrt等 |
| 场景三 | 暂未支持的双目 API | API_TYPE_BINARY = 2 | Sub, Div, Min等 |
| 场景四 | 暂未支持的双目标量 API | API_TYPE_BINARY_SCALAR = 3 | Muls等 |
| 场景五 | 暂未支持的其他类型 API | 自定义新 TYPE 值 | 其他功能 API |

---

## 通用流程：解析 API 信息

无论哪种场景，都需要从 `new_api.md` 中获取以下信息：

```
1、API 名称（如 `Exp`, `Mul`, `Adds`）
2、函数原型（Ascend C API 签名）
3、是单目API？双目API？双目标量API？
4、支持的数据类型（half/float/int16/int32）
5、功能说明（数学含义）
6、理论性能（理论并行度，参考填写方式：{"half": 64, "float": 32}）
```

**信息完整性检查**：如果用户信息未提供完整，进行告警提示，直到用户提供完整再执行下一步。

---

## 场景一：已支持的 API

### 适用条件
- API 已在 `performance_verify.asc` 中实现
- 已在 `scripts/gen_data_*.py` 中添加数据生成脚本
- 已在 `testcases.json` 中配置

### 操作步骤

只需更新配置文件即可：

1. **更新 `testcases.json`**

```json
{
  "test_apis": ["api_name"],
  "api_name": {
    "half": [4, 16],
    "float": [4, 16]
  },
  "theory": {
    "api_name": {"half": 64, "float": 32}
  }
}
```

2. **执行测试**
```bash
bash test.sh
```

---

## 场景二：暂未支持的单目 API（Unary）

### 适用条件
- API 类型为单目（Unary）
- `API_TYPE_UNARY = 1`
- 函数签名：`dst = Op(src)`

### 操作步骤

#### 2.1 更新 `testcases.json`

```json
{
  "test_apis": ["new_api"],
  "new_api": {
    "half": [4, 16],
    "float": [4, 16]
  },
  "theory": {
    "new_api": {"half": 64, "float": 32}
  }
}
```

#### 2.2 添加 API ID

在 `performance_verify.asc` 中添加：

```cpp
#define API_ID_{NEW_API}  5  // 下一个可用 ID
```

#### 2.3 修改 CopyIn

CopyIn 方法只需一个输入队列（`inQueueX`）：

```cpp
else if constexpr (INPUT_API_ID_VAL == API_ID_{NEW_API}) {
    AscendC::LocalTensor<T> xLocal = inQueueX.AllocTensor<T>();
    AscendC::DataCopy(xLocal, xGlobal, TOTAL_LENGTH);
    inQueueX.EnQue(xLocal);
}
```

#### 2.4 修改 Compute

```cpp
else if constexpr (INPUT_API_ID_VAL == API_ID_{NEW_API}) {
    AscendC::LocalTensor<T> xLocal = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> zLocal = outQueue.AllocTensor<T>();
    
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleBegin = AscendC::GetSystemCycle();
    for (int32_t i = 0; i < REPEAT_TIMES_FOR_INSTR_EXECUTE; ++i) {
        AscendC::{NewAPI}(zLocal, xLocal, TOTAL_LENGTH);
    }
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleEnd = AscendC::GetSystemCycle();
    
    AscendC::PRINTF("  [LOG] {NewAPI} instruction executed %d times, data length: %d, data type: %s, cost total cycles: %d\n",
        REPEAT_TIMES_FOR_INSTR_EXECUTE, TOTAL_LENGTH, DATATYPE_STR, (cycleEnd - cycleBegin));
    
    outQueue.EnQue<T>(zLocal);
    inQueueX.FreeTensor(xLocal);
}
```

#### 2.5 修改 Kernel Launch

在 kernel 函数中，使用两个参数的 Init 和 Process：

```cpp
if constexpr (INPUT_API_TYPE_VAL == API_TYPE_UNARY) {
    if constexpr (INPUT_DATA_TYPE_VAL == 1) {
        KernelPerformanceVerify<half> op;
        op.Init(x, z, &pipe);  // x 输入，z 输出
        op.Process();
    }
    // ... 其他数据类型
}
```

#### 2.6 修改 main 函数

```cpp
if constexpr (INPUT_API_TYPE_VAL == API_TYPE_UNARY) {
    performance_verify_kernel<<<1, nullptr, stream>>>(xDevice, 0, zDevice);
}
```

#### 2.7 创建数据生成脚本

创建 `scripts/gen_data_{new_api}.py`：

```python
#!/usr/bin/env python3
import numpy as np
from gen_data_utils import DATA_TYPE_NP_MAP, save_bin

DATA_RANGE_CONFIG = {
    "half":  {"min": -10.0, "max": 10.0},
    "float": {"min": -10.0, "max": 10.0},
}

def compute_{new_api}_golden(a):
    """使用 NumPy 计算 golden 结果"""
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.{operation}(a)
    return result.astype(a.dtype)

def gen_{new_api}_data(input_type, input_size):
    data_type = DATA_TYPE_NP_MAP[input_type]
    dtype_name = {1: "half", 2: "float", 3: "int16", 4: "int32"}.get(input_type, "half")
    config = DATA_RANGE_CONFIG.get(dtype_name, {"min": -10.0, "max": 10.0})
    min_val, max_val = config["min"], config["max"]
    
    input_x = np.random.uniform(min_val, max_val, input_size).astype(data_type)
    golden = compute_{new_api}_golden(input_x)
    
    save_bin(input_x, f"./input/input_x.bin")
    save_bin(golden, f"./output/golden.bin")
```

#### 2.8 更新 `gen_data_utils.py`

添加 API ID 和名称映射：

```python
API_ID_{NEW_API} = 5  # 下一个可用 ID

API_NAME_MAP = {
    1: "exp",
    2: "mul",
    # ...
    5: "new_api",
}

THEORETICAL_PERFORMANCE = {
    # ...
    ("new_api", "half"): 64,
    ("new_api", "float"): 32,
}
```

#### 2.9 更新 `gen_data.py`

```python
from gen_data_{new_api} import gen_{new_api}_data

# 添加分支
elif args.apiId == API_ID_{NEW_API}:
    gen_{new_api}_data(args.inputType, args.inputSize)

# 更新 choices
parser.add_argument('-apiId', type=int, default=1, choices=[1, 2, 3, 4, 5])
```

---

## 场景三：暂未支持的双目 API（Binary）

### 适用条件
- API 类型为双目（Binary）
- `API_TYPE_BINARY = 2`
- 函数签名：`dst = Op(src0, src1)`

### 操作步骤

#### 3.1 ~ 3.2 同场景二

#### 3.3 修改 CopyIn

CopyIn 方法需要两个输入队列（`inQueueX` 和 `inQueueY`）：

```cpp
else if constexpr (INPUT_API_ID_VAL == API_ID_{NEW_API}) {
    AscendC::LocalTensor<T> xLocal = inQueueX.AllocTensor<T>();
    AscendC::LocalTensor<T> yLocal = inQueueY.AllocTensor<T>();
    AscendC::DataCopy(xLocal, xGlobal, TOTAL_LENGTH);
    AscendC::DataCopy(yLocal, yGlobal, TOTAL_LENGTH);
    inQueueX.EnQue(xLocal);
    inQueueY.EnQue(yLocal);
}
```

#### 3.4 修改 Compute

```cpp
else if constexpr (INPUT_API_ID_VAL == API_ID_{NEW_API}) {
    AscendC::LocalTensor<T> xLocal = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> yLocal = inQueueY.DeQue<T>();
    AscendC::LocalTensor<T> zLocal = outQueue.AllocTensor<T>();
    
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleBegin = AscendC::GetSystemCycle();
    for (int32_t i = 0; i < REPEAT_TIMES_FOR_INSTR_EXECUTE; ++i) {
        AscendC::{NewAPI}(zLocal, xLocal, yLocal, TOTAL_LENGTH);
    }
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleEnd = AscendC::GetSystemCycle();
    
    AscendC::PRINTF("  [LOG] {NewAPI} instruction executed %d times, data length: %d, data type: %s, cost total cycles: %d\n",
        REPEAT_TIMES_FOR_INSTR_EXECUTE, TOTAL_LENGTH, DATATYPE_STR, (cycleEnd - cycleBegin));
    
    outQueue.EnQue<T>(zLocal);
    inQueueX.FreeTensor(xLocal);
    inQueueY.FreeTensor(yLocal);
}
```

#### 3.5 修改 Kernel Launch

在 kernel 函数中，使用三个参数的 Init 和 Process：

```cpp
if constexpr (INPUT_API_TYPE_VAL == API_TYPE_BINARY) {
    if constexpr (INPUT_DATA_TYPE_VAL == 1) {
        KernelPerformanceVerify<half> op;
        op.Init(x, y, z, &pipe);  // x, y 输入，z 输出
        op.Process();
    }
}
```

#### 3.6 修改 main 函数

```cpp
uint8_t* yHost = nullptr;
uint8_t* yDevice = nullptr;
if constexpr (INPUT_API_TYPE_VAL == API_TYPE_BINARY) {
    aclrtMallocHost((void**)(&yHost), paramByteSize);
    aclrtMalloc((void**)&yDevice, paramByteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    ReadFile("./input/input_y.bin", paramByteSize, yHost, paramByteSize);
    aclrtMemcpy(yDevice, paramByteSize, yHost, paramByteSize, ACL_MEMCPY_HOST_TO_DEVICE);
}

// Kernel launch
performance_verify_kernel<<<1, nullptr, stream>>>(xDevice, yDevice, zDevice);
```

#### 3.7 创建数据生成脚本

创建 `scripts/gen_data_{new_api}.py`（双目版本）：

```python
def compute_{new_api}_golden(a, b):
    """使用 NumPy 计算 golden 结果"""
    with np.errstate(over='ignore', under='ignore', invalid='ignore'):
        result = np.{operation}(a, b)
    return result.astype(a.dtype)

def gen_{new_api}_data(input_type, input_size):
    # ... 生成两个输入
    input_x = np.random.uniform(min_val, max_val, input_size).astype(data_type)
    input_y = np.random.uniform(min_val, max_val, input_size).astype(data_type)
    golden = compute_{new_api}_golden(input_x, input_y)
    
    save_bin(input_x, f"./input/input_x.bin")
    save_bin(input_y, f"./input/input_y.bin")
    save_bin(golden, f"./output/golden.bin")
```

---

## 场景四：暂未支持的双目标量 API（BinaryScalar）

### 适用条件
- API 类型为双目标量（BinaryScalar）
- `API_TYPE_BINARY_SCALAR = 3`
- 函数签名：`dst = Op(src, scalar)`

### 操作步骤

#### 4.1 ~ 4.3 同场景三（CopyIn 需要两个输入）

#### 4.4 修改 Compute

需要从输入中获取标量值：

```cpp
else if constexpr (INPUT_API_ID_VAL == API_ID_{NEW_API}) {
    AscendC::LocalTensor<T> xLocal = inQueueX.DeQue<T>();
    AscendC::LocalTensor<T> yLocal = inQueueY.DeQue<T>();
    AscendC::LocalTensor<T> zLocal = outQueue.AllocTensor<T>();
    
    T scalar = yLocal.GetValue(0);
    
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleBegin = AscendC::GetSystemCycle();
    for (int32_t i = 0; i < REPEAT_TIMES_FOR_INSTR_EXECUTE; ++i) {
        AscendC::{NewAPI}(zLocal, xLocal, scalar, TOTAL_LENGTH);
    }
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleEnd = AscendC::GetSystemCycle();
    
    AscendC::PRINTF("  [LOG] {NewAPI} instruction executed %d times, data length: %d, data type: %s, cost total cycles: %d\n",
        REPEAT_TIMES_FOR_INSTR_EXECUTE, TOTAL_LENGTH, DATATYPE_STR, (cycleEnd - cycleBegin));
    
    outQueue.EnQue<T>(zLocal);
    inQueueX.FreeTensor(xLocal);
    inQueueY.FreeTensor(yLocal);
}
```

#### 4.5 ~ 4.7 同场景三

---

## 场景五：暂未支持的其他类型 API

### 适用条件
- API 类型不属于 Unary/Binary/BinaryScalar
- 需要定义新的 API TYPE 值
- 例如：三目运算、多输入、多输出等特殊 API

### 操作步骤

#### 5.1 添加新的 API TYPE

在 `performance_verify.asc` 中定义新的 API TYPE：

```cpp
#define API_TYPE_UNARY  1
#define API_TYPE_BINARY 2
#define API_TYPE_BINARY_SCALAR 3
#define API_TYPE_{NEW_TYPE}  4  // 新增类型
```

#### 5.2 修改 Kernel 类 Init 方法

添加新的初始化接口：

```cpp
__aicore__ inline void Init(GM_ADDR xGm, GM_ADDR yGm, GM_ADDR zGm, AscendC::TPipe* pipeIn)
{
    // 现有双目初始化
}

__aicore__ inline void Init(GM_ADDR xGm, GM_ADDR yGm, GM_ADDR zGm, GM_ADDR wGm, AscendC::TPipe* pipeIn)
{
    // 新增多输入初始化
    pipe = pipeIn;
    xGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(xGm), TOTAL_LENGTH);
    yGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(yGm), TOTAL_LENGTH);
    zGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(zGm), TOTAL_LENGTH);
    wGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(wGm), TOTAL_LENGTH);
    pipe->InitBuffer(inQueueX, 1, TOTAL_LENGTH * sizeof(T));
    pipe->InitBuffer(inQueueY, 1, TOTAL_LENGTH * sizeof(T));
    pipe->InitBuffer(inQueueZ, 1, TOTAL_LENGTH * sizeof(T));
    pipe->InitBuffer(outQueue, 1, TOTAL_LENGTH * sizeof(T));
}
```

#### 5.3 修改 CopyIn/Compute/CopyOut

根据新 API 的特点实现相应的数据搬移和计算逻辑。

#### 5.4 修改 Kernel Launch

```cpp
if constexpr (INPUT_API_TYPE_VAL == API_TYPE_{NEW_TYPE}) {
    if constexpr (INPUT_DATA_TYPE_VAL == 1) {
        KernelPerformanceVerify<half> op;
        op.Init(x, y, z, w, &pipe);  // 根据实际需要调整参数
        op.Process();
    }
}
```

#### 5.5 修改 main 函数

根据需要分配额外的内存和进行数据拷贝。

---

## 通用流程：测试执行与结果展示

### 1. 更新 testcases.json

确认配置正确，将 `test_apis` 设置为只测试新扩展的 API：

```json
{
  "test_apis": ["new_api_name"]
}
```

### 2. 执行测试

```bash
bash test.sh
```

### 3. 更新 test.sh（如需要）

如果添加了新的 API TYPE，确保在 `test.sh` 中更新映射：

```bash
declare -A API_MAP="([exp]='1:1' [mul]='2:2' [adds]='3:3' [leakyrelu]='4:3' [new_api]='5:2')"
```

### 4. 更新 verify_result.py（如需要）

如果添加了新的 API ID，确保更新参数验证：

```python
parser.add_argument('-api_id', type=int, default=1, choices=[1, 2, 3, 4, 5])
```

---

## 附录：文件修改清单

| 场景 | 修改文件 | 说明 |
|------|----------|------|
| 全部 | `new_api.md` | 获取新 API 信息 |
| 全部 | `testcases.json` | 添加配置 |
| 场景二~五 | `performance_verify.asc` | 添加 API ID、Compute 分支 |
| 场景三~五 | `performance_verify.asc` | 修改 CopyIn（双目） |
| 场景五 | `performance_verify.asc` | 添加新 API TYPE |
| 全部 | `scripts/gen_data_{api}.py` | 新建数据生成脚本 |
| 全部 | `scripts/gen_data_utils.py` | 添加 API ID 和性能 |
| 全部 | `scripts/gen_data.py` | 添加调用分支 |
| 如需要 | `scripts/verify_result.py` | 更新 API ID choices |
| 如需要 | `test.sh` | 更新 API MAP |
