uniform mat4 u_transform_matrix;
attribute vec3 a_position;
void main()
{
	gl_Position = u_transform_matrix*vec4(a_position,1.0);
}