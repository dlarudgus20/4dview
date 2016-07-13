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
* @file math4d.h
* @date 2016. 7. 3.
* @author dlarudgus20
* @copyright The BSD (2-Clause) License
*/

#ifndef MATH4D_H_
#define MATH4D_H_

namespace math4d
{
	struct vec5
	{
		float v[5];
	};

	struct mat5
	{
		union
		{
			float m[25];
			float _v[5][5];
		};
	};

	inline vec5 to_vec5(const glm::vec4 &v, float u)
	{
		vec5 rs;
		rs.v[0] = v[0];
		rs.v[1] = v[1];
		rs.v[2] = v[2];
		rs.v[3] = v[3];
		rs.v[5] = u;
	}

	inline glm::vec4 from_vec5(const vec5 &v)
	{
		return glm::vec4(v.v[0], v.v[1], v.v[2], v.v[3]) / v.v[4];
	}

	inline float dot(const vec5 &lhs, const vec5 &rhs)
	{
		return lhs.v[0] * rhs.v[0]
			+ lhs.v[1] * rhs.v[1]
			+ lhs.v[2] * rhs.v[2]
			+ lhs.v[3] * rhs.v[3]
			+ lhs.v[4] * rhs.v[4];
	}
	inline vec5 operator *(const vec5 &lhs, float rhs)
	{
		return {
			lhs.v[0] * rhs,
			lhs.v[1] * rhs,
			lhs.v[2] * rhs,
			lhs.v[3] * rhs,
			lhs.v[4] * rhs
		};
	}
	inline vec5 operator *(float lhs, const vec5 &rhs)
	{
		return rhs * lhs;
	}
	inline vec5 operator /(const vec5 &lhs, float rhs)
	{
		return {
			lhs.v[0] / rhs,
			lhs.v[1] / rhs,
			lhs.v[2] / rhs,
			lhs.v[3] / rhs,
			lhs.v[4] / rhs
		};
	}
	inline float norm(const vec5 &v)
	{
		return std::sqrt(dot(v, v));
	}
	inline vec5 normalize(const vec5& v)
	{
		return v / norm(v);
	}

	inline glm::vec4 cross4(const glm::vec4 &u, const glm::vec4 &v, const glm::vec4 &w)
	{
		float a, b, c, d, e, f;

		a = (v[0] * w[1]) - (v[1] * w[0]);
		b = (v[0] * w[2]) - (v[2] * w[0]);
		c = (v[0] * w[3]) - (v[3] * w[0]);
		d = (v[1] * w[2]) - (v[2] * w[1]);
		e = (v[1] * w[3]) - (v[3] * w[1]);
		f = (v[2] * w[3]) - (v[3] * w[2]);

		glm::vec4 rs;
		rs[0] = (u[1] * f) - (u[2] * e) + (u[3] * d);
		rs[1] = -(u[0] * f) + (u[2] * c) - (u[3] * b);
		rs[2] = (u[0] * e) - (u[1] * c) + (u[3] * a);
		rs[3] = -(u[0] * d) + (u[1] * b) - (u[2] * a);

		return rs;
	}

	inline mat5 identity()
	{
		static mat5 id = {
			1, 0, 0, 0, 0,
			0, 1, 0, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, 1, 0,
			0, 0, 0, 0, 1,
		};
		return id;
	}

	inline mat5 rotate_xy(float theta)
	{
		float s = std::sin(theta);
		float c = std::cos(theta);

		return {
			c, -s, 0, 0, 0,
			s, c, 0, 0, 0,
			0, 0, 1, 0, 0,
			0, 0, 0, 1, 0,
			0, 0, 0, 0, 1
		};
	}
	inline mat5 rotate_yz(float theta)
	{
		float s = std::sin(theta);
		float c = std::cos(theta);

		return {
			1, 0, 0, 0, 0,
			0, c, -s, 0, 0,
			0, s, c, 0, 0,
			0, 0, 0, 1, 0,
			0, 0, 0, 0, 1
		};
	}
	inline mat5 rotate_zx(float theta)
	{
		float s = std::sin(theta);
		float c = std::cos(theta);

		return {
			s, 0, c, 0, 0,
			0, 1, 0, 0, 0,
			c, 0, -s, 0, 0,
			0, 0, 0, 1, 0,
			0, 0, 0, 0, 1
		};
	}
	inline mat5 rotate_xw(float theta)
	{
		float s = std::sin(theta);
		float c = std::cos(theta);

		return {
			c, 0, 0, -s, 0,
			0, 1, 0, 0, 0,
			0, 0, 1, 0, 0,
			s, 0, 0, c, 0,
			0, 0, 0, 0, 1
		};
	}
	inline mat5 rotate_yw(float theta)
	{
		float s = std::sin(theta);
		float c = std::cos(theta);

		return {
			1, 0, 0, 0, 0,
			0, c, 0, -s, 0,
			0, 0, 1, 0, 0,
			0, s, 0, c, 0,
			0, 0, 0, 0, 1
		};
	}
	inline mat5 rotate_zw(float theta)
	{
		float s = std::sin(theta);
		float c = std::cos(theta);

		return {
			1, 0, 0, 0, 0,
			0, 1, 0, 0, 0,
			0, 0, c, -s, 0,
			0, 0, s, c, 0,
			0, 0, 0, 0, 1
		};
	}

	inline vec5 mul5(const mat5 &m, const vec5 &v)
	{
		vec5 rs;
		rs.v[0] = m.m[0 + 0] * v.v[0] + m.m[0 + 1] * v.v[1] + m.m[0 + 2] * v.v[2] + m.m[0 + 3] * v.v[3] + m.m[0 + 4] * v.v[4];
		rs.v[1] = m.m[5 + 0] * v.v[0] + m.m[5 + 1] * v.v[1] + m.m[5 + 2] * v.v[2] + m.m[5 + 3] * v.v[3] + m.m[5 + 4] * v.v[4];
		rs.v[2] = m.m[10 + 0] * v.v[0] + m.m[10 + 1] * v.v[1] + m.m[10 + 2] * v.v[2] + m.m[10 + 3] * v.v[3] + m.m[10 + 4] * v.v[4];
		rs.v[3] = m.m[15 + 0] * v.v[0] + m.m[15 + 1] * v.v[1] + m.m[15 + 2] * v.v[2] + m.m[15 + 3] * v.v[3] + m.m[15 + 4] * v.v[4];
		rs.v[4] = m.m[20 + 0] * v.v[0] + m.m[20 + 1] * v.v[1] + m.m[20 + 2] * v.v[2] + m.m[20 + 3] * v.v[3] + m.m[20 + 4] * v.v[4];
		return rs;
	}

	inline mat5 mul5(const mat5 &m1, const mat5 &m2)
	{
		mat5 rs;
		for (int i = 0; i < 25; ++i)
		{
			rs.m[i] = 0;
			for (int j = 0; j < 5; ++j)
			{
				for (int k = 0; k < 5; ++k)
				{
					int a = i / 5;
					int b = i % 5;
					rs.m[i] += m1.m[a * 5 + j] * m2.m[k * 5 + b];
				}
			}
		}
		return rs;
	}
}

#endif /* VEC5_H_ */
