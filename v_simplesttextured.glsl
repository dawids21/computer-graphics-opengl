#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 kd;
uniform vec4 ka;
uniform vec4 ks;
uniform float alpha;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 normal; //Vertex normal in model space
in vec2 texCoord;

//Varying variables
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord;
out vec4 i_kd;
out vec4 i_ka;
out vec4 i_ks;
out float i_alpha;

void main(void) {
    vec4 lp = vec4(0, 5, -6, 1); //light position, world space
    l = normalize(V * lp - V * M * vertex); //vector towards the light in eye space
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //vector towards the viewer in eye space
    n = normalize(V * M * normal); //normal vector in eye space
    iTexCoord = texCoord;

    i_kd = kd;
    i_ka = ka;
    i_ks = ks;
    i_alpha = alpha;

    gl_Position = P * V * M * vertex;
}
