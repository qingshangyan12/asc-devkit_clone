# fixpipe_co12gm_quantization_f322s8样例
## 概述
本样例介绍如何使用基础API Fixpipe将矩阵乘的结果从CO1搬入GM，并使能随路量化将矩阵乘结果由float类型数据量化为uint8类型。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── fixpipe_co12gm_quantization_f322s8
│   ├── scripts
│   │   ├── gen_data.py                                   // 输入数据和真值数据生成脚本
│   │   └── verify_result.py                              // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                                    // 编译工程文件
│   ├── data_utils.h                                      // 数据读入写出函数
│   └── fixpipe_co12gm_quantization_f322s8.asc            // Ascend C算子实现 & 调用样例
```
