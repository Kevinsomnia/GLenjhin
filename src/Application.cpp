#include "Application.h"
#include "Convert.h"
#include "Math.h"
#include "ShaderCompiler.h"
#include "Renderer/MeshRenderer.h"

static double gameTime = 0.0;
static double deltaTime = 0.0;

void update()
{
}

void cleanup()
{
}

int main()
{
	cout << "Initializing engine" << endl;

	if (!glfwInit())
		return 1;

	// OpenGL Core 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(1600, 900, "GLenjhin", nullptr, nullptr);

	if (!window)
	{
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	// Disable v-sync
	glfwSwapInterval(0);

	if (glewInit() != GLEW_OK)
	{
		cout << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return 1;
	}

	cout << "Running OpenGL " << glGetString(GL_VERSION) << endl;

	// Create 2 simple mesh objects
	const unsigned int numVerticesA = 5;
	static float verticesA[numVerticesA * 3] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.25f, 1.0f,
		0.5f, 0.9f, 0.5f,
		-0.5f, 0.8f, 0.5f,
		-1.0f, 0.1f, 0.0f
	};
	const unsigned int numIndicesA = 9;
	static unsigned int indicesA[numIndicesA] = {
		0, 1, 2,
		2, 3, 0,
		0, 3, 4
	};
	Mesh meshA(verticesA, numVerticesA, indicesA, numIndicesA);
	MeshRenderer rendererA(&meshA);

	const unsigned int numVerticesB = 4;
	static float verticesB[numVerticesB * 3] = {
		-0.25f, -0.75f, 0.0f,
		-0.25f, -0.25f, 0.5f,
		-0.75f, -0.3f, 0.0f,
		-0.75f, -0.6f, 0.5f
	};
	const unsigned int numIndicesB = 6;
	static unsigned int indicesB[numIndicesB] = {
		0, 1, 2,
		2, 3, 0,
	};
	Mesh meshB(verticesB, numVerticesB, indicesB, numIndicesB);
	MeshRenderer rendererB(&meshB);

	// Create shader
	ShaderParseResult unlitShader = parseShader("res/shaders/Unlit.shader");
	unsigned int unlit = createShader(unlitShader);
	glUseProgram(unlit);

	double elapsedTime = 0.0;
	unsigned int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		clock_t frameStartTick = clock();
		glClear(GL_COLOR_BUFFER_BIT);

		update();

		rendererA.draw();
		rendererB.draw();

		// Swap back and front buffers
		glfwSwapBuffers(window);

		glfwPollEvents();
		clock_t frameEndTick = clock();

		// Application timer
		deltaTime = convertClockTicksToSeconds(frameEndTick - frameStartTick);
		gameTime += deltaTime;

		// Print frame rate
		elapsedTime += deltaTime;
		frames++;

		if (elapsedTime > 1.0)
		{
			unsigned int fps = (unsigned int)(frames / elapsedTime);
			elapsedTime = 0.0;
			frames = 0;

			cout << "FPS: " << fps << endl;
		}
	}

	cleanup();
	glDeleteProgram(unlit);

	glfwTerminate();
	return 0;
}
