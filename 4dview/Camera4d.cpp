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
	: m_position(0, 0, 3, 0), m_distance(3)
	, m_pitch(0.0f), m_roll(0.0f), m_yaw(0.0f)
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

float Camera4d::getPitch() const
{
	return m_pitch;
}

float Camera4d::getRoll() const
{
	return m_roll;
}

float Camera4d::getYaw() const
{
	return m_yaw;
}

void Camera4d::setPitchRollYaw(float pitch, float roll, float yaw)
{
	m_pitch = pitch;
	m_roll = roll;
	m_yaw = yaw;

	// std::abs(pitch) must not be bigger than 90
	if (pitch > 89.99f)
		pitch = 89.99f;
	else if (pitch < -89.99f)
		pitch = -89.99f;

	calculate();
}

void Camera4d::move(int front, int right, int over, float unit)
{
	m_position += front * unit * m_front;
	m_position += right * unit * m_right;
	m_position += over * unit * m_over;
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
	float sin_pitch = std::sin(m_pitch);
	float cos_pitch = std::cos(m_pitch);
	float sin_yaw = std::sin(m_yaw);
	float cos_yaw = std::cos(m_yaw);
	float sin_roll = std::sin(m_roll - glm::pi<float>() / 2);
	float cos_roll = std::cos(m_roll - glm::pi<float>() / 2);

	m_front = glm::vec4(
		cos_roll * cos_pitch * cos_yaw,
		cos_roll * sin_pitch,
		cos_roll * cos_pitch * sin_yaw,
		sin_roll
	);

	m_right = glm::normalize(math4d::cross4(
		m_front, glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0)));
	m_up = glm::normalize(math4d::cross4(
		glm::vec4(0, 0, 1, 0), m_right, m_front));
	m_over = math4d::cross4(
		m_right, m_front, m_up);

	m_matrix = math4d::mat5 {
		-m_right.x, m_up.x, m_over.x, m_front.x, 0,
		-m_right.y, m_up.y, m_over.y, m_front.y, 0,
		-m_right.z, m_up.z, m_over.z, m_front.z, 0,
		-m_right.w, m_up.w, m_over.w, m_front.w, m_distance,
		0, 0, 0, 0, 1,
	};
}
