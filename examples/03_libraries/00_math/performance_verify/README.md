# Ascend C API 性能验证测试工具

本工具用于验证 Ascend C 数学 API 在不同数据类型和不同输入数据长度组合下的性能表现。

## 支持的 API

| API_ID | API 名称 | 类型 | 支持的数据类型 |
|--------|----------|------|---------------|
| 1 | Exp | 单目 (Unary) | half, float |
| 2 | Mul | 双目 (Binary) | half, float, int16, int32 |
| 3 | Adds | 双目标量 (BinaryScalar) | half, float, int16, int32 |
| 4 | LeakyRelu | 双目标量 (BinaryScalar) | half, float |

## 用户配置

通过 `testcases.json` 配置测试场景和理论性能：

```json
{
  "_comment": {
    "test_apis": "指定要执行的API列表，如: [\"exp\", \"mul\"]；配置为 [\"all\"] 则执行所有API",
    "exp": "Exp接口测试场景, 格式为数据类型: [数据长度1, 数据长度2, ..]",
    "mul": "Mul接口测试场景, 格式为数据类型: [数据长度1, 数据长度2, ..]"
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

### 配置说明

| 配置项 | 说明 |
|--------|------|
| `test_apis` | 指定要执行的API列表，如 `["all"]`、`["exp", "mul"]`、`["adds"]` |
| `{api_name}` | 各API的测试场景，格式为 `{"数据类型": [数据长度1, 数据长度2, ...]}` |
| `数据长度单位` | K (elements)，如 `4` 表示 `4096` elements |
| `theory` | 各API的理论并行度 (element/cycle)，用于计算达标率 |

## 快速开始

```bash
cd performance_verify
bash test.sh
```

## 输出示例

**测试进度**:
```
╔══════════════════════════════════════════════════════════════════════════════════╗
║              Ascend C Performance Verification - Auto Test Runner                ║
╚══════════════════════════════════════════════════════════════════════════════════╝

┌─────────────────────────────────────────────────────────────────────────────────┐
│ Case #1                                                                        │
│ API: exp | DataType: half | DataLength: 48K (49152 elements)                    │
└─────────────────────────────────────────────────────────────────────────────────┘
  ↳ Building...
  ✓ Build OK
  ↳ Generating data...
  ✓ Data OK
  ↳ Running kernel...
  ✓ Performance Verification
[LOG] Exp instruction executed 1000 times, data length: 49152, data type: float16, cost total cycles: 43556

  [DataType] float16
  ✓ Precision Verification:  PASS
  Matched: 49152 / 49152, Error Ratio: 0.000000
```

**性能汇总**:
```
============================================================================================================================
Performance Summary Report
============================================================================================================================
API          Type     Size(K)  Elements   TotalCycles  AvgCycles    Actual       Theory     Achieve%   Status
----------------------------------------------------------------------------------------------------------------------------
exp          half     48       49152      43556        43.556       31.35        32         97.96      PASS (Achieve≥70%)
exp          float    24       24576      22222        22.222       30.72        32         96.00      PASS (Achieve≥70%)
============================================================================================================================
```

## 数据范围配置

各 API 的 `gen_data_*.py` 支持独立配置数据范围，避免出现 `0` 或 `inf`：

| API | 数据类型 | 默认范围 |
|-----|----------|----------|
| Exp | half/float | [-10, 10] |
| Mul | half/float | [-10, 10] |
| Mul | int16/int32 | [-100, 100] |
| Adds/LeakyRelu | half/float | [-100, 100] |
| Adds/LeakyRelu | int16/int32 | [-1000, 1000] |

## 性能计算公式

| 指标 | 公式 |
|------|------|
| TotalCycles | systemCycleAfter - systemCycleBefore |
| AvgCycles | TotalCycles / 1000 |
| Actual (并行度) | DataLength / (AvgCycles / 50 × 1800) |
| Achieve% | (Actual / Theory) × 100% |
| Status | Achieve ≥ 70% → PASS，否则 FAIL |

## 目录结构

```
performance_verify/
├── performance_verify.asc      # 算子实现（支持 Exp/Mul/Adds/LeakyRelu）
├── test.sh                     # 测试入口（自动解析配置、批量执行）
├── CMakeLists.txt              # 编译配置
├── data_utils.h               # 数据读写工具
├── testcases.json             # 用户配置（API列表、测试场景、理论性能）
├── result.md                   # 生成的性能报告（Markdown格式）
├── performance_results.log      # 测试执行日志
├── README.md                   # 本文档
├── performance_verify_skill.md # SKILL文档
└── scripts/
    ├── gen_data.py             # 数据生成入口
    ├── gen_data_utils.py       # 公共配置和工具函数
    ├── gen_data_exp.py         # Exp 数据生成
    ├── gen_data_mul.py         # Mul 数据生成
    ├── gen_data_adds.py         # Adds 数据生成
    ├── gen_data_leakyrelu.py   # LeakyRelu 数据生成
    ├── verify_result.py        # 精度验证
    └── calc_performance.py      # 性能计算与报告生成
```

## 扩展新 API

如需添加新的 API（如 `Sub`），请参考以下步骤：

### 1. 修改 `performance_verify.asc`

- 添加 API ID 定义
- 在 `CopyIn` 中添加数据搬入处理
- 在 `Compute` 中添加计算逻辑

### 2. 添加数据生成脚本

- 创建 `scripts/gen_data_sub.py`
- 在 `scripts/gen_data.py` 中添加导入和分支处理
- 在 `scripts/verify_result.py` 中添加结果验证逻辑

### 3. 更新配置文件

- 在 `testcases.json` 中添加 API 配置和理论性能
