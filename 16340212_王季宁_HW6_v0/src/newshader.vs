#version 330 core
layout (location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ 1
out vec3 newColor; // ��Ƭ����ɫ�����һ����ɫ
//�ֱ���ģ�;��󡢹۲�����ͶӰ����
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos/5, 1.0f);
    newColor = aColor; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
}