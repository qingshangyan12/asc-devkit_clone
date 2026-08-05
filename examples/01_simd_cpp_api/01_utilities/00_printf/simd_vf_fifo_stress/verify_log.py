#!/usr/bin/env python3
"""Verify every deterministic record emitted by simd_vf_fifo_stress."""

from __future__ import annotations

import argparse
import math
import re
from pathlib import Path


KERNEL_ROUNDS = 5
SINGLE_RECORDS = 8192
MULTI_RECORDS = 2048
AICORE_RECORDS = 24
MIN_LOG_BYTES = 10 * 1024 * 1024

SINGLE_PATTERN = re.compile(
    r"\[FIFO_TEST\]\[VF_SINGLE\] round=(\d+) record=(\d+) signed=(-?\d+) hex=([0-9a-fA-F]+) "
    r"payload=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-single-burst"
)
A_PATTERN = re.compile(
    r"\[FIFO_TEST\]\[VF_A\] round=(\d+) phase=(\d+) record=(\d+) value=(\d+) payload=A{64}"
)
B_PATTERN = re.compile(
    r"\[FIFO_TEST\]\[VF_B\] round=(\d+) record=(\d+) signed=(-?\d+) hex=([0-9a-fA-F]+) "
    r"payload=B{64}"
)
C_PATTERN = re.compile(
    r"\[FIFO_TEST\]\[VF_C\] round=(\d+) record=(\d+) float=(-?\d+(?:\.\d+)?) payload=C{64}"
)
AICORE_PATTERN = re.compile(
    r"\[FIFO_TEST\]\[AICORE_BULK\] round=(\d+) record=(\d+) signed=(-?\d+) hex=([0-9a-fA-F]+) "
    r"payload=D{64}"
)


def require(condition: bool, message: str, failures: list[str]) -> None:
    if not condition:
        failures.append(message)


def require_marker(text: str, marker: str, failures: list[str]) -> None:
    count = text.count(marker)
    require(count == 1, f"marker {marker!r}: expected once, found {count}", failures)


def record_key(round_text: str, record_text: str, category: str, limit: int, failures: list[str]) -> tuple[int, int]:
    round_id = int(round_text)
    record = int(record_text)
    require(0 <= round_id < KERNEL_ROUNDS, f"{category}: invalid round={round_id}", failures)
    require(0 <= record < limit, f"{category}: invalid record={record}", failures)
    return round_id, record


def add_unique(seen: set[tuple[int, ...]], key: tuple[int, ...], category: str, failures: list[str]) -> None:
    require(key not in seen, f"{category}: duplicate key={key}", failures)
    seen.add(key)


def require_complete(
    seen: set[tuple[int, ...]], expected: set[tuple[int, ...]], category: str, failures: list[str]
) -> None:
    missing = sorted(expected - seen)
    unexpected = sorted(seen - expected)
    require(not missing, f"{category}: missing {len(missing)} keys, first={missing[:8]}", failures)
    require(not unexpected, f"{category}: unexpected {len(unexpected)} keys, first={unexpected[:8]}", failures)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("log", type=Path)
    args = parser.parse_args()

    text = args.log.read_text(encoding="utf-8", errors="replace")
    failures: list[str] = []
    log_bytes = len(text.encode("utf-8"))
    require(log_bytes >= MIN_LOG_BYTES, f"log size {log_bytes} is below {MIN_LOG_BYTES}", failures)

    for round_id in range(KERNEL_ROUNDS):
        for marker in (
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=start",
            f"[FIFO_TEST][AICORE_GLOBAL] round={round_id} stage=after_single",
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=between_a_b",
            f"[FIFO_TEST][AICORE_GLOBAL] round={round_id} stage=between_b_c",
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=before_repeat_a",
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=end",
            f"[FIFO_TEST][HOST_ROUND] round={round_id}",
        ):
            require_marker(text, marker, failures)
        require(
            f"[FIFO_TEST][HOST_ROUND] round={round_id} launch=0 sync=0 result=PASS" in text,
            f"round {round_id}: host status is not PASS",
            failures,
        )
        start = re.search(
            rf"\[FIFO_TEST\]\[AICORE_NS\] round={round_id} stage=start "
            r"single=(\d+) multi=(\d+) float=([0-9.]+)",
            text,
        )
        require(start is not None, f"round {round_id}: AICore start values are missing", failures)
        if start is not None:
            require(int(start.group(1)) == SINGLE_RECORDS, f"round {round_id}: single={start.group(1)}", failures)
            require(int(start.group(2)) == MULTI_RECORDS, f"round {round_id}: multi={start.group(2)}", failures)
            require(
                math.isclose(float(start.group(3)), 1.5, abs_tol=1e-6),
                f"round {round_id}: float={start.group(3)}",
                failures,
            )

    require_marker(text, "[FIFO_TEST][HOST]", failures)
    require(
        f"[FIFO_TEST][HOST] rounds={KERNEL_ROUNDS} completed={KERNEL_ROUNDS}" in text and "result=PASS" in text,
        "final host runtime status is not PASS",
        failures,
    )

    single_seen: set[tuple[int, int]] = set()
    for match in SINGLE_PATTERN.finditer(text):
        key = record_key(match.group(1), match.group(2), "VF_SINGLE", SINGLE_RECORDS, failures)
        add_unique(single_seen, key, "VF_SINGLE", failures)
        round_id, record = key
        require(int(match.group(3)) == record - 4096, f"VF_SINGLE{key}: signed={match.group(3)}", failures)
        expected_hex = round_id * 0x10000 + record + 0x100
        require(int(match.group(4), 16) == expected_hex, f"VF_SINGLE{key}: hex={match.group(4)}", failures)

    a_seen: set[tuple[int, int, int]] = set()
    for match in A_PATTERN.finditer(text):
        round_id, record = record_key(match.group(1), match.group(3), "VF_A", MULTI_RECORDS, failures)
        phase = int(match.group(2))
        key = (round_id, phase, record)
        require(phase in (1, 4), f"VF_A: invalid phase={phase}", failures)
        add_unique(a_seen, key, "VF_A", failures)
        expected_value = round_id * 1000000 + phase * 10000 + record
        require(int(match.group(4)) == expected_value, f"VF_A{key}: value={match.group(4)}", failures)

    b_seen: set[tuple[int, int]] = set()
    for match in B_PATTERN.finditer(text):
        key = record_key(match.group(1), match.group(2), "VF_B", MULTI_RECORDS, failures)
        add_unique(b_seen, key, "VF_B", failures)
        round_id, record = key
        require(int(match.group(3)) == record - 1024, f"VF_B{key}: signed={match.group(3)}", failures)
        expected_hex = round_id * 0x10000 + record + 0xB00
        require(int(match.group(4), 16) == expected_hex, f"VF_B{key}: hex={match.group(4)}", failures)

    c_seen: set[tuple[int, int]] = set()
    for match in C_PATTERN.finditer(text):
        key = record_key(match.group(1), match.group(2), "VF_C", MULTI_RECORDS, failures)
        add_unique(c_seen, key, "VF_C", failures)
        require(math.isclose(float(match.group(3)), 3.25, abs_tol=1e-6), f"VF_C{key}: float={match.group(3)}", failures)

    aicore_seen: set[tuple[int, int]] = set()
    for match in AICORE_PATTERN.finditer(text):
        key = record_key(match.group(1), match.group(2), "AICORE_BULK", AICORE_RECORDS, failures)
        add_unique(aicore_seen, key, "AICORE_BULK", failures)
        round_id, record = key
        require(int(match.group(3)) == record - 12, f"AICORE_BULK{key}: signed={match.group(3)}", failures)
        expected_hex = round_id * 0x10000 + record + 0xA00
        require(int(match.group(4), 16) == expected_hex, f"AICORE_BULK{key}: hex={match.group(4)}", failures)

    rounds = range(KERNEL_ROUNDS)
    require_complete(single_seen, {(r, i) for r in rounds for i in range(SINGLE_RECORDS)}, "VF_SINGLE", failures)
    require_complete(
        a_seen,
        {(r, phase, i) for r in rounds for phase in (1, 4) for i in range(MULTI_RECORDS)},
        "VF_A",
        failures,
    )
    require_complete(b_seen, {(r, i) for r in rounds for i in range(MULTI_RECORDS)}, "VF_B", failures)
    require_complete(c_seen, {(r, i) for r in rounds for i in range(MULTI_RECORDS)}, "VF_C", failures)
    require_complete(aicore_seen, {(r, i) for r in rounds for i in range(AICORE_RECORDS)}, "AICORE_BULK", failures)

    if failures:
        print("FIFO_STRESS_VERIFY FAIL")
        for failure in failures[:80]:
            print(f"- {failure}")
        if len(failures) > 80:
            print(f"- ... {len(failures) - 80} additional failures")
        return 1

    records_per_round = SINGLE_RECORDS + 4 * MULTI_RECORDS + AICORE_RECORDS
    print(
        f"FIFO_STRESS_VERIFY PASS rounds={KERNEL_ROUNDS} records={KERNEL_ROUNDS * records_per_round} "
        f"log_bytes={log_bytes}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
