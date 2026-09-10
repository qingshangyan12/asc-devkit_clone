# ELU 算子样例

## 概述
本样例基于 C API 编程接口实现 ELU（Exponential Linear Unit）激活函数运算。

数学公式：
```
ELU(x) = scale * x,                     if x > 0
ELU(x) = alpha * scale * (exp(x * inputScale) - 1),  if x <= 0
```

## 本样例支持的产品及 CANN 软件版本

| 产品 | CANN 软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍
```
├── elu
│   ├── op_kernel
│   │   └── elu.asc              // AscendC C API 实现 & 调用样例（核函数）
│   ├── scripts
│   │   └── gen_data.py          // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt           // 编译工程文件
│   └── README.md                // 样例介绍
```

## 样例描述
- 样例功能：
  计算 ELU 激活函数值，公式见概述。
- 样例规格：
  | 类型 | 值 |
  |------|-----|
  | 样例类型(OpType) | AIV 样例 |
  | 样例输入 | src: [1, 32, 1024, 2048], float / float16 |
  | 样例输出 | dst: [1, 32, 1024, 2048], float / float16 |
  | 核函数名 | elu_custom / elu_custom_fp16 |

## 编译运行
```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py
./demo
```

执行结果如下：
```
test pass!
fp16 test pass!
```