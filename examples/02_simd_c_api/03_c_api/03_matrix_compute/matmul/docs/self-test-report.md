# MatMul 自测报告

## 1. 测试对象

```text
C[30,70] = A[30,40] @ B[40,70] + bias[70]
dtype = FP32
transposeA = false
transposeB = false
numBlocks = 1
NPU arch = dav-3510
```

## 2. 结论

| 验证层 | 结果 | 证据 |
| --- | --- | --- |
| Ascend 950 Python/源码合同测试 | PASS | `Ran 21 tests ... OK` |
| CANN 9.1.0 仿真构建与运行 | PASS | clean build，P01-P08 `summary=8/8 passed` |
| CANN 9.1.0 Ascend 950 构建 | PASS | clean `dav-3510` NPU build |
| Ascend 950 精度与重复性 | PASS | P01-P08 每例 10 次，`summary=80/80 passed` |
| mssanitizer memcheck | PASS | 退出码 0，无越界/泄漏报告 |
| mssanitizer synccheck | PASS | `No error detected` |
| mssanitizer initcheck | TOOL LIMITATION | 仅报告 NZ 对齐填充区读取；官方 `mmad` 样例同样报告 |
| msprof P08 | PASS（需修复环境变量） | 修复 Python ABI 搜索路径后成功导出；单 block task duration `6.156 us` |

算子满足任务书的单核纯 Cube-Core、FP32、带 bias 和无 Host 逐元素计算约束。CANN 9.0.0-beta.2 不具备本算子所需的普通 Cube C API，因此当前受支持验证基线收窄为 CANN 9.1.0；该兼容性问题已记录在 Issue #1404。

## 3. 测试环境

### Ascend 950

- 设备环境：Ascend 950PR validation host，Linux x86_64
- NPU：`Ascend950PR_9579`，Health=OK
- NPU Arch：`dav-3510`
- CANN：`9.1.0`
- Python：`3.x`，已安装 `requirements.txt`
- 项目目录：当前样例目录

设备软件检查：

```bash
source /path/to/Ascend/cann/set_env.sh
python3 -c "import acl; print(acl.get_soc_name())"
# Ascend950PR_9579
```

## 4. 自动化测试覆盖

| 类别 | 覆盖内容 |
| --- | --- |
| 数据生成 | P01-P08、固定 Shape、dtype、准确文件长度、固定 seed、范围 `[-100,100]` |
| 语义用例 | bias 广播、全零、边界值、正负消减、B 稀疏布局、3 组随机输入 |
| 验证器 | exact pass、局部大误差、错误长度、NaN/Inf、matched ratio、abs/ULP |
| Runner | 多 case、多次重复、生成/执行/验证错误传播、旧输出清理 |
| Device 合同 | `__cube__`、单 block、必需 C API、无 HF32、固定 Shape |
| 布局合同 | B L1/L0B=3,200、`dn2nz`、`asc_copy_l12l0b`、Mmad N=70 |
| 内存安全 | L1/L0A/L0B/L0C/BT 容量编译期断言 |
| Host 合同 | 精确文件大小、无 Host 元素级计算循环、ACL 清理错误报告 |

## 5. 精度标准

Golden 使用 NumPy FP64 计算：

```python
golden = src0.astype(np.float64) @ src1.astype(np.float64) + weight.astype(np.float64)
```

验证器使用生态标准对应的混合判定：

```text
rtol = 2^-10
atol = 2^-16
matched_ratio >= 0.99
max_abs_error <= 1e-2 or ULP error at the max-absolute-error element <= 32
```

`max_ulp_error` 仍作为诊断指标输出；由于接近零的元素 ULP 间距很小，它可以高于 32，而不改变上面的标准判定。所有真机用例的 `matched_ratio=1.0`，最大绝对误差对应元素的 ULP 误差不超过 4。

## 6. 精度用例结果

以下是 Ascend 950 每例 10 次中第 1 次的指标；同一用例的 10 次结果完全一致。

| Case | 构造 | matched | max abs error | max ULP | max-abs ULP | 结果 |
| --- | --- | ---: | ---: | ---: | ---: | --- |
| P01 | A/B 全零，非零 bias | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P02 | A/B/bias 全零 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P03 | `-100/100/0` 周期边界 | 1.000000 | 0.000000000e+00 | 0 | 0 | PASS |
| P04 | 正负成对消减 | 1.000000 | 2.288818359e-03 | 1952 | 300 | PASS |
| P05 | B 稀疏基向量 | 1.000000 | 3.576278687e-05 | 1 | 1 | PASS |
| P06 | seed `20260804` 随机 | 1.000000 | 1.559900981e-02 | 442 | 4 | PASS |
| P07 | seed `20260805` 随机 | 1.000000 | 1.549896409e-02 | 1201 | 4 | PASS |
| P08 | seed `20260806` 随机 | 1.000000 | 1.425332281e-02 | 335 | 4 | PASS |

P01-P08 真机完整日志：由验收归档保存，结果为 `summary=80/80 passed`。

## 7. 构建与运行证据

### 仿真

使用 CANN 9.1.0 从空目录构建：

```bash
cmake -S . -B build-sim-dn2nz \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DCMAKE_ASC_RUN_MODE=sim
cmake --build build-sim-dn2nz -j
python3 scripts/run_cases.py --binary ./build-sim-dn2nz/demo --repeat 1
```

日志：仿真运行日志，结果 `summary=8/8 passed`。

### Ascend 950 真机

```bash
cmake -S . -B build-npu-dn2nz \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DCMAKE_ASC_RUN_MODE=npu
cmake --build build-npu-dn2nz -j
python3 scripts/run_cases.py --binary ./build-npu-dn2nz/demo --repeat 10
```

配置日志：CANN 9.1.0 `dav-3510` NPU 配置日志。

构建日志：CANN 9.1.0 `dav-3510` NPU 构建日志。

运行日志：CANN 9.1.0 Ascend 950 P01-P08 重复运行日志，结果 `summary=80/80 passed`。

## 8. 内存与同步检查

```text
mssanitizer memcheck: exit 0, no out-of-bounds or leak report
mssanitizer synccheck: No error detected
```

对应日志：

- mssanitizer memcheck 日志（验收归档）
- mssanitizer synccheck 日志（验收归档）

`initcheck` 在本算子中报告 L1 地址 `0x0`、`0x1400`、`0x4600` 的 320、6400、32 字节读取。这些地址对应 A、B、bias 的 NZ 对齐/填充区域，而不是逻辑 GM 输入范围。逻辑输出在 P01-P08 和 80 次重复运行中稳定正确。为确认工具行为，CANN 9.1.0 官方 `mmad` 样例在同一设备上运行 `initcheck` 也报告 L1 对齐区域未初始化读取；因此该项记录为工具对 C API NZ padding 的限制，不把它误报为无条件全绿。

官方样例对照日志和本项目 initcheck 日志均由验收归档保存。

## 9. 性能工具状态

任务书没有性能门槛。已在 Ascend 950 上执行一次 P08 profile。首次执行时，CANN 9.1.0 自带导出器使用系统 Python，动态链接器将 `_ctypes` 绑定到系统 `libpython3.12`，而该库没有扩展所需的 `_PyErr_SetLocaleString` 符号，错误为：

```text
ImportError: _ctypes... undefined symbol: _PyErr_SetLocaleString
```

原始 profile 采集成功，只有导出阶段失败。将与扩展匹配的 Python 3.12 库目录置于 `LD_LIBRARY_PATH` 首位，并显式传入对应解释器后，导出成功：

```bash
source /path/to/Ascend/cann/set_env.sh
cd examples/02_simd_c_api/03_c_api/03_matrix_compute/matmul
msprof \
  --output=./profiling-p08 \
  --task-time=on --runtime-api=on --task-memory=on \
  --aic-metrics=PipeUtilization ./build-npu-dn2nz/demo

export LD_LIBRARY_PATH=/opt/buildtools/python-3.12.9/lib:$LD_LIBRARY_PATH
msprof --output=./profiling-p08 \
  --python-path=/opt/buildtools/python-3.12.9/bin/python3.12 \
  --export=on --summary-format=csv
```

导出的 `op_summary_*.csv` 指标如下：

| 指标 | 值 |
| --- | ---: |
| Kernel | `_Z13matmul_customPfS_S_S_` |
| Block Num | 1 |
| Task duration | 6.156 us |
| AI core time | 5.43 us |
| AI core MAC time | 0.308 us |
| MTE2 time | 1.708 us |
| Cube utilization | 3.045% |

原始 profile 和导出 CSV 位于 `./profiling-p08/`；上述数值来自同一次 P08 采集。

## 10. CANN 9.0.0-beta.2 兼容性证据

CANN 9.0.0-beta.2 的 Arch 3510 自带头文件没有声明本实现所需的普通 Cube API：

```text
asc_set_gm2l1_nz_para
asc_copy_gm2l1_nd2nz
asc_copy_l12l0a
asc_copy_l12l0b
asc_copy_l12bt
asc_mmad
asc_copy_l0c2gm
```

使用 `asc-devkit/master` 头文件配合 9.0 编译器只能作为诊断：P01/P02 可运行，但非零 P03 失败。因此 README 和本报告不宣称 CANN 9.0.0-beta.2 支持该算子；Issue：

- [asc-devkit #1404：MatMul 任务书与 C API 发布版本范围不一致](https://gitcode.com/cann/asc-devkit/issues/1404)
- [asc-devkit #1408：3510 `asc_copy_l12l0b_trans` 公共声明与实现签名不一致](https://gitcode.com/cann/asc-devkit/issues/1408)

## 11. 复现与交付状态

本报告对应本样例的验证提交。提交上游 PR 前应再次在服务器执行合同测试、干净构建和 `git diff --check`，并在 PR 描述中填写实际提交 SHA 和 CI/真机验证链接。
