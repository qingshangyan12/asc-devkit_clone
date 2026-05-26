# asc_vf_call 打点结果（固定512线程版本）

## 测试配置

- 设备：Ascend NPU，dav-3510，64 AIV 核
- 数据量：32768
- 除数：7
- 线程配置：固定每核 512 线程，根据数据量反推核数（32768 / 512 = 64 核）
- 打点方式：`__global__ __vector__` 中用 `PipeBarrier<PIPE_ALL>()` + `GetSystemCycle()` 包住 `asc_vf_call`，仅 block 0 记录
- input 全量预加载到 UB，SIMT 函数从 UB 读取，output 写 GM
- 每次验证前清 cmake 缓存（`rm -rf *`），重新 cmake + make

## ASC 文件说明

- `integer_fast_div_vf_timing.asc`：快速除法（`__umulhi` + 移位），固定 512 线程，asc_vf_call 前后打点
- `integer_normal_div_vf_timing.asc`：普通除法（`/` 运算符），固定 512 线程，asc_vf_call 前后打点

## 结果（各 10 次，每次清 cmake 缓存）

| 次数 | fast_div（cycles） | normal_div（cycles） |
|:----:|:-----------------:|:-------------------:|
| 1    | 768               | 824                 |
| 2    | 553               | 619                 |
| 3    | 511               | 1100                |
| 4    | 609               | 706                 |
| 5    | 643               | 950                 |
| 6    | 485               | 730                 |
| 7    | 939               | 856                 |
| 8    | 665               | 1336                |
| 9    | 561               | 1010                |
| 10   | 721               | 1109                |
| **范围** | **485 ~ 939** | **619 ~ 1336**  |
| **均值** | **645.5**     | **924.0**       |
