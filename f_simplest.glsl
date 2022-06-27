#version 330

out vec4 pixelColor; //Output variable. Almost final pixel color.

//Varying variables
in vec4 ic;
in vec4 n;
in vec4 l;
in vec4 v;
in vec4 i_kd;
in vec4 i_ka;
in vec4 i_ks;
in float i_alpha;

void main(void) {
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	vec4 mr = reflect(-ml, mn);

	//Surface parameters
	vec4 kd = i_kd;
	vec4 ks = i_ks;
	vec4 ka = i_ka;

	//Lighting model computation
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), i_alpha);
	vec4 ambient = vec4(ka.rgb * vec3(0.3f), 0);
	vec4 diffuse = vec4(kd.rgb * nl, kd.a);
	vec4 specular = vec4(ks.rgb * rv, 0);
	pixelColor = (ambient + diffuse + specular) * kd;
}
