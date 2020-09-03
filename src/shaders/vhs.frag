#version 330 core
out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D scrTex;
uniform uint ticks;

float fkx=.7,fky=.7;
float finten=1.8;
float cdist=.01;

/*
Noise algorithm from
https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
*/
float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

/*
Based on
https://www.shadertoy.com/view/Ms3XWH
*/
float horizDeform(float refy,float y,float deformH)
{
    float r1=refy-deformH,r2=refy+deformH;
    float ry=smoothstep(refy,r2,y)*2;
    ry-=smoothstep(r1,refy,y)*2;
    return ry;
}

float offset=1.0/480.0;
vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right
);
vec3 kernel(float kernel[9],sampler2D tex,vec2 pos)
{
    vec3 acc=vec3(0,0,0);
    for(int i=0;i<9;i++)
    {
        acc+=kernel[i]*texture(tex,pos+offsets[i]).rgb;
    }
    return acc;
}
float bloomKernel[9]=float[](
    1.0,1.0,1.0,
    1.0,0.0,1.0,
    1.0,1.0,1.0
);

vec3 fisheye(in vec2 pos)
{
    vec2 spos=vec2(pos.x*fkx,pos.y*fky);
    float dist=sqrt(dot(spos,spos));
    float scal=tan(dist*finten)/tan(.5*finten);
    return vec3(normalize(pos)*scal,scal*cdist);
}

vec3 highlights(vec3 col,float t)
{
    return col*smoothstep(t-.1,t+.1,(col.r+col.g+col.b)/3.0);
}

vec3 bloom(sampler2D tex,vec2 pos)
{
    vec3 acc=vec3(0,0,0);
    vec3 col=texture(tex,pos).rgb;
    for(int i=0;i<9;i++)
    {
        acc+=bloomKernel[i]*highlights(texture(tex,pos+offsets[i]).rgb,.6);
    }
    acc/=9.0;
    return acc+(1.0-acc)*col;
}
void main()
{
    float time=float(ticks)/100;
    vec2 def=(vec2(rand(vec2(time,texCoords.y)),0)*.003-vec2(.0015,0))+vec2(sin(time+texCoords.y*640)*.001,0);
    vec3 coordsi = fisheye(texCoords+def-vec2(.5,.5))+vec3(.5,.5,0);
    for(float i=1;i<5;i++)
    {
        coordsi.x+=horizDeform(mod(time*i/100,1+sin(i-1)),coordsi.y,(i+2)*.005)*sin(tan(time*i*.01))*.005;
    }
    vec2 coords = vec2(coordsi.x,coordsi.y);
    vec3 cola = bloom(scrTex,coords).rgb;
    vec3 colb = bloom(scrTex,vec2(coords.x-coordsi.z,coords.y)).rgb;
    vec3 colc = bloom(scrTex,vec2(coords.x+coordsi.z,coords.y)).rgb;
    float contrast=1;
    FragColor = vec4(vec3((colb.r-.5)*contrast+.5,(colc.g-.5)*contrast+.5,(cola.b-.5)*contrast+.5),1.0);
}