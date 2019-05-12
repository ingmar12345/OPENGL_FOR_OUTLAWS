#version 420 

layout(triangles) in; // input formaat
layout(triangle_strip, max_vertices=3) out; // output formaat

out float polyDepth; // diepte

void main()
{	
	// gemiddelde depth value berekenen per polygoon
	polyDepth = (gl_in[0].gl_Position.z + gl_in[1].gl_Position.z + gl_in[2].gl_Position.z) / 3;

	// primitive opnieuw opbouwen
	for(int i=0; i<3; i++)
	{ 
		// vertex positie blijft hetzelfde
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}  