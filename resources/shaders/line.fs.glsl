#version 420

in FragmentData
{
	vec3 position;
	vec3 normal;
	vec4 color;
} fragment_data_in;

out vec4 frag_color;

void main()
{
	float dist = length( fragment_data_in.position );
	frag_color = ( abs( dot( fragment_data_in.position / dist, normalize( fragment_data_in.normal ) ) ) * max( 0.0, 0.95 - dist / 50.0 ) + 0.05 ) * fragment_data_in.color;
}
