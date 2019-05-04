#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "myshader.h"
#include "camera.h"
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
#include "myshader.h"
#include <iostream>
using namespace std;

// 初始化宽度和高度
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
//处理键盘输入函数，输入ESC之后退出
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	//初始化GLFW
	glfwInit();
	//指定主版本号为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//指定次版本号为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//创建窗口
	GLFWwindow* mwindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "draw3D", NULL, NULL);
	if (mwindow == NULL) {
		cout << "窗口建立失败！\n";
		//终止GLFW
		glfwTerminate();
		return -1;
	}
	//将窗口上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(mwindow);
	//绑定回调函数
	glfwSetFramebufferSizeCallback(mwindow, framebuffer_size_callback);
	//绑定glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "加载GLAD失败！" << endl;
		return -1;
	}

	//开启深度检测
	glEnable(GL_DEPTH_TEST);
	//关闭深度检测
	//glDisable(GL_DEPTH_TEST);

	//新建着色器类
	myshader cubeshader("phongshading.vs", "phongshading.fs");
	myshader cubegshader("GouraudShading.vs", "GouraudShading.fs");
	myshader lightshader("newshader.vs", "newshader.fs");
	//顶点坐标初始化
	float vertices[] = {
	  -0.2f, -0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
	  0.2f, -0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
	  0.2f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
	  0.2f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
	  -0.2f,  0.2f, -0.2f,  0.0f,  0.0f, -1.0f,
	  -0.2f, -0.2f, -0.2f,  0.0f,  0.0f, -1.0f,

	  -0.2f, -0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
	  0.2f, -0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
	  0.2f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
	  0.2f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
	  -0.2f,  0.2f,  0.2f,  0.0f,  0.0f,  1.0f,
	  -0.2f, -0.2f,  0.2f,  0.0f,  0.0f,  1.0f,

	  -0.2f,  0.2f,  0.2f, -1.0f,  0.0f,  0.0f,
	  -0.2f,  0.2f, -0.2f, -1.0f,  0.0f,  0.0f,
	  -0.2f, -0.2f, -0.2f, -1.0f,  0.0f,  0.0f,
	  -0.2f, -0.2f, -0.2f, -1.0f,  0.0f,  0.0f,
	  -0.2f, -0.2f,  0.2f, -1.0f,  0.0f,  0.0f,
	  -0.2f,  0.2f,  0.2f, -1.0f,  0.0f,  0.0f,

	  0.2f,  0.2f,  0.2f,  1.0f,  0.0f,  0.0f,
	  0.2f,  0.2f, -0.2f,  1.0f,  0.0f,  0.0f,
	  0.2f, -0.2f, -0.2f,  1.0f,  0.0f,  0.0f,
	  0.2f, -0.2f, -0.2f,  1.0f,  0.0f,  0.0f,
	  0.2f, -0.2f,  0.2f,  1.0f,  0.0f,  0.0f,
	  0.2f,  0.2f,  0.2f,  1.0f,  0.0f,  0.0f,

	  -0.2f, -0.2f, -0.2f,  0.0f, -1.0f,  0.0f,
	  0.2f, -0.2f, -0.2f,  0.0f, -1.0f,  0.0f,
	  0.2f, -0.2f,  0.2f,  0.0f, -1.0f,  0.0f,
	  0.2f, -0.2f,  0.2f,  0.0f, -1.0f,  0.0f,
	  -0.2f, -0.2f,  0.2f,  0.0f, -1.0f,  0.0f,
	  -0.2f, -0.2f, -0.2f,  0.0f, -1.0f,  0.0f,

	  -0.2f,  0.2f, -0.2f,  0.0f,  1.0f,  0.0f,
	  0.2f,  0.2f, -0.2f,  0.0f,  1.0f,  0.0f,
	  0.2f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,
	  0.2f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,
	  -0.2f,  0.2f,  0.2f,  0.0f,  1.0f,  0.0f,
	  -0.2f,  0.2f, -0.2f,  0.0f,  1.0f,  0.0f
	};
	//使用VAO和VBO完成数据的存储和解析
	unsigned int VBO, VAO, lightVAO;
	//分别创建这两个对象，第一个参数的含义为返回当前n个未使用的名称
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//指定要修改的着色器的顶点变量ID并且启用其属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	lightshader.usepro();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//初始化
	ImGui_ImplGlfw_InitForOpenGL(mwindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	//分别定义二者的位置
	glm::vec3 cubepos = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 lightpos = glm::vec3(0.6, 0.3, 0.3);
	//定义观察者位置
	glm::vec3 viewpos = glm::vec3(40.0f, 0.0f, 40.0f);
	//通过IMGUI来改变的部分参数
	bool use_phong = true;
	bool light_move = false;
	float aStrength = 0.1f;
	float specularStrength = 0.5f;
	float diffusefactor = 1.0f;
	float specfactor = 32;
	float radius = 70.0f;
	float rotate = 188.0f;
	while (!glfwWindowShouldClose(mwindow)) {
		//更新当前帧
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Lighting");
		ImGui::Checkbox("use phong shading", &use_phong);
		ImGui::Checkbox("light move", &light_move);
		ImGui::SliderFloat("change the ambient strength", &aStrength, 0.0f, 1.1f);
		ImGui::SliderFloat("change the specular strength", &specularStrength, 0.1f, 0.9f);
		ImGui::SliderFloat("change the diffuse factor", &diffusefactor, 0.3f, 3.0f);
		ImGui::SliderFloat("change the spec factor", &specfactor, 0.0f, 200.0f);
		ImGui::SliderFloat("change the radius", &radius, 60.0f, 95.0f);
		ImGui::SliderFloat("change the rotate angle", &rotate, 0.0f, 360.0f);
		processInput(mwindow);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//按照单位矩阵对三个变换矩阵进行初始化
		viewpos = glm::vec3(sin(rotate)*radius, 0.0f, cos(rotate)*radius);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::translate(model, cubepos);
		model = glm::rotate(model, glm::radians(90.0f) * 20, glm::vec3(1.0f, 1.0f, 0.0f));
		view = glm::lookAt(viewpos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 1.0, 0.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//新建着色器类
		myshader cubeshader("phongshading.vs", "phongshading.fs");
		myshader cubegshader("GouraudShading.vs", "GouraudShading.fs");
		myshader lightshader("newshader.vs", "newshader.fs");
		if (use_phong) {
			cubeshader.usepro();
			cubeshader.setVec3("object_Color", glm::vec3(1.0f, 0.7f, 0.6f));
			cubeshader.setVec3("light_Color", glm::vec3(1.0f, 1.0f, 1.0f));
			cubeshader.setVec3("light_pos", lightpos);
			cubeshader.setVec3("view_pos", viewpos);
			cubeshader.setFloat("aStrength", aStrength);
			cubeshader.setFloat("specularStrength", specularStrength);
			cubeshader.setFloat("diffusefactor", diffusefactor);
			cubeshader.setInt("specfactor", int(specfactor));
			unsigned int modelLoc = glGetUniformLocation(cubeshader.programid, "model");
			unsigned int viewLoc = glGetUniformLocation(cubeshader.programid, "view");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
			cubeshader.setMat4("projection", projection);
			glBindVertexArray(VAO);
		}
		else {
			cubegshader.usepro();
			cubegshader.setVec3("object_color", glm::vec3(1.0f, 0.7f, 0.6f));
			cubegshader.setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
			cubegshader.setVec3("light_pos", lightpos);
			cubegshader.setVec3("view_pos", viewpos);
			cubegshader.setFloat("aStrength", aStrength);
			cubegshader.setFloat("specularStrength", specularStrength);
			cubegshader.setFloat("diffusefactor", diffusefactor);
			cubegshader.setInt("specfactor", int(specfactor));
			unsigned int modelLoc = glGetUniformLocation(cubegshader.programid, "model");
			unsigned int viewLoc = glGetUniformLocation(cubegshader.programid, "view");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
			cubegshader.setMat4("projection", projection);
			glBindVertexArray(VAO);
		}
		//绘制三十六个顶点
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightshader.usepro();
		lightshader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
		lightshader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		//按照单位矩阵对三个变换矩阵进行初始化
		glm::mat4 lightmodel = glm::mat4(1.0f);
		if (light_move) {
			lightpos.x = sin(glfwGetTime())*0.6f;
			lightpos.z = cos(glfwGetTime())*0.3f;
			lightpos.y = cos(glfwGetTime())*0.3f;
		}
		lightmodel = glm::translate(lightmodel, lightpos);
		lightmodel = glm::rotate(lightmodel, glm::radians(90.0f) * 20, glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 lightview = glm::mat4(1.0f);
		lightview = glm::translate(lightview, glm::vec3(0.0f, 0.0f, -80.0f));
		glm::mat4 lightprojection = projection;
		lightshader.setMat4("model", lightmodel);
		lightshader.setMat4("view", lightview);
		lightshader.setMat4("projection", lightprojection);
		glBindVertexArray(lightVAO);
		////绘制三十六个顶点
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glfwSwapBuffers(mwindow);
		glfwPollEvents();
	}
	//释放对象
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//结束程序
	glfwTerminate();
	return 0;
}
