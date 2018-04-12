#version 330 core

out vec4 gl_FragColor;

void main()
{
	vec2 circCoord = gl_PointCoord - 0.5;
	if ( dot( circCoord, circCoord ) > 0.25 ) 
		discard;

	gl_FragColor = vec4(1.0,1.0,1.0,0.7);

}
