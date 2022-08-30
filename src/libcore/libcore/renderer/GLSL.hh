#pragma once
#include <string_view>

namespace Engine::Renderer {
static constexpr inline std::string_view glVertexSrc = R"({version}
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexIndex;

uniform mat4 u_ProjectionView;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = u_ProjectionView * vec4(a_Position, 1.0);
}}
)";

static constexpr inline std::string_view glFragmentSrc = R"({version}
layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[{maxTextureSlots}];

void main()
{{
    int id = int(v_TexIndex);
    {outputColor}
}}
)";
} // namespace Engine::Renderer