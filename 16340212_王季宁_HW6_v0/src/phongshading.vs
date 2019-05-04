#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Frag_pos;
out vec3 objectNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    Frag_pos = vec3(model * vec4(aPos, 1.0));
    //使用法线矩阵将法向量转换为世界空间坐标
    objectNormal = mat3(transpose(inverse(model))) * aNormal;  
    gl_Position = projection * view * vec4(Frag_pos, 1.0);
}