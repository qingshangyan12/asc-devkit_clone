# Conv3D Forward Tensor API 样例

## 概述

本样例介绍如何使用Tensor API实现Conv3D Forward计算，展示FeatureMap从Global Memory（NCDHW）搬入L1 Buffer（NDC1HWC0）、通过`CopyL12L0A`（Img2Col）完成空间展开、使用`Mmad`进行矩阵乘，并通过`CopyL0C2GM`将结果搬回Global Memory（NCDHW）的完整流程。

与Conv2D相比，Conv3D多出深度维`D`。本样例中深度方向由Tensor API按如下方式处理：

- **Global Memory→L1 Buffer**：`NCDHW→NDC1HWC0`通路内部按`D`循环，每个depth slice走一次dn2nz搬运。
- **L1 Buffer→L0A Buffer**：Img2Col将`D`与`C1`合并进通道轴（`channelSize = D * C1 * C0`），因此单次Img2Col即覆盖整个深度方向。这等价于`Kd == D`、深度方向不滑窗，输出深度`DOUT = 1`。
- **L0C Buffer→Global Memory**：`NZ→NCDHW`通路（nz2dn），输出深度视为1。

由于深度合并进通道轴，矩阵乘的`K = KH * KW * (D * C1 * C0)`，Weight按`[K, Cout]`行优先摆放，`K`序为`(D, C1, KH, KW, C0)`。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── conv3d_forward_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   │   └── verify_result.py           // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── conv3d_forward_tensor_api.asc  // Ascend C Tensor API样例实现与调用样例
│   └── README.md                      // 样例说明文档
```

## 样例规格

本样例固定使用如下Conv3D规格，核函数名为`conv3d_forward_tensor_api_custom`。每个AI Core处理一个batch，共使用4个AI Core。

<a name="表1"></a>
<table border="2" align="center">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表1：样例规格</span></caption>
<tr><td rowspan="1" align="center">参数</td><td align="center">取值</td><td align="center">说明</td></tr>
<tr><td align="center">FeatureMap</td><td align="center">[N, C, D, H, W] = [4, 32, 2, 9, 9]</td><td>Global Memory输入为half类型，NCDHW排布</td></tr>
<tr><td align="center">Weight</td><td align="center">[Cout, C, Kd, Kh, Kw] = [16, 32, 2, 3, 3]</td><td>Global Memory输入为half类型，按[K, Cout]二维排布展开</td></tr>
<tr><td align="center">卷积参数</td><td align="center">stride = [2, 2]，dilation = [2, 2]，padding = [2, 2, 2, 2]，padValue = 1</td><td>padding顺序为[top, bottom, left, right]，仅作用于H/W方向</td></tr>
<tr><td align="center">Output</td><td align="center">[N, Cout, DOut, HOut, WOut] = [4, 16, 1, 5, 5]</td><td>Global Memory输出为half类型，NCDHW排布</td></tr>
</table>

其中，`C1`和`C0`是FeatureMap在L1 Buffer上采用NDC1HWC0格式时的通道拆分维度。`C0`表示每个通道块包含的元素个数，本样例中为16；`C1`表示通道块个数。由于`C = 32`，因此`C1 = C / C0 = 2`、`C0 = 16`。

`Kd`固定等于`D`，即深度方向一次覆盖、不滑窗，因此`DOut = 1`。深度方向不做padding。

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

矩阵乘规格为`M = HOut * WOut = 25`，`K = D * C * Kh * Kw = 2 * 32 * 3 * 3 = 576`，`N = Cout = 16`。

## 核心流程

1. 将单batch FeatureMap从Global Memory搬到L1 Buffer，调用Copy接口通过`CopyGM2L1` `Operation`搬运能力，完成NCDHW到NDC1HWC0的随路转换。该通路内部按深度维`D`循环，每个depth slice走一次dn2nz。同时将Weight从Global Memory搬运到L1 Buffer，完成ND到ZN格式转换。
2. 将L1 Buffer上的FeatureMap执行Img2Col空间展开搬到L0A Buffer，调用Copy接口通过`CopyL12L0A` `Operation`的Img2Col搬运能力。`D`与`C1`合并进通道轴（`channelSize = D * C1 * C0`），单次展开即覆盖整个深度方向，并完成3x3卷积核在stride=2、dilation=2下的展开与padding填充。
3. 将L1 Buffer上的Weight搬到L0B Buffer，调用Copy接口通过`CopyL12L0B` `Operation`搬运能力。
4. 使用`Mmad`完成矩阵乘计算，L0A Buffer(NZ) × L0B Buffer(ZN) → L0C Buffer(NZ)。
5. 使用`CopyL0C2GM`将L0C Buffer结果搬回Global Memory，完成NZ到NCDHW的随路转换（nz2dn）。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # 编译工程（默认dav-3510 NPU模式）
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：dav-3510对应Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
