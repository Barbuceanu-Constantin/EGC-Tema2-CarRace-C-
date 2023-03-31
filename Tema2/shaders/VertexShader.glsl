#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 vertex_color;
layout(location = 1) in vec3 vertex_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 CarPos;
uniform float ScaleFactor;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;

void main()
{
	// TODO: send output to fragment shader
	vec4 aux = Model * vec4(v_position, 1.0);
	float d = distance(aux.xyz, CarPos);
	aux.y = aux.y - d * d * ScaleFactor;

	// TODO: compute gl_Position
	gl_Position = Projection * View * aux;
	frag_color = vertex_color;
	frag_normal = vertex_normal;
}
