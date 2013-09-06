// eggBox vertex shader

varying vec4 fcolor;

uniform float tempo;
uniform float a;
uniform float a2;

void main() {	
	
	vec4 v = gl_Vertex;
	
		
		
	v.y = a/8.0 * (sin((v.x+tempo/3.0)) + sin((v.z+tempo/3.0)))+a/5.0 * (sin((v.x+tempo)) + sin((v.z+tempo)))+
a/3.0 * (sin((v.x+tempo/10.0)) + sin((v.y+tempo/7.0)))+a/5.0 * (sin((v.x+tempo/2.0)) + sin((v.y+tempo/2.0)));
		
	gl_Position = gl_ModelViewProjectionMatrix * v;		
	
	fcolor = vec4(0, 0, 1.0, 1.0);
	
	
	
	
}
