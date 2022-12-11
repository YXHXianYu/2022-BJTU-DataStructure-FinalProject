#version 450 core
// copy from: https://www.shadertoy.com/view/4sBXzG
// amazing!

uniform float iTime;
uniform vec2 iResolution;
const vec2 iMouse = vec2(0., 0.);

in vec2 fragCoord;

out vec4 fragColor;

// Star map shader...procedural space background

// Uncomment to see the lat-long grid for context
// #define SHOW_LARGE_GRID
#define SHOW_SPHERE
#define SHOW_SPHERE_GRID
#define SHOW_PLANET

const float pi = 3.1415927;
const float deg = pi / 180.0;

// See derivation of noise functions by Morgan McGuire at https://www.shadertoy.com/view/4dS3Wd
const int NUM_OCTAVES = 4;
float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }
// 1 octave value noise
float noise(float x) { float i = floor(x); float f = fract(x); float u = f * f * (3.0 - 2.0 * f); return mix(hash(i), hash(i + 1.0), u); }
float noise(vec2 x) { vec2 i = floor(x); vec2 f = fract(x);	float a = hash(i); float b = hash(i + vec2(1.0, 0.0)); float c = hash(i + vec2(0.0, 1.0)); float d = hash(i + vec2(1.0, 1.0)); vec2 u = f * f * (3.0 - 2.0 * f); return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y; }
float noise(vec3 x) { const vec3 step = vec3(110, 241, 171); vec3 i = floor(x); vec3 f = fract(x); float n = dot(i, step); vec3 u = f * f * (3.0 - 2.0 * f); return mix(mix(mix( hash(n + dot(step, vec3(0, 0, 0))), hash(n + dot(step, vec3(1, 0, 0))), u.x), mix( hash(n + dot(step, vec3(0, 1, 0))), hash(n + dot(step, vec3(1, 1, 0))), u.x), u.y), mix(mix( hash(n + dot(step, vec3(0, 0, 1))), hash(n + dot(step, vec3(1, 0, 1))), u.x), mix( hash(n + dot(step, vec3(0, 1, 1))), hash(n + dot(step, vec3(1, 1, 1))), u.x), u.y), u.z); }
// Multi-octave value noise
float NOISE(float x) { float v = 0.0; float a = 0.5; float shift = float(100); for (int i = 0; i < NUM_OCTAVES; ++i) { v += a * noise(x); x = x * 2.0 + shift; a *= 0.5; } return v; }
float NOISE(vec2 x) { float v = 0.0; float a = 0.5; vec2 shift = vec2(100); mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50)); for (int i = 0; i < NUM_OCTAVES; ++i) { v += a * noise(x); x = rot * x * 2.0 + shift; a *= 0.5; } return v; }
// Fast hash2 from https://www.shadertoy.com/view/lsfGWH
float hash2(vec2 co) { return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453); }
float maxComponent(vec2 v) { return max(v.x, v.y); }
float maxComponent(vec3 v) { return max(max(v.x, v.y), v.z); }
float minComponent(vec2 v) { return min(v.x, v.y); }
mat3 rotation(float yaw, float pitch) { return mat3(cos(yaw), 0, -sin(yaw), 0, 1, 0, sin(yaw), 0, cos(yaw)) * mat3(1, 0, 0, 0, cos(pitch), sin(pitch), 0, -sin(pitch), cos(pitch)); }
float square(float x) { return x * x; }

///////////////////////////////////////////////////////////////////////

// Only globals needed for the actual spheremap

// starplane was derived from https://www.shadertoy.com/view/lsfGWH
float starplane(vec3 dir) {
    float screenscale = 1.0 / iResolution.x;

    // Project to a cube-map plane and scale with the resolution of the display
    vec2 basePos = dir.xy * (0.5 / screenscale) / max(1e-3, abs(dir.z));

    const float largeStarSizePixels = 20.0;

    // Probability that a pixel is NOT on a large star. Must change with largeStarSizePixels
    const float prob = 0.97;

    float color = 0.0;
    vec2 pos = floor(basePos / largeStarSizePixels);
    float starValue = hash2(pos);

    // Big stars
    if (starValue > prob) {
        // Sphere blobs
        vec2 delta = basePos - largeStarSizePixels * (pos + vec2(0.5));
        color = max(1.0 - length(delta) / (0.5 * largeStarSizePixels), 0.0);
        // Star shapes
        color *= 1.0 / max(1e-3, abs(delta.x) * abs(delta.y));
        // Avoid triplanar seams where star distort and clump
        color *= pow(abs(dir.z), 12.0);
    }
    // Small stars
    // Stabilize stars under motion by locking to a grid
    basePos = floor(basePos);
    if (hash2(basePos.xy * screenscale) > 0.997) {
        float r = hash2(basePos.xy * 0.5);
        color += r * (0.3 * sin(iTime * (r * 5.0) + r) + 0.7) * 1.5;
    }
    // Weight by the z-plane
    return color * abs(dir.z);
}


float starbox(vec3 dir) {
    return starplane(dir.xyz) + starplane(dir.yzx) + starplane(dir.zxy);
}


float starfield(vec3 dir) {
    return starbox(dir) + starbox(rotation(45.0 * deg, 45.0 * deg) * dir);
}


vec3 nebula(vec3 dir) {
    float purple = abs(dir.x);
    float yellow = noise(dir.y);
    vec3 streakyHue = vec3(purple + yellow, yellow * 0.7, purple);
    vec3 puffyHue = vec3(0.8, 0.1, 1.0);

    float streaky = min(1.0, 8.0 * pow(NOISE(dir.yz * square(dir.x) * 13.0 + dir.xy * square(dir.z) * 7.0 + vec2(150.0, 2.0)), 10.0));
    float puffy = square(NOISE(dir.xz * 4.0 + vec2(30, 10)) * dir.y);

    return clamp(puffyHue * puffy * (1.0 - streaky) + streaky * streakyHue, 0.0, 1.0);
}


vec3 sun(vec3 d) {
    float angle = atan(d.x, d.y);
    float falloff = pow(max(d.z, 0.0), 10.0);
    vec3 core = vec3(2.8, 1.5 + 0.5 * noise(iTime * 0.25 + d.xy * 5.0), 1.5) * falloff;
    float corona = NOISE(vec2(d.z * 250.0 + iTime, iTime * 0.2 + angle * 50.0)) * smoothstep(0.95, 0.92, d.z) * falloff * square(d.z);

    return core * (1.0 - corona);
}


vec4 planet(vec3 view) {
#ifdef SHOW_PLANET
    const float PLANET_RADIUS = 0.65;
    if (view.y > -PLANET_RADIUS) {
        return vec4(0.0);
    }

    // Compute the point on the planet sphere
    float angle  = atan(view.x, view.z);
    float radius = sqrt((1.0 + view.y) / (1.0 - PLANET_RADIUS));

    vec3 s = vec3(radius * normalize(view.xz), sqrt(1.0 - square(radius)));


    vec3 dir = s;
    dir = rotation(0.0, iTime * 0.01) * dir;
    float latLongLine = 0.0;// (1.0 - pow(smoothstep(0.0, 0.04, min(abs(fract(atan(dir.y, length(dir.xz)) / (15.0 * deg)) - 0.5), abs(fract(atan(dir.x, dir.z) / (15.0 * deg)) - 0.5)) * 2.0), 10.0));

    // Antialias the edge of the planet
    vec4 surface = vec4(1.2 * vec3(1.0, 0.3, 0.4) *
                        (noise(dir * 39.0 + 3.5) * 0.5 + noise(dir * 26.0) + 2.0 * noise(dir * 13.0 + 1.0)) *
         vec3(s.yx * 0.5 + 0.5, 0.0).rbg, smoothstep(0.992, 0.988, radius));

    // Keep the clouds above the planet
    vec4 cloud = vec4(vec3(1.5),
                      smoothstep(1.0, 0.995, radius) *
                      square(NOISE(vec2(iTime * 0.1, 0.0) + dir.xz * 11.0 * square(dir.y) + dir.yx * 3.0 + dir.zy * 1.2)));

    return vec4(
        mix(surface.rgb, cloud.rgb, cloud.a) * (max(0.1, s.y) * vec3(1.0 - latLongLine)),
        max(surface.a, cloud.a));
#else
    return vec4(0.0);
#endif
}


vec3 sphereColor(vec3 dir) {
    vec3 n = nebula(dir);
    vec4 p = planet(dir);
    vec3 color =
        sun(dir) +
        mix(vec3(starfield(dir)) * (1.0 - maxComponent(n)) +  // Nebula holds out star
            n, // nebula
            p.rgb, p.a); // planet

        return color;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Spheremap visualization code from https://www.shadertoy.com/view/4sSXzG

void main() {
    float scale = 1.0 / min(iResolution.x, iResolution.y);
    // Of the background
    const float verticalFieldOfView = 60.0 * deg;
    const float insetSphereRadius = 0.22;

    float yaw   = -((iMouse.x / iResolution.y) * 2.0 - 1.0) * 3.0;
    float pitch = ((iMouse.y / iResolution.y) * 2.0 - 1.0) * 3.0;

    vec3 dir = rotation(yaw, pitch) * normalize(vec3(fragCoord.xy - iResolution.xy / 2.0, iResolution.y / ( -2.0 * tan(verticalFieldOfView / 2.0))));

    fragColor.rgb = sphereColor(dir);
    #ifdef SHOW_LARGE_GRID
    float latLongLine = (1.0 - pow(smoothstep(0.0, 0.04, min(abs(fract(atan(dir.y, length(dir.xz)) / (15.0 * deg)) - 0.5), abs(fract(atan(dir.x, dir.z) / (15.0 * deg)) - 0.5)) * 2.0), 10.0));
    fragColor.rgb += latLongLine * vec3(0.0, 0.7, 1.5);
    #endif


    #ifdef SHOW_SPHERE
    // Inset sphere
    vec2 spherePoint = (fragCoord.xy * scale - insetSphereRadius * 1.1) / insetSphereRadius;
    if (length(spherePoint) <= 1.0) {
        // Antialias using many samples
        vec3 c = vec3(0);
        for (int x = -3; x <= 3; ++x) {
            for (int y = -3; y <= 3; ++y) {
                vec2 s = clamp(((fragCoord.xy + vec2(x, y) / 7.0) * scale - insetSphereRadius * 1.1) / insetSphereRadius, vec2(-1.0), vec2(1.0));
                dir = rotation(iTime, -iTime * 0.17) * vec3(s.xy, sqrt(max(0.0, 1.0 - dot(s.xy, s.xy))));
                c += sphereColor(dir);
                #ifdef SHOW_SPHERE_GRID
                float latLongLine = (1.0 - pow(smoothstep(0.0, 0.04, min(abs(fract(atan(dir.y, length(dir.xz)) / (15.0 * deg)) - 0.5), abs(fract(atan(dir.x, dir.z) / (15.0 * deg)) - 0.5)) * 2.0), 10.0));
                c += latLongLine * vec3(0.0, 0.7, 1.5);
                #endif
            }
        }
        c /= 36.0;
        // Fade the inset sphere to antialias its border transition
        // fragColor.rgb = mix(sqrt(fragColor.rgb), c, clamp((1.0 - length(spherePoint)) * 100.0, 0.0, 1.0));
    }
    #endif

    fragColor.rgb = sqrt(fragColor.rgb);
    fragColor.a = 1.0;

    // fragColor.rgb = vec3(fragCoord.xy / iResolution.xy, 0.0);
}
