import struct
import subprocess
import sys
from pathlib import Path


def test_parse_data_bin_decodes_valid_words_and_skips_high_nibble_out_of_range(tmp_path):
    input_path = tmp_path / "data.bin"
    output_path = tmp_path / "parsed.csv"
    raw_output_path = tmp_path / "raw.csv"

    words = [
        0x0123ABCD,
        0x6FFF0001,
        0x70000002,
    ]
    input_path.write_bytes(b"".join(struct.pack("<I", word) for word in words))

    script = Path(__file__).with_name("parse_data_bin.py")
    result = subprocess.run(
        [
            sys.executable,
            str(script),
            str(input_path),
            "--output",
            str(output_path),
            "--raw-output",
            str(raw_output_path),
        ],
        text=True,
        capture_output=True,
        check=True,
    )

    assert "records=2" in result.stdout
    assert "skipped=1" in result.stdout
    assert output_path.read_text().splitlines() == [
        "index,offset,raw,high,marker,time",
        "0,0,0x0123abcd,0,291,43981",
        "1,4,0x6fff0001,6,4095,1",
    ]
    assert raw_output_path.read_text().splitlines() == [
        "index,offset,raw",
        "0,0,0x0123abcd",
        "1,4,0x6fff0001",
        "2,8,0x70000002",
    ]
