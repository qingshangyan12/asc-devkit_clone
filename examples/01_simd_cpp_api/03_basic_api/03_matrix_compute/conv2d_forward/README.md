# Conv2D Forward样例

## 概述

本样例演示在Ascend 950PR/Ascend 950DT（dav-3510）上使用基础API实现一个小shape Conv2D Forward。样例GM输入FeatureMap为NHWC格式，Weight为NCHW格式，输出为NHWC格式；每个AI Core处理一个batch，共使用4个AI Core。

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
| FeatureMap | [N, H, W, C] = [4, 4, 4, 16] |
| Weight | [Cout, C, Kh, Kw] = [16, 16, 1, 1] |
| Stride/Dilation/Pad | stride=1, dilation=1, pad=0 |
| Output | [N, HOut, WOut, Cout] = [4, 4, 4, 16] |

核心流程如下：

1. 使用`DataCopy（Nd2NzParams）`将单batch NHWC FeatureMap从GM搬到L1，得到可作为NC1HWC0使用的L1布局。
2. 使用`DataCopy（Dn2NzParams）`将NCHW Weight从GM搬到L1，得到`[C1, Kh, Kw, Cout, C0]`展开后的Fractal_Z布局。
3. 使用`LoadData3DV2`将FeatureMap执行img2col并搬到L0A。
4. 使用`LoadData2D`将Fractal_Z格式Weight搬到L0B。
5. 使用`Mmad`完成矩阵乘计算。
6. 使用`Fixpipe`将L0C结果转换为ROW_MAJOR并按NHWC格式搬回GM。

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
