#version 330 core
layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[3];

void main()
{
    int id = int(v_TexIndex);
    // o_Color = texture(u_Textures[id], v_TexCoord) * v_Color;
    // vec4 color;

    switch (id) {
    case 0:
        o_Color = texture(u_Textures[0], v_TexCoord) * v_Color;
        break;
    case 1:
        o_Color = texture(u_Textures[1], v_TexCoord) * v_Color;
        break;
    case 2:
        o_Color = texture(u_Textures[2], v_TexCoord) * v_Color;
        break;
    }
} 