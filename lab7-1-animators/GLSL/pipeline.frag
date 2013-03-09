varying vec3 n;
varying vec3 v;

uniform sampler2D texDiffuse0;

void main(void)
{
    vec4 result = vec4(0.0);
    for (int li = 0; li < gl_MaxLights; ++li)
    {
        vec3 l;
        if (gl_LightSource[li].position.w != 0.0)
        {
            l = normalize(gl_LightSource[li].position.xyz - v);
        }
        else
        {
            l = normalize(gl_LightSource[li].position.xyz);
        }
        vec3 e = normalize(-v);
        vec3 r = normalize(-reflect(l, n));
 
        vec4 Iamb = gl_FrontLightProduct[li].ambient;
 
        vec4 Idiff = gl_FrontLightProduct[li].diffuse * max(dot(n, l), 0.0);
        Idiff = clamp(Idiff, 0.0, 1.0);
        Idiff *= texture2D(texDiffuse0, gl_TexCoord[0]);
 
        vec4 Ispec = gl_FrontLightProduct[li].specular
                     * pow(max(dot(r, e), 0.0),
                           gl_FrontMaterial.shininess);
        Ispec = clamp(Ispec, 0.0, 1.0);
 
        result += Iamb + Idiff + Ispec;
    }
 
    gl_FragColor = gl_FrontLightModelProduct.sceneColor + result;
} 
