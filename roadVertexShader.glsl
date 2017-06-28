#version 330
#define M_PI 3.1415926535897932384626433832795

layout(location = 0) in vec3 verts;
layout(location = 1) in vec4 pos;  //x pos, y pos, z pos start, z pos end
layout(location = 2) in vec3 scale; //x scale, y scale, rot

flat out vec4 normalOut;
flat out vec4 color;

uniform vec3 lightVec;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform vec3 camPos;
uniform mat4 VP;

void main() {
   vec3 tmp = pos.xyz / 2048.0f;
	float theta = scale.z - M_PI / 2.0f;
	vec2 scaled = verts.xz * scale.xy;
	vec2 rotated = vec2(scaled.x * cos(theta) + scaled.y * sin(theta), scaled.y * cos(theta) - scaled.x * sin(theta));
	vec2 wsp2d = pos.xy + rotated;
	vec3 wsp = vec3(wsp2d.x, 0.2f, wsp2d.y);
	if (camPos.y > 50.0) wsp.y += camPos.y * 0.04;
	wsp.y = verts.x < 0.0 ? pos.z : pos. w;
	gl_Position = VP * vec4(wsp, 1.0f);
	vec3 lightVector = normalize(lightVec - wsp.xyz);
	float diffuse = 0.5f;
   normalOut = vec4(1.0, 0.0, 0.0, 1.0);
	color = vec4(0.3, 0.3, 0.3, 1.0);
}