#version 150
uniform mat4 projview;

uniform mat4 world;


in vec4 a_position;
in vec4 a_color;
out vec4 v_color;

void main()
{
    gl_Position = projview * world * a_position;
	v_color = a_color;
}
