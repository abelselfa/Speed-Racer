#version 330

in vec2 v_uv;
in vec4 v_normal;
in vec3 v_pos;

uniform int u_aro_surpassed;
uniform int u_position;
uniform int u_meta;
uniform int u_next_aro;

out vec4 fragColor;

void main(void)
{	
	if (u_position == u_next_aro) {

		fragColor = vec4(vec3(1, 0.918, 0), 1.0);
	}
	else {
		if (u_aro_surpassed == 0) {

			fragColor = vec4(vec3(0, 1, 0), 1.0);
		}
		else {
		
			fragColor = vec4(vec3(0, 0, 1), 1.0);
		}
	}
	if (u_position == u_meta - 1) {
		fragColor = vec4(vec3(1, 1, 1), 1.0);
	}
}

