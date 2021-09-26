#include "Application.h"
#include "Convert.h"
#include "GameContainer.h"

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

	// Create GameContainer for update loop and other game object setup
	GameContainer game = GameContainer();

	double deltaTime = 0.0;
	double elapsedTime = 0.0;
	unsigned int frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		clock_t frameStartTick = clock();

		game.update(deltaTime);
		game.render();

		// Swap back and front buffers
		glfwSwapBuffers(window);

		glfwPollEvents();
		clock_t frameEndTick = clock();

		// Application timer
		deltaTime = convertClockTicksToSeconds(frameEndTick - frameStartTick);

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
