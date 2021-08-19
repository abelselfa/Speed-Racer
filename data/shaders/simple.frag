#version 330

in vec2 v_uv;
in vec4 v_normal;
in vec3 v_pos;

uniform sampler2D u_texture_diffuse;
uniform vec3 u_light;

out vec4 fragColor;

void main(void)
{	
	// simple red colour
	//fragColor =  vec4(1.0, 0.0, 0.0, 1.0);

	vec3 N = normalize(v_normal.xyz);
	
	vec3 L = normalize(u_light);
	float NdotL = max(dot(N,L), 0.0);

	vec3 diffuse = texture2D(u_texture_diffuse, v_uv).xyz * NdotL;
	fragColor = vec4(diffuse, 1.0);
	//vec3 diffuse = vec3(1.0, 0.0, 1.0);
	//fragColor = vec4(diffuse * NdotL, 1.0);
}

