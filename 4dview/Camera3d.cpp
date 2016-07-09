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
 * @file Camera3d.cpp
 * @date 2015. 9. 24.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "Camera3d.h"

Camera3d::Camera3d()
	: m_position(0, 0, 3), m_distance(3), m_pitch(0.0f), m_yaw(0.0f)
{
	calculate();
}

Camera3d::~Camera3d()
{
	// TODO Auto-generated destructor stub
}

const glm::mat4 &Camera3d::getMatrix() const
{
	return m_matrix;
}

const glm::mat4 & Camera3d::getProjection() const
{
	return m_projection;
}

float Camera3d::getPitch() const
{
	return m_pitch;
}

float Camera3d::getYaw() const
{
	return m_yaw;
}

void Camera3d::setPitchYaw(float pitch, float yaw)
{
	m_pitch = pitch;
	m_yaw = yaw;

	// std::abs(pitch) must not be bigger than 90
	if (pitch > 89.99f)
		pitch = 89.99f;
	else if (pitch < -89.99f)
		pitch = -89.99f;

	calculate();
}

void Camera3d::move(int front, int right, float unit)
{
	///*
	m_position += front * unit * m_front;
	m_position += right * unit * m_right;
	//*/
	/*
	float f = front * unit;
	float r = right * unit;
	float sin_yaw = std::sin(m_yaw - glm::pi<float>() / 2);
	float cos_yaw = std::cos(m_yaw - glm::pi<float>() / 2);
	m_position += glm::vec3(
		cos_yaw * f - sin_yaw * r,
		0,
		sin_yaw * f + cos_yaw * r
		);
	//*/
	calculate();
}

void Camera3d::calcProjection(float aspect)
{
	m_projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void Camera3d::calculate()
{
	float sin_pitch = std::sin(m_pitch);
	float cos_pitch = std::cos(m_pitch);
	float sin_yaw = std::sin(m_yaw - glm::pi<float>() / 2);
	float cos_yaw = std::cos(m_yaw - glm::pi<float>() / 2);

	m_front = glm::vec3(
		m_distance * cos_pitch * cos_yaw,
		m_distance * sin_pitch,
		m_distance * cos_pitch * sin_yaw
		);

	m_right = glm::normalize(glm::cross(m_front, glm::vec3(0, 1, 0)));
	m_up = glm::cross(m_right, m_front);

	m_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
}
