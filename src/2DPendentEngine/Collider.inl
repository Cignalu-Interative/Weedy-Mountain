#pragma once
#include <type_traits>

//template<typename T>
//const T& Collider::GetShapeCollider() const
//{
//    static_assert(std::is_same_v<T, Circle> || std::is_same_v<T, Rectangle>,
//        "T doit être Circle ou Rectangle !");
//
//    if constexpr (std::is_same_v<T, Circle>)
//    {
//        return m_circle;
//    }
//    else if constexpr (std::is_same_v<T, Rectangle>)
//    {
//        return m_rectangle;
//    }
//}
//
//template<typename T>
//T& Collider::GetShapeCollider()
//{
//    static_assert(std::is_same_v<T, Circle> || std::is_same_v<T, Rectangle>,
//        "T doit être Circle ou Rectangle !");
//
//    if constexpr (std::is_same_v<T, Circle>)
//    {
//        return m_circle;
//    }
//    else if constexpr (std::is_same_v<T, Rectangle>)
//    {
//        return m_rectangle;
//    }
//}