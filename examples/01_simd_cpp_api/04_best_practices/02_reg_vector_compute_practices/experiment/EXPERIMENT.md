## 样例描述

**样例功能**：

Gelu近似公式计算为：

$$
GELU(x) \approx 0.5 \cdot x \cdot \left(1 + \tanh\left(\sqrt{\frac{2}{\pi}} \cdot \left(x + 0.044715 \cdot x^3\right)\right)\right) \tag{1}
$$

tanh的计算公式为：

$$
\tanh(u) = \frac{e^{2u} - 1}{e^{2u} + 1} \tag{2}
$$

其中，$u = \sqrt{\frac{2}{\pi}} \cdot (x + 0.044715 \cdot x^3)$。

将tanh公式代入Gelu公式并化简，得到：

$$
GELU(x) \approx \frac{x}{1 + e^{-2 \cdot \sqrt{\frac{2}{\pi}} \cdot (x + 0.044715 \cdot x^3)}} \tag{3}
$$

其中，$-2 \cdot \sqrt{\frac{2}{\pi}} \approx -1.595769$。

本样例以公式（3）进行编程计算，用户在设计向量算子时，需要考虑对原始计算进行化简，可以有效减少计算步骤和内存占用。


## 开发者体验

推荐开发者尝试基于Gelu示例编写以下单目数学计算公式并完成验证：

**atanh**

$$
\operatorname{atanh}(x) = 0.5\ln\left(\frac{1+x}{1-x}\right)
$$

- **推荐使用Reg矢量计算接口**：Add/Sub/Div/Ln/Muls等


**sign**

$$
\operatorname{sign}(x)=
\begin{cases}
1, & x > 0 \\
0, & x = 0 \\
-1, & x < 0
\end{cases}
$$

- **推荐使用Reg矢量计算接口**：Duplicate/Compares/Select等
