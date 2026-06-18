# 循环展开(unroll)<a name="ZH-CN_TOPIC_0000002081579973"></a>

如果循环内存在依赖关系过多的指令，指令无法并发执行，即无法使能双发特性。可以通过展开循环，提升指令双发能力，贴近硬件乱序执行的特性；减少指令因为寄存器资源未到位而产生的等待。当前支持以下两种展开方式：

| 展开方式 | 使能方式 | 优点 | 缺点 |
| :-- | :-- | :-- | :-- |
| 手动展开VF循环 | / | &bull;自主可控；<br>&bull;能处理复杂循环或src/dst寄存器复用等场景； | &bull;代码复杂度高；|
| 编译器自动展开 | #pragma unroll number （number表示展开层数）| &bull;代码复杂度低；| &bull;src/dst寄存器复用时无法展开； |
>[!NOTE]说明

> 循环展开通过消除指令依赖，提升指令双发能力来优化VF性能，需根据实际计算过程使用，不保证产生收益。展开时，若寄存器超出数量上限，可能会造成性能劣化或其他非预期行为。

## 手动展开VF循环
```cpp
for (uint16_t i = 0; i < 32; ++i) {  
// for循环间没有依赖关系：i=0,i=1可以并行执行，但是由于循环内数据依赖指令过多导致i=1的指令无法加载进执行队列中 
    AscendC::Reg::LoadAlign(srcReg, srcAddr + i * offset); 
    AscendC::Reg::Adds(dstReg0, srcReg, 10, mask); 
    AscendC::Reg::Muls(dstReg1 dstReg0, 20, mask); 
    ... // 超过64条有数据依赖的指令 
    AscendC::Reg::StoreAlign(dstAddr, dstReg1, i * offset, mask); 
}
```
展开后
```cpp
for (uint16_t i = 0; i < 8; ++i) { // 32做4展开 
    AscendC::Reg::LoadAlign(srcReg0, srcAddr + i * offset); 
    AscendC::Reg::LoadAlign(srcReg1, srcAddr + (i + 1) * offset); 
    AscendC::Reg::LoadAlign(srcReg2, srcAddr + (i + 2) * offset); 
    AscendC::Reg::LoadAlign(srcReg3, srcAddr + (i + 3) * offset); 
    AscendC::Reg::Adds(...) 
    AscendC::Reg::Adds(...) 
    AscendC::Reg::Adds(...) 
    AscendC::Reg::Adds(...) 
    AscendC::Reg::Muls(...) 
    AscendC::Reg::Muls(...) 
    AscendC::Reg::Muls(...) 
    AscendC::Reg::Muls(...) 
    ... 
    AscendC::Reg::StoreAlign(...) 
    AscendC::Reg::StoreAlign(...) 
    AscendC::Reg::StoreAlign(...) 
    AscendC::Reg::StoreAlign(...) 
}
```

## 编译器自动展开
```cpp
#pragma unroll 4 
for (uint16_t i = 0; i < 32; ++i) {  
    AscendC::Reg::LoadAlign(srcReg, srcAddr + i * offset); 
    AscendC::Reg::Adds(dstReg0, srcReg, 10, mask); 
    AscendC::Reg::Muls(dstReg1 dstReg0, 20, mask); 
    AscendC::Reg::StoreAlign(dstAddr, dstReg1, i * offset, mask); 
}
```
