#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "myshader.h"
#include <stdio.h>

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
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
//定义点类
class MYPOINT {
public:
	float x;
	float y;
	MYPOINT();
	MYPOINT(float x, float y);
	~MYPOINT();
	MYPOINT operator*(float m);//乘法  
	MYPOINT operator+(MYPOINT p);//加法  
	void operator=(MYPOINT p);//赋值
	bool operator<(MYPOINT p);//小于
};

MYPOINT::MYPOINT() {
	this->x = .0f;
	this->y = .0f;
}

MYPOINT::MYPOINT(float x, float y) {
	this->x = x;
	this->y = y;
}

MYPOINT::~MYPOINT() {
	;
}

MYPOINT MYPOINT::operator*(float m) {
	MYPOINT c(this->x*m, this->y*m);
	return c;
}

MYPOINT MYPOINT::operator+(MYPOINT p) {
	MYPOINT c(this->x+p.x, this->y+p.y);
	return c;
}

void MYPOINT::operator=(MYPOINT p) {
	this->x = p.x;
	this->y = p.y;
}

bool MYPOINT::operator<(MYPOINT p) {
	return this->x < p.x;
}

//按递归的方式计算目标点
MYPOINT getB(vector<MYPOINT> points, float t, int i, int n) {
	if (n == 0) {
		return points[i];
	}
	return getB(points, t, i - 1, n - 1) * (1 - t) + getB(points, t, i, n - 1) * t;
}

//保存中间结果
MYPOINT getDB(vector<MYPOINT> points, float t, int i, int n, vector<MYPOINT>&result) {
	if (n == 0) {
		//result.push_back(points[i]);
		return points[i];	
	}
	MYPOINT x = getDB(points, t, i - 1, n - 1, result) * (1 - t) + getDB(points, t, i, n - 1, result) * t;
	result.push_back(x);
	return x;
}

float* dynamicpoints_to_vertices(vector<MYPOINT>points) {
	float* vertices = new float[12 * (points.size() - 1)];
	int current = 0;
	for (int i = 0; i < points.size() - 1; i++) {
		vertices[current++] = points[i].x;
		vertices[current++] = points[i].y;
		vertices[current++] = 0.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = points[i + 1].x;
		vertices[current++] = points[i + 1].y;
		vertices[current++] = 0.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
	}
	return vertices;
}

//将二维坐标转换为vertices数组，第一个vector是绘制点的，第二个vector是绘制线的
float* points_to_vertices(vector<MYPOINT>points, vector<MYPOINT>controlpoints) {
	//有几个点
	int num = points.size();
	//要画的线需要几个端点
	int linenum = 2 * (controlpoints.size() - 1);
	float* vertices = new float[num * 6 + linenum * 6];
	for (int i = 0; i < num; i++) {
		vertices[i * 6] = points[i].x;
		vertices[i * 6 + 1] = points[i].y;
		vertices[i * 6 + 2] = 0.0f;
		vertices[i * 6 + 3] = 0.0f;
		vertices[i * 6 + 4] = 0.0f;
		vertices[i * 6 + 5] = 1.0f;
	}
	int current = num * 6;
	for (int i = 0; i < controlpoints.size() - 1; i++) {
		vertices[current++] = controlpoints[i].x;
		vertices[current++] = controlpoints[i].y;
		vertices[current++] = 0.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = controlpoints[i + 1].x;
		vertices[current++] = controlpoints[i + 1].y;
		vertices[current++] = 0.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
		vertices[current++] = 1.0f;
	}
	return vertices;
}


//注册改变窗口时视口的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
//处理键盘输入函数，输入ESC之后退出
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//定义存储控制点的全局变量
vector<MYPOINT> P;
//定义状态是否发生了改变
bool if_state_change = false;
//定义帧之间相隔的时间
//float deltaTime = 0.0f;
//float curframe = 0.0f;
//float lastframe = 0.0f;
//鼠标点击的回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		P.push_back(MYPOINT(float((xpos - 400.0) / 400.0), float((400.0 - ypos) / 400.0)));
		//P.push_back(MYPOINT(float(xpos), float(ypos)));
		if_state_change = true;
		//cout << xpos << " " << ypos << endl;
	}
	else {
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			if (!P.empty()) {
				P.pop_back();
				if_state_change = true;
			}
		}
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
	GLFWwindow* mwindow = glfwCreateWindow(800, 800, "drawline", NULL, NULL);
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
	//点击鼠标的函数
	glfwSetMouseButtonCallback(mwindow, mouse_button_callback);
	//绑定glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "加载GLAD失败！" << endl;
		return -1;
	}
	//初始化视口
	glViewport(0, 0, 400, 400);
	glfwSwapInterval(1);
	//使用VAO和VBO完成数据的存储和解析
	unsigned int VBOID, VAOID;
	//分别创建这两个对象，第一个参数的含义为返回当前n个未使用的名称
	glGenVertexArrays(1, &VAOID);
	glGenBuffers(1, &VBOID);
	glBindVertexArray(0);
	unsigned int newVBOID, newVAOID;
	//分别创建这两个对象，第一个参数的含义为返回当前n个未使用的名称
	glGenVertexArrays(1, &newVAOID);
	glGenBuffers(1, &newVBOID);
	glBindVertexArray(0);
	myshader newshader("newshader.vs", "newshader.fs");
	myshader dyshader("newshader.vs", "newshader.fs");
	while (!glfwWindowShouldClose(mwindow)) {
		glfwPollEvents();
		processInput(mwindow);
		int display_w, display_h;
		glfwGetFramebufferSize(mwindow, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		//对颜色进行渲染, 也就是改了背景板
		newshader.usepro();
		glBindVertexArray(VAOID);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID);
		dyshader.usepro();
		glBindVertexArray(newVAOID);
		glBindBuffer(GL_ARRAY_BUFFER, newVBOID);
		//构建顶点
		float* vertices = NULL;
		float* dvertices = NULL;
		int num_of_points = 400;
		int num = P.size();
		float tt = (int)(float(glfwGetTime()*500.f)) % 500 / 500.f;
		vector<MYPOINT> dynamicpoints;
		if (num > 1) {
			getDB(P, tt, num - 1, num - 1, dynamicpoints);
			dvertices = dynamicpoints_to_vertices(dynamicpoints);
		}
		if (if_state_change) {
			if (num > 1) {
				vector<MYPOINT> temp = P;
				//对各个控制点按照从左到右的顺序进行排序
				//sort(temp.begin(), temp.end());
				vector<MYPOINT> everypoints;
				float t = .0f;
				for (unsigned int i = 0; i < num_of_points; i++) {
					t += 1.0f / num_of_points;
					everypoints.push_back(getB(temp, t, num - 1, num - 1));
					//cout << everypoints.back().x << " " << everypoints.back().y << endl;
				}
				vertices = points_to_vertices(everypoints, temp);
			}
			else {
				num_of_points = 0;
			}
			if_state_change = false;
		}
		//将顶点数据复制到缓冲的内存中（VBO）
		glBufferData(GL_ARRAY_BUFFER, 6 * (num_of_points + 2 * (P.size() - 1)) * sizeof(float), vertices, GL_STATIC_DRAW);
		//指定要修改的着色器的顶点变量ID并且启用其属性
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//解除绑定
		glDrawArrays(GL_POINTS, 0, num_of_points);
		if (P.size() > 1) {
			glDrawArrays(GL_LINES, num_of_points, 2 * (P.size() - 1));
		}
		glBindVertexArray(VAOID);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID);
		cout << dynamicpoints.size();
		if (dynamicpoints.size() > 1) {
			//将顶点数据复制到缓冲的内存中（VBO）
			glBufferData(GL_ARRAY_BUFFER, 12 * (dynamicpoints.size() - 1) * sizeof(float), dvertices, GL_STATIC_DRAW);
			//指定要修改的着色器的顶点变量ID并且启用其属性
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glDrawArrays(GL_LINES, 0, 2 * (dynamicpoints.size() - 1));
			glBindVertexArray(newVAOID);
			glBindBuffer(GL_ARRAY_BUFFER, newVBOID);
		}
		if (vertices) {
			delete vertices;
		}
		if (dvertices) {
			delete dvertices;
		}
		newshader.freepro();
		dyshader.freepro();
		glfwMakeContextCurrent(mwindow);
		glfwSwapBuffers(mwindow);
	}
	//释放对象
	glDeleteVertexArrays(1, &VAOID);
	glDeleteBuffers(1, &VBOID);
	//结束程序
	glfwTerminate();
	return 0;
}

