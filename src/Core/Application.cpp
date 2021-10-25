#include "Application.h"

static void GLAPIENTRY openGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            cout << "[High Severity] " << message << endl;
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            cout << "[Medium Severity] " << message << endl;
            break;
        case GL_DEBUG_SEVERITY_LOW:
            cout << "[Low Severity] " << message << endl;
            break;
    }
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
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(1600, 900, "GLenjhin", nullptr, nullptr);

    if (!window)
    {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        cout << "Failed to initialize GLEW" << endl;
        glfwTerminate();
        return 1;
    }

    // Enable debugging
    glDebugMessageCallback(openGLLogMessage, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    // Enable Linear color space (sRGB)
    glEnable(GL_FRAMEBUFFER_SRGB);

    cout << "Running OpenGL " << glGetString(GL_VERSION) << endl;

    // Create GameContainer for update loop and other game object setup
    GameContainer game = GameContainer(window);

    double deltaTime = 0.0;

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
        deltaTime = clockTicksToSeconds(frameEndTick - frameStartTick);
    }

    glfwTerminate();
    return 0;
}
