#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
out vec3 newColor; // 向片段着色器输出一个颜色
//分别是模型矩阵、观察矩阵和投影矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos/5, 1.0f);
    newColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}