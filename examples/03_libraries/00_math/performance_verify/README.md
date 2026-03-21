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

通过 `user_input.json` 配置测试场景和理论性能：

```json
{
  "exp": {
    "half": [4, 16],
    "float": [2, 8]
  },
  "theory": {
    "exp": {"half": 32, "float": 32},
    "mul": {"half": 128, "float": 64}
  }
}
```

- `数据长度单位`: K (elements)，如 `4` 表示 `4096` elements
- `theory`: 各API的理论并行度 (element/cycle)

## 快速开始

```bash
bash test.sh
```

## 输出示例

**测试进度**:
```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
▶ API: 1 | Type: half | Size: 4K (4096 elements)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
[Step 1/3] Build ...
[OK] Build completed
[Step 2/3] Generate test data ...
[OK] Data generated
[Step 3/3] Run kernel & Verify ...
```

**验证结果** (通过时抽样显示前10个):
```
[Sample] First 10 results:
idx      input_x         input_y         expected        actual         
--------------------------------------------------------------------
0        699.01          N/A             0.11            0.11           
1        0.00            N/A             65.38           65.38          
Test PASSED!
```

**性能汇总**:
```
========================================================================================================================
Performance Summary Report
========================================================================================================================
API          Type     Size(K)  Elements   TotalCycles  AvgCycles    Throughput   Theory     Achieve%   Status  
------------------------------------------------------------------------------------------------------------------------
exp          half     4        4096       4445         4.445        25.60        32         79.99      PASS    
========================================================================================================================
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
| T_total (50MHz) | systemCycleAfter - systemCycleBefore |
| T_avg (50MHz) | T_total / 1000 |
| T_ai (1800MHz) | T_avg / 50 × 1800 |
| Throughput | DataLength / T_ai |

## 文件结构

```
performance_verify/
├── performance_verify.asc      # 算子实现
├── test.sh                     # 测试入口
├── user_input.json             # 用户配置
├── CMakeLists.txt              # 编译配置
├── data_utils.h               # 数据工具
├── scripts/
│   ├── gen_data.py            # 数据生成入口
│   ├── gen_data_utils.py      # 公共配置和工具
│   ├── gen_data_*.py          # 各API数据生成
│   ├── verify_result.py       # 结果验证
│   └── calc_performance.py    # 性能计算
└── README.md
```
