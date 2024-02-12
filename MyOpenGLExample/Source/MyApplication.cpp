#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow * window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* We are gonna create a 4 side polygon, so the definition goes between begin and end */
        glBegin(GL_POLYGON);
        glVertex2f(-0.5f, 0.5f);   // v1 - Top left corner
        glVertex2f(0.5f, 0.5f);    // v2 - Top right corner
        glVertex2f(0.5f, -0.5f);   // v3 - Bottom right corner
        glVertex2f(-0.5f, -0.5f);  // v4 - Bottom left corner
        glEnd();


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}