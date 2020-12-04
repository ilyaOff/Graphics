#version 330 core

uniform mat4 mvp;
uniform mat4 mv;
//uniform mat3 nm;


in vec3 modelPos;
//in vec3 modelNormal;

out vec3 pyramidVertex;
out vec3 normal;

void main() {
	pyramidVertex = modelPos;
	//normal = modelPos;
	normal = normalize(mat3(mv)* modelPos);

	gl_Position = mvp * vec4(modelPos, 1.0);
}
