#!/bin/bash
rm -rf /home/jinhu/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include/c_api
rm -rf /home/jinhu/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/impl/c_api
cp -r /home/jinhu/myworks/asc-devkit/include/c_api /home/jinhu/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/include
cp -r /home/jinhu/myworks/asc-devkit/impl/c_api /home/jinhu/Ascend/cann-9.0.0-beta.2/x86_64-linux/asc/impl
