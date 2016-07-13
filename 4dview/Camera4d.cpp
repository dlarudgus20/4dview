// Copyright (c) 2014, ÀÓ°æÇö
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
* @file Camera3d.cpp
* @date 2016. 7. 3.
* @author dlarudgus20
* @copyright The BSD (2-Clause) License
*/

#include "pch.h"
#include "Camera4d.h"

Camera4d::Camera4d()
{
	calculate();
}

Camera4d::~Camera4d()
{
	// TODO Auto-generated destructor stub
}

const math4d::mat5 &Camera4d::getMatrix() const
{
	return m_matrix;
}

const math4d::mat5 & Camera4d::getProjection() const
{
	return m_projection;
}

float Camera4d::getTheta1() const
{
	return m_theta1;
}

float Camera4d::getTheta2() const
{
	return m_theta2;
}

float Camera4d::getTheta3() const
{
	return m_theta3;
}

void Camera4d::setTheta(float theta1, float theta2, float theta3)
{
	m_theta1 = theta1;
	m_theta2 = theta2;
	m_theta3 = theta3;

	calculate();
}

void Camera4d::calcProjection()
{
	const float fovy = glm::radians(45.0f);
	const float near = 0.1f;
	const float far = 100.0f;

	float t = std::tan(fovy / 2);

	float A = -t * (near + far);
	float B = (-2 * t * near * far) / (far - near);

	m_projection = math4d::mat5 {
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, A, B, 0,
		0, 0, 0, -t, 0,
	};
}

void Camera4d::calculate()
{
	const float distance = 3.0f;

	float sin1 = std::sin(m_theta1);
	float cos1 = std::cos(m_theta1);
	float sin2 = std::sin(m_theta2);
	float cos2 = std::cos(m_theta2);
	float sin3 = std::sin(m_theta3);
	float cos3 = std::cos(m_theta3);

	glm::vec4 front = glm::vec4(
		-sin1 * sin2 * sin3,
		-sin1 * sin2 * cos3,
		-sin1 * cos2,
		-cos1
	);

	glm::vec4 right = glm::normalize(math4d::cross4(
		front, glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0)));
	glm::vec4 up = glm::normalize(math4d::cross4(
		glm::vec4(0, 0, 1, 0), right, front));
	glm::vec4 over = math4d::cross4(
		right, front, up);

	glm::vec4 pos = -distance * front;

	m_matrix = math4d::mat5 {
		-right.x, up.x, over.x, front.x, pos.x,
		-right.y, up.y, over.y, front.y, pos.y,
		-right.z, up.z, over.z, front.z, pos.z,
		-right.w, up.w, over.w, front.w, pos.w,
		0, 0, 0, 0, 1,
	};
}
