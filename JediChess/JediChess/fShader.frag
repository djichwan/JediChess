varying vec4 fColor;
varying vec3 fN;
varying vec3 fL;
varying vec3 fV;
varying vec2 texCoord;

uniform sampler2D texture;

uniform float picking;
varying float fambientCoefficient;
varying float fdiffuseCoefficient;
varying float fspecularCoefficient;
varying float fshininess;

void main()
{
    if (picking == 0.0)
    {
        //Smooth Shading
        vec3 N,V,L,H;
        
        N = normalize(fN);
        V = normalize(fV);
        L = normalize(fL);
        H = normalize(L + V);
        
        vec4 ambient = fambientCoefficient*fColor;
        vec4 diffuse = max(dot(L,N),0.0)*fdiffuseCoefficient*fColor;
        vec4 specular = pow(max(dot(N,H),0.0),fshininess)*fspecularCoefficient*vec4(1.0,1.0,1.0,1.0);
        if(dot(L,N) < 0.0)
        {
            specular = vec4(0.0,0.0,0.0,1.0);
        }
        
        vec4 c = ambient + diffuse + specular;
        c.a = 1.0;
        gl_FragColor = c * texture2D( texture, texCoord );
        
        
        // TODO: for now
        //gl_FragColor = fColor * texture2D( texture, texCoord );
    }
    else
        gl_FragColor = fColor;
}