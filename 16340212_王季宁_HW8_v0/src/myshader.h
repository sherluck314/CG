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
	//激活程序
	void usepro();
	//释放程序
	void freepro();
	//使用uniform更新bool值
	void setBool(const string &name, bool value);
	//使用uniform更新int值
	void setInt(const string &name, int value);
	//使用uniform更新float值
	void setFloat(const string &name, float value);
	//使用uniform更新mat4值
	void setMat4(const string &name, glm::mat4 value);
	//使用uniform更新vec3值
	void setVec3(const string &name, glm::vec3 value);
	~myshader();
};

