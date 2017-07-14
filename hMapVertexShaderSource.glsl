#version 330

layout(location = 0) in vec3 verts;
layout(location = 1) in vec4 pos;  //z value of this vertex
layout(location = 2) in vec3 norm; //x scale, y scale, rot

flat out vec4 normalOut;
flat out vec4 color;

uniform vec3 lightVec;
uniform int gridSize;
uniform float quadScale;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;

void main() {
	vec2 translated = verts.xz + vec2(0.5, 0.5); //Warp to origin, forms a quad of unit length
	translated *= quadScale;
// x = / gridDimension, y = % gridDimension
	vec2 wsp2d = translated;
	wsp2d.x += quadScale * (gl_InstanceID / gridSize);
	wsp2d.y += quadScale * mod(gridSize - gl_InstanceID, gridSize);
	vec3 wsp = vec3(wsp2d.x, - 0.1f, wsp2d.y);
	if (verts.x == -0.5f && verts.z == -0.5f) wsp.y = pos.z; //bl
	else if (verts.x == 0.5f && verts.z == -0.5f) wsp.y = pos.w; //br
	else if (verts.x == -0.5f && verts.z == 0.5f) wsp.y = pos.x; //tl
	else if (verts.x == 0.5f && verts.z == 0.5f) wsp.y = pos.y; //tr
	gl_Position = VP * vec4(wsp, 1.0f);
	vec3 lightVector = normalize(lightVec - wsp.xyz);
	float diffuse = min(0.8, max(dot(norm, lightVector), 0.2));
   normalOut = vec4(norm, 1.0);
	color = vec4(0.4f, 1.0f, 0.4f, 1.0f);
   if (pos.y < 0) color = vec4(0.2f, 0.20f, 1.0f, 1.0f);
}