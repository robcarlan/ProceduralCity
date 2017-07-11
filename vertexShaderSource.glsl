#version 330
#define M_PI 3.1415926535897932384626433832795f

layout(location = 0) in vec3 verts;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec4 pos;
layout(location = 3) in vec3 scale;

out vec4 normalOut;
out vec4 color;

uniform vec3 lightVec; //Position
uniform vec3 CameraRight_worldspace;
uniform vec3 Camera_worldspace;
uniform mat4 VP;

void main() {
   vec3 scaled = verts * scale;
	float theta = (pos.w + M_PI / 2.0f);
	
	float thetaNorm = theta;

	//Because mod doesn't have this behaviour???
	while (thetaNorm > M_PI / 2.0f)
		thetaNorm -= M_PI / 2.0f;
	
	vec3 rotatedVerts = vec3(
		scaled.x * cos(theta) + scaled.z * sin(theta), 
		scaled.y, 
		scaled.z * cos(theta) - scaled.x * sin(theta)
		); 

	vec3 rotatedNorm = vec3(
		norm.x * cos(theta) + norm.z * sin(theta), 
		norm.y, 
		norm.z * cos(theta) - norm.x * sin(theta)
		); 
	
	vec3 wsp = pos.xyz + rotatedVerts;
	gl_Position = VP * vec4(wsp, 1.0f);
	vec3 lightToVert = normalize(lightVec.xyz - wsp.xyz);
	
	//For now, light is always at a constant vector to each object
	lightToVert = normalize(vec3(1.0, 0.0, 0.0));
	
	normalOut = vec4(rotatedNorm.xyz, 1.0);
	
	color = vec4(0.4, 0.4, 0.4, 1.0);
}