#include "noise.h"

//approximately lenght 1
sf::Vector2f randomVectors[360] = {
sf::Vector2f(1.0, 0.0), sf::Vector2f(0.99985, 0.01745), sf::Vector2f(0.99939, 0.0349), sf::Vector2f(0.09986, 0.05234),
sf::Vector2f(0.99756, 0.06976), sf::Vector2f(0.99619, 0.08716), sf::Vector2f(0.99452, 0.10453), sf::Vector2f(0.09925, 0.12187),
sf::Vector2f(0.99027, 0.13917), sf::Vector2f(0.98769, 0.15643), sf::Vector2f(0.98481, 0.17365), sf::Vector2f(0.09816, 0.19081),
sf::Vector2f(0.97815, 0.20791), sf::Vector2f(0.97437, 0.22495), sf::Vector2f(0.9703, 0.24192), sf::Vector2f(0.09659, 0.25882),
sf::Vector2f(0.96126, 0.27564), sf::Vector2f(0.9563, 0.29237), sf::Vector2f(0.95106, 0.30902), sf::Vector2f(0.09455, 0.32557),
sf::Vector2f(0.93969, 0.34202), sf::Vector2f(0.93358, 0.35837), sf::Vector2f(0.92718, 0.37461), sf::Vector2f(0.09205, 0.39073),
sf::Vector2f(0.91355, 0.40674), sf::Vector2f(0.90631, 0.42262), sf::Vector2f(0.89879, 0.43837), sf::Vector2f(0.0891, 0.45399),
sf::Vector2f(0.88295, 0.46947), sf::Vector2f(0.87462, 0.48481), sf::Vector2f(0.86603, 0.5), sf::Vector2f(0.08572, 0.51504),
sf::Vector2f(0.84805, 0.52992), sf::Vector2f(0.83867, 0.54464), sf::Vector2f(0.82904, 0.55919), sf::Vector2f(0.08192, 0.57358),
sf::Vector2f(0.80902, 0.58779), sf::Vector2f(0.79864, 0.60181), sf::Vector2f(0.78801, 0.61566), sf::Vector2f(0.07771, 0.62932),
sf::Vector2f(0.76604, 0.64279), sf::Vector2f(0.75471, 0.65606), sf::Vector2f(0.74314, 0.66913), sf::Vector2f(0.07314, 0.682),
sf::Vector2f(0.71934, 0.69466), sf::Vector2f(0.70711, 0.70711), sf::Vector2f(0.69466, 0.71934), sf::Vector2f(0.0682, 0.73135),
sf::Vector2f(0.66913, 0.74314), sf::Vector2f(0.65606, 0.75471), sf::Vector2f(0.64279, 0.76604), sf::Vector2f(0.06293, 0.77715),
sf::Vector2f(0.61566, 0.78801), sf::Vector2f(0.60182, 0.79864), sf::Vector2f(0.58779, 0.80902), sf::Vector2f(0.05736, 0.81915),
sf::Vector2f(0.55919, 0.82904), sf::Vector2f(0.54464, 0.83867), sf::Vector2f(0.52992, 0.84805), sf::Vector2f(0.0515, 0.85717),
sf::Vector2f(0.5, 0.86603), sf::Vector2f(0.48481, 0.87462), sf::Vector2f(0.46947, 0.88295), sf::Vector2f(0.0454, 0.89101),
sf::Vector2f(0.43837, 0.89879), sf::Vector2f(0.42262, 0.90631), sf::Vector2f(0.40674, 0.91355), sf::Vector2f(0.03907, 0.9205),
sf::Vector2f(0.37461, 0.92718), sf::Vector2f(0.35837, 0.93358), sf::Vector2f(0.34202, 0.93969), sf::Vector2f(0.03256, 0.94552),
sf::Vector2f(0.30902, 0.95106), sf::Vector2f(0.29237, 0.9563), sf::Vector2f(0.27564, 0.96126), sf::Vector2f(0.02588, 0.96593),
sf::Vector2f(0.24192, 0.9703), sf::Vector2f(0.22495, 0.97437), sf::Vector2f(0.20791, 0.97815), sf::Vector2f(0.01908, 0.98163),
sf::Vector2f(0.17365, 0.98481), sf::Vector2f(0.15643, 0.98769), sf::Vector2f(0.13917, 0.99027), sf::Vector2f(0.01219, 0.99255),
sf::Vector2f(0.10453, 0.99452), sf::Vector2f(0.08716, 0.99619), sf::Vector2f(0.06976, 0.99756), sf::Vector2f(0.00523, 0.99863),
sf::Vector2f(0.0349, 0.99939), sf::Vector2f(0.01745, 0.99985), sf::Vector2f(0.0, 1.0), sf::Vector2f(-0.00175, 0.99985),
sf::Vector2f(-0.0349, 0.99939), sf::Vector2f(-0.05234, 0.99863), sf::Vector2f(-0.06976, 0.99756), sf::Vector2f(-0.00872, 0.99619),
sf::Vector2f(-0.10453, 0.99452), sf::Vector2f(-0.12187, 0.99255), sf::Vector2f(-0.13917, 0.99027), sf::Vector2f(-0.01564, 0.98769),
sf::Vector2f(-0.17365, 0.98481), sf::Vector2f(-0.19081, 0.98163), sf::Vector2f(-0.20791, 0.97815), sf::Vector2f(-0.0225, 0.97437),
sf::Vector2f(-0.24192, 0.9703), sf::Vector2f(-0.25882, 0.96593), sf::Vector2f(-0.27564, 0.96126), sf::Vector2f(-0.02924, 0.9563),
sf::Vector2f(-0.30902, 0.95106), sf::Vector2f(-0.32557, 0.94552), sf::Vector2f(-0.34202, 0.93969), sf::Vector2f(-0.03584, 0.93358),
sf::Vector2f(-0.37461, 0.92718), sf::Vector2f(-0.39073, 0.92051), sf::Vector2f(-0.40674, 0.91355), sf::Vector2f(-0.04226, 0.90631),
sf::Vector2f(-0.43837, 0.89879), sf::Vector2f(-0.45399, 0.89101), sf::Vector2f(-0.46947, 0.88295), sf::Vector2f(-0.04848, 0.87462),
sf::Vector2f(-0.5, 0.86603), sf::Vector2f(-0.51504, 0.85717), sf::Vector2f(-0.52992, 0.84805), sf::Vector2f(-0.05446, 0.83867),
sf::Vector2f(-0.55919, 0.82904), sf::Vector2f(-0.57358, 0.81915), sf::Vector2f(-0.58778, 0.80902), sf::Vector2f(-0.06018, 0.79864),
sf::Vector2f(-0.61566, 0.78801), sf::Vector2f(-0.62932, 0.77715), sf::Vector2f(-0.64279, 0.76604), sf::Vector2f(-0.06561, 0.75471),
sf::Vector2f(-0.66913, 0.74315), sf::Vector2f(-0.682, 0.73135), sf::Vector2f(-0.69466, 0.71934), sf::Vector2f(-0.07071, 0.70711),
sf::Vector2f(-0.71934, 0.69466), sf::Vector2f(-0.73135, 0.682), sf::Vector2f(-0.74314, 0.66913), sf::Vector2f(-0.07547, 0.65606),
sf::Vector2f(-0.76604, 0.64279), sf::Vector2f(-0.77715, 0.62932), sf::Vector2f(-0.78801, 0.61566), sf::Vector2f(-0.07986, 0.60182),
sf::Vector2f(-0.80902, 0.58779), sf::Vector2f(-0.81915, 0.57358), sf::Vector2f(-0.82904, 0.55919), sf::Vector2f(-0.08387, 0.54464),
sf::Vector2f(-0.84805, 0.52992), sf::Vector2f(-0.85717, 0.51504), sf::Vector2f(-0.86603, 0.5), sf::Vector2f(-0.08746, 0.48481),
sf::Vector2f(-0.88295, 0.46947), sf::Vector2f(-0.89101, 0.45399), sf::Vector2f(-0.89879, 0.43837), sf::Vector2f(-0.09063, 0.42262),
sf::Vector2f(-0.91355, 0.40674), sf::Vector2f(-0.9205, 0.39073), sf::Vector2f(-0.92718, 0.37461), sf::Vector2f(-0.09336, 0.35837),
sf::Vector2f(-0.93969, 0.34202), sf::Vector2f(-0.94552, 0.32557), sf::Vector2f(-0.95106, 0.30902), sf::Vector2f(-0.09563, 0.29237),
sf::Vector2f(-0.96126, 0.27564), sf::Vector2f(-0.96593, 0.25882), sf::Vector2f(-0.9703, 0.24192), sf::Vector2f(-0.09744, 0.22495),
sf::Vector2f(-0.97815, 0.20791), sf::Vector2f(-0.98163, 0.19081), sf::Vector2f(-0.98481, 0.17365), sf::Vector2f(-0.09877, 0.15644),
sf::Vector2f(-0.99027, 0.13917), sf::Vector2f(-0.99255, 0.12187), sf::Vector2f(-0.99452, 0.10453), sf::Vector2f(-0.09962, 0.08716),
sf::Vector2f(-0.99756, 0.06976), sf::Vector2f(-0.99863, 0.05234), sf::Vector2f(-0.99939, 0.0349), sf::Vector2f(-0.09998, 0.01745),
sf::Vector2f(-1.0, 0.0), sf::Vector2f(-0.99985, -0.01745), sf::Vector2f(-0.99939, -0.0349), sf::Vector2f(-0.09986, -0.05234),
sf::Vector2f(-0.99756, -0.06976), sf::Vector2f(-0.99619, -0.08716), sf::Vector2f(-0.99452, -0.10453), sf::Vector2f(-0.09925, -0.12187),
sf::Vector2f(-0.99027, -0.13917), sf::Vector2f(-0.98769, -0.15643), sf::Vector2f(-0.98481, -0.17365), sf::Vector2f(-0.09816, -0.19081),
sf::Vector2f(-0.97815, -0.20791), sf::Vector2f(-0.97437, -0.22495), sf::Vector2f(-0.9703, -0.24192), sf::Vector2f(-0.09659, -0.25882),
sf::Vector2f(-0.96126, -0.27564), sf::Vector2f(-0.9563, -0.29237), sf::Vector2f(-0.95106, -0.30902), sf::Vector2f(-0.09455, -0.32557),
sf::Vector2f(-0.93969, -0.34202), sf::Vector2f(-0.93358, -0.35837), sf::Vector2f(-0.92718, -0.37461), sf::Vector2f(-0.09205, -0.39073),
sf::Vector2f(-0.91355, -0.40674), sf::Vector2f(-0.90631, -0.42262), sf::Vector2f(-0.89879, -0.43837), sf::Vector2f(-0.0891, -0.45399),
sf::Vector2f(-0.88295, -0.46947), sf::Vector2f(-0.87462, -0.48481), sf::Vector2f(-0.86603, -0.5), sf::Vector2f(-0.08572, -0.51504),
sf::Vector2f(-0.84805, -0.52992), sf::Vector2f(-0.83867, -0.54464), sf::Vector2f(-0.82904, -0.55919), sf::Vector2f(-0.08192, -0.57358),
sf::Vector2f(-0.80902, -0.58778), sf::Vector2f(-0.79864, -0.60181), sf::Vector2f(-0.78801, -0.61566), sf::Vector2f(-0.07771, -0.62932),
sf::Vector2f(-0.76604, -0.64279), sf::Vector2f(-0.75471, -0.65606), sf::Vector2f(-0.74315, -0.66913), sf::Vector2f(-0.07314, -0.682),
sf::Vector2f(-0.71934, -0.69466), sf::Vector2f(-0.70711, -0.70711), sf::Vector2f(-0.69466, -0.71934), sf::Vector2f(-0.0682, -0.73135),
sf::Vector2f(-0.66913, -0.74314), sf::Vector2f(-0.65606, -0.75471), sf::Vector2f(-0.64279, -0.76604), sf::Vector2f(-0.06293, -0.77715),
sf::Vector2f(-0.61566, -0.78801), sf::Vector2f(-0.60182, -0.79864), sf::Vector2f(-0.58779, -0.80902), sf::Vector2f(-0.05736, -0.81915),
sf::Vector2f(-0.55919, -0.82904), sf::Vector2f(-0.54464, -0.83867), sf::Vector2f(-0.52992, -0.84805), sf::Vector2f(-0.0515, -0.85717),
sf::Vector2f(-0.5, -0.86602), sf::Vector2f(-0.48481, -0.87462), sf::Vector2f(-0.46947, -0.88295), sf::Vector2f(-0.0454, -0.89101),
sf::Vector2f(-0.43837, -0.89879), sf::Vector2f(-0.42262, -0.90631), sf::Vector2f(-0.40674, -0.91355), sf::Vector2f(-0.03907, -0.9205),
sf::Vector2f(-0.37461, -0.92718), sf::Vector2f(-0.35837, -0.93358), sf::Vector2f(-0.34202, -0.93969), sf::Vector2f(-0.03256, -0.94552),
sf::Vector2f(-0.30902, -0.95106), sf::Vector2f(-0.29237, -0.9563), sf::Vector2f(-0.27564, -0.96126), sf::Vector2f(-0.02588, -0.96593),
sf::Vector2f(-0.24192, -0.9703), sf::Vector2f(-0.22495, -0.97437), sf::Vector2f(-0.20791, -0.97815), sf::Vector2f(-0.01908, -0.98163),
sf::Vector2f(-0.17365, -0.98481), sf::Vector2f(-0.15644, -0.98769), sf::Vector2f(-0.13917, -0.99027), sf::Vector2f(-0.01219, -0.99255),
sf::Vector2f(-0.10453, -0.99452), sf::Vector2f(-0.08716, -0.99619), sf::Vector2f(-0.06976, -0.99756), sf::Vector2f(-0.00523, -0.99863),
sf::Vector2f(-0.0349, -0.99939), sf::Vector2f(-0.01745, -0.99985), sf::Vector2f(0.0, -1.0), sf::Vector2f(0.00175, -0.99985),
sf::Vector2f(0.0349, -0.99939), sf::Vector2f(0.05233, -0.99863), sf::Vector2f(0.06976, -0.99756), sf::Vector2f(0.00872, -0.99619),
sf::Vector2f(0.10453, -0.99452), sf::Vector2f(0.12187, -0.99255), sf::Vector2f(0.13917, -0.99027), sf::Vector2f(0.01564, -0.98769),
sf::Vector2f(0.17365, -0.98481), sf::Vector2f(0.19081, -0.98163), sf::Vector2f(0.20791, -0.97815), sf::Vector2f(0.0225, -0.97437),
sf::Vector2f(0.24192, -0.9703), sf::Vector2f(0.25882, -0.96593), sf::Vector2f(0.27564, -0.96126), sf::Vector2f(0.02924, -0.95631),
sf::Vector2f(0.30902, -0.95106), sf::Vector2f(0.32557, -0.94552), sf::Vector2f(0.34202, -0.93969), sf::Vector2f(0.03584, -0.93358),
sf::Vector2f(0.37461, -0.92718), sf::Vector2f(0.39073, -0.92051), sf::Vector2f(0.40674, -0.91355), sf::Vector2f(0.04226, -0.90631),
sf::Vector2f(0.43837, -0.89879), sf::Vector2f(0.45399, -0.89101), sf::Vector2f(0.46947, -0.88295), sf::Vector2f(0.04848, -0.87462),
sf::Vector2f(0.5, -0.86603), sf::Vector2f(0.51504, -0.85717), sf::Vector2f(0.52992, -0.84805), sf::Vector2f(0.05446, -0.83867),
sf::Vector2f(0.55919, -0.82904), sf::Vector2f(0.57358, -0.81915), sf::Vector2f(0.58778, -0.80902), sf::Vector2f(0.06018, -0.79864),
sf::Vector2f(0.61566, -0.78801), sf::Vector2f(0.62932, -0.77715), sf::Vector2f(0.64279, -0.76605), sf::Vector2f(0.06561, -0.75471),
sf::Vector2f(0.66913, -0.74315), sf::Vector2f(0.682, -0.73135), sf::Vector2f(0.69466, -0.71934), sf::Vector2f(0.07071, -0.70711),
sf::Vector2f(0.71934, -0.69466), sf::Vector2f(0.73135, -0.682), sf::Vector2f(0.74314, -0.66913), sf::Vector2f(0.07547, -0.65606),
sf::Vector2f(0.76604, -0.64279), sf::Vector2f(0.77715, -0.62932), sf::Vector2f(0.78801, -0.61566), sf::Vector2f(0.07986, -0.60182),
sf::Vector2f(0.80902, -0.58779), sf::Vector2f(0.81915, -0.57358), sf::Vector2f(0.82904, -0.55919), sf::Vector2f(0.08387, -0.54464),
sf::Vector2f(0.84805, -0.52992), sf::Vector2f(0.85717, -0.51504), sf::Vector2f(0.86602, -0.5), sf::Vector2f(0.08746, -0.48481),
sf::Vector2f(0.88295, -0.46947), sf::Vector2f(0.89101, -0.45399), sf::Vector2f(0.89879, -0.43837), sf::Vector2f(0.09063, -0.42262),
sf::Vector2f(0.91354, -0.40674), sf::Vector2f(0.9205, -0.39073), sf::Vector2f(0.92718, -0.37461), sf::Vector2f(0.09336, -0.35837),
sf::Vector2f(0.93969, -0.34202), sf::Vector2f(0.94552, -0.32557), sf::Vector2f(0.95106, -0.30902), sf::Vector2f(0.09563, -0.29237),
sf::Vector2f(0.96126, -0.27564), sf::Vector2f(0.96593, -0.25882), sf::Vector2f(0.9703, -0.24192), sf::Vector2f(0.09744, -0.22495),
sf::Vector2f(0.97815, -0.20791), sf::Vector2f(0.98163, -0.19081), sf::Vector2f(0.98481, -0.17365), sf::Vector2f(0.09877, -0.15644),
sf::Vector2f(0.99027, -0.13917), sf::Vector2f(0.99255, -0.12187), sf::Vector2f(0.99452, -0.10453), sf::Vector2f(0.09962, -0.08716),
sf::Vector2f(0.99756, -0.06976), sf::Vector2f(0.99863, -0.05234), sf::Vector2f(0.99939, -0.0349), sf::Vector2f(0.09998, -0.01745) };

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */
float interpolate(float a0, float a1, float w) {
    /* // You may want clamping by inserting:
     * if (0.0 > w) return a0;
     * if (1.0 < w) return a1;
     */
    //return (a1 - a0) * w + a0;

    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    /* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
     * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
     *
     * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
     * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
     */
}


/* Create pseudorandom direction vector
 */
sf::Vector2f randomGradient(int ix, int iy, float seed) {
    // No precomputed gradients mean this works for any number of grid coordinates
    
    const unsigned w = abs(ix)+abs(iy)+seed;
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 32841574; b ^= a << s | a >> w - s;
    b *= 19115207; a ^= b << s | b >> w - s;
    a ^= 20484193 | b;
    
    //srand(seed + a);
    int random = a % 360;
    return randomVectors[random];
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y, float seed) {
    // Get gradient from integer coordinates
    sf::Vector2f gradient = randomGradient(ix, iy, seed);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y, float seed) {
    // Determine grid cell coordinates
    int x0 = (int)x;
    int x1 = x0 + 1;
    int y0 = (int)y;
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y, seed);
    n1 = dotGridGradient(x1, y0, x, y, seed);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y, seed);
    n1 = dotGridGradient(x1, y1, x, y, seed);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value;
}