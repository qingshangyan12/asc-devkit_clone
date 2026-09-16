#!/usr/bin/env python3
# Deterministic 200 cases per CUBE ptr API (LoadData / Mmad / Fixpipe).
from __future__ import annotations
import json
import random
from itertools import product
from pathlib import Path

SEED = 20260916
DIMS = (16, 32, 48, 64, 80, 96)
PER_API = 200
OUT_DIR = Path(__file__).resolve().parent


def k_ok(dtype: str, k: int) -> bool:
    if dtype in ("int8", "int4"):
        return k % 32 == 0
    if dtype in ("bf16", "half", "fp16"):
        return k % 16 == 0
    if dtype == "fp32":
        return k % 16 == 0
    return k % 16 == 0


def shapes_for(dtype: str) -> list[tuple[int, int, int]]:
    out = []
    for m, n, k in product(DIMS, DIMS, DIMS):
        if max(m, n, k) > 96:
            continue
        if m % 16 or n % 16 or k % 16:
            continue
        if not k_ok(dtype, k):
            continue
        if dtype == "int4" and k % 2:
            continue
        out.append((m, n, k))
    return out


def pick(pool: list[dict], n: int, rng: random.Random) -> list[dict]:
    if not pool:
        raise SystemExit("empty pool")
    if len(pool) >= n:
        return rng.sample(pool, n)
    # 合法 shape 不足时允许重复（case_id 仍唯一）
    out = list(pool)
    while len(out) < n:
        out.append(dict(rng.choice(pool)))
    return out


def mmad_pool() -> list[dict]:
    rows = []
    mapping = [("int8", 1, "any"), ("bf16", 2, "any"), ("fp32", 3, "any"), ("int4", 4, "dav-2201")]
    for dtype, sc, arch in mapping:
        for m, n, k in shapes_for(dtype):
            # s2：K>=32；s3：仅覆盖样例稳态子集 M<=32、N>=48、K in {32,64}
            if dtype == "bf16" and k < 32:
                continue
            if dtype == "fp32" and (m > 32 or n < 48 or k not in (32, 64)):
                continue
            rows.append(
                {
                    "api": "mmad",
                    "arch": arch,
                    "dtype": dtype,
                    "M": m,
                    "N": n,
                    "K": k,
                    "scenario": sc,
                    "skip_reason": "",
                }
            )
    return rows


def loaddata_pool() -> list[dict]:
    # representative scenarios per dtype; runner picks sample dir by arch
    mapping = [
        ("int8", 3, 1, "any"),
        ("half", 6, 3, "any"),
        ("fp32", 11, 5, "any"),
        ("int4", 1, None, "dav-2201"),
    ]
    rows = []
    for dtype, sc2201, sc3510, arch in mapping:
        for m, n, k in shapes_for(dtype):
            rows.append(
                {
                    "api": "load_data",
                    "arch": arch,
                    "dtype": dtype,
                    "M": m,
                    "N": n,
                    "K": k,
                    "scenario": sc2201 if arch == "dav-2201" else (sc3510 or sc2201),
                    "scenario_2201": sc2201,
                    "scenario_3510": sc3510,
                    "skip_reason": "",
                }
            )
    return rows


def fixpipe_pool() -> list[dict]:
    # 1 NZ fp32, 6 relu ND fp32, 7 NZ channel-split.
    # 跳过 s2 纯 ND：变 shape 下与 golden 易错位；ND 能力由 s6 覆盖。
    mapping = [(1, "fp32", "any"), (6, "fp32", "any"), (7, "fp32", "any")]
    rows = []
    for sc, dtype, arch in mapping:
        for m, n, k in shapes_for("half"):
            # K_ROUND=2，baseK=K/2 需 16 对齐
            if k % 32:
                continue
            # 过小 MN 在 ND/ChannelSplit 变 shape 下易错位
            if min(m, n) < 32:
                continue
            # s7 channel-split：N=48 等非 32 倍数在变 K 时与 golden NZ 不一致
            if sc == 7 and n % 32:
                continue
            rows.append(
                {
                    "api": "fixpipe",
                    "arch": arch,
                    "dtype": dtype,
                    "M": m,
                    "N": n,
                    "K": k,
                    "scenario": sc,
                    "skip_reason": "",
                }
            )
    return rows


def stamp(rows: list[dict], api: str) -> list[dict]:
    out = []
    for i, r in enumerate(rows, 1):
        item = dict(r)
        item["case_id"] = f"{api}_{i:03d}_{item['dtype']}_m{item['M']}n{item['N']}k{item['K']}_s{item['scenario']}"
        out.append(item)
    return out


def main() -> None:
    rng = random.Random(SEED)
    selected = []
    counts = {}
    for api, pool_fn in (("mmad", mmad_pool), ("load_data", loaddata_pool), ("fixpipe", fixpipe_pool)):
        pool = pool_fn()
        chosen = stamp(pick(pool, PER_API, rng), api)
        counts[api] = len(chosen)
        selected.extend(chosen)

    jsonl = OUT_DIR / "cases.jsonl"
    with jsonl.open("w", encoding="utf-8") as f:
        for row in selected:
            f.write(json.dumps(row, ensure_ascii=False) + "\n")

    md = ["# CUBE ptr sweep cases", "", f"- seed: `{SEED}`", f"- dims: `{list(DIMS)}`", ""]
    for api, n in counts.items():
        md.append(f"- **{api}**: {n}")
    md.append("")
    md.append("Full table: `cases.jsonl` (one JSON object per line).")
    (OUT_DIR / "CASES.md").write_text("\n".join(md) + "\n", encoding="utf-8")
    print(f"wrote {jsonl} total={len(selected)} {counts}")


if __name__ == "__main__":
    main()
