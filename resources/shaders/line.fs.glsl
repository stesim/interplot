#version 420

in vec4 fs_in_color;
in vec3 fs_in_normal;
in vec3 fs_in_pos;

out vec4 frag_color;

void main()
{
	float dist = length( fs_in_pos );
	frag_color = ( abs( dot( fs_in_pos / dist, normalize( fs_in_normal ) ) ) * max( 0.0, 0.95 - dist / 50.0 ) + 0.05 ) * fs_in_color;
}
