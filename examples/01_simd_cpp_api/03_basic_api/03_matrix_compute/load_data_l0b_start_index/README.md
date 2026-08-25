# load_data_l0b_start_index样例

## 概述

本样例从legacy deep和legacy square_32 shape用例中拆分而来，展示如何使用`LoadData2DParams`将B矩阵从L1 Buffer搬运到L0B Buffer。

两个场景的输入类型均为half，输出类型均为float，规格如下：

| 场景 | 原始用例 | [M, N, K] | blockDim |
|------|----------|-----------|----------|
| 1 | legacy deep | [16, 16, 32] | 2 |
| 2 | legacy square_32 | [32, 32, 32] | 1 |

B矩阵包含两个K方向分形，样例循环调用`LoadData`两次。场景1在dav-2201上通过非零`startIndex`选择第2个分形；场景2以及dav-3510通过源Tensor偏移选择分形，`startIndex`保持为0。场景2在dav-2201上按照单个32×32矩阵生成L1 Nz布局，并通过`srcStride = K / 16`读取同一K分形中的两个N分形。

结果从L0C Buffer搬出到GM时使用`FixpipeParamsV220`参数结构体，并开启NZ2ND格式转换。该接口同时支持dav-2201和dav-3510架构。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.1.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── load_data_l0b_start_index
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本
│   │   └── verify_result.py            // 逐比特验证输出数据的脚本
│   ├── CMakeLists.txt                   // 编译工程文件
│   ├── data_utils.h                     // 数据读写函数
│   ├── load_data_l0b_start_index.asc    // Ascend C样例实现
│   ├── README.md                        // 中文说明文档
│   └── README_en.md                     // 英文说明文档
```

## 样例描述

样例执行流程如下：

```text
GM(ND) -> L1(Nz) -> L0A(Nz)/L0B(Zn) -> L0C(Nz) -> GM(ND)
       │         │                  │          │
    DataCopy  LoadData             Mmad        Fixpipe
```

B矩阵从L1 Buffer搬运到L0B Buffer的关键代码如下：

```cpp
for (uint32_t kIdx = 0; kIdx < kFractals; ++kIdx) {
#if SCENARIO_NUM == 2 && defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    AscendC::LoadData2DParams params(0, nFractals, kFractals, 0, 0, true, 0);
    AscendC::LoadData(dst[kIdx * N * FRACTAL_EDGE], src[kIdx * FRACTAL_ELEMENTS], params);
#elif SCENARIO_NUM == 2 || (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    AscendC::LoadData2DParams params(0, nFractals, 1, 0, 0, true, 0);
    AscendC::LoadData(dst[kIdx * N * FRACTAL_EDGE],
        src[kIdx * nFractals * FRACTAL_ELEMENTS], params);
#else
    AscendC::LoadData2DParams params(kIdx * nFractals, nFractals, 1, 0, 0, true, 0);
    AscendC::LoadData(dst[kIdx * N * FRACTAL_EDGE], src, params);
#endif
}
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`、`dav-3510`（默认） | NPU架构 |
  | `SCENARIO_NUM` | `1`（默认）、`2` | 场景编号 |

验证成功时输出：

```text
test pass!
```
