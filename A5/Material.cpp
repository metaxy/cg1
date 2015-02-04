#include "Material.hpp"

#include <fstream>
#include <string>
#include <vector>

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\constants.hpp"
#include "Utils.hpp"

using namespace glm;
using namespace std;

Material* Material::Loader::load(const Material::LoadDesc& desc) {
	// Determin the file format
	auto tokens = split(desc.path, '.');
	string format = tokens[tokens.size() - 1];

	//
	Material* material(nullptr);

	// Use the loader for the right format
	if(format == "mtl") {
		material = Material::Loader::loadMtl(desc);
	} else {
		throw std::exception((std::string("[Material::Loader] Can't load material with format: ") + format).c_str());
	}

	return material;
}
Material* Material::Loader::loadMtl(const Material::LoadDesc& desc) {
	// Open the material file
	std::fstream stream(desc.path, std::ios_base::in);

	if(!stream.is_open()) {
		// File could not be opened
		throw std::exception(("[Material::Loader] Failed to open the file " + desc.path).c_str());
	}

	std::string line("");
	Material* material(nullptr);
	while(getline(stream, line)) {
		// Read the file line by line

		// Handle empty lines
		if(line.empty())
			continue;

		// Handle commentary lines
		if(line[0] == '#')
			continue;

		// Split the line by spaces
		auto tokens = split(line, ' ');

		// Get the command token
		std::string command = tokens[0];

		if(command == "newmtl") {
			assert(!material);

			// Create a new material
			material = new Material();

			// ATTENTION: We assume that there are not two materials with the same name in the same file
			//mats[tokens[1]] = current;
		} else if(command == "Ns" && material) {
			std::stringstream ss(tokens[1]);
			ss >> material->m_shininess;
		} else if(command == "Ka" && material) {
			float r, g, b;
			std::stringstream(tokens[1]) >> r;
			std::stringstream(tokens[2]) >> g;
			std::stringstream(tokens[3]) >> b;

			material->m_ambient = glm::vec4(r, g, b, 1.f);
		} else if(command == "Kd" && material) {
			float r, g, b;
			std::stringstream(tokens[1]) >> r;
			std::stringstream(tokens[2]) >> g;
			std::stringstream(tokens[3]) >> b;

			material->m_diffuse = glm::vec4(r, g, b, 1.f);
		} else if(command == "Ks" && material) {
			float r, g, b;
			std::stringstream(tokens[1]) >> r;
			std::stringstream(tokens[2]) >> g;
			std::stringstream(tokens[3]) >> b;

			material->m_specular = glm::vec4(r, g, b, 1.f);
		} else if(command == "Ni" && material) {
			std::stringstream(tokens[1]) >> material->m_refraction;
		} else if(command == "d" && material) {
			// Ignore dissolving
		} else {
			// Unknown command
		}
	}

	return material;
}