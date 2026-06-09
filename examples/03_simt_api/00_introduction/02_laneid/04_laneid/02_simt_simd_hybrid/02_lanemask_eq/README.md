# Lanemask_eq 功能验证示例

## 概述
本示例用于验证 SIMT API 中 `lanemask_eq()` 接口的功能正确性。`lanemask_eq()` 返回当前线程的一个32位掩码，在当前线程所属的Warp中，只有当前线程所在的 lane 位被置为1，其余位为0。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- >= CANN 9.0.0-beta.2

## lanemask_eq() 接口说明
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_eq();
```

**功能**: 返回当前线程的32位掩码，当前线程所在 lane 位置为1，其余位为0。

**返回值**: 
- 类型: `uint32_t`
- 范围: 32位掩码值
- 特性: 对于 laneid 为 N 的线程，返回值为 `(1 << N)`

**示例**:
- laneid 为 0 的线程：`lanemask_eq() = 0x00000001` (二进制: 0000 0000 0000 0000 0000 0000 0000 0001)
- laneid 为 31 的线程：`lanemask_eq() = 0x80000000` (二进制: 1000 0000 0000 0000 0000 0000 0000 0000)

## 样例实现

### 文件说明
- `lanemask_eq.asc`: 主程序文件，包含验证 lanemask_eq() 功能的完整代码

### 实现原理
1. **线程配置**: 启动 32 个线程（1 个 block，每个 block 32 个线程）
2. **掩码获取**: 每个 SIMT 线程调用 `lanemask_eq()` 获取自己的 lane 掩码
3. **结果验证**: 
   - 预期值: `lanemask_eq = (1 << (threadIdx.x % 32))`
   - 实际值: `lanemask_eq()` 返回值
   - 对比验证每个线程的 lanemask_eq 是否正确

### 核心代码逻辑
```cpp
__simt_vf__ inline void simt_lanemask_eq_test(
    __gm__ uint32_t* lanemaskOutput, uint32_t totalThreads)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= totalThreads) {
        return;
    }

    uint32_t mask = lanemask_eq();
    lanemaskOutput[idx] = mask;
}
```

## 编译运行

### 环境准备
```bash
# 配置环境变量（根据实际安装路径选择）
source /usr/local/Ascend/cann/set_env.sh
```

### 编译执行
```bash
# 创建并进入 build 目录
mkdir -p build && cd build

# 编译（NPU 模式）
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j

# 执行测试
./demo
```

### 仿真模式编译
```bash
# NPU 仿真模式
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
```

## 预期输出

执行程序后，将输出每个线程的 lanemask_eq 验证结果：

```
output:0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000
golden:0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000
[Success] Lanemask_eq verification passed.
```

## 验证要点

1. **掩码唯一性**: 同一个 warp 内的 32 个线程应有不同的掩码值
2. **单比特置位**: 每个掩码值应只有一个比特位为1
3. **位置正确**: 第 N 个线程的掩码应在第 N 位为1（从右向左计数，从0开始）
4. **二进制表示**: 
   - 线程0: bit 0 = 1 (0x00000001)
   - 线程1: bit 1 = 1 (0x00000002)
   - ...
   - 线程31: bit 31 = 1 (0x80000000)

## 注意事项

1. **Warp 大小**: Ascend NPU 的 warp 大小固定为 32
2. **线程分配**: 线程按 threadIdx.x 顺序分配到不同的 warp
3. **边界检查**: 核函数中需检查线程索引是否超出总线程数
4. **返回类型**: lanemask_eq() 返回 uint32_t 类型

## 故障排查

如果测试失败，请检查：
1. 环境变量是否正确配置
2. CANN 版本是否符合要求
3. NPU 驱动是否正常加载
4. 每个线程的掩码值是否符合 `(1 << laneid)` 的预期