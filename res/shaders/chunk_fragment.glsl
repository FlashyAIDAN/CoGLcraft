#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 texCoord;
in vec3 viewpos;

// texture samplers
uniform sampler2D texture1;
uniform float fogNear;
uniform float fogFar;
uniform float globalLightLevel;
uniform bool fog = true;

void main()
{
	vec4 color = clamp(vec4(ourColor, 1.0) + vec4(globalLightLevel, globalLightLevel, globalLightLevel, 1.0), 0.05, 1.0) * texture(texture1, texCoord);

	if(color.a < 0.1)
		discard;

	vec4 final;

	if(fog)
		final = color * (1.0 - smoothstep(fogNear, fogFar, length(viewpos)));
	else
		final = color; 

	fragColor = final;
}
