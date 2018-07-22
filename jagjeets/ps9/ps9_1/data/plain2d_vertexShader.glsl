attribute vec2 vertex;
attribute vec4 color;

// Map (0,0) -> (-1,1)
//     (0,600) -> (-1,-1)
//     (800,0) -> (1,1)
//     (800,600) -> (1,-1)

varying vec4 colorOut;

void main(void)
{	
	colorOut = color;
	gl_Position=vec4(vertex,0,1); // w must be 1.  Not 0.
	gl_Position.x = gl_Position.x/400.0-1.0;
	gl_Position.y = gl_Position.y/300.0-1.0;

	
}

