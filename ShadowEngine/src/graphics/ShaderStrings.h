#pragma once

const char* LIGHTING_VERT = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 vertNormal;
out vec3 vertPos;
out vec2 vertTexCoords;
out vec4 vertPosLightSpace;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
	vertPos = vec3(model * vec4(aPos, 1.0f));
	vertNormal = mat3(transpose(inverse(model))) * aNormal;
	vertTexCoords = aTexCoords;
	vertPosLightSpace = lightSpaceMatrix * vec4(vertPos, 1.0);
}

)";

const char* LIGHTING_FRAG = R"(
#version 330 core

in vec3 vertNormal;
in vec3 vertPos;
in vec2 vertTexCoords;
in vec4 vertPosLightSpace;

uniform vec4 objectColor;
uniform vec3 viewPosition;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NUM_DIR_LIGHTS 2
uniform DirLight dirLights[NUM_DIR_LIGHTS];

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NUM_POINT_LIGHTS 5
uniform PointLight pointLights[NUM_POINT_LIGHTS];

struct Material
{
	sampler2D diffuse;
	bool usesDiffuse;

	float shininess;

	sampler2D specularMap;
	bool usesSpecularMap;
};
uniform Material material;

uniform sampler2D depthMap;

// output
out vec4 fragColor;

// funcs
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow, vec3 materialSpecular);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 materialSpecular);
float calcShadow(vec4 vertPosLightSpace);

void main()
{
	vec3 norm = normalize(vertNormal);
	vec3 viewDir = normalize(viewPosition - vertPos);
	float shadow = calcShadow(vertPosLightSpace);

	// material diffuse
	vec3 materialDiffuse;
	if (material.usesDiffuse)
	{
		materialDiffuse = vec3(texture(material.diffuse, vertTexCoords));
	}
	else
	{
		materialDiffuse = vec3(1.0);
	}
	vec3 materialSpecular;
	if (material.usesSpecularMap)
	{
		materialSpecular = vec3(texture(material.specularMap, vertTexCoords));
	}
	else
	{
		materialSpecular = vec3(1.0);
	}

	// directional lights
	vec3 result = vec3(0);
	for (int i = 0; i < NUM_DIR_LIGHTS; i++)
	{
		if (dirLights[i].ambient.xyz == vec3(0))
		{
			break;
		}
		result += calcDirLight(dirLights[i], norm, viewDir, shadow, materialSpecular);
	}

	// point lights
	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		if (pointLights[i].constant == 0)
		{
			break;
		}
		result += calcPointLight(pointLights[i], norm, vertPos, viewDir, materialSpecular);
	}

	if (result.x > 1.0)
	{
		result.x = 1.0;
	}
	if (result.y > 1.0)
	{
		result.y = 1.0;
	}
	if (result.z > 1.0)
	{
		result.z = 1.0;
	}
	
	result *= materialDiffuse;

	fragColor = vec4(result, 1.0) * objectColor;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow, vec3 materialSpecular)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	// combine them
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec * materialSpecular;

	return (ambient + (1.0 - shadow) * diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 materialSpecular)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combien them
	vec3 ambient = light.ambient;
	vec3 diffuse = light.diffuse * diff;
	vec3 specular = light.specular * spec * materialSpecular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	return (ambient + diffuse + specular);
}

float calcShadow(vec4 vertPosLightSpace)
{
	// perform perspective divide
    vec3 projCoords = vertPosLightSpace.xyz / vertPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // make the shadow itself
    float bias = max(0.00001 * (1.0 - dot(vertNormal, dirLights[0].direction)), 0.005); // TODO for multiple lights
	float shadow = 0.0;
	// for more smooth
	vec2 texelSize = 1.0 / textureSize(depthMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	// test if outside range
	if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

)";

const char* LIGHTING_DEPTH_VERT = R"(
	#version 330 core
	layout (location = 0) in vec3 aPos;

	uniform mat4 lightSpaceMatrix;
	uniform mat4 model;

	void main()
	{
		gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
	}  

)";

const char* LIGHTING_DEPTH_FRAG = R"(
	#version 330 core
	
	void main()
	{             
	}  
)";

const char* CUBEMAP_VERT = R"(
	#version 330 core

	layout (location = 0) in vec3 aPos;

	uniform mat4 view;
	uniform mat4 projection;

	out vec3 vertTexCoords;

	void main()
	{
		vec4 pos = projection * view * vec4(aPos, 1.0);
		gl_Position = pos.xyww;
		vertTexCoords = aPos;
	}
)";

const char* CUBEMAP_FRAG = R"(
	#version 330 core

	in vec3 vertTexCoords;

	uniform samplerCube cubeMap;

	out vec4 fragColor;

	void main()
	{
		fragColor = texture(cubeMap, vertTexCoords);
	}
)";

const char* MODEL_VERT = R"(
	#version 430 core

	layout(location = 0) in vec3 aPosition;
	layout(location = 1) in vec3 aNormal;
	layout(location = 2) in vec2 aTexCoords;
	layout(location = 3) in ivec4 aBoneIds;     // INT pointer
	layout(location = 4) in vec4 aWeights;

	out vec2 vertTexCoords;
	out vec3 vertNormal;
	out vec3 vertPos;
	out vec4 vertPosLightSpace;

	uniform mat4 mvp;
	uniform mat4 model;
	uniform mat4 normalsMatrix;
	uniform mat4 lightSpaceMatrix;

	const int MAX_BONES = 100;
	uniform mat4 bones[MAX_BONES];

	void main()
	{
		mat4 boneTransform = bones[aBoneIds[0]] * aWeights[0];
			boneTransform += bones[aBoneIds[1]] * aWeights[1];
			boneTransform += bones[aBoneIds[2]] * aWeights[2];
			boneTransform += bones[aBoneIds[3]] * aWeights[3];
			
		vec4 bonedPosition = boneTransform * vec4(aPosition, 1.0); // transformed by bones

		vertNormal = normalize(vec3(normalsMatrix * (boneTransform * vec4(aNormal, 0.0))));

		vertPos = vec3(model * bonedPosition);
		vertTexCoords = aTexCoords;
		
		vertPosLightSpace = lightSpaceMatrix * vec4(vertPos, 1.0);

		gl_Position = mvp * bonedPosition;
	}
)";

const char* MODEL_FRAG = R"(
	#version 430 core

	in vec2 vertTexCoords;
	in vec3 vertPos;
	in vec3 vertNormal;
	in vec4 vertPosLightSpace;

	struct Material
	{
		sampler2D texture_diffuse1;
		bool usesDiffuse;

		sampler2D texture_specular1;
		bool usesSpecularMap;

		float shininess;
	};
	uniform Material material;

	struct DirLight
	{
		vec3 direction;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};
	#define NUM_DIR_LIGHTS 2
	uniform DirLight dirLights[NUM_DIR_LIGHTS];

	struct PointLight
	{
		vec3 position;

		float constant;
		float linear;
		float quadratic;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};
	#define NUM_POINT_LIGHTS 5
	uniform PointLight pointLights[NUM_POINT_LIGHTS];

	uniform vec3 viewPos;
	uniform sampler2D depthMap;

	out vec4 fragColor;

	// funcs
	vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow, vec3 materialSpecular);
	vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 materialSpecular);
	float calcShadow(vec4 vertPosLightSpace);

	void main()
	{
		vec3 norm = normalize(vertNormal);
		vec3 viewDir = normalize(viewPos - vertPos);
		float shadow = calcShadow(vertPosLightSpace);

		// textures
		vec3 materialDiffuse;
		if (material.usesDiffuse)
		{
			materialDiffuse = vec3(texture(material.texture_diffuse1, vertTexCoords));
		}
		else
		{
			materialDiffuse = vec3(1.0);
		}
		vec3 materialSpecular;
		if (material.usesSpecularMap)
		{
			materialSpecular = vec3(texture(material.texture_specular1, vertTexCoords));
		}
		else
		{
			materialSpecular = vec3(1.0);
		}
		

		// directional lights
		vec3 result = vec3(0);
		for (int i = 0; i < NUM_DIR_LIGHTS; i++)
		{
			if (dirLights[i].ambient.xyz == vec3(0))
			{
				break;
			}
			result += calcDirLight(dirLights[i], norm, viewDir, shadow, materialSpecular);
		}

		// point lights
		for (int i = 0; i < NUM_POINT_LIGHTS; i++)
		{
			if (pointLights[i].constant == 0)
			{
				break;
			}

			result += calcPointLight(pointLights[i], norm, vertPos, viewDir, materialSpecular);
		}

		if (result.x > 1.0)
		{
			result.x = 1.0;
		}
		if (result.y > 1.0)
		{
			result.y = 1.0;
		}
		if (result.z > 1.0)
		{
			result.z = 1.0;
		}

		result *= materialDiffuse;

		fragColor = vec4(result, 1.0);
	}

	vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shadow, vec3 materialSpecular)
	{
		vec3 lightDir = normalize(-light.direction);
		// diffuse
		float diff = max(dot(normal, lightDir), 0.0);
		// specular
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
		// combine them
		vec3 ambient = light.ambient;
		vec3 diffuse = light.diffuse * diff;
		vec3 specular = light.specular * spec * materialSpecular;
		
		return (ambient + (1.0 - shadow) * diffuse + specular);
	}

	vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 materialSpecular)
	{
		vec3 lightDir = normalize(light.position - fragPos);
		// diffuse
		float diff = max(dot(normal, lightDir), 0.0);
		// specular
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
		// attenuation
		float distance = length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		// combine them
		vec3 ambient = light.ambient;
		vec3 diffuse = light.diffuse * diff;
		vec3 specular = light.specular * spec * materialSpecular;

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
	
		return (ambient + diffuse + specular);
	}

	float calcShadow(vec4 vertPosLightSpace)
	{
		// do nothing if no dir light
		if (dirLights[0].ambient.xyz == vec3(0))
		{
			return 0.0;
		}
		
		// perform perspective divide
		vec3 projCoords = vertPosLightSpace.xyz / vertPosLightSpace.w;
		// transform to [0,1] range
		projCoords = projCoords * 0.5 + 0.5;
		// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
		float closestDepth = texture(depthMap, projCoords.xy).r; 
		// get depth of current fragment from light's perspective
		float currentDepth = projCoords.z;
		// make the shadow itself
		float bias = max(0.00001 * (1.0 - dot(vertNormal, dirLights[0].direction)), 0.005); // TODO for multiple lights
		float shadow = 0.0;
		// for more smooth
		vec2 texelSize = 1.0 / textureSize(depthMap, 0);
		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float pcfDepth = texture(depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
			}    
		}
		shadow /= 9.0;
		// test if outside range
		if(projCoords.z > 1.0)
			shadow = 0.0;
		return shadow;
	}


)";

const char* MODEL_DEPTH_VERT = R"(
	#version 430 core

	layout(location = 0) in vec3 aPosition;
	layout(location = 3) in ivec4 aBoneIds;     // INT pointer
	layout(location = 4) in vec4 aWeights;

	out vec3 vertPos;

	uniform mat4 mvp;
	uniform mat4 model;
	uniform mat4 normalsMatrix;
	uniform mat4 lightSpaceMatrix;

	const int MAX_BONES = 100;
	uniform mat4 bones[MAX_BONES];

	void main()
	{
		mat4 boneTransform = bones[aBoneIds[0]] * aWeights[0];
			boneTransform += bones[aBoneIds[1]] * aWeights[1];
			boneTransform += bones[aBoneIds[2]] * aWeights[2];
			boneTransform += bones[aBoneIds[3]] * aWeights[3];
			
		vec4 bonedPosition = boneTransform * vec4(aPosition, 1.0); // transformed by bones

		vertPos = vec3(model * bonedPosition);

		gl_Position = lightSpaceMatrix * model * bonedPosition;
	}
)";

const char* MODEL_DEPTH_FRAG = R"(
	#version 330 core
	
	void main()
	{             
	}
)";

const char* SHADOW_DEBUG_VERT = R"(
#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTexCoords;

	out vec2 vertTexCoords;

	void main()
	{
		vertTexCoords = aTexCoords;
		gl_Position = vec4(aPos, 1.0);
	}
)";


const char* SHADOW_DEBUG_FRAG = R"(
	#version 330 core
	out vec4 FragColor;

	in vec2 vertTexCoords;

	uniform sampler2D depthMap;
	uniform float near_plane;
	uniform float far_plane;

	void main()
	{             
		float depthValue = texture(depthMap, vertTexCoords).r;
		FragColor = vec4(vec3(depthValue), 1.0);
	}
)";