# GM与UB切片数据搬运(DataCopy)<a name="ZH-CN_TOPIC_0000002568770166"></a>

该接口为软仿接口，从易用性角度出发进行设计，支持数据的切片搬运，提取多维Tensor数据的子集进行搬运。

具体支持的数据通路为（以[逻辑位置TPosition](../../../../其他数据类型/TPosition.md)表示）：

- Global Memory -> Unified Buffer
    - GM -> VECIN
- Unified Buffer -> Global Memory
    - VECOUT -> GM
    - <cann-filter npu-type="310p">CO2 -> GM（仅Atlas 推理系列产品AI Core支持）</cann-filter>

搬运的数据用于[矢量计算](../../Memory矢量计算/Memory矢量计算.md)，具体的接口请参考：[GM与UB切片数据搬运(DataCopy)](../../Memory矢量计算/数据搬运/GM与UB数据搬运/GM与UB切片数据搬运(DataCopy).md)。

src和dst分别为源操作数和目的操作数；dstSliceInfo和srcSliceInfo分别为目的和源操作数的切片信息，类型为SliceInfo；dimValue为操作数维度信息。

- Global Memory -> Unified Buffer

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue = 1)
    ```

- Unified Buffer -> Global Memory

    ```cpp
    template <typename T>
    __aicore__ inline void DataCopy(const GlobalTensor<T> &dst, const LocalTensor<T> &src, const SliceInfo dstSliceInfo[], const SliceInfo srcSliceInfo[], const uint32_t dimValue = 1)
    ```
