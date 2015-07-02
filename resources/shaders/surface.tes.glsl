#version 420

layout( quads, equal_spacing, cw ) in;

in InstanceData
{
	int instance;
} instance_data_in[];

out FragmentData
{
	vec3 position;
	vec3 normal;
	vec4 color;
} fragment_data_out;

// camera uniforms
layout( std140 ) uniform CameraUniforms
{
	uniform mat4 mat_view;
	uniform mat3 mat_view_normal;
	uniform mat4 mat_view_projection;
};

// model uniforms
uniform mat4 mat_model;
uniform mat3 mat_model_normal;

// global uniforms
uniform float f_time;
#define t f_time

// line uniforms
uniform uvec2 vec_instances;
uniform vec2  vec_paramStart;
uniform vec2  vec_paramEnd;

const float PI     = 3.14159265359;
const float TWO_PI = 6.28318530718;
const float PI_SQR = 9.86960440109; 

const float PERIODS = 4.0;

vec3 fun( vec2 v );

vec3 numeric_normal( vec2 v )
{
	vec2 h = ( vec_paramEnd - vec_paramStart ) * 0.001;
	vec2 hx = vec2( h.x, 0.0 );
	vec2 hy = vec2( 0.0, h.y );

	vec3 tangentx = ( fun( v + hx ) - fun( v - hx ) );
	vec3 tangenty = ( fun( v + hy ) - fun( v - hy ) );

	return normalize( cross( tangentx, tangenty ) );
}

void main()
{
	vec2 alpha   = vec2(
		( float( instance_data_in[ 0 ].instance % vec_instances.x ) + gl_TessCoord.x ) / float( vec_instances.x ),
		( float( instance_data_in[ 0 ].instance / vec_instances.x ) + gl_TessCoord.y ) / float( vec_instances.y ) );

	vec2 param   = vec_paramStart * ( 1.0 - alpha ) + vec_paramEnd * alpha;

	vec4 pos     = vec4( fun( param ), 1.0 );
	vec3 normal  = numeric_normal( param );
	/*
	vec4 pos     = vec4( gl_TessCoord.x, 0.0, gl_TessCoord.y, 1.0 );
	vec3 normal  = vec3( 0.0, 1.0, 0.0 );
	*/

	gl_Position  = mat_view_projection * mat_model * pos;
	
	fragment_data_out.position = ( mat_view * mat_model * pos ).xyz;
	fragment_data_out.normal   = mat_view_normal * mat_model_normal * normal;
	fragment_data_out.color    = vec4( 0.5, 0.0, 0.5, 1.0 );
}
