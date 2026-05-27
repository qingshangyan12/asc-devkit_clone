# Lanemask_eq 示例

本示例演示如何使用 `lanemask_eq()` API 获取线程在 Warp 内的掩码。

## 功能说明

`lanemask_eq()` 返回一个32位掩码，在当前线程所属的 Warp 中，只有当前线程所在的 lane 位被置为1，其余位为0。

掩码位排列：最低位对应 laneid=0，最高位对应 laneid=31。

### API 定义

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_eq();
```

## 示例

| Lane ID | Lanemask_eq (十六进制) | 二进制 |
|---------|----------------------|--------|
| 0 | 0x00000001 | 0000 0000 0000 0000 0000 0000 0000 0001 |
| 1 | 0x00000002 | 0000 0000 0000 0000 0000 0000 0000 0010 |
| 2 | 0x00000004 | 0000 0000 0000 0000 0000 0000 0000 0100 |
| 3 | 0x00000008 | 0000 0000 0000 0000 0000 0000 0000 1000 |
| ... | ... | ... |
| 31 | 0x80000000 | 1000 0000 0000 0000 0000 0000 0000 0000 |

## 核函数实现

```cpp
__global__ __maxnreg__(REG_COUNT) void lanemask_eq_test(uint32_t* lanemask_output, uint64_t total_length)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx >= total_length) {
        return;
    }
    
    lanemask_output[idx] = lanemask_eq();
}
```

## Golden 数据生成

```cpp
for (uint32_t i = 0; i < total_length; i++) {
    golden[i] = 1U << i;
}
```

## 预期输出

```
Output: 0x00000001 0x00000002 0x00000004 0x00000008 ... 0x80000000 
Golden: 0x00000001 0x00000002 0x00000004 0x00000008 ... 0x80000000 
[Success] Lanemask_eq verification passed.
```

## 编译运行

```bash
mkdir build && cd build
cmake ..
make
./demo
```

## 关键概念

- **Lanemask_eq**: 返回仅当前线程 laneid 位为1的掩码
- **掩码排列**: 最低位对应 laneid=0，最高位对应 laneid=31
- **Warp**: 32 个线程的执行单元
- **计算公式**: mask = 1U << laneid