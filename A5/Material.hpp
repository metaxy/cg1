#pragma once

#include "glm\glm\glm.hpp"

#include <map>
#include <string>

class Material {
public:
	struct LoadDesc {
		std::string path;
	};
	class Loader {
	public:
		static std::map<std::string, Material*> load(const Material::LoadDesc& desc);
	private:
		static std::map<std::string, Material*> loadMtl(const Material::LoadDesc& desc);
	};

public:
	inline Material(glm::vec4 ambient = glm::vec4(),
					glm::vec4 diffuse = glm::vec4(),
					glm::vec4 specular = glm::vec4(),
					float shininess = 0)
					: m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess) {
	}

	inline glm::vec4 getAmbient() const {
		return m_ambient;
	}
	inline glm::vec4 getDiffuse() const {
		return m_diffuse;
	}
	inline glm::vec4 getSpecular() const {
		return m_specular;
	}
	inline float getShininess() const {
		return m_shininess;
	}

private:
	// ambient color
	glm::vec4 m_ambient;
	// diffuse color
	glm::vec4 m_diffuse;
	// specular color
	glm::vec4 m_specular;
	// shininess
	float m_shininess;
};