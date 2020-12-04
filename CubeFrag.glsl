#version 330 core


uniform vec3 kLight = vec3(1.0,1.0,1.0);


//uniform vec4 LightAmbient = vec4(0.9,0.9,0.9,1.0);
uniform vec4 LightColor = vec4(0.9,0.9,0.9,1.0);
uniform vec3 LightDir = vec3(0,0.5,0.5);//на источник

uniform vec4 MaterialDiffuse = vec4(1,1,0.5,0);

in vec3 cubeVertex;
in vec3 normal;


out vec4 color;

vec4 DiffuseColor(vec4 lightColor, vec3 lightdir)
{
	float k = clamp(dot(normal, lightdir), 0.0, 1.0);
	//return k * MaterialDiffuse * lightColor;
	return k *  lightColor *vec4(cubeVertex + vec3(0.5,0.5,0.5),0);
}
vec4 FongLight(vec4 lightColor, vec3 lightdir)
{
	vec4 diffuse = DiffuseColor(lightColor,  lightdir);
	
	return diffuse * kLight.y;
}


void main(void) {
	//color = vec4(cubeVertex + vec3(0.5,0.5,0.5),0);
	//color = vec4(1,1,0,0);
	 
	color = FongLight(LightColor, LightDir);
}