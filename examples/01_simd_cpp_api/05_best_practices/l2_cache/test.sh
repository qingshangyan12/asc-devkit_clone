  SCENARIO_NUM=1 ASC_ARCH=dav-2201
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=$ASC_ARCH ..
  make -j
  cd ..
  python3 scripts/gen_data.py -scenarioNum $SCENARIO_NUM
  cd build
  ./demo
msprof ./demo
msprof --ai-core=on --aic-metrics=L2Cache ./demo
