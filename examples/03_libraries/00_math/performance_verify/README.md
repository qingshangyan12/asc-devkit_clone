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

通过 `user_input.json` 文件配置要测试的 API、数据类型和数据长度组合：

```json
{
  "exp": {
    "half": [4, 16],
    "float": [2, 8]
  }
}
```

**配置说明**：
- 数据长度单位：K (elements)，如 `4` 表示 `4 × 1024 = 4096` elements

## 快速开始

```bash
bash test.sh
```

## 输出结果

```
========================================================================================================================
Performance Summary Report
========================================================================================================================
API          Type     Size(K)  Elements   TotalCycles  AvgCycles    Throughput   Theory     Achieve%   Status  
------------------------------------------------------------------------------------------------------------------------
exp          half     4        4096       4445         4.445        25.60        32         79.99      PASS    
exp          half     16       16384      15111        15.111       30.12        32         94.12      PASS    
========================================================================================================================
```

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
│   ├── gen_data.py            # 数据生成
│   ├── gen_data_*.py          # 各API数据生成
│   ├── verify_result.py        # 结果验证
│   └── calc_performance.py     # 性能计算
└── README.md
```
