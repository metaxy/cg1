#pragma once

#include <map>
#include <string>

template<class T>
class PoolResource {
public:
	static void Add(T* pObject, std::string name) {		
		// Remove any existing resource
		Remove(name);

		// Add the new object to the pool
		m_pool[name] = pObject;
	}
	static T* Get(std::string name) {
		auto it = m_pool.find(name);
		if(it != m_pool.end()) {
			return it->second;
		}

		return nullptr;
	}
	static void Remove(std::string name) {
		auto it = m_pool.find(name);
		if(it != m_pool.end()) {
			delete it->second;
			m_pool.erase(it);
		}
	}
private:
	static std::map<std::string, T*> m_pool;
};

template<class T>
std::map<std::string, T*> PoolResource<T>::m_pool;