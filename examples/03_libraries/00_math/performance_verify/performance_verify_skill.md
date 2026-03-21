# Ascend C API 性能验证 SKILL

## 核心理念

本 SKILL 的核心是基于用户指定的 API 信息，完成自动化代码适配、结果计算统计和展示。

## 功能特性

- **多 API 支持**：Exp、Mul、Adds、LeakyRelu
- **多数据类型**：half、float、int16、int32
- **灵活配置**：通过 `test_apis` 选择要测试的 API
- **自动化执行**：解析配置后批量运行测试
- **性能分析**：计算并行度、达标率，生成报告

## 工作流程

```
┌─────────────────────────────────────────────────────────────────┐
│                     1. 解析 testcases.json                      │
├─────────────────────────────────────────────────────────────────┤
│  • test_apis: ["all"] 或 ["exp", "mul"]                         │
│  • 各API的测试场景: {api: {dtype: [sizes]}}                     │
│  • 理论并行度: theory: {api: {dtype: parallelism}}              │
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                     2. 批量执行测试用例                           │
├─────────────────────────────────────────────────────────────────┤
│  for api in test_apis:                                          │
│    for dtype, sizes in api_configs:                              │
│      for size in sizes:                                          │
│        build → generate_data → run_kernel → verify_result        │
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                     3. 性能计算与汇总                             │
├─────────────────────────────────────────────────────────────────┤
│  • 解析日志中的 [LOG] 行                                         │
│  • 计算 Actual、Achieve%、Status                                │
│  • 生成 Performance Summary Report                               │
└─────────────────────────────────────────────────────────────────┘
                              ↓
┌─────────────────────────────────────────────────────────────────┐
│                     4. 输出结果                                  │
├─────────────────────────────────────────────────────────────────┤
│  • 终端打印汇总表格                                              │
│  • 生成 result.md 文件                                           │
└─────────────────────────────────────────────────────────────────┘
```

## 用户配置格式

### 完整配置示例

```json
{
  "_comment": {
    "test_apis": "指定要执行的API列表，支持配置多个",
    "exp": "Exp接口测试场景",
    "mul": "Mul接口测试场景"
  },
  "test_apis": ["all"],
  "exp": {
    "half": [4, 16, 48],
    "float": [2, 8, 24]
  },
  "mul": {
    "half": [4, 16, 32],
    "float": [4, 8, 16],
    "int16": [4, 16, 32],
    "int32": [4, 8, 16]
  },
  "adds": {
    "half": [4, 16, 32],
    "float": [4, 8, 16]
  },
  "leakyrelu": {
    "half": [4, 16, 32],
    "float": [4, 8, 16]
  },
  "theory": {
    "exp": {"half": 32, "float": 32},
    "mul": {"half": 128, "float": 64, "int16": 128, "int32": 64},
    "adds": {"half": 128, "float": 64, "int16": 128, "int32": 64},
    "leakyrelu": {"half": 128, "float": 64}
  }
}
```

### 配置项说明

| 配置项 | 类型 | 说明 |
|--------|------|------|
| `test_apis` | array | 要测试的API列表，`["all"]` 表示全部 |
| `{api_name}` | object | API名称（如exp/mul/adds/leakyrelu） |
| `{dtype}` | array | 数据类型，支持 half/float/int16/int32 |
| `theory` | object | 理论并行度，用于计算达标率 |

### 数据长度说明

- 单位：K (elements)
- 示例：`4` 表示 `4 × 1024 = 4096` elements

## 性能计算公式

| 指标 | 公式 | 说明 |
|------|------|------|
| TotalCycles | cycleEnd - cycleBegin | 1000次执行总周期数 |
| AvgCycles | TotalCycles / 1000 | 单次执行平均周期数 |
| Actual | DataLength / T_ai | 实测并行度 |
| T_ai | AvgCycles / 50 × 1800 | 换算到AI Core频率 |
| Achieve% | (Actual / Theory) × 100% | 达标率 |
| Status | Achieve ≥ 70% ? PASS : FAIL | 测试状态 |

### 计算示例

假设 Exp(half, 16K) 测试结果：
- TotalCycles = 15112
- AvgCycles = 15112 / 1000 = 15.112
- T_ai = 15.112 / 50 × 1800 = 544.032
- Actual = (16 × 1024) / 544.032 = 30.12
- Achieve% = (30.12 / 32) × 100% = 94.12%
- Status = PASS (≥70%)

## 日志格式

### 性能日志

```
[LOG] Exp instruction executed 1000 times, data length: 49152, data type: float16, cost total cycles: 43556
```

### 精度验证日志

```
[DataType] float16
✓ Precision Verification:  PASS
Matched: 49152 / 49152, Error Ratio: 0.000000
```

## 输出报告

### Performance Summary Report

```
============================================================================================================================
Performance Summary Report
============================================================================================================================
API          Type     Size(K)  Elements   TotalCycles  AvgCycles    Actual       Theory     Achieve%   Status
----------------------------------------------------------------------------------------------------------------------------
exp          half     48       49152      43556        43.556       31.35        32         97.96      PASS (Achieve≥70%)
exp          float    24       24576      22222        22.222       30.72        32         96.00      PASS (Achieve≥70%)
exp          float    8        8192       8001         8.001        28.44        32         88.88      PASS (Achieve≥70%)
mul          half     32       32768      14917        14.917       61.02        128        47.67      FAIL (Achieve<70%)
============================================================================================================================
```

## 扩展新 API

### 1. 修改 `performance_verify.asc`

```cpp
// 添加 API ID
#define API_ID_NEW_API  5

// 在 CopyIn 中添加数据搬入
if constexpr (INPUT_API_ID_VAL == API_ID_NEW_API) {
    AscendC::LocalTensor<T> xLocal = inQueueX.AllocTensor<T>();
    AscendC::LocalTensor<T> yLocal = inQueueY.AllocTensor<T>();
    AscendC::DataCopy(xLocal, xGlobal, TOTAL_LENGTH);
    AscendC::DataCopy(yLocal, yGlobal, TOTAL_LENGTH);
    inQueueX.EnQue(xLocal);
    inQueueY.EnQue(yLocal);
}

// 在 Compute 中添加计算逻辑
else if constexpr (INPUT_API_ID_VAL == API_ID_NEW_API) {
    // ...
    AscendC::PRINTF("[LOG] NewApi instruction executed ...\n");
}
```

### 2. 创建数据生成脚本

```python
# scripts/gen_data_new_api.py
def compute_new_api_golden():
    """计算 golden 数据"""
    pass

def gen_new_api_data():
    """生成测试数据"""
    pass
```

### 3. 更新 `gen_data.py`

```python
from gen_data_new_api import gen_new_api_data, compute_new_api_golden

# 添加分支处理
elif api_type == API_TYPE_NEW_API:
    gen_new_api_data(input_x, input_y, input_size, dtype_nbits)
    compute_new_api_golden(input_x, input_y, output, input_size, dtype_nbits)
```

### 4. 更新配置文件

```json
{
  "test_apis": ["all"],
  "new_api": {
    "half": [4, 16],
    "float": [4, 16]
  },
  "theory": {
    "new_api": {"half": 128, "float": 64}
  }
}
```

## 目录结构

```
performance_verify/
├── performance_verify.asc       # 算子实现
├── test.sh                      # 测试入口
├── CMakeLists.txt               # 编译配置
├── data_utils.h                # 数据工具
├── testcases.json             # 用户配置
├── result.md                   # 生成的报告
├── performance_results.log      # 执行日志
├── README.md                   # 使用说明
├── performance_verify_skill.md  # 本文档
└── scripts/
    ├── gen_data.py             # 数据生成入口
    ├── gen_data_utils.py       # 公共配置
    ├── gen_data_exp.py         # Exp 数据
    ├── gen_data_mul.py         # Mul 数据
    ├── gen_data_adds.py        # Adds 数据
    ├── gen_data_leakyrelu.py   # LeakyRelu 数据
    ├── verify_result.py        # 精度验证
    └── calc_performance.py     # 性能计算
```
