#define _USE_MATH_DEFINES
#include <Novice.h>
#include <cmath>
#include <stdlib.h>
#include <assert.h>
#include <imgui.h>

#include <algorithm>
#include <vector>

const char kWindowTitle[] = "LE2B_07_クリタ_ユウダイ_タイトル";

const int kWindowWidth = 1280;
const int kWindowHeight = 720;


struct Vector3
{
	float x, y, z;

	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
};

struct Matrix4x4
{
	float m[4][4];
};

static const int kColumnWidth = 60;
static const int kRowHeight = 20;



// 00_01
// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v)
{
	return { scalar * v.x, scalar * v.y, scalar * v.z };
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// 長さ(ノルム)
float Length(const Vector3& v)
{
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

// 正規化
Vector3 Normalize(const Vector3& v)
{
	float length = Length(v);

	return { v.x / length, v.y / length, v.z / length };
}

Vector3 operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }
Vector3 operator+(const Vector3& v) { return { v.x, v.y, v.z }; }

Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }
Vector3 operator*(float s, const Vector3& v) { return Multiply(s, v); }
Vector3 operator*(const Vector3& v, float s) { return s * v; }
Vector3 operator/(const Vector3& v, float s) { return Multiply(1.0f / s, v); }

// 3次元ベクトルの数値表示
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// 00_02
// 1. 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 m3;
	m3 =
	{
		m1.m[0][0] + m2.m[0][0], m1.m[0][1] + m2.m[0][1], m1.m[0][2] + m2.m[0][2], m1.m[0][3] + m2.m[0][3],
		m1.m[1][0] + m2.m[1][0], m1.m[1][1] + m2.m[1][1], m1.m[1][2] + m2.m[1][2], m1.m[1][3] + m2.m[1][3],
		m1.m[2][0] + m2.m[2][0], m1.m[2][1] + m2.m[2][1], m1.m[2][2] + m2.m[2][2], m1.m[2][3] + m2.m[2][3],
		m1.m[3][0] + m2.m[3][0], m1.m[3][1] + m2.m[3][1], m1.m[3][2] + m2.m[3][2], m1.m[3][3] + m2.m[3][3],
	};
	return m3;
}

// 2. 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 m3;
	m3 =
	{
		m1.m[0][0] - m2.m[0][0], m1.m[0][1] - m2.m[0][1], m1.m[0][2] - m2.m[0][2], m1.m[0][3] - m2.m[0][3],
		m1.m[1][0] - m2.m[1][0], m1.m[1][1] - m2.m[1][1], m1.m[1][2] - m2.m[1][2], m1.m[1][3] - m2.m[1][3],
		m1.m[2][0] - m2.m[2][0], m1.m[2][1] - m2.m[2][1], m1.m[2][2] - m2.m[2][2], m1.m[2][3] - m2.m[2][3],
		m1.m[3][0] - m2.m[3][0], m1.m[3][1] - m2.m[3][1], m1.m[3][2] - m2.m[3][2], m1.m[3][3] - m2.m[3][3],
	};
	return m3;
}

// 3. 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 m3;
	float factor1[4], factor2[4] = { 0.0f };
	for (int row = 0; row < 4; ++row)
	{
		factor1[0] = m1.m[row][0];
		factor1[1] = m1.m[row][1];
		factor1[2] = m1.m[row][2];
		factor1[3] = m1.m[row][3];
		for (int column = 0; column < 4; ++column)
		{
			factor2[0] = m2.m[0][column];
			factor2[1] = m2.m[1][column];
			factor2[2] = m2.m[2][column];
			factor2[3] = m2.m[3][column];
			m3.m[row][column]
				= factor1[0] * factor2[0] + factor1[1] * factor2[1] + factor1[2] * factor2[2] + factor1[3] * factor2[3];
		}
	}
	return m3;
}

Matrix4x4 Multiply(float s, const Matrix4x4& m)
{
	Matrix4x4 m2;
	m2 =
	{
		s * m.m[0][0], s * m.m[0][1], s * m.m[0][2], s * m.m[0][3],
		s * m.m[1][0], s * m.m[1][1], s * m.m[1][2], s * m.m[1][3],
		s * m.m[2][0], s * m.m[2][1], s * m.m[2][2], s * m.m[2][3],
		s * m.m[3][0], s * m.m[3][1], s * m.m[3][2], s * m.m[3][3],
	};
	return m2;
}

// 4. 逆行列
Matrix4x4 Inverse(const Matrix4x4& a)
{
	Matrix4x4 inverseM;
	inverseM.m[0][0]
		= (a.m[1][1] * a.m[2][2] * a.m[3][3]) + (a.m[1][2] * a.m[2][3] * a.m[3][1]) + (a.m[1][3] * a.m[2][1] * a.m[3][2])
		- (a.m[1][3] * a.m[2][2] * a.m[3][1]) - (a.m[1][2] * a.m[2][1] * a.m[3][3]) - (a.m[1][1] * a.m[2][3] * a.m[3][2]);
	inverseM.m[0][1]
		= -(a.m[0][1] * a.m[2][2] * a.m[3][3]) - (a.m[0][2] * a.m[2][3] * a.m[3][1]) - (a.m[0][3] * a.m[2][1] * a.m[3][2])
		+ (a.m[0][3] * a.m[2][2] * a.m[3][1]) + (a.m[0][2] * a.m[2][1] * a.m[3][3]) + (a.m[0][1] * a.m[2][3] * a.m[3][2]);
	inverseM.m[0][2]
		= (a.m[0][1] * a.m[1][2] * a.m[3][3]) + (a.m[0][2] * a.m[1][3] * a.m[3][1]) + (a.m[0][3] * a.m[1][1] * a.m[3][2])
		- (a.m[0][3] * a.m[1][2] * a.m[3][1]) - (a.m[0][2] * a.m[1][1] * a.m[3][3]) - (a.m[0][1] * a.m[1][3] * a.m[3][2]);
	inverseM.m[0][3]
		= -(a.m[0][1] * a.m[1][2] * a.m[2][3]) - (a.m[0][2] * a.m[1][3] * a.m[2][1]) - (a.m[0][3] * a.m[1][1] * a.m[2][2])
		+ (a.m[0][3] * a.m[1][2] * a.m[2][1]) + (a.m[0][2] * a.m[1][1] * a.m[2][3]) + (a.m[0][1] * a.m[1][3] * a.m[2][2]);

	inverseM.m[1][0]
		= -(a.m[1][0] * a.m[2][2] * a.m[3][3]) - (a.m[1][2] * a.m[2][3] * a.m[3][0]) - (a.m[1][3] * a.m[2][0] * a.m[3][2])
		+ (a.m[1][3] * a.m[2][2] * a.m[3][0]) + (a.m[1][2] * a.m[2][0] * a.m[3][3]) + (a.m[1][0] * a.m[2][3] * a.m[3][2]);
	inverseM.m[1][1]
		= (a.m[0][0] * a.m[2][2] * a.m[3][3]) + (a.m[0][2] * a.m[2][3] * a.m[3][0]) + (a.m[0][3] * a.m[2][0] * a.m[3][2])
		- (a.m[0][3] * a.m[2][2] * a.m[3][0]) - (a.m[0][2] * a.m[2][0] * a.m[3][3]) - (a.m[0][0] * a.m[2][3] * a.m[3][2]);
	inverseM.m[1][2]
		= -(a.m[0][0] * a.m[1][2] * a.m[3][3]) - (a.m[0][2] * a.m[1][3] * a.m[3][0]) - (a.m[0][3] * a.m[1][0] * a.m[3][2])
		+ (a.m[0][3] * a.m[1][2] * a.m[3][0]) + (a.m[0][2] * a.m[1][0] * a.m[3][3]) + (a.m[0][0] * a.m[1][3] * a.m[3][2]);
	inverseM.m[1][3]
		= (a.m[0][0] * a.m[1][2] * a.m[2][3]) + (a.m[0][2] * a.m[1][3] * a.m[2][0]) + (a.m[0][3] * a.m[1][0] * a.m[2][2])
		- (a.m[0][3] * a.m[1][2] * a.m[2][0]) - (a.m[0][2] * a.m[1][0] * a.m[2][3]) - (a.m[0][0] * a.m[1][3] * a.m[2][2]);

	inverseM.m[2][0]
		= (a.m[1][0] * a.m[2][1] * a.m[3][3]) + (a.m[1][1] * a.m[2][3] * a.m[3][0]) + (a.m[1][3] * a.m[2][0] * a.m[3][1])
		- (a.m[1][3] * a.m[2][1] * a.m[3][0]) - (a.m[1][1] * a.m[2][0] * a.m[3][3]) - (a.m[1][0] * a.m[2][3] * a.m[3][1]);
	inverseM.m[2][1]
		= -(a.m[0][0] * a.m[2][1] * a.m[3][3]) - (a.m[0][1] * a.m[2][3] * a.m[3][0]) - (a.m[0][3] * a.m[2][0] * a.m[3][1])
		+ (a.m[0][3] * a.m[2][1] * a.m[3][0]) + (a.m[0][1] * a.m[2][0] * a.m[3][3]) + (a.m[0][0] * a.m[2][3] * a.m[3][1]);
	inverseM.m[2][2]
		= (a.m[0][0] * a.m[1][1] * a.m[3][3]) + (a.m[0][1] * a.m[1][3] * a.m[3][0]) + (a.m[0][3] * a.m[1][0] * a.m[3][1])
		- (a.m[0][3] * a.m[1][1] * a.m[3][0]) - (a.m[0][1] * a.m[1][0] * a.m[3][3]) - (a.m[0][0] * a.m[1][3] * a.m[3][1]);
	inverseM.m[2][3]
		= -(a.m[0][0] * a.m[1][1] * a.m[2][3]) - (a.m[0][1] * a.m[1][3] * a.m[2][0]) - (a.m[0][3] * a.m[1][0] * a.m[2][1])
		+ (a.m[0][3] * a.m[1][1] * a.m[2][0]) + (a.m[0][1] * a.m[1][0] * a.m[2][3]) + (a.m[0][0] * a.m[1][3] * a.m[2][1]);

	inverseM.m[3][0]
		= -(a.m[1][0] * a.m[2][1] * a.m[3][2]) - (a.m[1][1] * a.m[2][2] * a.m[3][0]) - (a.m[1][2] * a.m[2][0] * a.m[3][1])
		+ (a.m[1][2] * a.m[2][1] * a.m[3][0]) + (a.m[1][1] * a.m[2][0] * a.m[3][2]) + (a.m[1][0] * a.m[2][2] * a.m[3][1]);
	inverseM.m[3][1]
		= (a.m[0][0] * a.m[2][1] * a.m[3][2]) + (a.m[0][1] * a.m[2][2] * a.m[3][0]) + (a.m[0][2] * a.m[2][0] * a.m[3][1])
		- (a.m[0][2] * a.m[2][1] * a.m[3][0]) - (a.m[0][1] * a.m[2][0] * a.m[3][2]) - (a.m[0][0] * a.m[2][2] * a.m[3][1]);
	inverseM.m[3][2]
		= -(a.m[0][0] * a.m[1][1] * a.m[3][2]) - (a.m[0][1] * a.m[1][2] * a.m[3][0]) - (a.m[0][2] * a.m[1][0] * a.m[3][1])
		+ (a.m[0][2] * a.m[1][1] * a.m[3][0]) + (a.m[0][1] * a.m[1][0] * a.m[3][2]) + (a.m[0][0] * a.m[1][2] * a.m[3][1]);
	inverseM.m[3][3]
		= (a.m[0][0] * a.m[1][1] * a.m[2][2]) + (a.m[0][1] * a.m[1][2] * a.m[2][0]) + (a.m[0][2] * a.m[1][0] * a.m[2][1])
		- (a.m[0][2] * a.m[1][1] * a.m[2][0]) - (a.m[0][1] * a.m[1][0] * a.m[2][2]) - (a.m[0][0] * a.m[1][2] * a.m[2][1]);

	float A =
		a.m[0][0] * a.m[1][1] * a.m[2][2] * a.m[3][3] + a.m[0][0] * a.m[1][2] * a.m[2][3] * a.m[3][1] + a.m[0][0] * a.m[1][3] * a.m[2][1] * a.m[3][2]
		- a.m[0][0] * a.m[1][3] * a.m[2][2] * a.m[3][1] - a.m[0][0] * a.m[1][2] * a.m[2][1] * a.m[3][3] - a.m[0][0] * a.m[1][1] * a.m[2][3] * a.m[3][2]
		- a.m[0][1] * a.m[1][0] * a.m[2][2] * a.m[3][3] - a.m[0][2] * a.m[1][0] * a.m[2][3] * a.m[3][1] - a.m[0][3] * a.m[1][0] * a.m[2][1] * a.m[3][2]
		+ a.m[0][3] * a.m[1][0] * a.m[2][2] * a.m[3][1] + a.m[0][2] * a.m[1][0] * a.m[2][1] * a.m[3][3] + a.m[0][1] * a.m[1][0] * a.m[2][3] * a.m[3][2]
		+ a.m[0][1] * a.m[1][2] * a.m[2][0] * a.m[3][3] + a.m[0][2] * a.m[1][3] * a.m[2][0] * a.m[3][1] + a.m[0][3] * a.m[1][1] * a.m[2][0] * a.m[3][2]
		- a.m[0][3] * a.m[1][2] * a.m[2][0] * a.m[3][1] - a.m[0][2] * a.m[1][1] * a.m[2][0] * a.m[3][3] - a.m[0][1] * a.m[1][3] * a.m[2][0] * a.m[3][2]
		- a.m[0][1] * a.m[1][2] * a.m[2][3] * a.m[3][0] - a.m[0][2] * a.m[1][3] * a.m[2][1] * a.m[3][0] - a.m[0][3] * a.m[1][1] * a.m[2][2] * a.m[3][0]
		+ a.m[0][3] * a.m[1][2] * a.m[2][1] * a.m[3][0] + a.m[0][2] * a.m[1][1] * a.m[2][3] * a.m[3][0] + a.m[0][1] * a.m[1][3] * a.m[2][2] * a.m[3][0];

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			inverseM.m[i][j] *= 1 / A;
		}
	}

	return inverseM;
}

// 5. 転置行列
Matrix4x4 Transpose(const Matrix4x4& m)
{
	Matrix4x4 m1 = { 0.0f };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m1.m[i][j] = m.m[j][i];
		}
	}

	return m1;
}

// 6. 単位行列の作成
Matrix4x4 MakeIdentity4x4()
{
	return { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return Add(m1, m2); }
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return Subtract(m1, m2); }
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Multiply(m1, m2); }

// 4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.03f", matrix.m[row][column]
			);
		}
	}
}

// 00_03
// 1. 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	return
	{
		1.0f,		 0.0f,		  0.0f,		   0.0f,
		0.0f,		 1.0f,		  0.0f,		   0.0f,
		0.0f,		 0.0f,		  1.0f,		   0.0f,
		translate.x, translate.y, translate.z, 1.0f
	};
}

// 2. 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	return
	{
		scale.x,	0.0f,		0.0f,		0.0f,
		0.0f,		scale.y,	0.0f,		0.0f,
		0.0f,		0.0f,		scale.z,	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	};
}

// 3. 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0]
		+ vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1]
		+ vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2]
		+ vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float  w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3]
		+ vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

// 00_04
// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
	return
	{
		 1.0f,	 0.0f,			   0.0f,			 0.0f,
		 0.0f,   std::cos(radian), std::sin(radian), 0.0f,
		 0.0f,	-std::sin(radian), std::cos(radian), 0.0f,
		 0.0f,	 0.0f,			   0.0f,			 1.0f
	};
}

// 2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
	return
	{
		 std::cos(radian), 0.0f, -std::sin(radian), 0.0f,
		 0.0f,			   1.0f,  0.0f,				0.0f,
		 std::sin(radian), 0.0f,  std::cos(radian), 0.0f,
		 0.0f,			   0.0f,  0.0f,				1.0f
	};
}

// 3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
	return
	{
		 std::cos(radian), std::sin(radian), 0.0f, 0.0f,
		-std::sin(radian), std::cos(radian), 0.0f, 0.0f,
		 0.0f,			   0.0f,			 1.0f, 0.0f,
		 0.0f,			   0.0f,			 0.0f, 1.0f
	};
}

//// 00_05
//// 3次元アフィン変換行列
//Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
//{
//	return Matrix4x4
//	{
//
//		scale.x * (std::cos(rotate.y) * std::cos(rotate.z)),
//		scale.x * (std::cos(rotate.y) * std::sin(rotate.z)),
//		scale.x * (-std::sin(rotate.y)),
//		0.0f,
//
//		scale.y * (-std::cos(rotate.x) * std::sin(rotate.z) + std::sin(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z)),
//		scale.y * (std::cos(rotate.x) * std::cos(rotate.z) + std::sin(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z)),
//		scale.y * (std::sin(rotate.x) * std::cos(rotate.y)),
//		0.0f,
//
//		scale.z * (std::sin(rotate.x) * std::sin(rotate.z) + std::cos(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z)),
//		scale.z * (-std::sin(rotate.x) * std::cos(rotate.z) + std::cos(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z)),
//		scale.z * (std::cos(rotate.x) * std::cos(rotate.y)),
//		0.0f,
//
//		translate.x,
//		translate.y,
//		translate.z,
//		1.0f
//	};
//
//}

// 3次元アフィン変換行列（Y → X → Z）
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 rotateMatrix = Multiply(MakeRotateZMatrix(rotate.z),
		Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)));

	return Multiply(Multiply(MakeScaleMatrix(scale), rotateMatrix), MakeTranslateMatrix(translate));
}

// 3次元アフィン変換行列(GPT製)（Y → X → Z）
Matrix4x4 MakeAffineMatrixB(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	float cx = cosf(rotate.x), sx = sinf(rotate.x);
	float cy = cosf(rotate.y), sy = sinf(rotate.y);
	float cz = cosf(rotate.z), sz = sinf(rotate.z);

	Matrix4x4 m;

	m.m[0][0] = scale.x * (cz * cy + sz * sx * sy);
	m.m[0][1] = scale.x * (sz * cx);
	m.m[0][2] = scale.x * (cz * -sy + sz * sx * cy);
	m.m[0][3] = 0.0f;

	m.m[1][0] = scale.y * (-sz * cy + cz * sx * sy);
	m.m[1][1] = scale.y * (cz * cx);
	m.m[1][2] = scale.y * (-sz * -sy + cz * sx * cy);
	m.m[1][3] = 0.0f;

	m.m[2][0] = scale.z * (cx * sy);
	m.m[2][1] = scale.z * (-sx);
	m.m[2][2] = scale.z * (cx * cy);
	m.m[2][3] = 0.0f;

	m.m[3][0] = translate.x;
	m.m[3][1] = translate.y;
	m.m[3][2] = translate.z;
	m.m[3][3] = 1.0f;

	return m;
}

// 3次元アフィン変換行列（FPSカメラ用）(Y → X)
Matrix4x4 MakeAffineMatrixFPS(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 rotateMatrix = Multiply(
		MakeRotateXMatrix(rotate.x),  // Pitch（上下）
		MakeRotateYMatrix(rotate.y)   // Yaw（左右）
	);
	return Multiply(Multiply(MakeScaleMatrix(scale), rotateMatrix), MakeTranslateMatrix(translate));
}

// 01_00

// cot（コタンジェント）関数が無いので作る
float cot(float theta)
{
	return 1.0f / std::tan(theta);
}

// 1. 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	return
	{
		(1.0f / aspectRatio) * cot(fovY / 2.0f), 0.0f, 0.0f, 0.0f,
		0.0f, cot(fovY / 2.0f), 0.0f, 0.0f,
		0.0f, 0.0f, farClip / (farClip - nearClip), 1.0f,
		0.0f, 0.0f, (-nearClip * farClip) / (farClip - nearClip), 0.0f,
	};
}

// 2. 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	return
	{
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f
	};
}

// 3. ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	return
	{
		width / 2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -height / 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, maxDepth - minDepth, 0.0f,
		left + (width / 2.0f), top + (height / 2.0f), minDepth, 1.0f
	};
}

// 01_01
// クロス積関数
Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

/// <summary>
/// 指定した座標を中心とする正三角形の頂点を計算する関数
/// </summary>
/// <param name="center">正三角形の中心座標</param>
/// <param name="sideLength">正三角形の辺の長さ</param>
/// <param name="vertices">正三角形の頂点を格納する配列</param>
void MakeRegularTriangle(const Vector3& center, float sideLength, Vector3 vertices[3])
{
	// 正三角形の高さを計算
	float height = std::sqrt(3.0f) / 2.0f * sideLength;

	// 頂点の計算
	vertices[0] = { center.x, center.y + 2.0f / 3.0f * height, center.z }; // 上の頂点
	vertices[1] = { center.x - sideLength / 2.0f, center.y - 1.0f / 3.0f * height, center.z }; // 左下の頂点
	vertices[2] = { center.x + sideLength / 2.0f, center.y - 1.0f / 3.0f * height, center.z }; // 右下の頂点
}

void RotateAngleCorrection(float& rotate)
{
	if (rotate > float(M_PI))
	{
		rotate = rotate - 2.0f * float(M_PI);
	}
	if (rotate < -float(M_PI))
	{
		rotate = rotate + 2.0f * float(M_PI);
	}
}

// 01_02
// Gridを表示する関数
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 0.5f;										// Gridの半分の幅
	const uint32_t kSubdivision = 10;										// 分割数												
	//const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);	// 1つ分の長さ
	Vector3 start, end, screenStart, screenEnd;
	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		start = { kGridHalfWidth * (xIndex - kSubdivision / 2.0f), 0.0f, kGridHalfWidth * kSubdivision / 2.0f };
		end = { kGridHalfWidth * (xIndex - kSubdivision / 2.0f), 0.0f, -kGridHalfWidth * kSubdivision / 2.0f };
		screenStart = Transform(start, viewProjectionMatrix);
		screenEnd = Transform(end, viewProjectionMatrix);
		screenStart = Transform(screenStart, viewportMatrix);
		screenEnd = Transform(screenEnd, viewportMatrix);
		if (xIndex != kSubdivision / 2.0f)
		{
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
		}
	}
	// 左から右への線も順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		start = { kGridHalfWidth * kSubdivision / 2.0f, 0.0f, kGridHalfWidth * (zIndex - kSubdivision / 2.0f) };
		end = { -kGridHalfWidth * kSubdivision / 2.0f, 0.0f, kGridHalfWidth * (zIndex - kSubdivision / 2.0f) };
		screenStart = Transform(start, viewProjectionMatrix);
		screenEnd = Transform(end, viewProjectionMatrix);
		screenStart = Transform(screenStart, viewportMatrix);
		screenEnd = Transform(screenEnd, viewportMatrix);
		if (zIndex != kSubdivision / 2.0f)
		{
			Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
				static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0xAAAAAAFF);
		}
	}
	start = { 0.0f, 0.0f, kGridHalfWidth * kSubdivision / 2.0f };
	end = { 0.0f, 0.0f, -kGridHalfWidth * kSubdivision / 2.0f };
	screenStart = Transform(start, viewProjectionMatrix);
	screenEnd = Transform(end, viewProjectionMatrix);
	screenStart = Transform(screenStart, viewportMatrix);
	screenEnd = Transform(screenEnd, viewportMatrix);
	Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
		static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0x000000FF);

	start = { kGridHalfWidth * kSubdivision / 2.0f, 0.0f, 0.0f };
	end = { -kGridHalfWidth * kSubdivision / 2.0f, 0.0f, 0.0f };
	screenStart = Transform(start, viewProjectionMatrix);
	screenEnd = Transform(end, viewProjectionMatrix);
	screenStart = Transform(screenStart, viewportMatrix);
	screenEnd = Transform(screenEnd, viewportMatrix);
	Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
		static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), 0x000000FF);
}

// Sphere構造体
struct Sphere
{
	Vector3 center;	//!< 中心座標
	float radius;	//!< 半径
};

// Sphereを表示する関数
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color)
{
	const uint32_t kLatSubdivision = 10;	// 緯度方向の分割数
	const uint32_t kLonSubdivision = 20;	// 経度方向の分割数

	const float kLonEvery = static_cast<float>(M_PI) / kLatSubdivision;			// 経度分割1つ分の角度
	const float kLatEvery = 2.0f * static_cast<float>(M_PI) / kLonSubdivision;		// 緯度分割1つ分の角度
	float lat, lon;						// 緯度、経度
	Vector3 a, b, c;					// 描画用座標	
	Vector3 screenA, screenB, screenC;
	// 緯度の方向に分割　-π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kLatSubdivision; ++latIndex)
	{
		lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * latIndex;	// 現在の緯度
		// 経度の方向に分割　0 ～ 2π
		for (uint32_t lonIndex = 0; lonIndex < kLonSubdivision; ++lonIndex)
		{
			lon = lonIndex * kLonEvery;						// 現在の経度
			// World座標系でのa、b、cを求める
			a = {
				sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon),
				sphere.center.y + sphere.radius * std::sin(lat),
				sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon)
			};

			b = {
				sphere.center.x + sphere.radius * std::cos(lat + kLatEvery) * std::cos(lon),
				sphere.center.y + sphere.radius * std::sin(lat + kLatEvery),
				sphere.center.z + sphere.radius * std::cos(lat + kLatEvery) * std::sin(lon)
			};

			c = {
				sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon + kLonEvery),
				sphere.center.y + sphere.radius * std::sin(lat),
				sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon + kLonEvery)
			};

			// a、b、cをScreen座標まで変換
			screenA = Transform(a, viewProjectionMatrix);
			screenA = Transform(screenA, viewportMatrix);
			screenB = Transform(b, viewProjectionMatrix);
			screenB = Transform(screenB, viewportMatrix);
			screenC = Transform(c, viewProjectionMatrix);
			screenC = Transform(screenC, viewportMatrix);
			// ab、bcで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y),
				static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y),
				static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
		}
	}
}

// 直線
struct Line
{
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

// 半直線
struct Ray
{
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

// 線分
struct Segment
{
	Vector3 origin;	//!< 始点
	Vector3 diff;	//!< 終点への差分ベクトル
};

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	// a・b / ||b||²
	float t = Dot(v1, v2) / powf(Length(v2), 2);

	return
	{
		t * v2.x,
		t * v2.y,
		t * v2.z,
	};
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{

	Vector3 projBA = Project(Subtract(point, segment.origin), segment.diff);

	return Add(segment.origin, projBA);
}

// 球の当たり判定を求める関数
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	// 2つの球の中心点間の距離を求める
	float distance = Length(Subtract(sphere2.center, sphere1.center));
	// 半径の合計よりも短ければ衝突
	if (distance <= sphere1.radius + sphere2.radius)
	{
		return true;
	} else
	{
		return false;
	}
}

Matrix4x4 MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zAxis = Normalize(Subtract(target, eye));   // 前方向
	Vector3 xAxis = Normalize(Cross(up, zAxis));         // 右方向
	Vector3 yAxis = Cross(zAxis, xAxis);                 // 上方向

	Matrix4x4 view{};
	view.m[0][0] = xAxis.x; view.m[0][1] = yAxis.x; view.m[0][2] = zAxis.x; view.m[0][3] = 0.0f;
	view.m[1][0] = xAxis.y; view.m[1][1] = yAxis.y; view.m[1][2] = zAxis.y; view.m[1][3] = 0.0f;
	view.m[2][0] = xAxis.z; view.m[2][1] = yAxis.z; view.m[2][2] = zAxis.z; view.m[2][3] = 0.0f;
	view.m[3][0] = -Dot(xAxis, eye); view.m[3][1] = -Dot(yAxis, eye); view.m[3][2] = -Dot(zAxis, eye); view.m[3][3] = 1.0f;
	return view;
}

struct Plane
{
	Vector3 normal;	//!< 法線
	float distance;	//!< 距離
};

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	// 平面と球の中心点間の距離を求める
	float distance = Dot(plane.normal, sphere.center) - plane.distance;
	// 半径よりも短ければ衝突
	if (std::abs(distance) <= sphere.radius)
	{
		return true;
	} else
	{
		return false;
	}
}

/// <summary>
/// 直行する任意軸
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	//Vector3 center = Multiply(plane.distance, plane.normal);	// 1
	//Vector3 perpendiculars[4];
	//perpendiculars[0] = Normalize(Perpendicular(plane.normal));	// 2
	//perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };	// 3
	//perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);	// 4
	//perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };	// 5
	//// 6
	//Vector3 points[4];
	//for (int32_t index = 0; index < 4; ++index)
	//{
	//	Vector3 extend = Multiply(2.0f, perpendiculars[index]);
	//	Vector3 point = Add(center, extend);
	//	points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);

	//	
	//	switch (index)
	//	{
	//		case 0:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, RED);
	//			break;

	//		case 1:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, BLUE);
	//			break;

	//		case 2:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, GREEN);
	//			break;

	//		case 3:
	//			DrawSphere({ point, 0.03f }, viewProjectionMatrix, viewportMatrix, BLACK);
	//			break;
	//	}
	//}

	// 中心から ±u ±v を組み合わせた４点を取る
	Vector3 N = Normalize(plane.normal);              // 法線はまず正規化
	Vector3 U = Normalize(Perpendicular(N));          // 平面内の１方向
	Vector3 V = Normalize(Cross(N, U));               // U と直交するもう１方向

	float halfSize = 2.0f;
	Vector3 center = Multiply(plane.distance, N);      // 正規化済み N と distance で中心
	Vector3 point[4] =
	{
		Add(Add(center, Multiply(halfSize, U)), Multiply(halfSize, V)),  // +u +v
		Add(Add(center, Multiply(-halfSize, U)), Multiply(halfSize, V)),  // -u +v
		Add(Add(center, Multiply(-halfSize, U)), Multiply(-halfSize, V)),  // -u -v
		Add(Add(center, Multiply(halfSize, U)), Multiply(-halfSize, V))   // +u -v
	};

	DrawSphere({ point[0], 0.03f }, viewProjectionMatrix, viewportMatrix, RED);
	DrawSphere({ point[1], 0.03f }, viewProjectionMatrix, viewportMatrix, BLUE);
	DrawSphere({ point[2], 0.03f }, viewProjectionMatrix, viewportMatrix, GREEN);
	DrawSphere({ point[3], 0.03f }, viewProjectionMatrix, viewportMatrix, BLACK);

	Vector3 points[4];


	for (int32_t index = 0; index < 4; ++index)
	{
		points[index] = Transform(Transform(point[index], viewProjectionMatrix), viewportMatrix);
	}

	for (int32_t index = 0; index < 4; ++index)
	{
		Novice::DrawLine(
			static_cast<int>(points[index].x), static_cast<int>(points[index].y),
			static_cast<int>(points[(index + 1) % 4].x), static_cast<int>(points[(index + 1) % 4].y), color);

	}
}

bool IsCollision(const Plane& plane, const Line& line)
{
	// 法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);
	// 内積が0であれば平行
	if (dot == 0.0f)
	{
		return false;
	}

	// 平行でなければ直線は衝突する
	return true;
}

bool IsCollision(const Plane& plane, const Ray& ray)
{
	// 法線と線の内積を求める
	float dot = Dot(plane.normal, ray.diff);
	// 内積が0であれば平行
	if (dot == 0.0f)
	{
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;

	// tが0未満であれば半直線は衝突しない
	if (t < 0.0f)
	{
		return false;
	}

	// tが0以上であれば半直線は衝突する
	return true;

}

// 線分と平面の衝突判定を求める関数
bool IsCollision(const Plane& plane, const Segment& segment)
{
	// 法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 内積が0であれば平行
	if (dot == 0.0f)
	{
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// tが0未満または1より大きければ線分は衝突しない
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	// tが0以上1以下であれば線分は衝突する
	return true;

}

struct Triangle
{
	Vector3 vertices[3];	//!< 頂点
};

bool IsCollision(const Triangle& triangle, const Segment& segment)
{
	// 三角形の頂点から平面を求める
	Plane plane;
	plane.normal = Normalize(Cross(Subtract(triangle.vertices[1], triangle.vertices[0]),
		Subtract(triangle.vertices[2], triangle.vertices[1])));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	// 線分と平面の衝突判定を求める

	// 法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 内積が0であれば平行
	if (dot == 0.0f)
	{
		return false;
	}

	// tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// tが0未満または1より大きければ線分は衝突しない
	if (t < 0.0f || t > 1.0f)
	{
		return false;
	}

	// 衝突している場合、三角形の内側かどうかを判定する
	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));	// 衝突点



	// 各辺を結んだベクトルと、頂点と衝突点を結んだベクトルのクロス積を求める
	Vector3 cross01 = Cross(Subtract(triangle.vertices[1], triangle.vertices[0]),
		Subtract(p, triangle.vertices[1]));
	Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]),
		Subtract(p, triangle.vertices[2]));
	Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]),
		Subtract(p, triangle.vertices[0]));

	// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, plane.normal) >= 0.0f &&
		Dot(cross12, plane.normal) >= 0.0f &&
		Dot(cross20, plane.normal) >= 0.0f)
	{
		return true;
	}

	return false;
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenVertices[3];
	for (int32_t index = 0; index < 3; ++index)
	{
		screenVertices[index] = Transform(Transform(triangle.vertices[index], viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawTriangle(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		color, kFillModeWireFrame);
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y),
		static_cast<int>(end.x), static_cast<int>(end.y), color);
}

struct AABB
{
	Vector3 min;	//!< 最小点
	Vector3 max;	//!< 最大点
};

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
	{
		return true; // 衝突している
	}

	return false; // 衝突していない
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 vertices[8];
	// AABBの8頂点を計算
	vertices[0] = { aabb.min.x, aabb.min.y, aabb.min.z };
	vertices[1] = { aabb.max.x, aabb.min.y, aabb.min.z };
	vertices[2] = { aabb.max.x, aabb.max.y, aabb.min.z };
	vertices[3] = { aabb.min.x, aabb.max.y, aabb.min.z };
	vertices[4] = { aabb.min.x, aabb.min.y, aabb.max.z };
	vertices[5] = { aabb.max.x, aabb.min.y, aabb.max.z };
	vertices[6] = { aabb.max.x, aabb.max.y, aabb.max.z };
	vertices[7] = { aabb.min.x, aabb.max.y, aabb.max.z };
	Vector3 screenVertices[8];
	for (int32_t index = 0; index < 8; ++index)
	{
		screenVertices[index] = Transform(Transform(vertices[index], viewProjectionMatrix), viewportMatrix);
	}
	Novice::DrawLine(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y), color);

	Novice::DrawLine(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y), color);

	Novice::DrawLine(
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y), color);

	Novice::DrawLine(
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y), color);

}

bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	float loX = min(aabb.min.x, aabb.max.x);
	float hiX = max(aabb.min.x, aabb.max.x);
	float loY = min(aabb.min.y, aabb.max.y);
	float hiY = max(aabb.min.y, aabb.max.y);
	float loZ = min(aabb.min.z, aabb.max.z);
	float hiZ = max(aabb.min.z, aabb.max.z);


	Vector3 closestPoint
	{
		std::clamp(sphere.center.x, loX, hiX),
		std::clamp(sphere.center.y, loY, hiY),
		std::clamp(sphere.center.z, loZ, hiZ)
	};
	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	// 半径よりも短ければ衝突
	if (distance <= sphere.radius)
	{
		return true; // 衝突している
	}

	return false; // 衝突していない
}

bool IsCollision(const AABB& aabb, const Segment& segment)
{
	float txmin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float txmax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tymin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tymax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tzmin = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tzmax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	float tNearX = min(txmin, txmax);
	float tFarX = max(txmin, txmax);
	float tNearY = min(tymin, tymax);
	float tFarY = max(tymin, tymax);
	float tNearZ = min(tzmin, tzmax);
	float tFarZ = max(tzmin, tzmax);

	// AABBとの衝突点（貫通点）のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点（貫通点）のtが大きい方
	float tmax = min(min(tFarX, tFarY), tFarZ);
	if (tmin > tmax)
	{
		return false;
	}

	if (tmax < 0.0f || tmin > 1.0f)
	{
		return false;
	}

	return true;
}

struct OBB
{
	Vector3 center;	//!< 中心点
	Vector3 orientations[3];	//!< 座標軸。正規化・直行必須
	Vector3 size;	//!< 座標軸方向の長さの半分。中心点から面までの距離
};

bool IsCollision(const OBB& obb, const Sphere& sphere)
{
	Matrix4x4 obbWorldMatrix =
	{
		obb.orientations[0].x, obb.orientations[1].x, obb.orientations[2].x, 0.0f,
		obb.orientations[0].y, obb.orientations[1].y, obb.orientations[2].y, 0.0f,
		obb.orientations[0].z, obb.orientations[1].z, obb.orientations[2].z, 0.0f,
		obb.center.x,		   obb.center.y,		  obb.center.z,			 1.0f
	};

	Vector3 centerInOBBLocalSpace = Transform(sphere.center, Inverse(obbWorldMatrix));

	AABB aabbOBBLocal
	{
		.min = { -obb.size.x, -obb.size.y, -obb.size.z },
		.max = obb.size,
	};

	Sphere sphereOBBLocal
	{
		.center = centerInOBBLocalSpace,
		.radius = sphere.radius
	};

	return IsCollision(aabbOBBLocal, sphereOBBLocal);

}

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Matrix4x4 obbWorldMatrix =
	{
		obb.orientations[0].x, obb.orientations[1].x, obb.orientations[2].x, 0.0f,
		obb.orientations[0].y, obb.orientations[1].y, obb.orientations[2].y, 0.0f,
		obb.orientations[0].z, obb.orientations[1].z, obb.orientations[2].z, 0.0f,
		obb.center.x,		   obb.center.y,		  obb.center.z,			 1.0f
	};



	AABB aabbOBBLocal
	{
		.min = { -obb.size.x, -obb.size.y, -obb.size.z },
		.max = obb.size,
	};

	Vector3 vertices[8];
	// AABBの8頂点を計算
	vertices[0] = { aabbOBBLocal.min.x, aabbOBBLocal.min.y, aabbOBBLocal.min.z };
	vertices[1] = { aabbOBBLocal.max.x, aabbOBBLocal.min.y, aabbOBBLocal.min.z };
	vertices[2] = { aabbOBBLocal.max.x, aabbOBBLocal.max.y, aabbOBBLocal.min.z };
	vertices[3] = { aabbOBBLocal.min.x, aabbOBBLocal.max.y, aabbOBBLocal.min.z };
	vertices[4] = { aabbOBBLocal.min.x, aabbOBBLocal.min.y, aabbOBBLocal.max.z };
	vertices[5] = { aabbOBBLocal.max.x, aabbOBBLocal.min.y, aabbOBBLocal.max.z };
	vertices[6] = { aabbOBBLocal.max.x, aabbOBBLocal.max.y, aabbOBBLocal.max.z };
	vertices[7] = { aabbOBBLocal.min.x, aabbOBBLocal.max.y, aabbOBBLocal.max.z };
	Vector3 screenVertices[8];
	for (int32_t index = 0; index < 8; ++index)
	{
		// OBBのワールド座標系に変換
		screenVertices[index] = Transform(Transform(Transform(vertices[index], obbWorldMatrix), viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y), color);

	Novice::DrawLine(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y), color);

	Novice::DrawLine(
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y), color);

	Novice::DrawLine(
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y), color);
	Novice::DrawLine(
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y), color);

}

bool IsCollision(const OBB& obb, const Segment& segment)
{
	Matrix4x4 obbWorldMatrix =
	{
		obb.orientations[0].x, obb.orientations[1].x, obb.orientations[2].x, 0.0f,
		obb.orientations[0].y, obb.orientations[1].y, obb.orientations[2].y, 0.0f,
		obb.orientations[0].z, obb.orientations[1].z, obb.orientations[2].z, 0.0f,
		obb.center.x,		   obb.center.y,		  obb.center.z,			 1.0f
	};

	Vector3 localOrigin = Transform(segment.origin, Inverse(obbWorldMatrix));
	Vector3 localEnd = Transform(Add(segment.origin, segment.diff), Inverse(obbWorldMatrix));

	AABB localAABB
	{
		.min = { -obb.size.x, -obb.size.y, -obb.size.z },
		.max = obb.size,
	};

	Segment localSegment
	{
		.origin = localOrigin,
		.diff = Subtract(localEnd, localOrigin)
	};

	return IsCollision(localAABB, localSegment);

}

bool IsCollision(const OBB(&obb)[2])
{
	Vector3 axis[15]
	{
		obb[0].orientations[0],	// OBB1のX軸
		obb[0].orientations[1],	// OBB1のY軸
		obb[0].orientations[2],	// OBB1のZ軸
		obb[1].orientations[0],	// OBB2のX軸
		obb[1].orientations[1],	// OBB2のY軸
		obb[1].orientations[2],	// OBB2のZ軸
		/*Cross(obb[0].orientations[0], obb[1].orientations[0]),
		Cross(obb[0].orientations[0], obb[1].orientations[1]),
		Cross(obb[0].orientations[0], obb[1].orientations[2]),
		Cross(obb[0].orientations[1], obb[1].orientations[0]),
		Cross(obb[0].orientations[1], obb[1].orientations[1]),
		Cross(obb[0].orientations[1], obb[1].orientations[2]),
		Cross(obb[0].orientations[2], obb[1].orientations[0]),
		Cross(obb[0].orientations[2], obb[1].orientations[1]),
		Cross(obb[0].orientations[2], obb[1].orientations[2]),*/
	};



	// OBB1とOBB2の各軸の外積を求める
	for (int32_t i = 0; i < 3; ++i)
	{
		for (int32_t j = 0; j < 3; ++j)
		{
			auto v = Cross(obb[0].orientations[i], obb[1].orientations[j]);
			if (Length(v) < 1e-6f)
			{
				continue;	// 長さが0に近い場合は無視
			}
			axis[6 + i * 3 + j] = Normalize(v);
		}
	}

	Vector3 vertices[2][8];
	const size_t projSize = 8;

	// OBBの各頂点を求める
	for (int32_t index = 0; index < 2; index++)
	{
		Matrix4x4 obbWorldMatrix =
		{
			obb[index].orientations[0].x, obb[index].orientations[1].x, obb[index].orientations[2].x, 0.0f,
			obb[index].orientations[0].y, obb[index].orientations[1].y, obb[index].orientations[2].y, 0.0f,
			obb[index].orientations[0].z, obb[index].orientations[1].z, obb[index].orientations[2].z, 0.0f,
			obb[index].center.x,		  obb[index].center.y,		    obb[index].center.z,		  1.0f
		};

		AABB aabbOBBLocal
		{
			.min = { -obb[index].size.x, -obb[index].size.y, -obb[index].size.z },
			.max = obb[index].size,
		};

		Vector3 verticesAABB[8];
		// AABBの8頂点を計算
		verticesAABB[0] = { aabbOBBLocal.min.x, aabbOBBLocal.min.y, aabbOBBLocal.min.z };
		verticesAABB[1] = { aabbOBBLocal.max.x, aabbOBBLocal.min.y, aabbOBBLocal.min.z };
		verticesAABB[2] = { aabbOBBLocal.max.x, aabbOBBLocal.max.y, aabbOBBLocal.min.z };
		verticesAABB[3] = { aabbOBBLocal.min.x, aabbOBBLocal.max.y, aabbOBBLocal.min.z };
		verticesAABB[4] = { aabbOBBLocal.min.x, aabbOBBLocal.min.y, aabbOBBLocal.max.z };
		verticesAABB[5] = { aabbOBBLocal.max.x, aabbOBBLocal.min.y, aabbOBBLocal.max.z };
		verticesAABB[6] = { aabbOBBLocal.max.x, aabbOBBLocal.max.y, aabbOBBLocal.max.z };
		verticesAABB[7] = { aabbOBBLocal.min.x, aabbOBBLocal.max.y, aabbOBBLocal.max.z };

		for (int32_t vertexNum = 0; vertexNum < 8; ++vertexNum)
		{
			// OBBのワールド座標系に変換
			vertices[index][vertexNum] = Transform(verticesAABB[vertexNum], obbWorldMatrix);
		}
	}

	for (int32_t axisNum = 0; axisNum < 15; ++axisNum)
	{
		float L[2], min[2], max[2];

		std::vector<float> projection(projSize);	// 投影結果（最大最小を求めやすくするためstd::vector）
		// 各OBBの頂点を投影する
		for (int32_t index = 0; index < 2; ++index)
		{
			for (int32_t vertexNum = 0; vertexNum < 8; ++vertexNum)
			{
				projection[vertexNum] = Dot(vertices[index][vertexNum], axis[axisNum]);
			}

			// 最小値と最大値を求める
			auto it_max = std::max_element(projection.begin(), projection.end());
			max[index] = (it_max != projection.end()) ? *it_max : 0.0f;
			auto it_min = std::min_element(projection.begin(), projection.end());
			min[index] = (it_min != projection.end()) ? *it_min : 0.0f;
			L[index] = max[index] - min[index];
		}

		float sumSpan = L[0] + L[1];	// 2つのOBBの投影の長さの和
		float longSpan = (std::max)(max[0], max[1]) - (std::min)(min[0], min[1]);	// 2つのOBBの投影の長さの差)
		if (sumSpan <= longSpan)
		{
			// 投影の長さの和が投影の長さの差よりも大きい場合、衝突していない
			return false;
		}
	}

	// すべての軸で衝突している場合、衝突している
	return true;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return
	{
		v2.x * t + v1.x * (1.0f - t),
		v2.y * t + v1.y * (1.0f - t),
		v2.z * t + v1.z * (1.0f - t)
	};
}

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int32_t color)
{
	for (uint32_t i = 0; i < 32; ++i)
	{
		float t = static_cast<float>(i) / 32.0f;
		Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, t);
		Vector3 p1p2 = Lerp(controlPoint1, controlPoint2, t);
		Vector3 p = Lerp(p0p1, p1p2, t);
		Vector3 nextP0P1 = Lerp(controlPoint0, controlPoint1, t + 1.0f / 32.0f);
		Vector3 nextP1P2 = Lerp(controlPoint1, controlPoint2, t + 1.0f / 32.0f);
		Vector3 nextP = Lerp(nextP0P1, nextP1P2, t + 1.0f / 32.0f);
		Vector3 screenP = Transform(Transform(p, viewProjectionMatrix), viewportMatrix);
		Vector3 screenNextP = Transform(Transform(nextP, viewProjectionMatrix), viewportMatrix);
		Novice::DrawLine(static_cast<int>(screenP.x), static_cast<int>(screenP.y),
			static_cast<int>(screenNextP.x), static_cast<int>(screenNextP.y), color);
	}

	Sphere controlSphere[3];
	controlSphere[0] = { controlPoint0, 0.01f };
	controlSphere[1] = { controlPoint1, 0.01f };
	controlSphere[2] = { controlPoint2, 0.01f };

	for (uint32_t i = 0; i < 3; ++i)
	{
		DrawSphere(controlSphere[i], viewProjectionMatrix, viewportMatrix, 0x000000FF);
	}

}

void DrawLine(const Vector3& start, const Vector3& end,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenStart = Transform(Transform(start, viewProjectionMatrix), viewportMatrix);
	Vector3 screenEnd = Transform(Transform(end, viewProjectionMatrix), viewportMatrix);
	Novice::DrawLine(static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
		static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y), color);
}

struct Spring
{
	// アンカー。固定された端の位置
	Vector3 anchor;
	float naturalLength;		// 自然長
	float stiffness;			// 剛性。ばね定数k
	float dampingCoefficient;	// 減衰係数
};

struct Ball
{
	Vector3 position;		// ボールの位置
	Vector3 velocity;		// ボールの速度
	Vector3 aceleration;	// ボールの加速度
	float mass;				// ボールの質量
	float radius;			// ボールの半径
	unsigned int color;		// ボールの色
};

struct  Pendulum
{
	Vector3 anchor;		// アンカーポイント。固定された端の位置
	float length;		// 紐の長さ
	float angle;		// 現在の角度
	float angularVelocity;	// 角速度w
	float angularAcceleration;	// 角加速度
};

struct ConicalPendulum
{
	Vector3 anchor;			// アンカーポイント。固定された端の位置
	float length;			// 紐の長さ
	float halfApexAngle;	// 円錐の聴覚の半分
	float angle;			// 現在の角度
	float angularVelocity;	// 角速度w
};

Vector3 Reflect(const Vector3& input, const Vector3& normal)
{
	return input - 2.0f * Dot(input, normal) * normal;
}

struct Capsule
{
	Segment segment;
	float radius;
};

bool IsCollision(const Capsule& capsel, const Plane& plane)
{
	// 線分始点の平面からの距離
	float distOrigin = Dot(plane.normal, capsel.segment.origin) - plane.distance;
	// 線分ベクトル
	Vector3 diff = capsel.segment.diff;
	// 線分ベクトルと法線の内積
	float denom = Dot(plane.normal, diff);

	if (denom == 0.0f)
	{
		// 線分が平面と平行 → 始点だけ見て判定
		return std::abs(distOrigin) <= capsel.radius;
	}

	// 平面に最も近づく位置 t を求め、線分上に制限
	float t = -distOrigin / denom;
	t = std::clamp(t, 0.0f, 1.0f);

	// 線分上の最近接点
	Vector3 closestPoint = capsel.segment.origin + diff * t;
	// 最近接点の平面からの距離
	float distClosest = Dot(plane.normal, closestPoint) - plane.distance;

	return std::abs(distClosest) <= capsel.radius;
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	/*Matrix4x4 S = MakeScaleMatrix({std::cosf(angle), std::cosf(angle), std::cosf(angle)});

	Matrix4x4 P = Multiply(1.0f - std::cosf(angle), 
		{
			axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, 0.0f,
			axis.y * axis.x, axis.y * axis.y, axis.y * axis.z, 0.0f,
			axis.z * axis.x, axis.z * axis.y, axis.z * axis.z, 0.0f,
			0.0f,			 0.0f,			  0.0f,			   0.0f
		}
	);

	Matrix4x4 C = Multiply(-std::sinf(angle), 
		{
			0.0f,  -axis.z, axis.y, 0.0f,
			axis.z, 0.0f,  -axis.x, 0.0f,
		   -axis.y, axis.x, 0.0f,	0.0f,
		    0.0f,	0.0f,   0.0f,   0.0f
		}
	);

	return Add(C, Add(S, P));*/

	return 
	{
		axis.x * axis.x * (1.0f - std::cosf(angle)) + std::cosf(angle), 
		axis.x * axis.y * (1.0f - std::cosf(angle)) + axis.z * std::sinf(angle),
		axis.x * axis.z * (1.0f - std::cosf(angle)) - axis.y * std::sinf(angle),
		0.0f,

		axis.y * axis.x * (1.0f - std::cosf(angle)) - axis.z * std::sinf(angle),
		axis.y * axis.y * (1.0f - std::cosf(angle)) + std::cosf(angle),
		axis.y * axis.z * (1.0f - std::cosf(angle)) + axis.x * std::sinf(angle),
		0.0f,

		axis.z * axis.x * (1.0f - std::cosf(angle)) + axis.y * std::sinf(angle),
		axis.z * axis.y * (1.0f - std::cosf(angle)) - axis.x * std::sinf(angle),
		axis.z * axis.z * (1.0f - std::cosf(angle)) + std::cosf(angle),
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{

	float cos = Dot(from, to);
	float sin = Length(Cross(from, to));

	if (std::abs(cos + 1.0f) < 1e-6f) 
	{
		// 完全逆方向の場合
		Vector3 axis = Perpendicular(from);
		axis = Normalize(axis);
		return MakeRotateAxisAngle(axis, static_cast<float>(M_PI));
	}
	if (std::abs(cos - 1.0f) < 1e-6f) 
	{
		// 完全同方向の場合
		return MakeIdentity4x4();
	}

	Vector3 n = Normalize(Cross(from, to));

	return 
	{
		n.x * n.x * (1.0f - cos) + cos,
		n.x * n.y * (1.0f - cos) + n.z * sin,
		n.x * n.z * (1.0f - cos) - n.y * sin,
		0.0f,

		n.y * n.x * (1.0f - cos) - n.z * sin,
		n.y * n.y * (1.0f - cos) + cos,
		n.y * n.z * (1.0f - cos) + n.x * sin,
		0.0f,
		
		n.z * n.x * (1.0f - cos) + n.y * sin,
		n.z * n.y * (1.0f - cos) - n.x * sin,
		n.z * n.z * (1.0f - cos) + cos,
		0.0f,
		
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

struct Quaternion
{
	float x, y, z, w;
};

void QuaternionScreenPrintf(int x, int y, const Quaternion& quaternion, const char* label)
{
	Novice::ScreenPrintf(x + 0 * kColumnWidth, y, "%6.02f", quaternion.x);
	Novice::ScreenPrintf(x + 1 * kColumnWidth, y, "%6.02f", quaternion.y);
	Novice::ScreenPrintf(x + 2 * kColumnWidth, y, "%6.02f", quaternion.z);
	Novice::ScreenPrintf(x + 3 * kColumnWidth, y, "%6.02f", quaternion.w);
	Novice::ScreenPrintf(x + 5 * kColumnWidth, y, ": %s", label);
}

// Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Vector3 cross = Cross(Vector3{ lhs.x, lhs.y, lhs.z }, Vector3{ rhs.x, rhs.y, rhs.z });
	return 
	{ 
		cross.x + rhs.w * lhs.x + lhs.w * rhs.x,
		cross.y + rhs.w * lhs.y + lhs.w * rhs.y,
		cross.z + rhs.w * lhs.z + lhs.w * rhs.z,
		lhs.w * rhs.w - Dot(Vector3{ lhs.x, lhs.y, lhs.z }, Vector3{ rhs.x, rhs.y, rhs.z }),
	};
}

// 単位Quaternionを返す
Quaternion IdentityQuaternion()
{
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

// 共役Quaternionを返す
Quaternion Conjugate(const Quaternion& quaternion)
{
	return
	{
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	};
}

// Quaternionのnormを返す
float Norm(const Quaternion& quaternion)
{
	return std::sqrtf(
		std::powf(quaternion.x, 2) + 
		std::powf(quaternion.y, 2) + 
		std::powf(quaternion.z, 2) + 
		std::powf(quaternion.w, 2));
}

// 正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion)
{
	float norm = Norm(quaternion);
	return 
	{ 
		quaternion.x / norm, 
		quaternion.y / norm, 
		quaternion.z / norm, 
		quaternion.w / norm,
	};
}

// 逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion)
{
	Quaternion conj = Conjugate(quaternion);
	float norm = Norm(quaternion);

	return
	{
		conj.x / std::powf(norm, 2),
		conj.y / std::powf(norm, 2),
		conj.z / std::powf(norm, 2),
		conj.w / std::powf(norm, 2),
	};	
}

// 任意軸回転を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	return 
	{
		axis.x * std::sinf(angle / 2.0f),
		axis.y * std::sinf(angle / 2.0f),	
		axis.z * std::sinf(angle / 2.0f),
		std::cosf(angle / 2.0f)
	};
}

// ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion v = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConj = Conjugate(quaternion);
	Quaternion rotatedV = Multiply(Multiply(quaternion, v), qConj);
	return { rotatedV.x, rotatedV.y, rotatedV.z };
}

// Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	float xx = quaternion.x * quaternion.x;
	float yy = quaternion.y * quaternion.y;
	float zz = quaternion.z * quaternion.z;
	float ww = quaternion.w * quaternion.w;

	return 
	{
		ww + xx - yy -zz,
		2.0f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z),
		2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y),
		0.0f,

		2.0f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z),
		ww - xx + yy - zz,
		2.0f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x),
		0.0f,

		2.0f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y),
		2.0f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x),
		ww - xx - yy + zz,
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*Sphere sphere;
	sphere.center = {1.0f, 0.0f, 0.0f};
	sphere.radius = 0.05f;

	Vector3 origin = {0.0f, 0.0f, 0.0f};
	float radius = 0.8f;

	float angularVelocity = 3.14f;
	float angle = 0.0f;*/

	/*Pendulum pendulum;
	pendulum.anchor = { 0.0f, 1.0f, 0.0f };
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = 0.0f;*/

	/*Vector3 p;

	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = { 0.0f, 1.0f, 0.0f };
	conicalPendulum.length = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f;
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;*/

	//Ball ball;
	//ball.position = { 0.0f, 0.0f, 0.0f };
	//ball.velocity = { 0.0f, 0.0f, 0.0f };
	//ball.aceleration = { 0.0f, 0.0f, 0.0f };
	//ball.mass = 2.0f;
	//ball.radius = 0.05f;
	//ball.color = WHITE;

	//Vector3 ballStartPosition = { -0.8f, 1.2f, 0.8f };
	//ball.position = ballStartPosition;

	//Plane plane;
	//plane.normal = Normalize({ -0.2f, -0.9f, -0.3f });
	//plane.distance = 0.0f;

	//Segment moveSegment;
	//moveSegment.origin = ball.position;
	//moveSegment.diff = { 0.0f, 0.0f, 0.0f };

	//float e = 0.8f; // 反発係数

	//bool start = false;

	/*Vector3 from0 = Normalize(Vector3{ 1.0f, 0.7f, 0.5f });
	Vector3 to0 = -from0;
	Vector3 from1 = Normalize(Vector3{ -0.6f, 0.9f, 0.2f });
	Vector3 to1 = Normalize(Vector3{ 0.4f, 0.7f, -0.5f });

	Quaternion q1 = {2.0f, 3.0f, 4.0f, 1.0f};
	Quaternion q2 = {1.0f, 3.0f, 5.0f, 2.0f};
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);*/

	Quaternion rotation = MakeRotateAxisAngleQuaternion(
		Normalize(Vector3{ 1.0f, 0.4f, -0.2f }), 0.45f);
	Vector3 pointY = { 2.1f, -0.9f,  1.3f };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);


	//float  deltaTime = 1.0f / 60.0f;

	//Vector3 cameraPos{ 0.0f, 0.0f, 0.0f };
	//Vector3 cameraSize{ 1.0f, 1.0f, 1.0f };
	//Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	//Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };

	///*Matrix4x4 cameraMatrix;*/
	//Matrix4x4 viewMatrix;
	//Matrix4x4 projectionMatrix;
	//Matrix4x4 viewportMatrix;

	//// main関数の直前あたりに追加

	//float cameraRadius = 6.0f;    // 原点からの距離
	//const float rotationSpeed = 0.005f; // ドラッグ速度の調整係数

	//float theta = 0.0f;
	//float phi = 0.0f;

	//// マウス座標を取得
	//int mouseX, mouseY;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*Novice::GetMousePosition(&mouseX, &mouseY);*/

		///
		/// ↓更新処理ここから
		///




		/*Matrix4x4 rotateMatrix0 = DirectionToDirection(
			Normalize(Vector3{ 1.0f, 0.0f, 0.0f }), Normalize(Vector3{ -1.0f, 0.0f, 0.0f }));
		Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
		Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);*/

		//if(start)
		//{ 
		//	angle += angularVelocity * deltaTime;
		//}

		//sphere.center.x = origin.x + std::cos(angle) * radius;
		//sphere.center.y = origin.y + std::sin(angle) * radius;
		//sphere.center.z = origin.z;

		//if (start)
		//{
		//	pendulum.angularAcceleration = 
		//		-(9.8f / pendulum.length) * std::sin(pendulum.angle);
		//	pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
		//	pendulum.angle += pendulum.angularVelocity * deltaTime;
		//}

		//// pは振り子の先端の位置。取り付けたいものを取り付ければ良い
		//p.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
		//p.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
		//p.z = pendulum.anchor.z;

		/*if (start)
		{
			conicalPendulum.angularVelocity =
				std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
		}

		float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;
		ball.position.x = conicalPendulum.anchor.x + radius * std::sin(conicalPendulum.angle);
		ball.position.y = conicalPendulum.anchor.y - height;
		ball.position.z = conicalPendulum.anchor.z + radius * std::cos(conicalPendulum.angle);*/

		//// フレーム開始時の位置を保存
		//Vector3 prevPosition = ball.position;

		//// 速度・位置を予測
		//ball.velocity += ball.aceleration * deltaTime;
		//Vector3 nextPosition = prevPosition + ball.velocity * deltaTime;

		//// 移動セグメントを作成
		//moveSegment = { prevPosition, Subtract(nextPosition, prevPosition) };

		//if (IsCollision(Capsule{ moveSegment, ball.radius }, plane))
		//{
		//	// カプセルの線分と平面の交点パラメータ t を求める（実装例は下記）
		//	float dist0 = Dot(plane.normal, prevPosition) - plane.distance;
		//	float denom = Dot(plane.normal, moveSegment.diff);
		//	// 法線方向に「面から半径分だけ離れた」位置で線分と交わる t
		//	float tHit = (ball.radius - dist0) / denom;

		//	if (tHit >= 0.0f && tHit <= 1.0f)
		//	{
		//		// 衝突時刻まで移動し、球の中心を設定
		//		Vector3 collisionCenter = Add(prevPosition, Multiply(tHit, moveSegment.diff));
		//		// ほんの少し法線方向へオフセットしてめり込み回避
		//		const float epsilon = 1e-4f;
		//		collisionCenter += plane.normal * epsilon;
		//		ball.position = collisionCenter;

		//		Vector3 reflected = Reflect(ball.velocity, plane.normal);
		//		Vector3 projectToNormal = Project(reflected, plane.normal);
		//		Vector3 movingDirection = reflected - projectToNormal;
		//		Vector3 reflectedVelocity = projectToNormal * e + movingDirection;

		//		// フレーム残り時間分だけ移動
		//		float remain = 1.0f - tHit;
		//		ball.position += reflectedVelocity * (deltaTime * remain);
		//		ball.velocity = reflectedVelocity;
		//	} else
		//	{
		//		// 衝突時刻が範囲外なら、衝突なしとみなす
		//		// 予測位置をそのまま使う
		//		ball.position = nextPosition;
		//	}
		//} else
		//{
		//	// 衝突なしなら予測位置をそのまま使う
		//	ball.position = nextPosition;
		//}

		//if (Novice::IsPressMouse(2) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
		//{
		//	// ウィンドウ上ではない領域でのドラッグ
		//	ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle, 0.0f);

		//	// カメラ角度に反映
		//	phi += delta.x * rotationSpeed;
		//	theta += delta.y * rotationSpeed;

		//	if (phi > static_cast<float>(M_PI))
		//	{
		//		phi = phi - 2.0f * static_cast<float>(M_PI);
		//	} else if (phi < -static_cast<float>(M_PI))
		//	{
		//		phi = phi + 2.0f * static_cast<float>(M_PI);
		//	}

		//	// ピッチを上下90°未満にクランプ
		//	const float limit = 0.4999f * static_cast<float>(M_PI); // 約85°
		//	theta = std::clamp(theta, -limit, limit);

		//	ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
		//}



		////cameraRotate.x = phi;
		////cameraRotate.y = theta;

		////cameraTranslate.x = cameraRadius * std::cos(theta) * std::sin(phi);
		////cameraTranslate.y = cameraRadius * std::sin(theta);
		////cameraTranslate.z = cameraRadius * std::cos(theta) * std::cos(phi);

		////ImGui::Begin("Window");
		////ImGui::DragFloat3("ballStartPosition", &ballStartPosition.x, 0.01f);
		////ImGui::DragFloat3("plane.normal", &plane.normal.x, 0.01f);
		////ImGui::DragFloat("plane.distance", &plane.distance, 0.01f);

		////if (ImGui::Button("Start"))
		////{
		////	start = true;
		////	ball.aceleration = { 0.0f, -9.8f, 0.0f };
		////}
		////if (ImGui::Button("Reset"))
		////{
		////	start = false;
		////	ball.velocity = { 0.0f, 0.0f, 0.0f };
		////	ball.aceleration = { 0.0f, 0.0f, 0.0f };
		////}
		////ImGui::End();

		////plane.normal = Normalize(plane.normal);

		////if (!start)
		////{
		////	ball.position = ballStartPosition;
		////}

		////// ビュー行列を生成
		////viewMatrix = MakeLookAtMatrix(cameraTranslate, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

		////projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);
		////// VPMatrixを作成
		////Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		////// ViewportMatrixを作成
		////viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth),
		////	static_cast<float>(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		

		QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, "   : rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, "   : rotateByMatrix");


		/*QuaternionScreenPrintf(0, 0, identity, "Identity");
		QuaternionScreenPrintf(0, kRowHeight * 1, conj, "Conjugate");
		QuaternionScreenPrintf(0, kRowHeight * 2, inv, "Inverse");
		QuaternionScreenPrintf(0, kRowHeight * 3, normal, "Normalize");
		QuaternionScreenPrintf(0, kRowHeight * 4, mul1, "Multiply(q1, q2)");
		QuaternionScreenPrintf(0, kRowHeight * 5, mul2, "Multiply(q2, q1)");
		Novice::ScreenPrintf(0, kRowHeight * 6, "%6.02f", norm);
		Novice::ScreenPrintf(kColumnWidth * 5, kRowHeight * 6, ": Norm");*/




		/*MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");*/

		/*DrawGrid(viewProjectionMatrix, viewportMatrix);

		DrawSphere(Sphere{ ball.position, ball.radius }, viewProjectionMatrix, viewportMatrix, WHITE);
		DrawSegment(moveSegment, viewProjectionMatrix, viewportMatrix, 0xFF0000FF);
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, 0x00FF00FF);*/

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
