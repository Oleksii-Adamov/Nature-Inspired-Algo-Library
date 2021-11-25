#include "Renderer.h"
#include <chrono>
#include <thread>

int viz::test()
{
    GLFWwindow* window;

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

    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();
    glfwSwapBuffers(window);
    glfwPollEvents();
    /* Loop until the user closes the window */
    //while (!glfwWindowShouldClose(window))
   // {
        /* Render here */
        
        //std::this_thread::sleep_for(std::chrono::seconds(3));
        /* Swap front and back buffers */
    //    glfwSwapBuffers(window);

        /* Poll for and process events */
  //      glfwPollEvents();
   // }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    glfwTerminate();
    return 0;
}