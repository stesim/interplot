#version 420

layout( location = 0 ) in vec3 vert_pos;
layout( location = 1 ) in vec4 vert_color;
layout( location = 2 ) in vec3 vert_normal;

out vec4 vert_color_out;
out vec3 vert_normal_out;
out vec3 vert_pos_out;

uniform mat4 mat_view;
uniform mat4 mat_viewProjection;
uniform mat4 mat_model;
uniform mat3 mat_viewNormal;
//uniform vec3 vec_camera;

void main()
{
	gl_Position     = mat_viewProjection * mat_model * vec4( vert_pos, 1.0 );
	vert_color_out  = vert_color;
	vert_normal_out = normalize( mat_viewNormal * vert_normal );
	vec4 pos_tmp    = mat_view * mat_model * vec4( vert_pos, 1.0 );
	vert_pos_out    = pos_tmp.xyz / pos_tmp.w;
}
