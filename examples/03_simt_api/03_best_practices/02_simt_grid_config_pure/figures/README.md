# 图片资源说明

本目录保存README中引用的示意图源文件（`.drawio` 格式）以及可选的PNG导出。

## 文件清单

| 文件 | 用途 | README 引用位置 |
| ---- | ---- | --------------- |
| `fig1_logical_to_physical.drawio` | 逻辑核到物理核的调度映射示意 | `物理核与逻辑核` 小节 |
| `fig2_case5_vs_case6.drawio` | Case 5 vs Case 6 工作集与stride循环对比 | Case 6 分析后、`性能对比总结` 之前 |
| `fig3_case2_131072_curve.drawio` | Case 2 中shape(131072) Task Duration 随核数变化折线图 | Case 2 性能数据 |
| `fig4_case3_16384_curve.drawio` | Case 3 小shape(16384) Task Duration 随核数变化折线图 | Case 3 性能数据 |
| `fig5_case1_large_curve.drawio` | Case 1 大shape(2097152) Task Duration 随核数变化（32/64 strided + 1024 naive） | Case 1 性能数据 |

## 导出 PNG

`.drawio` 文件可以用以下任一方式导出为 PNG，命名为对应的 `<name>.png`：

1. 在 [draw.io 在线编辑器](https://app.diagrams.net/) 中打开 → File → Export As → PNG
2. 安装 draw.io 桌面版后：

   ```bash
   drawio -x -f png -o fig1_logical_to_physical.png fig1_logical_to_physical.drawio
   drawio -x -f png -o fig2_case5_vs_case6.png       fig2_case5_vs_case6.drawio
   drawio -x -f png -o fig3_case2_131072_curve.png   fig3_case2_131072_curve.drawio
   ```

3. 通过 VSCode 的 `Draw.io Integration` 插件直接保存为 PNG

PNG 导出后放在本目录下即可被 README 自动引用。
