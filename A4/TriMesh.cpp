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

enum VertexType {
	UNKNOWN, LEFT, RIGHT
};

TriMesh::TriMesh() {
	winding = CW;
}

TriMesh::TriMesh(const std::string& fileName, bool normalize) {

	name = fileName;
	winding = CW;
	loadOff(fileName);
	if(normalize) {
		center();
		unitize();
	}
	computeNormals();
}

TriMesh::~TriMesh() {
}

void TriMesh::setWinding(PolygonWinding winding) {
	this->winding = winding;
}

// center model at its origin
void TriMesh::center(void) {

	calculateBoundingBox();

	vec3 center = (boundingBoxMin + boundingBoxMax) * vec3(0.5);

	for(unsigned int i = 0; i < positions.size(); i++) {
		positions[i] -= center;
	}
	boundingBoxMin -= center;
	boundingBoxMax -= center;
}

// normalize to bounding sphere radius 1
void TriMesh::unitize(void) {

	calculateBoundingSphere();

	for(unsigned int i = 0; i < positions.size(); ++i) {
		positions[i] /= boundingSphereRadius;
	}
	boundingSphereRadius = 1;
	boundingBoxMin = vec3(-1);
	boundingBoxMax = vec3(1);
}

// calculate bounding sphere
void TriMesh::calculateBoundingSphere(void) {

	boundingSphereRadius = 0;
	for(unsigned int i = 0; i<positions.size(); i++) {
		vec3 v = positions[i];
		if(length(v) > boundingSphereRadius) boundingSphereRadius = length(v);
	}
}

// calculate bounding box
void TriMesh::calculateBoundingBox(void) {

	boundingBoxMin = vec3(numeric_limits<float>::max());
	boundingBoxMax = vec3(numeric_limits<float>::min());
	for(unsigned int i = 0; i < positions.size(); i++) {
		if(positions[i].x < boundingBoxMin.x) boundingBoxMin.x = positions[i].x;
		if(positions[i].x > boundingBoxMax.x) boundingBoxMax.x = positions[i].x;
		if(positions[i].y < boundingBoxMin.y) boundingBoxMin.y = positions[i].y;
		if(positions[i].y > boundingBoxMax.y) boundingBoxMax.y = positions[i].y;
		if(positions[i].z < boundingBoxMin.z) boundingBoxMin.z = positions[i].z;
		if(positions[i].z > boundingBoxMax.z) boundingBoxMax.z = positions[i].z;
	}
}

void TriMesh::correctTexture(bool correct) {
	textureCorrection = correct;
}

// load triangle mesh in OFF format
void TriMesh::reload() {
	loadOff(name);
}

// load triangle mesh in .OFF format
void TriMesh::loadOff(const string& fileName) {
	name = fileName;

	// Clear all lists and buffers
	positions.clear();
	normals.clear();
	faces.clear();
	texCoords.clear();

	// Open the model file
	ifstream inStream;
	inStream.open(fileName.c_str());

	if(!inStream.is_open()) {
		exit(1);
	}

	string line;
	int lineNumber = 0;

	int numVerticesRead = 0;
	int numFacesRead = 0;
	int numVertices = 0;
	int numFaces = 0;

	// Read the file line by line
	while(getline(inStream, line)) {
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
			if(numVertices > positions.size()) {
				// Read a vertex and add it to the list
				GLfloat x, y, z;
				stringstream sStream(line);
				sStream >> x >> y >> z;

				positions.push_back(glm::vec3(x, y, z));
			} else {
				// Read a face, make sure the winding is correct and add it to the list
				GLuint i, a, b, c;
				stringstream sStream(line);
				sStream >> i >> a >> b >> c;

				if(winding == PolygonWinding::CCW) {
					// Swap a and c face index
					i = a;
					a = c;
					c = i;
				}

				faces.push_back(glm::uvec3(a, b, c));
			}
		}

		lineNumber++;
	}
}

// calculate smooth per-vertex normals
void TriMesh::computeNormals(void) {
	vector<vec3> faceNormals;

	// Calculate the normals for all faces
	for(uvec3 face : faces) {
		faceNormals.push_back(
			glm::normalize(glm::cross(
			(positions[face.y] - positions[face.x]),
			(positions[face.z] - positions[face.x])
			))
		);
	}

	// Iterate over all vertices
	for(int i = 0; i < positions.size(); ++i) {
		int numFaces = 0;
		vec3 normal = vec3(0, 0, 0);

		// Determin which face contains this vertex and add the face normal to the vertex normal
		for(int j = 0; j < faces.size(); ++j) {
			if(faces[j].x == i || faces[j].y == i || faces[j].z == i) {
				++numFaces;
				normal += faceNormals[j];
			}
		}

		// Weight the face normals and calculate the final vertex normal
		normal /= numFaces;
		normals.push_back(glm::normalize(normal));
	}
}

// Compute uv coordinates with a spherical mapping
// (vertices are projected on a sphere along the normal and classical sphere uv unwrap is used)
void TriMesh::computeSphereUVs(void) {
	// Calculate the texcoords for each vertex
	int id = 0;
	for each(vec3 v in positions) {
		vec3 d = normals[id];

		// (0, 0, 1) -> (0.5, 0.5)
		float v = 1.f - acos(d.y) / glm::pi<float>();
		float u = 0.5f + atan2(d.x, d.z) / (2 * glm::pi<float>());
		texCoords.push_back(vec2(u, v));

		++id;
	}

	if(!textureCorrection)
		return;

	// Get the seam faces
	for(int f = 0; f < faces.size(); ++f) {
		// Get the normals of all vertices
		vec3 n1 = normals[faces[f].x];
		vec3 n2 = normals[faces[f].y];
		vec3 n3 = normals[faces[f].z];

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

		int a = faces[f].x;
		int b = faces[f].y;
		int c = faces[f].z;

		if(t1 == VertexType::RIGHT) {
			int vid = positions.size();

			// Copy the vertex with index a
			positions.push_back(positions[a]);
			normals.push_back(normals[a]);

			// Assign the right texture coordinates
			vec2 uv;
			uv.x = 1.f + texCoords[a].x;
			uv.y = texCoords[a].y;
			texCoords.push_back(uv);

			// Set the new face index
			a = vid;
		}
		if(t2 == VertexType::RIGHT) {
			int vid = positions.size();

			// Copy the vertex with index a
			positions.push_back(positions[b]);
			normals.push_back(normals[b]);

			// Assign the right texture coordinates
			vec2 uv;
			uv.x = 1.f + texCoords[b].x;
			uv.y = texCoords[b].y;
			texCoords.push_back(uv);

			// Set the new face index
			b = vid;
		}
		if(t3 == VertexType::RIGHT) {
			int vid = positions.size();

			// Copy the vertex with index a
			positions.push_back(positions[c]);
			normals.push_back(normals[c]);

			// Assign the right texture coordinates
			vec2 uv;
			uv.x = 1.f + texCoords[c].x;
			uv.y = texCoords[c].y;
			texCoords.push_back(uv);

			// Set the new face index
			c = vid;
		}

		// Alter the face
		faces[f] = uvec3(a, b, c);
	}
}

// draw the mesh using vertex arrays
void TriMesh::draw(void) {
	if(!positions.empty()) {
		glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, &positions[0]);
		glEnableVertexAttribArray(attribVertex);
	}

	if(!normals.empty()) {
		glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, &normals[0]);
		glEnableVertexAttribArray(attribNormal);
	}

	if(!texCoords.empty()) {
		glVertexAttribPointer(attribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, &texCoords[0]);
		glEnableVertexAttribArray(attribTexCoord);
	}

	if(!faces.empty())
		glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, &faces[0]);
}
