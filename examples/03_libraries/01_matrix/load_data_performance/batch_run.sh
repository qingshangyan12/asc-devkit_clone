SOC_VERSION=Ascend910B1

# =========================== DataCopy ==============================
# rm -rf build
# mkdir -p build
# cd build
# cmake .. -DDEMO_SOURCE=data_copy.asc
# make -j
# cd ../
# python3 scripts/run_tests.py ./cases/data_copy/gm2l1.csv "aic_mte2_time(us)"
# rm -rf PROF*

# # =========================== load data ==============================
rm -rf build
mkdir -p build
cd build
cmake .. -DDEMO_SOURCE=load_data.asc
make -j
cd ../
python3 scripts/run_tests.py ./cases/gm2l1_cico.csv "aic_mte2_time(us)"
python3 scripts/run_tests.py ./cases/gm2l1_siso.csv "aic_mte2_time(us)"
python3 scripts/run_tests.py ./cases/gm2l0a.csv "aic_mte2_time(us)"
python3 scripts/run_tests.py ./cases/gm2l0b.csv "aic_mte2_time(us)"
python3 scripts/run_tests.py ./cases/l12l0a.csv "aic_mte1_time(us)"
python3 scripts/run_tests.py ./cases/l12l0a_transpose.csv "aic_mte1_time(us)"
python3 scripts/run_tests.py ./cases/l12l0b.csv "aic_mte1_time(us)"
python3 scripts/run_tests.py ./cases/l12l0b_transpose.csv "aic_mte1_time(us)"
rm -rf PROF*