#version 420

in vec4 vert_color_out;
in vec3 vert_normal_out;
in vec3 vert_pos_out;

out vec4 frag_color;

void main()
{
	vec3 d = vert_pos_out;
	float dist = length( d );
	d /= dist;
	frag_color = ( abs( dot( d, normalize( vert_normal_out ) ) ) * max( 0.0, 0.96 - dist / 50.0 ) + 0.04 ) * vert_color_out;

	//frag_color = vec4( 0.0, 1.0, 0.0, 1.0 );
}
