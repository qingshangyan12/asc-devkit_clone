#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static inline float u2f(uint32_t u) {float f; memcpy(&f, &u, sizeof(f)); return f;}
static inline uint32_t f2u(float f) {uint32_t u; memcpy(&u, &f, sizeof(u)); return u;}

void log1p_kernel(float *output, float x){
    uint32_t r1 = f2u(x);

    float x1 = x + 1.0f;
    uint32_t r2 = f2u(x1);
    
    int32_t r3 = (int32_t)r2 - 0x3F400000;
    uint32_t r4 = (uint32_t)r3 & 0xFF800000u;

    float f9 = u2f(r1 - r4);
    float f10 = u2f(0x40800000u - r4);
    float t = fmaf(0.25f, f10, -1.0f) + f9;
    float nln2 = (float)(int32_t)r4 * u2f(0x34000000u);
    float p = u2f(0xBD39BF78u);
    p = fmaf(p, t, u2f(0x3DD80012u));
    p = fmaf(p, t, u2f(0xBE0778E0u));
    p = fmaf(p, t, u2f(0x3E146475u));
    p = fmaf(p, t, u2f(0xBE2A68DDu));
    p = fmaf(p, t, u2f(0x3E4CAF9Eu));
    p = fmaf(p, t, u2f(0xBE800042u));
    p = fmaf(p, t, u2f(0x3EAAAAE6u));
    p = fmaf(p, t, -0.5f);

    float result = fmaf(t * p, t, t);

    result = fmaf(nln2, u2f(0x3F317218u), result);

    if (r1 >= 2139095040u) {
        if ((int32_t)r1 >= -1082130431) {
            float inf = u2f(0x7F8000000u);
            result = fmaf(x, inf, inf);
        }

        if (x == 0.0f) {
            result = u2f(0x80000000u);
        }
        *output = result;
    }
}