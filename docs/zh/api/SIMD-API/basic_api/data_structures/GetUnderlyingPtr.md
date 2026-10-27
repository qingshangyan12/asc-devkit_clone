# GetUnderlyingPtr

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90处理器系列产品：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030处理器系列产品：不支持
<!-- end id9 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_ptr_utils.h"`。

从 Tensor 或硬件指针取出底层地址。指针参数原样返回。参数是 [LocalTensor](LocalTensor/LocalTensor_intro.md) 时，返回其 [GetPhyAddr](LocalTensor/GetPhyAddr.md)()；参数是 [GlobalTensor](GlobalTensor/GlobalTensor_intro.md) 时，返回其 [GetPhyAddr](GlobalTensor/GetPhyAddr.md)()。

## 函数原型<a name="section620mcpsimp"></a>

- Tensor 与普通指针

    ```cpp
    template <typename U>
    __aicore__ inline auto GetUnderlyingPtr(const U& val)

    template <typename T>
    __aicore__ inline T* GetUnderlyingPtr(T* val)
    ```

- 地址空间指针

    ```cpp
    template <typename T>
    __aicore__ inline __ubuf__ T* GetUnderlyingPtr(__ubuf__ T* val)

    template <typename T>
    __aicore__ inline __cbuf__ T* GetUnderlyingPtr(__cbuf__ T* val)

    template <typename T>
    __aicore__ inline __cc__ T* GetUnderlyingPtr(__cc__ T* val)

    template <typename T>
    __aicore__ inline __fbuf__ T* GetUnderlyingPtr(__fbuf__ T* val)
    ```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| val | 输入 | 源操作数。可为硬件指针、[LocalTensor](LocalTensor/LocalTensor_intro.md) 或 [GlobalTensor](GlobalTensor/GlobalTensor_intro.md)。 |

## 返回值说明<a name="section640mcpsimp"></a>

- 指针参数：返回传入的指针。
- LocalTensor：返回 [GetPhyAddr](LocalTensor/GetPhyAddr.md)()。NPU 上该地址类型为 uint64_t，CPU 调试模式下为元素指针。
- GlobalTensor：返回 [GetPhyAddr](GlobalTensor/GetPhyAddr.md)()，类型为 `__gm__` 指针。

## 约束说明<a name="section633mcpsimp"></a>

- `__ubuf__`、`__cbuf__`、`__cc__`、`__fbuf__` 重载仅在非 `ASCENDC_CPU_DEBUG` 编译下提供。
- `__fbuf__` 重载在 Atlas 训练系列产品（dav-c100）和 Atlas 推理系列产品（dav-m200）上不提供。这些架构没有 Fixpipe Buffer，指针透传使用 `T*` 重载。
- `__gm__` 重载默认不提供，由 `T*` 重载覆盖。仅当构建定义 `ASCENDC_ADDRESS_SPACE_TYPES` 时启用。

## 调用示例<a name="section837496171220"></a>

```cpp
AscendC::LocalTensor<float> dstLocal;
AscendC::GlobalTensor<float> srcGlobal;
auto dstAddr = AscendC::GetUnderlyingPtr(dstLocal);
auto srcAddr = AscendC::GetUnderlyingPtr(srcGlobal);

__ubuf__ float* dstUb = nullptr;
__ubuf__ float* sameUb = AscendC::GetUnderlyingPtr(dstUb);
```
