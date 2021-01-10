uniform vec2 size;
uniform vec2 coor;
uniform vec3 color;
uniform float blur;

void main()
{
	vec2 coorNew = vec2(coor.x, 600 - coor.y);
	float left = gl_FragCoord.x - (coorNew.x - size.x/2);
	float right = (coorNew.x + size.x/2) - gl_FragCoord.x;
	float top = (coorNew.y + size.y/2) - gl_FragCoord.y;
	float bottom = gl_FragCoord.y - (coorNew.y - size.y/2);
	float delta = min(min(right,left),min(bottom,top));
	float opacity = 1.0;
	if(delta > blur)
		opacity = 1.0;
	else
		opacity = delta/blur;
	
	gl_FragColor = vec4(color,opacity);
}