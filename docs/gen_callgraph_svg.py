#!/usr/bin/env python3
"""
Generate an SVG architecture/call-graph diagram for
HcommImpl<COMM_PROTOCOL_UBC_CTP> AIV URMA communication.
Pure Python, no third-party dependencies.
"""

import os

OUTPUT_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                           "hcomm_aiv_urma_callgraph.svg")

# ──────────────────────────────────────────────────────────────────────
# Layout constants
# ──────────────────────────────────────────────────────────────────────
W = 1200       # canvas width
H = 980        # canvas height
BOX_R = 8      # rounded rect radius
FONT = "Consolas, 'Source Code Pro', monospace"
TITLE_FONT = "Arial, 'Noto Sans SC', sans-serif"

# Colors
C_BG = "#f8fafc"
C_USER_LAYER = "#e0e7ff"      # indigo-100
C_IMPL_LAYER = "#fef3c7"      # amber-100
C_HW_LAYER = "#dcfce7"        # green-100
C_BOX_USER = "#4f46e5"        # indigo-600
C_BOX_IMPL = "#d97706"        # amber-600
C_BOX_HW = "#16a34a"          # green-600
C_ARROW = "#475569"           # slate-600
C_TEXT = "#1e293b"            # slate-800
C_SUBTITLE = "#64748b"       # slate-500


def rect(x, y, w, h, fill, stroke, stroke_w=1.5, rx=BOX_R, opacity=1.0):
    return (f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{rx}" '
            f'fill="{fill}" stroke="{stroke}" stroke-width="{stroke_w}" '
            f'opacity="{opacity}"/>')


def text(x, y, content, size=13, color=C_TEXT, anchor="middle", bold=False, font=FONT):
    weight = 'font-weight="bold"' if bold else ''
    return (f'<text x="{x}" y="{y}" font-family="{font}" font-size="{size}" '
            f'fill="{color}" text-anchor="{anchor}" {weight}>{content}</text>')


def arrow(x1, y1, x2, y2, color=C_ARROW, dashed=False):
    dash = 'stroke-dasharray="6,3"' if dashed else ''
    return (f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" '
            f'stroke="{color}" stroke-width="1.8" marker-end="url(#arrowhead)" {dash}/>')


def arrow_path(d, color=C_ARROW, dashed=False):
    dash = 'stroke-dasharray="6,3"' if dashed else ''
    return (f'<path d="{d}" fill="none" stroke="{color}" stroke-width="1.8" '
            f'marker-end="url(#arrowhead)" {dash}/>')


def build_svg():
    parts = []
    parts.append(f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {H}" '
                 f'width="{W}" height="{H}">')
    # Defs
    parts.append('<defs>')
    parts.append('<marker id="arrowhead" markerWidth="10" markerHeight="7" '
                 'refX="9" refY="3.5" orient="auto">')
    parts.append(f'  <polygon points="0 0, 10 3.5, 0 7" fill="{C_ARROW}"/>')
    parts.append('</marker>')
    # Drop shadow filter
    parts.append('<filter id="shadow" x="-4%" y="-4%" width="108%" height="108%">')
    parts.append('  <feDropShadow dx="2" dy="2" stdDeviation="3" flood-opacity="0.10"/>')
    parts.append('</filter>')
    parts.append('</defs>')

    # Background
    parts.append(f'<rect width="{W}" height="{H}" fill="{C_BG}"/>')

    # ─── Title ───
    parts.append(text(W // 2, 35, "AIV直驱URMA代码逻辑图",
                      size=18, bold=True, font=TITLE_FONT))

    # ═══════════════════════════════════════════════════════════════════
    # Layer 1: 用户调用层  (y: 55 ~ 160)
    # ═══════════════════════════════════════════════════════════════════
    ly1_y = 55
    ly1_h = 110
    parts.append(rect(30, ly1_y, W - 60, ly1_h, C_USER_LAYER, C_BOX_USER, rx=12))
    parts.append(text(90, ly1_y + 20, "用户调用层", size=14, bold=True, color=C_BOX_USER, font=TITLE_FONT))

    # User-facing API boxes
    api_names = ["Init", "WriteNbi", "ReadNbi", "Commit", "Drain"]
    api_x_start = 100
    api_spacing = 210
    api_box_w = 150
    api_box_h = 50
    api_y = ly1_y + 40
    api_centers = []
    for i, name in enumerate(api_names):
        x = api_x_start + i * api_spacing
        parts.append(rect(x, api_y, api_box_w, api_box_h, "#ffffff", C_BOX_USER, stroke_w=2))
        parts.append(text(x + api_box_w // 2, api_y + 30, name, size=15, bold=True, color=C_BOX_USER))
        api_centers.append((x + api_box_w // 2, api_y + api_box_h))

    # ═══════════════════════════════════════════════════════════════════
    # Layer 2: 内部实现层  (y: 185 ~ 800)
    # ═══════════════════════════════════════════════════════════════════
    ly2_y = 185
    ly2_h = 600
    parts.append(rect(30, ly2_y, W - 60, ly2_h, C_IMPL_LAYER, C_BOX_IMPL, rx=12))
    parts.append(text(90, ly2_y + 20, "内部实现层", size=14, bold=True, color=C_BOX_IMPL, font=TITLE_FONT))

    # ─── Init internals (left column) ───
    init_x = 80
    init_bx = init_x
    init_by = ly2_y + 50
    init_bw = 160
    init_bh = 120
    parts.append(rect(init_bx, init_by, init_bw, init_bh, "#fff7ed", C_BOX_IMPL))
    lines = ["分配UB工作区", "├ wqeItem(128B)", "├ cqeItem(64B)", "├ sqPI(32B)", "├ sqCI(32B)", "└ cqCI(32B)"]
    for j, ln in enumerate(lines):
        parts.append(text(init_bx + init_bw // 2, init_by + 20 + j * 16, ln, size=11, color=C_TEXT))
    # Arrow from Init to init box
    parts.append(arrow(api_centers[0][0], api_centers[0][1], init_bx + init_bw // 2, init_by - 2))

    # ─── PostSend column (center) ───
    ps_x = 310
    ps_y = ly2_y + 40
    ps_w = 320
    ps_h = 540
    parts.append(rect(ps_x, ps_y, ps_w, ps_h, "#fffbeb", C_BOX_IMPL, stroke_w=1))
    parts.append(text(ps_x + ps_w // 2, ps_y + 20, "PostSend&lt;OpCode&gt;", size=14, bold=True, color=C_BOX_IMPL))

    steps = [
        "1. HcommFindBufferIdx",
        "   (查找远端buffer索引)",
        "2. sqPI",
        "   (读取SQ头指针)",
        "3. PollCqWhenSqOverflow",
        "   (SQ满时自动poll cq)",
        "4. HcommUrmaFillSqeCtx",
        "   (填充SQE结构)",
        "5. HcommUrmaFillSgeCtx",
        "   (填充SGE结构)",
        "6. 更新WQE内容",
        "   (WQE刷新到对应gm地址)",
        "7. 更新sqPI",
        "   (sqPI回写GM)",
        "8. if(commit) → Commit",
    ]
    for j, s in enumerate(steps):
        parts.append(text(ps_x + ps_w // 2, ps_y + 50 + j * 33, s, size=12, color=C_TEXT, anchor="middle"))

    # Arrows from WriteNbi / ReadNbi to PostSend
    parts.append(arrow(api_centers[1][0], api_centers[1][1], ps_x + ps_w // 2 - 40, ps_y - 2))
    parts.append(arrow(api_centers[2][0], api_centers[2][1], ps_x + ps_w // 2 + 40, ps_y - 2))

    # ─── Commit box (right-upper) ───
    cm_x = 720
    cm_y = ly2_y + 50
    cm_w = 200
    cm_h = 100
    parts.append(rect(cm_x, cm_y, cm_w, cm_h, "#ecfdf5", C_BOX_HW))
    cm_lines = ["读取sqPI", "st_dev(doorbell)", "(通知硬件处理SQ)"]
    for j, ln in enumerate(cm_lines):
        parts.append(text(cm_x + cm_w // 2, cm_y + 30 + j * 24, ln, size=12, color=C_TEXT))
    # Arrow from Commit API
    parts.append(arrow(api_centers[3][0], api_centers[3][1], cm_x + cm_w // 2, cm_y - 2))
    # Dashed arrow from PostSend step 8 to Commit box
    parts.append(arrow_path(
        f"M {ps_x + ps_w} {ps_y + 50 + 14 * 33 - 10} L {cm_x} {cm_y + cm_h // 2}",
        dashed=True))

    # ─── Drain / PollCq box (right-lower) ───
    dr_x = 720
    dr_y = ly2_y + 240
    dr_w = 200
    dr_h = 240
    parts.append(rect(dr_x, dr_y, dr_w, dr_h, "#f0fdf4", C_BOX_HW))
    parts.append(text(dr_x + dr_w // 2, dr_y + 24, "PollCq", size=14, bold=True, color=C_BOX_HW))
    dr_steps = [
        "1. 读取cqCI",
        "2. 循环等待CQE",
        "3. 检查status",
        "4. 更新cqCI",
        "5. Ring CQ doorbell",
        "6. 更新sqCI",
    ]
    for j, s in enumerate(dr_steps):
        parts.append(text(dr_x + dr_w // 2, dr_y + 55 + j * 32, s, size=12, color=C_TEXT))
    # Arrow from Drain API
    parts.append(arrow(api_centers[4][0], api_centers[4][1],
                       dr_x + dr_w // 2, dr_y - 2))

    # ═══════════════════════════════════════════════════════════════════
    # Layer 3: 硬件层  (y: 810 ~ 950)
    # ═══════════════════════════════════════════════════════════════════
    ly3_y = 810
    ly3_h = 140
    parts.append(rect(30, ly3_y, W - 60, ly3_h, C_HW_LAYER, C_BOX_HW, rx=12))
    parts.append(text(90, ly3_y + 20, "硬件层 (URMA Engine)", size=14, bold=True, color=C_BOX_HW, font=TITLE_FONT))

    # SQ box
    sq_x = 150
    sq_y = ly3_y + 40
    sq_w = 220
    sq_h = 60
    parts.append(rect(sq_x, sq_y, sq_w, sq_h, "#ffffff", C_BOX_HW, stroke_w=2))
    parts.append(text(sq_x + sq_w // 2, sq_y + 35, "SQ Ring Buffer", size=14, bold=True, color=C_BOX_HW))

    # HW processing
    hw_x = 470
    hw_y = ly3_y + 40
    hw_w = 200
    hw_h = 60
    parts.append(rect(hw_x, hw_y, hw_w, hw_h, "#ffffff", C_BOX_HW, stroke_w=2))
    parts.append(text(hw_x + hw_w // 2, hw_y + 35, "URMA HW Engine", size=14, bold=True, color=C_BOX_HW))

    # CQ box
    cq_x = 770
    cq_y = ly3_y + 40
    cq_w = 220
    cq_h = 60
    parts.append(rect(cq_x, cq_y, cq_w, cq_h, "#ffffff", C_BOX_HW, stroke_w=2))
    parts.append(text(cq_x + cq_w // 2, cq_y + 35, "CQ Ring Buffer", size=14, bold=True, color=C_BOX_HW))

    # Arrows between HW components
    parts.append(arrow(sq_x + sq_w, sq_y + sq_h // 2, hw_x, hw_y + hw_h // 2))
    parts.append(text((sq_x + sq_w + hw_x) // 2, sq_y + sq_h // 2 - 10, "doorbell触发",
                      size=10, color=C_SUBTITLE))
    parts.append(arrow(hw_x + hw_w, hw_y + hw_h // 2, cq_x, cq_y + cq_h // 2))
    parts.append(text((hw_x + hw_w + cq_x) // 2, hw_y + hw_h // 2 - 10, "完成通知",
                      size=10, color=C_SUBTITLE))

    # Arrow from PostSend DataCopy to SQ
    parts.append(arrow_path(
        f"M {ps_x + ps_w // 2} {ps_y + ps_h} L {sq_x + sq_w // 2} {sq_y}",
        color=C_BOX_IMPL, dashed=True))
    parts.append(text(ps_x + ps_w // 2 - 60, ps_y + ps_h + 15, "WQE刷新→SQ(GM)",
                      size=10, color=C_SUBTITLE, anchor="start"))

    # Arrow from CQ to PollCq
    parts.append(arrow_path(
        f"M {cq_x + cq_w // 2} {cq_y} L {dr_x + dr_w // 2} {dr_y + dr_h}",
        color=C_BOX_HW, dashed=True))

    # Legend
    lg_y = H - 25
    parts.append(text(50, lg_y, "━━ 同步调用    ┈┈ 异步/条件调用", size=11, color=C_SUBTITLE, anchor="start"))

    parts.append('</svg>')
    return '\n'.join(parts)


if __name__ == "__main__":
    svg_content = build_svg()
    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        f.write(svg_content)
    print(f"SVG diagram generated: {OUTPUT_PATH}")
