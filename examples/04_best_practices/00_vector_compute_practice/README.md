# Add性能调优样例

## 概述

本样例以加法为例，介绍基于静态Tensor方式编程的性能调优方法。整个调优过程分为八个步骤（case 0-7），逐步展示从标量运算到向量运算、从单核到多核、从基础实现到深度优化的完整调优路径。

**优化路径**：
- Case 0: 单核标量版本（基准）
- Case 1: 单核向量版本
- Case 2-4: 多核均匀切分 + 不同的数据搬运量
- Case 5: 多核均匀切分 + 双缓冲优化
- Case 6: 多核均匀切分 + 双缓冲+L2Cache bypass
- Case 7: 多核均匀切分 + 双缓冲+L2Cache bypass+避免Bank Conflict

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── 00_vector_compute_practice
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── add.asc                 // Ascend C算子实现（包含8个优化case）
```

## 样例描述

- **样例功能**：

  样例实现的是固定shape为8192×8192的两个矩阵相加。

  Add的计算公式为：

  ```python
  z = x + y
  ```

  - x：输入，形状为[8192, 8192]，数据类型为half；
  - y：输入，形状为[8192, 8192]，数据类型为half；
  - z：输出，形状为[8192, 8192]，数据类型为half；

**表1 AI Core 性能指标字段说明表**
| 字段名 | 字段含义 |
|:---:|:---|
|Task Duration(μs)|Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。|
|aiv_time|Task在AI Vector Core上的理论执行时间，单位为μs。|
| aiv_vec_time(us) | vec类型指令（向量类运算指令）耗时，单位us。Atlas 200I/500 A2 推理产品 不支持该字段，给予默认值N/A。 |
| aiv_vec_ratio | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比。Atlas 200I/500 A2 推理产品 不支持该字段，给予默认值N/A。 |
| aiv_scalar_time(us) | scalar类型指令（标量类运算指令）耗时，单位us。 |
| aiv_scalar_ratio | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比。 |
| aiv_mte2_time(us) | mte2类型指令（DDR->AICORE搬运类指令）耗时，单位us。 |
| aiv_mte2_ratio | mte2类型指令（DDR->AICORE搬运类指令）的cycle数在total cycle数中的占用比。 |
| aiv_mte3_time(us) | mte3类型指令（AICORE->DDR搬运类指令）耗时，单位us。 |
| aiv_mte3_ratio | mte3类型指令（AICORE->DDR搬运类指令）的cycle数在total cycle数中的占用比。 |

## 样例实现

### Case 0: 单核标量版本（基准程序）

**实现方式**：`ProcessScalar()`

基准程序实现了`half`类型的两组输入数据的加法，采用`for`循环`scalar`运算的方式进行计算。

**关键代码**：
```cpp
for (uint32_t i = 0; i < curLen; i++) {
    float xVal = (float)xLocal.GetValue(i);
    float yVal = (float)yLocal.GetValue(i);
    zLocal.SetValue(i, (half)(xVal + yVal));
}
```

**配置**：
- 单核标量运算
- `dataCopyLen = 4096` 为每次搬运的数据量元素个数
- 单次搬运的数据量为 8192 Byte，单次scalar处理的数据量为4 Byte

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 1239712.68 | 1239712.15 | 0.015 | 0% | 1233761.015 | 99.5% | 5876.827 | 0.5% | 2408.051 | 0.2% |

**分析**：
- 受限于标量运算的性能，单核标量执行加法运算时间很长（1239712.68μs）
- 标量指令占比99.5%，向量指令占比接近0
- 该场景仅作为向量运算的性能对比样例，不建议用户使用

---

### Case 1: 单核向量版本

**实现方式**：`Process()`

将标量运算转换为向量运算，每次参与计算的向量长度为`dataCopyLen`个数据，大幅提升计算效率。

**关键代码**：
```cpp
AscendC::Add(zLocal, xLocal, yLocal, dataCopyLen);
```

**配置**：
- 单核运算
- `dataCopyLen = 4096` 为每次切分的数据量元素个数
- 单次搬运的数据量为 8192 Byte，单次Add处理的数据量为16384 Byte


**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 6854.9 | 6854.37 | 761.65 | 11.1% | 315.83 | 4.6% | 5954.701 | 86.9% | 2501.03 | 36.5% |

**优化效果**：

- 相比Case 0，端到端性能提升约 **181倍**（1239712.68μs → 6854.9μs）
- 标量运算 `aiv_scalar_time / aiv_time` 占比从 99.52% 降至 4.6%
- 向量指令占比提升到11.1%，可以充分利用带宽

由此可得第一个性能优化点：
    
    > Tips : 尽量避免使用scalar指令的运算。

**待改进点**：
- 数据搬运（mte2）占比高达86.9%，成为主要瓶颈。

---

### Case 2: 多核均匀切分

**实现方式**：`Process()`

开启多核并行计算，将8192×8192的矩阵切分到多个AIV Core上并行处理，采用均匀切分策略。

**配置**：
- 行方向切分6份，列方向切分8份，将数据均匀切分至48个核运算
- `dataCopyLen = 4096` 为每次切分的数据量元素个数
- 单次搬运的数据量为 8192 Byte，单次Add处理的数据量为16384 Byte

**关键代码**：
```cpp
// 均匀切分计算每个核处理的行列数
uint32_t baseCoreN = totalN / splitN;
uint32_t remainderN = totalN % splitN;
if (blockIdxN < remainderN) {
    actualCoreN = baseCoreN + 1;  // 均匀分配余数
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 308.98 | 303.47 | 15.897 | 0.052 | 6.987 | 0.023 | 221.218 | 0.729 | 54.823 | 0.181 |

**优化效果**：
- 通过多核并行，端到端任务执行时间减少了95.47%，性能得到了大幅提升

由此可以得到第二个性能优化点：
    
    > Tips: 充分利用芯片性能，采取多核并行策略；同时为了达到负载均衡，建议用户使用均匀切分策略，即每个核处理的数据量相似。

**待改进点**：
- 数据搬运（mte2）占比高达72.9%，mte3 占比18.1%, 可见计算速度快于搬运， 造成了带宽资源浪费。
---

### Case 3: 多核均匀切分 + 大块数据搬运

**实现方式**：`Process()`

为了充分利用带宽资源，增大搬运指令的数据量。

**配置**：
- 行方向切分6份，列方向切分8份，将数据均匀切分至48个核运算
- `dataCopyLen = 16384` 为每次切分的数据量元素个数（4倍于Case2）
- 单次搬运的数据量为 32678 Byte，单次Add处理的数据量为65536 Byte

**性能数据**：

| Task Duration(μs)  | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 272.02 | 267 | 12.853 | 0.048 | 4.838 | 0.018 | 187.364 | 0.702 | 56.398 | 0.211 |

**优化效果**：
- 相比于case2，更大的dataCopyLen可以减少搬运次数，提升带宽利用率
- mte2的占比下降，且mte3的占比上升

由此可得以下的优化建议：
    
    > Tips: 尽量使用访存指令加载连续的大块数据，避免带宽资源的浪费。

---

### Case 4: 多核优化切分 + 大块数据搬运

**实现方式**：`Process()`

尝试在case4的基础上，增大单次搬运数据量。

**配置**：
- 行方向切分6份，列方向切分8份，将数据均匀切分至48个核运算
- `dataCopyLen = 16512` 为每次切分的数据量元素个数
- 单次搬运的数据量为 33024 Byte，单次Add处理的数据量为 66048 Byte

**性能数据**：

| Task Duration(μs)  | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 274 |265.57 | 12.852 | 0.048 | 5.184 | 0.02 | 186.451 | 0.702 | 55.063 | 0.207 |

**优化效果**：
- 相比于case3，单次搬运的数据量增大并没有带来性能提升，反而得到了劣化

由此可得：
    
    > Tips：单次搬运的数据量并不是越大越好，需要合理配置。


---

### Case 5: 双缓冲优化

**实现方式**：`ProcessDoubleBuffer()`

采用双缓冲（Double Buffer）技术，实现数据搬运与计算的流水线并行，隐藏内存访问延迟。

**关键代码**：
```cpp
// Ping-Pong双缓冲
AscendC::LocalTensor<half> xPing(..., xAddrPing, ...);
AscendC::LocalTensor<half> xPong(..., xAddrPong, ...);

// 双缓冲流水线
for (uint32_t blockIdx = 0; blockIdx < totalBlocks; blockIdx += 2) {
    // Ping buffer处理第0块
    AscendC::DataCopy(xPing, xGm[...], curLen0);  // MTE2搬运
    AscendC::Add(zPing, xPing, yPing, curLen0);   // V计算
    
    // Pong buffer处理第1块（与前一块并行）
    AscendC::DataCopy(xPong, xGm[...], curLen1);  // MTE2搬运
    AscendC::Add(zPong, xPong, yPong, curLen1);   // V计算
}
```

**配置**：
- 行方向切分6份，列方向切分8份，将数据均匀切分至48个核运算
- `dataCopyLen = 16384` 为每次切分的数据量元素个数
- 单次搬运的数据量为 32678 Byte，单次Add处理的数据量为65536 Byte
- 将待处理的数据一分为二，由此数据的进出搬运和Vector计算实现并行执行

**内存布局**：

TODO： 待配图
```
UB内存分配（双缓冲）：
┌─────────────┬
│  xPing      │
│  16384*2B   │
└─────────────┴
│  yPing      │
│  16384*2B   │
└─────────────┴
│  zPing      │
│  16384*2B   │
└─────────────┴
│  xPong      │
│  16384*2B   │
└─────────────┴
│  yPong      │
│  16384*2B   │
└─────────────┴
│  zPong      │
│  16384*2B   │
└─────────────┴
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 263.18 | 256.75 | 12.853 | 0.05 | 4.775 | 0.019 | 247.337 | 0.963 | 86.266 | 0.336 |

**优化效果**：
- 双缓冲实现MTE2（搬运）和V（计算）的流水线并行,性能得到了3.2%的优化
- 充分利用硬件流水线能力

由此可得：

    > Tips: 开启双缓冲，将单核内的访存与搬运做到并行，可以充分利用硬件的流水线能力，获得收益。
---

### Case 6: 双缓冲 + L2 Cache绕过

**实现方式**：`ProcessDoubleBufferL2Bypass()`

在双缓冲基础上，对于只需要载入一次的数据量可以设置绕过L2 Cache，直接从HBM载入到AICORE内部。

**关键代码**：
```cpp
// 初始化时设置L2 Cache绕过
if (enableL2Bypass) {
    xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
    yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
}
```

**配置**：
- 行方向切分6份，列方向切分8份，将数据均匀切分至48个核运算
- `dataCopyLen = 16384` 为每次切分的数据量元素个数
- 单次搬运的数据量为 32678 Byte，单次Add处理的数据量为65536 Byte
- 将待处理的数据一分为二，由此数据的进出搬运和Vector计算实现并行执行

**L2 Cache策略**：
- xGm：禁用L2 Cache（一次性读取）
- yGm：禁用L2 Cache（一次性读取）

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 184.32 | 180.51 | 12.852 | 0.071 | 4.703 | 0.026 | 170.953 | 0.947 | 65.124 | 0.361 |

**优化效果**：
- 绕过L2 Cache避免Cache污染
- 对于流式访问（只读/只写一次的数据）特别有效

由此可得以下性能优化建议：

    > Tips: 对于流式访问（只读/只写一次的数据），绕过L2 Cache可以得到性能收益。
---

### Case 7: 双缓冲 + 避免Bank Conflict

**实现方式**：`ProcessDoubleBufferBankConflictV2()`

在双缓冲基础上，优化内存地址布局，避免UB（Unified Buffer）的Bank Conflict，实现最优性能。

**关键代码**：
```cpp
// 优化后的地址布局（避免Bank Conflict）
    static constexpr uint32_t xAddrPingBC = 0;
    static constexpr uint32_t yAddrPingBC = BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t xAddrPongBC = MAX_DATA_COPY_LEN * sizeof(half) * 2;
    static constexpr uint32_t yAddrPongBC = xAddrPongBC + BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
    static constexpr uint32_t zAddrPingBC = MAX_DATA_COPY_LEN * sizeof(half) * 4;
    static constexpr uint32_t zAddrPongBC = zAddrPingBC + BANK_CONFLICT_DATA_COPY_LEN * sizeof(half);
```

**配置**：
- 行方向切分6份，列方向切分8份，将数据均匀切分至48个核运算
- `dataCopyLen = 16256` 为每次切分的数据量元素个数
- 单次搬运的数据量为 32678 Byte，单次Add处理的数据量为65536 Byte
- 将待处理的数据一分为二，由此数据的进出搬运和Vector计算实现并行执行

**内存布局优化**：
```
UB内存分配（避免Bank Conflict）：
┌──────────────┬──────┬──────────────┬──────┬──────────────┬──────┐
│  xPing       │ pad  │  yPing       │ pad  │  xPong       │ pad  │
│  8192*2B     │ 256B │  8192*2B     │ 256B │  8192*2B     │ 256B │
└──────────────┴──────┴──────────────┴──────┴──────────────┴──────┘
         ↑ 偏移避免Bank Conflict

输出区域从0x20000开始，避免与输入区域冲突：
┌──────────────┬──────────────┐
│  zPing       │  zPong       │
│  8192*2B     │  8192*2B     │
└──────────────┴──────────────┘
```

**Bank Conflict原理**：
- UB分为多个Bank Group，同时读读/写写同一Bank group会导致冲突
- 通过减小DatacopyLen，使数据的起始地址偏移，确保vec指令一拍访存的数据在不同的Bank Group

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 184.7 | 180.99 | 7.012 | 0.039 | 10.164 | 0.056 | 172.063 | 0.951 | 52.144 | 0.288 |

**优化效果**：
- 相比于case6，aiv_vec_time提升了54.6%，完全避免UB Bank Conflict
- 由于当前的样例中主要的性能瓶颈点在于MTE2（占比高达95.1%），故端到端性能没有得到收益

由此可得以下性能优化建议：

    > Tips: 在实际场景中，遇到vector bound的情况可以使用ub bank优化手段来解决。


---

## 性能对比总结

| Case | 优化策略 | 核数 | dataCopyLen | Task Duration(μs) | 相对基准提升 |
|:---:|:---|:---:|:---:|:---:|:---:|
| 0 | 单核标量（基准） | 1 | 4096 | 1239712.68 | 1x |
| 1 | 单核向量 | 1 | 4096 | 6854.9 | 181x |
| 2 | 多核均匀切分 | 48 | 4096 | 308.98 | - |
| 3 | 多核均匀切分 | 48 | 16384 | 272.02 | - |
| 4 | 多核优化切分 | 48 | 16512 | 274 | - |
| 5 | 双缓冲 | 48 | 16384 | 263.18 | - |
| 6 | 双缓冲+L2 Cache绕过 | 48 | 16384 | 184.32 | - |
| 7 | 双缓冲+避免Bank Conflict | 48 | 16256 | 184.7 | - |

**优化要点总结**：
1. **标量→向量**：性能提升180倍以上（Case 0→1）
2. **单核→多核**：利用多核并行，理论上接近线性加速
3. **双缓冲**：实现搬运与计算流水线并行
4. **L2 Cache优化**：避免Cache污染，提升带宽利用率
5. **Bank Conflict优化**：优化内存布局，避免Bank冲突

---

## 编译运行

### 切换Case

在 `add.asc` 的 `main` 函数中修改 `caseType` 变量：

```cpp
int32_t main(int32_t argc, char* argv[])
{
    uint32_t caseType = 7;  // 修改此处切换不同case（0-7）
    // ...
}
```

### 编译执行

在本样例根目录下执行如下步骤，编译并执行算子：

- **配置环境变量**  
  请根据当前环境上CANN开发套件包的[安装方式](../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
    
- **样例执行**
  ```bash 
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

### 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof --output=./prof_result ./demo
```

查看性能分析结果：
```bash
# 查看Task Duration
cat ./prof_result/*/summary/*.csv

# 查看详细的指令耗时
cat ./prof_result/*/device_*/*.csv
```

## 参考文档

- [Ascend C编程指南](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2alpha003/apiref/appdevg/topics/topic_0000001848969504.html)
- [性能优化最佳实践](../../README.md)
- [双缓冲技术](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2alpha003/apiref/appdevg/topics/topic_0000001806899122.html)
- [Bank Conflict优化](../01_bank_conflict/README.md)
