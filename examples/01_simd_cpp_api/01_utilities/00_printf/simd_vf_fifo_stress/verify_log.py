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
HOST_ROUND_PATTERN = re.compile(
    r"\[FIFO_TEST\]\[HOST_ROUND\] round=(\d+) launch=(-?\d+) sync=(-?\d+) result=(PASS|FAIL)\n?"
)
HOST_PATTERN = re.compile(
    r"\[FIFO_TEST\]\[HOST\] rounds=(\d+) completed=(\d+) init=(-?\d+) set_device=(-?\d+) "
    r"create_stream=(-?\d+) launch=(-?\d+) sync=(-?\d+) destroy_stream=(-?\d+) reset_device=(-?\d+) "
    r"finalize=(-?\d+) result=(PASS|FAIL)\n?"
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


def split_host_records(text: str) -> tuple[str, list[re.Match[str]], list[re.Match[str]], int]:
    """Remove complete host records that may interrupt a device printf record."""
    host_rounds = list(HOST_ROUND_PATTERN.finditer(text))
    hosts = list(HOST_PATTERN.finditer(text))
    matches = sorted([*host_rounds, *hosts], key=lambda match: match.start())
    device_parts: list[str] = []
    cursor = 0
    interleaved = 0
    for match in matches:
        device_parts.append(text[cursor : match.start()])
        if match.start() > 0 and text[match.start() - 1] != "\n":
            interleaved += 1
        cursor = match.end()
    device_parts.append(text[cursor:])
    return "".join(device_parts), host_rounds, hosts, interleaved


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("log", type=Path)
    args = parser.parse_args()

    text = args.log.read_text(encoding="utf-8", errors="replace")
    failures: list[str] = []
    log_bytes = len(text.encode("utf-8"))
    require(log_bytes >= MIN_LOG_BYTES, f"log size {log_bytes} is below {MIN_LOG_BYTES}", failures)
    device_text, host_rounds, hosts, host_interleavings = split_host_records(text)

    require(
        text.count("[FIFO_TEST][HOST_ROUND]") == KERNEL_ROUNDS,
        f"HOST_ROUND: expected {KERNEL_ROUNDS} markers, found {text.count('[FIFO_TEST][HOST_ROUND]')}",
        failures,
    )
    require(
        len(host_rounds) == KERNEL_ROUNDS,
        f"HOST_ROUND: expected {KERNEL_ROUNDS} valid records, found {len(host_rounds)}",
        failures,
    )
    host_round_ids: set[int] = set()
    for match in host_rounds:
        round_id = int(match.group(1))
        require(round_id not in host_round_ids, f"HOST_ROUND: duplicate round={round_id}", failures)
        host_round_ids.add(round_id)
        require(0 <= round_id < KERNEL_ROUNDS, f"HOST_ROUND: invalid round={round_id}", failures)
        require(
            int(match.group(2)) == 0 and int(match.group(3)) == 0 and match.group(4) == "PASS",
            f"HOST_ROUND: round={round_id} launch={match.group(2)} sync={match.group(3)} result={match.group(4)}",
            failures,
        )
    require(
        host_round_ids == set(range(KERNEL_ROUNDS)),
        f"HOST_ROUND: rounds={sorted(host_round_ids)}",
        failures,
    )

    require(
        text.count("[FIFO_TEST][HOST]") == 1,
        f"HOST: expected 1 marker, found {text.count('[FIFO_TEST][HOST]')}",
        failures,
    )
    require(len(hosts) == 1, f"HOST: expected 1 valid record, found {len(hosts)}", failures)
    if len(hosts) == 1:
        host = hosts[0]
        status_values = [int(host.group(index)) for index in range(3, 11)]
        require(
            int(host.group(1)) == KERNEL_ROUNDS
            and int(host.group(2)) == KERNEL_ROUNDS
            and all(value == 0 for value in status_values)
            and host.group(11) == "PASS",
            f"HOST: invalid final status: {host.group(0).rstrip()}",
            failures,
        )

    for round_id in range(KERNEL_ROUNDS):
        for marker in (
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=start",
            f"[FIFO_TEST][AICORE_GLOBAL] round={round_id} stage=after_single",
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=between_a_b",
            f"[FIFO_TEST][AICORE_GLOBAL] round={round_id} stage=between_b_c",
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=before_repeat_a",
            f"[FIFO_TEST][AICORE_NS] round={round_id} stage=end",
        ):
            require_marker(device_text, marker, failures)
        start = re.search(
            rf"\[FIFO_TEST\]\[AICORE_NS\] round={round_id} stage=start "
            r"single=(\d+) multi=(\d+) float=([0-9.]+)",
            device_text,
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

    single_seen: set[tuple[int, int]] = set()
    for match in SINGLE_PATTERN.finditer(device_text):
        key = record_key(match.group(1), match.group(2), "VF_SINGLE", SINGLE_RECORDS, failures)
        add_unique(single_seen, key, "VF_SINGLE", failures)
        round_id, record = key
        require(int(match.group(3)) == record - 4096, f"VF_SINGLE{key}: signed={match.group(3)}", failures)
        expected_hex = round_id * 0x10000 + record + 0x100
        require(int(match.group(4), 16) == expected_hex, f"VF_SINGLE{key}: hex={match.group(4)}", failures)

    a_seen: set[tuple[int, int, int]] = set()
    for match in A_PATTERN.finditer(device_text):
        round_id, record = record_key(match.group(1), match.group(3), "VF_A", MULTI_RECORDS, failures)
        phase = int(match.group(2))
        key = (round_id, phase, record)
        require(phase in (1, 4), f"VF_A: invalid phase={phase}", failures)
        add_unique(a_seen, key, "VF_A", failures)
        expected_value = round_id * 1000000 + phase * 10000 + record
        require(int(match.group(4)) == expected_value, f"VF_A{key}: value={match.group(4)}", failures)

    b_seen: set[tuple[int, int]] = set()
    for match in B_PATTERN.finditer(device_text):
        key = record_key(match.group(1), match.group(2), "VF_B", MULTI_RECORDS, failures)
        add_unique(b_seen, key, "VF_B", failures)
        round_id, record = key
        require(int(match.group(3)) == record - 1024, f"VF_B{key}: signed={match.group(3)}", failures)
        expected_hex = round_id * 0x10000 + record + 0xB00
        require(int(match.group(4), 16) == expected_hex, f"VF_B{key}: hex={match.group(4)}", failures)

    c_seen: set[tuple[int, int]] = set()
    for match in C_PATTERN.finditer(device_text):
        key = record_key(match.group(1), match.group(2), "VF_C", MULTI_RECORDS, failures)
        add_unique(c_seen, key, "VF_C", failures)
        require(math.isclose(float(match.group(3)), 3.25, abs_tol=1e-6), f"VF_C{key}: float={match.group(3)}", failures)

    aicore_seen: set[tuple[int, int]] = set()
    for match in AICORE_PATTERN.finditer(device_text):
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
        f"log_bytes={log_bytes} host_interleavings={host_interleavings}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
