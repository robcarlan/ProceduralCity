flat in lowp vec4 normalOut;
flat in lowp vec4 color;

void main() {
   gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0) + abs(dot(normalOut, vec4(0, 1, 0, 1.0))) * color * 0.2;
}