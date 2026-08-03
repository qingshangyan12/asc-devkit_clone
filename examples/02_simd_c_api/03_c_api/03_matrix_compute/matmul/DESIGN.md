# 【社区任务】MatMul算子设计文档

## 1. 需求背景

目标是在 Ascend 950 上基于 Ascend C C API 开发纯 Cube-Core、单核、固定规格 MatMul：

$$
C_{i,j}=\sum_{k=0}^{39} A_{i,k}B_{k,j}+Bias_j
$$

规格为 A `[30,40]`、B `[40,70]`、Bias `[1,70]`、C `[30,70]`，所有数据类型均为 float，A/B 均不转置。

## 2. 总体方案

采用一个 `__global__ __cube__` 核函数完成完整流水：

```text
GM(ND) ──ND2NZ──> L1(Nz) ──Load2Dv2──> L0A(Nz) / L0B(Zn)
 Bias ──────────> L1 ─────────────────> BiasTable
 L0A × L0B + BiasTable ──asc_mmad──> L0C(Nz) ──Fixpipe NZ2ND──> GM(ND)
```

Host 侧只完成输入读取、Device 内存申请、H2D/D2H 拷贝和一次核函数启动，不执行任何逐元素运算。

## 3. 数据排布与对齐

FP32 的 C0 大小为 8 个元素（32B），Cube 的 M/N 基础分形维度为 16。FP32 转置搬运时，两个 `16×8` 分形沿列方向拼接为一个 `16×16` 方块，因此 B 的 N 轴需对齐到 16。

| Buffer | 逻辑数据 | 对齐后shape | 元素数 | 字节数 |
|---|---|---:|---:|---:|
| L1 A | [30,40] | [32,40] | 1280 | 5120 |
| L0A A | [30,40] | [32,40] | 1280 | 5120 |
| L1 B | [40,70] | [48,80] | 3840 | 15360 |
| L0B B | [40,70] | [48,80] | 3840 | 15360 |
| L1 Bias | [1,70] | 320B对齐区 | 80 | 320 |
| BiasTable | [1,70] | 320B搬运 | 80 | 320 |
| L0C C | [30,70] | [32,80] | 2560 | 10240 |

L1 同时占用 `5120 + 15360 + 320 = 20800B`；L0A、L0B、L0C 分别占用 5120B、15360B、10240B，均无缓冲区越界。

## 4. 关键API设计

### 4.1 GM到L1

- A：`n_value=30`、`d_value=40`、`C0 stride=32`；
- B：`n_value=40`、`d_value=70`、`C0 stride=48`；
- Bias：`n_value=1`、`d_value=70`。

使用 `asc_set_gm2l1_nz_para` 配置 NZ 目的步长，再调用 `asc_copy_gm2l1_nd2nz`。接口自动对最内层 D 轴补零。

### 4.2 L1到L0A

调用 `asc_copy_l12l0a`：

| 参数 | 值 |
|---|---:|
| `m_step` | 2 |
| `k_step` | 5 |
| `src_stride` | 2 |
| `dst_stride` | 2 |

### 4.3 L1到L0B

B 在 GM 中为非转置 `[K,N]`，L0B 需要 Zn 排布，因此调用 `asc_copy_l12l0b_transpose`：

| 参数 | 值 | 说明 |
|---|---:|---|
| `m_step` | 3 | `48 / 16` |
| `k_step` | 10 | `80 / 8`，满足 FP32 转置时必须为偶数 |
| `src_stride` | 3 | L1 K轴分形跨度 |
| `dst_stride` | 5 | L0B N轴分形跨度 `80 / 16` |

`asc_mmad` 的 K 参数仍传原始值 40，转置搬运产生的 K 轴尾部无效分形不会参与有效计算。

### 4.4 Bias

Ascend 950 上 b32 数据从 C1 搬到 C2 时，32B block 数需为偶数。70 个 float 占 280B，向上取 9 个 block 后再对齐为偶数，共搬运 10 个 block，即 320B。调用：

```cpp
asc_copy_l12bt(0, bias_l1, 320);
```

### 4.5 Cube计算

调用参数：

```cpp
asc_mmad(c_l0, a_l0, b_l0, 30, 40, 70, 0, true, true, false);
```

- `disable_gemv=true`：固定使用矩阵路径；
- `c_matrix_source=true`：C 初值来自 BiasTable；
- `c_matrix_init_val=false`：不将 C 清零；
- 不调用 `asc_enable_hf32`，使用 FP32 模式满足精度优先目标。

### 4.6 L0C到GM

`asc_copy_l0c2gm` 配置 `n_size=70`、`m_size=30`、`dst_stride=70`、`src_stride=32`，只搬出有效区域，丢弃对齐区结果。

## 5. 流水同步

| 数据 | 生产流水 | 消费流水 | Event |
|---|---|---|---|
| A L1 | MTE2 | MTE1 | 0 |
| B L1 | MTE2 | MTE1 | 1 |
| Bias L1 | MTE2 | MTE1 | 2 |
| A L0A | MTE1 | M | 0 |
| B L0B | MTE1 | M | 1 |
| BiasTable | MTE1 | M | 2 |
| C L0C | M | FIX | 0 |

核函数退出前执行 `asc_sync_pipe(PIPE_ALL)`。

## 6. 精度方案

CPU 侧使用 float64 完成参考矩阵乘和 Bias 加法，最终一次舍入为 float32。验证采用生态算子 FLOAT32 标准：

- 混合容差：`atol=2^-16`、`rtol=2^-10`；
- 逐元素匹配率不低于 0.99；
- 最大绝对误差不超过 `max(1e-2, 32 ULP)`。

测试数据覆盖均匀分布、正态分布、边界/抵消值、Bias-only 和全零数据，所有输入均在 `[-100,100]`。Bias-only 用例令 A/B 全零并使用非零 Bias，可直接验证 BiasTable 的逐列广播结果。

## 7. 异常与风险

1. `asc_mmad` 标准矩阵乘 C API 在 CANN 9.1.0 发布说明中标注为新增，任务书给出的 9.0.0～9.1.0 范围存在歧义；当前实现以 9.1.0 为基线。
2. 当前工作区具有 Ascend950PR，但只安装 CANN 9.0.0-beta.2。安装包头文件无法编译标准 C API；使用当前 asc-devkit master 头文件后，ASC 编译和五组实机精度测试均通过。正式合入前仍需在纯 CANN 9.1.0 环境复编，排除混合头文件/工具链的差异。
3. 本任务无性能门槛，设计不启用 HF32，也不引入多核和分块优化。

## 8. 交付物

- 算子与Host样例：`matmul.asc`；
- 构建文件：`CMakeLists.txt`；
- 数据生成：`scripts/gen_data.py`；
- 精度校验：`scripts/verify_result.py`；
- 批量执行：`scripts/run_cases.py`；
- 使用说明：`README.md` / `README_en.md`；
- 自测报告：`SELF_TEST_REPORT.md`；
- 易用性问题：[asc-devkit #1404](https://gitcode.com/cann/asc-devkit/issues/1404)。
