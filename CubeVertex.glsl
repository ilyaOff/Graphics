#version 330 core


uniform mat4 mvp;
uniform mat4 m;
uniform mat4 Camera;
uniform mat3 nm;
uniform vec3 LightPos;//солнце

in vec3 modelPos;
in vec3 modelNormal;

out vec3 Vertex;
out vec3 normal;
out vec3 e;
out vec3 r;
out vec3 lightDirection;


void main() {
	Vertex = modelPos;
	//lightDirection = //vec3(mv * vec4(LightDir, 1));//LightDir; //нормализован до шейдера
	lightDirection = LightDir; //нормализован до шейдера
	
	vec4 pos =	 m * vec4(modelPos, 1);	
	e = pos.xyz / pos.w;
	
	
	normal = normalize(nm * modelNormal);
	//normal = normalize(mat3(m)* modelPos);

	
	//r =  2*dot(LightDir, normal)-LightDir;
	r = normalize(  2*dot(LightDir, normal)-LightDir);//???


	gl_Position = mvp * vec4(modelPos, 1.0);
}