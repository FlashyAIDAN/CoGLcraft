#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 texCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	fragColor = vec4(ourColor, 1.0) * texture(texture1, texCoord);
}
