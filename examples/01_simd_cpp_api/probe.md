
bisheng -c -O2 -xcce test.cce -std=c++17 --cce-aicore-arch=dav-c220-vec -I$ASCEND_HOME_PATH/include -o test.o


bisheng test.o -L$ASCEND_HOME_PATH/lib64 -lascendcl -lruntime -o main

bisheng -w 