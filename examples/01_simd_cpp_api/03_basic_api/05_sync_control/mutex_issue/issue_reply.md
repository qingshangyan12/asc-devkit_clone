# Issue回复

感谢您的反馈，也很抱歉当前资料阐述不够清晰，给您带来了困扰。我们已在Ascend 950PR/Ascend 950DT（NPU架构3510）上关闭毕昇编译器自动同步，并对您提出的写法逐一进行了实机验证，验证结果如下：

- 问题1第一种写法可以正确执行。其中，使用相同MutexID完成PIPE\_V到PIPE\_MTE3的跨流水同步是正确的；但在NPU架构3510上，PIPE\_V之间的同步由硬件保证，因此两个Vector操作之间的`PipeBarrier<PIPE_V>`可以省略，不推荐保留该冗余指令。
- 问题1第二种写法也可以正确执行，但不推荐。将两个Vector操作拆分到两组使用相同MutexID的PIPE\_V锁中不会增强同步效果，中间的`PipeBarrier<PIPE_V>`在NPU架构3510上同样可以省略，因此该写法存在冗余。
- 问题2中的理解是正确的。在NPU架构3510上，连续Vector操作之间无需额外插入`PipeBarrier<PIPE_V>`。但PIPE\_V与PIPE\_MTE3之间仍属于跨流水依赖，因此仍需使用相同MutexID的`Lock`和`Unlock`进行同步。
- 问题3中的写法经验证可以正确执行，但拆分为两组PIPE\_V锁没有必要。由于后续PIPE\_MTE3依赖`Vector1`和`Vector2`均执行完成，推荐将两个Vector操作放在同一组PIPE\_V的`Lock`和`Unlock`之间，再使用相同MutexID锁定PIPE\_MTE3。

需要补充说明的是，相同MutexID、相同流水的第二次`Lock`不会阻塞该流水，不能用来替代单流水同步。对于NPU架构3510，PIPE\_V之间已经由硬件保证同步，因此无需通过重复加锁或插入`PipeBarrier<PIPE_V>`完成连续Vector操作之间的同步。

推荐写法如下：

```cpp
AscendC::Mutex::Lock<PIPE_V>(mutexId);
Vector1();
Vector2();
AscendC::Mutex::Unlock<PIPE_V>(mutexId);

AscendC::Mutex::Lock<PIPE_MTE3>(mutexId);
AscendC::DataCopyPad(...);
AscendC::Mutex::Unlock<PIPE_MTE3>(mutexId);
```

上述结论仅适用于Ascend 950PR/Ascend 950DT对应的NPU架构3510。我们已根据您的反馈补充Mutex文档中的连续Vector操作与MTE3同步说明，并新增完整验证样例。再次感谢您指出资料中的不足。
