# GraphicsEngine_OpenGL_Exploration
a small repo exploring the OpenGL library along with other libraries such ImGui and Assimp. In order to create a minimal Graphics Engine 


## how to run this project ?

### Using Visual Studio (Windows)
	1) Generate Visual Studio Project Files:
		- Open CMake GUI (CMake's graphical interface)
		- Browse to your project's root directory (where CMakeLists.txt is)
		- Create a new folder (e.g., build) inside your project root and select it
		- Click Configure. Choose your desired Visual Studio version and platform (e.g., "x64") and click finish
		- Click Generate
	2) Open in Visual Studio:
		- Navigate to your build directory.
		- Open the generated .sln (solution) file
	3) Build:
		- In Visual Studio, go to Build > Build Solution
	4) Run:
		- Select your executable project as the "Startup Project" in the Solution Explorer (right-click project -> Set as Startup Project).
		- Press Ctrl + F5 (Start without Debugging) or F5 (Start Debugging).

### Without a GUI/IDE (Command Line)
	1) Open your Terminal/Command Prompt and navigate to the project root
		- cd path/to/folder
	2) Make a 'build' directory 
		- mkdir build && cd build
	3) Generate the platform-specific build files:
		- cmake ..
	4) Build:
		- cmake --build .
	5) Run:
		- the .exe (executable) can be found where cmake made it's files
