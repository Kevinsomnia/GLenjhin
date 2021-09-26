#include "Application.h"
#include "Convert.h"
#include "Math.h"
#include "ShaderCompiler.h"
#include "Renderer/MeshRenderer.h"

static double gameTime = 0.0;
static double deltaTime = 0.0;

const unsigned int NUM_VERTICES = 5;
const unsigned int NUM_INDICES = 9;	// Should be divisible by 3
static float vertices[NUM_VERTICES * 3] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.25f, 1.0f,
	0.5f, 0.9f, 0.5f,
	-0.5f, 0.8f, 0.5f,
	-1.0f, 0.1f, 0.0f
};

static unsigned int indices[NUM_INDICES] = {
	0, 1, 2,
	2, 3, 0,
	0, 3, 4
};

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

	// Create a simple mesh object
	Mesh mesh(vertices, NUM_VERTICES, indices, NUM_INDICES);
	MeshRenderer renderer(&mesh);

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

		renderer.draw();

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
	mesh.~Mesh();

	glfwTerminate();
	return 0;
}
