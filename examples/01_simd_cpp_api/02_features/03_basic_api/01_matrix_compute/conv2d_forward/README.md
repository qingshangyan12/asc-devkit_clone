# Conv2D Forward样例

## 概述

本样例演示在Atlas A2/A3（dav-2201）上使用基础API实现一个小shape Conv2D Forward。样例输入FeatureMap为NC1HWC0格式，Weight为[C1, Kh, Kw, Cout, C0]展开后的Fractal_Z格式，每个AI Core处理一个batch，共使用4个AI Core。

## 支持产品

| 产品 | CANN软件版本 |
|------|-------------|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

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
| FeatureMap | [N, C1, H, W, C0] = [4, 1, 4, 4, 16] |
| Weight | [C1, Kh, Kw, Cout, C0] = [1, 3, 3, 16, 16] |
| Stride/Dilation/Pad | stride=1, dilation=1, pad=1 |
| Output | [N, C1Out, HOut, WOut, C0] = [4, 1, 4, 4, 16] |

核心流程如下：

1. 使用`DataCopy`将单batch FeatureMap和Weight从GM搬到L1。
2. 使用`LoadData3DV2`将FeatureMap执行img2col并搬到L0A。
3. 使用`LoadData2D`将Fractal_Z格式Weight搬到L0B。
4. 使用`Mmad`完成矩阵乘计算。
5. 使用`Fixpipe`将L0C结果转换为ND格式并搬回GM。

## 编译运行

在本样例目录下执行：

```bash
python3 scripts/gen_data.py
cmake -B build -S . -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`并通过`msprof op simulator`执行：

```bash
python3 scripts/gen_data.py
cmake -B build_sim -S . -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DCMAKE_ASC_RUN_MODE=sim
cmake --build build_sim
msprof op simulator --soc-version=Ascend910B1 ./build_sim/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```
