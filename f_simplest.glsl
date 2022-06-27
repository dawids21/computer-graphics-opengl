#version 330

in vec4 i_color;
in vec4 l;
in vec4 n;
in vec4 v;

out vec4 pixelColor; //Output variable. Almost final pixel color.

void main(void) {

	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);

	vec4 mr = reflect(-ml, mn);
	float nl = clamp(dot(mn, ml), 0, 1);
    float rv = clamp(dot(mr, mv), 0, 1);
    rv = pow(rv, 25); // change 25 until it looks good

	pixelColor = i_color * nl + rv;
}
