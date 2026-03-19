1、API 名称（如`Exp`, `Mul`, `Adds`）

用户填写：LeakyRelu

2、函数原型（Ascend C API 签名，举例所下）
```cpp
template <typename T>
__aicore__ inline void Exp(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
```

用户填写：
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyRelu(const LocalTensor<T>& dst, const LocalTensor<T>& src, const T& scalarValue, const int32_t& count)


3、是单目API？双目API？双目标量API？

用户填写：双目标量API

4、支持的数据类型（half/float/int16/int32）

用户填写：half/float

5、功能说明（数学含义，如Exp）

用户填写：按元素执行Leaky ReLU（Leaky Rectified Linear Unit）操作
