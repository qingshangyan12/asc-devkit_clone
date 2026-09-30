from pathlib import Path
import sys

text = Path(sys.argv[1]).read_text(errors="replace")
expected = [
    "[simd_vf] string in gm!\n",
    "[simd_vf] int=10, uint=20, float=3.140000, string=hello",
    "[simd_vf] hex=ff, ptr=0x12ff",
    "[simd_vf] This is a simd_vf printf demo string.",
    "[simd_vf] add[0]: 1.250000 + 10.000000 = 11.250000",
    "[gm_long] GM_BEGIN:" + "G" * 2300 + ":GM_END",
]
missing = [i + 1 for i, value in enumerate(expected) if value not in text]
if missing:
    sys.exit(f"FAIL: missing or incorrect output cases {missing}; see {sys.argv[1]}")
print("PASS: direct literal + 5 address-only GM printf cases, including all 2300 payload characters")
