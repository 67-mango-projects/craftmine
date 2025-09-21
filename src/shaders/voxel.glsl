#shader vertex 
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 acolor;

out vec3 trianglecolor;
out vec2 v_texCoord;

uniform mat4 u_mvp;

void main() {
   gl_Position = u_mvp * position;
   trianglecolor = acolor;
   v_texCoord = texCoord;

}; 

#shader fragment
#version 330 core

uniform sampler2D u_texture;
uniform float u_opacity;
out vec4 color;
in vec3 trianglecolor;
in vec2 v_texCoord;

void main() {
	vec4 texColor = texture(u_texture,v_texCoord);
   color = texColor;
};