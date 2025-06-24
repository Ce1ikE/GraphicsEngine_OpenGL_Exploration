// just a version field
#version 420 core


// a optimization for the shaders you can have is :

// Think about how many vertices you have vs how many pixels on your screen 
// these days you can have WAY more pixels then vertices (especially in 4K) e.g.: 1080p HD resolution : 1920x1080 = 2.073.600 pixels (A lot)
// so handling some matrices multiplication would be smarter in the vertex shader then in the fragement shader
// however if something is far away from your camera and only equals to 30 pixels through your camera it becomes more expensive doing it in the vertex shader then in the fragement shader
 



// the "in" keyword defines the input we can give to the program
// the "layout" tells us in which location (previously set in the main function)
// we can find the data
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

// the "out" keyword defines the output the program gives to the next program (the fragment shader)
// here we specify a color output to the fragment shader
out vec3 vertexPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


// GLSL just as in C and C++ has a main function in which the function is executed
void main()
{
	gl_Position = projection * view * model * vec4(aPos,1.0);

	vertexPosition = aPos.xyz;
};
