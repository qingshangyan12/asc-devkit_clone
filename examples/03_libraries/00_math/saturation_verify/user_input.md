1、API 名称（如`Exp`, `Mul`, `Adds`）

用户填写：Reciprocal

2、函数原型（Ascend C API 签名，举例所下）
```cpp
template <typename T>
__aicore__ inline void Exp(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
```

用户填写：
template <typename T>
__aicore__ inline void Reciprocal(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)


3、是单目API？双目API？双目标量API？

用户填写：单目

4、支持的数据类型（half/float/int16/int32）

用户填写：half/float

5、功能说明（数学含义，如Exp）

用户填写：将输入的tensor按元素取自然指数
