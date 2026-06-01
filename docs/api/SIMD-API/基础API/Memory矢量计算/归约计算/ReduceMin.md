# ReduceMin

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品 AI Core | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品 Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | √ </cann-filter>|
| <cann-filter npu-type="x90">Kirin X90 | √ </cann-filter>|
| <cann-filter npu-type="9030">Kirin 9030 | √ </cann-filter>|

## 功能说明

头文件路径为：`"basic_api/kernel_operator_vec_reduce_intf.h"`。

`ReduceMin`接口用于从所有输入数据中找出最小值和最小值索引。

`ReduceMin`计算过程如下图所示：首先，在每个repeat迭代中计算得到最小值和repeat内部索引，这些中间结果暂存于`sharedTmpBuffer`工作区中；然后，在中间结果的基础上继续按repeat迭代得到最终的最小值和最小值索引。需要注意的是，每次repeat迭代获取的最小值索引是repeat内部索引，返回最终结果时，需要根据迭代位置和repeat内部索引推导全量数据的最小值索引。

**图 1**  `ReduceMin`计算示意图<a id="fig1"></a>

![ReduceMin计算示意图](../../../../figures/ReduceMin_scale.png "ReduceMin计算示意图")

## 函数原型

- tensor前n个数据计算：

    ```cpp
    template <typename T>
    __aicore__ inline void ReduceMin(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<T>& sharedTmpBuffer, const int32_t count, bool calIndex = 0)
    ```

- tensor高维切分计算：
    - `mask`逐bit模式：

        ```cpp
        template <typename T>
        __aicore__ inline void ReduceMin(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<T>& sharedTmpBuffer, const uint64_t mask[], const int32_t repeatTime, const int32_t srcRepStride, bool calIndex = 0)
        ```

    - `mask`连续模式：

        ```cpp
        template <typename T>
        __aicore__ inline void ReduceMin(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<T>& sharedTmpBuffer, const int32_t mask, const int32_t repeatTime, const int32_t srcRepStride, bool calIndex = 0)
        ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN、VECCALC、VECOUT（存储位置为Unified Buffer）。 |
| src | 输入 | 源操作数。<br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN、VECCALC、VECOUT（存储位置为Unified Buffer）。 |
| sharedTmpBuffer | 输入 | 指令执行期间存储中间结果，用于内部计算所需操作空间，需特别注意空间大小。详情请参考[关键特性说明](#关键特性说明)。<br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN、VECCALC、VECOUT（存储位置为Unified Buffer）。 |
| count | 输入 | 参与计算的元素个数。关于该参数的具体说明请参考[连续计算](../SIMD计算说明/连续计算.md)。最大处理的数据量不能超过UB大小限制。  |
| mask/mask[] | 输入 | `mask`用于控制每次迭代内参与计算的源操作数。详细设置参考[掩码概述](../SIMD计算说明/掩码/概述.md)。 |
| repeatTime | 输入 | 迭代次数。关于该参数的具体描述请参考[高维切分](../SIMD计算说明/高维切分.md)。**注：与高维切分中不同的是，`repeatTime`可以支持更大的取值范围，保证不超过int32_t的最大值即可。** |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的DataBlock数目。取值范围为[0, $2^{16}-1$]。 |
| calIndex | 输入 | 指定是否获取最小值的索引，bool类型，默认值为`false`，取值：<br>&bull; `true`：同时获取最小值和最小值索引。<br>&bull; `false`：不获取索引，只获取最小值。 |

**注：以上高维切分相关参数`mask`，`repeatTime`，`srcRepStride`请参考[高维切分](../SIMD计算说明/高维切分.md)中的介绍。**

## 数据类型

支持的数据类型如下：

- <cann-filter npu-type = "950">Ascend 950PR/Ascend 950DT，支持int16_t、uint16_t、half、int32_t、uint32_t、float、int64_t、uint64_t。</cann-filter>
- <cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持half、float。</cann-filter>
- <cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持half、float。</cann-filter>
- <cann-filter npu-type = "310b">Atlas 200I/500 A2 推理产品，支持half、float。</cann-filter>
- <cann-filter npu-type = "310p">Atlas 推理系列产品 AI Core，支持half、float。</cann-filter>
- <cann-filter npu-type = "910">Atlas 训练系列产品，支持half。</cann-filter>
- <cann-filter npu-type = "x90">Kirin X90，支持half、float。</cann-filter>
- <cann-filter npu-type = "9030">Kirin 9030，支持half、float。</cann-filter>

## 返回值说明

无

## 约束说明

- 源操作数及`sharedTmpBuffer`的地址对齐约束请参考[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)，起始地址需要32字节对齐；目的操作数的起始地址对齐约束请参考[ReduceRepeat-表3](./ReduceRepeat.md#tab3)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../通用说明和约束.md#通用地址重叠约束)。
- 需要使用`sharedTmpBuffer`的情况下，支持`dst`与`sharedTmpBuffer`地址重叠（通常情况下`dst`比`sharedTmpBuffer`所需的空间要小），此时`sharedTmpBuffer`必须满足所需空间要求，详情请参考[关键特性说明](#关键特性说明)。
- <cann-filter npu-type = "A3,910b"><cann-filter npu-type = "A3">针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、</cann-filter><cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>，当`repeatTime=0`时，不会执行归约操作，不会对目的操作数进行写入，该接口将被视为`NOP`（空操作）。</cann-filter>
- <cann-filter npu-type = "A3,910b"><cann-filter npu-type = "A3">针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、</cann-filter><cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品</cann-filter>，当`mask=0`时，不会执行归约操作，不会对目的操作数进行写入，该接口将被视为`NOP`（空操作）。</cann-filter>
- `srcRepStride`取值范围为[0, $2^{16}-1$]，需要结合UB的实际大小避免出现越界。
- 如果存在多个最小值，该指令会将最小索引写入目的操作数。
- `dst`结果存储顺序为最小值，最小值索引，若不需要索引，只会存储最小值。
- 当输入类型是`half`的时候，只支持获取最大不超过65535（`uint16_t`能表示的最大值）的索引值。
- 索引按操作数的数据类型存储，读取索引需要将类型转换到整型。请参考[ReduceRepeat关键特性说明](./ReduceRepeat.md#关键特性说明)。
- <cann-filter npu-type = "950">针对Ascend 950PR/Ascend 950DT，`int64_t`/`uint64_t`数据类型仅支持tensor前n个数据计算接口。</cann-filter>
- <cann-filter npu-type = "950">对于Ascend 950PR/Ascend 950DT，因接口内部算法实现不同，无需使用`sharedTmpBuffer`，可以直接传入`src`或者任意大小的`sharedTmpBuffer`。</cann-filter>

<cann-filter npu-type = "A3,910b,310b,310p,910,x90,9030">

- 对于<cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品、</cann-filter><cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品、</cann-filter><cann-filter npu-type = "310b">Atlas 200I/500 A2 推理产品、</cann-filter><cann-filter npu-type = "310p">Atlas 推理系列产品 AI Core、</cann-filter><cann-filter npu-type = "910">Atlas 训练系列产品、</cann-filter><cann-filter npu-type = "x90">Kirin X90、</cann-filter><cann-filter npu-type = "9030">Kirin 9030</cann-filter>，需要使用`sharedTmpBuffer`。

</cann-filter>

## 关键特性说明

- **索引值需要强制类型转换**，详情请参考[`ReduceRepeat`关键特性说明](./ReduceRepeat.md#关键特性说明)。

- **`sharedTmpBuffer`最小所需空间设置**：与`ReduceMax`类似，详情请参考[ReduceMax关键特性说明](./ReduceMax.md#关键特性说明)。

## 调用示例

详细示例请参考[ReduceMin样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/reduce_computation)。

- tensor高维切分计算样例-`mask`连续模式：

    ```cpp
    // dstLocal,srcLocal和sharedTmpBuffer均为half类型,srcLocal的计算数据量为8320,并且连续排布，需要索引值，使用tensor高维切分计算接口，设定repeatTime为65，mask为全部元素参与计算
    int32_t mask = 128;
    AscendC::ReduceMin<half>(dstLocal, srcLocal, sharedTmpBuffer, mask, 65, 8, true);
    ```

- tensor高维切分计算样例-`mask`逐bit模式：

    ```cpp
    // dstLocal,srcLocal和sharedTmpBuffer均为half类型,srcLocal的计算数据量为8320,并且连续排布，需要索引值，使用tensor高维切分计算接口，设定repeatTime为65,mask为全部元素参与计算
    uint64_t mask[2] = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };
    AscendC::ReduceMin<half>(dstLocal, srcLocal, sharedTmpBuffer, mask, 65, 8, true);
    ```

- tensor前n个数据计算样例：

    ```cpp
    // dstLocal,srcLocal和sharedTmpBuffer均为half类型,srcLocal的计算数据量为8320,并且连续排布，需要索引值，使用tensor前n个数据计算接口
    AscendC::ReduceMin<half>(dstLocal, srcLocal, sharedTmpBuffer, 8320, true);
    ```

- <a name="li0437172884019"></a>tensor高维切分计算接口完整调用示例：

    ```cpp
    #include "kernel_operator.h"

    int srcDataSize = 512;
    int dstDataSize = 512;
    int mask = 128;
    int repStride = 8;
    int repeat = srcDataSize / mask;

    // 初始化srcLocal 、dstLocal 、sharedTmpBuffer
    AscendC::LocalTensor<half> srcLocal = inQueueSrc.DeQue<half>();
    AscendC::LocalTensor<half> dstLocal = outQueueDst.AllocTensor<half>();
    AscendC::LocalTensor<half> sharedTmpBuffer = workQueue.AllocTensor<half>();

    // mask为128一次计算128个元素,4次repeat计算完512个数,calIndex为true，获取最小值的索引
    AscendC::ReduceMin<half>(dstLocal, srcLocal, sharedTmpBuffer, mask, repeat, repStride, true);
    // 释放Tensor
    outQueueDst.EnQue<half>(dstLocal);
    inQueueSrc.FreeTensor(srcLocal);
    workQueue.FreeTensor(sharedTmpBuffer);
    ```

    示例结果如下：

    ```cpp
    // 输入数据(src_gm)：
    [0.769    0.8584   0.1082   0.2715   0.1759   0.7646   0.6406   0.2944   0.4255   0.927    0.8022   0.04507  0.9688   0.919    0.3008   0.7144   0.3206   0.6753   0.8276
     0.3374   0.4636   0.3591   0.112    0.93     0.822    0.7314   0.01165  0.31     0.5586   0.2808   0.3997   0.04544  0.0931   0.8438   0.612    0.03052  0.3652   0.1153
     0.06213  0.12103  0.4421   0.8003   0.1583   0.845    0.125    0.6934   0.4592   0.871    0.573    0.4133   0.885    0.6875   0.2854   0.7007   0.1294   0.2092   0.3794
     0.7534   0.5923   0.03888  0.2412   0.8584   0.6704   0.429    0.77     0.427    0.6323   0.524    0.0519   0.514    0.2408   0.09357  0.1702   0.3694   0.665    0.2651
     0.9507   0.661    0.459    0.1317   0.7334   0.289    0.0325   0.1187   0.6626   0.2769   0.3083   0.923    0.826    0.7275   0.976    0.4854   0.724    0.7783   0.8022
     0.677    0.2401   0.377    0.839    0.2297   0.54     0.743    0.511    0.1346   0.7183   0.4775   0.3442   0.561    0.2935   0.04065  0.1001   0.753    0.6816   0.8955
     0.07324  0.5947   0.508    0.2229   0.468    0.3135   0.0898   0.5625   0.7407   0.803    0.1071   0.6724   0.797    0.8296   0.807    0.8604   0.7437   0.967    0.4307
     0.3833   0.03394  0.02478  0.9385   0.3105   0.43     0.0706   0.4363   0.05832  0.0812   0.2418   0.03967  0.557    0.2705   0.963    0.8125   0.342    0.8853   0.3047
     0.7197   0.7173   0.02887  0.7695   0.4304   0.691    0.4285   0.9917   0.3994   0.19     0.3984   0.1888   0.83     0.0644   0.9766   0.857    0.09784  0.831    0.224
     0.8228   0.8975   0.1775   0.725    0.882    0.7188   0.3257   0.05347  0.1026   0.05902  0.9697   0.445    0.728    0.626    0.3577   0.711    0.2343   0.3865   0.03888
     0.3318   0.855    0.891    0.3647   0.9297   0.5083   0.7163   0.5737   0.2155   0.804    0.2118   0.525    0.1116   0.558    0.05203  0.6343   0.5796   0.5605   0.449
     0.4475   0.3713   0.3708   0.11017  0.2048   0.087    0.265    0.937    0.933    0.4683   0.5884   0.4312   0.9326   0.839    0.592    0.566    0.4229   0.05493  0.4578
     0.353    0.2915   0.8345   0.888    0.8394   0.8774   0.3582   0.2913   0.798    0.87     0.3372   0.6914   0.9185   0.4368   0.3276   0.8125   0.782    0.885    0.6543
     0.1626   0.0965   0.8247   0.03952  0.459    0.5596   0.694    0.59     0.02153  0.3762   0.2428   0.9727   0.3672   0.732    0.2676   0.2102   0.128    0.5957   0.988
     0.583    0.9097   0.144    0.3845   0.2151   0.327    0.2925   0.974    0.771    0.9224   0.147    0.6206   0.1774   0.1415   0.7637   0.573    0.9736   0.183    0.837
     0.0753   0.098    0.8184   0.08527  0.889    0.528    0.2207   0.1852   0.5903   0.594    0.04865  0.5806   0.6006   0.2048   0.4934   0.1302   0.7217   0.949    0.04105
     0.6875   0.3975   0.845    0.6045   0.4077   0.01927  0.1505   0.4407   0.8457   0.9614   0.4504   0.7134   0.07837  0.3557   0.521    0.545    0.02188  0.581    0.3215
     0.4458   0.853    0.4656   0.928    0.2927   0.3467   0.3516   0.1686   0.88     0.1509   0.2993   0.4006   0.611    0.1251   0.0887   0.896    0.2651   0.5596   0.0359
     0.6895   0.3494   0.871    0.673    0.1486   0.7812   0.0925   0.434    0.09985  0.02402  0.2932   0.01034  0.744    0.6357   0.658    0.1487   0.3416   0.1171   0.3088
     0.557    0.837    0.10944  0.7036   0.9097   0.3706   0.73     0.2844   0.78     0.5117   0.5537   0.776    0.6553   0.128    0.3184   0.8022   0.686    0.1785   0.2212
     0.74     0.8955   0.4773   0.6084   0.7827   0.239    0.4849   0.1816   0.2854   0.166    0.012505 0.4421   0.2179   0.06094  0.2124   0.409    0.641    0.1841   0.776
     0.4685   0.2334   0.4094   0.3447   0.6836   0.434    0.10516  0.514    0.8345   0.371    0.8555   0.5396   0.844    0.7554   0.171    0.749    0.7344   0.05936  0.4482
     0.9873   0.3137   0.7627   0.871    0.5503   0.956    0.2607   0.0904   0.535    0.3079   0.762    0.793    0.545    0.889    0.8936   0.6094   0.6533   0.5737   0.945
     0.4434   0.2686   0.05872  0.0776   0.0915   0.5386   0.6777   0.3164   0.8955   0.3398   0.3801   0.3784   0.3904   0.4849   0.816    0.962    0.335    0.705    0.1871
     0.3643   0.7163   0.6484   0.4526   0.8096   0.2408   0.608    0.0215   0.7246   0.412    0.609    0.03342  0.653    0.0424   0.672    0.627    0.3025   0.9424   0.3784
     0.1012   0.4192   0.7695   0.7383   0.9395   0.06494  0.3027   0.11523  0.6035   0.1727   0.4048   0.932    0.4053   0.3528   0.8193   0.0355   0.01953  0.574    0.509
     0.1443   0.0848   0.568    0.8716   0.968    0.613    0.535    0.0389   0.84     0.0655   0.127    0.06104  0.526    0.504    0.4175   0.8027   0.482    0.304   ]
    // 输出数据(dst_gm)：
    [0.01034,  2.104e-05] // 2.104e-05需要使用reinterpret_cast方法转换得到索引值353
    ```

- tensor前n个数据计算接口完整调用示例：

    ```cpp
    #include "kernel_operator.h"

    int srcDataSize = 288;
    // 初始化srcLocal 、dstLocal 、sharedTmpBuffer
    AscendC::LocalTensor<half> srcLocal = inQueueSrc.DeQue<half>();
    AscendC::LocalTensor<half> dstLocal = outQueueDst.AllocTensor<half>();
    AscendC::LocalTensor<half> sharedTmpBuffer = workQueue.AllocTensor<half>();

    // level2接口计算前288个数，calIndex为true，获取最小值的索引
    AscendC::ReduceMin<half>(dstLocal, srcLocal, sharedTmpBuffer, srcDataSize, true);
    // 释放Tensor
    outQueueDst.EnQue<half>(dstLocal);
    inQueueSrc.FreeTensor(srcLocal);
    workQueue.FreeTensor(sharedTmpBuffer);
    ```

    示例结果如下：

    ```cpp
    // 示例结果
    // 输入数据(src_gm)：
    [0.556    0.5225   0.3623   0.214    0.556    0.0643   0.769    0.594    0.261    0.3652   0.911    0.924    0.386    0.3696   0.2296   0.5957   0.1709   0.79     0.8516
     0.341    0.705    0.728    0.8135   0.7534   0.5874   0.771    0.05835  0.7456   0.1049   0.3105   0.1729   0.9253   0.8003   0.918    0.5005   0.7744   0.688    0.6807
     0.1456   0.4136   0.1055   0.12054  0.275    0.3848   0.08405  0.3843   0.3218   0.6904   0.878    0.3706   0.3586   0.3518   0.429    0.7275   0.6123   0.8096   0.563
     0.54     0.8857   0.8594   0.4143   0.525    0.2744   0.1376   0.382    0.6406   0.1534   0.134    0.2993   0.365    0.8843   0.2986   0.00393  0.6577   0.313    0.8164
     0.8706   0.7686   0.873    0.3286   0.03787  0.8145   0.4656   0.66     0.1362   0.1075   0.1376   0.9097   0.9214   0.833    0.3657   0.8438   0.006973 0.2408   0.801
     0.1862   0.864    0.8745   0.1805   0.4324   0.8647   0.844    0.8936   0.8496   0.311    0.0334   0.3967   0.579    0.43     0.2332   0.5366   0.3557   0.3542   0.945
     0.9336   0.252    0.4375   0.9727   0.859    0.6294   0.6787   0.8887   0.1884   0.524    0.787    0.04755  0.3984   0.0508   0.4065   0.716    0.3184   0.21     0.10645
     0.7544   0.2827   0.7856   0.4878   0.5903   0.12146  0.6426   0.8438   0.063    0.7617   0.6396   0.1995   0.6475   0.1464   0.7617   0.514    0.3506   0.2708   0.8643
     0.1204   0.04337  0.21     0.528    0.0644   0.2133   0.0643   0.0125   0.602    0.654    0.866    0.225    0.9473   0.408    0.4597   0.2793   0.11145  0.293    0.04156
     0.7705   0.3555   0.3977   0.7485   0.76     0.9824   0.2832   0.1239   0.4915   0.878    0.5986   0.7217   0.832    0.6206   0.6455   0.0639   0.772    0.01854  0.7437
     0.1962   0.485    0.5483   0.414    0.9253   0.2452   0.2942   0.9478   0.879    0.586    0.659    0.635    0.7197   0.933    0.08905  0.02892  0.74     0.499    0.02054
     0.2241   0.5137   0.8325   0.185    0.6196   0.949    0.935    0.5605   0.04108  0.3672   0.5566   0.3958   0.4565   0.8135   0.3015   0.46     0.1196   0.5044   0.54
     0.05203  0.687    0.8525   0.501    0.3464   0.307    0.804    0.0926   0.202    0.999    0.955    0.581    0.06216  0.271    0.9365   0.854    0.4202   0.269    0.985
     0.04547  1.       0.1208   0.5225   0.00935  0.4128   0.644    0.3826   0.6963   0.2942   0.007626 0.7144   0.609    0.3206   0.694    0.393    0.6265   0.6904   0.2487
     0.9478   0.798    0.891    0.8867   0.9414   0.395    0.11285  0.515    0.919    0.013855 0.749    0.5527   0.465    0.451    0.1458   0.59     0.893    0.0146   0.062
     0.06604  0.934    0.2242  ]
    // 输出数据(dst_gm)：
    [0.00393,  4.3e-06] // 4.3e-06需要使用reinterpret_cast方法转换得到索引值72
    ```
