#!/usr/bin/env python3
"""Verify one progressive SIMD VF assert case log."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


CASE_MARKERS = {
    0: (
        "[ASSERT_TEST][CASE0][VF] stage=before value=7",
        "[ASSERT_TEST][CASE0][VF] stage=after_pass value=7",
    ),
    1: ("[ASSERT_TEST][CASE1][VF] stage=before value=7",),
    2: (
        "[ASSERT_TEST][CASE2][VF] stage=before value=7",
        "[ASSERT_TEST][CASE2][VF] stage=detail signed=-3 hex=207",
        "[ASSERT_TEST][CASE2][CUSTOM] value=7 expected=9",
    ),
    3: (
        "[ASSERT_TEST][CASE3][VF] stage=before value=7",
        "[ASSERT_TEST][CASE3][VF] stage=between_sites value=7",
        "[ASSERT_TEST][CASE3][SITE1_FAIL] value=7 limit=4",
    ),
    4: (
        "[ASSERT_TEST][CASE4][FIRST_VF] stage=before value=7",
        "[ASSERT_TEST][CASE4][FIRST_VF] stage=after_pass value=7",
        "[ASSERT_TEST][AICORE] case=4 stage=between_vfs",
        "[ASSERT_TEST][CASE4][SECOND_VF] stage=before value=7",
        "[ASSERT_TEST][CASE4][SECOND_VF] stage=before_fail value=7",
        "[ASSERT_TEST][CASE4][SECOND_FAIL] value=7 expected=8",
    ),
    5: ("[ASSERT_TEST][CASE5][FAIL] value=7 expected=10",),
    6: (
        "[ASSERT_TEST][CASE6][ROOT] stage=before value=7",
        "[ASSERT_TEST][CASE6][LEVEL1] value=7",
        "[ASSERT_TEST][CASE6][ROOT] stage=between_levels value=7",
        "[ASSERT_TEST][CASE6][LEVEL2] value=7",
        "[ASSERT_TEST][CASE6][LEVEL2_FAIL] value=7 expected=42",
    ),
    7: (
        "[ASSERT_TEST][CASE7][VF] stage=before value=7",
        "[ASSERT_TEST][CASE7][VF] stage=after_pass value=7",
    ),
    8: ("[ASSERT_TEST][CASE8][VF] stage=before value=7",),
    9: (
        "[ASSERT_TEST][CASE9][VF] stage=before value=7",
        "[ASSERT_TEST][CASE9][MIXED] signed=-3 unsigned=7 hex=907 float=1.250000 text=simd-vf",
    ),
    10: (
        "[ASSERT_TEST][CASE10][VF] invocation=0 stage=before value=7 expected=7",
        "[ASSERT_TEST][CASE10][VF] invocation=0 stage=after_pass",
        "[ASSERT_TEST][AICORE] case=10 stage=between_invocations",
        "[ASSERT_TEST][CASE10][VF] invocation=1 stage=before value=7 expected=8",
        "[ASSERT_TEST][CASE10][FAIL] invocation=1 value=7 expected=8",
    ),
    11: ("[ASSERT_TEST][CASE11][FAIL] record=8 limit=8",),
    12: (
        "[ASSERT_TEST][CASE12][FIRST_VF] value=7",
        "[ASSERT_TEST][AICORE] case=12 stage=between_vfs",
        "[ASSERT_TEST][CASE12][SECOND_VF] value=7",
    ),
    14: ("[ASSERT_TEST][DISABLED][VF] case=14 value=7",),
    15: (
        "[ASSERT_TEST][CASE15][VF] stage=before value=7",
        "[ASSERT_TEST][CASE15][VF] stage=after_pass value=7",
    ),
}

FAILURE_CASES = {1, 2, 3, 4, 5, 6, 8, 9, 10, 11}
DISABLED_OUTPUT_CASES = {13}
FORBIDDEN_MARKERS = {
    3: "[ASSERT_TEST][CASE3][SITE0_FAIL]",
    4: "[ASSERT_TEST][CASE4][FIRST_FAIL]",
    8: "[ASSERT_TEST][CASE8][VF] stage=forbidden_after_fail",
    9: "[ASSERT_TEST][CASE9][VF] stage=forbidden_after_fail",
    10: "[ASSERT_TEST][CASE10][VF] invocation=1 stage=after_pass",
    11: "[ASSERT_TEST][CASE11][VF] stage=forbidden_after_loop",
    12: "[ASSERT_TEST][CASE12][UNEXPECTED_FAIL]",
    14: "[ASSERT_TEST][DISABLED][FAIL]",
    15: "[ASSERT_TEST][CASE15][UNEXPECTED_FAIL]",
}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("case", type=int, choices=range(16))
    parser.add_argument("log", type=Path)
    args = parser.parse_args()

    text = args.log.read_text(encoding="utf-8", errors="replace")
    failures: list[str] = []

    def require(condition: bool, message: str) -> None:
        if not condition:
            failures.append(message)

    if args.case in DISABLED_OUTPUT_CASES:
        require("[ASSERT_TEST][AICORE]" not in text, "disabled debug output unexpectedly reached the log")
        require("[ASSERT_TEST][DISABLED]" not in text, "disabled SIMD VF output unexpectedly reached the log")
        require("Assertion " not in text, "disabled assertion unexpectedly trapped")
    else:
        require(f"[ASSERT_TEST][AICORE] case={args.case} stage=before" in text, "missing AICore start marker")
        for marker in CASE_MARKERS[args.case]:
            require(marker in text, f"missing marker {marker!r}")

    require(f"[ASSERT_TEST][HOST] case={args.case}" in text, "missing host status")
    require("result=PASS" in text, "host runtime status is not PASS")

    if args.case in FAILURE_CASES:
        require("expected=failure observed=failure" in text, "expected Vector Core failure was not observed")
        require("Assertion " in text, "assert failure text is missing")
    else:
        require("expected=success observed=success" in text, "case did not complete successfully")
        require("Assertion " not in text, "successful case unexpectedly emitted an assertion failure")
        if args.case not in DISABLED_OUTPUT_CASES:
            require(
                f"[ASSERT_TEST][AICORE] case={args.case} stage=after" in text,
                "successful case is missing the final AICore marker",
            )

    forbidden = FORBIDDEN_MARKERS.get(args.case)
    if forbidden is not None:
        require(forbidden not in text, f"forbidden marker was emitted: {forbidden!r}")

    if args.case == 5:
        records = [
            (int(record), int(value))
            for record, value in re.findall(
                r"\[ASSERT_TEST\]\[CASE5\]\[BURST\] record=(\d+) value=(\d+) payload=E{64}", text
            )
        ]
        require(len(records) == 36, f"case 5 expected 36 burst records, found {len(records)}")
        for index, (record, value) in enumerate(records):
            require(record == index, f"case 5 record[{index}] has index {record}")
            require(value == 700 + index, f"case 5 record[{index}] has value {value}")

    if args.case == 11:
        records = [
            (int(record), int(value))
            for record, value in re.findall(r"\[ASSERT_TEST\]\[CASE11\]\[LOOP\] record=(\d+) value=(\d+)", text)
        ]
        require(len(records) == 9, f"case 11 expected 9 loop records, found {len(records)}")
        for index, (record, value) in enumerate(records):
            require(record == index, f"case 11 record[{index}] has index {record}")
            require(value == 700 + index, f"case 11 record[{index}] has value {value}")

    if failures:
        print(f"ASSERT_CASE_{args.case} VERIFY FAIL")
        for failure in failures:
            print(f"- {failure}")
        return 1

    print(f"ASSERT_CASE_{args.case} VERIFY PASS log_bytes={len(text.encode('utf-8'))}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
