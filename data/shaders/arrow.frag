#version 330

in vec2 v_uv;
in vec4 v_normal;
in vec3 v_pos;

uniform float u_distance;

out vec4 fragColor;

void main(void)
{	
	
	if (u_distance <= 1) {

		fragColor = vec4(vec3(1 - u_distance, 0, u_distance), 1.0);
	}
	else {
		fragColor = vec4(vec3(0, 0, 1), 1.0);
	}
}

