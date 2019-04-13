#version 330 core

uniform vec3 light_dir;
uniform vec3 presetColor;
uniform int toon;

in vec3 fragNormal;
in float edge;
in vec3 world_pos;
out vec4 color;


void main()
{
	float intensity;
	vec4 colorResult;
	intensity = dot(normalize(vec3(-light_dir)),fragNormal);
	if(toon == 0){
		colorResult = vec4(presetColor,1.0);
	}
	else if(edge<0.1){
		colorResult = vec4(0,0,0,1.0);
	}
	else {
		if (intensity > 0.95)
			colorResult = vec4(presetColor,1.0);
		else if (intensity > 0.5)
			colorResult = vec4(presetColor*0.7,1.0);
		else if (intensity > 0.2)
			colorResult = vec4(presetColor*0.5,1.0);
		else
			colorResult = vec4(presetColor*0.3,1.0);
	}
	color = colorResult;
}