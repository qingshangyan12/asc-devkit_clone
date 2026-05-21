# 接口内设置Mask

## Counter模式

- **前n个数据计算**

    采用Counter模式前n个数据计算，接口内部会先调用SetMaskCount，然后根据接口参数count调用SetVectorMask，再进行计算，计算完成后调用ResetMask、SetMaskNorm恢复系统默认Normal模式。

    调用示例：

    ```cpp
    uint64_t count = 128;
    half addsValue = 1.0;
    AscendC::Adds<half, true>(dstLocal, srcLocal, addsValue, count);
    ```

    **图 1**  Counter模式前n个数据计算示例  
    ![](../../../../../figures/counter_first_n_calc.png)<a id="图1-counter模式前n个数据计算示例"></a>

    完整样例参考[Mask样例](https://gitcode.com/cann/asc-devkit/pull/810)。

- **高维切分：连续计算**

    在Counter模式连续计算中，MASK寄存器中的值只有低64bit有效，被当做参与计算的元素数量。高维切分矢量计算接口的repeatTime参数会被忽略，由系统内部根据元素数量自动计算。

    调用示例：

    ```cpp
    half addsValue = 1.0;
    uint8_t repeatTimes = 1;
    AscendC::UnaryRepeatParams repeatParams{2, 2, 16, 16};
    AscendC::SetMaskCount();
    uint64_t mask[2] = {130, 0};
    AscendC::Adds<half, true>(dstLocal, srcLocal, addsValue, mask, repeatTimes, repeatParams);
    AscendC::SetMaskNorm();
    ```

    **图 2**  Counter模式高维切分连续计算示例  
    ![](../../../../../figures/counter_high_dim_continuous.png)<a id="图2-counter模式高维切分连续计算示例"></a>

## Normal模式

在Normal计算模式中，MASK寄存器会作为每个Repeat的掩码使用。只有高维切分支持此模式，在接口上区分为逐bit模式和连续模式。

**注：Normal 为默认模式。若未发生模式切换，则无需重复配置该模式。**

- **高维切分：逐bit计算**

    调用示例：

    ```cpp
    half addsValue = 1.0;
    int8_t repeatTimes = 1;
    UnaryRepeatParams repeatParams {2, 2, 16, 16};
    uint64_t mask[2] = {0x69A569A569A569A5, 0x69A569A569A569A5};
    AscendC::Adds<half, true>(dstLocal, srcLocal, addsValue, mask, repeatTimes, repeatParams);
    ```

    **图 3**  Normal模式高维切分逐bit计算示例  
    ![](../../../../../figures/normal_high_dim_bitwise.png)<a id="图3-normal模式高维切分逐bit计算示例"></a>

- **高维切分：连续计算**

    调用示例：

    ```cpp
    half addsValue = 1.0;
    int8_t repeatTimes = 1;
    UnaryRepeatParams repeatParams{2, 2, 16, 16};
    uint64_t mask = 128;
    AscendC::Adds<half, true>(dstLocal, srcLocal, addsValue, mask, repeatTimes, repeatParams);
    ```

    **图 4**  Normal模式高维切分连续计算示例  
    ![](../../../../../figures/normal_high_dim_continuous.png)<a id="图4-normal模式高维切分连续计算示例"></a>

> [!NOTE]说明
> 
> 当数据类型为half（操作数为16位）时，每次迭代内能够处理的元素个数mask∈\[1, 128\]。当 mask \> 128 时，仍按一次 repeat（128 元素）执行，不支持超过128元素的mask控制。
