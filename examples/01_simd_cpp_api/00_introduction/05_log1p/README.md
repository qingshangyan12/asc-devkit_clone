# 基于RegBase编程log1p样例

## 概述
本样例基于RegBase编程范式实现log(1+x)高精度计算，覆盖float和half数据类型，重点验证x接近0时的精度。

## 目录结构
```
├── log1p
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   └── log1p.asc                      // AscendC样例实现 & 调用样例
```
