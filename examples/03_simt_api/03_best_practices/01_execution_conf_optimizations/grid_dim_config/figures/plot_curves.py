#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Regenerate Case 0/1/2/3 Task Duration curves for grid_config sample README.
Data = msprof Task Duration medians (5 runs each) measured on the grid-stride
(non-division) kernel with adaptive per-block thread count.

Usage:
    python3 figures/plot_curves.py
Outputs fig0_case0_empty_curve.png, fig1_case2_131072_curve.png,
fig2_case3_16384_curve.png, fig3_case1_large_curve.png into the figures/
directory next to this script.
"""
import os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib import font_manager

# --- CJK font so Chinese titles render (fallback to default if absent) ---
for _name in ("Noto Sans CJK JP", "Noto Sans CJK SC", "WenQuanYi Zen Hei"):
    if any(f.name == _name for f in font_manager.fontManager.ttflist):
        plt.rcParams["font.sans-serif"] = [_name]
        break
plt.rcParams["axes.unicode_minus"] = False

OUT = os.path.dirname(os.path.abspath(__file__))

# --- unified style ---
LINE_COLOR = "#2471a3"   # 折线与普通数据点统一蓝色
MIN_COLOR = "#27ae60"    # 最低点绿色
XLABEL = "gridDim"
YLABEL = "Task Duration (us)"


def _plot_curve(cores, dur, title, fname, mark_min=True):
    fig, ax = plt.subplots(figsize=(7, 4.2))
    xs = range(len(cores))
    # 折线 + 普通点（统一蓝色）
    ax.plot(xs, dur, "-o", color=LINE_COLOR, linewidth=2, markersize=7, zorder=3)
    # 最低点用绿色覆盖
    if mark_min:
        imin = dur.index(min(dur))
        ax.plot(imin, dur[imin], "o", color=MIN_COLOR, markersize=11, zorder=5,
                label="最低点")
        ax.legend(loc="upper right")
    # 数值标注
    for x, y in zip(xs, dur):
        ax.annotate(f"{y:.3f}", (x, y), textcoords="offset points",
                    xytext=(0, 8), ha="center", fontsize=9)
    ax.set_xticks(list(xs))
    ax.set_xticklabels([str(c) for c in cores])
    ax.set_xlabel(XLABEL)
    ax.set_ylabel(YLABEL)
    ax.set_title(title)
    ax.grid(True, linestyle="--", alpha=0.4)
    ax.margins(y=0.18)
    path = os.path.join(OUT, fname)
    fig.savefig(path, dpi=120, bbox_inches="tight")
    plt.close(fig)
    print("wrote", path)


if __name__ == "__main__":
    # Case 0: 空核调度基线 [1024,2048], cores 1/32/64/128/1024（无最低点高亮）
    _plot_curve([1, 32, 64, 128, 1024],
                [1.221, 1.557, 2.173, 3.141, 16.422],
                "Case 0 空核调度基线 Task Duration 随核数变化",
                "fig0_case0_empty_curve.png", mark_min=False)
    # Case 1: 大shape(2097152), cores 32/64/1024
    _plot_curve([32, 64, 1024],
                [110.540, 58.583, 78.289],
                "Case 1 大shape(2097152) Task Duration 随核数变化",
                "fig3_case1_large_curve.png")
    # Case 2: 中shape(131072), cores 16/32/64/128
    _plot_curve([16, 32, 64, 128],
                [17.644, 10.907, 8.629, 10.964],
                "Case 2 中shape(131072) Task Duration 随核数变化",
                "fig1_case2_131072_curve.png")
    # Case 3: 小shape(16384), cores 4/8/16/32/64
    _plot_curve([4, 8, 16, 32, 64],
                [10.831, 6.569, 4.500, 3.696, 4.133],
                "Case 3 小shape(16384) Task Duration 随核数变化",
                "fig2_case3_16384_curve.png")
