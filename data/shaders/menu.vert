#version 330
 
in vec3 a_vertex;
in vec2 a_uv;

uniform mat4 u_m;
uniform mat4 u_mvp;

out vec2 v_uv;
out vec3 v_pos;

void main()
{	
	v_uv = a_uv;

	//calculate the position of the vertex using the matrices
	v_pos = (u_m * vec4(a_vertex, 1.0)).xyz;
	
	gl_Position = u_mvp * vec4(a_vertex, 1.0);
	//gl_Position = vec4( v_pos , 1.0 );
}




