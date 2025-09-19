#shader vertex 
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 acolor;
out vec3 trianglecolor;

void main() {

   gl_Position = position;
   trianglecolor = acolor;
};

#shader fragment
#version 330 core

out vec4 color;
in vec3 trianglecolor;
void main() {
   color = vec4(trianglecolor,1.0);
};