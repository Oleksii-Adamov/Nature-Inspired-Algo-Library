#pragma once
/*!
\file
\brief Header for Visualization class
*/
#define GLEW_STATIC
#include "../Dependencies/GLEW/include/GL/glew.h"
#include <GLFW/glfw3.h>
#include "../Dependencies/GLM/glm/glm.hpp"
#include "../Dependencies/GLM/glm/gtc/matrix_transform.hpp"
#include <string>
namespace vis {
    class Visualization {
    private:
        unsigned int positions_buffer_id;
        double ans_value = -1.0;
        unsigned int ans_buffer_id;
        size_t positions_size;
        size_t positions_cur_count = 0;
        double max_value;
        unsigned int program_shader;
        bool is_initialized = false;
        glm::mat4 proj;
        int u_proj_location;
        int u_size_location;
        int u_color_location;
        void init_buffers();
        void parse_shader(const std::string& filepath, std::string& vertex_shader, std::string& fragment_shader);
        unsigned int compile_shader(unsigned int type, const std::string& source);
        unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
        void init_shader();
        void private_init();
        
    public:
        GLFWwindow* window;
        /// <summary>
        /// draws graph of data stored in this class
        /// </summary>
        void draw();
        /// <summary>
        /// Add new value to connect with previous value, which will result in updating of graph rendered in window
        /// </summary>
        /// \param[in] value value (unnormalized - just plain value you have)
        void add_and_draw(double value);
        ~Visualization();
        /// <summary>
        /// Visualization Constructor
        /// </summary>
        /// \param[in] number_of_values number of values you are planning to pass (affects horizontal scale)
        /// \param[in] maximim_value maximum value > 0 that your value could take (affects vertical scale) 
        Visualization(size_t number_of_values, double maximim_value);
        /// <summary>
       /// Visualization Constructor
       /// </summary>
       /// \param[in] number_of_values number of values you are planning to pass (affects horizontal scale)
       /// \param[in] maximim_value maximum value > 0 that your value could take (affects vertical scale) 
       /// \param[in] ans_value value of answer > 0 a horizontal line between (0, ans_value) and (number_of_values - 1, ans_value) would be drawn 
        Visualization(size_t number_of_values, double maximim_value, double ans_value);

        /// <summary>
        /// Initialization needed if constructor without arguments was used. Initialization could be done only once, otherwise throws std::string exception
        /// </summary>
        /// \param[in] number_of_values number of values you are planning to pass (affects horizontal scale)
        /// \param[in] maximim_value maximum value > 0 that your value could take (affects vertical scale)
        void init(size_t number_of_values, double maximim_value);

        /// <summary>
        /// Initialization needed if constructor without arguments was used. Initialization could be done only once, otherwise throws std::string exception
        /// </summary>
        /// \param[in] number_of_values number of values you are planning to pass (affects horizontal scale)
        /// \param[in] maximim_value maximum value > 0 that your value could take (affects vertical scale)
        /// \param[in] ans_value value of answer > 0 a horizontal line between (0, ans_value) and (number_of_values - 1, ans_value) would be drawn 
        void init(size_t number_of_values, double maximim_value, double ans_value);
    };
}