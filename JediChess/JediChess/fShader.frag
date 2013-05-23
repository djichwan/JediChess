// per-fragment interpolated values from the vertex shader
varying  vec3 fN;
varying  vec3 fL;
varying  vec3 fE;
varying  vec2 texCoord;

uniform vec4  AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4  ModelView;
uniform vec4  LightPosition;
uniform float Shininess;
uniform sampler2D Tex;
uniform int   EnableTex;
uniform mat4  Texture;
uniform float Board;
uniform float Picking;
uniform vec4  color;
uniform float useTex;

void main()
{
    vec3 N, E, L, H;
    
    if (Picking == 1.0)
    {
        gl_FragColor = color;
        return;
    }
    
    if (Board == 1.0)
    {
        N = normalize(fN);
        E = normalize(fE);
        L = normalize(fL);
        H = normalize(L + E);
        
        vec4 ambient = AmbientProduct*color;
        vec4 diffuse = max(dot(L,N),0.0)*DiffuseProduct*color;
        vec4 specular = pow(max(dot(N,H),0.0),Shininess)*SpecularProduct*vec4(1.0,1.0,1.0,1.0);
        if(dot(L,N) < 0.0)
        {
            specular = vec4(0.0,0.0,0.0,1.0);
        }
        
        vec4 c = ambient + diffuse + specular;
        c.a = 1.0;
        
        gl_FragColor = c * texture2D( Tex, texCoord );
    }
    else
    {
        // Normalize the input lighting vectors
        N = normalize(fN);
        E = normalize(fE);
        L = normalize(fL);
        
        H = normalize( L + E );
        //vec3 R = normalize(reflect(L, N));
        
        vec4 ambient = AmbientProduct;
        
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd * DiffuseProduct;
        
        if (EnableTex == 1)
        {
            diffuse *= texture2D(Tex, vec2(gl_TexCoord[0]*Texture));
            
        }
        
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        //float Ks = pow(max(dot(R, E), 0.0), Shininess);
        
        vec4 specular = Ks * SpecularProduct;
        
        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 )
        {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }
        
        gl_FragColor = ambient + diffuse + specular;
        gl_FragColor.a = 1.0;
    }
    
}//end main()