#version 330 core

in vec3 pyramidVertex;

out vec4 color;

void main(void) {
	color = vec4(pyramidVertex + vec3(0.5,0.5,0.5),0);
	//color = vec4(1,1,1,0);
}