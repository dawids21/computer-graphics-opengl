#version 330

out vec4 pixelColor; //Output variable. Almost final pixel color.

//Varying variables
in vec4 n;
in vec4 l1;
in vec4 l2;
in vec4 v;
in vec4 i_kd;
in vec4 i_ka;
in vec4 i_ks;
in float i_alpha;

vec4 calculateLight(vec4 light, vec4 normal, vec4 view, vec4 kd, vec4 ks, vec4 ka, float alpha) {
	vec4 ml = normalize(light);
	vec4 mn = normalize(normal);
	vec4 mv = normalize(view);
	//Reflected vector
	vec4 mr = reflect(-ml, mn);

	//Lighting model computation
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), alpha);

	vec4 ambient = vec4(ka.rgb * vec3(0.2f), 0);
	vec4 diffuse = vec4(kd.rgb * nl, kd.a);
	vec4 specular = vec4(ks.rgb * rv, 0);
	return ambient + diffuse + specular;
}

void main(void) {
	vec4 texColor=texture(textureMap,iTexCoord);
	vec4 light = calculateLight(l1, n, v, i_kd, i_ks, i_ka, i_alpha) + calculateLight(l2, n, v, i_kd, i_ks, i_ka, i_alpha);
	pixelColor = light * texColor;
}
