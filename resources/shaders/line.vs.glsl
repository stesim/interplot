#version 420

layout( location = 0 ) in vec2 v_pos;

out InstanceData
{
	int instance;
} instance_data_out;

void main()
{
	instance_data_out.instance = gl_InstanceID;
}
