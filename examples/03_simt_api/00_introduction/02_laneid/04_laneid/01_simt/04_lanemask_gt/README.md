# Lanemask_gt 示例

本示例演示如何使用 `lanemask_gt()` API 获取线程在 Warp 内的掩码。

## 功能说明

`lanemask_gt()` 返回一个32位掩码，在当前线程所属的 Warp 中，将 "laneid 严格大于当前线程" 的线程对应位设为1，其余位为0。

掩码位排列：最低位对应 laneid=0，最高位对应 laneid=31。

### API 定义

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_gt();
```

## 示例

| Lane ID | Lanemask_gt (十六进制) | 二进制 |
|---------|----------------------|--------|
| 0 | 0xfffffffe | 1111 1111 1111 1111 1111 1111 1111 1110 |
| 1 | 0xfffffffc | 1111 1111 1111 1111 1111 1111 1111 1100 |
| 2 | 0xfffffff8 | 1111 1111 1111 1111 1111 1111 1111 1000 |
| 3 | 0xfffffff0 | 1111 1111 1111 1111 1111 1111 1111 0000 |
| ... | ... | ... |
| 30 | 0x80000000 | 1000 0000 0000 0000 0000 0000 0000 0000 |
| 31 | 0x00000000 | 0000 0000 0000 0000 0000 0000 0000 0000 |

## 核函数实现

```cpp
__global__ __maxnreg__(REG_COUNT) void lanemask_gt_test(uint32_t* lanemask_output, uint64_t total_length)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx >= total_length) {
        return;
    }
    
    lanemask_output[idx] = lanemask_gt();
}
```

## Golden 数据生成

```cpp
for (uint32_t i = 0; i < total_length; i++) {
    golden[i] = 0xFFFFFFFFU << (i + 1);
}
```

## 预期输出

```
Output: 0xfffffffe 0xfffffffc 0xfffffff8 0xfffffff0 ... 0x00000000 
Golden: 0xfffffffe 0xfffffffc 0xfffffff8 0xfffffff0 ... 0x00000000 
[Success] Lanemask_gt verification passed.
```

## 编译运行

```bash
mkdir build && cd build
cmake ..
make
./demo
```

## 关键概念

- **Lanemask_gt**: 返回 laneid > 当前线程的掩码
- **掩码排列**: 最低位对应 laneid=0，最高位对应 laneid=31
- **Warp**: 32 个线程的执行单元
- **计算公式**: mask = 0xFFFFFFFFU << (laneid + 1)