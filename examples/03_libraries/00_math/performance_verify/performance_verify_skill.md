# Ascend C API 性能验证 SKILL

## 核心理念

本 SKILL 的核心是基于用户指定扩展的 API 信息，自动化完成输入信息解析、代码适配、测试用例执行与结果展示。

## 工作流程

### 1. 解析提供的API信息
- 从 `new_api.md` 中获取要扩展的新 API 的基本信息：
  - API名称
  - 函数原型
  - API类型：单目Unary/双目Binary/双目标量BinaryScalar等
  - API支持的数据类型：half/float/int16/int32等
  - API功能说明
  - 理论性能

- 如果用户信息没提供完整，进行告警提示，直到用户提供完整，再执行下一步

- 基于提供的理论性能，更新配置文件`testcases.json`

  例如新扩展的API为NEWAPI，支持的数据类型为half/float，理论性能为{"half": 64, "float": 32}，对应更新配置文件如下：
  ```json
  {
    "test_apis": ["all"],
    "NEWAPI": {
      "half": [4, 16],
      "float": [4, 16]
    },
    "theory": {
      "NEWAPI": {"half": 64, "float": 32}
    }
  }
  ```

### 2. 自动代码适配

根据 `new_api.md` 中的信息，自动完成以下适配：

#### 2.1 添加 API ID

在 `performance_verify.asc` 中添加新的 API ID：

```cpp
#define API_ID_{new_api}  5
```

#### 2.2 添加 API TYPE

```cpp
#define API_TYPE_xxx  4  // 如果是单目Unary/双目Binary/双目标量BinaryScalar之外的其他类型API
```

#### 2.3 修改 CopyIn

```cpp
else if constexpr (INPUT_API_ID_VAL == API_ID_ADD) {
  ...
}
```

#### 2.4 修改 Compute

```cpp
else if constexpr (INPUT_API_ID_VAL == NEW_API) {
    ...
    
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleBegin = AscendC::GetSystemCycle();
    for (int32_t i = 0; i < REPEAT_TIMES_FOR_INSTR_EXECUTE; ++i) {
        AscendC::NewAPI(...)
    }
    AscendC::PipeBarrier<PIPE_ALL>();
    uint64_t cycleEnd = AscendC::GetSystemCycle();
    
    AscendC::PRINTF("  [LOG] Add instruction executed %d times, data length: %d, data type: %s, cost total cycles: %d\n",
        REPEAT_TIMES_FOR_INSTR_EXECUTE, TOTAL_LENGTH, DATATYPE_STR, (cycleEnd - cycleBegin));
    
    ...
}
```

#### 2.5 添加数据生成脚本

创建 `scripts/gen_data_{new_api}.py`：

```python
import numpy as np

def compute_{new_api}_golden():
  ...

def gen_{new_api}_data():
  ...
```

### 2.6 更新 `gen_data.py`

```python
from gen_data_new_api import gen_new_api_data, compute_new_api_golden

# 添加分支处理
elif api_type == API_TYPE_NEW_API:
    gen_{new_api}_data()
    compute_{new_api}_golden()
```

### 3. 测试用例执行与结果展示

- 确认testcases.json已正确添加新扩展的API信息，把"test_apis"改成只测试新扩展的API
  ```json
  {
    "test_apis": ["new_api_name"],
  }
  ```
- 执行以下命令进行验证和结果展示
  `bash test.sh`