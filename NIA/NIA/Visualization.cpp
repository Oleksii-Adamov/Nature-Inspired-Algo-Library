/*!
\file
\brief Implementation of Visualization class
*/
#include "Visualization.h"
#include <chrono>
#include <thread>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Point.h"
namespace viz {
    void Visualization::init_buffers() {
        // initialization buffer for positions
        glGenBuffers(1, &positions_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, positions_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, positions_size * sizeof(Point<double>), positions, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(Point<double>), 0);
        // initialization buffer for answer
        if (ans_positions != nullptr) {
            glGenBuffers(1, &ans_buffer_id);
            glBindBuffer(GL_ARRAY_BUFFER, ans_buffer_id);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Point<double>), ans_positions, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, sizeof(Point<double>), 0);
        }
    }
}
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
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -2;
    }
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