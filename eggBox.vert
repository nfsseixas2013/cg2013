// eggBox vertex shader

varying vec4 fcolor;
varying vec3 r;
uniform float tempo;
uniform float a;
uniform float a2;



//-------------fresnel-------------------------

//////////////////////////////////////////////






varying vec3 Q;
varying vec3 w;

void main() {	
	
	w = vec3(gl_ModelViewMatrix * gl_Vertex);       
    Q = normalize(gl_NormalMatrix * gl_Normal);
	
	vec4 v = gl_Vertex;
	
	v.y = a/8.0 * (sin((v.z + tempo/2)) + sin((v.z+tempo/3))) + a/15.0 * (sin((v.x + tempo*2)) + sin((v.x+tempo)));

	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec3 j = normalize(vec3(gl_ModelViewMatrix * gl_Vertex));
	
	vec3 n = normalize(vec3(gl_NormalMatrix * gl_Normal));
	
	r = reflect(j, n);
	
	//--------------------fresnel---------------------------------

	////////////////////////////////////////////////////////////

	gl_Position = gl_ModelViewProjectionMatrix * v ;
	
}
