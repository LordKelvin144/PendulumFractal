#version 430 core

layout(location = 0) in vec2 vPos;

out vec2 coordinate;

void main() {
	gl_Position = vec4(vPos.x, vPos.y, 0.0, 1.0);
	coordinate = vPos;
}