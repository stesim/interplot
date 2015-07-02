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

uniform uvec2 vec_pointsPerInstance;

void main()
{
	if( gl_InvocationID == 0 )
	{
		gl_TessLevelInner[ 0 ] = vec_pointsPerInstance.x;
		gl_TessLevelInner[ 1 ] = vec_pointsPerInstance.y;
		gl_TessLevelOuter[ 0 ] = vec_pointsPerInstance.y;
		gl_TessLevelOuter[ 1 ] = vec_pointsPerInstance.x;
		gl_TessLevelOuter[ 2 ] = vec_pointsPerInstance.y;
		gl_TessLevelOuter[ 3 ] = vec_pointsPerInstance.x;
	}

	gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;

	instance_data_out[ gl_InvocationID ].instance = instance_data_in[ gl_InvocationID ].instance;
}
