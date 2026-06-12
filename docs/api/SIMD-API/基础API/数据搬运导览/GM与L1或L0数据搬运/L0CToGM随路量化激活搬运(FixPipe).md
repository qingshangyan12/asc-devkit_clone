# L0CToGM随路量化激活搬运(FixPipe)<a name="ZH-CN_TOPIC_0000002543422576"></a>

该接口主要实现将数据从L0C Buffer搬运至Global Memory，并支持多种随路能力的组合，接口内包含了设置寄存器与数据搬运能力。

具体支持的数据通路为（以[逻辑位置TPosition](../../../其他数据类型/TPosition.md)表示）：

- L0C Buffer -> Global Memory
    - CO1 -> GM

搬运的数据为[矩阵计算](../../矩阵计算（ISASI）/矩阵计算（ISASI）.md)的结果，接口具体介绍请参考：[L0C到GM数据搬运（Fixpipe）](../../矩阵计算（ISASI）/矩阵计算的搬出/L0C到GM数据搬运（Fixpipe）.md)

src和dst分别为源操作数和目的操作数；intriParams为搬运参数，cbufWorkspace为开启tensor量化时所需的量化参数。

**传入FixpipeParamsArch3510参数的接口：**

- 不开启随路[tensor量化](../../矩阵计算（ISASI）/矩阵计算的搬出/关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

- 开启随路[tensor量化](../../矩阵计算（ISASI）/矩阵计算的搬出/关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<uint64_t>& cbufWorkspace, const FixpipeParamsArch3510<config.format>& intriParams)
    ```

**传入FixpipeParamsV220参数的接口：**

- 不开启随路[tensor量化](../../矩阵计算（ISASI）/矩阵计算的搬出/关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
    ```

- 开启随路[tensor量化](../../矩阵计算（ISASI）/矩阵计算的搬出/关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t, typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace, const FixpipeParamsV220& intriParams)
    ```

**传入FixpipeParamsM300参数的接口：**

- 不开启随路[tensor量化](../../矩阵计算（ISASI）/矩阵计算的搬出/关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM300& intriParams)
    ```

- 开启随路[tensor量化](../../矩阵计算（ISASI）/矩阵计算的搬出/关键特性说明/随路量化.md)功能：

    ```cpp
    template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t, typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
    __aicore__ inline void Fixpipe(const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace, const FixpipeParamsM300& intriParams)
    ```
