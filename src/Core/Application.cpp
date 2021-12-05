#include "Application.h"
#include "../Math/Quaternion.h"

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
    Quaternion a = Quaternion::EulerDegrees(15.0f, 5.0f, 0.0f);
    Quaternion b = Quaternion::EulerDegrees(15.0f, 4.0f, 2.0f);
    cout << a.getEulerAnglesDegrees() << endl;
    cout << a.getMagnitude() << endl;
    a *= b;
    cout << a.getEulerAnglesDegrees() << endl;
    cout << a.getMagnitude() << endl;
    cout << b.getEulerAnglesDegrees() << endl;
    cout << b.getMagnitude() << endl;
    cout << (a * b).getEulerAnglesDegrees() << endl;
    cout << (a * b).getMagnitude() << endl;
    cout << (Quaternion::identity * a).getEulerAnglesDegrees() << endl;
    cout << (Quaternion::identity * b).getEulerAnglesDegrees() << endl;

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize Glad" << endl;
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
        steady_clock::time_point frameStart = high_resolution_clock::now();

        game.update(deltaTime);
        game.render();

        // Swap back and front buffers
        glfwSwapBuffers(window);

        glfwPollEvents();
        steady_clock::time_point frameEnd = high_resolution_clock::now();

        // Application timer
        duration<double> elapsed = frameEnd - frameStart;
        deltaTime = elapsed.count();
    }

    glfwTerminate();
    return 0;
}
