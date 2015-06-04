#version 420

layout( vertices = 4 ) out;

in InstanceData
{
	int instance;
} instance_data_in[];

out InstanceData
{
	int instance;
} instance_data_out[];

uniform uint  ui_numPoints;
uniform uint  ui_tubeFaces;

void main()
{
	if( gl_InvocationID == 0 )
	{
		gl_TessLevelInner[ 0 ] = ui_numPoints;
		gl_TessLevelInner[ 1 ] = ui_tubeFaces;
		gl_TessLevelOuter[ 0 ] = ui_tubeFaces;
		gl_TessLevelOuter[ 1 ] = ui_numPoints;
		gl_TessLevelOuter[ 2 ] = ui_tubeFaces;
		gl_TessLevelOuter[ 3 ] = ui_numPoints;
	}

	gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;

	instance_data_out[ gl_InvocationID ].instance = instance_data_in[ gl_InvocationID ].instance;
}
