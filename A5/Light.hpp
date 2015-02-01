#pragma once

#include "glm\glm\glm.hpp"
#include "Material.hpp"

struct Light {
	//position in view space
	glm::vec4 position;
	// ambient color
	glm::vec4 ambient;
	// diffuse color
	glm::vec4 diffuse;

	/* Coordinates in light space (scene space for scene lights) */
	inline glm::vec4 BlinnPhong(glm::vec4 pPosition, glm::vec3 pNormal, const Material* material) {
		// Calculate the ambient color
		glm::vec4 ambientColor = ambient * material->getAmbient();

		// Calculate the light direction and the strength of the diffuse light
		glm::vec3 lightDirection = glm::normalize(glm::vec3(position) - glm::vec3(pPosition));
		float diffAngle = std::fmaxf(glm::dot(lightDirection, pNormal), 0.);

		// Calculate the diffuse color
		glm::vec4 diffuseColor = diffAngle * diffuse * material->getDiffuse();

		// Calculate the specular color
		// Return the final color
		return ambientColor + diffuseColor;
	}
};