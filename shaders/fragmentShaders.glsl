// just a version field
#version 420 core

// this the input of the program from the previous program
in vec3 vertexPosition;

// a uniform variable are globals. They can be accessed from any shader at any stage within the shader program
// whatever you set the uniform value to, uniforms will keep their values until they're either reset or updated
// so that means we have a other way of passing variable to our program
uniform float intensity;

// this is the output of the program
out vec4 FragColor;

// just as in the vertex shader we have a main function
void main()
{
	FragColor = vec4(vertexPosition.xyz, 1.0);
};
	