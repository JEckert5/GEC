#version 460 core

out vec4 FragColor;
in vec2 oTexCoords;

uniform sampler2D uTexture;
// uniform vec3 color;

void main() {
	FragColor = texture(uTexture, oTexCoords);
}