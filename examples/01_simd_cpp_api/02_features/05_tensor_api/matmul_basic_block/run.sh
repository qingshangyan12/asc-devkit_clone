mkdir -p build && cd build;  
cmake ..;make -j;             
python3 ../scripts/gen_data.py   
./demo ./input/input_x.bin ./input/input_y.bin ./output/output.bin                    
python3 ../scripts/verify_result.py output/output.bin output/golden.bin