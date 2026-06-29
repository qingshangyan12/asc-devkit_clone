# Reg矢量计算接口软仿情况汇总

**表1**  基础算术

| 接口 | 软仿支持数据类型 |
| :--- | :--------------- |
| Exp | half（SubNormal）、float（SubNormal） |
| Sqrt | half（SubNormal）、float（SubNormal） |
| Add | complex32、int64_t、uint64_t、complex64 |
| AddC | - |
| Sub | complex32、int64_t、uint64_t、complex64 |
| SubC | - |
| Adds | complex32、int64_t、uint64_t、complex64 |
| Max | int64_t、uint64_t |
| Min | int64_t、uint64_t |
| Maxs | int64_t、uint64_t |
| Mins | int64_t、uint64_t |
| Div | int16_t、uint16_t、half（SubNormal）、int32_t、uint32_t、float（SubNormal）、complex32、int64_t、uint64_t、complex64 |
| Neg | int64_t |
| Ln/log | half（SubNormal）、float（SubNormal） |
| Log2/log10 | half、float |
| Mul | complex32、int64_t、uint64_t、complex64 |
| Mull | - |
| Muls | complex32、int64_t、uint64_t、complex64 |

**表2**  逻辑计算

| 接口        | 软仿支持数据类型  |
| :---------- | :---------------- |
| Not         | int64_t、uint64_t |
| And         | int64_t、uint64_t |
| Or          | int64_t、uint64_t |
| Xor         | int64_t、uint64_t |
| ShiftLeft   | int64_t、uint64_t |
| ShiftRight  | int64_t、uint64_t |
| ShiftLefts  | int64_t、uint64_t |
| ShiftRights | int64_t、uint64_t |

**表3**  复合计算

| 接口      | 软仿支持数据类型  |
| :-------- | :---------------- |
| AbsSub    | int64_t           |
| ExpSub    | -                 |
| Axpy      | int64_t、uint64_t |
| MulDstAdd | -                 |
| MulAddDst | int64_t、uint64_t |
| MulsCast  | -                 |

**表4**  比较与选择

| 接口     | 软仿支持数据类型        |
| :------- | :---------------------- |
| Compare  | -                       |
| Compares | -                       |
| Select   | bool、int64_t、uint64_t |
| Squeeze  | -                       |

**表1**  类型转换

| 接口     | 软仿支持数据类型     |
| :------- | :------------------- |
| Cast     | e8m02bf16、bf162e8m0 |
| Truncate | -                    |

**表1**  归约计算

| 接口            | 软仿支持数据类型  |
| :-------------- | :---------------- |
| Reduce          | int64_t、uint64_t |
| ReduceDataBlock | -                 |
| PairReduceElem  | -                 |

**表5**  数据填充

| 接口 | 软仿支持数据类型 |
| :--- | :--------------- |
| Duplicate | bool、fp4x2_e2m1_t、fp4x2_e1m2_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、complex32、int64_t、uint64_t、complex64 |

**表6**  离散操作

| 接口   | 软仿支持数据类型 |
| :----- | :--------------- |
| Gather | -                |

**表7**  数据重排

| 接口         | 软仿支持数据类型  |
| :----------- | :---------------- |
| Interleave   | int64_t、uint64_t |
| DeInterleave | int64_t、uint64_t |

**表8**  数据压缩

| 接口      | 软仿支持数据类型 |
| :-------- | :--------------- |
| Unsqueeze | -                |
| Pack      | -                |
| UnPack    | -                |

**表9**  直方图计算

| 接口       | 软仿支持数据类型 |
| :--------- | :--------------- |
| Histograms | -                |

**表10**  索引操作

| 接口   | 软仿支持数据类型 |
| :----- | :--------------- |
| Arange | int64_t          |

**表1**  同步控制

| 接口        | 软仿支持数据类型 |
| :---------- | :--------------- |
| LocalMemBar | -                |

**表11**  系统变量访问

| 接口     | 软仿支持数据类型 |
| :------- | :--------------- |
| ClearSpr | -                |
