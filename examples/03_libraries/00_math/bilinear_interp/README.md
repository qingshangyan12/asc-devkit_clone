# BilinearInterpolation样例

## 概述

本样例是 BilinearInterpolation API 的算子实现，参考 vec_binary 样例结构。

**注意**：BilinearInterpolation API 实际用于**变形卷积(Deformable Convolution)/ROI 池化**等场景，而非通用的图像双线性插值。它的工作原理是：
- 从 src0Offset 读取偏移值
- 每个偏移指向 src0 的一个 DataBlock 起始地址
- 将 src1 的值与 src0 中对应位置的数据相乘并累加

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构

```
bilinear_interp/
├── scripts
│   └── gen_data.py              # 输入数据生成脚本
├── CMakeLists.txt               # 编译工程文件
├── data_utils.h                 # 数据读入写出函数
├── bilin_interp.asc             # Ascend C算子实现
├── test.sh                      # 编译运行脚本
└── README.md                    # 本文件
```

## 算子描述

- 算子功能：BilinearInterpolation 矢量计算
- 输入：
  - src: 源数据，shape [512]，half 类型
  - weight: 权重，shape [128]，half 类型
  - offset: 偏移数组，shape [128]，uint32_t 类型
- 输出：
  - dst: 结果，shape [128]，half 类型

## API 参数说明

| 参数 | 说明 |
|-----|------|
| mask | 控制每次迭代参与计算的元素 |
| hRepeat | 水平方向迭代次数 |
| repeatMode | 迭代模式（false: src1 单值；true: 逐元素） |
| dstBlkStride | 目的操作数不同 DataBlock 间地址步长 |
| vROffset | 垂直迭代间目的操作数地址偏移 |
| vRepeat | 垂直方向迭代次数 |
| sharedTmpBuffer | 临时空间（A2/A3 需要 srcSize*32 + weightSize*32 字节） |

## 编译运行

```bash
./test.sh
```

## 参考资料

- BilinearInterpolation API文档: `asc-devkit/docs/api/context/BilinearInterpolation(ISASI).md`
