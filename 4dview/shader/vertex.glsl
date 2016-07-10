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
 * @file vertex.glsl
 * @date 2015. 9. 18.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#version 330 core

struct vec5
{
	float v[5];
};
struct mat5
{
	float m[25];
};

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out VS_OUT
{
	vec4 fragColor;
} vs_out;

uniform mat5 projMatrix4d;
uniform mat5 vmMatrix4d;

uniform mat4 projMatrix3d;
uniform mat4 vmMatrix3d;

vec5 to_vec5(vec4 v, float u);
vec4 from_vec5(vec5 v);
vec5 mul5(mat5 m, vec5 v);

void main()
{
	vec5 pos5 = to_vec5(position, 1.0f);
	pos5 = mul5(vmMatrix4d, pos5);
	pos5 = mul5(projMatrix4d, pos5);

	vec4 pos4 = from_vec5(pos5);
	gl_Position = projMatrix3d * vmMatrix3d * pos4;

	vs_out.fragColor = color;
}

vec5 to_vec5(vec4 v, float u)
{
	vec5 rs;
	rs.v[0] = v[0];
	rs.v[1] = v[1];
	rs.v[2] = v[2];
	rs.v[3] = v[3];
	rs.v[4] = u;
	return rs;
}

vec4 from_vec5(vec5 v)
{
	vec4 rs = vec4(v.v[0], v.v[1], v.v[2], v.v[3]) / v.v[4];
	return vec4(rs.xyz, 1.0f);
}

vec5 mul5(mat5 m, vec5 v)
{
	vec5 rs;
	rs.v[0] = m.m[0 + 0]*v.v[0] + m.m[0 + 1]*v.v[1] + m.m[0 + 2]*v.v[2] + m.m[0 + 3]*v.v[3] + m.m[0 + 4]*v.v[4];
	rs.v[1] = m.m[5 + 0]*v.v[0] + m.m[5 + 1]*v.v[1] + m.m[5 + 2]*v.v[2] + m.m[5 + 3]*v.v[3] + m.m[5 + 4]*v.v[4];
	rs.v[2] = m.m[10 + 0]*v.v[0] + m.m[10 + 1]*v.v[1] + m.m[10 + 2]*v.v[2] + m.m[10 + 3]*v.v[3] + m.m[10 + 4]*v.v[4];
	rs.v[3] = m.m[15 + 0]*v.v[0] + m.m[15 + 1]*v.v[1] + m.m[15 + 2]*v.v[2] + m.m[15 + 3]*v.v[3] + m.m[15 + 4]*v.v[4];
	rs.v[4] = m.m[20 + 0]*v.v[0] + m.m[20 + 1]*v.v[1] + m.m[20 + 2]*v.v[2] + m.m[20 + 3]*v.v[3] + m.m[20 + 4]*v.v[4];
	return rs;
}
