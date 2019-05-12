#version 420 

layout(triangles) in; // input formaat
layout(triangle_strip, max_vertices=3) out; // output formaat

in vec3 vertPosWorld[3];
out vec3 geoPosWorld;  // vertex world positie
out vec3 geoNormWorld; // polygoon normaal

void main()
{	
	// oppervlakte normaal berekenen m.b.v. 2 driehoekszijden
	vec3 ab = vertPosWorld[1] - vertPosWorld[0]; 
	vec3 ac = vertPosWorld[2] - vertPosWorld[0];
	geoNormWorld = normalize(cross(ab, ac));

	// primitive opnieuw opbouwen en output positie toewijzen
	for(int i=0; i<3; i++)
	{ 
		// vertex positie blijft hetzelfde
		gl_Position = gl_in[i].gl_Position;
		geoPosWorld = vertPosWorld[i];
		EmitVertex();
	}
	EndPrimitive();
}  