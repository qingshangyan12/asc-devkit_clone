# Reg矢量计算API数据类型及精度标准汇总

**表 1**  Reg矢量计算API精度标准

| 接口类型 | 接口 | 软仿支持数据类型 |
| :------- | :--- | :--------------- |
| 基础算术 | Exp | half/float （SubNormal） |
| 基础算术 | Sqrt | half/float （SubNormal） |
| 基础算术 | Add | complex32、int64_t、uint64_t、complex64 |
| 基础算术 | AddC | / |
| 基础算术 | Sub | complex32、int64_t、uint64_t、complex64 |
| 基础算术 | SubC | / |
| 基础算术 | Adds | complex32、int64_t、uint64_t、complex64 |
| 基础算术 | Max | int64_t、uint64_t |
| 基础算术 | Min | int64_t、uint64_t |
| 基础算术 | Maxs | int64_t、uint64_t |
| 基础算术 | Mins | int64_t、uint64_t |
| 基础算术 | Div | int16_t、uint_16_t、half（SubNormal）、int32_t、uint32_t、float（SubNormal）、complex32、int64_t、uint64_t、complex64 |
| 基础算术 | Neg | int64_t |
| 基础算术 | Ln/log | half/float (SubNormal) |
| 基础算术 | Log2/log10 | half/float |
| 基础算术 | Mul | complex32、int64_t、uint64_t、complex64 |
| 基础算术 | Mull | / |
| 基础算术 | Muls | complex32、int64_t、uint64_t、complex64 |
| 逻辑计算 | Not | int64_t、uint64_t |
| 逻辑计算 | And | int64_t、uint64_t |
| 逻辑计算 | Or | int64_t、uint64_t |
| 逻辑计算 | Xor | int64_t、uint64_t |
| 逻辑计算 | ShiftLeft | int64_t、uint64_t |
| 逻辑计算 | ShiftRight | int64_t、uint64_t |
| 逻辑计算 | ShiftLefts | int64_t、uint64_t |
| 逻辑计算 | ShiftRights | int64_t、uint64_t |
| 复合计算 | AbsSub | int64_t |
| 复合计算 | ExpSub | / |
| 复合计算 | Axpy | int64_t、uint64_t |
| 复合计算 | MulDstAdd | / |
| 复合计算 | MulAddDst | int64_t、uint64_t |
| 复合计算 | MulsCast | / |
| 比较与选择 | Compare | / |
| 比较与选择 | Compares | / |
| 比较与选择 | Select | bool、int64_t、uint64_t |
| 比较与选择 | Squeeze | / |
| 类型转换 | Cast | e8m02bf16、bf162e8m0 |
| 类型转换 | Truncate | / |
| 归约计算 | Reduce | int64_t、uint64_t |
| 归约计算 | ReduceDataBlock | / |
| 归约计算 | PairReduceElem | / |
| 数据填充 | Duplicate | bool、fp4x2_e2m1_t、fp4x2_e1m2_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、complex32、int64_t、uint64_t、complex64 |
| 离散操作 | Gather | / |
| 数据重排 | Interleave | int64_t、uint64_t |
| 数据重排 | Deinterleave | int64_t、uint64_t |
| 数据压缩 | Unsqueeze | / |
| 数据压缩 | Pack | / |
| 数据压缩 | UnPack | / |
| 直方图计算 | Histograms | / |
| 索引操作 | Arange | int64_t |
| 同步控制 | LocalMemBar | / |
| 系统变量访问 | ClearSpr | / |

**表 2**  Vector低精度指令支持情况

| 数据类型 | 接口 |
| :------- | :--- |
| fp4x2_e2m1_t | Cast、Duplicate(软仿、scalarValue需要为int8_t) |
| fp4x2_e1m2_t | Cast、Duplicate(软仿、scalarValue需要为int8_t) |
| fp8_e5m2_t | Cast、Duplicate(软仿、scalarValue需要为int8_t) |
| fp8_e4m3fn_t | Cast、Duplicate(软仿、scalarValue需要为int8_t) |
| fp8_e8m0_t | Cast(软仿)、Duplicate(软仿、scalarValue需要为int8_t) |
| hifloat8_t | Cast、Duplicate(软仿、scalarValue需要为int8_t) |
| int4x2_t | Cast |
| int8_t | Cast、Add、Sub、Max、Min、Compare、Abs、Neg、Adds、Muls、Maxs、Mins、Compares、Duplicate |
| uint8_t | Cast、Add、Sub、Max、Min、Compare、Adds、Muls、Maxs、Mins、Compares、Duplicate |
