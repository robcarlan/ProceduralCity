#version 330

layout(location = 0) in vec3 verts;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec4 pos;
layout(location = 3) in vec3 scale;

out vec4 normalOut;
out vec4 color;

uniform vec3 lightVec;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform vec3 quadColour;
uniform mat4 VP;

void main() {
	vec3 wsp = pos.xyz + (verts * scale);
	gl_Position = VP * vec4(wsp, 1.0f);
	vec3 lightVector = normalize(lightVec - wsp.xyz);
	float diffuse = max(dot(norm, lightVector), 0.2);
   normalOut = vec4(diffuse, diffuse, diffuse, 1.0);
	color = vec4(0.5, 0.5, 0.8, 0.7);
}
