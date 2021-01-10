uniform float r;
uniform vec2 coor;
uniform vec3 color;
uniform float blur;

void main()
{
	vec2 coorNew = vec2(coor.x, 600 - coor.y);
	float delta = r - pow(pow(coorNew.x - gl_FragCoord.x, 2.0) + pow(coorNew.y - gl_FragCoord.y,2.0),0.5);
	float opacity = 1.0;
	if(delta > blur)
		opacity = 1.0;
	else
		opacity = delta/blur;
	
	gl_FragColor = vec4(color,opacity);
}