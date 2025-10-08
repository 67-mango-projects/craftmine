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
uniform bool u_useTexture;
uniform float u_opacity;

uniform float u_lightLevel;
uniform vec3 u_nightColor;
uniform vec3 u_dayColor;

uniform vec4 u_color;

out vec4 color;

in vec3 trianglecolor;
in vec2 v_texCoord;

void main()
{
    vec4 baseColor = u_useTexture ? texture(u_texture, v_texCoord) : u_color;

    vec3 lighting = mix(u_nightColor, u_dayColor, u_lightLevel);

    vec3 finalRGB = baseColor.rgb * u_color.rgb * lighting;
    float finalAlpha = baseColor.a * u_color.a * u_opacity;

    color = vec4(finalRGB, finalAlpha);
}