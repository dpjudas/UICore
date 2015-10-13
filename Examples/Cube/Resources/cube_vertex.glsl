
#version 330

layout(std140) uniform UniformBlock
{
	mat4 Transform;
};

in vec4 Vertex;
out vec4 Color;

void main()
{
	Color = vec4((Vertex.xyz + 1) * 0.5, Vertex.w);
	gl_Position = Transform * vec4(Vertex.xyz, 1);
}
