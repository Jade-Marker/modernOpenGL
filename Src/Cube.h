#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include "Structures.h"
#include "InputManager.h"

#include "Shader.h"
#include "Vao.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Material.h"

#include "SceneObject.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class Cube: public SceneObject
{
private:
	static float vertexData[];
	static unsigned int indices[];

public:
	Cube(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material);
	~Cube();
	void Render();
	void Update(float deltaTime);
};

