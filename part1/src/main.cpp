
/* Compilation on Linux:
 g++ -std=c++17 ./src/*.cpp -o prog -I ./include/ -I./../common/thirdparty/ -lSDL2 -ldl
*/

// Third Party Libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// C++ Standard Template Library (STL)
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Our libraries
#include <Camera.hpp>
#include <OBJModel.hpp>
#include <Light.hpp>
#include <Texture.hpp>

// vvvvvvvvvvvvvvvvvvvvvvvvvv Globals vvvvvvvvvvvvvvvvvvvvvvvvvv
// Globals generally are prefixed with 'g' in this application.

// Screen Dimensions
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

// Main loop flag
bool gQuit = false; // If this is quit = 'true' then the program terminates.

// shader
// The following stores the a unique id for the graphics pipeline
// program object that will be used for our OpenGL draw calls.
GLuint gGraphicsPipelineShaderProgram = 0;
// Special Graphics Pipeline that does debugging for us
GLuint gGraphicsPipelineShaderProgramDebug = 0;

std::vector<GLfloat> gVertexData{};
std::vector<GLuint> gIndexBufferData{};
std::string gTextureFilename = "";
std::string gNormalMapFilename = "";

std::vector<std::string> gObjectFilenames;

// OpenGL Objects
// Vertex Array Object (VAO)
// Vertex array objects encapsulate all of the items needed to render an object.
// For example, we may have multiple vertex buffer objects (VBO) related to rendering one
// object. The VAO allows us to setup the OpenGL state to render that object using the
// correct layout and correct buffers with one call after being setup.
GLuint gVertexArrayObject = 0;
// Vertex Buffer Object (VBO)
// Vertex Buffer Objects store information relating to vertices (e.g. positions, normals, textures)
// VBOs are our mechanism for arranging geometry on the GPU.
GLuint gVertexBufferObject = 0;
// Index Buffer Object (IBO)
// This is used to store the array of indices that we want
// to draw from, when we do indexed drawing.
GLuint gIndexBufferObject = 0;

// A second object for drawing a normal
GLuint gVertexArrayObjectForNormal = 0;
// Vertex Buffer Object (VBO)
// Vertex Buffer Objects store information relating to vertices (e.g. positions, normals, textures)
// VBOs are our mechanism for arranging geometry on the GPU.
GLuint gVertexBufferObjectForNormal = 0;

// Shaders
// Here we setup two shaders, a vertex shader and a fragment shader.
// At a minimum, every Modern OpenGL program needs a vertex and a fragment
// shader.
float g_uOffset = 0.0f;

// Camera
Camera gCamera;

// Light
std::vector<Light> gLights{Light(2.5f, 1.2f, 1)};

// Texture
Texture gTexture;

// Normal Map
Texture gNormalMap;

// Draw wireframe mode
GLenum gPolygonMode = GL_FILL;

// ^^^^^^^^^^^^^^^^^^^^^^^^ Globals ^^^^^^^^^^^^^^^^^^^^^^^^^^^

// vvvvvvvvvvvvvvvvvvv Error Handling Routines vvvvvvvvvvvvvvv
static void GLClearAllErrors()
{
	while (glGetError() != GL_NO_ERROR)
	{
	}
}

// Returns true if we have an error
static bool GLCheckErrorStatus(const char *function, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OpenGL Error:" << error
				  << "\tLine: " << line
				  << "\tfunction: " << function << std::endl;
		return true;
	}
	return false;
}

#define GLCheck(x)      \
	GLClearAllErrors(); \
	x;                  \
	GLCheckErrorStatus(#x, __LINE__);
// ^^^^^^^^^^^^^^^^^^^ Error Handling Routines ^^^^^^^^^^^^^^^

/**
 * LoadShaderAsString takes a filepath as an argument and will read line by line a file and return a string that is meant to be compiled at runtime for a vertex, fragment, geometry, tesselation, or compute shader.
 * e.g.
 *       LoadShaderAsString("./shaders/filepath");
 *
 * @param filename Path to the shader file
 * @return Entire file stored as a single string
 */
std::string LoadShaderAsString(const std::string &filename)
{
	// Resulting shader program loaded as a single string
	std::string result = "";

	std::string line = "";
	std::ifstream myFile(filename.c_str());

	if (myFile.is_open())
	{
		while (std::getline(myFile, line))
		{
			result += line + '\n';
		}
		myFile.close();
	}

	return result;
}

/**
 * CompileShader will compile any valid vertex, fragment, geometry, tesselation, or compute shader.
 * e.g.
 *	    Compile a vertex shader: 	CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
 *       Compile a fragment shader: 	CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
 *
 * @param type We use the 'type' field to determine which shader we are going to compile.
 * @param source : The shader source code.
 * @return id of the shaderObject
 */
GLuint CompileShader(GLuint type, const std::string &source)
{
	// Compile our shaders
	GLuint shaderObject;

	// Based on the type passed in, we create a shader object specifically for that
	// type.
	if (type == GL_VERTEX_SHADER)
	{
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == GL_FRAGMENT_SHADER)
	{
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	const char *src = source.c_str();
	// The source of our shader
	glShaderSource(shaderObject, 1, &src, nullptr);
	// Now compile our shader
	glCompileShader(shaderObject);

	// Retrieve the result of our compilation
	int result;
	// Our goal with glGetShaderiv is to retrieve the compilation status
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		char *errorMessages = new char[length]; // Could also use alloca here.
		glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

		if (type == GL_VERTEX_SHADER)
		{
			std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n"
					  << errorMessages << "\n";
		}
		else if (type == GL_FRAGMENT_SHADER)
		{
			std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n"
					  << errorMessages << "\n";
		}
		// Reclaim our memory
		delete[] errorMessages;

		// Delete our broken shader
		glDeleteShader(shaderObject);

		return 0;
	}

	return shaderObject;
}

/**
 * Creates a graphics program object (i.e. graphics pipeline) with a Vertex Shader and a Fragment Shader
 *
 * @param vertexShaderSource Vertex source code as a string
 * @param fragmentShaderSource Fragment shader source code as a string
 * @return id of the program Object
 */
GLuint CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{

	// Create a new program object
	GLuint programObject = glCreateProgram();

	// Compile our shaders
	GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	// Link our two shader programs together.
	// Consider this the equivalent of taking two .cpp files, and linking them into
	// one executable file.
	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFragmentShader);
	glLinkProgram(programObject);

	// Validate our program
	glValidateProgram(programObject);

	// Once our final program Object has been created, we can
	// detach and then delete our individual shaders.
	glDetachShader(programObject, myVertexShader);
	glDetachShader(programObject, myFragmentShader);
	// Delete the individual shaders once we are done
	glDeleteShader(myVertexShader);
	glDeleteShader(myFragmentShader);

	return programObject;
}

/**
 * Create the graphics pipeline
 *
 * @return void
 */
void CreateGraphicsPipeline()
{

	std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
	std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

	gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

/**
 * Initialization of the graphics application. Typically this will involve setting up a window
 * and the OpenGL Context (with the appropriate version)
 *
 * @return void
 */
void InitializeProgram()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Setup the OpenGL Context
	// Use OpenGL 4.1 core or greater
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// We want to request a double buffer for smooth updating.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create an application window using OpenGL that supports SDL
	gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL First Program",
												  SDL_WINDOWPOS_UNDEFINED,
												  SDL_WINDOWPOS_UNDEFINED,
												  gScreenWidth,
												  gScreenHeight,
												  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	// Check if Window did not create.
	if (gGraphicsApplicationWindow == nullptr)
	{
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Create an OpenGL Graphics Context
	gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
	if (gOpenGLContext == nullptr)
	{
		std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Initialize GLAD Library
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		std::cout << "glad did not initialize" << std::endl;
		exit(1);
	}
}

// Little helper function for creating edges from two vertices
glm::vec3 CreateEdgeFromTwoVertices(float x1, float y1, float z1,
									float x2, float y2, float z2)
{
	return glm::vec3(x2 - x1, y2 - y1, z2 - z1);
}

float avgOfThree(float a, float b, float c)
{
	return (a + b + c) / 3;
}

/**
 * Setup your geometry during the vertex specification step
 *
 * @return void
 */
void VertexSpecification()
{
	// load texture
	gTexture.LoadTexture(gTextureFilename);
	gNormalMap.LoadTexture(gNormalMapFilename);

	// Vertex Arrays Object (VAO) Setup
	// Note: We can think of the VAO as a 'wrapper around' all of the Vertex Buffer Objects,
	//       in the sense that it encapsulates all VBO state that we are setting up.
	//       Thus, it is also important that we glBindVertexArray (i.e. select the VAO we want to use)
	//       before our vertex buffer object operations.
	glGenVertexArrays(1, &gVertexArrayObject);
	// We bind (i.e. select) to the Vertex Array Object (VAO) that we want to work withn.
	glBindVertexArray(gVertexArrayObject);

	// Vertex Buffer Object (VBO) creation
	// Create a new vertex buffer object
	// Note:  We’ll see this pattern of code often in OpenGL of creating and binding to a buffer.
	glGenBuffers(1, &gVertexBufferObject);
	// Next we will do glBindBuffer.
	// Bind is equivalent to 'selecting the active buffer object' that we want to
	// work with in OpenGL.
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	// Now, in our currently binded buffer, we populate the data from our
	// 'vertexPositions' (which is on the CPU), onto a buffer that will live
	// on the GPU.
	glBufferData(GL_ARRAY_BUFFER,						// Kind of buffer we are working with
														// (e.g. GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER)
				 gVertexData.size() * sizeof(GL_FLOAT), // Size of data in bytes
				 gVertexData.data(),					// Raw array of data
				 GL_STATIC_DRAW);						// How we intend to use the data

	// Index buffer data for a quad
	// const std::vector<GLuint> indexBufferData {0,1,2};
	// Setup the Index Buffer Object (IBO i.e. EBO)
	glGenBuffers(1, &gIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
				 gIndexBufferObject);
	// Populate our Index Buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 gIndexBufferData.size() * sizeof(GLuint),
				 gIndexBufferData.data(),
				 GL_STATIC_DRAW);

	// For our Given Vertex Array Object, we need to tell OpenGL
	// 'how' the information in our buffer will be used.
	glEnableVertexAttribArray(0);
	// For the specific attribute in our vertex specification, we use
	// 'glVertexAttribPointer' to figure out how we are going to move
	// through the data.

	int numberOfFloatsPerVertex = 17;

	// x,y,z positions
	glVertexAttribPointer(0,					 // Attribute 0 corresponds to the enabled glEnableVertexAttribArray
												 // In the future, you'll see in our vertex shader this also correspond
												 // to (layout=0) which selects these attributes.
						  3,					 // The number of components (e.g. x,y,z = 3 components)
						  GL_FLOAT,				 // Type
						  GL_FALSE,				 // Is the data normalized
						  sizeof(GL_FLOAT) * numberOfFloatsPerVertex, // Stride
						  (void *)0				 // Offset
	);

	// vertex colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
						  3,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(GL_FLOAT) * numberOfFloatsPerVertex,
						  (void *)(sizeof(GL_FLOAT) * 3));

	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
						  3,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(GL_FLOAT) * numberOfFloatsPerVertex,
						  (void *)(sizeof(GL_FLOAT) * 6));

	// texture coordinates
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,
						  2,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(GL_FLOAT) * numberOfFloatsPerVertex,
						  (void *)(sizeof(GL_FLOAT) * 9));

	// tangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4,
						  3,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(GL_FLOAT) * numberOfFloatsPerVertex,
						  (void *)(sizeof(GL_FLOAT) * 11));

	// bitangent
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5,
						  3,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(GL_FLOAT) * numberOfFloatsPerVertex,
						  (void *)(sizeof(GL_FLOAT) * 14));


	// Unbind our currently bound Vertex Array Object
	glBindVertexArray(0);
	// Disable any attributes we opened in our Vertex Attribute Arrray,
	// as we do not want to leave them open.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
}

/**
 * PreDraw
 * Typically we will use this for setting some sort of 'state'
 * Note: some of the calls may take place at different stages (post-processing) of the
 * 		 pipeline.
 * @return void
 */
void PreDraw()
{
	// Disable depth test and face culling.
	glEnable(GL_DEPTH_TEST); // NOTE: Need to enable DEPTH Test
	glDisable(GL_CULL_FACE);

	// Set the polygon fill mode
	glPolygonMode(GL_FRONT_AND_BACK, gPolygonMode);

	// Initialize clear color
	// This is the background of the screen.
	glViewport(0, 0, gScreenWidth, gScreenHeight);
	glClearColor(0.1f, 4.f, 7.f, 1.f);

	// Clear color buffer and Depth Buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Use our shader
	glUseProgram(gGraphicsPipelineShaderProgram);

	// Model transformation by translating our object into world space
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// MVP Matrices

	// Retrieve our location of our Model Matrix
	GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
	if (u_ModelMatrixLocation >= 0)
	{
		glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
	}
	else
	{
		std::cout << "Could not find u_ModelMatrix, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// Update the View Matrix
	GLint u_ViewMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ViewMatrix");
	if (u_ViewMatrixLocation >= 0)
	{
		glm::mat4 viewMatrix = gCamera.GetViewMatrix();
		glUniformMatrix4fv(u_ViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	}
	else
	{
		std::cout << "Could not find u_ViewMatrix, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// Projection matrix (in perspective)
	glm::mat4 perspective = glm::perspective(glm::radians(45.0f),
											 (float)gScreenWidth / (float)gScreenHeight,
											 0.1f,
											 200.0f);

	// Retrieve our location of our perspective matrix uniform
	GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");
	if (u_ProjectionLocation >= 0)
	{
		glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
	}
	else
	{
		std::cout << "Could not find u_Projection, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// VIEW POSITION

	GLint u_ViewPosition = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_viewPosition");
	if (u_ViewPosition >= 0)
	{
		glm::vec3 viewPosition = glm::vec3(gCamera.GetEyeXPosition(), gCamera.GetEyeYPosition(), gCamera.GetEyeZPosition());
		glUniform3fv(u_ViewPosition, 1, &viewPosition[0]);
	}
	else
	{
		std::cout << "Could not find u_viewPosition, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// LIGHTING

	GLint u_lightCount = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_lightCount");
	if (u_lightCount >= 0)
	{
		glUniform1i(u_lightCount, gLights.size());
	}
	else
	{
		std::cout << "Could not find u_lightCount, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	GLint u_lightPositions = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_lights.positions");
	if (u_lightPositions >= 0)
	{
		std::vector<glm::vec3> lightPositions;
		for (Light &light : gLights)
		{
			lightPositions.push_back(light.GetPosition());
		}
		glUniform3fv(u_lightPositions, lightPositions.size(), &lightPositions[0][0]);
	}
	else
	{
		std::cout << "Could not find u_lights.positions, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	GLint u_lightColor = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_lights.colors");
	if (u_lightColor >= 0)
	{
		std::vector<glm::vec3> lightColors;
		for (Light &light : gLights)
		{
			lightColors.push_back(light.GetColor());
		}
		glUniform3fv(u_lightColor, lightColors.size(), &lightColors[0][0]);
	}
	else
	{
		std::cout << "Could not find u_lights.colors, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	GLint u_lightConstant = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_lights.constant");
	if (u_lightConstant >= 0)
	{
		glUniform1f(u_lightConstant, 1.0f);
	}
	else
	{
		std::cout << "Could not find u_lights.constant, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	GLint u_lightLinear = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_lights.linear");
	if (u_lightLinear >= 0)
	{
		glUniform1f(u_lightLinear, 0.09f);
	}
	else
	{
		std::cout << "Could not find u_lights.linear, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	GLint u_lightQuadratic = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_lights.quadratic");
	if (u_lightQuadratic >= 0)
	{
		glUniform1f(u_lightQuadratic, 0.032f);
	}
	else
	{
		std::cout << "Could not find u_lights.quadratic, maybe a mispelling?\n";
		exit(EXIT_FAILURE);
	}

	// TEXTURES

	// From Professor Shah's example code
	// Bind our texture to slot number 0
	gTexture.Bind(0);

	// Setup our uniform for our texture
	GLint u_textureLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_DiffuseTexture");
	if (u_textureLocation >= 0)
	{
		// Setup the slot for the texture
		glUniform1i(u_textureLocation, 0);
	}
	else
	{
		std::cout << "Could not find u_DiffuseTexture, maybe a misspelling?" << std::endl;
		exit(EXIT_FAILURE);
	}

	gNormalMap.Bind(1);

	// Setup our uniform for our normal map
	GLint u_normalMapLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_BumpMap");
	if (u_normalMapLocation >= 0)
	{
		// Setup the slot for the texture
		glUniform1i(u_normalMapLocation, 1);
	}
	else
	{
		std::cout << "Could not find u_BumpMap, maybe a misspelling?" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Draw
 * The render function gets called once per loop.
 * Typically this includes 'glDraw' related calls, and the relevant setup of buffers
 * for those calls.
 *
 * @return void
 */
void Draw()
{

	// Render OBJ
	glBindVertexArray(gVertexArrayObject);
	glDrawElements(GL_TRIANGLES,
				   gIndexBufferData.size(),
				   GL_UNSIGNED_INT,
				   0);

	// render lights
	for (Light &light : gLights)
	{
		glBindVertexArray(light.GetVAO());
		glDrawElements(GL_TRIANGLES, light.GetIndexBufferData().size(), GL_UNSIGNED_INT, 0);
	}

	glUseProgram(0);
}

/**
 * Helper Function to get OpenGL Version Information
 *
 * @return void
 */
void getOpenGLVersionInfo()
{
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
	std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
	std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

void updateBuffersFromModel(OBJModel model)
{
	gVertexData = model.readVertexData();
	gIndexBufferData = model.readIndexBufferData();
	gTextureFilename = model.readTextureFilename();
	gNormalMapFilename = model.readNormalMapFilename();
	VertexSpecification();
}

/**
 * Function called in the Main application loop to handle user input
 *
 * @return void
 */
void Input()
{
	// Two static variables to hold the mouse position
	static int mouseX = gScreenWidth / 2;
	static int mouseY = gScreenHeight / 2;

	// Event handler that handles various events in SDL
	// that are related to input and output
	SDL_Event e;
	// Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		// If users posts an event to quit
		// An example is hitting the "x" in the corner of the window.
		if (e.type == SDL_QUIT)
		{
			std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
			gQuit = true;
		}
		if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
		{
			std::cout << "ESC: Goodbye! (Leaving MainApplicationLoop())" << std::endl;
			gQuit = true;
		}
		if (e.type == SDL_MOUSEMOTION)
		{
			// Capture the change in the mouse position
			mouseX = e.motion.xrel;
			mouseY = e.motion.yrel;
			gCamera.MouseLook(mouseX, mouseY);
		}
	}

	const Uint8 *state = SDL_GetKeyboardState(NULL);

	// q for quit
	if (state[SDL_SCANCODE_Q])
	{
		std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
		gQuit = true;
	}

	// Camera
	// Update our position of the camera
	if (state[SDL_SCANCODE_UP])
	{
		gCamera.MoveForward(0.1f);
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		gCamera.MoveBackward(0.1f);
	}
	if (state[SDL_SCANCODE_LEFT])
	{
		gCamera.MoveLeft(0.1f);
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		gCamera.MoveRight(0.1f);
	}

	// toggle wireframe mode
	if (state[SDL_SCANCODE_W])
	{
		SDL_Delay(250); // This is hacky in the name of simplicity,
						// but we just delay the
						// system by a few milli-seconds to process the
						// keyboard input once at a time.
		if (gPolygonMode == GL_FILL)
		{
			gPolygonMode = GL_LINE;
		}
		else
		{
			gPolygonMode = GL_FILL;
		}
	}

	// render object 1 < n < 9
	if (state[SDL_SCANCODE_1])
	{
		OBJModel model(gObjectFilenames[0]);
		updateBuffersFromModel(model);
		std::cout << "Rendering " << gObjectFilenames[0] << std::endl;
	}
	else if (state[SDL_SCANCODE_2])
	{
		OBJModel model(gObjectFilenames[1]);
		updateBuffersFromModel(model);
	}
	else if (state[SDL_SCANCODE_3])
	{
		OBJModel model(gObjectFilenames[2]);
		updateBuffersFromModel(model);
	}
	else if (state[SDL_SCANCODE_4])
	{
		OBJModel model(gObjectFilenames[3]);
		updateBuffersFromModel(model);
	}
	else if (state[SDL_SCANCODE_5])
	{
		OBJModel model(gObjectFilenames[4]);
		updateBuffersFromModel(model);
	}
	else if (state[SDL_SCANCODE_6])
	{
		OBJModel model(gObjectFilenames[5]);
		updateBuffersFromModel(model);
	}
	else if (state[SDL_SCANCODE_7])
	{
		OBJModel model(gObjectFilenames[6]);
		updateBuffersFromModel(model);
	}
	else if (state[SDL_SCANCODE_8])
	{
		OBJModel model(gObjectFilenames[7]);
		updateBuffersFromModel(model);
	}
	else if (state[SDL_SCANCODE_9])
	{
		OBJModel model(gObjectFilenames[8]);
		updateBuffersFromModel(model);
	}
}

void PrepBuffers()
{
	for (Light &light : gLights)
	{
		light.PrepBuffers();
	}
}

void UpdateLightPositions()
{
	for (Light &light : gLights)
	{
		light.Update(0.05f);
		light.UpdateBuffers();
	}
}

/**
 * Main Application Loop
 * This is an infinite loop in our graphics application
 *
 * @return void
 */
void MainLoop()
{

	// Little trick to map mouse to center of screen always.
	// Useful for handling 'mouselook'
	// This works because we effectively 're-center' our mouse at the start
	// of every frame prior to detecting any mouse motion.
	SDL_WarpMouseInWindow(gGraphicsApplicationWindow, gScreenWidth / 2, gScreenHeight / 2);
	// SDL_SetRelativeMouseMode(SDL_TRUE);

	PrepBuffers();
	// While application is running
	while (!gQuit)
	{
		// Handle Input
		Input();
		// Update light position
		UpdateLightPositions();
		// Setup anything (i.e. OpenGL State) that needs to take
		// place before draw calls
		PreDraw();
		// Draw Calls in OpenGL
		// When we 'draw' in OpenGL, this activates the graphics pipeline.
		// i.e. when we use glDrawElements or glDrawArrays,
		//      The pipeline that is utilized is whatever 'glUseProgram' is
		//      currently binded.
		Draw();
		// Update screen of our specified window
		SDL_GL_SwapWindow(gGraphicsApplicationWindow);
	}
}

/**
 * The last function called in the program
 * This functions responsibility is to destroy any global
 * objects in which we have create dmemory.
 *
 * @return void
 */
void CleanUp()
{
	// Destroy our SDL2 Window
	SDL_DestroyWindow(gGraphicsApplicationWindow);
	gGraphicsApplicationWindow = nullptr;

	// Delete our OpenGL Objects
	glDeleteBuffers(1, &gVertexBufferObject);
	glDeleteVertexArrays(1, &gVertexArrayObject);

	// Delete our Graphics pipeline
	glDeleteProgram(gGraphicsPipelineShaderProgram);

	// Quit SDL subsystems
	SDL_Quit();
}

/**
 * The entry point into our C++ programs.
 *
 * @return program status
 */
int main(int argc, char *args[])
{

	for (int i = 1; i < argc; i++)
	{
		gObjectFilenames.push_back(args[i]);
	}

	// 1. Setup the graphics program
	InitializeProgram();

	// 2. Setup our geometry
	VertexSpecification();

	// 3. Create our graphics pipeline
	// 	- At a minimum, this means the vertex and fragment shader
	CreateGraphicsPipeline();

	// 4. Call the main application loop
	MainLoop();

	// 5. Call the cleanup function when our program terminates
	CleanUp();

	return 0;
}
