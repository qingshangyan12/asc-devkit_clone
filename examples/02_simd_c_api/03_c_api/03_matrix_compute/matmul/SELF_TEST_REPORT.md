# MatMul算子自测报告

## 1. 环境信息

| 项目 | 实际值 |
|---|---|
| 测试日期 | 2026-08-03 |
| 硬件 | Ascend950PR，NPU 0，Health OK |
| NPU架构 | dav-3510 |
| CANN版本 | 编译器/运行时 9.0.0-beta.2；C API头文件使用 asc-devkit master |
| 编译器 | Bisheng clang 15.0.5，构建时间标识 2026-03-21 |
| 驱动 / npu-smi | 25.7.rc1 |
| OS / Kernel | Ubuntu 24.04.3 LTS / 6.6.0-132.0.0.111.oe2403sp3.x86_64 |
| 编译模式 | NPU |
| 待验收代码 | 分支 `feat/asc-matmul-202608`；基于 asc-devkit master `fb822b36d87163e1f46121b269b419f93f1a5b28` |

## 2. 编译结果

安装包自带的 CANN 9.0.0-beta.2 头文件不声明 `asc_mmad`、`asc_copy_gm2l1_nd2nz`、`asc_copy_l12l0a`、`asc_copy_l12bt` 和 `asc_copy_l0c2gm`，直接编译失败。为验证实现，使用当前 asc-devkit 仓库头文件：

```bash
cmake -S examples/02_simd_c_api/03_c_api/03_matrix_compute/matmul \
      -B /tmp/asc_matmul_tailfix_20260803 \
      -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
      '-DCMAKE_ASC_FLAGS=-iquote /workspace/asc-devkit/include -iquote /workspace/asc-devkit'
cmake --build /tmp/asc_matmul_tailfix_20260803 -j2
```

| 检查项 | 结果 | 证据 |
|---|---|---|
| CMake配置 | 通过 | `Configuring done / Generating done` |
| ASC编译 | 通过 | `Building ASC object ... matmul.asc.o` |
| 链接 | 通过 | `Built target demo` |
| 可执行文件生成 | 通过 | `/tmp/asc_matmul_tailfix_20260803/demo` |
| 纯9.0安装包头文件编译 | 失败（预期的版本问题） | 多个标准C API报 `undeclared identifier` |

## 3. 精度测试

统一规格：A `[30,40]`、B `[40,70]`、Bias `[1,70]`、C `[30,70]`，dtype 为 float32，输入范围 `[-100,100]`。

判定标准：`rtol=2^-10`、`atol=2^-16`、`matched_ratio>=0.99`、`max_abs_error<=max(1e-2,32 ULP)`。

### 3.1 验证方法

在样例目录执行：

```bash
cd examples/02_simd_c_api/03_c_api/03_matrix_compute/matmul
./run.sh
```

正式 CANN 9.1.0 验收环境使用安装包头文件：

```bash
USE_REPO_HEADERS=0 ./run.sh
```

脚本自动完成 CMake 配置、ASC 编译、六组数据生成、NPU 执行、FP64 CPU Golden 生成和精度校验。任一步骤失败都会返回非零退出码；全部通过时终端末尾的汇总表显示 `Cases: 6/6 passed` 和 `Overall result: PASS`，并列出每个 case 的 matched ratio、最大误差和有效阈值。

每个 case 在 `output/cases/` 下保存输入清单、NPU 输出、Golden 和 JSON 精度报告。验收时需要同时检查：

1. `run.sh` 退出码为 0；
2. 六个 `*_report.json` 的 `passed` 均为 `true`；
3. `matched_ratio >= 0.99`；
4. `max_abs_error <= effective_max_abs_error_limit`；
5. 终端、NPU 状态和精度报告截图来自同一次纯 CANN 9.1.0 运行。

### 3.2 精度结果

| Case | 数据分布 | Seed | matched_ratio | max_abs_error | effective_limit | 结果 |
|---|---|---:|---:|---:|---:|---|
| uniform | 均匀分布 | 20260803 | 1.000000 | 0.01171875 | 0.25 | 通过 |
| normal | 正态分布并裁剪 | 20260804 | 1.000000 | 0.0048828125 | 0.0625 | 通过 |
| boundary | 边界与抵消值 | 20260805 | 1.000000 | 0.0005092621 | 0.01 | 通过 |
| bias_only | A/B全零、Bias非零 | 20260806 | 1.000000 | 0 | 0.01 | 通过 |
| non_aligned_tail | 命中最后有效 M/K/N 坐标 | 20260807 | 1.000000 | 0 | 0.01 | 通过 |
| zeros | 全零 | 20260808 | 1.000000 | 0 | 0.01 | 通过 |

逐 case JSON 报告由 `run.sh` 生成在 `output/cases/*_report.json`。校验脚本同时覆盖错误结果、长度错误、NaN 和 Inf 的失败路径；非有限输出会返回失败并生成严格合法的 JSON 报告。

### 3.3 验收截图状态

![Ascend950PR MatMul validation summary](./figures/validation_summary.png)

截图应来自实际 CANN 9.1.0 验收环境，并同时展示 Ascend950PR/NPU 健康状态及 `run.sh` 最终汇总表。不得用历史日志或人工生成图片替代。

## 4. 性能测试

任务书明确标注“性能要求：无”，因此本次不设置性能验收 case，性能数据记为不适用。本实现固定单核且不启用 HF32，以功能正确性和精度为目标。

## 5. 约束检查

| 约束 | 检查结果 |
|---|---|
| Cube-Core计算 | 代码静态检查通过：核函数为 `__cube__`，计算调用 `asc_mmad` |
| 单核 | Host启动参数固定为 `<<<1, 0, stream>>>` |
| 禁止Host逐元素计算 | Host无逐元素计算循环；golden仅存在于独立Python测试脚本 |
| 输入范围 | 生成脚本保证全部case位于 `[-100,100]` |
| 非对齐尾部 | M/L0C 对齐到 32，B 的 K/N 对齐到 48/80，Mmad 使用 `right_width=80`，Fixpipe 仅写出 `[30,70]`；专用尾部 case 误差为 0 |
| 内存安全 | L1、L0A、L0B、L0C 和 BiasTable 均按对齐后尺寸静态申请，覆盖搬运 API 可能读写的完整区域 |

## 6. 易用性反馈

CANN 9.0.0～9.1.0 任务版本范围与标准 `asc_mmad` 的发布版本存在歧义，已提交 [asc-devkit #1404](https://gitcode.com/cann/asc-devkit/issues/1404)。

## 7. 当前结论

ASC 编译（混合版本头文件方案）和 Ascend950PR 实机精度验证均通过，6/6 case 的 matched ratio 为 100%，Bias-only 与 non-aligned-tail 结果误差均为 0。功能实现与当前环境自验证已经完成，但正式验收尚未闭环：仍需在纯 CANN 9.1.0 环境复编、重新执行全部 case，并补充对应终端截图。不得把本次混合头文件测试描述成纯 9.1.0 测试。
