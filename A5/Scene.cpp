#include "Scene.hpp"

#include <string>
#include <iostream>

#include "GLSLShader.hpp"
#include "KDTree.hpp"
#include "Resources.hpp"
#include "Triangle.hpp"
#include "Ray.hpp"

#define GLM_FORCE_RADIANS
#include "glm\glm\gtx\transform.hpp"

#include "Light.hpp"
#include "Texture.hpp"

using namespace glm;
using namespace std;

const int MAX_LIGHTS = 5;
static Material material1 = Material(vec4(0.7f, 0.1f, 0.1f, 1.f), vec4(0.7f, 0.1f, 0.1f, 1.f),
									 vec4(1.f, 1.f, 1.f, 1.f), 170);
static Material material2 = Material(vec4(0.3f, 0.3f, 0.3f, 1.f), vec4(0.3f, 0.3f, 0.3f, 1.f),
									 vec4(0.2f, 0.2f, 0.2f, 0.2f), 50);

Scene::Scene() {
}
Scene::~Scene() {
	for each(Triangle* t in m_triangles) {
		delete t;
	}
	for each(Light* l in m_lights) {
		delete l;
	}
}

void Scene::AddObject(std::string modelName, int mat,//std::string materialName,
					  glm::mat4 modelMatrix) {
	TriMesh* mesh = Resources::Get<TriMesh>(modelName);
	assert(mesh);
	// Material* mat = Resources::Get<TriMesh>(materialName);

	for each(glm::vec3 f in mesh->faces) {
		// Create a transformed triangle
		Triangle* t = new Triangle(
			vec3(modelMatrix * vec4(mesh->positions[f.x], 1)),
			vec3(modelMatrix * vec4(mesh->positions[f.y], 1)),
			vec3(modelMatrix * vec4(mesh->positions[f.z], 1))
			);

		// Add the uv coordinates if existing
		if(!mesh->texCoords.empty()) {
			t->uv(0) = mesh->texCoords[f.x];
			t->uv(1) = mesh->texCoords[f.y];
			t->uv(2) = mesh->texCoords[f.z];
		}

		// Calculate the face normal by interpolating the vertex normals
		if(!mesh->normals.empty()) {
			// Midpoint has barycentric coordinates a = b = c = 1/3

			auto vnx = mesh->normals[f.x];
			auto vny = mesh->normals[f.y];
			auto vnz = mesh->normals[f.z];

			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3x3(modelMatrix)));
			t->normal() = normalize(1.f / 3.f *
									(normalize(normalMatrix*vnx) +
									normalize(normalMatrix*vny) +
									normalize(normalMatrix*vnz)));

			t->vNormal(0) = normalMatrix*vnx;
			t->vNormal(1) = normalMatrix*vny;
			t->vNormal(2) = normalMatrix*vnz;
		}

		t->oid() = m_objects.size();

		m_triangles.push_back(t);
	}

	SceneObject o;
	o.mesh = mesh;
	o.transformation = modelMatrix;
	o.material = mat == 0 ? &material1 : &material2;

	m_objects.push_back(o);
}
void Scene::AddLight(glm::vec4 position, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular) {
	if(m_lights.size() >= MAX_LIGHTS)
		return;

	Light* l = new Light();
	l->position = position;
	l->ambient = ambient;
	l->diffuse = diffuse;
	l->specular = specular;

	m_lights.push_back(l);
}
void Scene::BuildKDTree() {
	m_tree.reset(new KDTree(m_triangles));
}

void Scene::RenderScene(GLSLShader& shader, const mat4& modelView, const mat4& projection) {
	shader.bind();
	Light* first = m_lights[0];

	for each(SceneObject o in m_objects) {
		// TODO: Apply the model matrix for each mode to the model view matrix
		shader.setUniform("modelView", modelView * o.transformation);
		shader.setUniform("normalMatrix", glm::transpose(glm::inverse(modelView * o.transformation)));
		shader.setUniform("modelViewProjection", projection * modelView * o.transformation);
		shader.setUniform("lighting", true);
		shader.setUniform("showTexture", false);

		// TODO: Apply the model view to the light sources
		//shader.setUniform("cameraEye", )

		BindLights(shader, modelView);

		shader.setUniform("material.ambient", o.material->getAmbient());
		shader.setUniform("material.diffuse", o.material->getDiffuse());
		shader.setUniform("material.specular", o.material->getSpecular());
		shader.setUniform("material.shininess", o.material->getShininess());

		o.mesh->draw();
	}
	shader.unbind();

	// Render the light dots
	for each(Light* l in m_lights) {
		glColor3f(l->diffuse.x, l->diffuse.y, l->diffuse.z);
		glPushMatrix();
		glTranslatef(l->position.x, l->position.y, l->position.z);
		glutSolidSphere(0.05f, 20, 20);
		glPopMatrix();
	}
}
void Scene::RenderTree() {
	m_tree->Render();
}
void Scene::BindLights(GLSLShader& shader, const mat4& modelView)  {
	for(int i = 0; i < MAX_LIGHTS; ++i) {
		if(i >= m_lights.size())
			break;

		string chunk = "lightSource[";
		chunk += to_string(i);
		chunk += "]";

		shader.setUniform(chunk + ".position", modelView * m_lights[i]->position);
		shader.setUniform(chunk + ".ambient", m_lights[i]->ambient);
		shader.setUniform(chunk + ".diffuse", m_lights[i]->diffuse);
		shader.setUniform(chunk + ".specular", m_lights[i]->specular);
	}

	shader.setUniform("numLights", (float)m_lights.size());
}

void Scene::BuildScene(Scene& scene) {
	scene.AddObject("quad", 1,
					glm::rotate(-radians(90.f), vec3(1, 0, 0)) * glm::scale(vec3(5.f, 5.f, 5.f)));
	scene.AddObject("sphere", 0,
					translate(vec3(0.f, 0.75f, 0.f)));
	scene.AddObject("shuttle", 1,
					translate(vec3(2.5f, 0.75f, 1.f))* rotate(-glm::half_pi<float>(), vec3(1.f, 0.f, 0.f)));
	/*scene.AddObject("shuttle", "",
					translate(vec3(1.25f, 1.25f, 0.f)) * rotate(-glm::quarter_pi<float>(), vec3(0.f, 0.f, 1.f)) * rotate(-glm::half_pi<float>(), vec3(1.f, 0.f, 0.f)));*/
	/*scene.AddObject("auto", 0,
					translate(vec3(0.f, 0.25f, 1.f)) * rotate(glm::quarter_pi<float>(), vec3(0.f, 1.f, 0.f)));*/
	scene.AddLight(vec4(1.f, 2.f, 2.f, 1.f), 
				   vec4(0.1f, 0.1f, 0.1f, 1.f), vec4(1.f, 1.0f, 1.0f, 1.f), vec4(1.f, 1.0f, 1.0f, 1.f));
	scene.AddLight(vec4(-3.f, 2.f, 1.5f, 1.f),
				   vec4(0.f, 0.f, 0.f, 1.f), vec4(1.0f, 0.0f, 0.675f, 1.f), vec4(1.f, 1.0f, 1.0f, 1.f));

	// Print some scene info
	cout << "[Scene]" << endl;
	cout << "Objects : " << to_string(scene.m_objects.size()) << endl;
	cout << "Lights  : " << to_string(scene.m_lights.size()) << endl << endl;
}
bool Scene::Hit(const Ray& r, Ray::HitInfo& rh) {
	return m_tree->hit(r, rh);
}