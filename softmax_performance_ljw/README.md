# Softmax性能调优样例

## 概述

本样例以单核Softmax为例，介绍基于静态Tensor方式编程的Regbase架构下的编程及性能调优方法。整个过程可以划分以下几个策略：

**优化策略**
- 充分利用指令能力
  - 搬运接口分布模式：随路Pack/Unpack + Cast进行格式转换
  - 指令融合：Exp/Sub -> ExpSub
  - 尾块处理：UpdateMask
- 指令双发 && 硬件循环
  - 循环融合/切分（Load/Store指令对消、冗余同步消除）
  - 循环Unroll
- 算法选择
  - 主尾块处理
  - ReduceSum
- 标量优化
  - 除法、取余外提或改位运算

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── softmax
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── 1go.sh                  // 一键执行脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── softmax.asc                 // Ascend C样例实现（包含7个优化case）
```

## 样例描述

**样例功能**：

  样例实现的是固定shape为128×128的Softmax运算。

  Softmax的计算公式为：

$$
\text{Softmax}(z_i) = \dfrac{e^{z_i}}{\sum_{j=1}^K e^{z_j}}
$$

  - x：输入，形状为[128, 128]，数据类型为float；
  - z：输出，形状为[128, 128]，数据类型为float；

## 样例实现

### 充分利用指令能力

- 搬运接口分布模式：通过LoadDist/StoreDist模板参数开启搬运指令的分布模式如Pack/Unpack/BroadCast/单点写入等；
```cpp
  // 搬入
  AscendC::Reg::LoadAlign<T, AscendC::Reg::LoadDist::DIST_UNPACK_B16>(srcReg, srcAddr + offset);
  AscendC::Reg::Cast<float, T, AscendC::layoutZMrgZ>(dstReg, srcReg, preg);

  // 搬出
  AscendC::Reg::Cast<T, float, AscendC::LayoutZMrgZRndRSatNS>(dstReg, srcReg, preg);
  AscendC::Reg::StoreAlign<T, AscendC::Reg::StoreDist::DIST_PACK_B32>(dstAddr + offset, dstReg, preg);
```

- 指令融合：Exp + Sub -> ExpSub
```cpp
  AscendC::Reg::ExpSub(expReg, srcReg, maxReg, mask);
```

- 尾块处理
```cpp
  mask = AscendC::Reg::UpdateMask<float>(count);
```

- 连续单点写入通过连续非对齐指令（取决于融合方式以及算法选择，此处仅作说明）
``` cpp
// StoreAlign写出数据，单次仅一个有效数据。
for (){
  AscendC::Reg::ReduceSum();
  AscendC::Reg::StoreAlign();
}

// StoreUnalign写出数据，连续写出时数据存放在非对齐寄存器，仅最后一条外提指令写入UB。
for (){
  AscendC::Reg::ReduceSum();
  AscendC::Reg::StoreUnAlign();
}
AscendC::Reg::StoreUnAlignPost();
```

### 尾块处理
  当前示例提供了两种尾块处理方式：UpdateMask及主尾块切分；根据以下代码片段和性能数据进行分析。

```cpp
// UpdateMask接口：每次外层循环需要重新赋值标量，调用UpdateMask传入引用，每次自动更新；
  for (uint32_t i = 0; i < aDim; i++) {
      uint32_t count = rDim;
      AscendC::Reg::Duplicate(maxReg, Custom::fp32MinValue.f, maskFull);
      for (uint16_t j = 0; j < repeatTimes; j++) {
          mask = AscendC::Reg::UpdateMask<float>(count);
          LoadWithCastIfNeed(srcAddr, srcReg, mask, i * rDimAlign + j * oneRepeatSize);
          AscendC::Reg::Max<float, AscendC::Reg::MaskMergeMode::MERGING>(maxReg, maxReg, srcReg, mask);
      }
      AscendC::Reg::ReduceMax(maxReg, maxReg, maskFull);
      AscendC::Reg::Duplicate(maxReg, maxReg, maskFull);
      uint32_t sreg0 = rDim;
      for (uint16_t j = 0; j < repeatTimes; j++) {
          mask = AscendC::Reg::UpdateMask<float>(sreg0);
          LoadWithCastIfNeed(srcAddr, srcReg, mask, i * rDimAlign + j * oneRepeatSize);
          AscendC::Reg::ExpSub(expReg, srcReg, maxReg, mask);
          AscendC::Reg::StoreAlign<float>(expAddr + i * rDimAlign + j * oneRepeatSize, expReg, mask);
      }
  }
```

```cpp
// 主尾块切分
  for (uint32_t i = 0; i < aDim; i++) {
    AscendC::Reg::Duplicate(maxReg, Custom::fp32MinValue.f, maskFull);
    // for (uint16_t j = 0; j < hasTail; j ++) {
    //     LoadWithCastIfNeed(srcAddr, srcReg, maskFull, i * rDimAlign + repeatTimesMain * oneRepeatSize);
    //     AscendC::Reg::Max(srcReg, maxReg, srcReg, maskTail);
    //     AscendC::Reg::Move(maxReg, srcReg, maskTail);
    // }
    for (uint16_t j = 0; j < repeatTimesMain; j++) {
        LoadWithCastIfNeed(srcAddr, srcReg, maskFull, i * rDimAlign + j * oneRepeatSize);
        AscendC::Reg::Max(maxReg, maxReg, srcReg, maskFull);
    }
    AscendC::Reg::ReduceMax(maxReg, maxReg, maskFull);
    AscendC::Reg::Duplicate(maxReg, maxReg, maskFull);
    for (uint16_t j = 0; j < repeatTimesMain; j++) {
        LoadWithCastIfNeed(srcAddr, srcReg, maskFull, i * rDimAlign + j * oneRepeatSize);
        AscendC::Reg::ExpSub(expReg, srcReg, maxReg, maskFull);
        AscendC::Reg::StoreAlign<float>(expAddr + i * rDimAlign + j * oneRepeatSize, expReg, maskFull);
    }
    // for (uint16_t j = 0; j < hasTail; j ++) {
    //     LoadWithCastIfNeed(srcAddr, srcReg, maskTail, i * rDimAlign + repeatTimesMain * oneRepeatSize);
    //     AscendC::Reg::ExpSub(expReg, srcReg, maxReg, maskTail);
    //     AscendC::Reg::StoreAlign<float>(expAddr + i * rDimAlign + repeatTimesMain * oneRepeatSize, expReg, maskTail); 
    // }
  }
```

**表1 尾块处理方式性能比对**
| 字段名 | aiv_vec_time | 折算aiv_vec_cycle |
|:---:|:---|:---|
| UpdateMask模式 | a | a |
| 主尾块切分模式 - 带尾块处理 | a | a |
| 主尾块切分模式 - 不带尾块处理 | a | a |

**表2 AI Core 性能指标字段说明表**
| 字段名 | 字段含义 |
|:---:|:---|
| aiv_vec_time(μs) | vec类型指令（向量类运算指令）耗时，单位μs。 |
| 折算aiv_vec_cycle | vec类型指令（向量类运算指令）的cycle数。 |

  结论：UpdateMask接口代码简洁，但是性能不及主尾块切分场景仅处理主块时的性能。主尾块切分后，尾块For(0)依赖编译器优化为HardWare Loop，当前性能不及理论预期。

### 循环Unroll
  循环展开可以提升指令双发能力，尤其是


### 算法选择


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  bash 1go.sh
  ```

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  error ratio: 0.0000, tolerance:0.0001
  test pass!
  ```
