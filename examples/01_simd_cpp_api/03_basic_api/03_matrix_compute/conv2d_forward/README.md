# conv2d_forward样例

## 概述

本样例介绍如何使用基础API实现Conv2D Forward计算，展示FeatureMap从GM搬入L1、通过LoadData3DV2完成img2col空间展开、使用Mmad进行矩阵乘累加，并通过Fixpipe将结果搬回GM的完整流程。
本样例选用3x3卷积核、padding、stride=2、dilation=2和多C1通道组合，演示padding填充、卷积核滑动、膨胀卷积、repeat搬运配置等常见卷积场景。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── conv2d_forward
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   │   └── verify_result.py           // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── conv2d_forward.asc             // Ascend C样例实现 & 调用样例
│   └── README.md                      // 样例说明文档
```

## 样例规格

本样例固定使用如下Conv2D规格，核函数名为 `conv2d_forward_custom`。每个AI Core处理一个batch，共使用4个AI Core。

<a name="表1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表1：样例规格</span></caption>
<tr><td rowspan="1" align="center">参数</td><td align="center">取值</td><td align="center">说明</td></tr>
<tr><td align="center">FeatureMap</td><td align="center">[N, H, W, C] = [4, 9, 9, 32]</td><td>GM输入为half类型，按场景选择NHWC或CHW排布</td></tr>
<tr><td align="center">Weight</td><td align="center">[Cout, C, Kh, Kw] = [16, 32, 3, 3]</td><td>GM输入为half类型，按img2col的K轴顺序预排布</td></tr>
<tr><td align="center">卷积参数</td><td align="center">stride = [2, 2]，dilation = [2, 2]，padding = [2, 2, 2, 2]</td><td>padding顺序为[top, bottom, left, right]</td></tr>
<tr><td align="center">Output</td><td align="center">[N, HOut, WOut, Cout] = [4, 5, 5, 16]</td><td>GM输出为half类型，NHWC格式</td></tr>
</table>

其中，`C1` 和 `C0` 是FeatureMap在L1上采用NC1HWC0格式时的通道拆分维度。`C0` 表示每个通道块包含的元素个数，本样例中为16；`C1` 表示通道块个数。由于 `C = 32`，因此 `C1 = C / C0 = 2`、`C0 = 16`。

有效卷积核尺寸计算如下：

$$
K_{\text{effective}} = dilation \times (K_h - 1) + 1 = 2 \times (3 - 1) + 1 = 5
$$

输出H/W计算如下：

$$
\begin{aligned}
H_{out} &= \frac{H + pad_{top} + pad_{bottom} - K_{\text{effective}}}{stride_h} + 1 = \frac{9 + 2 + 2 - 5}{2} + 1 = 5 \\
W_{out} &= \frac{W + pad_{left} + pad_{right} - K_{\text{effective}}}{stride_w} + 1 = \frac{9 + 2 + 2 - 5}{2} + 1 = 5
\end{aligned}
$$

矩阵乘规格为 `M = HOut * WOut = 25`，`K = C * Kh * Kw = 288`，`N = Cout = 16`。

## 场景详细说明

本样例通过编译参数 `SCENARIO_NUM` 选择不同的数据搬运和padding填充场景，`SCENARIO_NUM` 不同取值对应的含义如下表所示。所有场景使用相同的Conv2D规格和Mmad/Fixpipe流程。

<a name="表2"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表2：scenarioNum不同取值的含义</span></caption>
<tr><td rowspan="1" align="center">scenarioNum</td><td align="center">FeatureMap输入布局</td><td align="center">Weight输入布局</td><td align="center">GM到L1转换</td><td align="center">img2col搬运</td><td align="center">padding填充值</td><td align="center">支持架构</td></tr>
<tr><td align="center">1</td><td align="center">NHWC [81, 32]</td><td align="center">[288, 16]</td><td align="center">Nd2Nz</td><td align="center">单次搬完整个M，repeatTime = 1</td><td align="center">0</td><td align="center">dav-3510 / dav-2201</td></tr>
<tr><td align="center">2</td><td align="center">NCHW [32, 81]</td><td align="center">[16, 288]</td><td align="center">Dn2Nz</td><td align="center">单次搬完整个M，repeatTime = 1</td><td align="center">0</td><td align="center">dav-3510</td></tr>
<tr><td align="center">3</td><td align="center">NHWC [81, 32]</td><td align="center">[288, 16]</td><td align="center">Nd2Nz</td><td align="center">沿K方向repeat搬运，repeatTime = 2</td><td align="center">0</td><td align="center">dav-3510 / dav-2201</td></tr>
<tr><td align="center">4</td><td align="center">NHWC [81, 32]</td><td align="center">[288, 16]</td><td align="center">Nd2Nz</td><td align="center">单次搬完整个M，repeatTime = 1</td><td align="center">1（结构体padValue字段）</td><td align="center">dav-3510 / dav-2201</td></tr>
<tr><td align="center">5</td><td align="center">NHWC [81, 32]</td><td align="center">[288, 16]</td><td align="center">Nd2Nz</td><td align="center">单次搬完整个M，repeatTime = 1</td><td align="center">1（显式SetLoadDataPaddingValue）</td><td align="center">dav-3510 / dav-2201</td></tr>
</table>

**场景1：Nd2Nz搬入并单次img2col搬运**
- 输入：FeatureMap [4, 9, 9, 32] half类型，NHWC格式；Weight [288, 16] half类型，按 `[K, Cout]` 排布
- 输出：Output [4, 5, 5, 16] half类型，NHWC格式
- 实现：使用 `DataCopy` 的 `Nd2NzParams` 将FeatureMap和Weight从GM搬入L1，使用 `LoadData3DV2` 单次搬完整个M方向数据
- 说明：该场景展示常规NHWC输入在Conv2D Forward中的基础搬运和计算流程

**场景2：Dn2Nz搬入并单次img2col搬运**
- 输入：FeatureMap [4, 32, 81] half类型，CHW格式；Weight [16, 288] half类型，按 `[Cout, K]` 排布
- 输出：Output [4, 5, 5, 16] half类型，NHWC格式
- 实现：使用 `DataCopy` 的 `Dn2NzParams` 将FeatureMap和Weight从GM搬入L1，使用 `LoadData3DV2` 单次搬完整个M方向数据
- 说明：该场景展示Dn2Nz随路转换能力，仅支持 `dav-3510`

**场景3：Nd2Nz搬入并沿K方向repeat搬运**
- 输入：FeatureMap [4, 9, 9, 32] half类型，NHWC格式；Weight [288, 16] half类型，按 `[K, Cout]` 排布
- 输出：Output [4, 5, 5, 16] half类型，NHWC格式
- 实现：使用 `SetLoadDataRepeat` 或 `SetLoadDataRepeatWithStride` 配置repeat参数，使一次Load3D调用沿K方向完成2次迭代搬运
- 说明：该场景展示 `repeatTime > 1` 时，img2col在K方向分块搬运的配置方式

**场景4：通过结构体padValue字段设置padding填充值**
- 输入：FeatureMap [4, 9, 9, 32] half类型，NHWC格式；Weight [288, 16] half类型，按 `[K, Cout]` 排布
- 输出：Output [4, 5, 5, 16] half类型，NHWC格式
- 实现：设置 `loadDataParams.padValue = 1`，通过 `LoadData3DParamsV2` 结构体字段控制padding填充值
- 说明：该场景展示结构体字段设置padding填充值的用法

**场景5：通过SetLoadDataPaddingValue设置padding填充值**
- 输入：FeatureMap [4, 9, 9, 32] half类型，NHWC格式；Weight [288, 16] half类型，按 `[K, Cout]` 排布
- 输出：Output [4, 5, 5, 16] half类型，NHWC格式
- 实现：将Load3D模板参数 `IsResetLoad3dConfig` 设为 `{true, false}`，使结构体 `padValue` 字段不生效，并显式调用 `SetLoadDataPaddingValue` 设置padding填充值
- 说明：该场景展示显式接口设置padding填充值的用法

核心流程如下：

1. 将单batch FeatureMap从GM搬到L1，C0通道块按 `H * W` 紧密排布，得到img2col可用的NC1HWC0布局。场景1/3/4/5使用 `Nd2NzParams`，场景2使用 `Dn2NzParams`。
2. 将预排布的Weight从GM搬到L1，得到 `[K, Cout]` 的Nz分形布局，再通过 `LoadData2D` 转置搬到L0B。
3. 使用 `LoadData3DV2` 将FeatureMap执行img2col搬到L0A，完成3x3卷积核在stride=2、dilation=2下的空间展开与padding填充。
4. 使用 `Mmad` 完成矩阵乘计算，其中 `K = 288`，包含多C1通道累加。
5. 使用 `Fixpipe` 将L0C结果按NHWC格式搬回GM。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。
- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # 编译工程（默认dav-3510 NPU模式）
  SCENARIO_NUM=$SCENARIO_NUM python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # CPU调试模式
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # NPU仿真模式
  ```
  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认）、`dav-2201` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT，dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和 Atlas A3 训练系列产品/Atlas A3 推理系列产品 |
  | `SCENARIO_NUM` | 1-5 | 场景编号，其中场景2仅支持 `dav-3510` |

  `gen_data.py` 需与cmake使用相同的 `SCENARIO_NUM`。若修改场景编号，请同步修改编译命令中的 `-DSCENARIO_NUM` 和生成数据命令中的 `SCENARIO_NUM`。

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
