# 使用 C API 实现 GM 到 L1 数据搬运（Cube 场景）

本样例使用 Ascend C API 将矩阵从 Global Memory（GM）搬运到 L1 Buffer，随后完成 L1 到 L0 的数据搬运、矩阵乘法，以及从 L0C 到 GM 的结果校验。

本样例仅支持 Ascend 950PR 和 Ascend 950DT（`dav-3510`），且仅支持在 NPU 上运行；不提供 CPU Debug 或仿真运行模式。

## 目录结构

```text
asc_copy_gm2l1
├── CMakeLists.txt                 # 构建脚本
├── asc_copy_gm2l1.asc             # C API Kernel 和主机侧调用代码
├── data_utils.h                   # 二进制数据读写辅助函数
├── scripts
│   ├── gen_data.py                # 生成输入数据和真值数据
│   └── verify_result.py           # 校验计算结果
├── README.md
└── README_en.md
```

## 算子说明

输入矩阵 A、B 的数据类型均为 `half`，输出矩阵 C 的数据类型为 `float`：

```text
A[M, K] × B[K, N] = C[M, N]
M = 128, K = 128, N = 256
```

`SCENARIO_NUM` 用于选择 GM 中的输入布局：

| 场景 | GM 输入布局 | GM 到 L1 C API |
| --- | --- | --- |
| 1 | NZ | `asc_copy_gm2l1_sync` |
| 2 | ND | `asc_copy_gm2l1_nd2nz_sync` |
| 3 | DN | `asc_copy_gm2l1_dn2nz_sync` |

场景 1 的输入已经按 NZ 格式存储；场景 2 和场景 3 分别在从 GM 到 L1 的搬运过程中完成 ND 到 NZ、DN 到 NZ 的格式转换。

## 实现说明

Kernel 按以下 C API 流程执行：

1. 调用 `asc_init` 初始化 Kernel 状态。
2. 根据所选场景，调用 `asc_copy_gm2l1_sync`、`asc_copy_gm2l1_nd2nz_sync` 或 `asc_copy_gm2l1_dn2nz_sync`，将 A、B 搬运到 L1。
3. 调用 `asc_copy_l12l0a_sync` 和 `asc_copy_l12l0b_transpose_sync`，将 L1 中的 NZ 数据搬运到 L0A、L0B。
4. 调用 `asc_mmad_sync` 执行矩阵乘法。
5. 调用 `asc_copy_l0c2gm_sync`，将 L0C 中的 NZ 结果转换为行主序 `float` 数据并写回 GM。

所有 Kernel 侧操作均使用同步 C API；本样例不依赖 C++ `GlobalTensor`、`LocalTensor`、`DataCopyParams` 或 CPU Debug 宏。

## 构建与运行

执行下列命令前，请先按 CANN 文档配置构建环境和 NPU 运行环境。

```bash
cd asc_copy_gm2l1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=1
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

将命令中的 `SCENARIO_NUM` 和 `-scenarioNum` 同时替换为 `1`、`2` 或 `3`，即可运行对应输入布局的场景。

### 构建选项

| 选项 | 说明 | 可选值 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | NPU 架构 | `dav-3510` |
| `SCENARIO_NUM` | GM 输入布局场景 | `1`（NZ）、`2`（ND）、`3`（DN） |

## 预期结果

当 NPU 输出与真值数据的误差在允许范围内时，`verify_result.py` 会输出 `test pass!`。
