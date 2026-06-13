# Conv2D Forward样例

## 概述

本样例演示在Ascend 950PR/Ascend 950DT（dav-3510）上使用基础API实现一个真实的小shape Conv2D Forward（3×3卷积核、带padding、stride与dilation均大于1、多C1通道）。样例GM输入FeatureMap为NHWC格式，Weight按img2col的K轴顺序预排布，输出为NHWC格式；每个AI Core处理一个batch，共使用4个AI Core。

通过3×3卷积核 + padding + stride=2 + dilation=2 + 多C1（Cin=32）的组合，本样例真正触发了卷积计算的核心指令路径：`LoadData3DV2`的img2col空间展开、padding填充与padValue、卷积核滑动步长strideH/W、膨胀系数dilationFilterH/W、`SetLoadDataRepeatWithStride`的repeat配置、多C1通道在K方向的累加。

## 支持产品

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.0.0 |

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

核心流程如下：

1. 使用`DataCopy（Nd2NzParams）`将单batch NHWC FeatureMap从GM搬到L1，C0通道块按H*W紧密排布，得到img2col可用的NC1HWC0布局。
2. 使用`DataCopy（Dn2NzParams）`将预排布的`[Cout, K]`Weight从GM搬到L1，得到`[K, Cout]`的NZ分形布局。
3. 使用`SetLoadDataRepeatWithStride`配置repeat参数后，用`LoadDataWithStride（LoadData3DV2）`将FeatureMap执行img2col搬到L0A，单条指令完成整个3×3卷积核在stride=2、dilation=2下的空间展开与padding填充。
4. 使用`LoadData2D`将NZ格式Weight转置搬到L0B。
5. 使用`Mmad`完成矩阵乘计算（K=288，含多C1通道累加）。
6. 使用`Fixpipe`将L0C结果转换为ROW_MAJOR并按NHWC格式搬回GM。

> 说明：dav-3510上img2col的K轴顺序为`(C1, KH, KW, C0)`，Weight需在`gen_data.py`中按此顺序预排布为`[Cout, K]`后写盘，才能与img2col输出对齐。

## 编译运行

在本样例目录下执行：

```bash
mkdir -p build && cd build;      # 创建并进入build目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu ..;make -j;    # 编译工程
python3 ../scripts/gen_data.py   # 生成测试输入数据
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`后直接执行二进制：

```bash
mkdir -p build_sim && cd build_sim;      # 创建并进入build_sim目录
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim ..;make -j;    # 编译工程
python3 ../scripts/gen_data.py           # 生成测试输入数据
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
