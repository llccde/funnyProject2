#include<cmath>
#include"perlin.h"


float dot(float3 v1,float3 v2){
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}
float grad(int hash, float x, float y, float z)
{
    float3 v3 = float3(x,y,z);
    hash = hash & 0xb;
    return dot(grads[hash],v3);
}

int inc(int num) {
    num++;
    return num;
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);         
}
float lerp(float f1,float f2,float l){
    return (1-l)*f1+l*f2;
}
float perlin(float x,float y,float z)
{
    int xi = (int)x & 255;
    int yi = (int)y & 255;
    int zi = (int)z & 255;

    float xf = x - xi;
    float yf = y - yi;
    float zf = z - zi;

    float u = fade(xf);
    float v = fade(yf);
    float w = fade(zf);
    //从数组中随机取出一个值
    int aaa, aba, aab, abb, baa, bba, bab, bbb;
    aaa = p[p[p[    xi ]+    yi ]+    zi ];
    aba = p[p[p[    xi ]+inc(yi)]+    zi ];
    aab = p[p[p[    xi ]+    yi ]+inc(zi)];
    abb = p[p[p[    xi ]+inc(yi)]+inc(zi)];
    baa = p[p[p[inc(xi)]+    yi ]+    zi ];
    bba = p[p[p[inc(xi)]+inc(yi)]+    zi ];
    bab = p[p[p[inc(xi)]+    yi ]+inc(zi)];
    bbb = p[p[p[inc(xi)]+inc(yi)]+inc(zi)];

    float x1, x2, y1, y2;
    x1 = lerp(    grad (aaa, xf  , yf  , zf),           
        grad (baa, xf-1, yf  , zf),             
        u);                                     
    x2 = lerp(    grad (aba, xf  , yf-1, zf),           
    grad (bba, xf-1, yf-1, zf),             
        u);
    y1 = lerp(x1, x2, v);

    x1 = lerp(    grad (aab, xf  , yf  , zf-1),
        grad (bab, xf-1, yf  , zf-1),
        u);
    x2 = lerp(    grad (abb, xf  , yf-1, zf-1),
        grad (bbb, xf-1, yf-1, zf-1),
        u);
    y2 = lerp (x1, x2, v);

    return lerp (y1, y2, w); 
}
