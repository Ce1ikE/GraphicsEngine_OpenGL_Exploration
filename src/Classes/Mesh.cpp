#include "ResourceClasses/Mesh.h"

Mesh::Mesh() {};

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) 
{
	try
	{
		if (indices.size())
		{
			m_isIndexed = true;
			m_indexCount = indices.size();
		}
		setupMesh(vertices,indices);

	}
	catch (const std::exception& e)
	{
		Logger::error(
			MESSAGE("MESH: Failed to read shader files")
		);
	}
};

Mesh::~Mesh()
{
	freeResources();
};

void Mesh::freeResources()
{
	glDeleteVertexArrays(1, &m_VAO_ID);
	glDeleteBuffers(1, &m_VBO_ID);
	if (m_isIndexed) {
		glDeleteBuffers(1, &m_EBO_ID);
	}
}

void Mesh::bind() const
{
	// once we specify what VAO we want to use to draw something then all the buffer pointers (that are pointing to VBO)
	// are called and passed trough the shaders that we have actived for this specific frame
	// remember that the transformation matrices are "uniform" variables defined inside the shader
	glBindVertexArray(m_VAO_ID);
};

void Mesh::unbind() const
{
	glBindVertexArray(0);
};

void Mesh::draw() const
{
	bind();

	if (m_isIndexed)
	{
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_indexCount);
	}

	unbind();
};

void Mesh::updateVertices(const std::vector<Vertex>& newVertices)
{

};

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	// Sending data to the graphics card (a.k.a. GPU) from the CPU is relatively slow, so wherever we can, we try to send as much data as possible at once
	// We manage this memory via so called vertex buffer objects (VBO) that can store a large number of vertices in the GPU's memory. 
	// The advantage of using those buffer objects is that we can send large batches of data all at once to the graphics card, 
	// and keep it there if there's enough memory left, without having to send data one vertex at a time.

	// first we generate the buffers
	glGenVertexArrays(1, &m_VAO_ID);
	glGenBuffers(1, &m_VBO_ID);
	if (m_isIndexed)
	{
		glGenBuffers(1, &m_EBO_ID);
	}

	// next we bind the newly created buffers
	// bind the Vertex Array Object first
	glBindVertexArray(m_VAO_ID);

	// then we copy our data to the buffer
	// The fourth parameter specifies how we want the graphics card to manage the given data. This can take 3 forms:
	// 1) GL_STREAM_DRAW	: the data is set only once and used by the GPU at most a few times
	// 2) GL_STATIC_DRAW	: the data is set only once and used many times
	// 3) GL_DYNAMIC_DRAW	: the data is changed a lot and used many times
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	if (m_isIndexed)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	// we sent the input vertex data to the GPU 
	// instructed the GPU how it should process the vertex data within a vertex and fragment shader however...
	// OpenGL does not yet know how it should interpret the vertex data in memory  (floats , ints, doubles , 2 or 3 or 4 values)
	// and how it should connect the vertex data to the vertex shader's attributes

	// we can tell OpenGL how it should interpret the vertex data (per vertex attribute(positions , normals, etc...)) using
	// this following line basically says :
	// at location "0" of the buffer (VBO)
	// you'll find vertices with "3" "float" components
	// which should "not" be normalized (not clamped/cliped between -1.0 and 1.0)(they already are)
	// with a stride (byte offset) of "6" "of size" "float"
	// this data starts at offset "0"

	// Position attribute at location 0 has 3 floats 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,m_position));
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
	glEnableVertexAttribArray(1);

	// VOB memory layout (all floats):
	// | ----- Vertex 1 ----- | ----- Vertex 2 ----- | ----- Vertex 2 ----- | ----- etc... ----- |
	//   X , Y , Z , R , G , B, X , Y , Z , R , G , B, X , Y , Z , R , G , B , etc...

	// you don't have to specify the "layout" of you're data you can query for the attribute locations in the OpenGL code 
	// however this way saves OpenGL and us as a programmer some work 

	// binding to 0 resets it to NULL
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// do NOT unbind the EBO while a VAO is active
	// the EBO buffer object IS stored in the VAO
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// what is a VAO ?
	// what we did so far :
	// --------------------
	// (vertices -> VBO)
	// 1) copy our vertices array in a buffer for OpenGL to use 
	// 
	// (vertex shader + frag shader ->  shaderProgram)
	// 2) create a shader program by compiling shaders and linking them 
	// 
	// (dataShape)
	// 3) tell OpenGL how to interpret our current data inside the VBO
	// 
	// (draw object) 
	// 4) by using the shader program we can finally draw a object 

	// however there is a slight problem , we have to repeat this process every time we want to draw an object
	// the reason is because you're VBO is just an array of raw data that the GPU stores and can access for various processes
	// one of those "various processes" that the GPU can use them for is providing array data for vertex rendering operations
	// however there is no rule that says that a buffer you're currently using for vertex data cannot be used for other buffer usages too.
	// This is where a vertex array object (VAO) comes into play which is a object that stores 1 or more VBO's
	// and is designed to store the information (the state) necessary for drawing/rendering a object.
	// This reduces OpenGL calls of re-specifying the vertex format and bindings each time, you simply bind the VAO.

	// VBO -> contains the raw data (can be reused for multiple processes e.g.: drawing 50 of the same trees or 10 of the same road blocks)
	// VAO -> contains the configuration for how to use that data
};


