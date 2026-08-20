# AICPU 大量 printf 回绕样例

## 概述

本样例演示 AICPU 内核持续输出大量 `AscendC::printf`，当累计写入数据超过 ringbuf 容量（默认 32KB）时触发回绕（wrap-around），验证 split-write 的正确性。

## 原理

- 默认 ringbuf 大小为 32KB
- 每条 entry = `PrintfInfo 头(24字节) + 格式化字符串 + NUL`，8 字节对齐
- 本样例每条 printf 约 232 字节（200 字符填充 + 参数），32KB 可容约 140 条
- 当第 141 条 printf 写入时，剩余 tail 空间不足，触发 split-write：
  - 第一段写入 ringbuf 尾部
  - 第二段从 ringbuf 头部继续写入
- rts 读端持续消费数据，bufOffset 回绕到头部区域继续推进

## 回绕处理流程

```
ringbuf: [==已有数据==|tail空间|][头|==已被rts读走==]
                     ^wOff               ^rOff
写入entry时tail空间不够:
  1. malloc临时缓冲区(ringBufLen大小)
  2. 在临时缓冲区中组装完整entry(header + vsnprintf_s格式化字符串)
  3. 分段memcpy到ringbuf: 先写tail部分, 再写head部分
  4. printfType从WRITING设为READY(DUMP_AICPU)
```

## 样例内容

- `aicpu_wraparound.aicpu`：AICPU 内核，输出 500 条 printf，总数据约 116KB，远超 32KB
- `main.asc`：主机端入口
- `kernel_args.h`：参数结构体定义
- `CMakeLists.txt`：构建配置

## 参数说明

| 参数 | 默认值 | 说明 |
| ---- | ------ | --- |
| totalPrints | 500 | 总 printf 次数 |
| strLen | 200 | 每条 printf 填充字符串长度 |
| batchSize | 50 | 每 50 条打印一次 batch 标记 |

## 运行方式

```bash
mkdir build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201
make
./demo
```

## 预期输出

rts 端持续读取 ringbuf，输出 500 条 printf 及 batch 标记：

```
WraparoundPrintfKernel start, totalPrints=500 strLen=200
[0] ABCDEFGHIJ...
[1] ABCDEFGHIJ...
...
[49] ABCDEFGHIJ...
=== batch 0 done ===
[50] ABCDEFGHIJ...
...
=== batch 9 done ===
WraparoundPrintfKernel done, totalPrints=500
```

## 注意事项

- rts 读端需要持续消费数据，否则 ringbuf 写满后 `WaitForRts` 会等待最多 4.5s（15 × 300ms），超时后丢弃
- 临时缓冲区在首次回绕时 `malloc` 分配（ringBufLen 大小），全局复用，程序退出时析构释放
