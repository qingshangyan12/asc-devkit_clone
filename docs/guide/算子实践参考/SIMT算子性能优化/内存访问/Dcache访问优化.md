# Dcache访问优化

【优先级】中

【描述】SIMT编程模式下，算子访问Global Memory数据时会经过DCache。当算子中存在多种不同访问模式的数据（如仅遍历一次的流式数据与需要反复访问的热点数据）时，若不加区分地使用普通加载/存储指令，所有数据会共同竞争DCache空间，导致热点数据被流式数据挤出DCache，后续重新访问热点数据时需要从GM重新加载，增加访存延迟。此时可以通过数据缓存提示函数，为不同类型的数据指定不同的缓存策略，使热点数据优先驻留DCache，减少不必要的GM访问。

SIMT编程模式下提供以下数据缓存提示函数：

- **`asc_ldcg`**：加载数据时提示硬件该数据可能不需要优先占用Cache空间，适用于流式访问的数据（如仅遍历一次的输入数据），减少其对DCache空间的占用，避免将热点数据挤出DCache。
- **`asc_ldca`**：加载数据时提示硬件优先为该数据分配Cache空间，适用于需要频繁访问的热点数据（如查找表），确保热点数据常驻DCache，减少从Global Memory重新加载的次数。
- **`asc_stcg`**：存储数据时提示硬件优化写入后的Cache分配策略，适用于写入后不需要立即驻留DCache的数据（如输出数据），避免输出数据占用DCache空间影响热点数据的缓存。

- 使用数据缓存提示函数优化查表计算

  【样例介绍】以sin查表算子为例，使用查表法计算sin值，通过线性插值提高精度。输入数据长度为65536（256KB），sin查找表长度为8192（32KB）。算法实现中，每个线程根据输入值计算查找表索引，从sin表中读取相邻两个表项，通过线性插值得到结果。其中输入数据每个元素仅访问一次，属于非热点数据；sin查找表会被反复访问，属于热点数据。

  【反例】所有数据使用普通加载/存储指令，输入、输出和sin表数据共同竞争DCache空间。

  ```
  __global__ void sin_table_lookup_baseline(float* input, float* sin_table, float* output,
                                              uint32_t input_length, uint32_t table_length)
  {
      for (int idx = threadIdx.x; idx < input_length; idx += blockDim.x) {
          float x = input[idx];
          float sign = 1.0f;
          if (x < 0.0f) {
              sign = -1.0f;
              x = -x;
          }
          float two_pi = 2.0f * PI;
          x = fmodf(x, two_pi);
          if (x >= PI) {
              x = two_pi - x;
              sign = -sign;
          }

          float index_float = x * static_cast<float>(table_length) / PI;
          uint32_t n = static_cast<uint32_t>(floorf(index_float));
          float frac = index_float - static_cast<float>(n);

          float low_val = sin_table[n];
          float high_val = 0.0f;
          if (n + 1 >= table_length) {
              high_val = sin_table[0];
          } else {
              high_val = sin_table[n + 1];
          }
          output[idx] = sign * (low_val + frac * (high_val - low_val));
      }
  }
  ```

  上述实现中，`input[idx]`、`sin_table[n]`、`sin_table[n+1]`和 `output[idx]`均使用普通加载/存储指令访问Global Memory。输入数据虽然仅访问一次，但其加载时会占用DCache空间；输出数据写入后也可能驻留DCache。当输入和输出数据大量加载时，sin查找表的数据会被挤出DCache，导致后续线程查表时需要重新从Global Memory加载，增加DCache Read GM次数。

  反例算子的性能数据如下：

  | Task Duration(us) | DCache Read GM | DCache Read Vector | DCache Write Vector |
  | :---------------: | :------------: | :-----------------: | :------------------: |
  |      56.82        |      5064      |        2048         |        6144          |

  【正例】使用数据缓存提示函数区分不同数据的缓存策略。

  ```
  __global__ void sin_table_lookup_optimized(float* input, float* sin_table, float* output,
                                               uint32_t input_length, uint32_t table_length)
  {
      for (int idx = threadIdx.x; idx < input_length; idx += blockDim.x) {
          float x = asc_ldcg(&input[idx]);
          float sign = 1.0f;
          if (x < 0.0f) {
              sign = -1.0f;
              x = -x;
          }
          float two_pi = 2.0f * PI;
          x = fmodf(x, two_pi);
          if (x >= PI) {
              x = two_pi - x;
              sign = -sign;
          }

          float index_float = x * static_cast<float>(table_length) / PI;
          uint32_t n = static_cast<uint32_t>(floorf(index_float));
          float frac = index_float - static_cast<float>(n);

          float low_val = asc_ldca(&sin_table[n]);
          float high_val = 0.0f;
          if (n + 1 >= table_length) {
              high_val = asc_ldca(&sin_table[0]);
          } else {
              high_val = asc_ldca(&sin_table[n + 1]);
          }
          float y = sign * (low_val + frac * (high_val - low_val));
          asc_stcg(&output[idx], y);
      }
  }
  ```

  上述实现中：
  - 输入数据使用 `asc_ldcg`加载，提示硬件输入数据不需要优先占用DCache空间，减少输入数据对DCache的占用优先级；
  - sin查找表使用 `asc_ldca`加载，提示硬件优先为sin表数据分配DCache空间，确保sin表常驻DCache；
  - 输出数据使用 `asc_stcg`写入，提示硬件优化写入后的Cache分配策略，避免输出数据驻留DCache影响热点数据的缓存。

  正例算子的性能数据如下：

  | Task Duration(us) | DCache Read GM | DCache Read Vector | DCache Write Vector |
  | :---------------: | :------------: | :-----------------: | :------------------: |
  |      50.895       |      3531      |        2048         |        6144          |

  从Task Duration看，优化后的执行耗时为50.895us，相比优化前的56.82us，下降约10.4%。从DCache Read GM看，优化后为3531次，相比优化前的5064次，减少约30.2%（减少1533次GM访问）。这说明优化前的主要瓶颈来自所有数据共同竞争DCache空间，导致热点数据（sin查找表）被挤出DCache后需要重新从GM加载；通过数据缓存提示函数区分缓存策略后，sin查找表优先驻留DCache，减少了从GM重新加载的次数，DCache命中率提升，整体性能得到改善。

【总结】当SIMT算子中存在多种不同访问模式的数据时，应分析各类数据的访问特征，通过数据缓存提示函数为不同类型的数据指定合适的缓存策略：对仅遍历一次的流式数据使用 `asc_ldcg`降低其Cache占用优先级；对需要频繁访问的热点数据使用 `asc_ldca`确保其常驻DCache；对写入后无需驻留DCache的输出数据使用 `asc_stcg`优化写入后的Cache分配。这样可以减少热点数据被挤出DCache的情况，降低GM访问次数，提升算子整体性能。