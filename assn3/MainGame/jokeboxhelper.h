/*----------------------------------------------------------*
 *															*
 *					Jokebox Helper v1.01					*
 *															*
 * 			이광무에 의해 작성된 Jokebox Helper는			*
 *		크리에이티브 커먼즈 저작자표시-동일조건변경허락		*
 * 		3.0 Unported 라이선스에 따라 이용할 수 있습니다.	*
 *															*
 *					ORANGEYELLOW, 2013.						*
 *					iss300@postech.ac.kr					*
 *															*
 *----------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include <math.h>

typedef struct Rectangle
{
	float x;
	float y;
	float width;
	float height;
}RECTANGLE;

typedef struct Vector2
{
	float x;
	float y;
}VECTOR2;


RECTANGLE rect_create(float x, float y, float width, float height);	// RECTANGLE 구조체를 생성
int rect_contains(RECTANGLE rect, float x, float y);				// 사각형이 특정 위치를 포함하는지 여부를 반환
int rect_intersects(RECTANGLE rect1, RECTANGLE rect2);				// 사각형들이 서로 교차하는지 여부를 반환

VECTOR2 vec2_create(float x, float y);						// VECTOR2 구조체를 생성
VECTOR2 vec2_signflip(VECTOR2 vec);							// 벡터의 부호를 반전시킨 벡터를 반환
VECTOR2 vec2_add(VECTOR2 vec1, VECTOR2 vec2);				// 벡터들의 합을 반환
VECTOR2 vec2_subtract(VECTOR2 vec1, VECTOR2 vec2);			// 첫째 벡터에서 둘째 벡터를 뺀 벡터를 반환
VECTOR2 vec2_multiply(VECTOR2 vec, float multiplier);		// 벡터를 상수배한 것을 반환
float vec2_dot(VECTOR2 vec1, VECTOR2 vec2);					// 벡터들의 내적을 반환
VECTOR2 vec2_normalize(VECTOR2 vec);						// 벡터를 정규화(Normalize)한 결과를 반환
float vec2_length(VECTOR2 vec);								// 벡터의 길이를 반환


__inline RECTANGLE rect_create(float x, float y, float width, float height)
{
	RECTANGLE rect;

	rect.x = x;
	rect.y = y;
	rect.width = width;
	rect.height = height;

	return rect;
}

__inline int rect_contains(RECTANGLE rect, float x, float y)
{
	return (rect.x < x &&
		rect.y < y &&
		rect.x + rect.width > x &&
		rect.y + rect.height > y);
}

__inline int rect_intersects(RECTANGLE rect1, RECTANGLE rect2)
{
	return (fabs(rect1.x + rect1.width / 2 - rect2.x - rect2.width / 2) * 2 < (rect1.width + rect2.width) &&
		fabs(rect1.y + rect1.height / 2 - rect2.y - rect2.height / 2) * 2 < (rect1.height + rect2.height));
}

__inline VECTOR2 vec2_create(float x, float y)
{
	VECTOR2 vec;

	vec.x = x;
	vec.y = y;
	
	return vec;
}

__inline VECTOR2 vec2_signflip(VECTOR2 vec)
{
	vec.x = -vec.x;
	vec.y = -vec.y;

	return vec;
}

__inline VECTOR2 vec2_add(VECTOR2 vec1, VECTOR2 vec2)
{
	VECTOR2 result;

	result.x = vec1.x + vec2.x;
	result.y = vec1.y + vec2.y;
	
	return result;
}

__inline VECTOR2 vec2_subtract(VECTOR2 vec1, VECTOR2 vec2)
{
	VECTOR2 result;

	result.x = vec1.x - vec2.x;
	result.y = vec1.y - vec2.y;
	
	return result;
}

__inline VECTOR2 vec2_multiply(VECTOR2 vec, float multiplier)
{
	vec.x *= multiplier;
	vec.y *= multiplier;

	return vec;
}

__inline float vec2_dot(VECTOR2 vec1, VECTOR2 vec2)
{
	return (vec1.x * vec2.x + vec1.y + vec2.y);
}

__inline VECTOR2 vec2_normalize(VECTOR2 vec)
{
	float len = (float)sqrt(vec.x * vec.x + vec.y * vec.y);
	
	if (len > 0)
	{
		vec.x /= len;
		vec.y /= len;
	}

	return vec;
}

__inline float vec2_length(VECTOR2 vec)
{
	return (float)sqrt(vec.x * vec.x + vec.y * vec.y);
}

#ifdef __cplusplus
};
#endif


#ifdef __cplusplus
// C++ 사용자를 위한 Vector2 연산자 오버로딩

// Sign flip
__inline VECTOR2 operator-(const VECTOR2& vec)
{
	VECTOR2 res;
	res.x = -vec.x;
	res.y = -vec.y;

	return res;
}

// Add
__inline VECTOR2 operator+(const VECTOR2& vec1, const VECTOR2& vec2)
{
	VECTOR2 res;
	res.x = vec1.x + vec2.x;
	res.y = vec1.y + vec2.y;

	return res;
}

// Subtract
__inline VECTOR2 operator-(const VECTOR2& vec1, const VECTOR2& vec2)
{
	VECTOR2 res;
	res.x = vec1.x - vec2.x;
	res.y = vec1.y - vec2.y;
	
	return res;
}

// Multiply (const)
__inline VECTOR2 operator*(const VECTOR2& vec, const float multiplier)
{
	VECTOR2 res;
	res.x = vec.x * multiplier;
	res.y = vec.y * multiplier;

	return res;
}

__inline VECTOR2 operator*(const float multiplier, const VECTOR2& vec)
{
	VECTOR2 res;
	res.x = vec.x * multiplier;
	res.y = vec.y * multiplier;

	return res;
}
#endif