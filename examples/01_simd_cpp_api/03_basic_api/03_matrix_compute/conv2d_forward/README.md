# Conv2D Forward样例

## 概述

本样例演示在Ascend 950PR/Ascend 950DT（dav-3510）与Atlas A2/A3（dav-2201）上使用基础API实现一个真实的小shape Conv2D Forward（3×3卷积核、带padding、stride与dilation均大于1、多C1通道）。样例GM输入FeatureMap为NHWC格式，Weight按img2col的K轴顺序预排布，输出为NHWC格式；每个AI Core处理一个batch，共使用4个AI Core。

通过3×3卷积核 + padding + stride=2 + dilation=2 + 多C1（Cin=32）的组合，本样例真正触发了卷积计算的核心指令路径：`LoadData3DV2`的img2col空间展开、padding填充与padValue、卷积核滑动步长strideH/W、膨胀系数dilationFilterH/W、repeat配置、多C1通道在K方向的累加。

## 支持产品

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT（dav-3510） | >= CANN 9.0.0 |
| Atlas A2 训练/推理系列、Atlas A3 训练/推理系列（dav-2201） | >= CANN 9.0.0 |

## 目录结构

```
├── conv2d_forward
│   ├── scripts
│   │   ├── gen_data.py
│   │   └── verify_result.py
│   ├── CMakeLists.txt
│   ├── conv2d_forward.asc
│   ├── data_utils.h
│   └── README.md
```

## 样例说明

本样例固定shape如下：

| 参数 | 取值 |
|------|------|
| FeatureMap | [N, H, W, C] = [4, 9, 9, 32] |
| Weight | [Cout, C, Kh, Kw] = [16, 32, 3, 3] |
| Stride/Dilation/Pad | stride=2, dilation=2, pad=2 |
| Output | [N, HOut, WOut, Cout] = [4, 5, 5, 16] |

其中 C=32 即 C1=2、C0=16；有效卷积核尺寸=dilation*(Kh-1)+1=5，HOut=WOut=(9+2*2-5)/2+1=5；M=HOut*WOut=25，K=C*Kh*Kw=288，N=Cout=16。

本样例通过编译宏`SCENARIO_NUM`切换GM->L1的随路转换方式，演示两种数据搬运场景：

| SCENARIO_NUM | FeatureMap输入布局 | Weight输入布局 | GM->L1转换 | 支持架构 |
|:---:|------|------|------|------|
| 1（默认） | NHWC `[H*W, CIN]` | `[K, Cout]` | `Nd2Nz` | dav-3510 / dav-2201 |
| 2 | CHW `[CIN, H*W]`（通道优先） | `[Cout, K]` | `Dn2Nz` | 仅 dav-3510 |

> 两个场景的卷积计算结果（golden）、shape 及 img2col/Mmad/Fixpipe 流程完全一致，区别仅在`CopyIn`的搬运指令与对应的host端输入布局。`Dn2Nz`为dav-3510独有的随路转换指令，dav-2201不支持（编译时由CMakeLists校验拦截）。

核心流程如下：

1. 将单batch FeatureMap从GM搬到L1，C0通道块按H*W紧密排布，得到img2col可用的NC1HWC0布局。场景1用`DataCopy（Nd2NzParams）`（源NHWC），场景2用`DataCopy（Dn2NzParams）`（源CHW通道优先）。
2. 将预排布的Weight从GM搬到L1，得到`[K, Cout]`的NZ分形布局。场景1用`Nd2Nz`（源`[K,Cout]`），场景2用`Dn2Nz`（源`[Cout,K]`）。
3. 将FeatureMap执行img2col搬到L0A，单条指令完成整个3×3卷积核在stride=2、dilation=2下的空间展开与padding填充。其中：
   - dav-3510：先用`SetLoadDataRepeatWithStride`配置repeat参数，再用`LoadDataWithStride（LoadData3DV2）`搬运；
   - dav-2201：直接用`LoadData（LoadData3DV2）`搬运，无需repeat寄存器。
4. 使用`LoadData2D`将NZ格式Weight转置搬到L0B。
5. 使用`Mmad`完成矩阵乘计算（K=288，含多C1通道累加）。
6. 使用`Fixpipe`将L0C结果按NHWC格式搬回GM。其中dav-3510使用`FixpipeParamsArch3510`（CO2Layout::ROW_MAJOR），dav-2201使用`FixpipeParamsV220`。

> 说明：dav-3510与dav-2201上img2col的K轴顺序均为`(C1, KH, KW, C0)`，Weight需在`gen_data.py`中按此顺序预排布后写盘，才能与img2col输出对齐。

## 编译运行

在本样例目录下执行（`CMAKE_ASC_ARCHITECTURES`可取`dav-3510`或`dav-2201`，`SCENARIO_NUM`可取`1`或`2`，缺省均为默认值）：

```bash
mkdir -p build && cd build;      # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu -DSCENARIO_NUM=1 ..;make -j;    # 编译工程
python3 ../scripts/gen_data.py   # 生成测试输入数据（场景需与cmake一致，可用 SCENARIO_NUM=1 python3 ...）
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`后直接执行二进制：

```bash
mkdir -p build_sim && cd build_sim;      # 创建并进入build_sim目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim -DSCENARIO_NUM=1 ..;make -j;    # 编译工程
SCENARIO_NUM=1 python3 ../scripts/gen_data.py           # 生成测试输入数据
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

也可直接使用`run.sh`，通过环境变量`ARCH`与`SCENARIO_NUM`控制（默认`dav-3510`、场景1）：

```bash
bash run.sh                                  # dav-3510 + 场景1（默认）
ARCH=dav-2201 bash run.sh                     # dav-2201 + 场景1
SCENARIO_NUM=2 bash run.sh                     # dav-3510 + 场景2（Dn2Nz）
```

说明：
- 编译运行dav-2201时，将`-DCMAKE_ASC_ARCHITECTURES=dav-3510`替换为`-DCMAKE_ASC_ARCHITECTURES=dav-2201`即可。
- 场景2（`SCENARIO_NUM=2`，Dn2Nz）仅支持dav-3510；在dav-2201上配置会被CMakeLists校验拦截并报错。
- `gen_data.py`需与cmake使用相同的`SCENARIO_NUM`（通过环境变量传入），否则输入布局与kernel不匹配。
