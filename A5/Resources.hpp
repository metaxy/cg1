#pragma once

#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Material.hpp"
#include "TriMesh.hpp"

/* The loader to load meshes with */
class MeshLoader {
public:
	static TriMesh* load(const TriMesh::LoadDesc& desc);

private:
	static TriMesh* loadOff(const TriMesh::LoadDesc& desc);
	static TriMesh* loadObj(const TriMesh::LoadDesc& desc);

	static void center(TriMesh* mesh);
	static void unitize(TriMesh* mesh);
	static void computeNormals(TriMesh* mesh);

	static void computeSphereUVs(TriMesh* mesh, bool correct);
	// calculate bounding sphere
	static void calculateBoundingSphere(TriMesh* mesh);
	// calculate bounding box
	static void calculateBoundingBox(TriMesh* mesh);
};

/* The loader to load material with */
class MaterialLoader {
public:
	static std::map<std::string, Material*> load(const Material::LoadDesc& desc);

private:
	static std::map<std::string, Material*> loadMtl(const Material::LoadDesc& desc);
};

class Resources {
public:
	template<class T>
	static void Load(typename const T::LoadDesc& desc) {
		throw std::exception("Not implemented");
	}
	/* Load multiple materials form a *.mtl file 
	The name of the material is specified in the file*/
	template<>
	static void Load<Material>(typename const Material::LoadDesc& desc) {
		std::map<std::string, Material*> materials;

		// Load the materials
		materials = MaterialLoader::load(desc);

		// Insert the materials to our material table and remove duplicates
		for each(std::pair<std::string, Material*> p in materials) {
			RemoveDuplicate<Material>(p.first);
			m_nameMaterial[p.first] = p.second;
		}
	}
	template<>
	static void Load<TriMesh>(typename const TriMesh::LoadDesc& desc) {
		TriMesh* mesh(nullptr);

		mesh = MeshLoader::load(desc);

		if(mesh != nullptr) {
			RemoveDuplicate<TriMesh>(desc.name);
			m_nameMesh[desc.name] = mesh;
		}
	}

	template<class T>
	static void Unload(std::string name) {
		RemoveDuplicate(name);
	}

	template<class T>
	static T* Get(std::string name) {
		throw std::exception("Not implemented");
	}
	template<>
	static Material* Get(std::string name) {
		auto it = m_nameMaterial.find(name);
		if(it != m_nameMaterial.end()) {
			return it->second;
		} else {
			return nullptr;
		}
	}
	template<>
	static TriMesh* Get(std::string name) {
		auto it = m_nameMesh.find(name);
		if(it != m_nameMesh.end()) {
			return it->second;
		} else {
			return nullptr;
		}
	}

	template<class T>
	static void RemoveDuplicate(std::string) {
		throw std::exception("Not implemented");
	}
	template<>
	static void RemoveDuplicate<Material>(std::string name) {
		auto it = m_nameMaterial.find(name);
		if(it != m_nameMaterial.end()) {
			delete it->second;
			m_nameMaterial.erase(it);
		}
	}
	template<>
	static void RemoveDuplicate<TriMesh>(std::string name) {
		auto it = m_nameMesh.find(name);
		if(it != m_nameMesh.end()) {
			delete it->second;
			m_nameMesh.erase(it);
		}
	}
private:
	static std::map<std::string, Material*> m_nameMaterial;
	static std::map<std::string, TriMesh*> m_nameMesh;
};