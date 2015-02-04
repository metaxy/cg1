#pragma once

#include <memory>
#include <vector>

#include "glm\glm\glm.hpp"
#include "Ray.hpp"
#include "Material.hpp"

class GLSLShader;
struct Light;
class Triangle;
class TriMesh;
class KDTree;

class Scene {
	friend class Raytracer;

	struct SceneObject {
		TriMesh* mesh;
		Material* material;
		glm::mat4 transformation;
	};

public:
	Scene();
	~Scene();

	static void BuildScene(Scene& scene);

	void AddObject(std::string modelName, std::string materialName,
				   glm::mat4 modelMatrix);
	void AddLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular);
	//void AddLight();
	void BuildKDTree();

	bool Hit(const Ray& r, Ray::HitInfo& rh);

	/* Rendering stuff */
public:
	void RenderScene(GLSLShader& shader, const glm::mat4& modelView, const glm::mat4& projection);
	void RenderTree();

private:
	void BindLights(GLSLShader& shader, const glm::mat4& modelView);
private:
	std::vector<Triangle*> m_triangles;
	std::vector<Light*> m_lights;
	std::unique_ptr<KDTree> m_tree;
	std::vector<SceneObject> m_objects;
};