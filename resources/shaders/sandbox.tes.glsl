#version 420

layout( triangles, equal_spacing, cw ) in;

//in vec2 tcs_pos_out[];

out vec4 vert_color_out;
out vec3 vert_normal_out;
out vec3 vert_pos_out;

uniform mat4 mat_view;
uniform mat3 mat_view_normal;
uniform mat4 mat_view_projection;
uniform mat4 mat_model;
uniform mat3 mat_model_normal;

float fun( float x, float y )
{
	return 16.0 * x * ( 1.0 - x ) * y * ( 1.0 - y );
}

vec2 fun_grad( float x, float y )
{
	return vec2( 16.0 * ( 1.0 - 2.0 * x ) * y * ( 1.0 - y ), 16.0 * x * ( 1.0 - x ) * ( 1.0 - 2.0 * y ) );
}

void main()
{
	vec2 param      = gl_TessCoord.x * gl_in[ 0 ].gl_Position.xy + gl_TessCoord.y * gl_in[ 1 ].gl_Position.xy + gl_TessCoord.z * gl_in[ 2 ].gl_Position.xy;

	vec4 pos4       = vec4( param.x, fun( param.x, param.y ), param.y, 1.0 );
	gl_Position     = mat_view_projection * mat_model * pos4;
	
	vec4 pos_tmp    = mat_view * mat_model * pos4;
	vert_pos_out    = pos_tmp.xyz;// / pos_tmp.w;
	
	vec2 grad       = fun_grad( param.x, param.y );
	vert_normal_out = mat_view_normal * mat_model_normal * vec3( -grad.x, 1.0, -grad.y );

	vert_color_out  = vec4( 0.0, 1.0, 0.0, 1.0 );
}
