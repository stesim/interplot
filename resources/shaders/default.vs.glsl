#version 420

layout( location = 0 ) in vec3 vert_pos;
layout( location = 1 ) in vec4 vert_color;
layout( location = 2 ) in vec3 vert_normal;

out vec4 vert_color_out;
out vec3 vert_normal_out;
out vec3 vert_pos_out;

uniform mat4 mat_view;
uniform mat4 mat_view_projection;
uniform mat4 mat_model;
uniform mat3 mat_view_normal;
//uniform vec3 vec_camera;

void main()
{
	gl_Position     = mat_view_projection * mat_model * vec4( vert_pos, 1.0 );
	vert_color_out  = vert_color;
	vert_normal_out = normalize( mat_view_normal * vert_normal );
	vec4 pos_tmp    = mat_view * mat_model * vec4( vert_pos, 1.0 );
	vert_pos_out    = pos_tmp.xyz / pos_tmp.w;
}
