#version 330 core
out vec4 FragColor;

in vec3 lighting_color; 

uniform vec3 object_color;

void main(){
   FragColor = vec4(lighting_color * object_color, 1.0);
}