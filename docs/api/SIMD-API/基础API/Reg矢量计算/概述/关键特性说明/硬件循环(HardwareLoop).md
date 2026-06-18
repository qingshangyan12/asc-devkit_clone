# 硬件循环(HardwareLoop)<a name="ZH-CN_TOPIC_0000002081579973"></a>

Reg矢量计算架构中，Vector Function（VF）是实现高性能向量计算的核心载体。VF函数中可以包含最多四层嵌套循环，每层循环中还可以包含多个串行循环。VF循环对控制结构的支持有限，仅支持for循环和条件判断，不支持switch、do-while和 while-do等其他控制结构当VF函数中的循环满足Hardware Loop编码规范会被编译器优化为Hardware Loop，提升整体的编码性能，否则它的循环逻辑会由迭代变量和条件判断语句构成Software Loop，无法使能VF循环优化。

## Hardware Loop编码规范
- Hardware Loop最多能支持的嵌套层数为4层；支持串行的Hardware Loop循环。
- 迭代变量类型<br>VF内所有Loop的迭代变量必须是uint16_t 类型。
- 起始值与步长<br>循环起始值从0开始。<br>每次迭代的步长必须是递增1。
- 循环内不允许跳转指令，比如条件判断跳转，如if/else、三元运算符?:。<br>VF内if/else在Loop内会阻碍Hardware Loop的生成，编译器虽然会尽可能的做if/else消除优化，但是不做完全性保证。
- 一旦执行，循环计数/边界不允许被更改。
- 若要利用外层循环的计数作为循环边界，将外层循环计数器赋值给标量后作为内层循环边界。
