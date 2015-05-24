#version 420

layout( quads, equal_spacing, cw ) in;

out vec3 fs_in_pos;
out vec3 fs_in_normal;
out vec4 fs_in_color;

uniform mat4 mat_view;
uniform mat3 mat_viewNormal;
uniform mat4 mat_viewProjection;
uniform mat4 mat_model;
uniform mat3 mat_modelNormal;
uniform float f_lineRadius = 0.05;
uniform float f_time = 0.0;

const float PI    = 3.14159265359;
const float TWO_PI = 6.28318530718;

vec3 fun( float x )
{
	return vec3( 10.0 * x, 2.0 * sin( 1.0 * f_time + 1.0 * x * TWO_PI ), 0.0 );
}

vec3 fun_grad( float x )
{
	return vec3( 10.0, 2.0 * TWO_PI * cos( 1.0 * f_time + 1.0 * x * TWO_PI ), 0.0 );
}

vec3 fun_normal( float x )
{
	return vec3( 0.0, 0.0, 1.0 );
}

vec3 tube_normal( float x )
{
	return vec3( 0.0, sin( x * TWO_PI ), cos( x * TWO_PI ) );
}

vec3 tube( float x )
{
	return f_lineRadius * tube_normal( x );
}

void main()
{
	vec2 param   = gl_TessCoord.xy;

	vec3 dir     = normalize( fun_grad( param.x ) );
	vec3 normal  = fun_normal( param.x );
	mat3 basis   = mat3( dir, normal, cross( dir, normal ) );
	vec4 pos     = vec4( fun( param.x ) + basis * tube( param.y ), 1.0 );

	gl_Position  = mat_viewProjection * mat_model * pos;
	
	fs_in_pos    = ( mat_view * mat_model * pos ).xyz;
	fs_in_normal = mat_viewNormal * mat_modelNormal * basis * tube_normal( param.y );
	fs_in_color  = vec4( 0.0, 1.0, 0.0, 1.0 );
}
