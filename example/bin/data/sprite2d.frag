uniform sampler2D u_texture;
varying vec2 v_uv;
varying float v_opacity;
void main()
{
	vec4 color = texture2D(u_texture,v_uv);
	color.a *= v_opacity;
    gl_FragColor = color.bgra;
}