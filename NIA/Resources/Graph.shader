#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
uniform mat4 u_proj;
uniform vec4 u_size;
void main() {
	gl_Position = u_proj * (position * u_size);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_color;
void main() {
	color = u_color;
}