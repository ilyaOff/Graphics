#version 330 core

uniform vec3 kLight = vec3(0.0,0.0,1.0);


uniform vec4 LightColorAmbient = vec4(0.3,0.3,0.3,1.0);
uniform vec4 LightColor = vec4(1,1,1,1.0);

uniform vec4 MaterialAmbient = vec4(0.2,0.2,0.2,1.0);
uniform vec4 MaterialDiffuse = vec4(0.8,0.8,0.8,1);
uniform vec4 MaterialSurface = vec4(0.8,0.8,0.8,1);

uniform float Roughness = 0.9f;//[0,1]//шероховатость


in vec3 Vertex;
in vec3 normal;
in vec3 lightDirection;//на источник
in vec3 e;
//in vec3 r;


out vec4 color;

float Geometry( float dotNE,float dotEH, float dotNH, float dotNL )
{
	float k = min(dotNE, dotNL);
	k = 2*k*dotNH;	

	if(dotEH < k)
		return 1;

	k = k/dotEH;
	return k;
}


float Beksman(float dotNH)
{
	float k = dotNH*dotNH;
	float tg2 = 1/k - 1;
	float m2 = Roughness * Roughness;
	k = k*k*m2;
	tg2 = tg2/m2;

	k = k* exp(-tg2);
	return k;
}


vec4 SurfaceColor(vec3 lightdir, vec3 n, vec3 e,vec4 lightColor, vec4 material)
{	
	vec3 h = normalize(lightdir+e);

	float dotEH =  dot(h, e);
	float dotNH =  dot(n, h);

	//if(dotNH == 0)
	//	dotNH = 1.0e-7;

	float dotNE =  dot(n, e);	
	float dotNL =  dot(lightdir, n);
	float g = Geometry(dotNE, dotEH, dotNH, dotNL);
	float b = Beksman(dotNH);

	float kNE =(1 - dotNE);
	kNE = kNE*kNE*(1 - dotNE);
	vec4 resmat = material - (vec4(1,1,1,1) - material)*kNE;

	resmat = resmat * g * b / (4*dotNE*dotNL);
	return resmat;
}



vec4 AmbientColor(vec4 lightColor,  vec4 material)
{	
	return  material * lightColor;	
}

vec4 DiffuseColor(vec4 lightColor, vec3 lightdir, vec3 n, vec4 material)
{
	float k = clamp(dot(n, lightdir), 0.0, 1.0);	
	return k * material * lightColor;	
}


vec4 Light(vec4 DiffuselightColor, vec3 lightdir, 
				vec3 Normal, vec4 DiffuseMaterial, 
				vec4 AmbientlightColor, vec4 AmbientMaterial,
				vec4 SurfacelightColor, vec3 e, vec4 SurfaceMaterial)
{
	vec4 diffuse = DiffuseColor(DiffuselightColor,  lightdir, Normal, DiffuseMaterial);
	vec4 ambient = AmbientColor(AmbientlightColor,  AmbientMaterial);
	vec4 surface = SurfaceColor(lightdir, Normal,  e, SurfacelightColor, SurfaceMaterial);
 
	//if (dot(lightdir, Normal) <= 0.0)
	{
	//	surface = vec4(0.0, 0.0, 0.0, 0.0);
	}
	
	return ambient*kLight.x + diffuse * kLight.y + surface* kLight.z;
}


void main(void) {
	//color = vec4(Vertex + vec3(0.5,0.5,0.5),0);
	//color = vec4(1,1,0,0);

	//vec4 mat =  vec4(Vertex + vec3(0.5,0.5,0.5),0);
	//color = vec4(normal + vec3(0.5,0.5,0.5), 1);
	color = Light(LightColor,normalize(lightDirection),
						normalize(normal), MaterialDiffuse,	
						LightColorAmbient, MaterialAmbient,
						LightColor,  normalize(e), MaterialSurface);
}