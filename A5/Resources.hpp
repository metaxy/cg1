#pragma once

#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Material.hpp"
#include "Utils.hpp"

class Resources {
public:
	template<class T>
	static std::vector<T> Load(std::string file, std::string name = "") {
		throw std::exception("Not implemented");
	}

	template<>
	static std::vector<Material> Resources::Load<Material>(std::string file, std::string name) {
		// We interpret every file as mtl file
		std::vector<Material> matList;

		// Open the material file
		std::fstream stream(file, std::ios_base::in);

		if(!stream.is_open()) {
			// File could not be opened
			throw std::exception("Failed to open the file");
		}

		std::string line("");
		Material current;
		bool currentlyActive = false;
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
				if(currentlyActive) {
					// We have to save the current material
					matList.push_back(current);
				}

				current = Material();
				currentlyActive = true;

				// Get the name of the material
				current.m_name = tokens[1];
			} else if(command == "Ns" && currentlyActive) {
				std::stringstream ss(tokens[1]);
				ss >> current.m_shininess;
			} else if(command == "Ka" && currentlyActive) {
				float r, g, b;
				std::stringstream(tokens[1]) >> r;
				std::stringstream(tokens[2]) >> g;
				std::stringstream(tokens[3]) >> b;

				current.m_ambient = glm::vec4(r, g, b, 1.f);
			} else if(command == "Kd" && currentlyActive) {
				float r, g, b;
				std::stringstream(tokens[1]) >> r;
				std::stringstream(tokens[2]) >> g;
				std::stringstream(tokens[3]) >> b;

				current.m_diffuse = glm::vec4(r, g, b, 1.f);
			} else if(command == "Ks" && currentlyActive) {
				float r, g, b;
				std::stringstream(tokens[1]) >> r;
				std::stringstream(tokens[2]) >> g;
				std::stringstream(tokens[3]) >> b;

				current.m_specular = glm::vec4(r, g, b, 1.f);
			} else if(command == "Ni" && currentlyActive) {
				// Ignore optical density
			} else if(command == "d" && currentlyActive) {
				// Ignore dissolving
			} else {
				// Unknown command
			}
		}

		// TODO: Dont forget to save the last material
		if(currentlyActive) {
			matList.push_back(current);
		}

		return matList;
	}
};