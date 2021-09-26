#include "Application.h"
#include "Convert.h"
#include "Math.h"

static double gameTime = 0.0;
static double deltaTime = 0.0;

const unsigned int NUM_VERTICES = 9;
static float originalVertices[NUM_VERTICES * 3] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.25f, 1.0f,
	0.5f, 0.9f, 0.5f,

	0.5f, 0.9f, 0.5f,
	-0.5f, 0.8f, 0.5f,
	0.0f, 0.0f, 0.0f,

	-0.5f, 0.8f, 0.5f,
	-1.0f, 0.1f, 0.0f,
	0.0f, 0.0f, 0.0f
};

static float vertices[NUM_VERTICES * 3];

void update()
{
	for (int i = 0; i < NUM_VERTICES * 3; i += 3)
	{
		// Manipulating XY of each vertex position.
		double speed = PI * 2 * 0.15;
		double offset = 2.0 * PI * i / (NUM_VERTICES * 3);
		const float strength = 0.125f;

		float x = (float)cos(gameTime * speed + offset);
		float y = (float)sin(gameTime * speed + offset);
		vertices[i] = originalVertices[i] + x * strength;
		vertices[i + 1] = originalVertices[i + 1] + y * strength;
		vertices[i + 2] = originalVertices[i + 2] + x * y * strength;
	}

	glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * 3 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
}

int main()
{
	cout << "Initializing engine" << endl;

	if (!glfwInit())
		return 1;

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

	// Initialize vertex positions
	memcpy(vertices, originalVertices, NUM_VERTICES * 3 * sizeof(float));

	// Create and bind a vertex buffer
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, NUM_VERTICES * 3 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

	// Vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	double elapsedTime = 0.0;
	unsigned int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		clock_t frameStartTick = clock();
		glClear(GL_COLOR_BUFFER_BIT);

		update();

		glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

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

	glfwTerminate();
	return 0;
}
