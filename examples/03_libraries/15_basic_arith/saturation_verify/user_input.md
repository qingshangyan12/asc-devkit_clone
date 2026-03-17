1、API 名称（如`Exp`, `Sqrt`, `Mul`）

用户填写：Rsqrt

2、函数原型（Ascend C API 签名，举例所下）
```cpp
template <typename T>
__aicore__ inline void Exp(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
```

用户填写：
template <typename T>
__aicore__ inline void Rsqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)


3、单目API还是双目API

用户填写：单目

4、支持的数据类型（half/float/int16/int32）

用户填写：half/float

5、功能说明（数学含义，如Exp：）

用户填写：按元素进行开方后取倒数的计算
