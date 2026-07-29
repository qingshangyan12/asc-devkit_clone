# C API

CANN 9.0.0新增实验性C API系列，开放底层完备能力，提供直接操作NPU硬件资源的接口，覆盖数据搬运、矢量计算、矩阵计算、同步控制、标量计算、缓存控制等全场景。支持以数组形式分配内存，一般基于指针编程。提供与业界一致的C语言编程体验。

## 特性

### 数据结构

提供配置类联合体定义，用于矩阵填充、原子操作、L0C Buffer搬运、3D加载、N维填充等场景的参数封装。

- `asc_fill_value_config`：fill_value的初始化参数结构体，包含 `asc_fill_l0a` / `asc_fill_l0b` / `asc_fill_l1` 接口需要配置的各种初始化参数。
- `asc_store_atomic_config`：原子操作使能位与原子操作类型的值。
- `asc_set_l0c_copy_params_config`：DataCopy（CO1->GM、CO1->A1）过程中进行随路格式转换（NZ格式转换为ND格式）时，通过调用该接口设置格式转换的相关配置。
- `asc_load3d_v2_config`：Load3Dv2接口的repeat参数。
- `asc_ndim_pad_count_config`：N维搬运时各层循环的左右填充计数配置。
- `asc_l13d_fmatrix_config`：L1 3D fmatrix配置，用于设置 `asc_copy_l12l0a` / `asc_copy_l12l0b` 的3D格式搬运的fmatrix参数。

### 矢量数据搬运

矢量数据搬运类API，单独使用时可以引入vector_datamove.h。

**ARCH 2201：**

- `asc_copy_gm2ub` / `asc_copy_gm2ub_sync`：将数据从Global Memory搬运到Unified Buffer。
- `asc_copy_ub2gm` / `asc_copy_ub2gm_sync`：将数据从Unified Buffer搬运到Global Memory。
- `asc_copy_ub2ub` / `asc_copy_ub2ub_sync`：将数据从Unified Buffer搬运到Unified Buffer。
- `asc_copy_gm2ub_align` / `asc_copy_gm2ub_align_sync`：提供数据非对齐搬运的功能，将数据从Global Memory搬运到Unified Buffer，并支持8位/16位/32位数据类型搬运。
- `asc_copy_ub2gm_align` / `asc_copy_ub2gm_align_sync`：将数据从Unified Buffer搬运到Global Memory，支持8位/16位/32位分块拷贝操作。

**ARCH 3510：**

- `asc_copy_gm2ub_align` / `asc_copy_gm2ub_align_sync`：提供数据非对齐搬运的功能，将数据从Global Memory搬运到Unified Buffer，扩展支持 `fp8_e5m2_t` / `fp8_e4m3fn_t` / `hifloat8_t`，增加 `enable_constant_pad` / `l2_cache_mode` 参数。
- `asc_copy_ub2gm_align` / `asc_copy_ub2gm_align_sync`：将数据从Unified Buffer搬运到Global Memory，增加 `l2_cache_mode` 参数。
- `asc_copy_ub2l1` / `asc_copy_ub2l1_sync`：将数据从Unified Buffer搬运到L1 Buffer。
- `asc_ndim_copy_gm2ub`：N维GM到UB搬运，支持 `int8_t` / `uint8_t` / `fp4x2_e2m1_t` / `fp4x2_e1m2_t` / `fp8_e8m0_t` / `fp8_e5m2_t` / `fp8_e4m3fn_t` / `int16_t` / `uint16_t` / `half` / `bfloat16_t` / `int32_t` / `uint32_t` / `float`。
- `asc_set_gm2ub_loop_size` / `asc_set_gm2ub_loop1_stride` / `asc_set_gm2ub_loop2_stride`：GM到UB循环搬运参数配置。
- `asc_set_ub2gm_loop_size` / `asc_set_ub2gm_loop1_stride` / `asc_set_ub2gm_loop2_stride`：UB到GM循环搬运参数配置。
- `asc_set_gm2ub_pad`：GM到UB填充值配置。
- `asc_set_ndim_loop0_stride` ~ `asc_set_ndim_loop4_stride`：N维循环步长配置。
- `asc_set_ndim_pad_count` / `asc_set_ndim_pad_value`：N维填充配置。

### Memory矢量计算

矢量计算类API，单独使用时可以引入vector_compute.h，支持 `half` / `float` / `int16_t` / `int32_t` / `uint16_t` / `uint32_t` 等数据类型，每种接口提供前n个数据计算、高维切分计算、同步计算三种重载（ARCH 2201）。

- `asc_add`：按元素求和。
- `asc_add_scalar`：矢量内每个元素与标量求和。
- `asc_add_relu`：按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能。
- `asc_sub` / `asc_sub_scalar`：按元素求差 / 矢量内每个元素与标量求差。
- `asc_sub_relu`：按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能。
- `asc_mul`：按元素求积。
- `asc_mul_scalar`：矢量内每个元素与标量求积。
- `asc_mul_add`：执行矢量的乘加运算。
- `asc_mul_add_relu`：按元素将src0和dst相乘并加上src1，再进行Relu计算（结果和0对比取较大值），最终结果存放进dst中。
- `asc_mul_cast_half2int8`：按元素求积，并将结果转换为int8_t类型。
- `asc_mul_cast_half2uint8`：按元素求积，并将结果转换为uint8_t类型。
- `asc_div`：按元素求商。
- `asc_fma`：按元素将src0和src1相乘并和dst相加，将最终结果存放进dst中。
- `asc_max`：按元素求最大值。
- `asc_max_scalar`：矢量内每个元素与标量求最大值。
- `asc_min`：按元素求最小值。
- `asc_min_scalar`：源操作数矢量逐元素与标量相比，取较小值。
- `asc_abs`：按元素取绝对值。
- `asc_axpy`：源操作数src中每个元素与标量value求积后和目的操作数dst中的对应元素相加。
- `asc_leakyrelu`：执行矢量Leaky Relu运算。
- `asc_relu`：按元素做线性整流Relu。
- `asc_eq`：比较src0与src1在对应索引位置的元素大小。若比较结果为真，则输出结果的对应比特位设为1，否则设为0。
- `asc_eq_scalar`：执行矢量与标量的比较运算，如果值相等则输出1，否则输出0。
- `asc_ne`：按元素判断src0 != src1是否成立，若成立则输出结果为1，否则为0。
- `asc_ne_scalar`：按元素判断是否不等于输入标量，若成立则输出结果上的对应比特位为1，否则为0。
- `asc_lt`：按元素判断src0 < src1是否成立，若成立则输出结果上的对应比特位为1，否则为0。
- `asc_lt_scalar`：执行矢量中每个位置和标量比较，如果值小于标量值则为1，否则为0，结果为每个bit位按小端序排布。
- `asc_le`：按元素判断src0 <= src1是否成立，若成立则输出结果为1，否则为0。
- `asc_le_scalar`：按元素判断src <= value是否成立，若成立则输出结果为1，否则为0。
- `asc_gt`：按元素比较两个矢量的大小关系，若比较后的结果为真，则输出结果的对应比特位为1，否则为0。
- `asc_gt_scalar`：src中的每个元素逐个与标量value比较大小，如果某个位置上的元素大于value，则输出结果dst上的对应比特位为1，否则为0。
- `asc_ge`：ge（greater than or equal to），逐元素比较src0 >= src1是否成立，成立则输出结果为1，否则输出结果为0，每个元素的比较结果占一个bit。
- `asc_ge_scalar`：按元素判断src >= value是否成立，若成立则输出结果为1，否则为0。
- `asc_select`：根据掩码，从两个源操作数中选取元素，输出到目的操作数。
- `asc_and`：执行矢量与运算。
- `asc_or`：每对元素按位或运算。
- `asc_not`：按元素做按位取反。
- `asc_shiftleft`：将所有元素左移distance位。
- `asc_shiftright`：对源操作数中的每个元素执行右移。
- `asc_shiftright_round`：对源操作数中的每个元素执行舍入右移。
- `asc_exp`：按元素取自然指数。
- `asc_log`：按元素取自然对数。
- `asc_sqrt`：对元素进行开方。
- `asc_rsqrt`：按元素进行开方后取倒数的计算。
- `asc_rcp`：执行矢量的取倒数运算。
- `asc_float2half`：将float类型数据转换为half类型。
- `asc_half2float`：将half类型数据转换为float类型。
- `asc_float2bfloat16`：将float类型数据转换为bfloat16_t类型。
- `asc_bfloat162float`：将bfloat16_t类型数据转换为float类型。
- `asc_float2float`：将float类型数据转换为float类型。
- `asc_float2int16`：将float类型数据转换为int16_t类型。
- `asc_float2int32`：将float类型数据转换为int32_t类型。
- `asc_float2int64`：将float类型数据转换为int64_t类型。
- `asc_half2int8`：将half类型数据转换为int8_t类型。
- `asc_half2int16`：将half类型数据转换为int16_t类型。
- `asc_half2int32`：将half类型数据转换为int32_t类型。
- `asc_half2uint8`：将half类型数据转换为uint8_t类型，支持多种舍入模式。
- `asc_half2int4`：将half类型数据转换为int4b_t类型。
- `asc_int162float`：将int16_t类型数据转换为float类型。
- `asc_int322float`：将int32_t类型数据转换为float类型。
- `asc_int642float`：将int64_t类型数据转换为float类型。
- `asc_int322int16`：将int32_t类型数据转换为int16_t类型。
- `asc_int322int64`：将int32_t类型数据转换为int64_t类型。
- `asc_int642int32`：将int64_t类型数据转换为int32_t类型。
- `asc_int82half`：将int8_t类型数据转换为half类型。
- `asc_int42half`：将int4b_t类型数据转换为half类型。
- `asc_uint82half`：将uint8_t类型数据转换为half类型。
- `asc_bfloat162int32`：将bfloat16_t类型数据转换为int32_t类型。
- `asc_deq_int322half`：对输入的int32_t类型的数据按元素做量化并转换为half类型。
- `asc_deq_int162b8`：将int16_t类型转换为int8_t或uint8_t类型，并将数据存放在每个DataBlock的上半块或下半块。
- `asc_vdeq_int162b8`：将int16_t类型转换为int8_t或uint8_t类型，并将数据存放在每个DataBlock的上半块或下半块。
- `asc_copy`：将数据从Unified Buffer搬运到Unified Buffer。支持高维切分。
- `asc_duplicate`：将一个变量或立即数填充到一个矢量中。
- `asc_brcb`：将源操作数中的每一个数填充到目的操作数的一个DataBlock中。
- `asc_bitsort`：Score和Index分别存储在src0和src1中，按Score进行排序（Score大的元素排前面），排序后的Score与其对应的Index一起以（Score，Index）的结构存储在dst中。
- `asc_mrgsort4`：将已经排好序的最多4条队列，合并排列成1条队列，结果按照score域由大到小排序。
- `asc_transpose`：用于实现16*16的二维矩阵数据块转置。
- `asc_datablock_reduce_sum`：对每个DataBlock内所有元素求和。
- `asc_datablock_reduce_max`：对每个DataBlock内所有元素求最大值。
- `asc_datablock_reduce_min`：对每个DataBlock内所有元素求最小值。
- `asc_repeat_reduce_sum`：对每个Repeat内所有元素求和。
- `asc_repeat_reduce_max`：对每个Repeat内所有元素求最大值。
- `asc_repeat_reduce_min`：对每个Repeat内所有元素求最小值。
- `asc_pair_reduce_sum`：对输入数据做归约操作，得到数据总和。
- `asc_gather`：将源操作数按照给定的偏移按元素收集到目的操作数中。
- `asc_gather_datablock`：根据偏移地址按照DataBlock的粒度将源操作数收集到目的操作数中。
- `asc_reduce`：以内置固定模式对应的二进制或者用户自定义输入的数值对应的gather mask（数据收集的掩码），从源操作数中选取元素写入目的操作数中。
- `asc_set_va_reg`：用于设置transpose的地址，将操作数地址序列与地址寄存器关联。
- `asc_set_vector_mask`：设置Mask值。
- `asc_set_mask_count`：设置Mask模式为Counter模式。
- `asc_set_mask_norm`：设置Mask模式为Normal模式。
- `asc_set_cmp_mask`：为Select操作设置用于选择的掩码。
- `asc_get_cmp_mask`：获取Compare操作的比较结果。
- `asc_set_deq_scale`：设置DEQSCALAR寄存器的值。
- `asc_get_reduce_max_cnt`：获取执行asc_repeat_reduce_max操作后的最大值，以及第一个最大值时的索引。
- `asc_get_reduce_min_cnt`：获取执行asc_repeat_reduce_min操作后的最小值，以及第一个最小值时的索引。
- `asc_get_rsvd_count`：获取Gather Mask操作后剩余的元素数量。
- `asc_get_vms4_sr`：获取执行asc_mrgsort4操作后的队列中，每个队列已经理过的Region Proposal个数。

### Reg矢量计算

提供基于矢量寄存器的计算接口，使用 `__simd_callee__` 限定符，支持 `int8_t` / `uint8_t` / `int16_t` / `uint16_t` / `int32_t` / `uint32_t` / `half` / `float` / `bfloat16_t` / `fp8_e4m3fn_t` / `fp8_e5m2_t` / `hifloat8_t` / `fp8_e8m0_t` / `fp4x2_e2m1_t` / `fp4x2_e1m2_t` 等数据类型（ARCH 3510）。

**寄存器数据类型：**

- `asc_create_iter_reg_b8` / `asc_create_iter_reg_b16` / `asc_create_iter_reg_b32`：地址寄存器通过该接口初始化，然后在循环之中使用地址寄存器存储地址偏移量。
- `asc_create_mask_b8` / `asc_create_mask_b16` / `asc_create_mask_b32`（宏）：根据入参生成相应的掩码寄存器。
- `asc_get_mask_spr_b16` / `asc_get_mask_spr_b32`：从特殊寄存器SPR{MASK1, MASK0} 读取mask值并根据数据类型格式返回对应的mask数据，MASK0、MASK1均为64bit的寄存器。
- `asc_update_mask_b8` / `asc_update_mask_b16` / `asc_update_mask_b32`：根据value大小生成对应的掩码寄存器中的值。

**reg数据搬入：**

- `asc_loadalign`：对齐数据搬运接口，从UB连续对齐搬入目的操作数，支持多种搬入模式。
- `asc_loadalign_brc` / `asc_loadalign_brc_v2` / `asc_loadalign_brc_v3`：广播加载对齐数据。
- `asc_loadalign_unpack` / `asc_loadalign_unpack_v2`：解包加载对齐数据。
- `asc_loadalign_upsample` / `asc_loadalign_downsample`：上采样/下采样加载对齐数据。
- `asc_loadalign_deintlv`：解交织加载对齐数据。
- `asc_loadalign_postupdate` 及各变体的 `_postupdate` 版本：地址自更新加载。
- `asc_loadunalign`：reg计算数据搬运接口，适用于从UB非32B对齐的起始地址连续搬入矢量数据寄存器的场景。
- `asc_loadunalign_pre`：用于在进行非对齐数据搬入前的初始化，需配合 `asc_loadunalign` 接口使用。
- `asc_loadunalign_postupdate`：非对齐搬入的地址自更新版本。
- `asc_load`：reg计算数据搬运接口，支持从UB非32字节对齐的源地址src搬运至矢量数据寄存器，搬运量为VL。
- `asc_gather`：根据索引位置index将源操作数src按元素收集到目的操作数dst中。
- `asc_gather_datablock`：给定源操作数在UB中的基地址和索引，根据索引位置将源操作数按DataBlock收集到目的操作数中。
- `asc_copy`：完成数据在寄存器内的搬运。

**reg数据搬出：**

- `asc_storealign`：reg计算数据搬运接口，适用于从矢量数据寄存器连续对齐搬出到UB的场景，并支持多种搬出模式。
- `asc_storealign_1st`：首次对齐搬出。
- `asc_storealign_pack` / `asc_storealign_pack_v2`：打包搬出。
- `asc_storealign_intlv`：交织搬出。
- `asc_storeunalign`：reg计算数据搬运接口，适用于从矢量数据寄存器连续非32B对齐的起始地址连续搬出到UB的场景。
- `asc_storeunalign_post` / `asc_storeunalign_postupdate` / `asc_storeunalign_post_postupdate`：非对齐搬出的地址自更新变体。
- `asc_storealign_postupdate` 及各变体的 `_postupdate` 版本：地址自更新搬出。
- `asc_scatter`：数据分散存储。
- `asc_store`：reg计算数据搬运接口，适用于从矢量数据寄存器搬出到UB的场景，不区分是否对齐，在追求极致性能时，应尽量避免使用该接口。

**基础算术：**

- `asc_add`：按元素执行加法运算。
- `asc_addc`：按元素执行带进位的加法运算。
- `asc_add_scalar`：按元素执行矢量和标量的加法运算。
- `asc_sub`：按元素执行减法运算。
- `asc_subc`：按元素执行带借位的减法运算。
- `asc_mul`：对源操作数src0和src1进行乘法运算，将结果写入目的操作数dst。
- `asc_mul_scalar`：对源操作数src、value进行按元素乘法操作，将结果写入目的操作数dst。
- `asc_madd`：madd（multiply-add），对源操作数执行逐元素乘法和加法。
- `asc_mull`：根据mask对输入数据src0、src1按元素相乘，将结果写入dst0，溢出部分写入dst1。
- `asc_max`：按元素求最大值。
- `asc_max_scalar`：按元素求矢量和标量的最大值。
- `asc_min`：根据mask对源操作数src0、src1进行按元素求最小值操作，将结果写入目的操作数dst。
- `asc_min_scalar`：源操作数矢量内每个元素与标量比较，取较小值。
- `asc_abs`：逐元素计算绝对值。
- `asc_neg`：根据mask对源操作数src进行取相反数操作，将结果写入目的操作数dst。
- `asc_abs_sub`：逐元素计算差的绝对值。
- `asc_axpy`：根据mask对源操作数src、value进行按元素做乘加操作，将结果写入目的操作数dst。
- `asc_relu`：逐元素执行ReLU运算。
- `asc_leakyrelu`：按元素执行Leaky ReLU（Leaky Rectified Linear Unit）操作。
- `asc_prelu`：源操作数src0大于0的情况下直接将src0写入目的操作数dst，否则将src0 * src1的结果写入dst。
- `asc_exp`：对源操作数逐元素计算指数。
- `asc_ln`：对源操作数逐元素计算自然对数。
- `asc_sqrt`：逐元素计算平方根。
- `asc_div`：按元素执行除法运算。

**逻辑计算：**

- `asc_and`：对掩码寄存器操作：根据mask对源操作数src0、src1的有效bit进行逻辑与运算，得到新的掩码寄存器。
- `asc_or`：对源操作数src0和src1进行或（|）运算，将结果写入目的操作数dst。
- `asc_xor`：根据mask对输入的src0、src1按元素异或（^）进行操作，将结果写入dst。
- `asc_not`：执行矢量非运算。
- `asc_shiftleft`：根据掩码mask对输入数据src0，按照src1对应元素进行左移操作，完成后将结果写入dst中。
- `asc_shiftleft_scalar`：根据mask对源操作数src执行左移，左移的位数由输入参数value决定，将结果写入目的操作数dst。
- `asc_shiftright`：根据掩码mask对输入数据src0，按照src1对应元素进行右移操作，完成后将结果写入dst中。
- `asc_shiftright_scalar`：根据mask对源操作数src执行右移，右移的位数由输入参数value决定，将结果写入目的操作数dst。

**比较与选择：**

- `asc_eq`：对源操作数逐元素比较是否相等。
- `asc_eq_scalar`：对源操作数逐元素比较是否与标量相等。
- `asc_ne`：ne（not equal），对源操作数执行逐元素比较。
- `asc_ne_scalar`：src中的每个元素逐个与标量value比较大小，如果src_i != value，则输出结果dst对应比特位为1，否则为0。
- `asc_lt`：lt（less than），对源操作数执行逐元素比较。
- `asc_lt_scalar`：lt（less than），对源操作数与标量执行逐元素比较。
- `asc_le`：le（less than or equal to），对源操作数执行逐元素比较。
- `asc_le_scalar`：le（less than or equal to），对源操作数与标量执行逐元素比较。
- `asc_gt`：gt（greater than），对源操作数执行逐元素比较。
- `asc_gt_scalar`：gt（greater than），对源操作数与标量执行逐元素比较。
- `asc_ge`：ge（greater than or equal to），对源操作数执行逐元素比较。
- `asc_ge_scalar`：ge（greater than or equal to），对源操作数与标量执行逐元素比较。
- `asc_select`：根据mask的比特位值，从源操作数src0、src1中选择元素，得到目的操作数dst。
- `asc_squeeze` / `asc_squeeze_v2`：将src中被mask选择的有效元素依次复制到dst，有效元素在dst上连续排列。

**类型转换：**

- `asc_float2half`：将float类型转换为half类型，并支持多种舍入模式。
- `asc_half2float`：将half类型数据转换为float类型。
- `asc_float2bfloat16`：将float类型转换为bfloat16_t类型，并支持多种舍入模式。
- `asc_bfloat162float`：将bfloat16_t类型转换为float类型，无舍入模式。
- `asc_float2e4m3`：将float类型转换为fp8_e4m3fn_t类型，支持RINT舍入模式。
- `asc_float2e5m2`：将float类型数据转换为fp8_e5m2类型。
- `asc_float2hif8`：将float类型转换为hifloat8_t类型，并支持多种舍入模式。
- `asc_e4m32float`：将fp8_e4m3fn_t类型转换为float类型。
- `asc_e5m22float`：将vector_fp8_e5m2_t类型的源操作数以256B为单位分为四部分，读取其中一部分元素，将其转换成vector_float类型并写入目的操作数。
- `asc_hif82half`：将hifloat8_t类型数据转换为half类型。
- `asc_half2hif8`：将half类型转换为hifloat8_t类型，并支持多种舍入模式。
- `asc_bfloat162e1m2x2`：将bfloat16_t类型转换为fp4x2_e1m2_t类型，并支持多种舍入模式。
- `asc_bfloat162e2m1x2`：将bfloat16_t类型转换为fp4x2_e2m1_t类型，并支持多种舍入模式。
- `asc_e1m2x22bfloat16`：将fp4x2_e1m2_t类型转换为bfloat16_t类型。
- `asc_e2m1x22bfloat16`：将fp4x2_e2m1_t类型转换为bfloat16_t类型。
- `asc_bfloat162half`：将bfloat16_t类型转换为half类型，并支持多种舍入模式和饱和/非饱和模式。
- `asc_half2bf16`：将half类型数据转换为bfloat16_t类型，并支持多种舍入模式。
- `asc_int4x22half`：将int4x2_t类型数据转换为half类型。
- `asc_int4x22int16`：将int4x2_t类型转换为int16_t类型。
- `asc_int4x22bfloat16`：将int4x2_t类型转换为bfloat16_t类型，无舍入模式。
- `asc_half2int4x2`：将half类型数据转换为int4x2_t类型，并支持多种舍入模式。
- `asc_half2int8`：将half类型转换为int8_t类型，并支持多种舍入模式。
- `asc_half2int16`：将half类型数据转换为int16_t类型，并支持多种舍入模式。
- `asc_half2int32`：将half类型数据转换为int32_t类型，并支持多种舍入模式。
- `asc_half2uint8`：将half类型转换为uint8_t类型，并支持多种舍入模式。
- `asc_float2int16`：将float类型转换为int16_t类型，并支持多种舍入模式。
- `asc_float2int32`：将float类型转换为int32_t类型，并支持多种舍入模式。
- `asc_float2int64`：将float类型转换为int64_t类型，并支持多种舍入模式。
- `asc_int162float`：将int16_t类型转换为float类型。
- `asc_int322float`：将int32_t类型转换为float类型，并支持多种舍入模式。
- `asc_int642float`：将int64_t类型数据转换为float类型，并支持多种舍入模式。
- `asc_int162half`：将int16_t类型转换为half类型。
- `asc_int162int32`：将int16_t类型转换为int32_t类型。
- `asc_int162uint32`：将int16_t类型转换为uint32_t类型。
- `asc_int162uint8`：将int16_t类型转换为uint8_t类型。
- `asc_int322int16`：将int32_t类型转换为int16_t类型。
- `asc_int322int64`：将int32_t类型转换为int64_t类型。
- `asc_int322uint16`：将int32_t类型转换为uint16_t类型。
- `asc_int322uint8`：将int32_t类型转换为uint8_t类型。
- `asc_int642int32`：将int64_t类型转换为int32_t类型。
- `asc_int82half`：将int8_t类型转换为half类型。
- `asc_int82int16`：将int8_t类型转换为int16_t类型。
- `asc_int82int32`：将int8_t类型转换为int32_t类型。
- `asc_uint82half`：将uint8_t类型转换为half类型。
- `asc_uint82uint16`：将uint8_t类型转换为uint16_t类型。
- `asc_uint82uint32`：将uint8_t类型转换为uint32_t类型。
- `asc_uint162uint32`：将uint16_t类型转换为uint32_t类型。
- `asc_uint162uint8`：将uint16_t类型转换为uint8_t类型。
- `asc_uint322int16`：将uint32_t类型转换为int16_t类型。
- `asc_uint322uint16`：将uint32_t类型转换为uint16_t类型。
- `asc_uint322uint8`：将uint32_t类型转换为uint8_t类型。
- `asc_ceil` / `asc_floor` / `asc_rint` / `asc_round` / `asc_trunc`：将源操作数的浮点数元素截断到整数位，同时源操作数的数据类型保持不变，并支持多种舍入模式。

**归约计算：**

- `asc_reduce_max`：根据mask对源操作数src进行归约最大值操作，将结果写入目的操作数dst。
- `asc_reduce_min`：根据mask对源操作数src进行归约最小值操作，将结果写入目的操作数dst。
- `asc_reduce_sum`：归约求和功能，用于将src中的所有参与计算的元素求和，得到的结果保存在dst中。
- `asc_reduce_max_datablock`：找出DataBlock中的最大值，并将最终的计算结果依次保存在dst中的最低位。
- `asc_reduce_min_datablock`：根据mask将每个DataBlock(32B) 中的最小值，依次保存在dst中的最低位。
- `asc_reduce_sum_datablock`：归约求和功能，用于将src每个DataBlock(32B) 中参与计算的元素求和，得到的结果依次保存在dst中。
- `asc_pair_reduce_sum`：PairReduceSum，相邻两个（奇偶）元素求和，结果写入dst。

**数据压缩：**

- `asc_pack` / `asc_pack_v2`：将源操作数中的元素选取低8位（b16）、低16位（b32）、低32位（b64）写入目的操作数的低半部分或高半部分。
- `asc_unpack`：矢量解包操作。
- `asc_unsqueeze`：根据mask进行解压缩，将生成的数据输出到dst。

**数据填充：**

- `asc_duplicate`：根据mask将源操作数src的最低位元素填充到目的操作数dst。
- `asc_duplicate_scalar`：根据mask将value填充到目的操作数dst。

**数据重排：**

- `asc_intlv`：将源操作数src0和src1中的元素交织存入目的操作数dst0和dst1中。
- `asc_deintlv`：给定源操作数src0和src1，将src0和src1中的元素解交织存入结果操作数dst0和dst1中。

**直方图计算：**

- `asc_cumulative_histogram`：对直方图数据进行累计统计。
- `asc_frequency_histogram`：对直方图数据进行频率统计。

**索引操作：**

- `asc_arange` / `asc_arange_descend`：以传入的value为起始值，生成递增/递减的索引，并将生成的索引保存在dst中。

**同步控制：**

- `asc_mem_bar`（宏）：Reg计算宏函数内不同流水线之间的同步指令。

**系统变量访问：**

- `asc_clear_ar_spr`：对AR寄存器进行清理，AR寄存器通常由 `asc_squeeze` 接口使用。

### 矩阵数据搬运

数据搬运类API，单独使用时可以引入cube_datamove.h。

**GM -> L1 Buffer搬运（ARCH 2201）：**

- `asc_copy_gm2l1` / `asc_copy_gm2l1_sync`：将数据从Global Memory搬运到Level 1 cache。
- `asc_copy_gm2l1_pad1` ~ `asc_copy_gm2l1_pad8` / `_sync`：带填充模式的GM到L1 Buffer搬运。
- `asc_copy_gm2l1_nd2nz` / `_sync`：将数据从Global Memory搬运到Level 1 cache，支持在数据搬运时进行ND格式到NZ格式的转换。

**L1 Buffer -> L0A Buffer / L0B Buffer搬运（ARCH 2201）：**

- `asc_copy_l12l0a` / `asc_copy_l12l0a_sync`：用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里。
- `asc_copy_l12l0b` / `asc_copy_l12l0b_sync`：用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里。
- `asc_copy_l12l0a_trans` / `asc_copy_l12l0b_trans` / `_sync`：带转置的2D格式数据从L1 Buffer到L0A Buffer/L0B Buffer的加载。
- `asc_copy_l12l0b_sparse` / `_sync`：用于搬运存放在L1 Buffer里的512B大小的稠密权重矩阵到L0B Buffer里，同时读取128B大小的索引矩阵用于稠密矩阵的稀疏化。

**L0C Buffer搬运（ARCH 2201）：**

- `asc_copy_l0c2l1` / `_sync`：矩阵计算完成后，对结果进行量化处理，之后将处理结果搬运到GM中。
- `asc_copy_l0c2gm` / `_sync`：将L0C Buffer中的数据搬运到GM中。
- `asc_copy_l0c2ub` / `_sync`：将L0C Buffer中的数据搬运到UB中。

**GM -> L0A Buffer / L0B Buffer搬运（ARCH 2201）：**

- `asc_copy_gm2l0a` / `asc_copy_gm2l0b` / `_sync`：将数据从GM搬运到L0A Buffer/L0B Buffer。

**其他搬运（ARCH 2201）：**

- `asc_copy_l12fb` / `_sync`：将数据从L1 Buffer搬运到Fixpipe Buffer中，Fixpipe Buffer用于存放量化参数。
- `asc_copy_l12bt` / `_sync`：将数据从L1 Buffer搬运到BiasTable Buffer中，BiasTable Buffer用于存放矩阵计算中的Bias。
- `asc_load_image_to_cbuf` / `_sync`：将图像数据从Global Memory搬运到L1 Buffer。
- `asc_fill_l0a` / `asc_fill_l0b` / `asc_fill_l1` / `_sync`：将L0A Buffer/L0B Buffer/L1 Buffer的Local Memory初始化为某一具体数值。

**3D搬运配置（通用）：**

- `asc_set_l13d_size`：设置 `asc_copy_l12l0a` / `asc_copy_l12l0b` 的3D格式搬运接口在L1 Buffer的边界值。
- `asc_set_l13d_rpt`：用于设置Load3Dv2接口的repeat参数。
- `asc_set_l13d_padding`：设置Pad属性描述，用于在调用 `asc_copy_l12l0a` 接口时配置填充数值。
- `asc_set_l13d_fmatrix`：3D fmatrix配置。
- `asc_set_l0c_copy_prequant`：数据搬运过程中进行随路量化时，通过调用该接口设置量化流程中的标量量化参数。
- `asc_set_l0c_copy_params`：DataCopy（CO1->GM、CO1->A1）过程中进行随路格式转换（NZ格式转换为ND格式）时，通过调用该接口设置格式转换的相关配置。

**ARCH 3510新增：**

- `asc_copy_l0c2l1` / `_sync`：L0C Buffer到L1 Buffer搬运，扩展量化、ReLU、裁剪等随路操作参数。
- `asc_copy_l12l0a_mx` / `_sync`：L1 Buffer到L0A Buffer微缩格式搬运。
- `asc_set_gm2l1_loop_size` / `asc_set_gm2l1_loop1_stride` / `asc_set_gm2l1_loop2_stride` / `asc_set_gm2l1_pad`：GM到L1 Buffer循环搬运参数配置。

### 矩阵计算

矩阵计算类API，单独使用时可以引入cube_compute.h。

- `asc_mmad` / `asc_mmad_sync`：完成矩阵乘加操作，支持 `bfloat16_t` / `half` / `float` / `int8_t` 输入类型，输出 `float` / `int32_t`（ARCH 2201）。
- `asc_mmad_s4` / `asc_mmad_s4_sync`：INT4稀疏矩阵乘加操作（ARCH 2201）。
- `asc_mmad_sparse` / `asc_mmad_sparse_sync`：完成矩阵乘加操作，传入的左矩阵A为稀疏矩阵，右矩阵B为稠密矩阵（ARCH 2201）。
- `asc_mmad_mx` / `asc_mmad_mx_sync`：微缩格式矩阵乘法，支持 `fp4x2_e1m2_t` / `fp4x2_e2m1_t` / `fp8_e4m3fn_t` / `fp8_e5m2_t`（ARCH 3510）。
- `asc_set_l0c2gm_config`：数据搬运过程中进行随路量化时，通过调用该接口设置量化流程中的矢量量化参数。
- `asc_set_l0c2gm_nz2nd`：NZ到ND格式转换配置（ARCH 3510）。
- `asc_set_mmad_direction_m`：设置mmad计算时优先通过M/N中的N方向，然后通过M方向产生结果，M为矩阵的行，N为矩阵的列（ARCH 2201）。
- `asc_set_mmad_direction_n`：设置mmad计算时优先通过M/N中的M方向，然后通过N方向产生结果，M为矩阵的行，N为矩阵的列（ARCH 2201）。
- `asc_enable_hf32`：用于设置Mmad计算开启HF32模式，开启该模式后L0A Buffer/L0B Buffer中的FP32数据将在参与Mmad计算之前被舍入为HF32（ARCH 2201）。
- `asc_enable_hf32_trans`：设置HF32模式取整方式，需要先使用 `asc_enable_hf32` 开启HF32取整模式（ARCH 2201）。
- `asc_set_fp32_mode`：用于设置Mmad计算开启FP32模式，开启该模式后L0A Buffer/L0B Buffer中的FP32数据在参与Mmad计算之前不做舍入处理（ARCH 2201）。

### 标量计算

标量操作类API，单独使用时可以引入scalar_compute.h。

- `asc_sflbits`：计算一个int64_t类型数字的二进制中，从最高数值位开始与符号位相同的连续比特位的个数（通用）。
- `asc_ffz`：获取一个uint64_t类型数字的二进制表示中从最低有效位开始的第一个0出现的位置，如果没找到则返回 -1（通用）。
- `asc_ffs`：FindFirstSet接口，输入数据的二进制表示中从最低位向最高位查找第一个值为1的位，并返回其位置，如果没找到则返回 -1（通用）。
- `asc_popc`：获取一个uint64_t类型数字的二进制中1的个数（通用）。
- `asc_clz`：计算参数前导零的数量（二进制从最高位到第一个1共有多少个0）（ARCH 2201）。
- `asc_zero_bits_cnt`：获取一个uint64_t类型数字的二进制中0的个数（ARCH 2201）。
- `asc_set_nthbit`：计算一个uint64_t类型数字的指定二进制位置为1，其余位保持不变（ARCH 2201）。
- `asc_clear_nthbit`：位操作函数，用于将一个uint64_t整数bits的第idx位设置为0（ARCH 2201）。
- `asc_float2int32_rd` / `_ru` / `_rn` / `_rna`：浮点到整数转换（ARCH 2201）。
- `asc_store_dev`：标量数据存储到Global Memory，支持 `int8_t` / `uint8_t` / `int16_t` / `uint16_t` / `int32_t` / `uint32_t` / `int64_t` / `uint64_t`（ARCH 3510）。

### 同步控制

同步控制类API，单独使用时可以引入sync.h。

- `asc_sync`：等待所有流水线操作完成。
- `asc_sync_vec`：同步所有流水线。
- `asc_sync_mte2`：等待PIPE_MTE2流水完成。
- `asc_sync_mte3`：等待PIPE_MTE3流水完成。
- `asc_sync_notify`：设置同步标志（宏定义）。
- `asc_sync_wait`：等待同步标志（宏定义）。
- `asc_sync_pipe`：等待指定流水线操作完成（宏定义）。
- `asc_sync_block_arrive`：该指令用于发送同步信息数据到核间同步寄存器，设置同步点（ARCH 2201）。
- `asc_sync_block_wait`：和 `asc_sync_block_arrive` 配合使用（通过flagID关联），用于等待所有同步对象到达flagID对应的同步点（ARCH 2201）。
- `asc_sync_data_barrier`：用于阻塞后续的指令执行，直到所有之前的内存访问指令（需要等待的内存位置可以通过参数控制）执行结束。

### 系统变量

系统变量类API，单独使用时可以引入sys_var.h。

- `asc_get_ctrl`：读取CTRL寄存器（控制寄存器）的值（通用）。
- `asc_set_ctrl`：设置CTRL寄存器（控制寄存器）的值（通用）。
- `asc_get_block_num`：获取AI核数（通用）。
- `asc_get_system_cycle`：获取当前系统cycle数（通用）。
- `asc_get_block_idx`：获取当前运行核的索引（ARCH 2201）。
- `asc_get_core_id`：获取当前核的编号（ARCH 2201）。
- `asc_get_sub_block_num`：分离模式下，获取一个AI Core上Cube Core（AIC）或者Vector Core（AIV）的数量（ARCH 2201）。
- `asc_get_sub_block_id`：获取AI Core上Vector核的ID（ARCH 2201）。
- `asc_get_arch_ver`：获取当前AI处理器架构版本号（ARCH 2201）。
- `asc_get_program_counter`：获取程序计数器的指针，程序计数器用于记录当前程序执行的位置（ARCH 2201）。
- `asc_get_phy_buf_addr`：基于偏移量获取片上实际物理地址（ARCH 2201）。
- `asc_get_ffts_base_addr`：获取核间同步寄存器的基地址（ARCH 2201）。
- `asc_set_ffts_base_addr`：在 `asc_sync_block_arrive` 和 `asc_sync_block_wait` 之前使用，设置核间同步寄存器的基地址（ARCH 2201）。
- `asc_get_ar_spr`：获取AR特殊寄存器的值（ARCH 2201）。
- `asc_get_overflow_status`：获取溢出状态（ARCH 2201）。

### 缓存控制

缓存控制类API，单独使用时可以引入cache_ctrl.h。

- `asc_icache_preload`：从指令所在DDR地址预加载数据到对应的cacheline中。
- `asc_get_icache_preload_status`：获取ICache的Preload的状态。
- `asc_datacache_preload`：从源地址所在的特定GM地址预加载数据到Data Cache中。
- `asc_dcci_single` / `asc_dcci_entire_out` / `asc_dcci_entire_all` / `asc_dcci_entire_atomic`：用于刷新Cache，保证Cache的一致性。
- `asc_ub_dcci_single`：Unified Buffer数据缓存失效。
- `asc_dcci_entire_ub`：全量UB数据缓存失效（ARCH 2201）。

### 原子操作

原子操作类API，单独使用时可以引入atomic.h。

- `asc_set_atomic_add_float`：设置对后续的从Unified Buffer/L0C Buffer/L1 Buffer到Global Memory的数据传输开启原子累加。累加的数据类型为float（通用）。
- `asc_set_atomic_max_float16`：设置计算结果以原子比较的方式传输到GM。在拷贝前，将待传输的half数据与GM中已有数据进行逐元素比较，并将最大值写入GM（通用）。
- `asc_set_atomic_add_bfloat` / `asc_set_atomic_add_float16` / `asc_set_atomic_add_int` / `asc_set_atomic_add_int8` / `asc_set_atomic_add_int16`：设置对后续的从Unified Buffer/L0C Buffer/L1 Buffer到Global Memory的数据传输开启原子累加，分别对应bfloat16_t / half / int32_t / int8_t / int16_t类型（ARCH 2201）。
- `asc_set_atomic_max_bfloat` / `asc_set_atomic_max_float` / `asc_set_atomic_max_int` / `asc_set_atomic_max_int8` / `asc_set_atomic_max_int16`：设置计算结果以原子比较的方式传输到GM，将待传输数据与GM中已有数据进行逐元素比较，并将最大值写入GM（ARCH 2201）。
- `asc_set_atomic_min_bfloat` / `asc_set_atomic_min_float` / `asc_set_atomic_min_float16` / `asc_set_atomic_min_int` / `asc_set_atomic_min_int8` / `asc_set_atomic_min_int16`：设置计算结果以原子比较的方式传输到GM，将待传输数据与GM中已有数据进行逐元素比较，并将最小值写入GM（ARCH 2201）。
- `asc_set_atomic_none`：清空原子操作的状态（ARCH 2201）。
- `asc_get_store_atomic_config`：获取原子操作使能位与原子操作类型的值（ARCH 2201）。
- `asc_set_store_atomic_config_v1`：设置原子操作使能位与原子操作类型的值，适用于Atlas A3训练系列产品/Atlas A3推理系列产品和Atlas A2训练系列产品/Atlas A2推理系列产品（ARCH 2201）。
- `asc_set_store_atomic_config_v2`：设置原子操作使能位与原子操作类型的值，适用于Ascend 950PR/Ascend 950DT（ARCH 3510）。

### 其他操作

- `asc_init`：初始化NPU状态。

## 支持的芯片架构

| 芯片架构 | NpuArch | 说明 |
| :--- | :--- | :--- |
| Ascend 910B系列 | 2201 | Atlas A2训练系列产品/Atlas A2推理系列产品 |
| Ascend 950系列 | 3510 | Ascend 950PR/Ascend 950DT |
