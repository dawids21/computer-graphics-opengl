#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 light;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 color;
in vec4 normal;

out vec4 i_color;
out vec4 l;
out vec4 n;
out vec4 v;

void main(void) {
    l = normalize(V*light - V*M*vertex);
    n = normalize(V*M*normal);
    v = normalize(vec4(0, 0, 0, 1) - V*M*vertex); // viewer is in position 0,0,0 in the eye space

    gl_Position=P*V*M*vertex;
    i_color = color;
}
