#include "myshader.h"
myshader::myshader(const char* vertexPath, const char* fragmentPath)
{
	// 从文件中读取内容到字符串
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	// 确保能够抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件到输入流
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件句柄
		vShaderFile.close();
		fShaderFile.close();
		// 将流中的内容转换成字符串
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	//获取包含源代码的字符串
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	//编译程序
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// 编译顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "编译顶点着色器时出错： " << infoLog << endl;
	}
	// 编译片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "编译片段着色器时出错： " << infoLog << endl;
	}
	// 链接着色器
	programid = glCreateProgram();
	glAttachShader(programid, vertex);
	glAttachShader(programid, fragment);
	glLinkProgram(programid);
	glGetProgramiv(programid, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programid, 1000, NULL, infoLog);
		cout << "链接程序时出错： " << infoLog << endl;
	}
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


myshader::~myshader()
{
}

void myshader::usepro() {
	glUseProgram(programid);
}

void myshader::freepro() {
	glUseProgram(0);
}

void myshader::setBool(const string &name, bool value) {
	glUniform1i(glGetUniformLocation(programid, name.c_str()), (bool)value);
}

void myshader::setInt(const string &name, int value) {
	glUniform1i(glGetUniformLocation(programid, name.c_str()), (int)value);
}

void myshader::setFloat(const string &name, float value) {
	glUniform1f(glGetUniformLocation(programid, name.c_str()), value);
}

void myshader::setMat4(const string &name, glm::mat4 mat) {
	glUniformMatrix4fv(glGetUniformLocation(programid, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void myshader::setVec3(const string &name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(programid, name.c_str()), 1, &value[0]);
}