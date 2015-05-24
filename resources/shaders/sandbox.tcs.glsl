#version 420

layout( vertices = 3 ) out;

void main()
{
	if( gl_InvocationID == 0 )
	{
		gl_TessLevelInner[ 0 ] = 32.0;
		gl_TessLevelOuter[ 0 ] = 32.0;
		gl_TessLevelOuter[ 1 ] = 32.0;
		gl_TessLevelOuter[ 2 ] = 32.0;
	}

	gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
}
