#version 330

in vec2 v_uv;
in vec3 v_pos;

uniform sampler2D u_texture_diffuse;

out vec4 fragColor;

void main(void)
{	
	vec3 texture = texture2D(u_texture_diffuse, v_uv).xyz;
	
	fragColor = vec4(texture, 1.0);
}

