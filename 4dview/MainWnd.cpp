// Copyright (c) 2014, 임경현
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file MainWnd.cpp
 * @date 2015. 9. 17.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "ext.h"
#include "MainWnd.h"
#include "MainScene.h"

const int MAINWND_WIDTH = 800;
const int MAINWND_HEIGHT = 600;

std::unique_ptr<MainWnd> MainWnd::createInstance()
{
	return std::unique_ptr<MainWnd>(new MainWnd());
}

MainWnd::MainWnd()
	: m_wnd(nullptr)
{
	// TODO Auto-generated constructor stub
}

MainWnd::~MainWnd()
{
	// TODO Auto-generated destructor stub
}

bool MainWnd::create()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_wnd = glfwCreateWindow(MAINWND_WIDTH, MAINWND_HEIGHT,
		"pfps (press F1 to print help) https://github.com/dlarudgus20/4dview",
		nullptr, nullptr);
	if (m_wnd != nullptr)
	{
		glfwMakeContextCurrent(m_wnd);
		glfwSetWindowUserPointer(m_wnd, this);

		initCallback();
		return true;
	}
	else
	{
		return false;
	}
}

auto timeStr = [] (double time) {
	int s = static_cast<int>(time);
	char buf[11];
	int hour = s / 3600;
	int minutes = (s % 3600) / 60;
	int seconds = s % 60;
	snprintf(buf, 11, "[%02d:%02d:%02d]", hour, minutes, seconds);
	return std::string(buf);
};

void MainWnd::loop()
{
	static double startTime = glfwGetTime();
	static double frameTime = startTime, prevFrameTime;
	static double fpsResetTime = startTime + 1.0f;
	static int fps = 0;

	while (!glfwWindowShouldClose(m_wnd))
	{
		glfwPollEvents();
		render();
		fps++;
		idle();

		prevFrameTime = frameTime;
		frameTime = glfwGetTime();
		m_deltaTime = frameTime - prevFrameTime;

		if (fpsResetTime <= frameTime)
		{
			m_fps = fps;
			fps = 0;
			fpsResetTime += 1.0;
			std::cout << timeStr(frameTime) << " fps: " << m_fps << std::endl;
		}
	}

	glfwDestroyWindow(m_wnd);
}

void MainWnd::loopManualFPS()
{
	const double expectedDelta = 1.0 / 60.0;

	static double startTime = glfwGetTime();

	static double frameTime = startTime, prevFrameTime;
	static double remain = 0.0f;
	static int fps = 0;

	static double fpsResetTime = startTime + 1.0;

	while (!glfwWindowShouldClose(m_wnd))
	{
		glfwPollEvents();

		if (remain <= 0.0f)
		{
			render();
			fps++;
		}

		idle();

		prevFrameTime = frameTime;
		frameTime = glfwGetTime();
		m_deltaTime = frameTime - prevFrameTime;

		if (fpsResetTime <= frameTime)
		{
			m_fps = fps;
			fps = 0;
			fpsResetTime += 1.0;
			std::cout << timeStr(frameTime) << " fps: " << m_fps << std::endl;
		}

		if (remain <= 0.0f)
		{
			remain += expectedDelta - m_deltaTime;
		}
		else
		{
			remain -= m_deltaTime;
		}
	}

	glfwDestroyWindow(m_wnd);
}

bool MainWnd::tryClose()
{
	glfwSetWindowShouldClose(m_wnd, 1);
	onWindowClose();

	return (glfwWindowShouldClose(m_wnd) == 1);
}

void MainWnd::initCallback()
{
#define THIZ static_cast<MainWnd *>(glfwGetWindowUserPointer(wnd))
	glfwSetKeyCallback(m_wnd, [](GLFWwindow *wnd, int key, int scancode, int action, int mods) {
		THIZ->onKeyInput(key, scancode, action, mods);
	});
	glfwSetCursorPosCallback(m_wnd, [](GLFWwindow *wnd, double xpos, double ypos) {
		THIZ->onMouseCursorPos(xpos, ypos);
	});
	glfwSetFramebufferSizeCallback(m_wnd, [] (GLFWwindow *wnd, int width, int height) {
		THIZ->onFrameBufferSize(width, height);
	});
	glfwSetWindowCloseCallback(m_wnd, [](GLFWwindow *wnd) {
		THIZ->onWindowClose();
	});
#undef THIZ
}

bool MainWnd::initialize()
{
	glfwSetInputMode(m_wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int width, height;
	glfwGetFramebufferSize(m_wnd, &width, &height);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	calcProjection(width, height);

	glEnable(GL_DEPTH_TEST);

	try
	{
		std::cout << "->compile shader" << std::endl;
		m_shader.compile("shader/vertex.glsl", "shader/fragment.glsl");
		std::cout << ">vertex shader info:\n" << m_shader.getVSInfoString() << std::endl;
		std::cout << ">fragment shader info:\n" << m_shader.getFSInfoString() << std::endl;
	}
	catch (Shader::CompileError &e)
	{
		std::cerr << "##compile error##\n" << e.what() << std::endl;
		return false;
	}

	m_pScene = ext::make_unique<MainScene>();

	m_camera4d.calcProjection();

	return true;
}

void MainWnd::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader.use();
	m_shader.setUniformMatrix4f("projMatrix3d", m_camera3d.getProjection());
	m_shader.setUniform1fv("projMatrix4d.m", 25, m_camera4d.getProjection().m);

	m_pScene->render(m_camera3d.getMatrix(), m_camera4d.getMatrix());

	glfwSwapBuffers(m_wnd);
}

void MainWnd::idle()
{
	const float unit3d = 2.5f;
	int front = 0, right = 0;
	front += glfwGetKey(m_wnd, GLFW_KEY_W) == GLFW_PRESS ? 1 : 0;
	front += glfwGetKey(m_wnd, GLFW_KEY_S) == GLFW_PRESS ? -1 : 0;
	right += glfwGetKey(m_wnd, GLFW_KEY_D) == GLFW_PRESS ? 1 : 0;
	right += glfwGetKey(m_wnd, GLFW_KEY_A) == GLFW_PRESS ? -1 : 0;
	m_camera3d.move(front, right, unit3d * (float)m_deltaTime);

	const float unit4d = glm::radians(90.0f);
	int theta1 = 0, theta2 = 0, theta3 = 0;
	theta1 += glfwGetKey(m_wnd, GLFW_KEY_R) == GLFW_PRESS ? 1 : 0;
	theta1 += glfwGetKey(m_wnd, GLFW_KEY_T) == GLFW_PRESS ? -1 : 0;
	theta2 += glfwGetKey(m_wnd, GLFW_KEY_F) == GLFW_PRESS ? 1 : 0;
	theta2 += glfwGetKey(m_wnd, GLFW_KEY_G) == GLFW_PRESS ? -1 : 0;
	theta3 += glfwGetKey(m_wnd, GLFW_KEY_V) == GLFW_PRESS ? 1 : 0;
	theta3 += glfwGetKey(m_wnd, GLFW_KEY_B) == GLFW_PRESS ? -1 : 0;
	m_camera4d.setTheta(
		m_camera4d.getTheta1() + unit4d * theta1 * (float)m_deltaTime,
		m_camera4d.getTheta2() + unit4d * theta2 * (float)m_deltaTime,
		m_camera4d.getTheta3() + unit4d * theta3 * (float)m_deltaTime);


	const float unitrot = glm::radians(90.0f);
	int rot[6] = { 0, 0, 0, 0, 0, 0 };
	rot[0] += glfwGetKey(m_wnd, GLFW_KEY_Y) == GLFW_PRESS ? 1 : 0;
	rot[0] += glfwGetKey(m_wnd, GLFW_KEY_U) == GLFW_PRESS ? -1 : 0;
	rot[1] += glfwGetKey(m_wnd, GLFW_KEY_H) == GLFW_PRESS ? 1 : 0;
	rot[1] += glfwGetKey(m_wnd, GLFW_KEY_J) == GLFW_PRESS ? -1 : 0;
	rot[2] += glfwGetKey(m_wnd, GLFW_KEY_N) == GLFW_PRESS ? 1 : 0;
	rot[2] += glfwGetKey(m_wnd, GLFW_KEY_M) == GLFW_PRESS ? -1 : 0;
	rot[3] += glfwGetKey(m_wnd, GLFW_KEY_I) == GLFW_PRESS ? 1 : 0;
	rot[3] += glfwGetKey(m_wnd, GLFW_KEY_O) == GLFW_PRESS ? -1 : 0;
	rot[4] += glfwGetKey(m_wnd, GLFW_KEY_K) == GLFW_PRESS ? 1 : 0;
	rot[4] += glfwGetKey(m_wnd, GLFW_KEY_L) == GLFW_PRESS ? -1 : 0;
	rot[5] += glfwGetKey(m_wnd, GLFW_KEY_COMMA) == GLFW_PRESS ? 1 : 0;
	rot[5] += glfwGetKey(m_wnd, GLFW_KEY_PERIOD) == GLFW_PRESS ? -1 : 0;
	for (int i = 0; i < 6; ++i)
	{
		static_cast<MainScene *>(m_pScene.get())->getRotate()[i]
			+= unitrot * rot[i] * (float)m_deltaTime;
	}
}

void MainWnd::onKeyInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		m_pScene->setIsWireframe(!m_pScene->getIsWireframe());
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		tryClose();
	}
	else if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		std::cout <<
			"git repo : https://github.com/dlarudgus20/4dview\n"
			"W/A/S/D/mouse : move 3d-space camera\n"
			"R/T : rotate 4d-space camera (theta1)\n"
			"F/G : rotate 4d-space camera (theta2)\n"
			"V/B : rotate 4d-space camera (theta3)\n"
			"Y/U : rotate object (XY)\n"
			"H/J : rotate object (YZ)\n"
			"N/M : rotate object (ZX)\n"
			"I/O : rotate object (XW)\n"
			"K/L : rotate object (YW)\n"
			",/. : rotate object (ZW)\n"
			"1 : turn on/off wireframe\n"
			"Q: quit\n"
			"F1 : show this message"
			<< std::endl;
	}
}

void MainWnd::onMouseCursorPos(double xpos, double ypos)
{
	if (m_isFirstMouse)
	{
		m_prevMouseX = (float)xpos;
		m_prevMouseY = (float)ypos;
		m_isFirstMouse = false;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(m_wnd, &width, &height);
		if (!(width == 0 || height == 0))
		{
			const float maxPitch = glm::radians(75.0f);

			const float unit = glm::radians(60.0f);

			float dx = (float)xpos - m_prevMouseX;
			float dy = -((float)ypos - m_prevMouseY);

			float pitch = m_camera3d.getPitch() + (dy / height) * unit;
			float yaw = m_camera3d.getYaw() + (dx / width) * unit;

			if (pitch > maxPitch)
				pitch = maxPitch;
			else if (pitch < -maxPitch)
				pitch = -maxPitch;

			m_camera3d.setPitchYaw(pitch, yaw);

			m_prevMouseX = (float)xpos;
			m_prevMouseY = (float)ypos;
		}
	}
}

void MainWnd::onFrameBufferSize(int width, int height)
{
	// 최소화 상태일땐 width == height == 0임.
	if (!(width == 0 || height == 0))
	{
		glViewport(0, 0, width, height);
		calcProjection(width, height);
	}
}

void MainWnd::onWindowClose()
{
	// TODO: onWindowClose
}

void MainWnd::calcProjection(int width, int height)
{
	float aspect;

	// 최소화 상태일땐 width == height == 0임.
	if (width == 0 || height == 0)
		aspect = 1.0f;
	else
		aspect = (float)width / height;

	m_camera3d.calcProjection(aspect);
}
