#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 texCoord;
in vec3 viewpos;

// texture samplers
uniform sampler2D texture1;
uniform float fogNear;
uniform float fogFar;

void main()
{
	vec4 color = vec4(ourColor, 1.0) * texture(texture1, texCoord);

	if(color.a < 0.1)
		discard;

	fragColor = color * (1.0 - smoothstep(fogNear, fogFar, length(viewpos)));
}
