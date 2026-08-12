# MatMul自测报告

## 测试对象

| 项目 | 值 |
| --- | --- |
| 计算公式 | `C[30,70] = A[30,40] @ B[40,70] + bias[1,70]` |
| 数据类型 | FP32 |
| A转置 | false |
| B转置 | false |
| Block Num | 1 |
| NPU架构 | `dav-3510` |

## 测试环境

| 项目 | 值 |
| --- | --- |
| NPU | Ascend 950PR，Health=OK |
| CANN | 9.1.0 |
| Python | 3.12.13 |
| NumPy | 2.5.1 |
| CMake | 3.22.1 |
| 测试日期 | 2026年8月9日 |
| 真机验证代码提交 | `9a7b971a4dff96605ffd9fea156fbd2cbc64cceb` |

**图 1**  测试环境与NPU状态
![](../figures/environment.png)

## 自测用例

| Case | 输入构造 | 重复次数 | 验证目标 |
| --- | --- | ---: | --- |
| P01 | A、B全零，bias非零 | 10 | bias广播 |
| P02 | A、B、bias全零 | 10 | 全零边界 |
| P03 | `-100/100/0`周期边界 | 10 | 输入范围边界 |
| P04 | 正负成对消减 | 10 | 消减误差 |
| P05 | B稀疏基向量 | 10 | 稀疏布局 |
| P06 | seed `20260804`随机输入 | 10 | 随机精度 |
| P07 | seed `20260805`随机输入 | 10 | 随机精度 |
| P08 | seed `20260806`随机输入 | 10 | 随机精度与性能 |

精度判定参数如下：

| 参数 | 值 |
| --- | ---: |
| rtol | `2^-10` |
| atol | `2^-16` |
| matched ratio | `>= 0.99` |
| 最大绝对误差保护 | `<= 1e-2`或最大绝对误差位置ULP `<= 32` |

## 合同测试与构建结果

| 验证项 | 结果 |
| --- | --- |
| Python合同测试 | PASS，`Ran 21 tests`，`OK`，退出码0 |
| Ascend 950真机构建 | PASS，`Built target demo`，退出码0 |

**图 2**  合同测试结果
![](../figures/contract_tests.png)

**图 3**  Ascend 950真机构建结果
![](../figures/build_pass.png)

## 精度结果

| Case | matched ratio | max abs error | max ULP | max-abs ULP | 结果 |
| --- | ---: | ---: | ---: | ---: | --- |
| P01 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P02 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P03 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P04 | 1.000000 | 2.288818359e-03 | 1952 | 300 | PASS |
| P05 | 1.000000 | 3.576278687e-05 | 1 | 1 | PASS |
| P06 | 1.000000 | 1.559900981e-02 | 442 | 4 | PASS |
| P07 | 1.000000 | 1.549896409e-02 | 1201 | 4 | PASS |
| P08 | 1.000000 | 1.425332281e-02 | 335 | 4 | PASS |

P01-P08各重复10次，共80次，结果为`summary=80/80 passed`。P08最终一轮的`matched_ratio=1.000000`、`max_abs_error=1.425332281e-02`、`max_ulp_error=335`、`max_abs_error_ulp=4`。

**图 4**  P01-P08精度测试80次全部通过
![](../figures/precision_80_of_80.png)

## 内存与同步检查结果

| 工具 | 结果 |
| --- | --- |
| `mssanitizer memcheck` | PASS，`No error detected`，退出码0 |
| `mssanitizer synccheck` | PASS，`No error detected`，退出码0 |

**图 5**  memcheck检查结果
![](../figures/memcheck.png)

**图 6**  synccheck检查结果
![](../figures/synccheck.png)

## 性能结果

性能用例为P08，Block Num为1。结果如下：

| 指标 | 值 |
| --- | ---: |
| Kernel | `_Z13matmul_customPfS_S_S_` |
| Task Duration | 6.375 us |
| AI Core Time | 5.61 us |
| AI Core MAC Time | 0.307 us |
| MTE2 Time | 1.63 us |
| Cube Utilization | 3.030% |

**图 7**  P08性能数据
![](../figures/performance.png)

## 自测结论

MatMul算子在Ascend 950PR和CANN 9.1.0环境下完成合同测试、真机构建、80次精度测试、内存检查、同步检查和性能采集。上述测试均通过，精度结果满足设定标准。
