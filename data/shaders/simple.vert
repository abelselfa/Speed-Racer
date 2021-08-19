#version 330
 
// the following attributes are supported by this engine
// Mesh::render searches for, and binds them, if they exist
in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_uv;
in vec4 a_color;

uniform mat4 u_m;
uniform mat4 u_mvp;
uniform mat4 u_normal_matrix;
uniform vec3 u_light;

out vec2 v_uv;
out vec4 v_normal;
out vec3 v_pos;

void main()
{	
	v_uv = a_uv;
	v_normal = u_normal_matrix * vec4(a_normal, 1.0);

	//calculate the position of the vertex using the matrices
	v_pos = (u_m * vec4(a_vertex, 1.0)).xyz;
	
	gl_Position = u_mvp * vec4(a_vertex, 1.0);
	//gl_Position = vec4( v_pos , 1.0 );
}




