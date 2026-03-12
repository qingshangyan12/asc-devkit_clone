# asc_copy_gm2l1

## 产品支持情况

| 产品         | 是否支持 |
| :-----------------------| :-----:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

将矩阵数据从Global Memory (GM)搬运到Level 1 cache (L1)的DMA指令，包含padding操作。

## 函数原型

- 常规计算

    ```cpp
    __aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride, uint32_t dst_stride)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode, uint64_t src_stride, uint32_t dst_stride)
    ```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src | 输入 | 源操作数（矢量）的起始地址。 |
| burst_num | 输入 | 待搬运的连续传输数据块个数。 |
| burst_len | 输入 | 待搬运的每个连续传输数据块的长度，单位为DataBlock（32字节）。 |
| pad_func_mode | 输入 | 一个4Bit的值，用于指示通道上的PAD功能，他会在32B的数据中填充数据；或从每个32B的读取数据中移除PAD值。<br>&bull; 0：不做padding。 <br>&bull; 1：每有1B的数据，插入31个padding值。 <br>&bull; 2：每有2B的数据，插入15个padding值。 <br>&bull; 3：每有4B的数据，插入14个padding值。4：每有8B的数据，插入12个padding值。 <br>&bull; 5：每有16B的数据，插入8个padding值。<br>&bull; 6：每有32B的数据，移除28个MSB数据，只保留4个LSB元素。<br>&bull; 7：每有32B的数据，移除24个MSB数据，只保留8个LSB元素。<br>&bull; 7：每有32B的数据，移除16个MSB数据，只保留16个LSB元素。 |
| src_stride | 输入 | 输入数据中两个相邻连续数据块之间的距离。 |
| dst_stride | 输入 | 输出数据中两个相邻连续数据块之间的距离。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 对于插入padding值的场景，burst_len的单位为32B的指的是写入L1的实际数据数，它必须设置为1，此时一次仅从gm里读取1/2/4/8/16B的数据。
- 对于插入padding值的场景，每一次burst都是从gm中连续读取，因此不需要设置src_stride
- 对于插入padding值的场景，dst_stride以32B为单位
- 对于移除padding值的场景，源操作数中一次burst操作的数据为burst_len * 32B，目的操作数中一次burst的实际数据为burst_len * 4/8/16B
- 对于移除padding值的场景，每一次burst后都连续地往l1中写入，因此不需要设置dst_stride
- 对于移除padding值的场景，src_stride以32B为单位

## 调用示例

```cpp
__gm__ half src[256];
__cbuf__ half dst[256];
asc_copy_gm2l1(dst, src, 1, 4, 0, 0, 0);
```