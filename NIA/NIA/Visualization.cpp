/*!
\file
\brief Implementation of Visualization class.
*/
#include "Visualization.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include "Point.h"

namespace vis {
    /// <summary>
    /// Function to handle OPENGL errors 
    /// </summary>
    void GLAPIENTRY
        MessageCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
    }

    void Visualization::init_buffers() {
        // initialization of buffer for positions
        glGenBuffers(1, &positions_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, positions_buffer_id);
        glBufferData(GL_ARRAY_BUFFER, positions_size * sizeof(Point<float>), NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
        // initialization of buffer for answer
        if (!(ans_value < 0)) {
            glGenBuffers(1, &ans_buffer_id);
            glBindBuffer(GL_ARRAY_BUFFER, ans_buffer_id);
            // positions for drawing line between (0,ans) and (number_of_points - 1,ans), to do: with a bit of offset
            float ans_positions[4] = { 0.0f, (float)(ans_value / max_value), 1.0f, (float)(ans_value / max_value) };
            glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(Point<float>), ans_positions, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
        }
        /*
        // initialization of buffer for axis
        glGenBuffers(1, &axis_buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, axis_buffer_id);
        // positions for drawing line between (0, 0) and (0, max_value), and between (0, 0) and (NUMBER_OF_GENERATIONS, 0), with a bit of offset
        float offset_x = (float)((double)1 / (double)positions_size);
        float offset_y = (float)(0.01f * max_value);
        float axis_positions[8] = { offset_x, offset_y, offset_x, 1.0f, offset_x, offset_y, 1.0f, offset_y};
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point<float>), axis_positions, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
        */
    }
    void Visualization::parse_shader(const std::string& filepath, std::string& vertex_shader, std::string& fragment_shader) {
        vertex_shader = "";
        fragment_shader = "";
        std::ifstream in;
        in.open(filepath, std::ifstream::in);
        if (!in.is_open())
            throw "Cannot open " + filepath;
        if (!in.good())
            throw "Stream error opening " + filepath;
        enum class ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };
        std::string line;
        ShaderType type = ShaderType::NONE;
        while (std::getline(in, line)) {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos) {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
            }
            else {
                if (type == ShaderType::VERTEX)
                    vertex_shader += line + "\n";
                else if (type == ShaderType::FRAGMENT)
                    fragment_shader += line + "\n";
            }
        }
        in.close();
    }
    unsigned int Visualization::compile_shader(unsigned int type, const std::string& source) {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);  
            // allocating on stack
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            throw "Failed to compile " + std::string(type == GL_VERTEX_SHADER ? "vertex" : "fragment") + " shader!\n" + (std::string) message;
            glDeleteShader(id);
            return 0;
        }
        return id;
    }
    unsigned int Visualization::create_shader(const std::string& vertex_shader, const std::string& fragment_shader) {
        unsigned int program = glCreateProgram();
        unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
        unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    void Visualization::init_shader() {
        std::string vertex_shader, fragment_shader;
        parse_shader("../Resources/Graph.shader", vertex_shader, fragment_shader);
        program_shader = create_shader(vertex_shader, fragment_shader);
        glUseProgram(program_shader);
        u_size_location = glGetUniformLocation(program_shader, "u_size");
        u_proj_location = glGetUniformLocation(program_shader, "u_proj");
        u_color_location = glGetUniformLocation(program_shader, "u_color");
    }
    void Visualization::private_init() {
        if (is_initialized) {
            throw (std::string)"Double initialization of Visualization!";
        }
        // Initialize GLFW library
        if (!glfwInit()) {
            throw (std::string)"Cannot initialize GLFW!";
            this->~Visualization();
        }

        // Create a windowed mode window and its OpenGL context
        window = glfwCreateWindow(/*640*/1920, /*480*/1080, "Graph", NULL/*glfwGetPrimaryMonitor()*/, NULL);
        
        // Make the window's context current
        glfwMakeContextCurrent(window);

        // Initialize GLEW library
        if (glewInit() != GLEW_OK) {
            throw (std::string)"Cannot initialize GLEW!";
            this->~Visualization();
        }
        // Enable debug output
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
        
        init_buffers();
        init_shader();
        is_initialized = true;
    }
    
    Visualization::Visualization() {
        // do nothing
    }
    void Visualization::init(size_t number_of_values, double maximim_value, double input_ans_value) {
        if (maximim_value < 0)
            throw (std::string)"maximim_value < 0";
        positions_size = number_of_values;
        max_value = maximim_value;
        if (!(input_ans_value < 0))
            ans_value = input_ans_value;
        private_init();
    }
    /*
    void Visualization::init(size_t number_of_values, double maximim_value, double input_ans_value) {
        if (maximim_value < 0)
            throw (std::string)"maximim_value < 0";
        if (input_ans_value < 0)
            throw (std::string)"ans_value < 0";
        positions_size = number_of_values;
        max_value = maximim_value;
        ans_value = input_ans_value;
        private_init();
    }*/

    void Visualization::draw() {
        if (!glfwWindowShouldClose(window)) {
            // clearing
            glClear(GL_COLOR_BUFFER_BIT);
            // render
            // setting projection matrix uniform and size vector uniform to scale with window and maximum value of x and y
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            glViewport(0, 0, width, height);
            proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
            glUniform4f(u_size_location, (float)width, (float)height, 1.0f, 1.0f);
            glUniformMatrix4fv(u_proj_location, 1, GL_FALSE, &proj[0][0]);
            // drawing graph
            glUniform4f(u_color_location, 0.0f, 1.0f, 0.0f, 1.0f);
            glBindBuffer(GL_ARRAY_BUFFER, positions_buffer_id);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
            glDrawArrays(GL_LINE_STRIP, 0, positions_cur_count);

            /*bool last_draw_positions = false;
            if (is_last_draw_positions) {
               // glBindBuffer(GL_ARRAY_BUFFER, positions_buffer_id);
                //glEnableVertexAttribArray(0);
                //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
                glDrawArrays(GL_LINE_STRIP, 0, positions_cur_count);
            }*/
            
            if (!(ans_value < 0)) { // drawing ans line
                glUniform4f(u_color_location, 1.0f, 0.0f, 0.0f, 1.0f);
                glBindBuffer(GL_ARRAY_BUFFER, ans_buffer_id);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
                glDrawArrays(GL_LINES, 0, 2);
            }

            /*
            // drawing axis
            glUniform4f(u_color_location, 0.0f, 0.0f, 1.0f, 1.0f);
            glBindBuffer(GL_ARRAY_BUFFER, axis_buffer_id);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
            glDrawArrays(GL_LINES, 0, 4);
            */

           /* if (!is_last_draw_positions) {
                //glBindBuffer(GL_ARRAY_BUFFER, positions_buffer_id);
                //glEnableVertexAttribArray(0);
                //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point<float>), 0);
                glDrawArrays(GL_LINE_STRIP, 0, positions_cur_count);
                last_draw_positions = true;
            }
            else {
                // draw 
            }*/

            // Swap front and back buffers
            glfwSwapBuffers(window);
            // Poll for and process events
            glfwPollEvents();
            //std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        else {
            glfwHideWindow(window);
        }
    }
    void Visualization::add_and_draw(double value) {
        if (!glfwWindowShouldClose(window)) {
            float x_and_y[2] = { (float)(double(positions_cur_count) / positions_size) , (float)(value / max_value) };
            glBindBuffer(GL_ARRAY_BUFFER, positions_buffer_id);
            glBufferSubData(GL_ARRAY_BUFFER, positions_cur_count * sizeof(Point<float>), sizeof(Point<float>), x_and_y);
            positions_cur_count++;
            draw();
        }
        else {
            glfwHideWindow(window);
        }
    }
    Visualization::~Visualization() {
        if (is_initialized) {
            glDeleteProgram(program_shader);
            glDeleteBuffers(1, &positions_buffer_id);
            if (!(ans_value < 0))
                glDeleteBuffers(1, &ans_buffer_id);
            glfwTerminate();
        }
    }
}
