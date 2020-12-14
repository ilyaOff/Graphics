#version 330 core

uniform vec3 kLight = vec3(1.0,1.0,1.0);


uniform vec4 LightColorAmbient = vec4(0.3,0.3,0.3,1.0);
uniform vec4 LightColor = vec4(1,1,1,1.0);
uniform mat4 m;
uniform mat4 CameraV;
uniform float PowerGlare = 32.0f;

uniform vec4 MaterialAmbient = vec4(0.2,0.2,0.2,1.0);
uniform vec4 MaterialDiffuse = vec4(0.8,0.8,0.8,1);
uniform vec4 MaterialSurface = vec4(0.2,0.2,0.2,1.0);

in vec3 Vertex;
in vec3 normal;
in vec2 textCoor;
uniform sampler2D Map;

in vec3 lightDirection;//на источник
in vec3 e;
in vec3 r;


out vec4 color;


vec4 AmbientColor(vec4 lightColor,  vec4 material)
{	
	return  material * lightColor;	
}

vec4 DiffuseColor(vec4 lightColor, vec3 lightdir, vec3 n, vec4 material)
{
	float k = clamp(dot(n, lightdir), 0.0, 1.0);
	//float k = -clamp(dot(normal, lightdir), -1.0, 1.0);;
	return k * material * lightColor;	
}

vec4 SurfaceColor(vec4 lightColor, vec3 reflection, vec3 e, vec4 material)
{	
	float k = pow(clamp(dot(e,reflection ),0.0,1.0), PowerGlare);
	return k * material * lightColor;	
}

vec4 FongLight(vec4 DiffuselightColor, vec3 lightdir, vec4 DiffuseMaterial, 
				vec4 AmbientlightColor, vec4 AmbientMaterial, vec3 normal,
				vec4 SurfacelightColor, vec3 Reflection, vec3 e, vec4 SurfaceMaterial)
{
	vec4 diffuse = DiffuseColor(DiffuselightColor,  lightdir,  normal, DiffuseMaterial);
	vec4 ambient = AmbientColor(AmbientlightColor,  AmbientMaterial);
	vec4 surface= SurfaceColor(SurfacelightColor, Reflection,  e,  SurfaceMaterial);
 
	if (dot(lightdir, normal) <= 0.0)
	{
		surface = vec4(0.0, 0.0, 0.0, 0.0);
	}
	
	return ambient*kLight.x + diffuse * kLight.y + surface* kLight.z;
}


void main(void) {
	
	//vec4 mat = texture2D(Map, textCoor.xy);
	vec3 n =  (texture2D(Map, textCoor.xy).rgb - vec3(0.5f,0.5f,0.5f))*2;
	n = normalize(n);
	//vec4 nn = m*vec4(n,1);
	//n = normalize(nn.xyz/nn.w);
	vec3 reflect = normalize(reflect(lightDirection,n));

	color = FongLight(LightColor,normalize(lightDirection),MaterialDiffuse,	
						LightColorAmbient, MaterialAmbient, n,
						LightColor, reflect, normalize(e), MaterialSurface);
}