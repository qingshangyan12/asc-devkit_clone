# SIMT接口软仿情况汇总

表格按照 API 文档所在目录分级展示。

## Warp函数

### Lane-ID类函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `laneid` | 否 |  |
| `lanemask_eq` | 否 |  |
| `lanemask_ge` | 否 |  |
| `lanemask_gt` | 否 |  |
| `lanemask_le` | 否 |  |
| `lanemask_lt` | 否 |  |

### Warp-Reduce类函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `asc_reduce_add` | 否 |  |
| `asc_reduce_max` | 否 |  |
| `asc_reduce_min` | 否 |  |

### Warp-Shfl类函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `asc_shfl` | 否 |  |
| `asc_shfl_down` | 否 | |
| `asc_shfl_up` | 否 |  |
| `asc_shfl_xor` | 否 | |

### Warp-Vote类函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `asc_activemask` | 否 |  |
| `asc_all` | 否 |  |
| `asc_any` | 否 |  |
| `asc_ballot` | 否 |  |

## 协作组

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `binary_partition` | 否 |  |
| `tiled_partition` | 否 |  |

### coalesced_group

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `all` | 否 |  |
| `any` | 否 |  |
| `ballot` | 否 |  |
| `coalesced_group构造函数` | 否 |  |
| `meta_group_rank` | 否 |  |
| `meta_group_size` | 否 |  |
| `num_threads` | 否 |  |
| `shfl` | 否 |  |
| `shfl_down` | 否 |  |
| `shfl_up` | 否 |  |
| `size` | 否 |  |
| `sync` | 否 |  |
| `thread_rank` | 否 |  |

### thread_block

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `dim_threads` | 否 |  |
| `group_dim` | 否 |  |
| `group_index` | 否 |  |
| `num_threads` | 否 |  |
| `size` | 否 |  |
| `sync` | 否 |  |
| `thread_block构造函数` | 否 |  |
| `thread_index` | 否 |  |
| `thread_rank` | 否 |  |

### thread_block_tile

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `all` | 否 |  |
| `any` | 否 |  |
| `ballot` | 否 |  |
| `meta_group_rank` | 否 |  |
| `meta_group_size` | 否 |  |
| `num_threads` | 否 |  |
| `shfl` | 否 |  |
| `shfl_down` | 否 |  |
| `shfl_up` | 否 |  |
| `shfl_xor` | 否 |  |
| `size` | 否 |  |
| `sync` | 否 |  |
| `thread_block_tile构造函数` | 否 |  |
| `thread_rank` | 否 |  |

## 原子操作

以下信息是基于SIMD与SIMT编程场景进行说明的，对于SIMT编程场景，由于接口入参无UB/GM内存空间标识，底层编译器需要先将入参指针转换为UB/GM指针，再实现接口功能。
| API名 | 非软仿数据类型 | 软仿数据类型 |
| --- | --- | --- |
| `asc_atomic_add` | int32_t、uint32_t、int64_t、uint64_t、half、bfloat16_t | float、half2、bfloat16x2_t |
| `asc_atomic_and` | - | 所有数据类型 |
| `asc_atomic_cas` | uint32_t、uint64_t | int32_t、int64_t、float、half、bfloat16_t、half2、bfloat16x2_t |
| `asc_atomic_exch` | uint32_t、uint64_t | int32_t、int64_t、float、half、bfloat16_t、half2、bfloat16x2_t |
| `asc_atomic_inc` | - | 所有数据类型 |
| `asc_atomic_max` | uint32_t、int32_t、int64_t、uint64_t、half、bfloat16_t | float、half2、bfloat16x2_t |
| `asc_atomic_min` | uint32_t、int32_t、int64_t、uint64_t、half、bfloat16_t | float、half2、bfloat16x2_t |
| `asc_atomic_or` | - | 所有数据类型 |
| `asc_atomic_sub` | - | 所有数据类型 |
| `asc_atomic_xor` | - | 所有数据类型 |

## 同步与内存栅栏

### 内存栅栏接口

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `asc_threadfence` | 否 |  |
| `asc_threadfence_block` | 否 |  |

### 同步接口

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `asc_syncthreads` | 否 |  |

## 地址空间谓词函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__isGlobal` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__isLocal` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__isUbuf` | 未知 | 参考文档中未找到“API映射关系”章节。 |

## 地址空间转换函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__cvta_generic_to_global` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__cvta_generic_to_local` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__cvta_generic_to_ubuf` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__cvta_global_to_generic` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__cvta_local_to_generic` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__cvta_ubuf_to_generic` | 未知 | 参考文档中未找到“API映射关系”章节。 |

## 数学函数

### bfloat16类型

#### bfloat16x2类型数学库函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `h2ceil` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = __ceilf(x.x);<br>half tmp2 = __ceilf(x.y);<br>return {tmp1, tmp2}; |
| `h2cos` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过cosf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2exp` | 是 | 本接口是软仿实现的。先将输入数据转换为float类型，通过powf计算之后，再转为half类型。 |
| `h2exp10` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过powf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2exp2` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过powf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2floor` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = __floorf(x.x);<br>half tmp2 = __floorf(x.y);<br>return {tmp1, tmp2}; |
| `h2log` | 是 | 本接口是软仿实现的。先将输入数据转换为float类型，通过logf计算之后，再转为half类型后返回。 |
| `h2log10` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过logf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2log2` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过logf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2rcp` | 是 | 本接口是软仿实现的。先将输入数据各分量通过倒数计算之后，填充至half2变量后返回。 |
| `h2rint` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = __rintf(x.x);<br>half tmp2 = __rintf(x.y);<br>return {tmp1, tmp2}; |
| `h2rsqrt` | 是 | 本接口是软仿实现的。先将输入数据各分量通过sqrt和倒数计算之后，填充至half2变量后返回。 |
| `h2sin` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过sinf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2sqrt` | 否 |  |
| `h2tanh` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过tanhf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2trunc` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = x.x;<br>half tmp2 = x.y;<br>if(x.x > static_cast(0)){<br>tmp1 = __floorf(tmp1);<br>}<br>if(x.y > static_cast(0)){<br>tmp2 = __floorf(tmp2);<br>}<br>x = {tmp1, tmp2};<br>return x; |

#### bfloat16x2类型比较函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__hbequx2` | 否 |  |
| `__hbeqx2` | 否 |  |
| `__hbgeux2` | 否 |  |
| `__hbgex2` | 否 |  |
| `__hbgtux2` | 否 |  |
| `__hbgtx2` | 否 |  |
| `__hbleux2` | 否 |  |
| `__hblex2` | 否 |  |
| `__hbltux2` | 否 |  |
| `__hbltx2` | 否 |  |
| `__hbneux2` | 否 |  |
| `__hbnex2` | 否 |  |
| `__hequx2` | 否 |  |
| `__hequx2_mask` | 否 |  |
| `__heqx2` | 否 |  |
| `__heqx2_mask` | 否 |  |
| `__hgeux2` | 否 |  |
| `__hgeux2_mask` | 否 |  |
| `__hgex2` | 否 |  |
| `__hgex2_mask` | 否 |  |
| `__hgtux2` | 否 |  |
| `__hgtux2_mask` | 否 |  |
| `__hgtx2` | 否 |  |
| `__hgtx2_mask` | 否 |  |
| `__hleux2` | 否 |  |
| `__hleux2_mask` | 否 |  |
| `__hlex2` | 否 |  |
| `__hlex2_mask` | 否 |  |
| `__hltux2` | 否 |  |
| `__hltux2_mask` | 否 |  |
| `__hltx2` | 否 |  |
| `__hltx2_mask` | 否 |  |
| `__hmaxx2` | 否 |  |
| `__hmaxx2_nan` | 是 | 对两个half2类型数据对应分量做非nan判断并将最大值注入新创建的half2类型数据对应位置并返回该数据。 |
| `__hminx2` | 否 |  |
| `__hminx2_nan` | 是 | 对两个half2类型数据对应分量做非nan判断并将最小值注入新创建的half2类型数据对应位置并返回该数据。 |
| `__hneux2` | 否 |  |
| `__hneux2_mask` | 否 |  |
| `__hnex2` | 否 |  |
| `__hnex2_mask` | 否 |  |
| `__isnanx2` | 否 |  |

#### bfloat16x2类型算术函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__habsx2` | 否 |  |
| `__haddx2` | 否 |  |
| `__hcmadd` | 是 | |
| `__hdivx2` | 否 |  |
| `__hfmax2` | 否 |  |
| `__hfmax2_relu` | 是 | |
| `__hmulx2` | 否 |  |
| `__hnegx2` | 否 |  |
| `__hsubx2` | 否 |  |

#### bfloat16类型数学库函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `hceil` | 否 |  |
| `hcos` | 是 |  |
| `hexp10` | 是 |  |
| `hexp2` | 是 |  |
| `hfloor` | 否 |  |
| `hlog` | 是 |  |
| `hlog10` | 是 |  |
| `hlog2` | 是 |  |
| `hrcp` | 是 | 使用操作符“/”实现倒数计算：static_cast(1.0) / x； |
| `hrint` | 否 |  |
| `hrsqrt` | 是 | |
| `hsin` | 是 |  |
| `hsqrt` | 是 |  |
| `htanh` | 是 |  |
| `htrunc` | 是 |  |

#### bfloat16类型比较函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__heq` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hequ` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hge` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hgeu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hgt` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hgtu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hisinf` | 否 |  |
| `__hisnan` | 否 |  |
| `__hle` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hleu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hlt` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hltu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hmax` | 是 |  |
| `__hmax_nan` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hmin` | 是 |  |
| `__hmin_nan` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hne` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hneu` | 未知 | 未在参考目录中找到对应API文档。 |

#### bfloat16类型算术函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__habs` | 是 | 使用操作符“&”实现绝对值计算：<br>bits &= 0x7FFF； |
| `__hadd` | 否 |  |
| `__hdiv` | 否 |  |
| `__hfma` | 否 |  |
| `__hfma_relu` | 是 | |
| `__hmul` | 否 |  |
| `__hneg` | 否 |  |
| `__hsub` | 否 |  |

#### bfloat16类型精度转换函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__bfloat1622float2` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162bfloat162` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162bfloat16_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162bfloat16_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162bfloat16_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162bfloat16_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162bfloat16_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162float` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162int_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162int_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162int_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162int_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162int_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ll_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ll_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ll_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ll_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ll_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162uint_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162uint_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162uint_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162uint_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162uint_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ull_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ull_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ull_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ull_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__bfloat162ull_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rd_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rn_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rna_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_ru_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22bfloat162_rz_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat162_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rd_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rn_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rna_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_ru_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2bfloat16_rz_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__floats2bfloat162_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half2bfloat16_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half2bfloat16_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half2bfloat16_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half2bfloat16_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half2bfloat16_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__halves2bfloat162` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__high2bfloat16` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__high2bfloat162` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__high2float` | 否 |  |
| `__highs2bfloat162` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2bfloat16_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2bfloat16_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2bfloat16_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2bfloat16_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2bfloat16_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2bfloat16_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2bfloat16_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2bfloat16_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2bfloat16_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2bfloat16_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__low2bfloat16` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__low2bfloat162` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__low2float` | 否 |  |
| `__lowhigh2highlow` | 未知 | 接口实现为将输入half2类型数据的高16位和低16位分别赋值给新的half2类型数据的低16位和高16位并返回。<br>half2 tmp;<br>tmp.x = x.y;<br>tmp.y = x.x;<br>return tmp; |
| `__lows2bfloat162` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2bfloat16_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2bfloat16_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2bfloat16_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2bfloat16_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2bfloat16_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2bfloat16_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2bfloat16_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2bfloat16_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2bfloat16_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2bfloat16_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ushort_as_bfloat16` | 未知 | 参考文档中未找到“API映射关系”章节。 |

### float类型数学库函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__fdividef` | 是 | 实现代码为：return x / y; |
| `__saturatef` | 是 |  |
| `acosf` | 是 |  |
| `acoshf` | 是 |  |
| `asinf` | 是 |  |
| `asinhf` | 是 |  |
| `atan2f` | 是 |  |
| `atanf` | 是 |  |
| `atanhf` | 是 |  |
| `cbrtf` | 是 |  |
| `ceilf` | 否 |  |
| `copysignf` | 是 |  |
| `cosf` | 是 |  |
| `coshf` | 是 |  |
| `cospif` | 是 |  |
| `cyl_bessel_i0f` | 是 |  |
| `cyl_bessel_i1f` | 是 |  |
| `erfcf` | 是 |  |
| `erfcinvf` | 是 |  |
| `erfcxf` | 是 |  |
| `erff` | 是 |  |
| `erfinvf` | 是 |  |
| `exp10f` | 否 |  |
| `exp2f` | 否 |  |
| `expf` | 否 |  |
| `expm1f` | 是 |  |
| `fabsf` | 否 |  |
| `fdimf` | 是 |  |
| `fdividef` | 是 | 实现代码为：return x / y; |
| `floorf` | 否 |  |
| `fmaf` | 否 |  |
| `fmaxf` | 是 | 本接口是软仿实现的。实现如下：<br>if (isnan(x)) {<br>return y;<br>} else if (isnan(y)) {<br>return x;<br>}<br>return __fmaxf(x, y); |
| `fminf` | 是 | 本接口是软仿实现的。实现如下：<br>if (isnan(x)) {<br>return y;<br>} else if (isnan(y)) {<br>return x;<br>}<br>return __fminf(x, y); |
| `fmodf` | 是 | 本接口是软仿实现的。先处理 NaN、无穷大、除零等异常情况，再通过浮点数位运算快速构造缩放因子，用循环减法逐步逼近求余，最终返回带正确符号的x对y取模结果。 |
| `frexpf` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `hypotf` | 是 | 本接口是软仿实现的。先处理无穷大、NaN 等异常值，再通过取最大值、最小值避免计算溢出，再计算x、y平方和的平方根。 |
| `ilogbf` | 是 | 本接口是软仿实现的。实现如下：<br>// ASCRT_MIN_VAL_S为0x80000000<br>if (x < 0) {<br>x = -x;<br>}<br>if (isnan(x)) {<br>return ASCRT_MIN_VAL_S;<br>}<br>return static_cast(logbf(x)); |
| `isfinite` | 否 |  |
| `isinf` | 否 |  |
| `isnan` | 否 |  |
| `j0f` | 是 | 本接口是软仿实现的。实现如下：<br>if (isnan(x)) {<br>return ASCRT_INF_F / ASCRT_INF_F;<br>}<br>float f1 = fabsf(x);<br>if (f1 > 1e13f && isfinite(f1)) {<br>return 0;<br>}<br>if (f1 > 8.0f) {<br>return __internal_cal_j0_x_larger8(f1);<br>} else {<br>return __internal_cal_j0_x_less8(f1);<br>} |
| `j1f` | 是 | 本接口是软仿实现的。实现如下：<br>if (isnan(x)) {<br>return ASCRT_INF_F / ASCRT_INF_F;<br>}<br>float f1 = fabsf(x);<br>if (f1 > 1e13f && isfinite(f1)) {<br>return 0;<br>}<br>float res;<br>if (f1 > 8.0f) {<br>res = __internal_cal_j1_x_lager8(f1);<br>} else {<br>res = __internal_cal_j1_x_less8(f1);<br>}<br>return (x < 0) ? -res : res; |
| `jnf` | 是 | 本接口是软仿实现的。先处理参数合法性、特殊值与符号，再根据阶数n和自变量x的数值关系分4个区间调用专用内部函数。 |
| `ldexpf` | 是 | 本接口是软仿实现的。通过处理异常值并分阶缩放，实现将浮点数x * 2^exp功能，同时防止溢出与下溢。 |
| `lgammaf` | 是 | 本接口是软仿实现的。按输入绝对值分多区间（小数、中数、大数），分别调用内部函数或斯特林近似计算伽马函数对数，负数输入通过欧拉反射公式修正结果。 |
| `llrintf` | 是 | 本接口是软仿实现的。实现如下：<br>float tmp = rintf(x);<br>return __cvt_int64_t(), RoundingSaturation::RS_ENABLE_VALUE>(tmp); |
| `llroundf` | 是 | 本接口是软仿实现的。实现如下：<br>float tmp = roundf(x);<br>return __cvt_int64_t(), RoundingSaturation::RS_ENABLE_VALUE>(tmp); |
| `log10f` | 是 | 本接口是软仿实现的。实现如下：<br>return logf(x) / logf(10.0f); |
| `log1pf` | 是 | 本接口是软仿实现的。实现如下：<br>return logf(1.0f + x); |
| `log2f` | 是 | 本接口是软仿实现的。实现如下：<br>return logf(x) / logf(2.0f); |
| `logbf` | 是 | 本接口是软仿实现的。通过直接提取float浮点数二进制存储中的指数位，减去偏移量127得到真实指数，并处理NaN、无穷大、0等异常值，实现logb₁₀\(x\)函数功能。 |
| `logf` | 是 | 本接口是软仿实现的。直接调用的built-in接口如下：<br>float logf(float x); |
| `lrintf` | 是 | 本接口是软仿实现的。实现如下：<br>float tmp = rintf(x);<br>return __cvt_int64_t(), RoundingSaturation::RS_ENABLE_VALUE>(tmp); |
| `lroundf` | 是 | 本接口是软仿实现的。实现如下：<br>float tmp = roundf(x);<br>return __cvt_int64_t(), RoundingSaturation::RS_ENABLE_VALUE>(tmp); |
| `modff` | 未知 | 未在参考目录中找到对应API文档。 |
| `nearbyintf` | 未知 | 未在参考目录中找到对应API文档。 |
| `nextafterf` | 未知 | 未在参考目录中找到对应API文档。 |
| `norm3df` | 未知 | 未在参考目录中找到对应API文档。 |
| `norm4df` | 未知 | 未在参考目录中找到对应API文档。 |
| `normcdff` | 未知 | 未在参考目录中找到对应API文档。 |
| `normcdfinvf` | 未知 | 未在参考目录中找到对应API文档。 |
| `normf` | 未知 | 未在参考目录中找到对应API文档。 |
| `powf` | 未知 | 未在参考目录中找到对应API文档。 |
| `rcbrtf` | 未知 | 未在参考目录中找到对应API文档。 |
| `remainderf` | 未知 | 未在参考目录中找到对应API文档。 |
| `remquof` | 未知 | 未在参考目录中找到对应API文档。 |
| `rhypotf` | 未知 | 未在参考目录中找到对应API文档。 |
| `rintf` | 未知 | 未在参考目录中找到对应API文档。 |
| `rnorm3df` | 未知 | 未在参考目录中找到对应API文档。 |
| `rnorm4df` | 未知 | 未在参考目录中找到对应API文档。 |
| `rnormf` | 未知 | 未在参考目录中找到对应API文档。 |
| `roundf` | 未知 | 未在参考目录中找到对应API文档。 |
| `rsqrtf` | 未知 | 未在参考目录中找到对应API文档。 |
| `scalblnf` | 未知 | 未在参考目录中找到对应API文档。 |
| `scalbnf` | 未知 | 未在参考目录中找到对应API文档。 |
| `signbit` | 未知 | 未在参考目录中找到对应API文档。 |
| `sincosf` | 未知 | 未在参考目录中找到对应API文档。 |
| `sincospif` | 未知 | 未在参考目录中找到对应API文档。 |
| `sinf` | 未知 | 未在参考目录中找到对应API文档。 |
| `sinhf` | 未知 | 未在参考目录中找到对应API文档。 |
| `sinpif` | 未知 | 未在参考目录中找到对应API文档。 |
| `sqrtf` | 未知 | 未在参考目录中找到对应API文档。 |
| `tanf` | 未知 | 未在参考目录中找到对应API文档。 |
| `tanhf` | 未知 | 未在参考目录中找到对应API文档。 |
| `tanpif` | 未知 | 未在参考目录中找到对应API文档。 |
| `tgammaf` | 未知 | 未在参考目录中找到对应API文档。 |
| `truncf` | 未知 | 未在参考目录中找到对应API文档。 |
| `y0f` | 未知 | 未在参考目录中找到对应API文档。 |
| `y1f` | 未知 | 未在参考目录中找到对应API文档。 |
| `ynf` | 未知 | 未在参考目录中找到对应API文档。 |

### fp8类型

#### fp8数据转换函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__asc_cvt_float2_to_fp8x2` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__e4m3x22float2` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__e5m2x22float2` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22hif82_rh` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22hif82_rh_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22hif82_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float22hif82_rna_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half22hif82_rh` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half22hif82_rh_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half22hif82_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half22hif82_rna_sat` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__hif822float2` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__hif822half2` | 未知 | 参考文档中未找到“API映射关系”章节。 |

### half类型

#### half2类型数学库函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `h2ceil` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = __ceilf(x.x);<br>half tmp2 = __ceilf(x.y);<br>return {tmp1, tmp2}; |
| `h2cos` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过cosf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2exp` | 是 | 本接口是软仿实现的。先将输入数据转换为float类型，通过powf计算之后，再转为half类型。 |
| `h2exp10` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过powf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2exp2` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过powf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2floor` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = __floorf(x.x);<br>half tmp2 = __floorf(x.y);<br>return {tmp1, tmp2}; |
| `h2log` | 是 | 本接口是软仿实现的。先将输入数据转换为float类型，通过logf计算之后，再转为half类型后返回。 |
| `h2log10` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过logf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2log2` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过logf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2rcp` | 是 | 本接口是软仿实现的。先将输入数据各分量通过倒数计算之后，填充至half2变量后返回。 |
| `h2rint` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = __rintf(x.x);<br>half tmp2 = __rintf(x.y);<br>return {tmp1, tmp2}; |
| `h2rsqrt` | 是 | 本接口是软仿实现的。先将输入数据各分量通过sqrt和倒数计算之后，填充至half2变量后返回。 |
| `h2sin` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过sinf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2sqrt` | 否 |  |
| `h2tanh` | 是 | 本接口是软仿实现的。先将输入数据各分量转换为float类型，通过tanhf计算之后，再转为half类型，填充至half2变量后返回。 |
| `h2trunc` | 未知 | 本接口非built-in接口，具体实现如下：<br>half tmp1 = x.x;<br>half tmp2 = x.y;<br>if(x.x > static_cast(0)){<br>tmp1 = __floorf(tmp1);<br>}<br>if(x.y > static_cast(0)){<br>tmp2 = __floorf(tmp2);<br>}<br>x = {tmp1, tmp2};<br>return x; |

#### half2类型比较函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__hbequx2` | 否 |  |
| `__hbeqx2` | 否 |  |
| `__hbgeux2` | 否 |  |
| `__hbgex2` | 否 |  |
| `__hbgtux2` | 否 |  |
| `__hbgtx2` | 否 |  |
| `__hbleux2` | 否 |  |
| `__hblex2` | 否 |  |
| `__hbltux2` | 否 |  |
| `__hbltx2` | 否 |  |
| `__hbneux2` | 否 |  |
| `__hbnex2` | 否 |  |
| `__hequx2` | 否 |  |
| `__hequx2_mask` | 否 |  |
| `__heqx2` | 否 |  |
| `__heqx2_mask` | 否 |  |
| `__hgeux2` | 否 |  |
| `__hgeux2_mask` | 否 |  |
| `__hgex2` | 否 |  |
| `__hgex2_mask` | 否 |  |
| `__hgtux2` | 否 |  |
| `__hgtux2_mask` | 否 |  |
| `__hgtx2` | 否 |  |
| `__hgtx2_mask` | 否 |  |
| `__hleux2` | 否 |  |
| `__hleux2_mask` | 否 |  |
| `__hlex2` | 否 |  |
| `__hlex2_mask` | 否 |  |
| `__hltux2` | 否 |  |
| `__hltux2_mask` | 否 |  |
| `__hltx2` | 否 |  |
| `__hltx2_mask` | 否 |  |
| `__hmaxx2` | 否 |  |
| `__hmaxx2_nan` | 是 | 本接口是built-in接口，且是软仿实现的，对两个half2类型数据对应分量做非nan判断并将最大值注入新创建的half2类型数据对应位置并返回该数据。 |
| `__hminx2` | 否 |  |
| `__hminx2_nan` | 是 | 本接口是built-in接口，且是软仿实现的，对两个half2类型数据对应分量做非nan判断并将最小值注入新创建的half2类型数据对应位置并返回该数据。 |
| `__hneux2` | 否 |  |
| `__hneux2_mask` | 否 |  |
| `__hnex2` | 否 |  |
| `__hnex2_mask` | 否 |  |
| `__isnanx2` | 否 |  |

#### half2类型算术函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__habsx2` | 否 |  |
| `__haddx2` | 否 |  |
| `__hcmadd` | 是 | 本接口是built-in接口，且是软仿实现的，实现了复数乘法的并行计算。 |
| `__hdivx2` | 否 |  |
| `__hfmax2` | 否 |  |
| `__hfmax2_relu` | 是 | 本接口是built-in接口，且是软仿实现的，直接调用的built-in接口如下：<br>half2 __hfmax2(half in1, half in2, half3 in3);<br>half2 __hmaxx2(half in1, half in2); |
| `__hmulx2` | 否 |  |
| `__hnegx2` | 否 |  |
| `__hsubx2` | 否 |  |

#### half类型数学库函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `hceil` | 否 |  |
| `hcos` | 是 | 本接口是软仿实现的，先将操作数转为float类型，通过cosf\(\)接口实现后再转为float类型，调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>cosf(float in)<br>template<br>__cvt_half(T in) |
| `hexp` | 否 |  |
| `hexp10` | 是 | 本接口是软仿实现的，先将操作数转为float类型，通过powf\(\)接口实现后再转为half类型，调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>powf(float in1, float in2)<br>template<br>__cvt_half(T in) |
| `hexp2` | 是 | 本接口是软仿实现的，先将操作数转为float类型，通过powf\(\)接口实现后再转为half类型。调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>powf(float in1, float in2)<br>template<br>__cvt_half(T in) |
| `hfloor` | 否 |  |
| `hlog` | 否 |  |
| `hlog10` | 是 | 本接口是软仿实现的，先将操作数转为float类型，通过logf\(\)接口实现后再转为float类型，调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>logf(float in)<br>template<br>__cvt_half(T in) |
| `hlog2` | 是 | 本接口是软仿实现的，先将操作数转为float类型，通过logf\(\)接口实现后再转为half类型，调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>logf(float in)<br>template<br>__cvt_half(T in) |
| `hrcp` | 是 | 本接口是软仿实现的，实现如下：<br>half x = static_cast(1.0) / x; |
| `hrint` | 否 |  |
| `hrsqrt` | 否 |  |
| `hsin` | 是 | 本接口是软仿实现的，现将操作数转为float类型，通过sinf\(\)接口实现后再转为half类型，调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>sinf(float in)<br>template<br>__cvt_half(T in) |
| `hsqrt` | 是 | 本接口是软仿实现的，实现如下：<br>half x = (half)1.0 / __sqrtf(half in); |
| `htanh` | 是 | 本接口是软仿实现的，先将操作数转为float类型，通过tanhf\(\)接口实现后再转回half类型，调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>tanhf(float in)<br>template<br>__cvt_half(T in) |
| `htrunc` | 是 | 本接口是软仿实现的，调用的built-in接口如下：<br>floorf(half in)<br>ceilf(half in) |

#### half类型比较函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__heq` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hequ` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hge` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hgeu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hgt` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hgtu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hisinf` | 否 |  |
| `__hisnan` | 否 |  |
| `__hle` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hleu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hlt` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hltu` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hmax` | 是 | 本接口是软仿实现的，使用的built-in接口如下：<br>bool __hisnan(bfloat16_t x);<br>bfloat16_t __max(bfloat16_t in1, bfloat16_t in2) |
| `__hmax_nan` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hmin` | 是 | 本接口是软仿实现的，使用的built-in接口如下：<br>bool __isnan(bfloat16_t x);<br>bfloat16_t __min(bfloat16_t in1, bfloat16_t in2); |
| `__hmin_nan` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hne` | 未知 | 未在参考目录中找到对应API文档。 |
| `__hneu` | 未知 | 未在参考目录中找到对应API文档。 |

#### half类型算术函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__habs` | 是 | 本接口是软仿实现的，使用操作符“&”实现绝对值计算：<br>bits &= 0x7FFF； |
| `__hadd` | 否 |  |
| `__hdiv` | 否 |  |
| `__hfma` | 否 |  |
| `__hfma_relu` | 是 | 本接口是built-in接口，也是软仿实现的，通过两个接口嵌套调用实现：<br>__hmax_nan(__fma(a,b,c),0); |
| `__hmul` | 否 |  |
| `__hneg` | 否 |  |
| `__hsub` | 否 |  |

#### half类型精度转换函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__bfloat162half_rd` | 否 |  |
| `__bfloat162half_rd_sat` | 否 |  |
| `__bfloat162half_rn` | 否 |  |
| `__bfloat162half_rn_sat` | 否 |  |
| `__bfloat162half_rna` | 否 |  |
| `__bfloat162half_rna_sat` | 否 |  |
| `__bfloat162half_ru` | 否 |  |
| `__bfloat162half_ru_sat` | 否 |  |
| `__bfloat162half_rz` | 否 |  |
| `__bfloat162half_rz_sat` | 否 |  |
| `__float22half2_rd` | 否 |  |
| `__float22half2_rd_sat` | 否 |  |
| `__float22half2_rn` | 否 |  |
| `__float22half2_rn_sat` | 否 |  |
| `__float22half2_rna` | 否 |  |
| `__float22half2_rna_sat` | 否 |  |
| `__float22half2_ro` | 否 |  |
| `__float22half2_ro_sat` | 否 |  |
| `__float22half2_ru` | 否 |  |
| `__float22half2_ru_sat` | 否 |  |
| `__float22half2_rz` | 否 |  |
| `__float22half2_rz_sat` | 否 |  |
| `__float2half` | 否 |  |
| `__float2half_rd` | 否 |  |
| `__float2half_rd_sat` | 否 |  |
| `__float2half_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2half_rn_sat` | 否 |  |
| `__float2half_rna` | 否 |  |
| `__float2half_rna_sat` | 否 |  |
| `__float2half_ro` | 否 |  |
| `__float2half_ro_sat` | 否 |  |
| `__float2half_ru` | 否 |  |
| `__float2half_ru_sat` | 否 |  |
| `__float2half_rz` | 否 |  |
| `__float2half_rz_sat` | 否 |  |
| `__floats2half2_rn` | 是 | 本接口是软仿实现的，将两个float类型数据分别转换成half类型并填充到half2类型数据的两部分，直接调用的built-in接口如下：<br>template<br>__cvt_half(T in) |
| `__half22float2` | 否 |  |
| `__half2float` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__half2half_rd` | 否 |  |
| `__half2half_rn` | 未知 | 未在参考目录中找到对应API文档。 |
| `__half2half_rna` | 否 |  |
| `__half2half_ru` | 否 |  |
| `__half2half_rz` | 否 |  |
| `__half2int_rd` | 否 |  |
| `__half2int_rn` | 否 |  |
| `__half2int_rna` | 否 |  |
| `__half2int_ru` | 否 |  |
| `__half2int_rz` | 否 |  |
| `__half2ll_rd` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为int64_t类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_int64_t(T in) |
| `__half2ll_rn` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为int64_t类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_int64_t(T in) |
| `__half2ll_rna` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为int64_t类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_int64_t(T in) |
| `__half2ll_ru` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为int64_t类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_int64_t(T in) |
| `__half2ll_rz` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为int64_t类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_int64_t(T in) |
| `__half2uint_rd` | 否 |  |
| `__half2uint_rn` | 否 |  |
| `__half2uint_rna` | 否 |  |
| `__half2uint_ru` | 否 |  |
| `__half2uint_rz` | 否 |  |
| `__half2ull_rd` | 是 | 本接口是软仿实现的，先将half类型数据转换为float类型，再转换为usigned long long int类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_uint64_t(T in) |
| `__half2ull_rn` | 是 | 本接口是软仿实现的，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_uint64_t(T in) |
| `__half2ull_rna` | 是 | 本接口是软仿实现的，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_uint64_t(T in) |
| `__half2ull_ru` | 是 | 本接口是软仿实现的，先将half类型数据转换为float类型，再转换为usigned long long int类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_uint64_t(T in) |
| `__half2ull_rz` | 是 | 本接口是软仿实现的，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_uint64_t(T in) |
| `__halves2half2` | 未知 | 接口实现为将输入的两个half类型类型数据分别填充到half2类型数据的两个分量，并填充到half2数据类型中。<br>half2 tmp;<br>tmp.x = x;<br>tmp.y = y;<br>return tmp; |
| `__high2float` | 否 |  |
| `__high2half` | 未知 | 接口实现为直接返回输入half2类型数据的高16位。<br>return x.y; |
| `__high2half2` | 未知 | 接口实现为将两个输入数据的高16位分别填充到half2的两部分并返回：<br>// 在half2这个结构体中，成员属性x代表低16位，成员属性y代表高16位<br>half2 tmp;<br>tmp.x = x.y;<br>tmp.y = y.y;<br>return tmp; |
| `__highs2half2` | 未知 | 接口实现为分别提取两个half2输入的高16位，并填充到half2数据类型中。<br>half2 tmp;<br>tmp.x = x.y;<br>tmp.y = y.y;<br>return tmp; |
| `__int2half_rd` | 否 |  |
| `__int2half_rd_sat` | 否 |  |
| `__int2half_rn` | 否 |  |
| `__int2half_rn_sat` | 否 |  |
| `__int2half_rna` | 否 |  |
| `__int2half_rna_sat` | 否 |  |
| `__int2half_ru` | 否 |  |
| `__int2half_ru_sat` | 否 |  |
| `__int2half_rz` | 否 |  |
| `__int2half_rz_sat` | 否 |  |
| `__ll2half_rd` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型并返回，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ll2half_rn` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型并返回，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ll2half_rna` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型并返回，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ll2half_ru` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型并返回，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ll2half_rz` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型并返回，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__low2float` | 否 |  |
| `__low2half` | 未知 | 接口实现为直接返回half2类型数据中的低16位：<br>// 在half2这个结构体中，成员属性x代表低16位，成员属性y代表高16位<br>return x.x; |
| `__low2half2` | 未知 | 接口实现为将输入数据的低16位分别填充到half2的两部分并返回：<br>// 在half2这个结构体中，成员属性x代表低16位，成员属性y代表高16位<br>half2 tmp;<br>tmp.x = x.x;<br>tmp.y = x.x;<br>return tmp; |
| `__lowhigh2highlow` | 未知 | 接口实现为将输入half2类型数据的高16位和低16位分别赋值给新的half2类型数据的低16位和高16位并返回。<br>half2 tmp;<br>tmp.x = x.y;<br>tmp.y = x.x;<br>return tmp; |
| `__lows2half2` | 未知 | 接口实现为将两个输入数据的低16位分别填充到half2的两部分并返回：<br>// 在half2这个结构体中，成员属性x代表低16位，成员属性y代表高16位<br>half2 tmp;<br>tmp.x = x.x;<br>tmp.y = y.x;<br>return tmp; |
| `__uint2half_rd` | 否 |  |
| `__uint2half_rd_sat` | 否 |  |
| `__uint2half_rn` | 否 |  |
| `__uint2half_rn_sat` | 否 |  |
| `__uint2half_rna` | 否 |  |
| `__uint2half_rna_sat` | 否 |  |
| `__uint2half_ru` | 否 |  |
| `__uint2half_ru_sat` | 否 |  |
| `__uint2half_rz` | 否 |  |
| `__uint2half_rz_sat` | 否 |  |
| `__ull2half_rd` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ull2half_rn` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ull2half_rna` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ull2half_ru` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ull2half_rz` | 是 | 本接口是软仿实现的，先将输入数据转换为float类型，再转换为half类型，直接调用的built-in接口如下：<br>template<br>__cvt_float(T in)<br>template<br>__cvt_half(T in) |
| `__ushort_as_half` | 是 | 本接口是软仿实现的。通过定义一个包含ushort类型成员和half类型成员的共用体，将输入的ushort类型数据赋值给ushort类型成员，并返回half类型成员。 |

### 数据类型转换

#### 类型转换函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__float2float_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2float_rn` | 否 |  |
| `__float2float_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2float_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2float_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2int_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2int_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2int_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2int_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2int_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ll_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ll_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ll_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ll_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ll_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2uint_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2uint_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2uint_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2uint_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2uint_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ull_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ull_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ull_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ull_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float2ull_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float_as_int` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__float_as_uint` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2float_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2float_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2float_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2float_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int2float_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__int_as_float` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2float_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2float_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2float_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2float_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ll2float_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2float_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2float_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2float_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2float_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint2float_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__uint_as_float` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2float_rd` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2float_rn` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2float_rna` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2float_ru` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `__ull2float_rz` | 未知 | 参考文档中未找到“API映射关系”章节。 |

### 整型数学库函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `__brev` | 是 | 本接口是built-in接口，unsigned int类型接口是非软仿的，其他数据类型是用多条指令拼接过软仿实现的。 |
| `__byte_perm` | 否 |  |
| `__clz` | 否 |  |
| `__ffs` | 否 |  |
| `__hadd` | 否 |  |
| `__mul24` | 否 |  |
| `__mul64hi` | 否 |  |
| `__mul_i32toi64` | 否 |  |
| `__mulhi` | 否 |  |
| `__popc` | 否 |  |
| `__rhadd` | 否 |  |
| `__sad` | 否 |  |
| `__uhadd` | 否 |  |
| `__umul24` | 否 |  |
| `__umul64hi` | 否 |  |
| `__umulhi` | 否 |  |
| `__urhadd` | 否 |  |
| `__usad` | 否 |  |
| `labs` | 是 | 本接口是直接调用built-in接口实现的，built-in接口是通过软仿实现的：<br>long abs(long in) |
| `llabs` | 是 | 本接口是直接调用built-in接口实现的，built-in接口是通过软仿实现的：<br>long long abs(long long in) |
| `llmax` | 是 | 本接口是直接调用built-in接口实现的，是用多条int类型硬件指令拼接过软仿实现的，built-in接口定义如下：<br>long long max(long long in1,long long in2) |
| `llmin` | 否 |  |
| `max` | 未知 | 与built-in接口映射关系：<br>// 不同原型区别在于操作数的数据类型不同<br>void vmax(vector_s8 &dst, vector_s8 src0, vector_s8 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_s16 &dst, vector_s16 src0, vector_s16 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_s32 &dst, vector_s32 src0, vector_s32 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_u8 &dst, vector_u8 src0, vector_u8 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_u16 &dst, vector_u16 src0, vector_u16 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_u32 &dst, vector_u32 src0, vector_u32 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_f16 &dst, vector_f16 src0, vector_f16 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_f32 &dst, vector_f32 src0, vector_f32 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_2xvl_s64 &dst, vector_2xvl_s64 src0, vector_2xvl_s64 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmax(vector_2xvl_u64 &dst, vector_2xvl_u64 src0, vector_2xvl_u64 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ; |
| `min` | 未知 | 与built-in接口映射关系：<br>// 不同原型区别在于操作数的数据类型不同<br>void vmin(vector_s8 &dst, vector_s8 src0, vector_s8 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_s16 &dst, vector_s16 src0, vector_s16 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_s32 &dst, vector_s32 src0, vector_s32 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_u8 &dst, vector_u8 src0, vector_u8 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_u16 &dst, vector_u16 src0, vector_u16 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_u32 &dst, vector_u32 src0, vector_u32 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_f16 &dst, vector_f16 src0, vector_f16 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_f32 &dst, vector_f32 src0, vector_f32 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_2xvl_s64 &dst, vector_2xvl_s64 src0, vector_2xvl_s64 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ;<br>void vmin(vector_2xvl_u64 &dst, vector_2xvl_u64 src0, vector_2xvl_u64 src1, vector_bool mask, Literal mode = Mode_Unknown_Literalype()) ; |
| `ullmax` | 未知 | 未在参考目录中找到对应API文档。 |
| `ullmin` | 未知 | 未在参考目录中找到对应API文档。 |
| `umax` | 未知 | 未在参考目录中找到对应API文档。 |
| `umin` | 未知 | 未在参考目录中找到对应API文档。 |

## 访存函数

| API名 | API是否软仿 | 备注 |
| --- | --- | --- |
| `asc_dcci_entire` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `asc_dcci_single` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `asc_ldca` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `asc_ldcg` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `asc_stcg` | 未知 | 参考文档中未找到“API映射关系”章节。 |
| `asc_stwt` | 未知 | 参考文档中未找到“API映射关系”章节。 |
