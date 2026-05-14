# API列表

## 同步与内存栅栏

**表 1**  同步接口

| 接口名 | 功能描述 |
| --- | --- |
| asc_syncthreads | 等待当前thread block内所有thread代码都执行到该函数位置。 |

**表 2**  内存栅栏接口

| 接口名 | 功能描述 |
| --- | --- |
| asc_threadfence | 用于保证不同核对同一份全局、共享内存的访问过程中，写入操作的时序性。 |
| asc_threadfence_block | 用于协调同一线程块（Thread Block）内线程之间的内存操作顺序，确保某一线程在调用asc_threadfence_block()之前的所有内存读写操作对同一线程块内的其他线程可见。 |

## 原子操作

**表 3**  原子操作

| 接口名 | 功能描述 |
| --- | --- |
| asc_atomic_add | 对Unified Buffer或Global Memory上的数据与指定数据执行原子加操作，即将指定数据累加到Unified Buffer或Global Memory的数据中。 |
| asc_atomic_sub | 对Unified Buffer或Global Memory上的数据与指定数据执行原子减操作，即在Unified Buffer或Global Memory的数据上减去指定数据。 |
| asc_atomic_exch | 对Unified Buffer或Global Memory地址做原子赋值操作，即将指定数据赋值到Unified Buffer或Global Memory地址中。 |
| asc_atomic_max | 对Unified Buffer或Global Memory数据做原子求最大值操作，即将Unified Buffer或Global Memory的数据与指定数据中的最大值赋值到Unified Buffer或Global Memory地址中。 |
| asc_atomic_min | 对Unified Buffer或Global Memory数据做原子求最小值操作，即将Unified Buffer或Global Memory的数据与指定数据中的最小值赋值到Unified Buffer或Global Memory地址中。 |
| asc_atomic_inc | 对Unified Buffer或Global Memory上address的数值进行原子加1操作，如果address上的数值大于等于指定数值val，则对address赋值为0，否则将address上数值加1。 |
| asc_atomic_dec | 对Unified Buffer或Global Memory上address的数值进行原子减1操作，如果address上的数值等于0或大于指定数值val，则对address赋值为val，否则将address上数值减1。 |
| asc_atomic_cas | 对Unified Buffer或Global Memory上address的数值进行原子比较赋值操作，如果address上的数值等于指定数值compare，则对address赋值为指定数值val，否则address的数值不变。 |
| asc_atomic_and | 对Unified Buffer或Global Memory上address的数值与指定数值val进行原子与（&）操作，即将address数值与（&）val的结果赋值到Unified Buffer或Global Memory上。 |
| asc_atomic_or | 对Unified Buffer或Global Memory上address的数值与指定数值val进行原子或（|）操作，即将address数值或（|）val的结果赋值到Unified Buffer或Global Memory上。 |
| asc_atomic_xor | 对Unified Buffer或Global Memory上address的数值与指定数值val进行原子异或（^）操作，即将address数值异或（^）val的结果赋值到Unified Buffer或Global Memory上。 |

## Warp函数

**表 4**  Warp Vote类函数

| 接口名 | 功能描述 |
| --- | --- |
| asc_all | 判断是否所有活跃线程的输入均不为0。 |
| asc_any | 判断是否有活跃线程的输入不为0。 |
| asc_ballot | 判断Warp内每个活跃线程的输入是否不为0。 |
| asc_activemask | 查看Warp内所有线程是否为活跃状态。 |

**表 5**  Warp Shfl类函数

| 接口名 | 功能描述 |
| --- | --- |
| asc_shfl | 获取Warp内指定线程srcLane输入的用于交换的var值。 |
| asc_shfl_up | 获取Warp内当前线程向前偏移delta（当前线程LaneId-delta）的线程输入的用于交换的var值。 |
| asc_shfl_down | 获取Warp内当前线程向后偏移delta（当前线程LaneId+delta）的线程输入的用于交换的var值。 |
| asc_shfl_xor | 获取Warp内当前线程LaneId与输入laneMask做异或操作（LaneId^laneMask）得到的dstLaneId对应线程输入的用于交换的var值。 |

**表 6**  Warp Reduce类函数

| 接口名 | 功能描述 |
| --- | --- |
| asc_reduce_add | 对Warp内所有活跃线程输入的val求和。 |
| asc_reduce_max | 对Warp内所有活跃线程输入的val求最大值。 |
| asc_reduce_min | 对Warp内所有活跃线程输入val求最小值。 |

## 数学函数

**表 7**  half类型算术函数

| 接口名 | 功能描述 |
| --- | --- |
| __habs | 获取输入数据的绝对值。 |
| __hfma | 对输入数据x、y、z，计算x与y相乘加上z的结果。 |
| __hadd | 计算两个half类型数据的相加结果，并遵循CAST_RINT模式舍入。 |
| __hsub | 计算两个half类型数据的相减结果，并遵循CAST_RINT模式舍入。 |
| __hmul | 计算两个half类型数据的相乘结果，并遵循CAST_RINT模式舍入。 |
| __hdiv | 计算两个half类型数据的相除结果，并遵循CAST_RINT模式舍入。 |
| __hneg | 获取输入half类型数据的负值。 |
| __hfma_relu | 对输入half类型数据x、y、z，计算x与y相乘加上z的结果，并遵循CAST_RINT模式舍入。负数结果置为0。 |

**表 8**  half类型比较函数

| 接口名 | 功能描述 |
| --- | --- |
| __hmax | 获取两个输入数据中的最大值。 |
| __hmin | 获取两个输入数据中的最小值。 |
| __hisnan | 判断浮点数是否为nan。 |
| __hisinf | 判断浮点数是否为无穷。 |
| __heq | 比较两个half类型数据是否相等，相等时返回true。 |
| __hne | 比较两个half类型数据是否不相等，不相等时返回true。 |
| __hle | 比较两个half类型数据，仅当第一个数小于或等于第二个数时返回true。 |
| __hge | 比较两个half类型数据，仅当第一个数大于或等于第二个数时返回true。 |
| __hlt | 比较两个half类型数据，仅当第一个数小于第二个数时返回true。 |
| __hgt | 比较两个half类型数据，仅当第一个数大于第二个数时返回true。 |
| __hequ | 比较两个half类型数据是否相等，相等时返回true。若任一输入为nan，返回true。 |
| __hneu | 比较两个half类型数据是否不相等，不相等时返回true。若任一输入为nan，返回true。 |
| __hleu | 比较两个half类型数据，当第一个数小于或等于第二个数时返回true。若任一输入为nan，返回true。 |
| __hgeu | 比较两个half类型数据，当第一个数大于或等于第二个数时返回true。若任一输入为nan，返回true。 |
| __hltu | 比较两个half类型数据，当第一个数小于第二个数时返回true。若任一输入为nan，返回true。 |
| __hgtu | 比较两个half类型数据，当第一个数大于第二个数时返回true。若任一输入为nan，返回true。 |
| __hmax_nan | 获取两个输入数据中的最大值。任一输入为nan时返回nan。 |
| __hmin_nan | 获取两个输入数据中的最小值。任一输入为nan时返回nan。 |

**表 9**  half类型数学库函数

| 接口名 | 功能描述 |
| --- | --- |
| htanh | 获取输入数据的三角函数双曲正切值。 |
| hexp | 指定输入x，获取e的x次方。 |
| hexp2 | 指定输入x，获取2的x次方。 |
| hexp10 | 指定输入x，获取10的x次方。 |
| hlog | 获取以e为底，输入数据的对数。 |
| hlog2 | 获取以2为底，输入数据的对数。 |
| hlog10 | 获取以10为底，输入数据的对数。 |
| hcos | 获取输入数据的三角函数余弦值。 |
| hsin | 获取输入数据的三角函数正弦值。 |
| hsqrt | 获取输入数据x的平方根。 |
| hrsqrt | 获取输入数据x的平方根的倒数。 |
| hrcp | 获取输入数据x的倒数。 |
| hrint | 获取与输入数据最接近的整数，若存在两个同样接近的整数，则获取其中的偶数。 |
| hfloor | 获取小于或等于输入数据的最大整数值。 |
| hceil | 获取大于或等于输入数据的最小整数值。 |
| htrunc | 获取对输入数据的浮点数截断后的整数。 |

**表 10**  half类型精度转换函数

| 接口名 | 功能描述 |
| --- | --- |
| __float2half | 获取输入遵循CAST_RINT模式转换成的半精度浮点数。 |
| __float2half_rn | 获取输入遵循CAST_RINT模式转换成的半精度浮点数。 |
| __float2half_rn_sat | 饱和模式下获取输入遵循CAST_RINT模式转换成的半精度浮点数。 |
| __float22half2_rn_sat | 饱和模式下获取输入的两个分量遵循CAST_RINT模式转换成的half2类型数据。 |
| __float2half_rz | 获取输入遵循CAST_TRUNC模式转换成的半精度浮点数。 |
| __float2half_rz_sat | 饱和模式下获取输入遵循CAST_TRUNC模式转换成的半精度浮点数。 |
| __float22half2_rz | 获取输入的两个分量遵循CAST_TRUNC模式转换成的half2类型数据。 |
| __float22half2_rz_sat | 饱和模式下获取输入的两个分量遵循CAST_TRUNC模式转换成的half2类型数据。 |
| __float2half_rd | 获取输入遵循CAST_FLOOR模式转换成的半精度浮点数。 |
| __float2half_rd_sat | 饱和模式下获取输入遵循CAST_FLOOR模式转换成的半精度浮点数。 |
| __float22half2_rd | 获取输入的两个分量遵循CAST_FLOOR模式转换成的half2类型数据。 |
| __float22half2_rd_sat | 饱和模式下获取输入的两个分量遵循CAST_FLOOR模式转换成的half2类型数据。 |
| __float2half_ru | 获取输入遵循CAST_CEIL模式转换成的半精度浮点数。 |
| __float2half_ru_sat | 饱和模式下获取输入遵循CAST_CEIL模式转换成的半精度浮点数。 |
| __float22half2_ru | 获取输入的两个分量遵循CAST_CEIL模式转换成的half2类型数据。 |
| __float22half2_ru_sat | 饱和模式下获取输入的两个分量遵循CAST_CEIL模式转换成的half2类型数据。 |
| __float2half_rna | 获取输入遵循CAST_ROUND模式转换成的半精度浮点数。 |
| __float2half_rna_sat | 饱和模式下获取输入遵循CAST_ROUND模式转换成的半精度浮点数。 |
| __float22half2_rna | 获取输入的两个分量遵循CAST_ROUND模式转换成的half2类型数据。 |
| __float22half2_rna_sat | 饱和模式下获取输入的两个分量遵循CAST_ROUND模式转换成的half2类型数据。 |
| __float2half_ro | 获取输入遵循CAST_ODD模式转换成的半精度浮点数。 |
| __float2half_ro_sat | 饱和模式下获取输入遵循CAST_ODD模式转换成的半精度浮点数。 |
| __float22half2_ro | 获取输入的两个分量遵循CAST_ODD模式转换成的half2类型数据。 |
| __float22half2_ro_sat | 饱和模式下获取输入的两个分量遵循CAST_ODD模式转换成的half2类型数据。 |
| __half2float | 获取输入转换成的浮点数。 |
| __half2half_rn | 获取输入遵循CAST_RINT模式取整后的half类型数据。 |
| __half2half_rz | 获取输入遵循CAST_TRUNC模式取整后的half类型数据。 |
| __half2half_rd | 获取输入遵循CAST_FLOOR模式取整后的half类型数据。 |
| __half2half_ru | 获取输入遵循CAST_CEIL模式取整后的half类型数据。 |
| __half2half_rna | 获取输入遵循CAST_ROUND模式取整后的half类型数据。 |
| __half2uint_rn | 获取输入遵循CAST_RINT模式转换成的无符号整数。 |
| __half2uint_rz | 获取输入遵循CAST_TRUNC模式转换成的无符号整数。 |
| __half2uint_rd | 获取输入遵循CAST_FLOOR模式转换成的无符号整数。 |
| __half2uint_ru | 获取输入遵循CAST_CEIL模式转换成的无符号整数。 |
| __half2uint_rna | 获取输入遵循CAST_ROUND模式转换成的无符号整数。 |
| __half2int_rn | 获取输入遵循CAST_RINT模式转换成的有符号整数。 |
| __half2int_rz | 获取输入遵循CAST_TRUNC模式转换成的有符号整数。 |
| __half2int_rd | 获取输入遵循CAST_FLOOR模式转换成的有符号整数。 |
| __half2int_ru | 获取输入遵循CAST_CEIL模式转换成的有符号整数。 |
| __half2int_rna | 获取输入遵循CAST_ROUND模式转换成的有符号整数。 |
| __half2ull_rn | 获取输入遵循CAST_RINT模式转换成的64位无符号整数。 |
| __half2ull_rz | 获取输入遵循CAST_TRUNC模式转换成的64位无符号整数。 |
| __half2ull_rd | 获取输入遵循CAST_FLOOR模式转换成的64位无符号整数。 |
| __half2ull_ru | 获取输入遵循CAST_CEIL模式转换成的64位无符号整数。 |
| __half2ull_rna | 获取输入遵循CAST_ROUND模式转换成的64位无符号整数。 |
| __half2ll_rn | 获取输入遵循CAST_RINT模式转换成的64位有符号整数。 |
| __half2ll_rz | 获取输入遵循CAST_TRUNC模式转换成的64位有符号整数。 |
| __half2ll_rd | 获取输入遵循CAST_FLOOR模式转换成的64位有符号整数。 |
| __half2ll_ru | 获取输入遵循CAST_CEIL模式转换成的64位有符号整数。 |
| __half2ll_rna | 获取输入遵循CAST_ROUND模式转换成的64位有符号整数。 |
| __bfloat162half_rn | 获取输入遵循CAST_RINT模式转换成的half类型数据。 |
| __bfloat162half_rn_sat | 饱和模式下获取输入遵循CAST_RINT模式转换成的half类型数据。 |
| __bfloat162half_rz | 获取输入遵循CAST_TRUNC模式转换成的half类型数据。 |
| __bfloat162half_rz_sat | 饱和模式下获取输入遵循CAST_TRUNC模式转换成的half类型数据。 |
| __bfloat162half_rd | 获取输入遵循CAST_FLOOR模式转换成的half类型数据。 |
| __bfloat162half_rd_sat | 饱和模式下获取输入遵循CAST_FLOOR模式转换成的half类型数据。 |
| __bfloat162half_ru | 获取输入遵循CAST_CEIL模式转换成的half类型数据。 |
| __bfloat162half_ru_sat | 饱和模式下获取输入遵循CAST_CEIL模式转换成的half类型数据。 |
| __bfloat162half_rna | 获取输入遵循CAST_ROUND模式转换成的half类型数据。 |
| __bfloat162half_rna_sat | 饱和模式下获取输入遵循CAST_ROUND模式转换成的half类型数据。 |
| __uint2half_rn | 获取输入遵循CAST_RINT模式转换成的half类型数据。 |
| __uint2half_rn_sat | 饱和模式下获取输入的uint32数据转换成的half数据，并遵循CAST_RINT模式。 |
| __uint2half_rz | 获取输入遵循CAST_TRUNC模式转换成的half类型数据。 |
| __uint2half_rz_sat | 饱和模式下获取输入的uint32数据转换成的half数据，并遵循CAST_TRUNC模式。 |
| __uint2half_rd | 获取输入遵循CAST_FLOOR模式转换成的half类型数据。 |
| __uint2half_rd_sat | 饱和模式下获取输入的uint32数据转换成的half数据，并遵循CAST_FLOOR模式。 |
| __uint2half_ru | 获取输入遵循CAST_CEIL模式转换成的half类型数据。 |
| __uint2half_ru_sat | 饱和模式下获取输入的uint32数据转换成的half数据，并遵循CAST_CEIL模式。 |
| __uint2half_rna | 获取输入遵循CAST_ROUND模式转换成的half类型数据。 |
| __uint2half_rna_sat | 饱和模式下获取输入的uint32数据转换成的half数据，并遵循CAST_ROUND模式。 |
| __int2half_rn | 获取输入遵循CAST_RINT模式转换成的half类型数据。 |
| __int2half_rn_sat | 饱和模式下获取输入的int32数据转换成的half数据，并遵循CAST_RINT模式。 |
| __int2half_rz | 获取输入遵循CAST_TRUNC模式转换成的half类型数据。 |
| __int2half_rz_sat | 饱和模式下获取输入的int32数据转换成的half数据，并遵循CAST_TRUNC模式。 |
| __int2half_rd | 获取输入遵循CAST_FLOOR模式转换成的half类型数据。 |
| __int2half_rd_sat | 饱和模式下获取输入的int32数据转换成的half数据，并遵循CAST_FLOOR模式。 |
| __int2half_ru | 获取输入遵循CAST_CEIL模式转换成的half类型数据。 |
| __int2half_ru_sat | 饱和模式下获取输入的int32数据转换成的half数据，并遵循CAST_CEIL模式。 |
| __int2half_rna | 获取输入遵循CAST_ROUND模式转换成的half类型数据。 |
| __int2half_rna_sat | 饱和模式下获取输入的int32数据转换成的half数据，并遵循CAST_ROUND模式。 |
| __ull2half_rn | 获取输入遵循CAST_RINT模式转换成的half类型数据。 |
| __ull2half_rz | 获取输入遵循CAST_TRUNC模式转换成的half类型数据。 |
| __ull2half_rd | 获取输入遵循CAST_FLOOR模式转换成的half类型数据。 |
| __ull2half_ru | 获取输入遵循CAST_CEIL模式转换成的half类型数据。 |
| __ull2half_rna | 获取输入遵循CAST_ROUND模式转换成的half类型数据。 |
| __ll2half_rn | 获取输入遵循CAST_RINT模式转换成的half类型数据。 |
| __ll2half_rz | 获取输入遵循CAST_TRUNC模式转换成的half类型数据。 |
| __ll2half_rd | 获取输入遵循CAST_FLOOR模式转换成的half类型数据。 |
| __ll2half_ru | 获取输入遵循CAST_CEIL模式转换成的half类型数据。 |
| __ll2half_rna | 获取输入遵循CAST_ROUND模式转换成的half类型数据。 |
| __floats2half2_rn | 将输入的数据x，y遵循CAST_RINT模式分别转换为bfloat16类型并填充到half2的前后两部分，返回转换后的half2类型数据。 |
| __float22half2_rn | 将float2类型数据遵循CAST_RINT模式转换为half2类型，返回转换后的half2类型数据。 |
| __low2half | 返回输入数据的低16位。 |
| __low2half2 | 将输入数据的低16位填充到half2并返回。 |
| __high2half | 提取输入half2的高16位，并返回 |
| __high2half2 | 将输入数据的的高16位填充到half2并返回结果。 |
| __highs2half2 | 分别提取两个half2输入的高16位，并填充到half2中。返回填充后的数据。 |
| __lows2half2 | 分别提取两个half2输入的低16位，并填充到half2中。返回填充后的数据。 |
| __halves2half2 | 将输入的数据分别填充为half2前后两个分量，返回填充后数据。 |
| __half22float2 | 将half2的两个分量分别转换为float，并填充到float2返回。 |
| __ushort_as_half | 将unsigned short int的按位重新解释为half，即将unsigned short int的数据存储的位按照half的格式进行读取。 |

**表 11**  half2类型算术函数

| 接口名 | 功能描述 |
| --- | --- |
| __haddx2 | 计算两个half2类型数据各分量的相加结果，并遵循CAST_RINT模式舍入。 |
| __hsubx2 | 计算两个half2类型数据各分量的相减结果，并遵循CAST_RINT模式舍入。 |
| __hmulx2 | 计算两个half2类型数据各分量的相乘结果，并遵循CAST_RINT模式舍入。 |
| __hdivx2 | 计算两个half2类型数据各分量的相除结果，并遵循CAST_RINT模式舍入。 |
| __habsx2 | 计算输入half2类型数据各分量的绝对值。 |
| __hfmax2 | 计算两个half2类型数据各分量的乘加的结果（前两个输入相乘后与第三个输入相加），并遵循CAST_RINT模式舍入。 |
| __hnegx2 | 获取输入half2类型数据各分量的负值。 |
| __hfmax2_relu | 计算两个half2类型数据各分量的乘加的结果（前两个输入相乘后与第三个输入相加），并遵循CAST_RINT模式舍入。负数结果置为0。 |
| __hcmadd | 将三个half2输入视为复数（第一个分量为实部，第二个分量为虚部），执行复数乘加运算x*y+z。 |

**表 12**  half2类型比较函数

| 接口名 | 功能描述 |
| --- | --- |
| __hbeqx2 | 比较两个half2类型数据的两个分量是否相等，仅当两个分量均相等时返回true。 |
| __hbnex2 | 比较两个half2类型数据的两个分量是否不相等，仅当两个分量均不相等时返回true。 |
| __hblex2 | 比较两个half2类型数据的两个分量，仅当两个分量均满足第一个数小于或等于第二个数时返回true。 |
| __hbgex2 | 比较两个half2类型数据的两个分量，仅当两个分量均满足第一个数大于或等于第二个数时返回true。 |
| __hbltx2 | 比较两个half2类型数据的两个分量，仅当两个分量均满足第一个数小于第二个数时返回true。 |
| __hbgtx2 | 比较两个half2类型数据的两个分量，仅当两个分量均满足第一个数大于第二个数时返回true。 |
| __hbequx2 | 比较两个half2类型数据的两个分量是否相等，当两个分量均相等时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbneux2 | 比较两个half2类型数据的两个分量是否不相等，当两个分量均不相等时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbleux2 | 比较两个half2类型数据的两个分量，当两个分量均满足第一个数小于或等于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbgeux2 | 比较两个half2类型数据的两个分量，当两个分量均满足第一个数大于或等于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbltux2 | 比较两个half2类型数据的两个分量，当两个分量均满足第一个数小于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbgtux2 | 比较两个half2类型数据的两个分量，当两个分量均满足第一个数大于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __heqx2 | 比较两个half2类型数据的两个分量，如果分量相等，则对应比较结果为1.0，否则为0.0。 |
| __hnex2 | 比较两个half2类型数据的两个分量，如果分量不相等，则对应比较结果为1.0，否则为0.0。 |
| __hlex2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数小于或等于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hgex2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数大于或等于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hltx2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数小于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hgtx2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数大于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hequx2 | 比较两个half2类型数据的两个分量，如果分量相等，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hneux2 | 比较两个half2类型数据的两个分量，如果分量不相等，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hleux2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数小于或等于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hgeux2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数大于或等于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hltux2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数小于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hgtux2 | 比较两个half2类型数据的两个分量，如果分量满足第一个数大于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __heqx2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量相等，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hnex2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量不相等，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hlex2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hgex2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hltx2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hgtx2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hequx2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量相等，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hneux2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量不相等，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hleux2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hgeux2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hltux2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hgtux2_mask | 比较两个half2类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __isnanx2 | 判断half2类型数据的两个分量是否为nan。 |
| __hmaxx2 | 获取两个half2类型数据各分量的最大值。 |
| __hmaxx2_nan | 获取两个half2类型数据各分量的最大值。任一分量为nan时对应结果为nan。 |
| __hminx2 | 获取两个half2类型数据各分量的最小值。 |
| __hminx2_nan | 获取两个half2类型数据各分量的最小值。任一分量为nan时对应结果为nan。 |

**表 13**  half2类型数学库函数

| 接口名 | 功能描述 |
| --- | --- |
| h2tanh | 获取输入数据各元素的三角函数双曲正切值。 |
| h2exp | 指定输入x，对x的各元素，获取e的该元素次方。 |
| h2exp2 | 指定输入x，对x的各元素，获取2的该元素次方。 |
| h2exp10 | 指定输入x，对x的各元素，获取10的该元素次方。 |
| h2log | 获取以e为底，输入数据各元素的对数。 |
| h2log2 | 获取以2为底，输入数据各元素的对数。 |
| h2log10 | 获取以10为底，输入数据各元素的对数。 |
| h2cos | 获取输入数据各元素的三角函数余弦值。 |
| h2sin | 获取输入数据各元素的三角函数正弦值。 |
| h2sqrt | 获取输入数据x各元素的平方根。 |
| h2rsqrt | 获取输入数据x各元素的平方根的倒数。 |
| h2rcp | 获取输入数据x各元素的倒数。 |
| h2rint | 获取与输入数据各元素最接近的整数，若存在两个同样接近的整数，则获取其中的偶数。 |
| h2floor | 获取小于或等于输入数据各元素的最大整数值。 |
| h2ceil | 获取大于或等于输入数据各元素的最小整数值。 |
| h2trunc | 获取对输入数据各元素的浮点数截断后的整数。 |

**表 14**  bfloat16类型算术函数

| 接口名 | 功能描述 |
| --- | --- |
| __habs | 获取输入数据的绝对值。 |
| __hfma | 对输入数据x、y、z，计算x与y相乘加上z的结果。 |
| __hadd | 计算两个bfloat16类型数据的相加结果，并遵循CAST_RINT模式舍入。 |
| __hsub | 计算两个bfloat16类型数据的相减结果，并遵循CAST_RINT模式舍入。 |
| __hmul | 计算两个bfloat16类型数据的相乘结果，并遵循CAST_RINT模式舍入。 |
| __hdiv | 计算两个bfloat16类型数据的相除结果，并遵循CAST_RINT模式舍入。 |
| __hneg | 获取输入bfloat16类型数据的负值。 |
| __hfma_relu | 对输入bfloat16类型数据x、y、z，计算x与y相乘加上z的结果，并遵循CAST_RINT模式舍入。负数结果置为0。 |

**表 15**  bfloat16类型比较函数

| 接口名 | 功能描述 |
| --- | --- |
| __hmax | 获取两个输入数据中的最大值。 |
| __hmin | 获取两个输入数据中的最小值。 |
| __hisnan | 判断浮点数是否为nan。 |
| __hisinf | 判断浮点数是否为无穷。 |
| __heq | 比较两个bfloat16类型数据是否相等，相等时返回true。 |
| __hne | 比较两个bfloat16类型数据是否不相等，不相等时返回true。 |
| __hle | 比较两个bfloat16类型数据，仅当第一个数小于或等于第二个数时返回true。 |
| __hge | 比较两个bfloat16类型数据，仅当第一个数大于或等于第二个数时返回true。 |
| __hlt | 比较两个bfloat16类型数据，仅当第一个数小于第二个数时返回true。 |
| __hgt | 比较两个bfloat16类型数据，仅当第一个数大于第二个数时返回true。 |
| __hequ | 比较两个bfloat16类型数据是否相等，相等时返回true。若任一输入为nan，返回true。 |
| __hneu | 比较两个bfloat16类型数据是否不相等，不相等时返回true。若任一输入为nan，返回true。 |
| __hleu | 比较两个bfloat16类型数据，当第一个数小于或等于第二个数时返回true。若任一输入为nan，返回true。 |
| __hgeu | 比较两个bfloat16类型数据，当第一个数大于或等于第二个数时返回true。若任一输入为nan，返回true。 |
| __hltu | 比较两个bfloat16类型数据，当第一个数小于第二个数时返回true。若任一输入为nan，返回true。 |
| __hgtu | 比较两个bfloat16类型数据，当第一个数大于第二个数时返回true。若任一输入为nan，返回true。 |
| __hmax_nan | 获取两个输入数据中的最大值。任一输入为nan时返回nan。 |
| __hmin_nan | 获取两个输入数据中的最小值。任一输入为nan时返回nan。 |

**表 16**  bfloat16数学库函数

| 接口名 | 功能描述 |
| --- | --- |
| htanh | 获取输入数据的三角函数双曲正切值。 |
| hexp | 指定输入x，获取e的x次方。 |
| hexp2 | 指定输入x，获取2的x次方。 |
| hexp10 | 指定输入x，获取10的x次方。 |
| hlog | 获取以e为底，输入数据的对数。 |
| hlog2 | 获取以2为底，输入数据的对数。 |
| hlog10 | 获取以10为底，输入数据的对数。 |
| hcos | 获取输入数据的三角函数余弦值。 |
| hsin | 获取输入数据的三角函数正弦值。 |
| hsqrt | 获取输入数据x的平方根。 |
| hrsqrt | 获取输入数据x的平方根的倒数。 |
| hrcp | 获取输入数据x的倒数。 |
| hrint | 获取与输入数据最接近的整数，若存在两个同样接近的整数，则获取其中的偶数。 |
| hfloor | 获取小于或等于输入数据的最大整数值。 |
| hceil | 获取大于或等于输入数据的最小整数值。 |
| htrunc | 获取对输入数据的浮点数截断后的整数。 |

**表 17**  bfloat16类型精度转换函数

| 接口名 | 功能描述 |
| --- | --- |
| __float2bfloat16 | 获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __float2bfloat16_rn | 获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __float2bfloat16_rn_sat | 饱和模式下获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __float22bfloat162_rn_sat | 饱和模式下获取输入的两个分量遵循CAST_RINT模式转换成的bfloat16x2_t类型数据。 |
| __float2bfloat16_rz | 获取输入遵循CAST_TRUNC模式转换成的bfloat16类型数据。 |
| __float2bfloat16_rz_sat | 饱和模式下获取输入遵循CAST_TRUNC模式转换成的bfloat16类型数据。 |
| __float22bfloat162_rz | 获取输入的两个分量遵循CAST_TRUNC模式转换成的bfloat16x2_t类型数据。 |
| __float22bfloat162_rz_sat | 饱和模式下获取输入的两个分量遵循CAST_TRUNC模式转换成的bfloat16x2_t类型数据。 |
| __float2bfloat16_rd | 获取输入遵循CAST_FLOOR模式转换成的bfloat16类型数据。 |
| __float2bfloat16_rd_sat | 饱和模式下获取输入遵循CAST_FLOOR模式转换成的bfloat16类型数据。 |
| __float22bfloat162_rd | 获取输入的两个分量遵循CAST_FLOOR模式转换成的bfloat16x2_t类型数据。 |
| __float22bfloat162_rd_sat | 饱和模式下获取输入的两个分量遵循CAST_FLOOR模式转换成的bfloat16x2_t类型数据。 |
| __float2bfloat16_ru | 获取输入遵循CAST_CEIL模式转换成的bfloat16类型数据。 |
| __float2bfloat16_ru_sat | 饱和模式下获取输入遵循CAST_CEIL模式转换成的bfloat16类型数据。 |
| __float22bfloat162_ru | 获取输入的两个分量遵循CAST_CEIL模式转换成的bfloat16x2_t类型数据。 |
| __float22bfloat162_ru_sat | 饱和模式下获取输入的两个分量遵循CAST_CEIL模式转换成的bfloat16x2_t类型数据。 |
| __float2bfloat16_rna | 获取输入遵循CAST_ROUND模式转换成的bfloat16类型数据。 |
| __float2bfloat16_rna_sat | 饱和模式下获取输入遵循CAST_ROUND模式转换成的bfloat16类型数据。 |
| __float22bfloat162_rna | 获取输入的两个分量遵循CAST_ROUND模式转换成的bfloat16x2_t类型数据。 |
| __float22bfloat162_rna_sat | 饱和模式下获取输入的两个分量遵循CAST_ROUND模式转换成的bfloat16x2_t类型数据。 |
| __half2bfloat16_rn | 获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __half2bfloat16_rz | 获取输入遵循CAST_TRUNC模式转换成的bfloat16类型数据。 |
| __half2bfloat16_rd | 获取输入遵循CAST_FLOOR模式转换成的bfloat16类型数据。 |
| __half2bfloat16_ru | 获取输入遵循CAST_CEIL模式转换成的bfloat16类型数据。 |
| __half2bfloat16_rna | 获取输入遵循CAST_ROUND模式转换成的bfloat16类型数据。 |
| __bfloat162float | 获取输入转换为浮点数的结果。 |
| __bfloat162bfloat16_rn | 获取输入遵循CAST_RINT模式取整后的bfloat16_t类型数据。 |
| __bfloat162bfloat16_rz | 获取输入遵循CAST_TRUNC模式取整后的bfloat16_t类型数据。 |
| __bfloat162bfloat16_rd | 获取输入遵循CAST_FLOOR模式取整后的bfloat16_t类型数据。 |
| __bfloat162bfloat16_ru | 获取输入遵循CAST_CEIL模式取整后的bfloat16_t类型数据。 |
| __bfloat162bfloat16_rna | 获取输入遵循CAST_ROUND模式取整后的bfloat16_t类型数据。 |
| __bfloat162uint_rn | 获取输入遵循CAST_RINT模式转换成的无符号整数。 |
| __bfloat162uint_rz | 获取输入遵循CAST_TRUNC模式转换成的无符号整数。 |
| __bfloat162uint_rd | 获取输入遵循CAST_FLOOR模式转换成的无符号整数。 |
| __bfloat162uint_ru | 获取输入遵循CAST_CEIL模式转换成的无符号整数。 |
| __bfloat162uint_rna | 获取输入遵循CAST_ROUND模式转换成的无符号整数。 |
| __bfloat162int_rn | 获取输入遵循CAST_RINT模式转换成的有符号整数。 |
| __bfloat162int_rz | 获取输入遵循CAST_TRUNC模式转换成的有符号整数。 |
| __bfloat162int_rd | 获取输入遵循CAST_FLOOR模式转换成的有符号整数。 |
| __bfloat162int_ru | 获取输入遵循CAST_CEIL模式转换成的有符号整数。 |
| __bfloat162int_rna | 获取输入遵循CAST_ROUND模式转换成的有符号整数。 |
| __bfloat162ull_rn | 获取输入遵循CAST_RINT模式转换成的64位无符号整数。 |
| __bfloat162ull_rz | 获取输入遵循CAST_TRUNC模式转换成的64位无符号整数。 |
| __bfloat162ull_rd | 获取输入遵循CAST_FLOOR模式转换成的64位无符号整数。 |
| __bfloat162ull_ru | 获取输入遵循CAST_CEIL模式转换成的64位无符号整数。 |
| __bfloat162ull_rna | 获取输入遵循CAST_ROUND模式转换成的64位无符号整数。 |
| __bfloat162ll_rn | 获取输入遵循CAST_RINT模式转换成的64位有符号整数。 |
| __bfloat162ll_rz | 获取输入遵循CAST_TRUNC模式转换成的64位有符号整数。 |
| __bfloat162ll_rd | 获取输入遵循CAST_FLOOR模式转换成的64位有符号整数。 |
| __bfloat162ll_ru | 获取输入遵循CAST_CEIL模式转换成的64位有符号整数。 |
| __bfloat162ll_rna | 获取输入遵循CAST_ROUND模式转换成的64位有符号整数。 |
| __uint2bfloat16_rn | 获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __uint2bfloat16_rz | 获取输入遵循CAST_TRUNC模式转换成的bfloat16类型数据。 |
| __uint2bfloat16_rd | 获取输入遵循CAST_FLOOR模式转换成的bfloat16类型数据。 |
| __uint2bfloat16_ru | 获取输入遵循CAST_CEIL模式转换成的bfloat16类型数据。 |
| __uint2bfloat16_rna | 获取输入遵循CAST_ROUND模式转换成的bfloat16类型数据。 |
| __int2bfloat16_rn | 获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __int2bfloat16_rz | 获取输入遵循CAST_TRUNC模式转换成的bfloat16类型数据。 |
| __int2bfloat16_rd | 获取输入遵循CAST_FLOOR模式转换成的bfloat16类型数据。 |
| __int2bfloat16_ru | 获取输入遵循CAST_CEIL模式转换成的bfloat16类型数据。 |
| __int2bfloat16_rna | 获取输入遵循CAST_ROUND模式转换成的bfloat16类型数据。 |
| __ull2bfloat16_rn | 获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __ull2bfloat16_rz | 获取输入遵循CAST_TRUNC模式转换成的bfloat16类型数据。 |
| __ull2bfloat16_rd | 获取输入遵循CAST_FLOOR模式转换成的bfloat16类型数据。 |
| __ull2bfloat16_ru | 获取输入遵循CAST_CEIL模式转换成的bfloat16类型数据。 |
| __ull2bfloat16_rna | 获取输入遵循CAST_ROUND模式转换成的bfloat16类型数据。 |
| __ll2bfloat16_rn | 获取输入遵循CAST_RINT模式转换成的bfloat16类型数据。 |
| __ll2bfloat16_rz | 获取输入遵循CAST_TRUNC模式转换成的bfloat16类型数据。 |
| __ll2bfloat16_rd | 获取输入遵循CAST_FLOOR模式转换成的bfloat16类型数据。 |
| __ll2bfloat16_ru | 获取输入遵循CAST_CEIL模式转换成的bfloat16类型数据。 |
| __ll2bfloat16_rna | 获取输入遵循CAST_ROUND模式转换成的bfloat16类型数据。 |
| __float2bfloat162_rn | 将float类型数据遵循CAST_RINT模式转换为bfloat16类型并填充到bfloat16x2的前后两部分，返回填充后的bfloat16x2类型数据。 |
| __floats2bfloat162_rn | 将输入的数据x，y遵循CAST_RINT模式分别转换为bfloat16类型并填充到bfloat16x2的前后两部分，返回转换后的bfloat16x2类型数据。 |
| __float22bfloat162_rn | 将float2类型数据遵循CAST_RINT模式转换为bfloat16x2类型，返回转换后的bfloat16x2类型数据。 |
| __bfloat162bfloat162 | 将输入的数据的填充为bfloat16x2前后两个分量，返回转换后的bfloat16x2类型数据。 |
| __halves2bfloat162 | 将输入的数据分别填充为bfloat16x2前后两个分量，返回填充后数据。 |
| __high2bfloat16 | 提取输入bfloat16x2的高16位，并返回。 |
| __high2bfloat162 | 将输入数据的高16位填充到bfloat16x2并返回结果。 |
| __high2float | 将输入数据的高16位转换为float类型并返回结果。 |
| __highs2bfloat162 | 分别提取两个bfloat162输入的高16位，并填充到bfloat162中。返回填充后的数据。 |
| __low2bfloat16 | 返回输入数据的低16位。 |
| __low2bfloat162 | 将输入数据的低16位填充到bfloat16x2并返回。 |
| __low2float | 将输入数据的低16位转换为浮点数并返回结果。 |
| __lowhigh2highlow | 将输入数据的高低16位进行交换并返回。 |
| __lows2bfloat162 | 分别提取两个bfloat162输入的低16位，并填充到bfloat162中。返回填充后的数据。 |
| __bfloat1622float2 | 将bfloat16x2的两个分量分别转换为float，并填充到float2返回。 |
| __ushort_as_bfloat16 | 将unsigned short int的按位重新解释为bfloat16，即将unsigned short int的数据存储的位按照bfloat16的格式进行读取。 |

**表 18**  bfloat16x2类型算术函数

| 接口名 | 功能描述 |
| --- | --- |
| __haddx2 | 计算两个bfloat16x2_t类型数据各分量的相加结果，并遵循CAST_RINT模式舍入。 |
| __hsubx2 | 计算两个bfloat16x2_t类型数据各分量的相减结果，并遵循CAST_RINT模式舍入。 |
| __hmulx2 | 计算两个bfloat16x2_t类型数据各分量的相乘结果，并遵循CAST_RINT模式舍入。 |
| __hdivx2 | 计算两个bfloat16x2_t类型数据各分量的相除结果，并遵循CAST_RINT模式舍入。 |
| __habsx2 | 计算输入bfloat16x2_t类型数据各分量的绝对值。 |
| __hfmax2 | 计算两个bfloat16x2_t类型数据各分量的乘加的结果（前两个输入相乘后与第三个输入相加），并遵循CAST_RINT模式舍入。 |
| __hnegx2 | 获取输入bfloat16x2_t类型数据各分量的负值。 |
| __hfmax2_relu | 计算两个bfloat16x2_t类型数据各分量的乘加的结果（前两个输入相乘后与第三个输入相加），并遵循CAST_RINT模式舍入。负数结果置为0。 |
| __hcmadd | 将三个bfloat16x2_t输入视为复数（第一个分量为实部，第二个分量为虚部），执行复数乘加运算x*y+z。 |

**表 19**  bfloat16x2类型比较函数

| 接口名 | 功能描述 |
| --- | --- |
| __hbeqx2 | 比较两个bfloat16x2_t类型数据的两个分量是否相等，仅当两个分量均相等时返回true。 |
| __hbnex2 | 比较两个bfloat16x2_t类型数据的两个分量是否不相等，仅当两个分量均不相等时返回true。 |
| __hblex2 | 比较两个bfloat16x2_t类型数据的两个分量，仅当两个分量均满足第一个数小于或等于第二个数时返回true。 |
| __hbgex2 | 比较两个bfloat16x2_t类型数据的两个分量，仅当两个分量均满足第一个数大于或等于第二个数时返回true。 |
| __hbltx2 | 比较两个bfloat16x2_t类型数据的两个分量，仅当两个分量均满足第一个数小于第二个数时返回true。 |
| __hbgtx2 | 比较两个bfloat16x2_t类型数据的两个分量，仅当两个分量均满足第一个数大于第二个数时返回true。 |
| __hbequx2 | 比较两个bfloat16x2_t类型数据的两个分量是否相等，当两个分量均相等时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbneux2 | 比较两个bfloat16x2_t类型数据的两个分量是否不相等，当两个分量均不相等时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbleux2 | 比较两个bfloat16x2_t类型数据的两个分量，当两个分量均满足第一个数小于或等于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbgeux2 | 比较两个bfloat16x2_t类型数据的两个分量，当两个分量均满足第一个数大于或等于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbltux2 | 比较两个bfloat16x2_t类型数据的两个分量，当两个分量均满足第一个数小于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __hbgtux2 | 比较两个bfloat16x2_t类型数据的两个分量，当两个分量均满足第一个数大于第二个数时返回true。若任一输入的分量为nan，该分量的比较结果为true。 |
| __heqx2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量相等，则对应比较结果为1.0，否则为0.0。 |
| __hnex2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量不相等，则对应比较结果为1.0，否则为0.0。 |
| __hlex2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数小于或等于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hgex2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数大于或等于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hltx2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数小于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hgtx2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数大于第二个数，则对应比较结果为1.0，否则为0.0。 |
| __hequx2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量相等，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hneux2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量不相等，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hleux2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数小于或等于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hgeux2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数大于或等于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hltux2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数小于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __hgtux2 | 比较两个bfloat16x2_t类型数据的两个分量，如果分量满足第一个数大于第二个数，则对应比较结果为1.0，否则为0.0。若任一输入的分量为nan，该分量的比较结果为1.0。 |
| __heqx2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量相等，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hnex2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量不相等，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hlex2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hgex2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hltx2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hgtx2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于第二个数，则对应16位掩码为0xFFFF，否则为0x0。 |
| __hequx2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量相等，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hneux2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量不相等，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hleux2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hgeux2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于或等于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hltux2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数小于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __hgtux2_mask | 比较两个bfloat16x2_t类型数据的两个分量，结果以unsigned int形式返回，低16位为第一个分量的掩码结果，高16位为第二个分量的掩码结果。如果分量满足第一个数大于第二个数，则对应16位掩码为0xFFFF，否则为0x0。若任一输入的分量为nan，对应16位掩码为0xFFFF。 |
| __isnanx2 | 判断bfloat16x2_t类型数据的两个分量是否为nan。 |
| __hmaxx2 | 获取两个bfloat16x2_t类型数据各分量的最大值。 |
| __hmaxx2_nan | 获取两个bfloat16x2_t类型数据各分量的最大值。任一分量为nan时对应结果为nan。 |
| __hminx2 | 获取两个bfloat16x2_t类型数据各分量的最小值。 |
| __hminx2_nan | 获取两个bfloat16x2_t类型数据各分量的最小值。任一分量为nan时对应结果为nan。 |

**表 20**  bfloat16x2类型数学库函数

| 接口名 | 功能描述 |
| --- | --- |
| h2tanh | 获取输入数据各元素的三角函数双曲正切值。 |
| h2exp | 指定输入x，对x的各元素，获取e的该元素次方。 |
| h2exp2 | 指定输入x，对x的各元素，获取2的该元素次方。 |
| h2exp10 | 指定输入x，对x的各元素，获取10的该元素次方。 |
| h2log | 获取以e为底，输入数据各元素的对数。 |
| h2log2 | 获取以2为底，输入数据各元素的对数。 |
| h2log10 | 获取以10为底，输入数据各元素的对数。 |
| h2cos | 获取输入数据各元素的三角函数余弦值。 |
| h2sin | 获取输入数据各元素的三角函数正弦值。 |
| h2sqrt | 获取输入数据x各元素的平方根。 |
| h2rsqrt | 获取输入数据x各元素的平方根的倒数。 |
| h2rcp | 获取输入数据x各元素的倒数。 |
| h2rint | 获取与输入数据各元素最接近的整数，若存在两个同样接近的整数，则获取其中的偶数。 |
| h2floor | 获取小于或等于输入数据各元素的最大整数值。 |
| h2ceil | 获取大于或等于输入数据各元素的最小整数值。 |
| h2trunc | 获取对输入数据各元素的浮点数截断后的整数。 |

**表 21**  float类型数学库函数

| 接口名 | 功能描述 |
| --- | --- |
| tanf | 获取输入数据的三角函数正切值。 |
| tanhf | 获取输入数据的三角函数双曲正切值。 |
| tanpif | 获取输入数据与π相乘的正切值。 |
| atanf | 获取输入数据的反正切值。 |
| atan2f | 获取输入数据y/x的反正切值。 |
| atanhf | 获取输入数据的反双曲正切值。 |
| expf | 指定输入x，获取e的x次方。 |
| exp2f | 指定输入x，获取2的x次方。 |
| exp10f | 指定输入x，获取10的x次方。 |
| expm1f | 指定输入x，获取e的x次方减1。 |
| logf | 获取以e为底，输入数据的对数。 |
| log2f | 获取以2为底，输入数据的对数。 |
| log10f | 获取以10为底，输入数据的对数。 |
| log1pf | 获取以e为底，输入数据加1的对数。 |
| logbf | 计算以2为底，输入数据的对数，并对结果向下取整，返回浮点数。 |
| ilogbf | 计算以2为底，输入数据的对数，并对结果向下取整，返回整数。 |
| cosf | 获取输入数据的三角函数余弦值。 |
| coshf | 获取输入数据的双曲余弦值。 |
| cospif | 获取输入数据与π相乘的余弦值。 |
| acosf | 获取输入数据的反余弦值。 |
| acoshf | 获取输入数据的双曲反余弦值。 |
| sinf | 获取输入数据的三角函数正弦值。 |
| sinhf | 获取输入数据的双曲正弦值。 |
| sinpif | 获取输入数据与π相乘的正弦值。 |
| asinf | 获取输入数据的反正弦值。 |
| asinhf | 获取输入数据的双曲反正弦值。 |
| sincosf | 获取输入数据的三角函数正弦值和余弦值。 |
| sincospif | 获取输入数据与π相乘的三角函数正弦值和余弦值。 |
| frexpf | 将x转换为归一化[1/2, 1)的有符号数乘以2的积分幂。 |
| ldexpf | 获取输入x乘以2的exp次幂的结果。 |
| sqrtf | 获取输入数据x的平方根。 |
| rsqrtf | 获取输入数据x的平方根的倒数。 |
| hypotf | 获取输入数据x、y的平方和x^2 + y^2的平方根。 |
| rhypotf | 获取输入数据x、y的平方和x^2 + y^2的平方根的倒数。 |
| powf | 获取输入数据x的y次幂。 |
| norm3df | 获取输入数据a、b、c的平方和a^2 + b^2 + c^2的平方根。 |
| rnorm3df | 获取输入数据a、b、c的平方和a^2 + b^2 + c^2的平方根的倒数。 |
| norm4df | 获取输入数据a、b、c、d的平方和a^2 + b^2+ c^2+ d^2的平方根。 |
| rnorm4df | 获取输入数据a、b、c、d的平方和a^2 + b^2 + c^2 + d^2的平方根的倒数。 |
| normf | 获取输入数据a中前n个元素的平方和a[0]^2 + a[1]^2 +...+ a[n-1]^2的平方根。 |
| rnormf | 获取输入数据a中前n个元素的平方和a[0]^2 + a[1]^2 + ...+ a[n-1]^2的平方根的倒数。 |
| cbrtf | 获取输入数据x的立方根。 |
| rcbrtf | 获取输入数据x的立方根的倒数。 |
| erff | 获取输入数据的误差函数值。 |
| erfcf | 获取输入数据的互补误差函数值。 |
| erfinvf | 获取输入数据的逆误差函数值。 |
| erfcinvf | 获取输入数据的逆互补误差函数值。 |
| erfcxf | 获取输入数据的缩放互补误差函数值。 |
| tgammaf | 获取输入数据x的伽马函数值。 |
| lgammaf | 获取输入数据x伽马值的绝对值并求自然对数。 |
| cyl_bessel_i0f | 获取输入数据x的0阶常规修正圆柱贝塞尔函数的值。 |
| cyl_bessel_i1f | 获取输入数据x的1阶常规修正圆柱贝塞尔函数的值。 |
| normcdff | 获取输入数据x的标准正态分布的累积分布函数值。 |
| normcdfinvf | 获取输入数据x的标准正态累积分布的逆函数 |
| j0f | 获取输入数据x的0阶第一类贝塞尔函数j0的值。 |
| j1f | 获取输入数据x的1阶第一类贝塞尔函数j1的值。 |
| jnf | 获取输入数据x的n阶第一类贝塞尔函数jn的值。 |
| y0f | 获取输入数据x的0阶第二类贝塞尔函数y0的值。 |
| y1f | 获取输入数据x的1阶第二类贝塞尔函数y1的值。 |
| ynf | 获取输入数据x的n阶第二类贝塞尔函数yn的值。 |
| fabsf | 获取输入数据的绝对值。 |
| fmaf | 对输入数据x、y、z，计算x与y相乘加上z的结果。 |
| fmaxf | 获取两个输入数据中的最大值。 |
| fminf | 获取两个输入数据中的最小值。 |
| fdimf | 获取输入数据的差值，差值小于0时，返回0。 |
| remquof | 获取输入数据x除以y的余数。求余数时，商取最接近x除以y浮点数结果的整数，当x除以y的浮点数结果与左右最接近的整数距离相等时，商取偶数，同时将商赋值给指针变量quo。 |
| fmodf | 获取输入数据x除以y的余数。求余数时，商取x除以y浮点数结果的整数部分。 |
| remainderf | 获取输入数据x除以y的余数。求余数时，商取最接近x除以y浮点数结果的整数，当x除以y的浮点数结果与左右最接近的整数距离相等时，商取偶数。 |
| copysignf | 获取由第一个输入x的数值部分和第二个输入y的符号部分拼接得到的浮点数。 |
| nearbyIntf | 获取与输入浮点数最接近的整数，输入浮点数与左右整数的距离相等时，返回偶数。 |
| nextafterf | 如果y大于x，返回比x大的下一个可表示的浮点值，即浮点数二进制最低位加1。<br><br>如果y小于x，返回比x小的下一个可表示的浮点值，即浮点数二进制最低位减1。<br><br>如果y等于x，返回x。 |
| scalbnf | 获取输入数据x与2的n次方的乘积。 |
| scalblnf | 获取输入数据x与2的n次方的乘积。 |
| modff | 将输入数据分解为小数部分和整数部分。 |
| fdivdef | 获取两个输入数据相除的结果。 |
| signbit | 获取输入数据的符号位。 |
| __saturatef | 将输入数据钳位到[0.0, 1.0]区间。 |
| __fdividef | 获取两个输入数据相除的结果。 |
| rintf | 获取与输入数据最接近的整数，若存在两个同样接近的整数，则获取其中的偶数。 |
| lrintf | 获取与输入数据最接近的整数，若存在两个同样接近的整数，则获取其中的偶数。 |
| llrintf | 获取与输入数据最接近的整数，若存在两个同样接近的整数，则获取其中的偶数。 |
| roundf | 获取对输入数据四舍五入后的整数。 |
| lroundf | 获取对输入数据四舍五入后的整数。 |
| llroundf | 获取对输入数据四舍五入后的整数。 |
| floorf | 获取小于或等于输入数据的最大整数值。 |
| ceilf | 获取大于或等于输入数据的最小整数值。 |
| truncf | 获取对输入数据的浮点数截断后的整数。 |
| isfinite | 判断浮点数是否为有限数（非inf、非nan）。 |
| isnan | 判断浮点数是否为nan。 |
| isinf | 判断浮点数是否为无穷。 |

**表 22**  类型转换函数

| 接口名 | 功能描述 |
| --- | --- |
| __float2float_rn | 获取输入遵循CAST_RINT模式取整后的浮点数。 |
| __float2float_rz | 获取输入遵循CAST_TRUNC模式取整后的浮点数。 |
| __float2float_rd | 获取输入遵循CAST_FLOOR模式取整后的浮点数。 |
| __float2float_ru | 获取输入遵循CAST_CEIL模式取整后的浮点数。 |
| __float2float_rna | 获取输入遵循CAST_ROUND模式取整后的浮点数。 |
| __float2uint_rn | 获取输入遵循CAST_RINT模式转换成的无符号整数。 |
| __float2uint_rz | 获取输入遵循CAST_TRUNC模式转换成的无符号整数。 |
| __float2uint_rd | 获取输入遵循CAST_FLOOR模式转换成的无符号整数。 |
| __float2uint_ru | 获取输入遵循CAST_CEIL模式转换成的无符号整数。 |
| __float2uint_rna | 获取输入遵循CAST_ROUND模式转换成的无符号整数。 |
| __float2int_rn | 获取输入遵循CAST_RINT模式转换成的有符号整数。 |
| __float2int_rz | 获取输入遵循CAST_TRUNC模式转换成的有符号整数。 |
| __float2int_rd | 获取输入遵循CAST_FLOOR模式转换成的有符号整数。 |
| __float2int_ru | 获取输入遵循CAST_CEIL模式转换成的有符号整数。 |
| __float2int_rna | 获取输入遵循CAST_ROUND模式转换成的有符号整数。 |
| __float2ull_rn | 获取输入遵循CAST_RINT模式转换成的64位无符号整数。 |
| __float2ull_rz | 获取输入遵循CAST_TRUNC模式转换成的64位无符号整数。 |
| __float2ull_rd | 获取输入遵循CAST_FLOOR模式转换成的64位无符号整数。 |
| __float2ull_ru | 获取输入遵循CAST_CEIL模式转换成的64位无符号整数。 |
| __float2ull_rna | 获取输入遵循CAST_ROUND模式转换成的64位无符号整数。 |
| __float2ll_rn | 获取输入遵循CAST_RINT模式转换成的64位有符号整数。 |
| __float2ll_rz | 获取输入遵循CAST_TRUNC模式转换成的64位有符号整数。 |
| __float2ll_rd | 获取输入遵循CAST_FLOOR模式转换成的64位有符号整数。 |
| __float2ll_ru | 获取输入遵循CAST_CEIL模式转换成的64位有符号整数。 |
| __float2ll_rna | 获取输入遵循CAST_ROUND模式转换成的64位有符号整数。 |
| __uint2float_rn | 获取输入遵循CAST_RINT模式转换成的浮点数。 |
| __uint2float_rz | 获取输入遵循CAST_TRUNC模式转换成的浮点数。 |
| __uint2float_rd | 获取输入遵循CAST_FLOOR模式转换成的浮点数。 |
| __uint2float_ru | 获取输入遵循CAST_CEIL模式转换成的浮点数。 |
| __uint2float_rna | 获取输入遵循CAST_ROUND模式转换成的浮点数。 |
| __int2float_rn | 获取输入遵循CAST_RINT模式转换成的浮点数。 |
| __int2float_rz | 获取输入遵循CAST_TRUNC模式转换成的浮点数。 |
| __int2float_rd | 获取输入遵循CAST_FLOOR模式转换成的浮点数。 |
| __int2float_ru | 获取输入遵循CAST_CEIL模式转换成的浮点数。 |
| __int2float_rna | 获取输入遵循CAST_ROUND模式转换成的浮点数。 |
| __ull2float_rn | 获取输入遵循CAST_RINT模式转换成的浮点数。 |
| __ull2float_rz | 获取输入遵循CAST_TRUNC模式转换成的浮点数。 |
| __ull2float_rd | 获取输入遵循CAST_FLOOR模式转换成的浮点数。 |
| __ull2float_ru | 获取输入遵循CAST_CEIL模式转换成的浮点数。 |
| __ull2float_rna | 获取输入遵循CAST_ROUND模式转换成的浮点数。 |
| __ll2float_rn | 获取输入遵循CAST_RINT模式转换成的浮点数。 |
| __ll2float_rz | 获取输入遵循CAST_TRUNC模式转换成的浮点数。 |
| __ll2float_rd | 获取输入遵循CAST_FLOOR模式转换成的浮点数。 |
| __ll2float_ru | 获取输入遵循CAST_CEIL模式转换成的浮点数。 |
| __ll2float_rna | 获取输入遵循CAST_ROUND模式转换成的浮点数。 |
| __int_as_float | 将整数中的位重新解释为浮点数。 |
| __uint_as_float | 将无符号整数中的位重新解释为浮点数。 |
| __float_as_int | 将浮点数中的位重新解释为有符号整数。 |
| __float_as_uint | 将浮点数中的位重新解释为无符号整数。 |

**表 23**  整型数学库函数

| 接口名 | 功能描述 |
| --- | --- |
| labs | 获取输入数据的绝对值。 |
| llabs | 获取输入数据的绝对值。 |
| llmax | 获取两个输入数据中的最大值。 |
| ullmax | 获取两个输入数据中的最大值。 |
| umax | 获取两个输入数据中的最大值。 |
| llmin | 获取两个输入数据中的最小值。 |
| ullmin | 获取两个输入数据中的最小值。 |
| umin | 获取两个输入数据中的最小值。 |
| __mulhi | 获取输入int32类型数据x和y乘积的高32位。 |
| __umulhi | 获取输入uint32类型数据x和y乘积的高32位。 |
| __mul64hi | 获取输入int64类型数据x和y乘积的高64位。 |
| __umul64hi | 获取输入uint64类型数据x和y乘积的高64位。 |
| __mul_i32toi64 | 计算输入32位整数x和y的乘积，返回64位结果。 |
| __brev | 将输入数据的位序反转，返回反转后的值。 |
| __clz | 从输入数据的二进制最高有效位开始，返回连续的前导零的位数。 |
| __ffs | 从二进制输入数据的最低位开始，查找第一个值为1的比特位的位置，并返回该位置的索引，索引从1开始计数；如果二进制数据中没有1，则返回0。 |
| __popc | 统计输入数据从二进制的高位到低位比特位为1的数量。 |
| __byte_perm | 由输入的两个4字节的uint32_t类型数据组成一个8个字节的64比特位的整数，通过选择器s指定选取其中的4个字节，将这4个字节从低位到高位拼成一个uint32_t类型的整数。 |
| __sad | 对输入数据x、y、z，计算|x - y|+z的结果，即第一个入参和第二个入参之差的绝对值与第三个入参的和。 |
| __usad | 对输入数据x、y、z，计算|x - y|+z的结果，即第一个入参和第二个入参之差的绝对值与第三个入参的和。 |
| __mul24 | 获取输入int32类型数据x和y低24位乘积的低32位结果。x和y的高8位被忽略。 |
| __umul24 | 获取输入uint32类型数据x和y低24位乘积的低32位结果。x和y的高8位被忽略。 |
|  |  |
| __hadd | 获取输入int32类型数据x和y的平均值，避免中间求和溢出。 |
| __rhadd | 获取输入int32类型数据x和y的向上取整平均值，避免中间求和溢出。 |
| __uhadd | 获取输入uint32类型数据x和y的平均值，避免中间求和溢出。 |
| __urhadd | 获取输入uint32类型数据x和y的向上取整平均值，避免中间求和溢出。 |
| max | 获取两个输入数据中的最大值。 |
| min | 获取两个输入数据中的最小值。 |

## 地址空间谓词函数

**表 24**  地址空间谓词函数

| 接口名 | 功能描述 |
| --- | --- |
| __isGlobal | 判断输入的指针是否指向Global Memory内存空间的地址。 |
| __isUbuf | 判断输入的指针是否指向Unified Buffer内存空间的地址。 |
| __isLocal | 判断输入的指针是否指向栈空间的地址。 |

## 地址空间转换函数

**表 25**  地址空间转换函数

| 接口名 | 功能描述 |
| --- | --- |
| __cvta_generic_to_global | 将输入的指针转换为其指向的Global Memory内存空间的地址值并返回。 |
| __cvta_generic_to_ubuf | 将输入的指针转换为其指向的Unified Buffer内存空间的地址值并返回。 |
| __cvta_generic_to_local | 将输入的指针转换为其指向的栈空间地址的值并返回。 |
| __cvta_global_to_generic | 将Global Memory内存空间的地址值转换为对应的指针。 |
| __cvta_ubuf_to_generic | 将Unified Buffer内存空间的地址值转换为对应的指针。 |
| __cvta_local_to_generic | 将栈空间的地址值转换为对应的指针。 |

