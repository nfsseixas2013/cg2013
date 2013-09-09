
varying vec3 r;
uniform float tempo;
uniform float ampl;
uniform int control;


varying vec3 Q;
varying vec3 w;

void main() {	
	
	w = vec3(gl_ModelViewMatrix * gl_Vertex);       
    Q = normalize(gl_NormalMatrix * gl_Normal);
	
	vec4 v = gl_Vertex;
	if ( control == 1){
		v.y = ampl/8.0 * (sin((v.z + tempo/2)) + sin((v.z+tempo/3))) + ampl/15.0 * (sin((v.x + tempo*2)) + sin((v.x+tempo)));
	}
	else{
		if ( control == 2 ){
			v.y = ampl/30.0 * (sin((v.z + tempo/2.0)) + sin((v.z+tempo/3.0))) + ampl/20.0 * (sin((v.x + tempo/2.0)) + sin((v.x+tempo)));
		}
		else{
			v.y = ampl/8.0 * (sin((v.z + tempo/2)) + sin((v.z+tempo/3))) + ampl/6.0 * (sin((v.x + tempo*2)) + sin((v.x+tempo)));
		}
	}
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec3 j = normalize(vec3(gl_ModelViewMatrix * gl_Vertex));
	vec3 n = normalize(vec3(gl_NormalMatrix * gl_Normal));
	r = reflect(j, n);	
	gl_Position = gl_ModelViewProjectionMatrix * v ;
}
