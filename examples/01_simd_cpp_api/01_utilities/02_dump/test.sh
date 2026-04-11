mkdir -p build && cd build;
cmake .. -DASCENDC_TIME_STAMP_ON; make -j;
./demo
