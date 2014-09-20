uniform mat4 u_transform_matrix;
attribute vec3 a_position;
attribute vec2 a_uv;
attribute vec3 a_color;
attribute float a_color_intensity;
attribute float a_opacity;
varying vec2 v_uv;
varying vec3 v_color;
varying float v_color_intensity;
varying float v_opacity;
void main()
{
	v_uv = a_uv;
	v_color = a_color;
	v_color_intensity = a_color_intensity;
	v_opacity = a_opacity;
	gl_Position = u_transform_matrix*vec4(a_position,1.0);
}