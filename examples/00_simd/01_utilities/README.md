# Utilities样例介绍
## 概述
基于Ascend C SIMT编程的简单样例，通过printf、clock等API介绍上板打印等调测方法，适用于调试阶段。

## 算子开发样例
|  目录名称                               |  功能描述                                |
| ----------------------------------- | -------------------------- |
| [00_printf](./00_printf) | 本样例通过Ascend C SIMT编程语言实现了gather算子，同时在算子中添加printf调测 |

| [02_dumptensor](./02_dumptensor) | 本样例通过Ascend C编程语言实现了Add算子和Mmad算子，在算子中添加DumpTensor调测，给出了对应的端到端实现 |
| [03_cpudebug](./03_cpudebug) | 本样例通过Ascend C编程语言实现了Add算子的CPU Debug调测，给出了对应的端到端实现 |
| [04_clock](./04_clock) | 本样例通过Ascend C编程语言实现了Gather算子，同时在算子中添加clock调测，给出了对应的端到端实现 |
| [05_msProf](./05_msProf) | 本样例基于Ascend C编程语言实现了MatmulLeakyRelu算子, 同时使用msProf工具调试，给出了对应的端到端实现 |
| [06_msSanitizer](./06_msSanitizer) | 本样例通过Ascend C编程语言实现了Add算子，同时展示开发算子过程中，msSanitizer工具可以检测到哪些异常场景提前拦截 |
