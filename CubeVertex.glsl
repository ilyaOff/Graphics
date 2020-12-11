#version 330 core


uniform mat4 mvp;
uniform mat4 m;
uniform mat4 CameraPosition;
uniform mat4 CameraRotation;
uniform mat3 nm;
//uniform vec3 LightPos;//солнце
uniform vec3 LightDir;//солнце
uniform vec3 Vec;

in vec3 modelPos;
in vec3 modelNormal;

out vec3 Vertex;
out vec3 normal;
out vec3 e;
out vec3 r;
out vec3 lightDirection;


void main() {
	Vertex = modelPos;	
	vec4 light = CameraRotation * vec4(LightDir, 1);
	lightDirection = light.xyz / light.w;
	
	vec4 pos =	 m * vec4(modelPos, 1) ;	
	e = pos.xyz / pos.w ;
	//e = vec3(CameraRotation* pos);
	
	normal = normalize(nm * modelNormal);
	//normal = normalize(mat3(m)* modelPos);

	
	//r =  2*dot(LightDir, normal)-LightDir;
	r = (2*dot(lightDirection, normal)-lightDirection);
	gl_Position = mvp * vec4(modelPos, 1.0);
}