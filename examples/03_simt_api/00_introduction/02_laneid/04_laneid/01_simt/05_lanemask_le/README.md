# Lanemask_le 示例

本示例演示如何使用 `lanemask_le()` API 获取线程在 Warp 内的掩码。

## 功能说明

`lanemask_le()` 返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 小于等于当前线程" 的线程对应位设为1，其余位为0。

掩码位排列：最低位对应 laneid=0，最高位对应 laneid=31。

### API 定义

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_le();
```

## 示例

| Lane ID | Lanemask_le (十六进制) | 二进制 |
|---------|----------------------|--------|
| 0 | 0x00000001 | 0000 0000 0000 0000 0000 0000 0000 0001 |
| 1 | 0x00000003 | 0000 0000 0000 0000 0000 0000 0000 0011 |
| 2 | 0x00000007 | 0000 0000 0000 0000 0000 0000 0000 0111 |
| 3 | 0x0000000f | 0000 0000 0000 0000 0000 0000 0000 1111 |
| ... | ... | ... |
| 30 | 0x7fffffff | 0111 1111 1111 1111 1111 1111 1111 1111 |
| 31 | 0xffffffff | 1111 1111 1111 1111 1111 1111 1111 1111 |

## 核函数实现

```cpp
__global__ __maxnreg__(REG_COUNT) void lanemask_le_test(uint32_t* lanemask_output, uint64_t total_length)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx >= total_length) {
        return;
    }
    
    lanemask_output[idx] = lanemask_le();
}
```

## Golden 数据生成

```cpp
for (uint32_t i = 0; i < total_length; i++) {
    golden[i] = 0xFFFFFFFFU >> (31 - i);
}
```

## 预期输出

```
Output: 0x00000001 0x00000003 0x00000007 0x0000000f ... 0xffffffff 
Golden: 0x00000001 0x00000003 0x00000007 0x0000000f ... 0xffffffff 
[Success] Lanemask_le verification passed.
```

## 编译运行

```bash
mkdir build && cd build
cmake ..
make
./demo
```

## 关键概念

- **Lanemask_le**: 返回 laneid <= 当前线程的掩码
- **掩码排列**: 最低位对应 laneid=0，最高位对应 laneid=31
- **Warp**: 32 个线程的执行单元
- **计算公式**: mask = 0xFFFFFFFFU >> (31 - laneid)