/*
#version 420
*/

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
//uniform float f_time;

// line uniforms
uniform float f_lineRadius;
uniform uint  ui_instancesPerLine;
uniform float f_paramStart;
uniform float f_paramEnd;
uniform float f_lineDistance;

/*
const float PI     = 3.14159265359;
const float TWO_PI = 6.28318530718;
const float PI_SQR = 9.86960440109; 

const float PERIODS = 4.0;

vec3 fun1( float x )
{
	return vec3( 10.0 * x, 2.0 * sin( 1.0 * f_time + PERIODS * x * TWO_PI ), 2.0 * cos( 1.0 * f_time + PERIODS * x * TWO_PI ) );
}

vec3 fun( float x, float y )
{
	return vec3( 10.0 * x, 2.0 * sin( 1.0 * f_time + PERIODS * x * TWO_PI ), 2.0 * cos( 1.0 * f_time + PERIODS * x * TWO_PI ) ) + vec3( 0.0, 0.0, y );
}

vec3 fun_grad( float x )
{
	return vec3( 10.0, 2.0 * PERIODS * TWO_PI * cos( 1.0 * f_time + PERIODS * x * TWO_PI ), -2.0 * PERIODS * TWO_PI * sin( 1.0 * f_time + PERIODS * x * TWO_PI ) );
}

vec3 fun_normal( float x )
{
	//return normalize( vec3( 2.0 * PERIODS * TWO_PI * sin( 1.0 * f_time + PERIODS * x * TWO_PI ), 0.0, 10.0 ) );
	return normalize( vec3( 0.0, -4.0 * PI_SQR * 2.0 * sin( f_time + PERIODS * x * TWO_PI ), -4 * PI_SQR * 2.0 * cos( f_time + PERIODS * x * TWO_PI ) ) );
}
*/

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
	float alpha  = ( float( instance_data_in[ 0 ].instance % ui_instancesPerLine ) + param.x ) / float( ui_instancesPerLine );
	param.x      = f_paramStart * ( 1.0 - alpha ) + f_paramEnd * alpha;
	float y      = float( instance_data_in[ 0 ].instance / ui_instancesPerLine ) * f_lineDistance;

	vec3 dir     = fun_grad  ( param.x, y );
	vec3 normal  = fun_normal( param.x, y );
	mat3 basis   = mat3( dir, normal, cross( dir, normal ) );
	vec4 pos     = vec4( fun( param.x, y ) + basis * tube( param.y ), 1.0 );

	gl_Position  = mat_view_projection * mat_model * pos;
	
	fragment_data_out.position = ( mat_view * mat_model * pos ).xyz;
	fragment_data_out.normal   = mat_view_normal * mat_model_normal * basis * tube_normal( param.y );
	fragment_data_out.color    = vec4( float( param.y >= 0.5 ), float( param.y < 0.5 ), 0.0, 1.0 );
	//fs_in_color  = vec4( 0.0, 1.0, 0.0, 1.0 );
}
