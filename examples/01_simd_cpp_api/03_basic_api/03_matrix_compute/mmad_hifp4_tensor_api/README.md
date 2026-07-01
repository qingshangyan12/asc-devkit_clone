# 基于Tensor API实现的HiFP4 MX Mmad样例

## 概述

本样例基于Tensor API编程方式实现MX矩阵乘法，A、B矩阵数据类型为hifloat4x2_t（HiFP4），scaleA、scaleB以fp8_e8m0_t字节流形式存储（每64个K方向元素占用连续4个fp8_e8m0_t字节，组成一个32-bit打包的hifloat4 scale word），输出矩阵C数据类型为float。A矩阵按ND布局存储，B矩阵按DN布局存储（在GM上以转置后的[N, K]保存）。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | > CANN 9.1.0 |

> **说明：** 该样例依赖尚未正式发布的CANN特性，请使用最新的CANN master包。

## 目录结构介绍

```text
├── mmad_hifp4_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py            // 真值对比文件
│   ├── CMakeLists.txt                  // 编译工程文件
│   ├── data_utils.h                    // 数据读取写出函数
│   ├── mmad_hifp4_tensor_api.asc       // Ascend C样例实现与调用样例
│   └── README.md                       // 样例说明文档
```

## 样例描述

- 样例功能：

  本样例实现单核静态Shape下、HiFP4输入的MX矩阵乘法功能。

  1. MX矩阵乘法

     样例在Kernel中通过Tensor API构造GM、L1、L0A、L0B、L0AScale、L0BScale、L0C上的Tensor对象，使用Copy接口完成GM到L1、L1到L0的搬运，并通过`Mmad`接口（配置`MmadType::MX`）完成带量化的矩阵乘法。A矩阵按ND布局读取（[M, K]），B矩阵按DN布局读取（输入文件中以转置后的[N, K]保存）。L1上A矩阵使用`NZLayoutPtn`、B矩阵使用`ZNLayoutPtn`，scaleA使用`ZZLayoutPtn`、scaleB使用`NNLayoutPtn`。

  2. HiFP4数据类型说明

     hifloat4x2_t在GM上每个字节打包两个4-bit元素，与fp4_e1m2共享相同的4-bit编码布局。每64个连续的K方向元素共享一个32-bit的hifloat4 scale word，scale word由E6M2（低8位）、E1_8（8-15位）、E1_16（16-31位）三部分组成，对应每个元素的实际缩放因子为：`E6M2 * 2^(E1_8[i/8] + E1_16[i/4])`。在GM上，该scale word以`fp8_e8m0_t`字节流形式存储，每组占用4个连续的`fp8_e8m0_t`字节（`HIF4_SCALE_BYTES_PER_GROUP`）。

- 样例规格：

  在核函数直调样例中，样例默认支持的shape为：M = 128, N = 128, K = 256。

  <a name="表1"></a>
  <table border="2" align="center">
  <caption style="font-weight: normal;">
      <span style="font-weight: bold; font-size: 1.2em;">📌 表1：样例规格表</span></caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">HiFP4 MX Matmul</td></tr>
  <tr><td rowspan="6" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x1 (矩阵A)</td><td align="center">[M, K]</td><td align="center">hifloat4x2_t</td><td align="center">ND</td></tr>
  <tr><td align="center">x2 (矩阵B，转置存储)</td><td align="center">[N, K]</td><td align="center">hifloat4x2_t</td><td align="center">DN</td></tr>
  <tr><td align="center">scaleA</td><td align="center">[M, ScaleK * 4]</td><td align="center">fp8_e8m0_t</td><td align="center">ND</td></tr>
  <tr><td align="center">scaleB</td><td align="center">[N, ScaleK * 4]</td><td align="center">fp8_e8m0_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y (矩阵C)</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_hifp4_custom</td></tr>
  </table>

  其中ScaleK = CeilAlign(CeilDiv(K, 64), 2)，K=256 时 ScaleK = 4，每个scale word占用4个`fp8_e8m0_t`字节，因此scaleA和scaleB在GM上每行占用16个`fp8_e8m0_t`字节。

  运行时参数默认值：

  | 参数        | 默认值 | 说明                        |
  | :---------- | :----- | :-------------------------- |
  | M           | 128    | 矩阵A的行数                 |
  | N           | 128    | 矩阵B的列数                 |
  | K           | 256    | 矩阵A的列数/矩阵B的行数     |
  | NUM_BLOCKS  | 1      | Kernel启动的Block数量       |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。**当前仅支持使用[CANN master](../../../../../docs/quick_start.md#下载-cann-master)**。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | :--- | :----- | :--- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果
  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
