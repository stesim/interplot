#version 420

//layout( location = 0 ) in vec2 vert_pos;
//layout( location = 1 ) in vec4 vert_color;
//layout( location = 2 ) in vec3 vert_normal;

//out vec4 vert_color_out;
//out vec3 vert_normal_out;
//out vec3 vert_pos_out;

//uniform mat4 mat_view;
//uniform mat3 mat_viewNormal;
//uniform mat4 mat_viewProjection;
//uniform mat4 mat_model;
//uniform mat3 mat_modelNormal;
//
//float fun( float x, float y )
//{
//	return x * ( 1.0 - x ) * y * ( 1.0 - y );
//}
//
//void main()
//{
//	vec4 pos4       = vec4( vert_pos, /*fun( vert_pos.x, vert_pos.y )*/ 0.0, 1.0 ).xzyw;
//	gl_Position     = mat_viewProjection * mat_model * pos4;
//	vert_color_out  = vec4( 0.0, 1.0, 0.0, 1.0 );//vert_color;
//	vert_normal_out = mat_viewNormal * mat_modelNormal * vec3( 0.0, 1.0, 0.0 );// /*normalize(*/ mat_viewNormal * vert_normal /*)*/;
//	vec4 pos_tmp    = mat_view * mat_model * pos4;
//	vert_pos_out    = pos_tmp.xyz / pos_tmp.w;
//}

layout( location = 0 ) in vec2 vert_pos;

void main()
{
	gl_Position = vec4( vert_pos, 0.0, 1.0 );
}
