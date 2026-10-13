from pathlib import Path

import numpy as np


def verify_result():
    expected_size = 128 * np.dtype(np.uint16).itemsize
    for filename in ["output/output.bin", "output/golden.bin"]:
        if Path(filename).stat().st_size != expected_size:
            raise ValueError(f"{filename}: expected {expected_size} bytes")
    actual = np.fromfile("output/output.bin", dtype=np.uint16)
    expected = np.fromfile("output/golden.bin", dtype=np.uint16)
    mismatches = np.flatnonzero(actual != expected)
    if mismatches.size:
        print(f"FAIL: {mismatches.size}/128 mismatches; first index {mismatches[0]}")
        return False
    print("PASS: 128/128 elements match golden bit-for-bit")
    return True


if __name__ == "__main__":
    raise SystemExit(0 if verify_result() else 1)
