#version 450 core
// copy from: https://www.shadertoy.com/view/XsXXDn
// amazing!

uniform float iTime;
uniform vec2 iResolution;
const vec2 iMouse = vec2(0., 0.);

in vec2 fragCoord;

out vec4 fragColor;

#define t iTime
#define r iResolution.xy
void main()
{
    vec3 c;
    float l,z=t;
    for(int i=0;i<3;i++) {
            vec2 uv,p=fragCoord.xy/r;
            uv=p;
            p-=.5;
            p.x*=r.x/r.y;
            z+=.07;
            l=length(p);
            uv+=p/l*(sin(z)+1.)*abs(sin(l*9.-z-z));
            c[i]=.01/length(mod(uv,1.)-.5);
    }
    fragColor=vec4(c/l,t);
}
