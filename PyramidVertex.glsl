#version 330 core

in vec3 modelPos;
out vec3 pyramidVertex;
uniform mat4 mvp2;

void main() {
	pyramidVertex = modelPos;
	gl_Position = mvp2 * vec4(modelPos, 1.0);
}