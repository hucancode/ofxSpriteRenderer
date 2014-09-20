uniform sampler2D u_texture;
varying vec2 v_uv;
varying vec3 v_color;
varying float v_color_intensity;
varying float v_opacity;
void main()
{
	vec4 color = texture2D(u_texture,v_uv);
	color = vec4(mix(v_color, color.rgb, v_color_intensity), color.a*v_opacity);
    gl_FragColor = color.bgra;
}