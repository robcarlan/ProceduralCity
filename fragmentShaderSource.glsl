flat in lowp vec4 normalOut;
flat in lowp vec4 color;

void main() {
	float diff = max(
					dot(normalOut, 
						normalize(
							vec4(1, 0.25, 0.5, 0))
						)
					, 0.0);
	
	vec3 diffuseColor = diff * color.xyz;
	vec3 ambient = vec3(0.1, 0.1, 0.1); 

	gl_FragColor = vec4(ambient + diffuseColor, 1.0);
};