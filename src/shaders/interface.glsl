#shader vertex 
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 acolor;

out vec2 v_texCoord;

uniform mat4 u_mvp;

void main() {
   gl_Position = u_mvp * position;
   v_texCoord = texCoord;

}; 

#shader fragment
#version 330 core

in vec2 v_texCoord;
out vec4 color;

uniform bool u_useTexture;
uniform bool u_isFont; 
uniform sampler2D u_texture;
uniform vec4 u_color;

void main() {
    if (u_useTexture) {
        vec4 texColor = texture(u_texture, v_texCoord);

        if (u_isFont) {
            float alpha = texColor.r;
            color = vec4(u_color.rgb, u_color.a * alpha);
        } else {
            color = texColor * u_color;
        }
    } else {
        color = u_color;
    }
}
