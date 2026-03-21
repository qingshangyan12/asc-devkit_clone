1、API 名称（如`Exp`, `Mul`, `Adds`）

用户填写：Reciprocal

2、函数原型（Ascend C API 签名，举例Add API的函数原型如下）
```cpp
template <typename T>
__aicore__ inline void Add(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
```

用户填写：
template <typename T>
__aicore__ inline void Reciprocal(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)


3、是单目API？双目API？双目标量API？

用户填写：单目API

4、支持的数据类型（half/float/int16/int32）

用户填写：half/float

5、功能说明（数学含义，如Exp:按元素取自然指数）

用户填写：按元素执行取倒数操作

6、理论性能（理论并行度，参考填写方式：{"half": 64, "float": 32, "int16": 64, "int32": 32}, 代表half类型输入理论并行度64）

用户填写：{"half": 128, "float": 64}