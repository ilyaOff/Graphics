#version 330 core

uniform mat4 mvp;
uniform mat4 m;
uniform mat4 CameraV;
uniform mat3 nm;
uniform vec3 LightPos;// точечный источник

in vec3 modelPos;
in vec3 modelNormal;
in vec2 modelTexCoor;

out vec3 Vertex;
out vec3 normal;
out vec2 textCoor;

out vec3 et;

out vec3 lightDirectionT;


void main() {
	Vertex = modelPos;	
	textCoor = modelTexCoor;

	normal = normalize(nm * modelNormal);

	/*
	vec3 t = vec3(modelNormal.y, -modelNormal.x, 0);
	vec3 b = vec3(0, -modelNormal.z, modelNormal.y);
	b = b- dot(b,t)*t;
	
	*/
	// n = (0,1,0)
	vec3 t = vec3(1, 0, 0);
	vec3 b = vec3(0, 0, 1);
	t = normalize(nm * t);
	b = normalize(nm * b);

	vec4 pos = CameraV* m * vec4(modelPos, 1);
	et = normalize(-pos.xyz/pos.z);

	vec4 light = CameraV* vec4(LightPos, 1);
	lightDirectionT  = normalize(light.xyz/light.w - (pos.xyz / pos.w));	
	
	et = normalize(vec3( dot ( et, t ), dot ( et, b), dot ( et, normal ) ));
	lightDirectionT = normalize(vec3( dot ( lightDirectionT, t ), dot ( lightDirectionT, b ), dot ( lightDirectionT, normal ) ));

	gl_Position = mvp * vec4(modelPos, 1.0);
}