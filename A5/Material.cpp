#include "Material.hpp"

#include <fstream>
#include <map>
#include <string>

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\constants.hpp"
#include "Utils.hpp"

using namespace glm;
using namespace std;

map<string, Material*> Material::Loader::load(const Material::LoadDesc& desc) {
	// We interpret every file as mtl file

	// Use the loader for the right format
	//if(desc.format == "obj") {
	map<string, Material*> materials = Material::Loader::loadMtl(desc);
	/*} else {
	throw std::exception((std::string("[MeshLoader] Can't load mesh with format: ") + desc.format).c_str());
	}*/

	return materials;
}
map<string, Material*> Material::Loader::loadMtl(const Material::LoadDesc& desc) {
	map<string, Material*> mats;

	// Open the material file
	std::fstream stream(desc.path, std::ios_base::in);

	if(!stream.is_open()) {
		// File could not be opened
		throw std::exception("[MaterialLoader] Failed to open the file");
	}

	std::string line("");
	Material* current(nullptr);
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
			// Create a new material
			current = new Material();

			// ATTENTION: We assume that there are not two materials with the same name in the same file
			mats[tokens[1]] = current;
		} else if(command == "Ns" && current) {
			std::stringstream ss(tokens[1]);
			ss >> current->m_shininess;
		} else if(command == "Ka" && current) {
			float r, g, b;
			std::stringstream(tokens[1]) >> r;
			std::stringstream(tokens[2]) >> g;
			std::stringstream(tokens[3]) >> b;

			current->m_ambient = glm::vec4(r, g, b, 1.f);
		} else if(command == "Kd" && current) {
			float r, g, b;
			std::stringstream(tokens[1]) >> r;
			std::stringstream(tokens[2]) >> g;
			std::stringstream(tokens[3]) >> b;

			current->m_diffuse = glm::vec4(r, g, b, 1.f);
		} else if(command == "Ks" && current) {
			float r, g, b;
			std::stringstream(tokens[1]) >> r;
			std::stringstream(tokens[2]) >> g;
			std::stringstream(tokens[3]) >> b;

			current->m_specular = glm::vec4(r, g, b, 1.f);
		} else if(command == "Ni" && current) {
			// Ignore optical density
		} else if(command == "d" && current) {
			// Ignore dissolving
		} else {
			// Unknown command
		}
	}

	return mats;
}