#version 330 core

uniform vec3 kLight = vec3(1.0,1.0,1.0);

uniform vec4 LightColorAmbient = vec4(0.3,0.3,0.3,1.0);
uniform vec4 LightColor = vec4(0.9,0.9,0.9,1.0);
uniform vec3 LightDir = normalize(vec3(0.5,0.5,0.5));//на источник

//uniform vec4 MaterialAmbient = vec4(0.2,0.2,0.2,1.0);
//uniform vec4 MaterialDiffuse = vec4(1,1,0.5,0);
//uniform vec4 MaterialSurface = vec4(1,1,0,0);

in vec3 pyramidVertex;
in vec3 normal;


out vec4 color;


vec4 AmbientColor(vec4 lightColor,  vec4 material)
{	
	return  material * lightColor;	
}

vec4 DiffuseColor(vec4 lightColor, vec3 lightdir, vec4 material)
{
	float k = clamp(dot(normal, lightdir), 0.0, 1.0);
	return k * material * lightColor;	
}
/*
vec4 SurfaceColor(vec4 lightColor, vec3 lightdir, vec4 material)
{
	float k = clamp(dot(normal, lightdir), 0.0, 1.0);
	return k * material * lightColor;	
}
*/
vec4 FongLight(vec4 DiffuselightColor, vec3 lightdir, vec4 DiffuseMaterial, 
				vec4 AmbientlightColor, vec4 AmbientMaterial)
{
	vec4 diffuse = DiffuseColor(DiffuselightColor,  lightdir, DiffuseMaterial);
	vec4 ambient = AmbientColor(AmbientlightColor,  AmbientMaterial);

	return ambient*kLight.x + diffuse * kLight.y;
}


void main(void) {

	vec4 mat =  vec4(pyramidVertex + vec3(0.5,0.5,0.5),0);
	color = FongLight(LightColor,LightDir, mat,	
						LightColorAmbient, mat);
}