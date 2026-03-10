SOC_VERSION=Ascend910B1

# ========================== fixpipe_per_mmad ==============================
bash ./run.sh -r npu -v ${SOC_VERSION} -b Release -c ON -d mmad_fixpipe_per_mmad 
python3 scripts/run_tests.py ./cases/mmad_datasize.csv "aic_mac_time(us)"

# =========================== fixpipe_after_all_mmad ===============================
bash ./run.sh -r npu -v ${SOC_VERSION} -b Release -c ON -d mmad_fixpipe_after_all_mmad
python3 scripts/run_tests.py ./cases/mmad_datasize_v2.csv "aic_mac_time(us)"

# =========================== uniflag ==============================
bash ./run.sh -r npu -v ${SOC_VERSION} -b Release -c ON -d mmad_unitflag 
python3 scripts/run_tests.py ./cases/mmad_datasize_v3.csv "aic_mac_time(us)"

rm -rf PROF*