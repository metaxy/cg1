#pragma once

#include <exception>
#include <fstream>
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Material.hpp"
#include "TriMesh.hpp"

/* Resources are loaded as pool resources */
class Resources {
public:
	/**** LOADING A RESOURCE ****/
	template<class T>
	static void Load(typename const T::LoadDesc& desc) {
		std::cout << "Loading " << desc.path << "...";
		T* pObject = T::Loader::load(desc);
		if(!pObject) {
			std::cerr << "error" << std::endl;
		} else {
			T::Add(pObject, desc.name);
			std::cout << "done" << " [\"" << desc.name << "\"]" << std::endl;
		}
	}

	/**** UNLOADING A RESOURCE ****/
	template<class T>
	static void Remove(std::string name) {
		T::Remove(name);
	}

	/**** RECEIVING A RESOURCE ****/
	template<class T>
	static T* Get(std::string name) {
		return T::Get(name);
		//throw std::exception("Not implemented");
	}
};