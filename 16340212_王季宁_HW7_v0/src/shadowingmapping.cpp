#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "myshader.h"
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include "gl3w.h"    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
//#include "myshader.h"
#include <iostream>
using namespace std;

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// ��ʼ����Ⱥ͸߶�
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// ���庯��
void RenderScene(myshader &shader);
void RenderCube();
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// �÷����ƽ��ӵ����ͬ������
GLuint colourtexture;
GLuint planeVAO;
GLuint cubeVAO = 0;
GLuint cubeVBO = 0;

int main()
{
	//��ʼ��GLFW
	glfwInit();
	//ָ�����汾��Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//ָ���ΰ汾��Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//ʹ�ú���ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//��������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "draw3D", NULL, NULL);
	if (window == NULL) {
		cout << "���ڽ���ʧ�ܣ�\n";
		//��ֹGLFW
		glfwTerminate();
		return -1;
	}
	//����������������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(window);
	//�󶨻ص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "����GLADʧ�ܣ�" << endl;
		return -1;
	}

	//������ȼ��
	glEnable(GL_DEPTH_TEST);

	// ��ʼ��shader
	myshader shader("shadow_mapping.vs", "shadow_mapping.fs");
	myshader simpleDepthShader("shadow_mapping_depth.vs", "shadow_mapping_depth.fs");

	shader.usepro();
	glUniform1i(glGetUniformLocation(shader.programid, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(shader.programid, "shadowMap"), 1);

	//����ƽ������
	GLfloat planeVertices[] = {
		// ��������          // ��������        // ��������
		50.0f, -0.4f, 50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.1f,
		-50.0f, -0.4f, -50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-50.0f, -0.4f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		50.0f, -0.4f, 50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		50.0f, -0.4f, -50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-50.0f, -0.4f, -50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
	};
	// ����ƽ��VAO
	GLuint planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);


	// ���ù�Դ����
	float pos1 = -2.0f;
	float pos2 = 20.0f;
	float pos3 = -1.0f;
	glm::vec3 lightPos(pos1, pos2, pos3);

	
	// ��������
	glGenTextures(1, &colourtexture);
	glBindTexture(GL_TEXTURE_2D, colourtexture);
	//ָ����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ����ͼƬ
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("img/white.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ͼƬ����ʧ�ܣ�\n";
	}

	// Ϊ�����ͼ����һ��֡�������
	const GLuint SHADOW_WIDTH = 1600, SHADOW_HEIGHT = 1200;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// ����һ��2D�����ṩ��֡�������Ȼ���ʹ��
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float radius = 70.0f;
	float rotate = 188.0f;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//��ʼ��
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	bool use_ortho = true;
	while (!glfwWindowShouldClose(window))
	{
		lightPos.x = pos1;
		lightPos.y = pos2;
		lightPos.z = pos3;
		//��һ�仰һ��Ҫ������ǰ�棬��Ȼ���д�ӣ���֮ǰ�Ķ�������ʾ������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window);
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		GLfloat near_plane = 0.0f, far_plane = 100.0f;
		if (use_ortho) {
			lightProjection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, near_plane, far_plane);
		}
		else {
			lightProjection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 60.0f, 200.0f);
		}
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		simpleDepthShader.usepro();
		glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.programid, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		RenderScene(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.usepro();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::vec3 viewpos = glm::vec3(sin(rotate)*radius, 0.0f, cos(rotate)*radius);
		glm::mat4 view = glm::lookAt(viewpos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 1.0, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programid, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.programid, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// ���ݹ��ո������
		glUniform3fv(glGetUniformLocation(shader.programid, "lightPos"), 1, &lightPos[0]);
		glUniform3fv(glGetUniformLocation(shader.programid, "viewPos"), 1, &viewpos[0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.programid, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colourtexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene(shader);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("shadowingmapping");
		ImGui::SliderFloat("x of the light position", &pos1, -2.0f, 2.0f);
		ImGui::SliderFloat("y of the light position", &pos2, 10.0f, 40.0f);
		ImGui::SliderFloat("z of the light position", &pos3, -2.0f, 2.0f);
		ImGui::Checkbox("use ortho", &use_ortho);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

//��Ⱦ����
void RenderScene(myshader &shader) {

	//��Ⱦƽ��
	glm::mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(shader.programid, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	// ��Ⱦ������
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.programid, "model"), 1, GL_FALSE, glm::value_ptr(model));
	RenderCube();
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(shader.programid, "model"), 1, GL_FALSE, glm::value_ptr(model));
	RenderCube();
	model = glm::mat4();
	glUniformMatrix4fv(glGetUniformLocation(shader.programid, "model"), 1, GL_FALSE, glm::value_ptr(model));
	RenderCube();
}



void RenderCube()
{
	// ��VAO���г�ʼ��
	if (cubeVAO == 0)
	{
		GLfloat vertices[] = {

			-0.1f, -0.1f, -0.1f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			0.1f, 0.1f, -0.1f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			0.1f, -0.1f, -0.1f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
			0.1f, 0.1f, -0.1f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
			-0.1f, -0.1f, -0.1f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			-0.1f, 0.1f, -0.1f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

			-0.1f, -0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.1f, -0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			0.1f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			0.1f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.1f, 0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.1f, -0.1f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			-0.1f, 0.1f, 0.1f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			-0.1f, 0.1f, -0.1f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.1f, -0.1f, -0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.1f, -0.1f, -0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.1f, -0.1f, 0.1f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			-0.1f, 0.1f, 0.1f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			0.1f, 0.1f, 0.1f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.1f, -0.1f, -0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.1f, 0.1f, -0.1f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.1f, -0.1f, -0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.1f, 0.1f, 0.1f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.1f, -0.1f, 0.1f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

			-0.1f, -0.1f, -0.1f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			0.1f, -0.1f, -0.1f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
			0.1f, -0.1f, 0.1f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			0.1f, -0.1f, 0.1f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-0.1f, -0.1f, 0.1f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			-0.1f, -0.1f, -0.1f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

			-0.1f, 0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.1f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.1f, 0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.1f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-0.1f, 0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.1f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// ��仺����
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// ���Ӷ�������
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// ��Ⱦ������
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

