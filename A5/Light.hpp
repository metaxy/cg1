#pragma once

#include "glm\glm\glm.hpp"

struct Light {
	//position in view space
	glm::vec4 position;
	// ambient color
	glm::vec4 ambient;
	// diffuse color
	glm::vec4 diffuse;
	// specular color
	glm::vec4 specular;
};