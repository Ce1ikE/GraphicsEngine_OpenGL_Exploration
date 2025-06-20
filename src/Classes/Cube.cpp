#include "ResourceClasses/Cube.h"


Cube::Cube()
{ 
	// these are points represented in Normalized Device Coordinates (NDC)
	// where the Y-axis is pointing up
	// and the center is in the middle of the window instead of the top left corner , so just a normal cartesian plane.
	// (if you've used OpenGL frameworks before like Pygame (Python) or Love2D (Lua) then normally the Y-axis is pointing down from the top left corner of the screen)
	// see Element Buffer Objects (EBO) on why we add indices and vertices
	std::vector<Vertex> vertices = {
		// positions				// colors
		Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),glm::vec3(1.0f, 0.0f, 0.0f)), // bottom right back
		Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),glm::vec3(1.0f, 0.0f, 0.0f)), // bottom right front
		Vertex(glm::vec3(-0.0f, -0.5f, -0.5f),glm::vec3(0.0f, 0.0f, 1.0f)), // bottom left back
		Vertex(glm::vec3(-0.0f, -0.5f,  0.5f),glm::vec3(0.0f, 0.0f, 1.0f)), // bottom left front
		Vertex(glm::vec3( 0.0f,  0.5f, -0.5f),glm::vec3(0.0f, 0.0f, 1.0f)), // top right back
		Vertex(glm::vec3( 0.0f,  0.5f,  0.5f),glm::vec3(0.0f, 0.0f, 1.0f)), // top right front
		Vertex(glm::vec3(-0.0f,  0.5f, -0.5f),glm::vec3(0.0f, 0.0f, 1.0f)), // top left back
		Vertex(glm::vec3(-0.0f,  0.5f,  0.5f),glm::vec3(0.0f, 0.0f, 1.0f)), // top left front
	};
	// right know these are just static points in space however imagine you have more complicated meshes (e.g.: a robot , human , car , trees , water made in Blender or a other 3D modeling software)
	// how would we move this object ? Well instead of sending a new bunch of vertices , WHICH IS EXPENSIVE, we can send what's called a transformation matrix.
	// this matrix has the same look as the vertices above except that the data does not contain any color or coordinates
	// but instead contains floats telling how these vertices should be transformed (translated , scaled , rotated , skewed , etc...)
	// https://vitaminac.github.io/posts/Matrices-in-Computer-Graphics/

	// 3 Major matrices should be noted (the MVP's):
	// 1) Model Matrix		-> This matrix transforms vertices from a model/mesh's local space to world space
	// 2) View Matrix		-> This matrix represents the camera's position and orientation transforming vertices from world space into the camera's view space
	// 3) Projection Matrix -> This matrix defines how the 3D scene is projected onto a 2D screen (e.g.: perspective or orthographic) transforming view space into clip space (because well... it clips uncessary objects that don't need to be rendered)

	// then we just have to modify these matrices once per object and once per frame 
	// and in our vertex shader we multiply these matrices => projection * view * model * vertexPosition
	// https://learnopengl.com/img/getting-started/coordinate_systems.png

	// note that we start from 0 !!!!
	std::vector<unsigned int> indices = {
        // Front face (facing +Z)
		0, 1, 2,  // bottom-left-front, bottom-right-front, top-left-front
		2, 1, 3,  // top-left-front, bottom-right-front, top-right-front

		// Back face (facing -Z)
		5, 4, 7,  // bottom-right-back, bottom-left-back, top-right-back
		7, 4, 6,  // top-right-back, bottom-left-back, top-left-back

		// Right face (facing +X)
		1, 5, 3,  // bottom-right-front, bottom-right-back, top-right-front
		3, 5, 7,  // top-right-front, bottom-right-back, top-right-back

		// Left face (facing -X)
		4, 0, 6,  // bottom-left-back, bottom-left-front, top-left-back
		6, 0, 2,  // top-left-back, bottom-left-front, top-left-front

		// Top face (facing +Y)
		2, 3, 6,  // top-left-front, top-right-front, top-left-back
		6, 3, 7,  // top-left-back, top-right-front, top-right-back

		// Bottom face (facing -Y)
		4, 5, 0,  // bottom-left-back, bottom-right-back, bottom-left-front
		0, 5, 1   // bottom-left-front, bottom-right-back, bottom-right-front
	};

	GameObject::setMesh(new Mesh(vertices, indices));
	GameObject::setMaterial(new Material(ResourceManager::GetShader("basic")));
};