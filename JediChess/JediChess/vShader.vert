attribute vec4 vPosition;
attribute vec3 vNormal;

varying vec3 fN; //normal at current position
varying vec3 fV; //vector from point to viewer
varying vec3 fL; //vector from point to light
varying vec4 fColor;
varying float fambientCoefficient;
varying float fdiffuseCoefficient;
varying float fspecularCoefficient;
varying float fshininess;

uniform mat4 cMw;
uniform mat4 wMo;
uniform mat4 proj;

uniform vec4 cameraPosition;
uniform vec4 lightPosition;
uniform vec4 Color;

uniform float ambientCoefficient;
uniform float diffuseCoefficient;
uniform float specularCoefficient;
uniform float shininess;


void main()
{
    gl_Position = proj*cMw*wMo*vPosition;
    
    fN = (wMo*vec4(vNormal.x,vNormal.y,vNormal.z,0.0)).xyz;
    fV = (cameraPosition - wMo*vPosition).xyz;
    fL = (lightPosition - wMo*vPosition).xyz;
   
    fColor = Color;
    fambientCoefficient = ambientCoefficient;
    fdiffuseCoefficient = diffuseCoefficient;
    fspecularCoefficient = specularCoefficient;
    fshininess = shininess;
}
