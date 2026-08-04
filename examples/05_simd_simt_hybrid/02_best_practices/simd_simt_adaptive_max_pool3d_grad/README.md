# SIMD与SIMT混合编程实现 adaptive_max_pool3d_grad 算子样例

## 概述

本样例以 `adaptive_max_pool3d_grad` 算子为例，对比 SIMD、混合编程两种实现方式的复杂度和性能数据，说明在连续和离散读写并存的场景中，混合编程可以兼顾性能和开发体验。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── simd_simt_adaptive_max_pool3d_grad
│   ├── figures                              // README中的图片资源。
│   ├── adaptive_max_pool3d_grad_hybrid_ub.h // SIMD清零和SIMT静态UB回填实现。
│   ├── adaptive_max_pool3d_grad_hybrid.h    // SIMD清零和SIMT GM回填实现。
│   ├── adaptive_max_pool3d_grad_scalar.h    // SIMD清零和Scalar GM回填实现。
│   ├── adaptive_max_pool3d_grad_simd.h      // SIMD向量API实现清零和回填。
│   ├── adaptive_max_pool3d_grad.asc         // 统一main入口和Host侧运行逻辑。
│   ├── CMakeLists.txt                       // 编译工程文件。
│   ├── README.md                            // 中文样例说明。
│   └── README_en.md                         // 英文样例说明。
```

## 样例描述

**算子介绍**

adaptive_max_pool3d_grad 根据前向池化计算保存的 argmax，将梯度 grad 回填到 input 中。如下图所示：

![adaptive_max_pool3d_grad梯度回填示意图](./figures/adaptive_max_pool3d_grad_function.svg)

计算公式可以理解为：
```text
gradInput[nc, :] = 0
gradInput[nc, argmax[nc, od, oh, ow]] = grad[nc, od, oh, ow]
```

**参数说明：**

| 参数 | shape | 数据格式 |数据类型| 描述 |
|:---|:---|:---|:---|:---|
| `gradInput` | 样例中固定为 [1,4096,32,32,32] | NCDHW |FLOAT| 回填梯度的对象 |
| `grad` | 样例中默认取值为 [1,4096,8,8,8] | NCDHW |FLOAT| 记录每个池化窗口的反向梯度值 |
| `argmax` | 和grad一致，样例中默认取值为 [1,4096,8,8,8] | NCDHW |INT32| 前向池化保存的最大值下标，对应每个梯度要回填的位置 |

注：为简化代码便于理解，该样例实现不考虑池化窗口重叠的场景。且固定 input 的 shape 为 [1,4096,32,32,32]。

**Case概览**

样例中使用了4种方式实现该算子，可以在运行时通过参数控制执行对应版本的核函数，不同版本的

| Case | 运行参数| 实现文件 |线程块数量|每个线程块的线程数 | 对照目的 |
|---|---|---|---|---|---|
| 0 | `simd` | `adaptive_max_pool3d_grad_simd.h` |64|-| 展示纯SIMD表达下标回填时的数据组织复杂度。 |
| 1 | `scalar` | `adaptive_max_pool3d_grad_scalar.h` |64|-| 展示用`argmax`直接回填后避免窗口枚举的收益。 |
| 2 | `hybrid` | `adaptive_max_pool3d_grad_hybrid.h` |64|512| 展示连续清零使用SIMD、离散回填使用SIMT的混合方式。 |
| 3 | `hybrid_ub` | `adaptive_max_pool3d_grad_hybrid_ub.h` |64|512| 展示用UB缓存离散回填结果，降低随机GM写回压力。 |

## 样例实现

性能对比以 `msopprof` 工具输出的 `Task Duration(us)` 为准。下列数据在 Ascend 950PR、64个
Vector Core、AI Vector频率1650MHz、CANN 9.2.0 环境采集得到。采集时每组 `D_OUT/H_OUT/W_OUT`
取相同值，并使用普通运行路径完成正确性校验。

### Case 0：SIMD API 清零并回填梯度

**实现方式：**

Case 0按 `NC_TILE=64` 切分 `NC` 维，每个Block处理若干个 `NC` tile。对每个输出位置，
kernel先把同一输出位置在64个 `NC` 平面上的 `grad/argmax` 搬到UB，再枚举该输出位置对应的
输入池化窗口。SIMD向量比较 `argmax == input_index`，命中lane保留 `grad`，未命中lane写0。
随后通过UB Scatter整理为跨 `NC` 写回所需的32B间隔布局，再使用带stride的搬运写回GM。

**关键代码：**

```cpp
asc_vf_call<select_w_tile>(ub_.grad, ub_.argmax, ub_.selected_tile,
                           row_base_input_index + w, tile_count, nc_count_);
asc_vf_call<scatter_selected_w>(ub_.selected_tile + tile_index * NC_TILE,
                                ub_.scatter_offsets, ub_.expanded, nc_count_);
asc_copy_ub2gm_align(tensors_.input + output_offset_gm, ub_.expanded,
                     static_cast<uint16_t>(nc_count_), sizeof(float),
                     asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
                     static_cast<uint64_t>(input_spatial_) * sizeof(float), GM_BLOCK_BYTES);
```

**性能数据：**

| D_OUT/H_OUT/W_OUT | OUTPUT_SPATIAL | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 6569.102 |
| 2 | 8 | 6699.820 |
| 4 | 64 | 6846.461 |
| 8 | 512 | 7745.685 |
| 16 | 4096 | 13898.433 |

**性能数据分析：**

纯SIMD实现必须枚举输入池化窗口内的每个候选位置，并为未命中的输入位置显式写0。输出尺寸较小时，
单个输出位置覆盖的输入窗口更大；输出尺寸增大时，输出位置数量增加，`grad/argmax`搬入和跨NC写回
次数也随之增加。该路径虽然完全使用SIMD表达，但对下标回填类离散写入不够直接，整体耗时明显高于
后续直接使用 `argmax` 回填的实现。

**结论：**

回填梯度的位置属于离散写，这里为了使用 SIMD 的 api，不得不构造出一个向量处理，首先是代码复杂度较高，无论是编写还是阅读都非常困难，另外在该样例中，性能表现也较差，因此纯 SIMD 并不太适合这种 Scatter 离散写的场景。

> 该样例中 adaptive_max_pool3d_grad_simd.h 是参考实现的简化版本，更完整的处理方式可以[点击跳转](https://gitcode.com/cann/ops-nn/blob/master/pooling/adaptive_max_pool3d_grad/op_kernel/adaptive_max_pool3d_grad_normal.h)查看。

### Case 1：SIMD清零 + Scalar回填梯度

**实现方式：**

Case 1先让每个Block负责一段完整的 `NC` 切片，使用SIMD填充UB零值并连续搬到该切片的
`gradInput` GM空间，完成初始化。随后按 `TILE_GRAD_COUNT=4096` 将 `grad/argmax` 分块搬入UB，
由标量循环直接计算目标输入地址并写回GM。

**关键代码：**

```cpp
//...
__aicore__ inline void zero_input(__gm__ float* input_data, uint32_t block_nc_count, uint32_t input_spatial)
{
    __ubuf__ float zero_ub[ZERO_TILE_COUNT];
    const uint64_t zero_count = static_cast<uint64_t>(block_nc_count) * input_spatial;
    asc_vf_call<fill_float_ub>(zero_ub, 0.0F, ZERO_TILE_COUNT);
    wait_v_to_mte3();

    for (uint64_t offset = 0; offset < zero_count; offset += ZERO_TILE_COUNT) {
        const uint32_t copy_count = static_cast<uint32_t>(min<uint64_t>(ZERO_TILE_COUNT, zero_count - offset));
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
    input[target] = grad_ub[i];
}
```

**性能数据：**

| D_OUT/H_OUT/W_OUT | OUTPUT_SPATIAL | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 272.131 |
| 2 | 8 | 298.759 |
| 4 | 64 | 455.944 |
| 8 | 512 | 2010.422 |
| 16 | 4096 | 9140.783 |

**性能数据分析：**

相比Case 0，Case 1不再枚举池化窗口，而是直接使用 `argmax` 计算回填地址，因此在所有输出尺寸下都
大幅降低耗时。随着 `OUTPUT_SPATIAL` 从1增长到4096，标量循环次数按 `NC * OUTPUT_SPATIAL` 增长，
`D_OUT/H_OUT/W_OUT=16` 时标量逐元素回填成为主要瓶颈。

**结论：**

在 SIMD 中可以使用 Main Scalar 的方式直接处理离散读写，代码要比 Case 0 构造向量的方式简洁许多，但劣势也很明显：并行度很低会导致随着要离散写的数据量增加，耗时增加。

> 该样例中 adaptive_max_pool3d_grad_scalar.h 是参考实现的简化版本，更完整的处理方式可以[点击跳转](https://gitcode.com/cann/ops-nn/blob/master/pooling/pool_3d_common/op_kernel/arch32/max_pool3d_grad_scatter_unified.h)查看。

### Case 2：SIMD清零 + SIMT回填梯度

**实现方式：**

Case 2 保留Case 1 的 SIMD 清零方式，但是把梯度回填改为采用 SIMT 处理。每个 Block 仍拥有互不重叠
的 `NC` 切片，因此清零和回填只需要Block内顺序保证，不需要跨Block同步。SIMT VF 使用512个线程
遍历本Block的 `grad/argmax` 切片，直接把 `grad` 写到 `argmax` 指定的GM位置。

**关键代码：**

```cpp
for (uint64_t local_grad_index = static_cast<uint64_t>(threadIdx.x);
     local_grad_index < block_grad_count;
     local_grad_index += static_cast<uint64_t>(blockDim.x)) {
    const uint64_t local_nc_index = static_cast<uint32_t>(local_grad_index) / output_spatial;
    const uint64_t input_index =
        local_nc_index * input_spatial + static_cast<uint64_t>(argmax[local_grad_index]);
    input[input_index] = grad[local_grad_index];
}
```

**性能数据：**

| D_OUT/H_OUT/W_OUT | OUTPUT_SPATIAL | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 270.612 |
| 2 | 8 | 282.933 |
| 4 | 64 | 339.883 |
| 8 | 512 | 442.487 |
| 16 | 4096 | 856.143 |

**性能数据分析：**

Case 2 把 Case 1 中的标量回填循环并行化，输出尺寸越大收益越明显。在 `D_OUT/H_OUT/W_OUT=16`
时，Case 2 相对 Case 1 耗时从9140.783us降至856.143us，约10.68倍加速。

**结论：**

在 Scatter 这种离散读写场景下，SIMT 的多线程并行效率高于 SIMD，并且处理的数据量越大，收益越高。

### Case 3：使用 UB 进一步提升混合编程的性能

**实现方式：**

在 Case 2 中，清零和梯度回填分成了两个独立的步骤，并且使用 SIMT 处理梯度回填的时候是直接写回到 GM 中的。事实上，可以把两个步骤融合在一起，在 UB 中完成清零和梯度回填后再将数据搬到 GM 中。

**关键代码：**

```cpp
// 在 UB 中申请一段内存，初始值全置为 0
asc_vf_call<hybrid_ub_impl::fill_float_ub>(output_ub, 0.0F, active_output_count);
// ... 省略部分代码
// 处理这一段数据需要回填的梯度
asc_vf_call<hybrid_ub_impl::scatter_grad<float, int32_t>>(
    dim3(hybrid_ub_impl::THREAD_COUNT), grad_ub, argmax_ub, output_ub,
    input_spatial, output_spatial);
asc_copy_ub2gm_align(input_data + output_base_offset, output_ub, active_output_bytes);
```

**性能数据：**

| D_OUT/H_OUT/W_OUT | OUTPUT_SPATIAL | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 276.880 |
| 2 | 8 | 297.455 |
| 4 | 64 | 285.779 |
| 8 | 512 | 316.859 |
| 16 | 4096 | 454.797 |

**性能数据分析：**

输出空间较小时，GM离散写数量较少，Case 3额外引入的静态UB缓存和完整平面写回开销会抵消收益，因此
`1/2` 档略慢于Case 2。输出空间增大后，SIMT在UB内离散写、MTE连续写回GM的路径开始占优；
`D_OUT/H_OUT/W_OUT=16` 时，Case 3相对Case 2耗时从856.143us降至454.797us，约1.88倍加速。

**结论：**

使用混合编程时，SIMD 和 SIMT 的性能优化手段可以复用，性能优化的上限更高。

## 对比总结

所有 Case 的性能数据如下：

| D_OUT/H_OUT/W_OUT | Case 0 SIMD(us) | Case 1 Scalar(us) | Case 2 Hybrid(us) | Case 3 Hybrid UB(us) |
|---:|---:|---:|---:|---:|
| 1 | 6569.102 | 272.131 | 270.612 | 276.880 |
| 2 | 6699.820 | 298.759 | 282.933 | 297.455 |
| 4 | 6846.461 | 455.944 | 339.883 | 285.779 |
| 8 | 7745.685 | 2010.422 | 442.487 | 316.859 |
| 16 | 13898.433 | 9140.783 | 856.143 | 454.797 |

在该样例中，存在连续（清零）和离散（梯度回填）两部分数据处理，对比上面的性能数据以及四种核函数实现代码，可以发现此种情况更适合通过 SIMD&SIMT 混合编程来处理，既可以获得好的开发体验，又能够取得较好的性能。

> 数据清零部分也可以使用 SIMT 完成，但是性能不如混合版本，[点击跳转](https://gitcode.com/cann/ops-nn/blob/master/pooling/adaptive_max_pool3d_grad/op_kernel/arch35/adaptive_max_pool3d_grad_simt.h)获取代码实现。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build                           # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j  # 编译工程
  ./demo simd                                          # 执行样例选择 simd 版本核函数实现
  ./demo scalar                                        # 执行样例选择 scalar 版本核函数实现
  ./demo hybrid                                        # 执行样例选择 hybrid 版本核函数实现
  ./demo hybrid_ub                                     # 执行样例选择 hybrid_ub 版本核函数实现
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |

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

    使用 `msOpProf` 工具获取详细性能数据：
    ```bash
    msopprof ./demo simd profile   # 分析性能
    ```

    - 性能数据说明  
      命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的文件夹,性能数据文件夹结构示例如下：

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
# 查看Task Duration 以及各项数据
cat ./OPPROF_*/OpBasicInfo.csv
```
