#include "myshader.h"
myshader::myshader(const char* vertexPath, const char* fragmentPath)
{
	// ���ļ��ж�ȡ���ݵ��ַ���
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	// ȷ���ܹ��׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ����
		vShaderFile.close();
		fShaderFile.close();
		// �����е�����ת�����ַ���
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	//��ȡ����Դ������ַ���
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	//�������
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// ���붥����ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "���붥����ɫ��ʱ���� " << infoLog << endl;
	}
	// ����Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "����Ƭ����ɫ��ʱ���� " << infoLog << endl;
	}
	// ������ɫ��
	programid = glCreateProgram();
	glAttachShader(programid, vertex);
	glAttachShader(programid, fragment);
	glLinkProgram(programid);
	glGetProgramiv(programid, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programid, 1000, NULL, infoLog);
		cout << "���ӳ���ʱ���� " << infoLog << endl;
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