# Reg Compute通用约束统计

## 统计范围

- 根目录：`docs/zh/api/SIMD-API/c_api/reg_compute`
- 统计对象：递归扫描该目录下的Markdown文件，仅纳入包含`### 通用约束`小节的文件。
- 提取边界：从`### 通用约束`后一行开始，到下一个`##`或`###`标题前结束。
- 未包含`### 通用约束`小节的文件不纳入通用约束统计。

## 总体数据

| 指标 | 数量 |
| --- | ---: |
| `reg_compute`目录Markdown文件总数 | 208 |
| 含`### 通用约束`小节的文件数 | 53 |
| 提取到的通用约束条目数 | 238 |

## 含通用约束文件的目录分布

| 目录 | 文件数 | 占含通用约束文件比例 |
| --- | ---: | ---: |
| `load` | 21 | 39.6% |
| `store` | 14 | 26.4% |
| `reg_logic` | 4 | 7.5% |
| `reg_arith` | 3 | 5.7% |
| `reg_permute_sel` | 3 | 5.7% |
| `reg_fused` | 2 | 3.8% |
| `reg_histogram` | 2 | 3.8% |
| `ub_gather` | 1 | 1.9% |
| `scatter` | 1 | 1.9% |
| `reg_gather` | 1 | 1.9% |
| `reg_sync` | 1 | 1.9% |

## 归并后的约束组

下表“覆盖比例”以53个含`### 通用约束`小节的文件为分母；“全目录比例”以208个`reg_compute`目录Markdown文件为分母。

| 约束组 | 覆盖文件数 | 覆盖比例 | 全目录比例 | 条目数 | 涉及目录 | 初步归属判断 |
| --- | ---: | ---: | ---: | ---: | --- | --- |
| AIV生效/非AIV调用行为 | 53 | 100.0% | 25.5% | 57 | `load`、`store`、`reg_logic`、`reg_arith`、`reg_permute_sel`、`reg_fused`、`reg_histogram`、`ub_gather`、`scatter`、`reg_gather`、`reg_sync` | 候选全体Reg Compute通用约束 |
| 必须在Vector Function内调用 | 50 | 94.3% | 24.0% | 50 | `load`、`store`、`reg_logic`、`reg_permute_sel`、`reg_fused`、`reg_histogram`、`reg_arith`、`ub_gather`、`scatter`、`reg_gather`、`reg_sync` | 强候选全体Reg Compute通用约束，需确认3个未出现文件 |
| UB地址重叠需插入`asc_mem_bar`/同步 | 38 | 71.7% | 18.3% | 38 | `load`、`store`、`reg_permute_sel`、`scatter` | 更像搬运/写UB相关指令约束 |
| UB容量/访问范围不可越界 | 38 | 71.7% | 18.3% | 64 | `load`、`store`、`reg_permute_sel`、`scatter` | 更像访问UB地址的指令约束 |
| 读取地址对齐/读取地址范围 | 21 | 39.6% | 10.1% | 23 | `load`、`ub_gather` | Reg搬入/聚合搬入类约束 |
| 目的地址/写出地址对齐 | 11 | 20.8% | 5.3% | 11 | `store`、`load`、`scatter` | Reg搬出/离散写出及少量特殊搬入类约束 |
| `mask`必须预先赋值 | 14 | 26.4% | 6.7% | 14 | `reg_logic`、`reg_arith`、`store`、`reg_fused`、`reg_histogram`、`scatter` | 使用`vector_bool mask`参数的指令约束 |
| `mask`无效位输出置零/不参与计算 | 7 | 13.2% | 3.4% | 7 | `reg_logic`、`reg_fused`、`reg_arith` | 计算类指令结果语义约束 |
| 返回值/引用参数的非AIV返回差异 | 4 | 7.5% | 1.9% | 8 | `reg_arith`、`ub_gather`、`reg_gather` | 同时提供返回值和引用输出原型的指令约束 |
| 寄存器/操作数必须不同 | 1 | 1.9% | 0.5% | 1 | `load` | 接口专属约束 |
| 非对齐搬运调用顺序/后处理 | 1 | 1.9% | 0.5% | 1 | `store` | 接口专属约束 |

## 高频约束的原始表述归并

### AIV生效/非AIV调用行为

归并后语义：接口仅在AIV上生效，非AIV调用时按接口原型返回。

原始表述分布：

| 原始表述 | 次数 |
| --- | ---: |
| 本接口仅在AIV上生效，非AIV调用直接返回。 | 24 |
| 非AIV调用直接返回。 | 16 |
| 本接口非AIV调用直接返回。 | 7 |
| 通过函数返回值输出结果的函数原型在非AIV上调用时返回对应矢量类型的默认构造值。 | 4 |
| 通过引用参数输出结果的函数原型在非AIV上调用时直接返回。 | 4 |
| 本接口在非AIV上调用直接返回。 | 2 |

判断依据：该约束覆盖所有53个含通用约束的接口页，可以作为全体Reg Compute接口的候选通用约束。但其中返回值原型和引用输出原型存在行为差异，若抽成统一表述，需要保留“按接口原型返回”的说明。

### 必须在Vector Function内调用

归并后语义：接口在Vector Function（`__simd_vf__`标记的函数）内调用。

原始表述分布：

| 原始表述 | 次数 |
| --- | ---: |
| 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。 | 42 |
| 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为矢量数据寄存器或掩码寄存器，目的操作数为UB地址。UB地址空间外的指针不可作为目的地址传入。 | 2 |
| 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为矢量数据寄存器，目的操作数为UB地址，UB地址空间外的指针不可作为`dst`传入。 | 2 |
| 本接口在Vector Function（`__simd_vf__` 标记的函数）内调用，源操作数为UB地址、目的操作数为矢量数据寄存器。 | 1 |
| 本接口在Vector Function（`__simd_vf__` 标记的函数）内调用，源/目的均为矢量数据寄存器。 | 1 |
| 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数和目的操作数为矢量数据寄存器或掩码寄存器。 | 1 |
| 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数和索引为矢量数据寄存器，目的操作数为UB地址。 | 1 |

未命中文件：

- `reg_arith/asc_add.md`
- `reg_arith/asc_sub.md`
- `store/asc_storeunalign_post_postupdate.md`

判断依据：覆盖50/53，且`asc_add.md`示例本身使用`__simd_vf__`，因此该约束很可能属于全体Reg Compute接口；但上述3个文件未在`### 通用约束`中写出，需要确认是否补齐或作为例外。

### UB地址重叠需插入`asc_mem_bar`/同步

归并后语义：本指令与其他指令存在UB地址重叠时，需要插入`asc_mem_bar`等同步指令，保证串行化，防止异常数据。

原始表述分布：

| 原始表述 | 次数 |
| --- | ---: |
| 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。 | 34 |
| 如果本接口与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化。 | 2 |
| 如果本指令与其他指令存在UB地址重叠，必须插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。 | 2 |

判断依据：覆盖38/53，集中在`load`、`store`、`scatter`和少量`reg_permute_sel`。计算类Reg指令大多只操作寄存器，不直接访问UB，因此该约束更适合作为“访问UB的Reg搬运/写出类指令通用约束”，不宜直接归为全体Reg Compute约束。

### UB容量/访问范围不可越界

归并后语义：UB总容量为256KB，实际可用容量受编译选项和编程场景影响；接口访问的UB地址、偏移后地址、读取范围或写出范围不得超过实际可用UB范围。

判断依据：覆盖38/53，目录分布与“UB地址重叠需同步”基本一致，也集中在访问UB的搬入、搬出、scatter和部分写出类接口。可作为“访问UB地址的Reg指令通用约束”，不建议作为纯寄存器计算类指令约束。

### 读取地址对齐/读取地址范围

归并后语义：Reg搬入类接口读取UB地址时，需要满足对应的32字节、16字节或`dtype`对齐要求，且读取范围不能越界。

判断依据：覆盖21/53，主要为`load`目录，另有`ub_gather/asc_gather.md`。适合归为Reg数据搬入/聚合搬入类约束。

### 目的地址/写出地址对齐

归并后语义：写出到UB的目的地址需要满足接口规定的32字节、`dtype`或`sizeof(dtype)`对齐要求。

判断依据：覆盖11/53，主要为`store`目录，另有`scatter`和少量特殊`load`接口。适合归为Reg数据搬出/离散搬出及特殊写UB接口约束。

### `mask`必须预先赋值

归并后语义：带`mask`参数的接口调用前，`mask`必须通过掩码设置接口预先赋值，否则掩码寄存器内容不确定，可能导致有效元素位置错误。

判断依据：覆盖14/53，出现在`reg_arith`、`reg_logic`、`reg_fused`、`reg_histogram`、`store`、`scatter`。适合归为“带`vector_bool mask`参数的接口通用约束”。

### `mask`无效位输出置零/不参与计算

归并后语义：`mask`位为0的元素不参与对应运算，输出对应位置写0。

判断依据：覆盖7/53，集中在`reg_logic`、`reg_fused`和部分`reg_arith`。该约束描述的是计算结果语义，不适合作为全体Reg Compute通用约束。

## 初步结论

### 候选全体Reg Compute通用约束

1. AIV生效/非AIV调用行为：覆盖53/53，建议抽成全体通用约束。抽象表述需要兼容“直接返回”和“返回默认构造值”两类原型。
2. Vector Function内调用：覆盖50/53，建议作为全体通用约束候选。需要确认`reg_arith/asc_add.md`、`reg_arith/asc_sub.md`、`store/asc_storeunalign_post_postupdate.md`是否应补齐该约束。

### 候选分类通用约束

1. 访问UB的Reg搬运/写出类指令：UB地址重叠需同步、UB容量/访问范围不可越界。
2. Reg数据搬入/聚合搬入类指令：读取地址对齐和读取范围约束。
3. Reg数据搬出/离散搬出类指令：目的地址/写出地址对齐约束。
4. 带`vector_bool mask`参数的指令：mask必须预先赋值。
5. 计算类指令：mask无效位输出置零或不参与计算的结果语义。

### 接口专属约束

1. 寄存器或操作数必须不同：当前仅命中1个`load`文件。
2. 非对齐搬运调用顺序/后处理：当前仅命中1个`store`文件。

## 待确认问题

1. 是否将“AIV生效/非AIV调用行为”抽成`reg_compute`全局通用约束，并在各接口页中删减重复表述？
2. 是否将“Vector Function内调用”补齐到3个未命中文件后，也抽成全局通用约束？
3. 对“UB地址重叠需同步”和“UB容量/访问范围不可越界”，是否按`load`、`store`、`scatter`、相关`reg_permute_sel`接口归为“访问UB地址的Reg指令通用约束”？
4. 对`mask`相关约束，是否只在带`vector_bool mask`参数的接口中保留，不上升为全体Reg Compute通用约束？
