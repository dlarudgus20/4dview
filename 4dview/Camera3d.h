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
 * @file Camera3d.h
 * @date 2015. 9. 24.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#ifndef CAMERA3D_H_
#define CAMERA3D_H_

class Camera3d
{
private:
	glm::vec3 m_position;
	float m_distance;

	float m_pitch, m_yaw;
	glm::vec3 m_front, m_right, m_up;

	glm::mat4 m_matrix;

	glm::mat4 m_projection;

public:
	Camera3d();
	~Camera3d();

	const glm::mat4 &getMatrix() const;
	const glm::mat4 &getProjection() const;

	float getPitch() const;
	float getYaw() const;
	void setPitchYaw(float pitch, float yaw);

	void move(int front, int right, float unit);

	void calcProjection(float aspect);

private:
	void calculate();
};

#endif /* CAMERA3D_H_ */
