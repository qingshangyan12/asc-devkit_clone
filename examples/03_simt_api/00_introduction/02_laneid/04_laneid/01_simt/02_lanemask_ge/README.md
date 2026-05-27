# Lanemask_ge 示例

本示例演示如何使用 `lanemask_ge()` API 获取线程在 Warp 内的掩码。

## 功能说明

`lanemask_ge()` 返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 大于等于当前线程" 的线程对应位设为1，其余位为0。

掩码位排列：最高位对应 laneid=0，最低位对应 laneid=31。

### API 定义

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_ge();
```

## 示例

| Lane ID | Lanemask_ge (二进制) | 十进制值 |
|---------|---------------------|---------|
| 0 | 1111 1111 1111 1111 1111 1111 1111 1111 | 4294967295 |
| 1 | 0111 1111 1111 1111 1111 1111 1111 1111 | 2147483647 |
| 2 | 0011 1111 1111 1111 1111 1111 1111 1111 | 1073741823 |
| ... | ... | ... |
| 31 | 0000 0000 0000 0000 0000 0000 0000 0001 | 1 |

## 核函数实现

```cpp
__global__ __maxnreg__(REG_COUNT) void lanemask_ge_test(uint32_t* lanemask_output, uint64_t total_length)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx >= total_length) {
        return;
    }
    
    lanemask_output[idx] = lanemask_ge();
}
```

## Golden 数据生成

```cpp
for (uint32_t i = 0; i < total_length; i++) {
    uint32_t mask = 0;
    for (uint32_t lane = 0; lane < WARP_SIZE; lane++) {
        if (lane >= i) {
            mask |= (1U << (WARP_SIZE - 1 - lane));
        }
    }
    golden[i] = mask;
}
```

## 预期输出

```
Output: 4294967295 2147483647 1073741823 ... 1 
Golden: 4294967295 2147483647 1073741823 ... 1 
[Success] Lanemask_ge verification passed.
```

## 编译运行

```bash
mkdir build && cd build
cmake ..
make
./demo
```

## 关键概念

- **Lanemask_ge**: 返回 laneid >= 当前线程的掩码
- **掩码排列**: 最高位对应 laneid=0，最低位对应 laneid=31
- **Warp**: 32 个线程的执行单元
- **计算公式**: mask = ((1U << (32 - laneid)) - 1) << laneid 或使用循环生成