/* ----------------------------------------------------------------
   name:           TriMesh.cpp
   purpose:        cg1_ex3 2014 triangle mesh for OpenGL rendering
   version:	   SKELETON CODE
   TODO:           computeSphereUVs
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include "TriMesh.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "glm\glm\gtc\constants.hpp"
#include "Triangle.hpp"
#include "Utils.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// NVIDIA wants it like this
// see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
const GLuint TriMesh::attribVertex = 0;
const GLuint TriMesh::attribNormal = 2;
const GLuint TriMesh::attribColor = 3;
const GLuint TriMesh::attribTexCoord = 8;

TriMesh::TriMesh() {
}
TriMesh::~TriMesh() {
}

// draw the mesh using vertex arrays
void TriMesh::draw(void) {
	if(!positions.empty()) {
		glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, &positions[0]);
		glEnableVertexAttribArray(attribVertex);
	} else {
		glDisableVertexAttribArray(attribVertex);
	}

	if(!normals.empty()) {
		glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, &normals[0]);
		glEnableVertexAttribArray(attribNormal);
	} else {
		glDisableVertexAttribArray(attribNormal);
	}

	if(!texCoords.empty()) {
		glVertexAttribPointer(attribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]);
		glEnableVertexAttribArray(attribTexCoord);
	} else {
		glDisableVertexAttribArray(attribTexCoord);
	}

	if(!faces.empty())
		glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, &faces[0]);
}

/*** The loader class for the tri mesh object ***/

TriMesh* TriMesh::Loader::load(const TriMesh::LoadDesc& desc) {
	TriMesh* mesh(nullptr);

	// Use the loader for the right format
	if(desc.format == "off") {
		mesh = TriMesh::Loader::loadOff(desc);
	} else if(desc.format == "obj") {
		mesh = TriMesh::Loader::loadObj(desc);
	} else {
		throw std::exception((std::string("[MeshLoader] Can't load mesh with format: ") + desc.format).c_str());
	}

	if(desc.normalize) {
		TriMesh::Loader::center(mesh);
		TriMesh::Loader::unitize(mesh);
	}

	if(desc.calculateVertexNormals && desc.format != "obj")
		TriMesh::Loader::computeNormals(mesh);
	if(desc.calculateSphereUVs)
		TriMesh::Loader::computeSphereUVs(mesh, desc.textureCorrection);

	return mesh;
}
TriMesh* TriMesh::Loader::loadOff(const TriMesh::LoadDesc& desc) {
	// Load an *.off mesh

	// Open the material file
	std::fstream stream(desc.path, std::ios_base::in);

	if(!stream.is_open()) {
		// File could not be opened
		throw std::exception((string("[TriMesh::Loader] Failed to open the file: ") + desc.path.c_str()).c_str());
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

				if(desc.winding == TriMesh::PolygonWinding::CW) {
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
TriMesh* TriMesh::Loader::loadObj(const TriMesh::LoadDesc& desc) {
	// Open the file
	std::fstream stream(desc.path, std::ios_base::in);

	if(!stream.is_open()) {
		// File could not be opened
		throw std::exception((string("[TriMesh::Loader] Failed to open the file: ") + desc.path.c_str()).c_str());
	}

	int numVerticesRead = 0;
	int numFacesRead = 0;
	int numVertices = 0;
	int numFaces = 0;

	std::vector<string> chunks;

	std::string line("");

	TriMesh* mesh(nullptr);

	while(getline(stream, line)) {
		// Read the file line by line

		// Handle empty lines
		if(line.empty())
			continue;

		// Split the line by spaces into tokens
		auto tokens = split(line, ' ');

		// Get the command token
		std::string command = tokens[0];
		if(command == "o") {
			// Ignore the given name of the mesh
			mesh = new TriMesh();
		} else if(command == "v") {
			GLfloat x, y, z;
			stringstream(tokens[1]) >> x;
			stringstream(tokens[2]) >> y;
			stringstream(tokens[3]) >> z;
			mesh->positions.push_back(vec3(x, y, z));
		} else if(command == "vn") {
			GLfloat x, y, z;
			stringstream(tokens[1]) >> x;
			stringstream(tokens[2]) >> y;
			stringstream(tokens[3]) >> z;
			mesh->normals.push_back(vec3(x, y, z));
		} else if(command == "f") {
			// Read a face, make sure the winding is correct and add it to the list
			GLuint a, b, c;
			stringstream(tokens[1]) >> a;
			stringstream(tokens[2]) >> b;
			stringstream(tokens[3]) >> c;
			--a;
			--b;
			--c;

			if(desc.winding == TriMesh::PolygonWinding::CW) {
				// Swap a and c face index
				auto i = a;
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
		} else if(command == "vt") {
			GLfloat u, v;
			stringstream(tokens[1]) >> u;
			stringstream(tokens[2]) >> v;
			mesh->texCoords.push_back(vec2(u, v));
		}
	}

	return mesh;
}
void TriMesh::Loader::center(TriMesh* mesh) {
	TriMesh::Loader::calculateBoundingBox(mesh);

	vec3 center = (mesh->boundingBoxMin + mesh->boundingBoxMax) * vec3(0.5);

	for(unsigned int i = 0; i < mesh->positions.size(); i++) {
		mesh->positions[i] -= center;
	}
	mesh->boundingBoxMin -= center;
	mesh->boundingBoxMax -= center;
}
void TriMesh::Loader::unitize(TriMesh* mesh) {
	TriMesh::Loader::calculateBoundingSphere(mesh);

	for(unsigned int i = 0; i < mesh->positions.size(); ++i) {
		mesh->positions[i] /= mesh->boundingSphereRadius;
	}
	mesh->boundingSphereRadius = 1;
	mesh->boundingBoxMin = vec3(-1);
	mesh->boundingBoxMax = vec3(1);
}
void TriMesh::Loader::computeNormals(TriMesh* mesh) {
	for(int i = 0; i < mesh->vertexFaceIndices.size(); ++i) {
		vec3 normal = vec3(0, 0, 0);
		for(int j = 0; j < mesh->vertexFaceIndices[i].size(); ++j) {
			normal += mesh->faceNormals[mesh->vertexFaceIndices[i][j]];
		}
		normal /= mesh->vertexFaceIndices[i].size();
		mesh->normals.push_back(glm::normalize(normal));
	}
}
void TriMesh::Loader::computeSphereUVs(TriMesh* mesh, bool textureCorrection) {
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
void TriMesh::Loader::calculateBoundingSphere(TriMesh* mesh) {

	mesh->boundingSphereRadius = 0;
	for(unsigned int i = 0; i<mesh->positions.size(); i++) {
		vec3 v = mesh->positions[i];
		if(length(v) > mesh->boundingSphereRadius) mesh->boundingSphereRadius = length(v);
	}
}
void TriMesh::Loader::calculateBoundingBox(TriMesh* mesh) {
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