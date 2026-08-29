# Reg Tensor operator+样例

## 概述

本样例使用实验性Tensor API Reg Tensor `operator+`接口实现两个float向量的逐元素加法。输入数据从GM搬入UB后，通过`load`加载到寄存器并完成加法，再通过`store`写回UB和GM。

## 支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | CANN master |

> **说明：** 本样例使用实验性接口，编译时需要开启`CANN_ASC_USE_EXPERIMENTAL`。

## 目录结构

```text
reg_vector_add_tensor_api
├── scripts
│   ├── gen_data.py                # 生成输入数据和真值数据
│   └── verify_result.py           # 对比计算结果和真值
├── CMakeLists.txt                 # 编译工程文件
├── data_utils.h                   # 数据读写函数
├── reg_vector_add_tensor_api.asc  # Ascend C样例实现
├── README.md                      # 中文说明
└── README_en.md                   # 英文说明
```

## 样例说明

计算公式如下：

$$
dst_i = src0_i + src1_i
$$

输入和输出均为shape为`[64]`的float向量。样例使用一个Vector核，依次完成以下操作：

1. 使用`copy_gm_to_ub`将两个输入向量从GM搬入UB。
2. 使用`all_mask`和`load`将UB数据加载为Reg Tensor。
3. 使用实验性`operator+`完成逐元素加法。
4. 使用`store`和`copy_ub_to_gm`将结果写回GM。

## 编译运行

根据CANN开发套件包的[安装说明](../../../../../docs/zh/quick_start.md#prepare&install)配置环境变量，并在本样例目录下执行：

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py
```

使用NPU仿真模式时，在cmake命令中增加`-DCMAKE_ASC_RUN_MODE=sim`。

验证成功时输出：

```text
test pass! verified float Reg Tensor operator+ results.
```
