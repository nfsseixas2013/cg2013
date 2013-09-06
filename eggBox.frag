// minimal fragment shader
varying vec3 r;
uniform sampler2D envMapText;
//varying vec4 fcolor;
varying vec3 Q;
varying vec3 w;   
////////////////////Fresnel-----------------------------

///////////////////////////////////////////////////////////////

void main() {
	
	vec3 L = normalize(gl_LightSource[0].position.xyz - w);   
   vec3 E = normalize(-w); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 R = normalize(-reflect(L,Q));  
	
	float m = 1;
	
	vec2 coord = vec2(r.x +10 , r.y +10  );
	
	vec4 texel = texture2D(envMapText, coord.st);
	
	//calculate Ambient Term:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient;    

   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(Q,L), 0.0);    
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);

//---------------fresne----------------------------------------------



/////////////////////////////////////////////////////////////////////


   // write Total Color:  
    gl_FragColor =  texel + gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec; 
	//gl_FragColor = texel;
	//gl_FragColor = vec4(color, 1.0);
}
