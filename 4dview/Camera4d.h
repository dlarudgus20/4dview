// Copyright (c) 2014, �Ӱ���
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
* @file Camera4d.h
* @date 2016. 7. 3.
* @author dlarudgus20
* @copyright The BSD (2-Clause) License
*/

#ifndef CAMERA4D_H_
#define CAMERA4D_H_

#include "math4d.h"

class Camera4d
{
private:
	glm::vec4 m_position;
	float m_distance;

	float m_pitch, m_roll, m_yaw;
	glm::vec4 m_front, m_right, m_up, m_over;

	math4d::mat5 m_matrix = math4d::identity();
	math4d::mat5 m_projection = math4d::identity();

public:
	Camera4d();
	~Camera4d();

	const math4d::mat5 &getMatrix() const;
	const math4d::mat5 &getProjection() const;

	float getPitch() const;
	float getRoll() const;
	float getYaw() const;
	void setPitchRollYaw(float pitch, float roll, float yaw);

	void move(int front, int right, int over, float unit);

	void calcProjection();

private:
	void calculate();
};

#endif /* CAMERA4D_H_ */
