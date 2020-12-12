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
	vec4 light = CameraRotation * vec4(LightDir, 1);//!!!
	lightDirection = light.xyz / light.w;//!!
	
	vec4 pos =	 m * vec4(modelPos, 1) ;	
	//e = pos.xyz / pos.w ;//подвижный блик	
	//e = vec3(CameraRotation* pos);// круглый блик !!!//темнее
	//e = vec3(pos) - Vec;//светлее !!!!!!!!!!!!!!!!!!!!оставьть эту

	normal = normalize(nm * modelNormal);
	//normal = normalize(mat3(m)* modelPos);
	
	pos = CameraRotation*CameraPosition*pos;
	e = pos.xyz/pos.z;
	light = CameraRotation * CameraPosition* vec4(LightDir, 1);
	lightDirection  = normalize(light.xyz/light.w - (pos.xyz / pos.w));
	
	//r =  2*dot(LightDir, normal)-LightDir;
	//r = (2*dot(lightDirection, normal)*normal-lightDirection);//!!
	r = reflect(-lightDirection,normal );
	gl_Position = mvp * vec4(modelPos, 1.0);
}