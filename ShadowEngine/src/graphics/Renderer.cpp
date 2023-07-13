#include "Renderer.h"
#include <core/Camera.h>
#include <core/Log.h>
#include <graphics/ShaderPrograms.h>
#include <graphics/Shadow.h>

void Renderer::init()
{
	if (glewInit() != GLEW_OK)
	{
		LOG_FATAL("Failed to initialize GLEW!", true);
	}

	ShaderPrograms::init();
}

void Renderer::draw(VertexArray& vertexArray, LightingMaterial material, const glm::mat4& model, bool shadowPass)
{
	Shader& shader = vertexArray.getDefaultShader();
	shader.bind();
	shader.addUniform("model", model);
	shader.addUniform("lightSpaceMatrix", Shadow::getLightSpaceMatrix());
	if (!shadowPass)
	{
		setupUniforms(shader, Camera::getPos(), Light::getDirLights(), Light::getPointLights(), material);
		shader.addUniform("view", Camera::getView());
		shader.addUniform("projection", Camera::getProjection());
		shader.addTexture(3, "depthMap");
	}

	vertexArray.bind();

	if (vertexArray.hasIndicies())
	{
		glDrawElements(GL_TRIANGLES, vertexArray.getVertexCount(), GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexArray.getVertexCount());
	}
}

void Renderer::setupUniforms(Shader& shader, glm::vec3 cameraPos, std::vector<DirLight*> dirLights, std::vector<PointLight*> pointLights, LightingMaterial material)
{
	shader.addUniform("objectColor", material.color);
	shader.addUniform("viewPosition", cameraPos);

	for (int i = 0; i < dirLights.size(); i++)
	{
		shader.addUniform("dirLights[" + std::to_string(i) + "].direction", dirLights[i]->direction);
		shader.addUniform("dirLights[" + std::to_string(i) + "].ambient", dirLights[i]->ambient);
		shader.addUniform("dirLights[" + std::to_string(i) + "].diffuse", dirLights[i]->diffuse);
		shader.addUniform("dirLights[" + std::to_string(i) + "].specular", dirLights[i]->specular);
	}

	for (int i = 0; i < pointLights.size(); i++)
	{
		shader.addUniform("pointLights[" + std::to_string(i) + "].ambient", pointLights[i]->ambient);
		shader.addUniform("pointLights[" + std::to_string(i) + "].constant", pointLights[i]->constant);
		shader.addUniform("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i]->diffuse);
		shader.addUniform("pointLights[" + std::to_string(i) + "].linear", pointLights[i]->linear);
		shader.addUniform("pointLights[" + std::to_string(i) + "].position", pointLights[i]->position);
		shader.addUniform("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i]->quadratic);
		shader.addUniform("pointLights[" + std::to_string(i) + "].specular", pointLights[i]->specular);
	}

	if (material.usesTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		material.texture->bind();
		shader.addTexture(0, "material.diffuse");
	}

	if (material.usesSpecularMap)
	{
		glActiveTexture(GL_TEXTURE1);
		material.specularMap->bind();
		shader.addTexture(1, "material.specularMap");
	}

	shader.addUniform("material.usesDiffuse", material.usesTexture);
	shader.addUniform("material.shininess", material.shininess);
	shader.addUniform("material.usesSpecularMap", material.usesSpecularMap);
}
