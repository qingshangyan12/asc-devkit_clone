"""
test_public_dump.py — 引导链路看护入口（D1–D4）

用法:
    python3 test_public_dump.py scene1     # D1/D2：块表损坏注入 -> 异常回调 dump + per-binary 自动挂载
    python3 test_public_dump.py scene2     # 序列长度损坏注入（独立进程时为首异常）
    python3 test_public_dump.py baseline   # D4：无注入对照(回调不误触发)
    python3 test_public_dump.py all        # D3：单进程 scene1 -> scene2 -> baseline
                                           #     (per-binary 自动挂载后双回调双写；异常后同进程
                                           #      relaunch 可能主机侧失败，见 README 注意事项 3)

退出码: 注入场景 kernel 异常被捕获后以 2 结束(预期成功路径); baseline 正常结束为 0。
判定以日志与 dump 产物为准，勿按退出码判失败。
产物落盘: <本脚本目录>/task_fail_dump/dev{d}_task{t}_ts{ms}/（脚本自动切换 CWD 至脚本目录）。
"""
import glob
import os
import sys

# model 与 Python API 层位于 ../python/
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "python"))
# dump 产物按 CWD 落盘，统一切换到脚本目录，保证任意位置执行产物路径一致
os.chdir(os.path.dirname(os.path.abspath(__file__)))

from ifa_exception_dump import run  # noqa: E402

SCENE = sys.argv[1] if len(sys.argv) > 1 else "all"
SCENES = ["scene1", "scene2", "baseline"] if SCENE == "all" else [SCENE]

dirs_before = set(glob.glob("task_fail_dump/dev*"))
print("[INFO] 运行前已有 dump 目录: %d 个" % len(dirs_before))

expected_fail = run(SCENES)

new_dirs = sorted(set(glob.glob("task_fail_dump/dev*")) - dirs_before)
print("[RESULT] dump 目录存在: %s" % os.path.isdir("task_fail_dump"))
print("[RESULT] 本轮新增 dump 目录数: %d" % len(new_dirs))
for d in new_dirs:
    files = os.listdir(d)
    bins = [f for f in files if f.endswith(".bin")]
    print("  %s: %d 个文件 (%s ...)" % (d, len(files), bins[0] if bins else "-"))
# 注入场景 kernel 异常被捕获属预期成功路径，以退出码 2 标记；baseline 正常结束退出 0
sys.exit(2 if expected_fail else 0)
