#include "Resources.hpp"

#include "glm\glm\glm.hpp"
#include "glm\glm\gtc\constants.hpp"
#include "Utils.hpp"

using namespace glm;
using namespace std;

map<string, Material*> Resources::m_nameMaterial;
map<string, TriMesh*> Resources::m_nameMesh;

TriMesh* MeshLoader::load(const TriMesh::LoadDesc& desc) {
	TriMesh* mesh(nullptr);

	// Use the loader for the right format
	if(desc.format == "off") {
		mesh = MeshLoader::loadOff(desc);
	} else if(desc.format == "obj") {
		mesh = MeshLoader::loadObj(desc);
	} else {
		throw std::exception((std::string("[MeshLoader] Can't load mesh with format: ") + desc.format).c_str());
	}

	if(desc.normalize) {
		MeshLoader::center(mesh);
		MeshLoader::unitize(mesh);
	}

	if(desc.calculateVertexNormals)
		MeshLoader::computeNormals(mesh);
	if(desc.calculateSphereUVs)
		MeshLoader::computeSphereUVs(mesh, desc.textureCorrection);

	return mesh;
}

TriMesh* MeshLoader::loadOff(const TriMesh::LoadDesc& desc) {
	// Load an *.off mesh

	// Open the material file
	std::fstream stream(desc.path, std::ios_base::in);

	if(!stream.is_open()) {
		// File could not be opened
		throw std::exception((string("[MeshLoader] Failed to open the file: ") + desc.path.c_str()).c_str());
	}

	// Create a new empty mesh
	TriMesh* mesh = new TriMesh();

	string line;
	int lineNumber = 0;

	int numVerticesRead = 0;
	int numFacesRead = 0;
	int numVertices = 0;
	int numFaces = 0;

	// Read the file line by line
	while(getline(stream, line)) {
		if(line.empty())
			continue;

		if(lineNumber == 0) {
			// First line has to be "Off"
			if(line != "OFF") {
				exit(1);
			}
		} else if(lineNumber == 1) {
			// Second line contains number of vertices and faces
			stringstream sStream(line);
			sStream >> numVertices >> numFaces;
		} else {
			if(numVertices > mesh->positions.size()) {
				// Read a vertex and add it to the list
				GLfloat x, y, z;
				stringstream sStream(line);
				sStream >> x >> y >> z;
				// Add a position for this vertex 
				mesh->positions.push_back(glm::vec3(x, y, z));
				// And a vector with face indices
				mesh->vertexFaceIndices.push_back(std::vector<int>());
			} else {
				// Read a face, make sure the winding is correct and add it to the list
				GLuint i, a, b, c;
				stringstream sStream(line);
				sStream >> i >> a >> b >> c;

				if(desc.winding == TriMesh::PolygonWinding::CCW) {
					// Swap a and c face index
					i = a;
					a = c;
					c = i;
				}

				mesh->faces.push_back(glm::uvec3(a, b, c));

				// Compute the normal of the face
				mesh->faceNormals.push_back(
					glm::normalize(glm::cross(
					(mesh->positions[b] - mesh->positions[a]),
					(mesh->positions[c] - mesh->positions[a])
					))
					);

				// For all three positions add this face index to their face index list
				mesh->vertexFaceIndices[a].push_back(mesh->faces.size() - 1);
				mesh->vertexFaceIndices[b].push_back(mesh->faces.size() - 1);
				mesh->vertexFaceIndices[c].push_back(mesh->faces.size() - 1);
			}
		}

		lineNumber++;
	}

	return mesh;
}
TriMesh* MeshLoader::loadObj(const TriMesh::LoadDesc& desc) {
	// Load an *.obj mesh
	return nullptr;
}

void MeshLoader::center(TriMesh* mesh) {
	MeshLoader::calculateBoundingBox(mesh);

	vec3 center = (mesh->boundingBoxMin + mesh->boundingBoxMax) * vec3(0.5);

	for(unsigned int i = 0; i < mesh->positions.size(); i++) {
		mesh->positions[i] -= center;
	}
	mesh->boundingBoxMin -= center;
	mesh->boundingBoxMax -= center;
}
void MeshLoader::unitize(TriMesh* mesh) {
	MeshLoader::calculateBoundingSphere(mesh);

	for(unsigned int i = 0; i < mesh->positions.size(); ++i) {
		mesh->positions[i] /= mesh->boundingSphereRadius;
	}
	mesh->boundingSphereRadius = 1;
	mesh->boundingBoxMin = vec3(-1);
	mesh->boundingBoxMax = vec3(1);
}
void MeshLoader::computeNormals(TriMesh* mesh) {
	for(int i = 0; i < mesh->vertexFaceIndices.size(); ++i) {
		vec3 normal = vec3(0, 0, 0);
		for(int j = 0; j < mesh->vertexFaceIndices[i].size(); ++j) {
			normal += mesh->faceNormals[mesh->vertexFaceIndices[i][j]];
		}
		normal /= mesh->vertexFaceIndices[i].size();
		mesh->normals.push_back(glm::normalize(normal));
	}
}
void MeshLoader::computeSphereUVs(TriMesh* mesh, bool textureCorrection) {
	enum VertexType {
		UNKNOWN, LEFT, RIGHT
	};

	// Calculate the texcoords for each vertex
	int id = 0;
	for each(vec3 v in mesh->positions) {
		vec3 d = mesh->normals[id];

		// (0, 0, 1) -> (0.5, 0.5)
		float v = 1.f - acos(d.y) / glm::pi<float>();
		float u = 0.5f + atan2(d.x, d.z) / (2 * glm::pi<float>());
		mesh->texCoords.push_back(vec2(u, v));

		++id;
	}

	if(!textureCorrection)
		return;

	// Get the seam faces
	for(int f = 0; f < mesh->faces.size(); ++f) {
		// Get the normals of all vertices
		vec3 n1 = mesh->normals[mesh->faces[f].x];
		vec3 n2 = mesh->normals[mesh->faces[f].y];
		vec3 n3 = mesh->normals[mesh->faces[f].z];

		// Project normals to x-z plane
		n1.y = 0;
		n2.y = 0;
		n3.y = 0;

		VertexType t1 = VertexType::UNKNOWN;
		VertexType t2 = VertexType::UNKNOWN;
		VertexType t3 = VertexType::UNKNOWN;

		// ASSUMPTION: Normals are nearly the same (we have a continuum surface)
		// Calculate their position (left or right of the seam looking down positive z axis)
		if(n1.z < 0.f && n2.z < 0.f && n3.z < 0.f) {
			t1 = (n1.x >= 0.f) ? VertexType::LEFT : VertexType::RIGHT;
			t2 = (n2.x >= 0.f) ? VertexType::LEFT : VertexType::RIGHT;
			t3 = (n3.x >= 0.f) ? VertexType::LEFT : VertexType::RIGHT;
		}

		if((t1 == VertexType::UNKNOWN && t2 == VertexType::UNKNOWN && t3 == VertexType::UNKNOWN) ||
		   (t1 == VertexType::LEFT && t2 == VertexType::LEFT && t3 == VertexType::LEFT) ||
		   (t1 == VertexType::RIGHT && t2 == VertexType::RIGHT && t3 == VertexType::RIGHT)) {
			// All have the same vertex type
			continue;
		}

		int a = mesh->faces[f].x;
		int b = mesh->faces[f].y;
		int c = mesh->faces[f].z;

		if(t1 == VertexType::RIGHT) {
			int vid = mesh->positions.size();

			// Copy the vertex with index a
			mesh->positions.push_back(mesh->positions[a]);
			mesh->normals.push_back(mesh->normals[a]);

			// Assign the right texture coordinates
			vec2 uv;
			uv.x = 1.f + mesh->texCoords[a].x;
			uv.y = mesh->texCoords[a].y;
			mesh->texCoords.push_back(uv);

			// Set the new face index
			a = vid;
		}
		if(t2 == VertexType::RIGHT) {
			int vid = mesh->positions.size();

			// Copy the vertex with index a
			mesh->positions.push_back(mesh->positions[b]);
			mesh->normals.push_back(mesh->normals[b]);

			// Assign the right texture coordinates
			vec2 uv;
			uv.x = 1.f + mesh->texCoords[b].x;
			uv.y = mesh->texCoords[b].y;
			mesh->texCoords.push_back(uv);

			// Set the new face index
			b = vid;
		}
		if(t3 == VertexType::RIGHT) {
			int vid = mesh->positions.size();

			// Copy the vertex with index a
			mesh->positions.push_back(mesh->positions[c]);
			mesh->normals.push_back(mesh->normals[c]);

			// Assign the right texture coordinates
			vec2 uv;
			uv.x = 1.f + mesh->texCoords[c].x;
			uv.y = mesh->texCoords[c].y;
			mesh->texCoords.push_back(uv);

			// Set the new face index
			c = vid;
		}

		// Alter the face
		mesh->faces[f] = uvec3(a, b, c);
	}
}
// calculate bounding sphere
void MeshLoader::calculateBoundingSphere(TriMesh* mesh) {

	mesh->boundingSphereRadius = 0;
	for(unsigned int i = 0; i<mesh->positions.size(); i++) {
		vec3 v = mesh->positions[i];
		if(length(v) > mesh->boundingSphereRadius) mesh->boundingSphereRadius = length(v);
	}
}
// calculate bounding box
void MeshLoader::calculateBoundingBox(TriMesh* mesh) {
	mesh->boundingBoxMin = vec3(numeric_limits<float>::max());
	mesh->boundingBoxMax = vec3(numeric_limits<float>::min());
	for(unsigned int i = 0; i < mesh->positions.size(); i++) {
		if(mesh->positions[i].x < mesh->boundingBoxMin.x) mesh->boundingBoxMin.x = mesh->positions[i].x;
		if(mesh->positions[i].x > mesh->boundingBoxMax.x) mesh->boundingBoxMax.x = mesh->positions[i].x;
		if(mesh->positions[i].y < mesh->boundingBoxMin.y) mesh->boundingBoxMin.y = mesh->positions[i].y;
		if(mesh->positions[i].y > mesh->boundingBoxMax.y) mesh->boundingBoxMax.y = mesh->positions[i].y;
		if(mesh->positions[i].z < mesh->boundingBoxMin.z) mesh->boundingBoxMin.z = mesh->positions[i].z;
		if(mesh->positions[i].z > mesh->boundingBoxMax.z) mesh->boundingBoxMax.z = mesh->positions[i].z;
	}
}

map<string, Material*> MaterialLoader::load(const Material::LoadDesc& desc) {
	// We interpret every file as mtl file

	// Use the loader for the right format
	//if(desc.format == "obj") {
	 map<string, Material*> materials = MaterialLoader::loadMtl(desc);
	/*} else {
		throw std::exception((std::string("[MeshLoader] Can't load mesh with format: ") + desc.format).c_str());
		}*/

	 return materials;
}
map<string, Material*> MaterialLoader::loadMtl(const Material::LoadDesc& desc) {
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