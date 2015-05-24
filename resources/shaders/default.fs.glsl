#version 420

in vec4 vert_color_out;
in vec3 vert_normal_out;
in vec3 vert_pos_out;

out vec4 frag_color;

//uniform vec3 vec_camera;

void main()
{
	vec3 d = vert_pos_out;
	float dist = length( d );
	frag_color = ( abs( dot( d, normalize( vert_normal_out ) ) /
		dist * max( 0.0, 0.96 - dist / 50.0 ) ) + 0.04 ) * vert_color_out;
}
