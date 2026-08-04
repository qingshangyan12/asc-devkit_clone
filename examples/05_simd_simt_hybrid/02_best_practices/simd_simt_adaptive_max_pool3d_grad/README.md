# SIMD与SIMT混合编程实现adaptive_max_pool3d_grad算子样例

## 概述

本样例以adaptive_max_pool3d_grad算子为例，对比SIMD、混合编程两种实现方式的复杂度和性能数据，说明在连续和离散读写并存的场景中，混合编程可以兼顾性能和开发体验。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── simd_simt_adaptive_max_pool3d_grad
│   ├── figures                              // README中的图片资源。
│   ├── adaptive_max_pool3d_grad_common.h    // 四个Case共用的流水同步等待工具函数。
│   ├── adaptive_max_pool3d_grad_hybrid_ub.h // SIMD清零和SIMT静态UB回填实现。
│   ├── adaptive_max_pool3d_grad_hybrid.h    // SIMD清零和SIMT GM回填实现。
│   ├── adaptive_max_pool3d_grad_scalar.h    // SIMD清零和Scalar GM回填实现。
│   ├── adaptive_max_pool3d_grad_simd.h      // SIMD向量API实现清零和回填。
│   ├── adaptive_max_pool3d_grad_host.asc    // 统一main入口和Host侧运行逻辑。
│   ├── CMakeLists.txt                       // 编译工程文件。
│   ├── README.md                            // 中文样例说明。
│   └── README_en.md                         // 英文样例说明。
```

## 样例描述

**算子介绍**

adaptive_max_pool3d_grad根据`argmax`给出的下标，将梯度`grad`回填到`output`的对应位置，`argmax`范围之外的位置则填0。如下图所示：

**图 1**  adaptive_max_pool3d_grad算子功能示意图
![](./figures/adaptive_max_pool3d_grad_function_overview.png)

计算公式可以理解为：
```text
output[nc, :] = 0                                       # 将output的所有位置置为0。
output[nc, argmax[nc, d, h, w]] = grad[nc, d, h, w]     # 根据argmax将grad的值填到对应的位置。
```

该算子的特点是包含清零和填充离散数据两个步骤，清零是典型的规整计算，适合使用SIMD完成，离散写入梯度则适合SIMT实现。

**样例规格：**

| 参数 | shape | 数据格式 |数据类型| 描述 |
|:---|:---|:---|:---|:---|
| `output` | [1,4096,32,32,32] | NCDHW |FLOAT| 存储计算结果的张量。 |
| `grad` | [1,4096,N,N,N] | NCDHW |FLOAT| 记录了梯度值。样例中，N的取值范围是1/2/4/8/16。 |
| `argmax` | [1,4096,N,N,N] | NCDHW |INT32| 记录了`grad`中每个梯度要填在`output`中的索引位置，样例中取值范围是[0,32767]，shape和`grad`一致。 |

**Case概览**

样例中使用了4种方式实现该算子，以此来对比既有离散又有规整计算场景不同实现的性能差异。

| Case | 运行参数| 实现文件 |线程块数量|每个线程块的线程数 | 对照目的 |
|---|---|---|---|---|---|
| 0 | `simd` | `adaptive_max_pool3d_grad_simd.h` |64|-| 展示纯SIMD实现算子功能。 |
| 1 | `scalar` | `adaptive_max_pool3d_grad_scalar.h` |64|-| 展示用SIMD实现清零和Scalar处理离散写操作的实现方案。 |
| 2 | `hybrid` | `adaptive_max_pool3d_grad_hybrid.h` |64|512| 展示用SIMD实现清零和SIMT实现离散梯度回填的混合方式。 |
| 3 | `hybrid_ub` | `adaptive_max_pool3d_grad_hybrid_ub.h` |64|512| 展示在Case 2基础上融入UB的实现方式。 |

## 样例实现

### 性能指标说明

后续各Case的"性能数据"表格均包含如下三列。

**表 1**  性能指标说明

| 列名 | 含义 |
|:---|:---|
| `grad/argmax`的D/H/W | `grad`和`argmax`的D、H、W三个维度的取值，三者相同，通过编译选项`OUTPUT_DHW`配置，可选1/2/4/8/16。 |
| 每个`NC`平面回填的梯度数 | 该档位下每个`NC`平面上要离散回填的梯度个数，取值为`grad/argmax`的D×H×W，用于衡量离散写的数据量。 |
| Task Duration(us) | `msOpProf`工具输出的Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间，本样例的性能对比以该列为准。 |

样例中相关数据在Ascend 950PR、64个Vector Core、AI Vector频率1650MHz、CANN 9.1.0环境采集得到，并完成正确性校验。

样例固定`output`为 [1,4096,32,32,32]，即每个`NC`平面有32\*32\*32=32768个元素。启用64个Block，每个Block负责1\*4096/64=64个平面的清零和梯度回填，因此Block之间互不影响，不需要考虑核间同步的情况。且`grad/argmax`的D/H/W（三者取相同值）的取值范围为1/2/4/8/16，因此每个梯度占据的区域互不重叠。样例会采集每种实现方式在不同梯度数下的耗时数据。

### Case 0：SIMD API清零并回填梯度

**实现方式：**

SIMD操作的对象是向量而不是单个元素，因此首先找到目标向量，实现思路如下图所示：

**图 2**  Case 0纯SIMD构造向量思路示意图
![](./figures/adaptive_max_pool3d_grad_simd_overview.png)

使用纯SIMD实现的关键步骤如下：

- **遍历每个梯度**：`grad/argmax`中的每个梯度各有一个值和一个`argmax`下标。对于每一个梯度，推出它在`output`上占据的区域，区域内的每个位置`p`都要做一轮向量计算。命中的位置写`grad`、未命中的位置写0，因此遍历完区域后顺带完成了`output`清零。
- **构造SIMD向量**：纯SIMD以向量为单位处理数据，因此本实现将连续64个`NC`平面上的同一个位置看作一个待处理的向量，每个SIMD lane对应一个`NC`平面。本样例将`NC=4096`按`NC_TILE=64`分为64组，并使用64个Block，因此每个Block处理一组数据。
- **Compare-Select-Move**：将当前`output`位置`p`复制到向量的每个lane，逐lane判断`argmax`是否等于`p`，再逐lane二选一得到`selected`向量。`selected`在UB中按`NC`连续存放，而GM中同一空间位置在不同`NC`平面之间相隔`input_spatial`，所以写回前需要先在UB中将每个值整理到独立的32B槽位，再通过带stride的数据搬运写入GM。

**关键代码：**

```cpp
asc_duplicate_scalar(input_index_reg, static_cast<int32_t>(base_input_index + tile_index), active_mask);
asc_eq(cmp_mask, argmax_reg, input_index_reg, active_mask);
asc_select(selected_reg, grad_reg, zero_reg, cmp_mask);
asc_storealign(selected_tile + tile_index * NC_TILE, selected_reg, active_mask);
```

**性能数据：**

| `grad/argmax`的D/H/W | 每个`NC`平面回填的梯度数 | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 6569.102 |
| 2 | 8 | 6699.820 |
| 4 | 64 | 6846.461 |
| 8 | 512 | 7745.685 |
| 16 | 4096 | 13898.433 |

**性能数据分析：**

Case 0的特点是无论离散写的数据多还是少，`output`中的每一个位置都要经历compare、select等多步计算，因此耗时较高。

最后一档（4096个梯度）耗时几乎翻倍到13898.433us，是因为区域被切得太碎。此时区域在W方向只剩2个元素，`select_w_tile`的调用次数翻倍，而每次调用内部真正做的比较只有2次。总比较次数依然没变，但每次调用都要重新付出一遍固定开销——更新mask、生成零向量、两次对齐加载——区域越碎，这部分固定开销被摊薄的机会越少，最终累积成新的瓶颈。同样的效应在前四档已经存在，只是规模还小，被恒定的主体计算量盖住了。

**结论：**

回填梯度的位置属于离散写，这里为了使用SIMD的API，不得不构造向量，通过mask掩码控制写的数据，代码复杂度较高，无论是编写还是阅读都非常困难，并且性能表现也较差，因此纯SIMD并不太适合这种Scatter离散写的场景。

> 该样例中adaptive_max_pool3d_grad_simd.h是参考实现的简化版本，更完整的处理方式可以[点击跳转](https://gitcode.com/cann/ops-nn/blob/master/pooling/adaptive_max_pool3d_grad/op_kernel/adaptive_max_pool3d_grad_normal.h)查看。

### Case 1：SIMD清零 + Scalar回填梯度

**实现方式：**

Case 1将清零和梯度回填分成两阶段，每个Block内先使用SIMD将负责的`NC`平面全部填零，然后遍历`grad/argmax`，通过标量循环直接计算目标地址并填入梯度值。

**关键代码：**

```cpp
//...
__aicore__ inline void zero_input(__gm__ float* input_data, uint64_t count)
{
    __ubuf__ float zero_ub[ZERO_TILE_COUNT];
    asc_vf_call<fill_float_ub>(zero_ub, 0.0F, ZERO_TILE_COUNT);
    wait_v_to_mte3();

    for (uint64_t offset = 0; offset < count; offset += ZERO_TILE_COUNT) {
        const uint32_t copy_count = static_cast<uint32_t>(min<uint64_t>(ZERO_TILE_COUNT, count - offset));
        const uint32_t copy_bytes = copy_count * sizeof(float);
        asc_copy_ub2gm(input_data + offset, zero_ub, copy_bytes);
    }
    wait_mte3_to_scalar();
}

//...
for (uint32_t i = 0; i < tile_grad_count; ++i) {
    const uint64_t local_grad_index = static_cast<uint64_t>(grad_offset + i);
    const uint64_t local_nc_index = local_grad_index / output_spatial;
    const uint64_t target = local_nc_index * input_spatial + static_cast<uint64_t>(argmax_ub[i]);
    grad_input[target] = grad_ub[i];
}
```

**性能数据：**

| `grad/argmax`的D/H/W | 每个`NC`平面回填的梯度数 | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 272.131 |
| 2 | 8 | 298.759 |
| 4 | 64 | 455.944 |
| 8 | 512 | 2010.422 |
| 16 | 4096 | 9140.783 |

**性能数据分析：**

相比Case 0，清零这一步由于是操作连续内存，Case 1直接通过SIMD API将UB中的数据0搬运到GM，每个`NC`平面只回填1个梯度时耗时仅272.131us，可见SIMD非常适合连续内存读写。而针对离散梯度回填，Case 1不再枚举每个梯度的区域、遍历区域内的每一个位置，而是直接使用`argmax`计算目标回填地址，计算量比Case 0小了很多，因此耗时也大幅降低。但是随着每个`NC`平面回填的梯度数从1增长到4096，标量循环次数同比增长，成为主要的性能瓶颈。

**结论：**

在SIMD中可以使用Main Scalar的方式直接处理离散读写，代码要比Case 0构造向量的方式简洁许多，但劣势也很明显：并行度很低会导致随着要离散写的数据量增加，耗时增加。

> 该样例中adaptive_max_pool3d_grad_scalar.h是参考实现的简化版本，更完整的处理方式可以[点击跳转](https://gitcode.com/cann/ops-nn/blob/master/pooling/pool_3d_common/op_kernel/arch32/max_pool3d_grad_scatter_unified.h)查看。

### Case 2：SIMD清零 + SIMT回填梯度

**实现方式：**

Case 2保留Case 1的SIMD清零方式，但是把梯度回填改为采用SIMT处理，每个线程负责一个梯度值的计算，通过大量的线程并行实现功能。

**关键代码：**

```cpp
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void scatter_grad(
    const __gm__ float* grad, const __gm__ int32_t* argmax, __gm__ float* grad_input, uint32_t block_nc_count, uint32_t input_spatial, uint32_t output_spatial)
{
    const uint64_t block_grad_count = static_cast<uint64_t>(block_nc_count) * output_spatial;
    for (uint64_t local_grad_index = static_cast<uint64_t>(threadIdx.x); local_grad_index < block_grad_count;
         local_grad_index += static_cast<uint64_t>(blockDim.x)) {
        const uint64_t local_nc_index = static_cast<uint32_t>(local_grad_index) / output_spatial;
        const uint64_t input_index = local_nc_index * input_spatial + static_cast<uint64_t>(argmax[local_grad_index]);
        grad_input[input_index] = grad[local_grad_index];
    }
}
```

**性能数据：**

| `grad/argmax`的D/H/W | 每个`NC`平面回填的梯度数 | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 270.612 |
| 2 | 8 | 282.933 |
| 4 | 64 | 339.883 |
| 8 | 512 | 442.487 |
| 16 | 4096 | 856.143 |

**性能数据分析：**

Case 2把Case 1中的标量回填循环并行化，要回填的梯度数越多收益越明显。每个`NC`平面回填4096个梯度时，Case 2相对Case 1耗时从9140.783us降至856.143us，约10.68倍加速。

**结论：**

在Scatter这种离散读写场景下，SIMT的多线程并行效率高于Main Scalar，并且处理的数据量越大，收益越高。

### Case 3：使用UB进一步提升混合编程的性能

**实现方式：**

在Case 2混合编程的基础上，还可以继续使用SIMT的常用优化手段，这里可以将两阶段合成一步，在UB中完成清零和梯度回填后再将数据搬到GM中。

**关键代码：**

```cpp
// 在UB中申请一段内存，初始值全置为0。
asc_vf_call<hybrid_ub_impl::fill_float_ub>(grad_input_ub, 0.0F, input_spatial);
// ... 省略部分代码
// 处理这一段数据需要回填的梯度。
asc_vf_call<hybrid_ub_impl::scatter_grad<float, int32_t>>(
    dim3(hybrid_ub_impl::THREAD_COUNT), grad_ub, argmax_ub, grad_input_ub,
    input_spatial, output_spatial);
asc_copy_ub2gm_align(input_data + input_base_offset, grad_input_ub, active_input_bytes);
```

**性能数据：**

| `grad/argmax`的D/H/W | 每个`NC`平面回填的梯度数 | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 276.880 |
| 2 | 8 | 297.455 |
| 4 | 64 | 285.779 |
| 8 | 512 | 316.859 |
| 16 | 4096 | 454.797 |

**性能数据分析：**

要回填的梯度数较少时，GM离散写数量也较少，Case 3额外引入的静态UB缓存和完整平面写回开销会抵消收益，因此回填1个和8个梯度的两档略慢于Case 2。梯度数增大后，SIMT在UB内离散写、MTE连续写回GM的路径开始占优；每个`NC`平面回填4096个梯度时，Case 3相对Case 2耗时从856.143us降至454.797us，约1.88倍加速。

**结论：**

使用混合编程时，SIMD和SIMT的性能优化手段可以复用，性能优化的上限更高。

## 对比总结

所有Case的性能数据如下：

| 每个`NC`平面回填的梯度数 | Case 0 SIMD(us) | Case 1 Scalar(us) | Case 2 Hybrid(us) | Case 3 Hybrid UB(us) |
|---:|---:|---:|---:|---:|
| 1 | 6569.102 | 272.131 | 270.612 | 276.880 |
| 8 | 6699.820 | 298.759 | 282.933 | 297.455 |
| 64 | 6846.461 | 455.944 | 339.883 | 285.779 |
| 512 | 7745.685 | 2010.422 | 442.487 | 316.859 |
| 4096 | 13898.433 | 9140.783 | 856.143 | 454.797 |

在该样例中，存在连续（清零）和离散（梯度回填）两部分数据处理，对比上面的性能数据以及四种核函数实现代码，可以发现此种情况更适合通过SIMD&SIMT混合编程来处理，既可以获得好的开发体验，又能够取得较好的性能。

> 数据清零部分也可以使用SIMT完成，但是性能不如混合版本，[点击跳转](https://gitcode.com/cann/ops-nn/blob/master/pooling/adaptive_max_pool3d_grad/op_kernel/arch35/adaptive_max_pool3d_grad_simt.h)获取代码实现。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build                           # 创建并进入build目录。
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j  # 编译工程。
  ./adaptive_max_pool3d_grad simd                      # 执行样例选择simd版本核函数实现。
  ./adaptive_max_pool3d_grad scalar                    # 执行样例选择scalar版本核函数实现。
  ./adaptive_max_pool3d_grad hybrid                    # 执行样例选择hybrid版本核函数实现。
  ./adaptive_max_pool3d_grad hybrid_ub                 # 执行样例选择hybrid_ub版本核函数实现。
  ```

  上述命令使用默认的`grad/argmax`形状 [1,4096,8,8,8]。通过`OUTPUT_DHW`选项可以改变`grad/argmax`的D/H/W，复现前面各Case性能数据表格中的其他档位。例如采集每个`NC`平面回填4096个梯度那一档的数据：

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DOUTPUT_DHW=16 ..;make -j
  ./adaptive_max_pool3d_grad hybrid_ub
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT）。 |
  | `OUTPUT_DHW` | `1`、`2`、`4`、`8`、`16` | `grad/argmax`的D/H/W取值，三者相同，默认为`8`。每个`NC`平面回填的梯度数即`grad/argmax`的D×H×W，用于复现性能对比表格中的各档数据。 |

- 运行参数说明

  样例可执行文件的运行命令格式如下。

  ```bash
  ./adaptive_max_pool3d_grad <implementation> [profile]
  ```

  | 参数 | 可选值 | 是否必选 | 说明 |
  | ---- | ------ | -------- | ---- |
  | `implementation` | `simd`、`scalar`、`hybrid`、`hybrid_ub` | 是 | 选择样例运行的核函数实现版本。 |
  | `profile` | `profile` | 否 | 性能分析模式参数。指定该参数后仅执行算子并跳过结果精度对比，便于配合msOpProf采集性能数据，采集性能时可能会对计算结果有影响。 |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```text
  test pass!
  ```

## 性能分析

### msOpProf工具介绍
msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

- 上板性能采集

    通过上板性能采集，可以直接测定算子昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

    使用`msOpProf`工具获取详细性能数据：
    ```bash
    msopprof ./adaptive_max_pool3d_grad simd profile   # 分析性能
    ```

    - 性能数据说明

      命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的文件夹，性能数据文件夹结构示例如下：

      ```bash
      ├──dump                       # 原始的性能数据，用户无需关注
      ├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
      ├──L2Cache.csv                # L2 Cache命中率，影响MTE2，建议合理规划数据搬运逻辑，增加命中率
      ├──Memory.csv                 # UB，L1和主存储器读写带宽速率
      ├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
      ├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
      ├──OpBasicInfo.csv            # 算子基础信息
      ├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
      ├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
      └──visualize_data.bin         # MindStudio Insight呈现文件
      ```

查看具体的性能分析结果：

```bash
# 查看Task Duration以及各项数据。
cat ./OPPROF_*/OpBasicInfo.csv
```
