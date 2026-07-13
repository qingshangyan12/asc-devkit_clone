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

输入矩阵 A、B 的数据类型均为 `half`。场景 1-4 的输出矩阵 C 为 `float`，场景 5 的输出矩阵 C 为 `int8`：

```text
A[M, K] × B[K, N] = C[M, N]
M = 128, K = 128, N = 256
```

`SCENARIO_NUM` 用于选择 GM→L1 及输出路径：

| 场景 | GM 输入布局 | 附加操作 | C API 重点 |
| --- | --- | --- |
| 1 | NZ | 无 | `asc_copy_gm2l1_sync` |
| 2 | ND | 无 | `asc_copy_gm2l1_nd2nz_sync` |
| 3 | DN | 无 | `asc_copy_gm2l1_dn2nz_sync` |
| 4 | ND | `C = A * B + bias`，`bias[1, N]` | `asc_copy_l12bt_sync` 与 BiasTable 初值 MMAD |
| 5 | ND | 将 C 向量量化为 `int8` | `asc_copy_l12fb_sync`、`asc_set_l0c2gm_config` 与 `VQF322B8_PRE` |

场景 1 的输入已经按 NZ 格式存储；场景 2、4、5 在从 GM 到 L1 的搬运过程中完成 ND 到 NZ 转换，场景 3 完成 DN 到 NZ 转换。场景 4 对输出的每一行加同一个 `bias[1, N]`；场景 5 为每个输出列加载一个向量量化参数，并写出 `int8` 结果。

## 实现说明

Kernel 按以下 C API 流程执行：

1. 调用 `asc_init` 初始化 Kernel 状态。
2. 根据所选场景，调用 `asc_copy_gm2l1_sync`、`asc_copy_gm2l1_nd2nz_sync` 或 `asc_copy_gm2l1_dn2nz_sync`，将 A、B 搬运到 L1。
3. 场景 4 中，将 ND Bias 搬入 L1 后通过 `asc_copy_l12bt_sync` 搬入 BiasTable，以便 `asc_mmad_sync` 从 C2 获取初始值。
4. 调用 `asc_copy_l12l0a_sync` 和 `asc_copy_l12l0b_transpose_sync`，将 L1 中的 NZ 数据搬运到 L0A、L0B。
5. 调用 `asc_mmad_sync` 执行矩阵乘法。
6. 场景 5 中，通过 `asc_copy_l12fb_sync` 将每列量化参数从 L1 搬入 Fixpipe Buffer，并调用 `asc_set_l0c2gm_config` 配置其地址。
7. 调用 `asc_copy_l0c2gm_sync`，将 L0C 中的 NZ 结果转换为行主序 `float`（场景 1-4）或向量量化后的 `int8`（场景 5）数据并写回 GM。

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

将命令中的 `SCENARIO_NUM` 和 `-scenarioNum` 同时替换为 `1` 至 `5`，即可运行对应场景。

### 构建选项

| 选项 | 说明 | 可选值 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | NPU 架构 | `dav-3510` |
| `SCENARIO_NUM` | GM→L1 与输出场景 | `1`（NZ）、`2`（ND）、`3`（DN）、`4`（Bias）、`5`（向量量化） |

## 预期结果

场景 1-4 的 NPU 输出与浮点真值误差在允许范围内、或场景 5 的 `int8` 输出与真值完全一致时，`verify_result.py` 会输出 `test pass!`。
