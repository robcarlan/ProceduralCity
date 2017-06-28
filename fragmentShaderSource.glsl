flat in lowp vec4 normalOut;
flat in lowp vec4 color;
void main() {
   gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0) + abs(dot(normalOut, vec4(1))) * color;
};