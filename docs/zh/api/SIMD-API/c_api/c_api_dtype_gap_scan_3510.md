# 3510架构C API数据类型缺口扫描报告

## 扫描目的

本文档记录3510架构下Regbase C API与编译器CCE矢量指令之间的数据类型覆盖差异，为后续补齐C API重载、推动编译器能力确认以及补充单元测试提供依据。

本次扫描仅分析数据类型组合，不评估接口命名、舍入模式、饱和模式、奇偶位置和掩码模式是否需要扩展。

## 扫描范围

扫描范围如下：

- C API公开声明：`include/c_api/reg_compute/`。
- 3510架构C API实现：`impl/c_api/instr_impl/npu_arch_3510/`。
- C API单元测试：`tests/api/c_api/npu_arch_3510/`。
- 编译器CCE矢量指令头文件路径为：`${CANN_PATH}/x86_64-linux/ccec_compiler/lib/clang/15.0.5/include/__clang_cce_vector_intrinsics.h`。

本次共扫描214个实现头文件、3043个`asc_*_impl`重载，涉及117个CCE指令名称。

## 扫描方法

编译器头文件通过3510架构的AIV Device编译路径展开，关键编译选项如下：

```text
--npu-arch=dav-3510 --cce-aicore-only --cce-aiv
```

普通预处理结果会同时包含Host、Cube和Vector编译阶段的声明，可能将其他阶段或其他条件分支下的重载误判为3510架构AIV能力。因此，本次仅使用AIV Device编译路径下实际生效的重载。

对比过程按照完整参数形态进行，包含目的操作数、源操作数、索引类型和指针数据类型。仅比较第一个矢量参数会错误合并同名指令的不同加载模式、索引模式和数据转换方向。

## CCE已支持但C API缺失的类型组合

共确认29组高置信数据类型组合缺口。表中不重复统计舍入模式、饱和模式和位置模式产生的重载数量。

**表 1**  CCE已支持但C API缺失的类型组合

| C API | CCE指令 | C API缺失的数据类型组合 | 数量 |
| --- | --- | --- | --- |
| `asc_and` | `vand` | `vector_fp8_e8m0_t` | 1 |
| `asc_madd` | `vmadd` | `vector_bfloat16_t` | 1 |
| `asc_mul_scalar` | `vmuls` | `vector_bfloat16_t`与`bfloat16_t`标量 | 1 |
| `asc_not` | `vnot` | `vector_fp8_e8m0_t`、`vector_fp8_e5m2_t`、`vector_fp8_e4m3fn_t`、`vector_bfloat16_t` | 4 |
| `asc_or` | `vor` | `vector_fp8_e8m0_t`、`vector_fp8_e5m2_t`、`vector_fp8_e4m3fn_t`、`vector_bfloat16_t` | 4 |
| `asc_xor` | `vxor` | `vector_fp8_e8m0_t`、`vector_fp8_e5m2_t`、`vector_fp8_e4m3fn_t`、`vector_half`、`vector_bfloat16_t`、`vector_float` | 6 |
| `asc_select` | `vsel` | `vector_fp8_e8m0_t` | 1 |
| `asc_reduce_sum_datablock` | `vcgadd` | `vector_int16_t`到`vector_int16_t`、`vector_uint16_t`到`vector_uint16_t` | 2 |
| `asc_scatter` | `vscatter` | `vector_fp8_e8m0_t`、`vector_fp8_e5m2_t`、`vector_fp8_e4m3fn_t`，索引类型均为`vector_uint16_t` | 3 |
| 待新增转换接口 | `vcvt` | `vector_half`到`vector_fp8_e5m2_t`、`vector_half`到`vector_fp8_e4m3fn_t`、`vector_bfloat16_t`到`vector_fp8_e5m2_t`、`vector_bfloat16_t`到`vector_fp8_e4m3fn_t` | 4 |
| 待新增转换接口 | `vcvt_bf162e8m0`、`vcvt_e8m02bf16` | `vector_bfloat16_t`到`vector_fp8_e8m0_t`、`vector_fp8_e8m0_t`到`vector_bfloat16_t` | 2 |

这些缺口在公开声明、3510架构实现和现有单元测试中同时存在，说明并非单一转发层遗漏，而是完整C API能力链尚未覆盖。

### 逻辑运算类

3510架构下，`vand`、`vor`、`vxor`和`vnot`支持的数据类型集合基本一致，均包含三种FP8数据类型和`bfloat16_t`。当前C API对应接口的类型集合不一致：

- `asc_and`已支持`fp8_e5m2_t`和`fp8_e4m3fn_t`，但缺少`fp8_e8m0_t`。
- `asc_or`和`asc_not`缺少`bfloat16_t`及三种FP8数据类型。
- `asc_xor`仅支持整数和`bool`类型，缺少编译器已经提供的浮点类型。

因此，逻辑运算类属于优先级较高、实现方式明确的数据类型补齐候选。

### 乘法累加和标量乘法

`vmadd`在3510架构下提供`bfloat16_t`重载，但`asc_madd`当前仅支持`half`和`float`。`vmuls`同样提供`bfloat16_t`重载，但`asc_mul_scalar`未进行声明和转发。

这两个接口均可沿用现有重载的参数透传形式补齐，仍需同步增加公开声明、接口文档和单元测试。

### 选择和规约类

`vsel`提供`fp8_e8m0_t`重载，而`asc_select`已经支持另外两种FP8数据类型和`hifloat8_t`，因此`fp8_e8m0_t`属于明显缺口。

`vcgadd`同时提供16位同宽规约和16位到32位扩展规约。`asc_reduce_sum_datablock`当前仅封装16位到32位扩展形式，缺少以下同宽形式：

```text
vector_int16_t -> vector_int16_t
vector_uint16_t -> vector_uint16_t
```

### Scatter类

3510架构下，`vscatter`与`vgather2`支持的数据类型集合基本对称，均包含三种FP8数据类型。`asc_gather`已经封装FP8类型，而`asc_scatter`仅封装整数、`half`、`bfloat16_t`和`float`，缺少三种FP8重载。

FP8 Scatter接口的索引类型均应为`vector_uint16_t`。

### 数据转换类

`vcvt`在3510架构下明确提供`half`和`bfloat16_t`到`fp8_e5m2_t`、`fp8_e4m3fn_t`的转换能力。当前C API仅提供`float`到这两种FP8类型的转换接口。

此外，编译器通过独立指令提供`bfloat16_t`与`fp8_e8m0_t`之间的双向转换，但`reg_convert.h`中没有任何`fp8_e8m0_t`转换接口。这部分需要新增接口名称，并根据CCE支持情况定义舍入、饱和和位置参数组合。

## 编译器侧待确认的数据类型

以下类型缺失发生在CCE指令层，不能通过简单增加C API转发解决。是否属于编译器能力缺口，需要结合硬件指令支持情况进一步确认。

**表 2**  编译器侧待确认的数据类型

| C API | CCE指令 | CCE缺失的数据类型 | 对比依据 |
| --- | --- | --- | --- |
| `asc_gather`的Vector到Vector形式 | `vselr` | `vector_float` | 功能相近的`vsel`支持`vector_float`，且`vselr`已支持其他32位类型。 |
| `asc_abs_sub` | `vabsdif` | 整数类型 | 3510架构分支仅保留`half`和`float`，其他架构存在部分整数重载。 |
| `asc_mul`、`asc_mul_scalar` | `vmul`、`vmuls` | `int8_t`、`uint8_t` | 8位重载在3510架构条件下被明确排除。 |
| `asc_reduce_max`、`asc_reduce_min` | `vcmax`、`vcmin` | `int8_t`、`uint8_t` | 8位规约重载在3510架构条件下被明确排除。 |
| `asc_reduce_sum` | `vcadd` | `int8_t`到`int16_t`、`uint8_t`到`uint16_t` | 8位输入的扩展规约重载在3510架构条件下被明确排除。 |

### asc_gather的float类型

`asc_gather`的Unified Buffer地址到Vector形式已经支持`float`，对应`vgather2`。Vector到Vector形式对应`vselr`，编译器当前没有`f32`重载。

因此，不能直接在C API中增加以下重载：

```cpp
__simd_callee__ inline void asc_gather(
    vector_float& dst, vector_float src, vector_uint32_t index);
```

应先由编译器团队确认底层Builtin和硬件是否支持`vselr`的`f32`形式。

## 位模式转发候选

`hifloat8_t`没有对应的原生逻辑运算和Scatter CCE重载，但现有`asc_select`、`asc_gather`已经采用重解释为`uint8_t`的方式进行转发。基于相同位模式，可以进一步评估以下接口是否补充`hifloat8_t`：

- `asc_and`。
- `asc_or`。
- `asc_xor`。
- `asc_not`。
- `asc_scatter`。

这些接口不属于“CCE存在同类型重载”的直接缺口，因此未计入表 1的29组结果。增加此类接口前，需要确认C API是否允许通过位模式解释扩展数据类型能力。

## 已排除的误报

### Host阶段声明

未限制AIV Device编译路径时，编译器预处理结果会混入Host阶段声明，使`vabsdif`整数类型、`vmul`的8位类型等在扫描结果中出现。实际3510架构AIV分支并不包含这些重载，因此已从C API直接缺口中排除。

### 同名指令的不同模式

`vld`、`vlds`、`vst`、`vsts`和`vgather2_bc`等指令具有多种加载、存储、广播、解包和索引模式。不同模式支持的数据类型集合不同，不能将同名指令的全部数据类型套用到每个C API重载。本次按照完整参数和调用模式复核后，已排除相关误报。

### 64位矢量类型

编译器部分64位辅助指令使用`vector_2xvl_s64`和`vector_2xvl_u64`，C API中的`vector_int64_t`和`vector_uint64_t`分别映射为`vector_s64`和`vector_u64`。两组类型的布局和接口参数不同，不能据此认定`asc_add`、`asc_mul`等接口缺少现有64位C API重载。

## 后续建议

建议按照以下顺序处理：

1. 优先补齐表 1中逻辑运算、`asc_madd`、`asc_mul_scalar`、`asc_select`、`asc_reduce_sum_datablock`和`asc_scatter`的直接重载。
2. 单独设计FP8转换接口，明确接口命名以及舍入、饱和、位置参数组合。
3. 向编译器团队确认`vselr`的`f32`能力及表 2中的其他数据类型限制。
4. 评估`hifloat8_t`位模式转发是否符合C API的数据类型设计原则。
5. 每个新增类型同步补充公开声明、3510架构实现、接口文档和单元测试。

## 结论

本次扫描确认29组CCE已经支持但C API尚未覆盖的数据类型组合。其中，逻辑运算、BF16乘法累加、BF16标量乘法、FP8 Scatter和FP8转换的证据最明确。

`asc_gather`的Vector到Vector形式缺少`float`不是C API转发遗漏，而是`vselr`缺少`f32`重载，应优先由编译器侧确认能力边界。
