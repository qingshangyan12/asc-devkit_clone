# LaneID 和 LaneMask 功能融合验证示例

## 概述
本示例用于验证 SIMT API 中 `laneid()` 及 `lanemask_eq()`、`lanemask_le()`、`lanemask_lt()`、`lanemask_ge()`、`lanemask_gt()` 接口在 SIMD-SIMT 融合计算场景下的功能正确性。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- >= CANN 9.0.0-beta.2

## 接口说明

### laneid() 接口
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ int32_t laneid();
```
**功能**: 返回当前线程在 warp 中的 lane ID（0-31）。

### lanemask_eq() 接口
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_eq();
```
**功能**: 返回一个掩码，只有当前 lane ID 对应的位为 1，其他位为 0。

### lanemask_le() 接口
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_le();
```
**功能**: 返回一个掩码，lane ID 小于等于当前 lane ID 的位都为 1。

### lanemask_lt() 接口
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_lt();
```
**功能**: 返回一个掩码，lane ID 小于当前 lane ID 的位都为 1。

### lanemask_ge() 接口
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_ge();
```
**功能**: 返回一个掩码，lane ID 大于等于当前 lane ID 的位都为 1。

### lanemask_gt() 接口
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_gt();
```
**功能**: 返回一个掩码，lane ID 大于当前 lane ID 的位都为 1。

## 样例实现

### 文件说明
- `laneid_functions_fusion.asc`: 主程序文件，包含验证 laneid 和 lanemask 系列功能的完整代码

### 实现原理
1. **线程配置**: 启动 32 个线程（1 个 block，每个 block 32 个线程）
2. **融合测试**: 在 SIMD-SIMT 融合场景下测试各接口功能
3. **结果验证**: 
   - laneid: 预期 `threadIdx.x % 32`
   - lanemask_eq: 预期 `1 << (lane_id)`
   - lanemask_le: 预期 `(1 << (lane_id + 1)) - 1`
   - lanemask_lt: 预期 `(1 << lane_id) - 1`
   - lanemask_ge: 预期 `~((1 << lane_id) - 1)`
   - lanemask_gt: 预期 `~((1 << (lane_id + 1)) - 1)`

### 核心代码逻辑
```cpp
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void simt_laneid_test(
    __gm__ int32_t* laneidOutput)
{
    uint32_t idx = threadIdx.x;
    int32_t lane = laneid();
    laneidOutput[idx] = lane;
}

__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void simt_lanemask_eq_test(
    __gm__ uint32_t* lanemaskOutput)
{
    uint32_t idx = threadIdx.x;
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

执行程序后，将输出各接口的验证结果：

```
Testing laneid function...
Testing lanemask_eq function...
Testing lanemask_le function...
Testing lanemask_lt function...
Testing lanemask_ge function...
Testing lanemask_gt function...
[Success] All laneid functions fusion tests passed.
```

## 验证要点

1. **LaneID 范围**: 所有返回值必须在 0-31 范围内
2. **LaneID 唯一性**: 同一个 warp 内的 32 个线程应有不同的 laneid（0-31）
3. **LaneMask 正确性**: 各 lanemask 函数返回的掩码应符合预期
4. **融合场景**: 验证在 SIMD-SIMT 融合计算场景下各接口的正确性

## 注意事项

1. **Warp 大小**: Ascend NPU 的 warp 大小固定为 32
2. **线程分配**: 线程按 threadIdx.x 顺序分配到不同的 warp
3. **边界检查**: 核函数中需检查线程索引是否超出总线程数
4. **内存对齐**: 输出缓冲区大小需考虑内存对齐要求

## 故障排查

如果测试失败，请检查：
1. 环境变量是否正确配置
2. CANN 版本是否符合要求
3. NPU 驱动是否正常加载
4. 输出目录 `output/` 是否存在且有写权限