#version 330 core
//Фонг, точечный источник

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 CameraV;
uniform mat3 nm;
uniform vec3 LightPos;// точечный источник


in vec3 modelPos;
in vec3 modelNormal;

out vec3 Vertex;
out vec3 normal;
out vec3 e;
out vec3 r;
out vec3 lightDirection;


void main() {
	Vertex = modelPos;	

	normal = normalize(nm * modelNormal);
	
	vec4 pos = CameraV* m * vec4(modelPos, 1);
	e = normalize(-pos.xyz/pos.z);

	vec4 light = CameraV* vec4(LightPos, 1);
	lightDirection  = normalize(light.xyz/light.w - (pos.xyz / pos.w));	
	
	r = reflect(-lightDirection,normal );

	gl_Position = mvp * vec4(modelPos, 1.0);
}