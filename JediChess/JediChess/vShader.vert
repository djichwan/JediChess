attribute vec4 vPosition;
attribute vec3 vNormal;
attribute  vec4 vColor;
attribute  vec2 vTexCoord;

varying vec3 fN; //normal at current position
varying vec3 fV; //vector from point to viewer
varying vec3 fL; //vector from point to light
varying vec4 fColor;
varying vec2 texCoord;
varying float fambientCoefficient;
varying float fdiffuseCoefficient;
varying float fspecularCoefficient;
varying float fshininess;

uniform mat4 cMw;
uniform mat4 wMo;
uniform mat4 proj;
uniform mat4 modelView;

uniform vec4 cameraPosition;
uniform vec4 lightPosition;
uniform vec4 Color;

uniform float ambientCoefficient;
uniform float diffuseCoefficient;
uniform float specularCoefficient;
uniform float shininess;
uniform float picking;

void main()
{
    texCoord = vTexCoord;
    
    gl_Position = proj*cMw*wMo*vPosition;
    
    fN = (wMo*vec4(vNormal.x,vNormal.y,vNormal.z,0.0)).xyz;
    fV = (cameraPosition - wMo*vPosition).xyz;
    fL = (lightPosition - wMo*vPosition).xyz;
    
    fColor = vColor;
    fambientCoefficient = ambientCoefficient;
    fdiffuseCoefficient = diffuseCoefficient;
    fspecularCoefficient = specularCoefficient;
    fshininess = shininess;
}