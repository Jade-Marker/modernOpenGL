#include "SceneObject.h"
#include "Camera.h"

SceneObject::SceneObject(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect):
	_shader(shader), _texture(texture), _mesh(mesh), _material(material), _camera(camera), _worldTransform(1.0f), _transform(transform), _collisionRect(collisionRect)
{
	_isTransparent = texture->GetTransparent();
	_uniqueMesh = false;

	_vao = new Vao();
	_vao->BindVao();

	BufferLayout layout;

	layoutElement positionsLayout = layoutElement(3, GL_FLOAT, false, mesh->GetVertexCount());
	layout.AddElement(positionsLayout);
	layoutElement textureLayout = layoutElement(2, GL_FLOAT, false, mesh->GetUVCount());
	layout.AddElement(textureLayout);
	layoutElement normalLayout = layoutElement(3, GL_FLOAT, false, mesh->GetVertexNormalCount());
	layout.AddElement(normalLayout);

	_vao->CreateVertexBuffer(_mesh, layout);
	_vao->CreateIndexBuffer(_mesh);

	_shader->SetUniformInt(0, "u_Texture");
}

SceneObject::SceneObject(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect):
	_shader(shader), _texture(texture), _material(material), _camera(camera), _worldTransform(1.0f), _transform(transform), _collisionRect(collisionRect)
{
	_isTransparent = texture->GetTransparent();

	_mesh = new Mesh(meshPath);
	_uniqueMesh = true;

	_vao = new Vao();
	_vao->BindVao();

	BufferLayout layout;

	layoutElement positionsLayout = layoutElement(3, GL_FLOAT, false, _mesh->GetVertexCount());
	layout.AddElement(positionsLayout);
	layoutElement textureLayout = layoutElement(2, GL_FLOAT, false, _mesh->GetUVCount());
	layout.AddElement(textureLayout);
	layoutElement normalLayout = layoutElement(3, GL_FLOAT, false, _mesh->GetVertexNormalCount());
	layout.AddElement(normalLayout);

	_vao->CreateVertexBuffer(_mesh, layout);
	_vao->CreateIndexBuffer(_mesh);

	_shader->SetUniformInt(0, "u_Texture");
}

SceneObject::~SceneObject()
{
	delete _vao;

	for (int i = 0; i < _children.size(); i++)
		delete _children[i];

	if (_uniqueMesh)
		delete _mesh;

	_children.clear();
}

void SceneObject::Render()
{
	//set uniforms
	glm::mat4 transformMatrix =
		_worldTransform *
		glm::translate(glm::mat4(1.0f), _transform.position) *
		glm::eulerAngleXYZ(_transform.rotation.x, _transform.rotation.y, _transform.rotation.z) *
		glm::scale(glm::mat4(1.0f), _transform.scale);
	_shader->SetUniformMatrix(transformMatrix, "u_Transform");

	_material->SetMaterialUniforms(_shader);

	Renderer renderer;
	renderer.Render(_vao, _shader, _texture);
}

void SceneObject::Update(float deltaTime)
{
	for (int i = 0; i < _children.size(); i++)
	{
		_children[i]->Update(deltaTime);
	}
}

void SceneObject::OnTrigger()
{
}

const Transform SceneObject::GetTransform()
{
	return _transform;
}

void SceneObject::SetTransform(Transform transform)
{
	_transform = transform;
}

void SceneObject::AddChild(SceneObject* child)
{
	_children.push_back(child);
}

void SceneObject::DeleteChild(int index)
{
	if(_children.size() > index)
		_children.erase(_children.begin() + index);
}

std::vector<SceneObject*> const SceneObject::GetChildren()
{
	return _children;
}

void SceneObject::GetAllObjects(std::vector<SceneObject*>& transparentObjects, std::vector<SceneObject*>& opaqueObjects)
{
	if (_isTransparent)
		transparentObjects.push_back(this);
	else
		opaqueObjects.push_back(this);

	for (int i = 0; i < _children.size(); i++)
		_children[i]->GetAllObjects(transparentObjects, opaqueObjects);
}

void SceneObject::GetAllObjects(std::vector<SceneObject*>& objects)
{
	objects.push_back(this);

	for (int i = 0; i < _children.size(); i++)
		_children[i]->GetAllObjects(objects);
}

void SceneObject::SetChildrenWorldTransform(glm::mat4& worldTransform)
{
	_worldTransform = worldTransform;

	glm::mat4 transformMatrix =
		glm::translate(glm::mat4(1.0f), _transform.position) *
		glm::eulerAngleXYZ(_transform.rotation.x, _transform.rotation.y, _transform.rotation.z) *
		glm::scale(glm::mat4(1.0f), _transform.scale);
	transformMatrix = worldTransform * transformMatrix;

	for (int i = 0; i < _children.size(); i++)
		_children[i]->SetChildrenWorldTransform(transformMatrix);
}

bool distanceComparison(SceneObject* object, SceneObject* other)
{
	glm::vec3 position = object->GetTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec3 otherPosition = other->GetTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float distToCamera = glm::distance(position, object->GetCamera()->GetPosition());
	float distFromOtherToCamera = glm::distance(otherPosition, other->GetCamera()->GetPosition());

	return distToCamera > distFromOtherToCamera;
}
