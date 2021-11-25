#pragma once
/*!
\file
\brief Header for Visualization class
*/
#define GLEW_STATIC
#include "../Dependencies/GLEW/include/GL/glew.h"

namespace viz {
    class Visualization {
    private:
        float* positions = nullptr;
        unsigned int positions_buffer_id;
        float* ans_positions = nullptr;
        unsigned int ans_buffer_id;
        size_t positins_size;
        size_t positions_cur_count = 1;
        int max_value;
        int min_value;
        void init_buffers();
        void init();
        void draw();
    public:
        /// <summary>
        /// Add new value to connect with previous value, which will result in updating of graph rendered in window
        /// </summary>
        /// \param[in] value value (unnormalized - just plain value you have)
        void add(float value);
    };
}