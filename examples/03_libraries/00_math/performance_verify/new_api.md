# 新增 API 信息



**1. API 名称**

> 参考：`Exp`, `Mul`, `Adds`

请在下面填写：
```
Reciprocal
```



**2. 函数原型**

> 参考模板：
> ```cpp
> template <typename T>
> __aicore__ inline void Add(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
> ```

请在下面填写：
```
template <typename T>
__aicore__ inline void Reciprocal(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
```



**3. API 类型**

| 类型 | TYPE值 | 说明 |
||--||
| 单目API | 1 | `dst = Op(src)` |
| 双目API | 2 | `dst = Op(src0, src1)` |
| 双目标量API | 3 | `dst = Op(src, scalar)` |

请在下面填写：
```
单目API
```



**4. 支持的数据类型**

> 可选：half/float/int16/int32

请在下面填写：
```
half/float
```



**5. 功能说明**

> 参考：`Exp:按元素取自然指数`

请在下面填写：
```
按元素执行取倒数操作
```



**6. 理论性能**

> 格式：`{"half": 64, "float": 32}` 表示 half 类型理论并行度64

请在下面填写：
```
{"half": 128, "float": 64}
```
