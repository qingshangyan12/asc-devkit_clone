# 基于高阶API的MxFP4矩阵乘样例

## 概述

本样例基于 Ascend C 高阶 API，实现带 scale 的 MxFP4 Matmul矩阵乘法。通过编译参数 `SCENARIO_NUM` 和数据脚本参数 `scenarioNum` 选择不同场景。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── adv_api_practice
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── matmul_mxfp4.asc        // Ascend C样例实现 & 调用样例
│   └── README.md               // 样例说明文档
```

## 样例规格

样例当前支持两个场景，输出类型均为 `bfloat16_t`，输入 A/scaleA 均按 `[M, K]`、`[M, scaleK]` 排布，输入 B/scaleB 均按 `[N, K]`、`[N, scaleK]` 排布：
- 场景1：单核场景，固定 shape 为 `256 x 256 x 256` 的 MxFP4 Matmul，输出数据类型为 `bfloat16_t`。
- 场景2：单核场景，固定 shape 为 `40 x 70 x 80` 的 MxFP4 Matmul，输出数据类型为 `bfloat16_t`。
- 场景3：多核场景，固定 shape 为 `1024 x 1024 x 1024` 的 MxFP4 Matmul，输出数据类型为 `bfloat16_t`。

### 输入输出format

  <img src="figure/MxMatmul.png">

| 输入/输出 | 逻辑形状 | 数据类型 | 数据排布类型 | 说明 |
|------|------|----------|------|------|
| A | `[M, K]` | `fp4x2_e1m2_t` | ND | 左矩阵，每个字节打包 2 个 fp4 元素 |
| scaleA | `[M, scaleK]` | `fp8_e8m0_t` | ND | A 矩阵的缩放因子，A矩阵在K 方向每 32 个元素共享一个 scale |
| B | `[N, K]` | `fp4x2_e1m2_t` | ND | 右矩阵，按 `[N, K]` 形式输入 kernel |
| scaleB | `[N, scaleK]` | `fp8_e8m0_t` | ND | B 矩阵的缩放因子，B矩阵在K 方向每 32 个元素共享一个 scale |
| C | `[M, N]` | `bfloat16_t` | ND | 输出矩阵 |

- 四路输入的数据排布格式如下图所示：

  <img src="figure/formatOfMx.png">

其中：
- 两个`fp4x2_e1m2_t`打包存储在一个字节中，因此当数据类型为`fp4`时，K 需要为偶数
- `scaleK = align_even(ceil(K / 32))`，表示先向上整除再对齐到 2 的倍数。由于硬件约束要求 scale 数据在 K 方向满足 2Byte 连续对齐，因此 scaleK 必须为偶数
- 由于硬件约束，当 scaleB 矩阵为 `[scaleK, N]` 输入时，需要 K 方向 2 Byte连续，因此推荐 ScaleB 采用`[N, scaleK]`输入



## 样例实现： 
  - Kernel关键步骤
    - 创建Matmul对象：使用MatmulTypeWithScale使能scaleA、scaleB。
      ```cpp
      using aType = AscendC::MatmulTypeWithScale<AscendC::TPosition::GM, AscendC::TPosition::GM, CubeFormat::ND, fp4x2_e1m2_t, false>;
      using bType = AscendC::MatmulTypeWithScale<AscendC::TPosition::GM, AscendC::TPosition::GM, CubeFormat::ND, fp4x2_e1m2_t, false>;
      using cType = AscendC::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, bfloat16_t>;
      // 定义 matmul 对象时，传入 MatmulWithScalePolicy 表示开启 MxMatmul 模板策略，CFG_MDL表示使能Matmul Config的MDL模板
      AscendC::Matmul<aType, bType, cType, cType, CFG_MDL, 
          AscendC::MatmulCallBackFunc<nullptr, nullptr, nullptr>,
          AscendC::Impl::Detail::MatmulWithScalePolicy> matmulObj;
      ```
    - 设置左矩阵A与左量化系数矩阵scaleA、右矩阵B与右量化系数矩阵scaleB。
      ```cpp
      matmulObj.SetTensorA(aGlobal, IS_TRANS_A);
      matmulObj.SetTensorB(bGlobal, IS_TRANS_B);
      matmulObj.SetTensorScaleA(asGlobal, IS_TRANS_A);
      matmulObj.SetTensorScaleB(bsGlobal, IS_TRANS_B);
      ```

  - Tiling关键步骤
      - 创建一个Tiling对象：使用SetMadType设置Matmul模式，使能MxMatmul场景，使用SetScaleAType设置ScaleA的信息、使用SetScaleBType设置scaleB的信息。
        ```cpp
        tilingApi.SetMadType(matmul_tiling::MatrixMadType::MXMODE);
        tilingApi.SetScaleAType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, IS_TRANS_A);
        tilingApi.SetScaleBType(matmul_tiling::TPosition::GM, matmul_tiling::CubeFormat::ND, IS_TRANS_B);
        ```

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```
  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```
  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式或 `SCENARIO_NUM` 前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|---------|--------|
  | CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
  | CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-3510 | dav-3510 |
  | SCENARIO_NUM | 样例场景编号 | 1, 2, 3 | 1 |

- 执行结果

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```
