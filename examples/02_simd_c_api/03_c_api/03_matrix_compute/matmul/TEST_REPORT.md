# MatMul C API 自测报告

## 结论

MatMul C API 样例在 Ascend 950 实机上完成从空构建目录编译及三组精度测试。`random`、`ones` 和 `zero_bias` 的匹配率均为 100%，满足 FLOAT32 生态算子开源精度标准。

## 测试环境

| 项目 | 配置 |
| --- | --- |
| 测试日期 | 2026-08-03 |
| 硬件 | Ascend 950 |
| 架构 | `dav-3510` |
| CANN | 9.1.0 |
| Driver | 25.7.rc1 |
| Kernel | 单 Cube Core，FP32 `asc_mmad`，BiasTable 累加偏置 |

## 规格

| 参数 | 数据类型 | Shape | 转置 |
| --- | --- | --- | --- |
| `src0` | FLOAT | `[30,40]` | false |
| `src1` | FLOAT | `[40,70]` | false |
| `weight` | FLOAT | `[1,70]` | - |
| `dst` | FLOAT | `[30,70]` | - |

精度判定使用 `rtol=2^-10`、`atol=2^-16`，要求匹配率不低于 99%，并使用最大绝对误差 `1e-2` 或 32 ULP 硬上限。

## 编译结果

```text
[ 50%] Building ASC object CMakeFiles/demo.dir/matmul.asc.o
[100%] Linking ASC executable demo
[100%] Built target demo
```

## 精度结果

| 用例 | 输入 | 最大绝对误差 | 最大相对误差 | 匹配率 | 硬上限违规 | 结果 |
| --- | --- | ---: | ---: | ---: | ---: | --- |
| `random` | A/B/Bias 固定种子均匀分布 `[-100,100]` | 0.01171875 | 8.15592293e-05 | 100% | 0 | 通过 |
| `ones` | A/B/Bias 全 1 | 0 | 0 | 100% | 0 | 通过 |
| `zero_bias` | A/B 固定种子随机，Bias 全 0 | 0.0126953125 | 2.64279875e-04 | 100% | 0 | 通过 |

## 性能说明

任务书未设置性能门槛。CANN 9.1.0 `msprof` 实测 `matmul_custom` Kernel 单次执行耗时为 **6.760 us**（1 个 AI Core 任务）。

独立进程端到端耗时中，首次冷启动 `random` 为 24.538 s，随后 `ones`、`zero_bias` 分别为 6.025 s、6.208 s；再次执行 `random` 为 7.411 s。端到端数据包含 ACL 初始化/反初始化、H2D/D2H 和文件 I/O，不作为纯 Kernel 性能指标。

本次 CANN 9.1.0 验证使用仓库标准入口头 `c_api/asc_simd.h`，不再使用为 CANN 9.0 beta2 设置的头文件 include guard 兼容处理。

## 复现命令

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
for case_name in random ones zero_bias; do
    python3 ../scripts/gen_data.py --case "${case_name}"
    ./demo
    python3 ../scripts/verify_result.py output/output.bin output/golden.bin
done
```

## 相关提交

- 设计文档 PR：<https://gitcode.com/cann/cann-ops-competitions/pull/933>
- 设计评审 Issue：<https://gitcode.com/cann/asc-devkit/issues/1397>
- 易用性 Issue：<https://gitcode.com/cann/asc-devkit/issues/1398>
