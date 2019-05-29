#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
using namespace std;
class myshader
{
public:
	int programid;
	myshader(const char* vertexPath, const char* fragmentPath);
	//�������
	void usepro();
	//�ͷų���
	void freepro();
	//ʹ��uniform����boolֵ
	void setBool(const string &name, bool value);
	//ʹ��uniform����intֵ
	void setInt(const string &name, int value);
	//ʹ��uniform����floatֵ
	void setFloat(const string &name, float value);
	//ʹ��uniform����mat4ֵ
	void setMat4(const string &name, glm::mat4 value);
	//ʹ��uniform����vec3ֵ
	void setVec3(const string &name, glm::vec3 value);
	~myshader();
};

