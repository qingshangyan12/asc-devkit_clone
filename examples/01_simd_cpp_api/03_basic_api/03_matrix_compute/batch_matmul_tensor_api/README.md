# Batch Matmul Tensor API带Bias样例

## 概述

本样例介绍基于Tensor API编程方式实现带Bias的Batch Matmul计算，输入矩阵A、矩阵B和Bias的数据类型均为`half`，输出矩阵C的数据类型为`half`。矩阵A和矩阵B均不转置，计算逻辑如下：

```text
C[b] = A[b] * B[b] + Bias[b], b = 0, 1, ..., B - 1
```

本样例通过`MakeTensor`构造GM、L1 Buffer、L0A Buffer、L0B Buffer、L0C Buffer和BiasTable Buffer上的Tensor对象，通过`Slice`按batch维度切分输入输出，并通过Tensor API `Copy`和`Mmad`完成分批矩阵乘加计算。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT</cann-filter> | >= CANN 9.1.0 |

> **说明：** 该样例依赖尚未正式发布的CANN特性，请使用最新的CANN master包。

## 目录结构介绍

```text
├── batch_matmul_tensor_api
│   ├── scripts
│   │   ├── gen_data.py                 // 输入数据和真值数据生成脚本
│   │   └── verify_result.py            // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                  // 编译工程文件
│   ├── data_utils.h                    // 数据读入写出函数
│   ├── batch_matmul_tensor_api.asc     // Ascend C样例实现和调用样例
│   └── README.md                       // 样例说明文档
```

## 样例描述

本样例的数据流如下：

1. GM -> L1 Buffer：调用Tensor API `CopyGM2L1`，按`L1BatchSize`搬入矩阵A、矩阵B和Bias，其中矩阵A、矩阵B从ND格式转换为NZ格式。
2. L1 Buffer -> L0A Buffer/L0B Buffer/BiasTable Buffer：调用Tensor API `CopyL12L0A`、`CopyL12L0B`和`CopyL12BT`，按`L0BatchSize`继续切分并搬入参与计算的数据。
3. Mmad：循环当前`L0BatchSize`中的batch数据，每次计算一对A、B矩阵乘加Bias的结果，并写入L0C Buffer对应位置。
4. L0C Buffer -> GM：调用Tensor API `CopyL0C2GM`，将当前分片的结果矩阵C搬出到GM。

### batch matmul带Bias定义

批量矩阵乘法（batch matmul）是普通矩阵乘法在批次维度（Batch Dimension）上的扩展，核心逻辑是：对一个包含多个矩阵的批次数据，逐一对批次内的矩阵执行标准矩阵乘法，最终输出同批次数量的结果矩阵。

在带Bias场景下，若输入矩阵A、矩阵B和Bias的形状分别为`[B, M, K]`、`[B, K, N]`和`[B, 1, N]`，输出矩阵C的形状为`[B, M, N]`。对任意批次`i`，取`A[i]`和`B[i]`执行普通矩阵乘法，再与`Bias[i]`相加，最终得到`C[i]`。

需要注意的是，不同批次的矩阵之间不会互相计算。

### 样例规格

本样例中输入输出矩阵的规格，如下表1所示。

**表 1**  输入输出的规格

| 输入输出 | 数据类型 | Shape | Format |
|----------|----------|-------|--------|
| 输入矩阵A | half | [B, M, K] | ND |
| 输入矩阵B | half | [B, K, N] | ND |
| Bias | half | [B, 1, N] | ND |
| 输出矩阵C | half | [B, M, N] | ND |

核函数直调样例默认的shape为`B=128`、`M=32`、`K=32`、`N=32`，核函数名为`batch_matmul_tensor_api`。

编译期模板参数默认值，如下表2所示。

**表 2**  编译期模板参数默认值

| 参数 | 默认值 | 说明 |
|------|--------|------|
| B | 128 | batch数量 |
| M | 32 | 矩阵A的行数 |
| K | 32 | 矩阵A的列数、矩阵B的行数 |
| N | 32 | 矩阵B的列数 |
| L1BatchSize | 32 | 每次搬入L1 Buffer参与处理的batch数量 |
| L0BatchSize | 4 | 每次搬入L0A Buffer、L0B Buffer、L0C Buffer参与计算的batch数量 |

### Tensor API Tensor构造

样例通过`MakeTensor`和`MakeFrameLayout`分别描述GM、L1 Buffer、L0A Buffer、L0B Buffer、L0C Buffer和BiasTable Buffer上的Tensor对象。GM上的A、B、C和Bias使用`NDExtLayoutPtn`描述，L1 Buffer上的A、B使用`NZLayoutPtn`描述，L0A Buffer使用`NZLayoutPtn`描述，L0B Buffer使用`ZNLayoutPtn`描述，L0C Buffer使用`NZLayoutPtn`描述。

```cpp
auto gmA = MakeTensor(MakeMemPtr(a), MakeFrameLayout<NDExtLayoutPtn>(B, M, K));
auto gmB = MakeTensor(MakeMemPtr(b), MakeFrameLayout<NDExtLayoutPtn>(B, K, N));
auto gmC = MakeTensor(MakeMemPtr(c), MakeFrameLayout<NDExtLayoutPtn>(B, M, N));
auto gmBias = MakeTensor(MakeMemPtr(bias), MakeFrameLayout<NDExtLayoutPtn>(B, 1, N));

auto l1ATensor = MakeTensor(MakeMemPtr(l1ABuf), MakeFrameLayout<NZLayoutPtn, T>(L1BatchSize, M, K));
auto l1BTensor = MakeTensor(MakeMemPtr(l1BBuf), MakeFrameLayout<NZLayoutPtn, T>(L1BatchSize, K, N));
auto l1BiasTensor = MakeTensor(MakeMemPtr(l1BiasBuf), MakeFrameLayout<NDExtLayoutPtn, T>(L1BatchSize, 1, N));
auto l0ATensor = MakeTensor(MakeMemPtr(l0ABuf), MakeFrameLayout<NZLayoutPtn, T>(L0BatchSize, M, K));
auto l0BTensor = MakeTensor(MakeMemPtr(l0BBuf), MakeFrameLayout<ZNLayoutPtn, T>(L0BatchSize, K, N));
auto l0CTensor = MakeTensor(MakeMemPtr(l0CBuf), MakeFrameLayout<NZLayoutPtn>(L0BatchSize, M, N));
auto l0BiasTensor = MakeTensor(MakeMemPtr(l0BiasBuf), MakeFrameLayout<NDExtLayoutPtn>(L0BatchSize, 1, N));
```

### batch维度切分

样例通过`L1BatchSize`和`L0BatchSize`两级编译期模板参数控制每次进入L1 Buffer以及L0A Buffer、L0B Buffer、L0C Buffer的batch数量。外层循环按`L1BatchSize`从GM侧切分batch数据，内层循环按`L0BatchSize`继续切分到L0A Buffer、L0B Buffer、L0C Buffer侧，避免一次缓存全部batch数据造成片上存储空间超限。

```cpp
for (uint32_t l1batchIndex = BIndexStart; l1batchIndex < BIndexEnd; l1batchIndex += L1BatchSize) {
    uint32_t l1BatchSize = min(L1BatchSize, BIndexEnd - l1batchIndex);
    Copy(copyGM2L1Atom, l1ATensor,
         gmA.Slice(MakeCoord(l1batchIndex, MakeCoord(0, 0)), MakeShape(l1BatchSize, MakeShape(M, K))));
    Copy(copyGM2L1Atom, l1BTensor,
         gmB.Slice(MakeCoord(l1batchIndex, MakeCoord(0, 0)), MakeShape(l1BatchSize, MakeShape(K, N))));
    Copy(copyGM2L1Atom, l1BiasTensor,
         gmBias.Slice(MakeCoord(l1batchIndex, MakeCoord(0, 0)), MakeShape(l1BatchSize, MakeShape(1, N))));

    for (uint32_t l0BatchIndex = 0; l0BatchIndex < l1BatchSize; l0BatchIndex += L0BatchSize) {
        uint32_t l0BatchSize = min(L0BatchSize, l1BatchSize - l0BatchIndex);
        Copy(copyL12L0AAtom, l0ATensor,
             l1ATensor.Slice(MakeCoord(l0BatchIndex, MakeCoord(0, 0)), MakeShape(l0BatchSize, MakeShape(M, K))));
        Copy(copyL12L0BAtom, l0BTensor,
             l1BTensor.Slice(MakeCoord(l0BatchIndex, MakeCoord(0, 0)), MakeShape(l0BatchSize, MakeShape(K, N))));
        Copy(copyL12BTAtom, l0BiasTensor,
             l1BiasTensor.Slice(MakeCoord(l0BatchIndex, MakeCoord(0, 0)), MakeShape(l0BatchSize, MakeShape(1, N))));
    }
}
```

### Mmad带Bias计算

每个batch拥有独立Bias，Bias的shape为`[B, 1, N]`。样例先将Bias构造为Tensor对象，再通过Tensor API `CopyL12BT`搬入BiasTable Buffer，并在调用`Mmad`时作为Bias操作数参与矩阵乘加计算。

```cpp
for (uint32_t l0CBatchIndex = 0; l0CBatchIndex < l0BatchSize; l0CBatchIndex++) {
    Mmad(mmadAtom.with(MmadParams{static_cast<uint16_t>(M), static_cast<uint16_t>(N), static_cast<uint16_t>(K), 0, true}),
        l0CTensor.Slice(MakeCoord(l0CBatchIndex, MakeCoord(0, 0)), MakeShape(1, MakeShape(M, N))),
        l0ATensor.Slice(MakeCoord(l0CBatchIndex, MakeCoord(0, 0)), MakeShape(1, MakeShape(M, K))),
        l0BTensor.Slice(MakeCoord(l0CBatchIndex, MakeCoord(0, 0)), MakeShape(1, MakeShape(K, N))),
        l0BiasTensor.Slice(MakeCoord(l0CBatchIndex, MakeCoord(0, 0)), MakeShape(1, MakeShape(1, N))));
}
```

### 矩阵分片搬出

每次完成当前`L0BatchSize`分片中的矩阵乘加计算后，样例调用Tensor API `CopyL0C2GM`将L0C Buffer上的结果矩阵C搬出到GM。

```cpp
Copy(copyL0C2GMAtom,
     gmC.Slice(MakeCoord(l1batchIndex + l0BatchIndex, MakeCoord(0, 0)),
               MakeShape(l0BatchSize, MakeShape(M, N))),
     l0CTensor);
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  mkdir -p build && cd build;                                                     # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                            # 编译工程，默认NPU模式
  python3 ../scripts/gen_data.py                                                  # 生成测试输入数据
  ./demo                                                                          # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin         # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` |`dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
