#include "Cube.h"


Cube::Cube()
{ 
	// A vertex is NOT just a spatial position, but a whole bag of attributes:
	// - A position p is a point in some spatial space or a homogeneous coordinate.
	// - A texcoord tc is a point in texture space.
	// - A normal n is a bivector, and so on.

	// a Cube has 8 unique vertices (4 bottom square + 4 top square + 4 connecting corners)
	// and 36 indices (6 faces * 2 triangles per face * 3 vertices per triangle)
	std::vector<Vertex> vertices = {
		// positions                           // colors
		Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f)), // 0
		Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f)), // 1
		Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f)), // 2
		Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f)), // 3
		Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 1.0f)), // 4
		Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 1.0f)), // 5
		Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f)), // 6
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.5f, 0.5f, 0.5f))  // 7
	};
	// Vertex::positions
	// these are points represented in Normalized Device Coordinates (NDC)
	// where the Y-axis is pointing up
	// and the center is in the middle of the window instead of the top left corner , so just a normal cartesian plane.
	// (if you've used OpenGL frameworks before like Pygame (Python) or Love2D (Lua) then normally the Y-axis is pointing down from the top left corner of the screen)
	// see Element Buffer Objects (EBO) on why we add indices and vertices
	// right know these are just static points in space however imagine you have more complicated meshes (e.g.: a robot , human , car , trees , water made in Blender or a other 3D modeling software)
	// how would we move this object ? Well instead of sending a new bunch of vertices , WHICH IS EXPENSIVE, we can send what's called a transformation matrix.
	// this matrix has the same look as the vertices above except that the data does not contain any color or coordinates
	// but instead contains floats telling how these vertices should be transformed (translated , scaled , rotated , skewed , etc...)
	// https://vitaminac.github.io/posts/Matrices-in-Computer-Graphics/

	// note that we start from 0 !!!!
	std::vector<unsigned int> indices = {
		// Back face
		0, 1, 2,
		2, 3, 0,

		// Front face
		4, 5, 6,
		6, 7, 4,

		// Left face
		4, 0, 3,
		3, 7, 4,

		// Right face
		1, 5, 6,
		6, 2, 1,

		// Bottom face
		4, 5, 1,
		1, 0, 4,

		// Top face
		3, 2, 6,
		6, 7, 3
	};

	GameObject::setMesh(new Mesh(vertices, indices));
	GameObject::setMaterial(new Material(ResourceManager::GetShader(STD_SHADER)));
};

Cube::Cube(glm::vec3& cubePosition)
	:Cube()
{
	setPosition(cubePosition);
}
