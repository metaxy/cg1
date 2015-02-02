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
	// specular color
	glm::vec4 specular;

	/* Coordinates in light space (scene space for scene lights) */
	inline glm::vec4 BlinnPhong(glm::vec4 eye, glm::vec4 pPosition, glm::vec3 pNormal, const Material* material) {
		// Calculate the light direction and the strength of the diffuse light
		glm::vec3 lightDir = glm::normalize(glm::vec3(position) - glm::vec3(pPosition));
		glm::vec3 viewDir = glm::normalize(glm::vec3(eye) - glm::vec3(pPosition));
		
		// Calculate the ambient color
		glm::vec4 ambientColor = ambient * material->getAmbient();

		
		// Calculate the diffuse color
		float diffAngle = std::fmaxf(glm::dot(lightDir, pNormal), 0.);
		glm::vec4 diffuseColor = diffAngle * diffuse * material->getDiffuse();

		// If the diffuse intensity is more than 0 calculate the specular component
		float specularIntensity = 0.;
		if(diffAngle > 0) {
			// Calculate the specular angle
			glm::vec3 H = glm::normalize(lightDir + viewDir);
			float specAngle = glm::max(glm::dot(H, pNormal), 0.0f);

			// Calculate the specular intensity
			specularIntensity = pow(specAngle, material->getShininess());
		}

		// Calculate the specular color
		glm::vec4 specularColor = specular * material->getSpecular() * specularIntensity;

		// Return the final color
		return ambientColor + diffuseColor + specularColor;
	}
};