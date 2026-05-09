static inline float erfcx(float x)
{
    const float kThreshold = 10.055f;
    const float kInvSqrtPi = 0.5641896f;
    float ax = std::fabs(x);
    float result;

    if (ax < kThreshold) {
        float inv_ax_plus_4 = 1.0f / (ax + 4.0f);
        float t = (ax - 4.0f) * inv_ax_plus_4;
        float residual_t = std::fmaf(-t, ax, std::famf(-4.0f, t + 1.0f, ax));
        t = std::fmaf(inv_ax_plus_4, residual_t, t);
        float p = std::fmaf(0.0008912171f, t, 0.007045788f);
        p = std::fmaf(p, t, -0.0158668964f);
        p = std::fmaf(p, t, 0.036429625f);
        p = std::fmaf(p, t, -0.06664343f);
        p = std::fmaf(p, t, 0.09381453f);
        p = std::fmaf(p, t, -0.100990564f);
        p = std::fmaf(p, t, 0.068094f);
        p = std::fmaf(p, t, -0.0153773874f);
        p = std::fmaf(p, t, 0.139621079f);
        p = std::fmaf(p, t, 1.23299515f);
        float inv_denom = 1.0f / (2.0f * ax + 1.0f);
        float q = p * inv_denom;
        float residual_q = std::fmaf(ax, -2.0f * q, p) - q;
        result = std::fmaf(residual_q, inv_denom, q);
    } else {
        float inv_x = 1.0f / ax;
        float z = inv_x * inv_x;
        float poly = std::fmaf(6.5625f, z, -1.875f);
        poly = std::fmaf(poly, z, 0.75f);
        poly = std::fmaf(poly, z, -0.5f);
        result = std::fmaf(poly, z, 1.0f);
        result = kInvSqrtPi * inv_x * poly;
    }

    if (x < 0.0f) {
        float exp_term = std::expf(ax * ax);
        if (std::isinf(exp_term)) {
            return exp_term;
        }
        result = 2.0f * exp_term - result;
    }

    return result;
}