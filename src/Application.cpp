#include "Application.h"
#include "Convert.h"

void update()
{
}

void renderTriangle()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.25f, 1.0f);
	glVertex3f(0.5f, 0.9f, 0.5f);
	glEnd();
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

	double elapsedTime = 0.0;
	unsigned int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		clock_t frameStartTick = clock();

		update();

		renderTriangle();

		// Swap back and front buffers
		glfwSwapBuffers(window);

		glfwPollEvents();
		clock_t frameEndTick = clock();

		// Application timer
		double deltaTime = convertClockTicksToSeconds(frameEndTick - frameStartTick);
		elapsedTime += deltaTime;
		frames++;

		// Print frame rate
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
