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
 * @file MainScene.cpp
 * @date 2015. 10. 23.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "MainScene.h"
#include "Shader.h"

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}

bool MainScene::getIsWireframe() const
{
	return m_bWireframe;
}

void MainScene::setIsWireframe(bool wire)
{
	m_bWireframe = wire;
}

void MainScene::render(const glm::mat4 &viewMatrix3, const math4d::mat5 &viewMatrix4) const
{
	Shader *pShader = Shader::getCurrentShader();

	glm::mat4 vmMatrix3 = viewMatrix3;
	math4d::mat5 vmMatrix4 = viewMatrix4;

	if (m_bWireframe)
	{
		glDisable(GL_BLEND);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	{
		math4d::mat5 prevMat = vmMatrix4;
		vmMatrix4 = math4d::mul5(vmMatrix4, math4d::rotate_xy(m_rotate[0]));
		vmMatrix4 = math4d::mul5(vmMatrix4, math4d::rotate_yz(m_rotate[1]));
		vmMatrix4 = math4d::mul5(vmMatrix4, math4d::rotate_zx(m_rotate[2]));
		vmMatrix4 = math4d::mul5(vmMatrix4, math4d::rotate_xw(m_rotate[3]));
		vmMatrix4 = math4d::mul5(vmMatrix4, math4d::rotate_yw(m_rotate[4]));
		vmMatrix4 = math4d::mul5(vmMatrix4, math4d::rotate_zw(m_rotate[5]));
		pShader->setUniformMatrix4f("vmMatrix3d", vmMatrix3);
		pShader->setUniform1fv("vmMatrix4d.m", 25, vmMatrix4.m);
		m_container.draw();
		vmMatrix4 = prevMat;
	}
}
