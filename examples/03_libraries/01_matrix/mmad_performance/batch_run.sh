#!/bin/bash

# ========================== fixpipe_per_mmad ==============================
rm -rf build
mkdir -p build
cd build
cmake .. -DDEMO_SOURCE=mmad_fixpipe_per_mmad.asc
make -j
cd ../
python3 scripts/run_tests.py ./cases/mmad_datasize.csv "aic_mac_time(us)"
rm -rf PROF*

# =========================== fixpipe_after_all_mmad ===============================
rm -rf build
mkdir -p build
cd build
cmake .. -DDEMO_SOURCE=mmad_fixpipe_after_all_mmad.asc
make -j
cd ../
python3 scripts/run_tests.py ./cases/mmad_datasize_v2.csv "aic_mac_time(us)"
rm -rf PROF*

# =========================== uniflag ==============================
rm -rf build
mkdir -p build
cd build
cmake .. -DDEMO_SOURCE=mmad_unitflag.asc
make -j
cd ../
python3 scripts/run_tests.py ./cases/mmad_datasize_v3.csv "aic_mac_time(us)"
rm -rf PROF*
