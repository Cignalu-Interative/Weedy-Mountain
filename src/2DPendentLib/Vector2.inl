#include "Vector2.h"
#pragma once

template<typename T>
template<typename U>
Vector2<T>::Vector2(const Vector2<U>& vector) :
    m_x(static_cast<T>(vector.GetX())),
    m_y(static_cast<T>(vector.GetY())) {}

template<typename T>
void Vector2<T>::SetX(T p_x)
{
    m_x = p_x;
}
template<typename T>
void Vector2<T>::SetY(T p_y)
{
    m_y = p_y;
}

template<typename T>
T& Vector2<T>::GetX()
{
    return m_x;
}
template<typename T>
T& Vector2<T>::GetY()
{
    return m_y;
}

template<typename T>
T Vector2<T>::GetX() const
{
    return m_x;
}
template<typename T>
T Vector2<T>::GetY() const
{
    return m_y;
}

template<typename T>
T& Vector2<T>::operator[](int p_index)
{
    return (p_index == 0) ? m_x : m_y;
}
template<typename T>
T Vector2<T>::operator[](int p_index) const
{
    return (p_index == 0) ? m_x : m_y;
}

template<typename T>
Vector2<T> Vector2<T>::operator + (const Vector2<T>& p_vector) const
{
    return Vector2<T>(m_x + p_vector.m_x, m_y + p_vector.m_y);
}
template<typename T>
Vector2<T>& Vector2<T>::operator + (T p_scalar)
{
    m_x += static_cast<T>(p_scalar);
    m_y += static_cast<T>(p_scalar);
	return *this;
}
template<typename T>
Vector2<T>& Vector2<T>::operator += (const Vector2<T>& p_vector)
{
    m_x += p_vector.m_x;
    m_y += p_vector.m_y;
    return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator - (const Vector2<T>& p_vector) const
{
    return Vector2<T>(m_x - p_vector.m_x, m_y - p_vector.m_y);
}
template<typename T>
Vector2<T>& Vector2<T>::operator - (T p_scalar)
{
    m_x -= static_cast<T>(p_scalar);
    m_y -= static_cast<T>(p_scalar);
	return *this;
}
template<typename T>
Vector2<T>& Vector2<T>::operator -= (const Vector2<T>& p_vector)
{
    m_x -= p_vector.m_x;
    m_y -= p_vector.m_y;
    return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator * (const Vector2<T>& p_vector) const
{
    return Vector2<T>(m_x * p_vector.m_x, m_y * p_vector.m_y);
}
template<typename T>
Vector2<T> Vector2<T>::operator * (T p_scalar) const
{
    return Vector2<T>(m_x * static_cast<T>(p_scalar), m_y * static_cast<T>(p_scalar));
}
template<typename T>
Vector2<T>& Vector2<T>::operator *= (T p_scalar)
{
    m_x *= static_cast<T>(p_scalar);
    m_y *= static_cast<T>(p_scalar);
    return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator / (T scalar) const
{
    T s = static_cast<T>(scalar);
    return Vector2<T>(m_x / s, m_y / s);
}
template<typename T>
Vector2<T>& Vector2<T>::operator /= (T scalar)
{
    T s = static_cast<T>(scalar);
    m_x /= s;
    m_y /= s;
    return *this;
}

template<typename T>
bool Vector2<T>::operator == (const Vector2<T>& p_vector) const
{
    return m_x == p_vector.m_x && m_y == p_vector.m_y;
}
template<typename T>
bool Vector2<T>::operator != (const Vector2<T>& p_vector) const
{
    return !(*this == p_vector);
}

template<typename T>
Vector2<T> Vector2<T>::operator - () const
{
    return Vector2<T>(-m_x, -m_y);
}

template<typename T>
bool Vector2<T>::operator < (const Vector2<T>& other) const
{
    T a = m_x * m_x + m_y * m_y;
    T b = other.m_x * other.m_x + other.m_y * other.m_y;
    return a < b;
}
template<typename T>
bool Vector2<T>::operator > (const Vector2<T>& other) const
{
    T a = m_x * m_x + m_y * m_y;
    T b = other.m_x * other.m_x + other.m_y * other.m_y;
    return a > b;
}
template<typename T>
bool Vector2<T>::operator <= (const Vector2<T>& other) const
{
    return !(*this > other);
}
template<typename T>
bool Vector2<T>::operator >= (const Vector2<T>& other) const
{
    return !(*this < other);
}