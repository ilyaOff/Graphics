#version 330 core

uniform vec3 kLight = vec3(1.0,1.0,1.0);


uniform vec4 LightColorAmbient = vec4(0.3,0.3,0.3,1.0);
uniform vec4 LightColor = vec4(1,1,1,1.0);

uniform float PowerGlare = 32.0f;

uniform vec4 MaterialAmbient = vec4(0.2,0.2,0.2,1.0);
uniform vec4 MaterialDiffuse = vec4(0.8,0.8,0.8,1);
uniform vec4 MaterialSurface = vec4(0.2,0.2,0.2,1.0);

in vec3 Vertex;
in vec3 normal;
in vec2 textCoor;
//in vec2 textCoorH;
uniform sampler2D Map;
uniform sampler2D Map2;

in vec3 lightDirectionT;//�� ��������
in vec3 et;



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

uniform float scale = 0.2f;
uniform float bias = 0.005f;
void main(void) {
	
	//vec3 et = vec3(,,dot(n,e));
	float h = bias + scale*(texture2D(Map2, textCoor).r -0.5f);
	vec2 T1 = textCoor+ h*et.xy/ et.z;
	
	vec3 n =  (texture2D(Map,T1).rgb - vec3(0.5f,0.5f,0.5f))*2;
	n = normalize(n);
	
	vec3 R = normalize(reflect(lightDirectionT,n));

	color = FongLight(LightColor,normalize(lightDirectionT),MaterialDiffuse,	
						LightColorAmbient, MaterialAmbient, n,
					LightColor, R, normalize(et), MaterialSurface);
}