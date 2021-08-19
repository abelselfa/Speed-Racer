#version 330

in vec2 v_uv;
in vec4 v_normal;
in vec3 v_pos;

uniform sampler2D u_texture_diffuse;
uniform vec3 u_light;
uniform vec3 u_eye;

out vec4 fragColor;

void main(void)
{	
	vec3 N = normalize(v_normal.xyz);
	
	vec3 L = normalize(u_light);
	float NdotL = max(dot(N,L), 0.0);

	vec3 diffuse = texture2D(u_texture_diffuse, v_uv).xyz * NdotL;
	vec3 ambient = texture2D(u_texture_diffuse, v_uv).xyz;
	vec3 R = reflect(-L, N);
	vec3 E = normalize(u_eye - v_pos);

	float RdotE = max(0.0, dot(R,E));

	vec3 specular = vec3(1.0, 0.918, 0.0) * pow(RdotE, 0.95);
	
	//sumo el color difuso mas el especular para poder variar su intensidad 
	//en funcion de la distancia a la camara (el ambiente no quiero alterarlo)
	//vec3 diff_spec = diffuse + specular;	
	//vec3 eye_minus_pos = u_eye - v_pos;
	//float distancia = sqrt(((eye_minus_pos).x * (eye_minus_pos).x) + ((eye_minus_pos).y * (eye_minus_pos).y) + ((eye_minus_pos).z * (eye_minus_pos).z));
	//diff_spec = diff_spec * pow(0.2 * distancia, -1);

	fragColor = vec4(ambient, 1.0);
	//fragColor = vec4(diffuse, 1.0);
	//vec3 diffuse = texture2D(u_texture_diffuse, v_uv).xyz;
	//fragColor = vec4(diffuse, 1.0);
}

