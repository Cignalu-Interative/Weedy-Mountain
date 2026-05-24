#pragma once
#include "Vector2.h"
#include "Utils.h"
#include <algorithm>
#include <cassert>
#include <utility>
#include <type_traits>

namespace Utils
{
	template<typename T>
	float Angle(const Vector2<T>& p_a, const Vector2<T>& p_b)
	{
		return std::atan2f(static_cast<float>(p_b.GetY() - p_a.GetY()), static_cast<float>(p_b.GetX() - p_a.GetX()));
	}

	template<typename T>
	Vector2<float> NormalizeVector(const Vector2<T>& p_vector)
	{
		float squareLength = p_vector.GetX() * p_vector.GetX() + p_vector.GetY() * p_vector.GetY();

		assert(squareLength > 0.0f && "Lib/Utils.inl/NormalizeVector: Normalisation de vecteur nul impossible");
		float vectorLength = std::sqrtf(squareLength);

		Vector2<float> normalizedVector = { p_vector.GetX() / vectorLength,
											p_vector.GetY() / vectorLength };

		return normalizedVector;
	}

	template<typename T>
	Vector2<T> Perpendicular(const Vector2<T>& p_v)
	{
		return Vector2<T>(-p_v.GetY(), p_v.GetX());
	}

	template<typename T>
	Vector2<T> Reflect(const Vector2<T>& p_incident, const Vector2<T>& p_normal)
	{
		float length = p_normal.GetX() * p_normal.GetX() + p_normal.GetY() * p_normal.GetY();

		assert(length > 0.0f && "Lib/Utils.inl/Reflect: Normale nulle dans Reflect");

		float d = static_cast<float>(p_incident.GetX()) * static_cast<float>(p_normal.GetX())
			+ static_cast<float>(p_incident.GetY()) * static_cast<float>(p_normal.GetY());

		return Vector2<T>(
			static_cast<T>(p_incident.GetX() - 2.0f * d * p_normal.GetX()),
			static_cast<T>(p_incident.GetY() - 2.0f * d * p_normal.GetY())
		);
	}

	template<typename T>
	float DistanceSquared(const Vector2<T>& p_a, const Vector2<T>& p_b)
	{
		Vector2<T> diff = p_b - p_a;

		float sq = static_cast<float>(diff.GetX() * diff.GetX() + diff.GetY() * diff.GetY());

		//assert(sq >= 0.0f && "Lib/Utils.inl/DistanceSquared: Distance negative impossible");

		if (sq <= 0)
		{
			return -1;
		}

		float sqfinal = std::sqrtf(sq);
		return sqfinal;
	}

	template<typename T>
	float Distance(const Vector2<T>& p_a, const Vector2<T>& p_b)
	{
		Vector2<T> diff = p_b - p_a;

		return static_cast<float>(diff.GetX() * diff.GetX() + diff.GetY() * diff.GetY());
	}

	template<typename T>
	float Dot(const Vector2<T>& p_a, const Vector2<T>& p_b)
	{
		return float(p_a.GetX() * p_b.GetX() + p_a.GetY() * p_b.GetY());
	}

	template<typename T>
	float Cross(const Vector2<T>& p_a, const Vector2<T>& p_b)
	{
		return float(p_a.GetX() * p_b.GetY() - p_a.GetY() * p_b.GetX());
	}

	template<typename T>
	T Hypotenuse(T p_a, T p_b)
	{
		return std::hypot(p_a, p_b);
	}
	template<typename T>
	T OppositeSide(T p_hypotenuse, T p_side)
	{
		return std::sqrt(p_hypotenuse * p_hypotenuse - p_side * p_side);
	}
	template<typename T>
	T FindHypotenuse(T p_a, T p_b, T p_c)
	{
		return Max({ p_a, p_b, p_c });
	}
	template<typename T>
	bool IsRightTriangle(T p_a, T p_b, T p_c)
	{
		T sorted[3] = { p_a, p_b, p_c };
		std::sort(sorted, sorted + 3);

		T side1 = sorted[0];
		T side2 = sorted[1];
		T hyp = sorted[2];

		T lhs = hyp * hyp;
		T rhs = side1 * side1 + side2 * side2;

		return std::abs(lhs - rhs) < T(1e-6) || std::abs(lhs - rhs) == 0;
	}
	template<typename T>
	std::string FindAngleRight(T p_a, T p_b, T p_c)
	{
		T sides[3] = { p_a, p_b, p_c };
		std::sort(sides, sides + 3);

		T side1 = sides[0];
		T side2 = sides[1];
		T hyp = sides[2];

		T lhs = hyp * hyp;
		T rhs = side1 * side1 + side2 * side2;

		if (std::abs(lhs - rhs) < T(1e-6) || std::abs(lhs - rhs) == 0)
		{
			if (p_a == hyp) return "CB";
			if (p_b == hyp) return "AC";
			if (p_c == hyp) return "BA";
		}

		return "non_rectangle";
	}

	template<typename T>
	T ThalesTheorem(T p_a, T p_b, T p_c)
	{
		assert(p_b != T(0) && "Lib/Utils.inl/ThalesTheorem: p_b = 0 impossible");

		assert(p_c > T(0) && "Lib/Utils.inl/ThalesTheorem: p_c doit �tre positif");

		// Formule : AN = (AB � AC) / AM
		return (p_b * p_c) / p_a;
	}
	template<typename T>
	bool ReciprocalThales(T p_a, T p_b, T p_c, T p_d)
	{
		assert(p_b != T(0) && p_d != T(0) && "Lib/Utils.inl/ReciprocalThales: p_b / p_d = 0 impossible");
		assert(p_a >= T(0) && p_c >= T(0) && "Lib/Utils.inl/ReciprocalThales: Longueurs n�gatives impossibles");

		// Test �galit� rapports
		return (p_a / p_b) == (p_c / p_d);
	}
	template<typename T>
	bool ReciprocalThalesVector(const Vector2<T>& p_A, const Vector2<T>& p_D, const Vector2<T>& p_B, const Vector2<T>& p_E, const Vector2<T>& p_C)
	{
		float len_AD = Distance(p_A, p_D);
		float len_AB = Distance(p_A, p_B);
		float len_AE = Distance(p_A, p_E);
		float len_AC = Distance(p_A, p_C);

		assert(len_AB > 0.0f && "A et B identiques");
		assert(len_AC > 0.0f && "A et C identiques");

		return (len_AD / len_AB) == (len_AE / len_AC) || std::abs((len_AD / len_AB) - (len_AE / len_AC)) < T(1e-6);
	}

	template<typename T>
	float Cos(T p_adjacent, T p_hypotenuse)
	{
		assert(p_hypotenuse > T(0) && "Lib/Utils.inl/Cos: hypot�nuse nulle");

		return static_cast<float>(p_adjacent) / static_cast<float>(p_hypotenuse);
	}
	template<typename T>
	T Cos(T p_angle)
	{
		if constexpr (std::is_same_v<T, double>)
		{
			return std::cos(p_angle);
		}
		else
		{
			return std::cosf(p_angle);
		}
	}

	template<typename T>
	float Sin(T p_opposite, T p_hypotenuse)
	{
		assert(p_hypotenuse > T(0) && "Lib/Utils.inl/Sin: hypot�nuse nulle");

		return static_cast<float>(p_opposite) / static_cast<float>(p_hypotenuse);
	}
	template<typename T>
	T Sin(T p_angle)
	{
		if constexpr (std::is_same_v<T, double>)
		{
			return std::sin(p_angle);
		}
		else
		{
			return std::sinf(p_angle);
		}
	}

	template<typename T>
	float Tan(T p_opposite, T p_adjacent)
	{
		assert(p_adjacent != T(0) && "Lib/Utils.inl/Tan: adjacent = 0 (90�)");

		return static_cast<float>(p_opposite) / static_cast<float>(p_adjacent);
	}
	template<typename T>
	T Tan(T p_angle)
	{
		if constexpr (std::is_same_v<T, double>)
		{
			return std::tan(p_angle);
		}
		else
		{
			return std::tanf(p_angle);
		}
	}

	template<typename T>
	float ArcSin(T p_opposite, T p_hypotenuse)
	{
		assert(p_hypotenuse > T(0) && "Lib/Utils.inl/ArcSin: hypot�nuse = 0");

		float sin_val = static_cast<float>(p_opposite) / static_cast<float>(p_hypotenuse);

		assert(sin_val >= -1.0f && sin_val <= 1.0f && "Lib/Utils.inl/ArcSin: sin hors [-1,1]");
		return std::asinf(sin_val);
	}

	template<typename T>
	float ArcCos(T p_adjacent, T p_hypotenuse)
	{
		assert(p_hypotenuse > T(0) && "Lib/Utils.inl/ArcCos: hypot�nuse = 0");

		float cos_val = Cos(p_adjacent, p_hypotenuse);

		assert(cos_val >= -1 && cos_val <= 1 && "Lib/Utils.inl/ArcCos: cos hors [-1,1]");

		return std::acosf(cos_val);
	}

	template<typename T>
	float ArcTan(T p_opposite, T p_adjacent)
	{
		assert(p_adjacent != T(0) && "Lib/Utils.inl/ArcTan: adjacent = 0");

		return std::atan2f(static_cast<float>(p_opposite), static_cast<float>(p_adjacent));
	}

	template<typename T>
	float NormalizeAngle(T p_angle)
	{
		float a = std::fmod(static_cast<float>(p_angle), 360.0f);
		if (a < 0.0f) {
			a += 360.0f;
		}
		return a;
	}

	template<typename T>
	float AngleLerp(T p_start, T p_end, float p_t)
	{
		float delta = std::fmod(static_cast<float>(p_end - p_start) + 180.0f, 360.0f);
		if (delta < 0.0f)
			delta += 360.0f;
		delta -= 180.0f;

		p_t = Clamp(p_t, 0.0f, 1.0f);
		return static_cast<float>(p_start) + delta * p_t;
	}

	template<typename T>
	float AngleDiff(T p_a, T p_b)
	{
		return NormalizeAngle(p_b - p_a);
	}

	template<typename T>
	static T Clamp(T p_value, T p_min, T p_max)
	{
		assert(p_min <= p_max && "Lib/Utils.inl/Clamp: min > max impossible");

		return std::clamp(p_value, p_min, p_max);
	}

	template<typename T>
	static T LimitMin(T p_value, T p_min)
	{
		return std::max(p_value, p_min);
	}
	template<typename T>
	static T LimitMax(T p_value, T p_max)
	{
		return std::min(p_value, p_max);
	}

	template<typename T>
	T Max(std::initializer_list<T> p_values)
	{
		assert(p_values.size() > 0 && "Lib/Utils.inl/Max: Max sur liste vide indefini");

		T result = *p_values.begin();
		for (const T& v : p_values)
		{
			result = std::max(result, v);
		}
		return result;
	}

	template<typename T>
	T Min(std::initializer_list<T> p_values)
	{
		assert(p_values.size() > 0 && "Lib/Utils.inl/Min: Min sur liste vide indefini");

		T result = *p_values.begin();
		for (const T& v : p_values)
		{
			result = std::min(result, v);
		}
		return result;
	}


	template<typename T>
	static T AbsValue(T value)
	{
		return std::fabs(value);
	}

	template<typename T, typename U>
	void Sort(T p_first, T p_last, U p_comparator)
	{
		std::sort(p_first, p_last, p_comparator);
	}

	template<typename T>
	bool EpsilonEqual(T p_a, T p_b, float p_abs_eps, float p_rel_eps)
	{
		float diff = std::fabs(p_a - p_b);
		if (diff <= p_abs_eps)
		{
			return true;
		}

		float maxAbs = std::max(std::fabs(p_a), std::fabs(p_b));
		return diff <= maxAbs * p_rel_eps;
	}

	template<typename T, typename U>
	auto CreateRule(T p_extract, U p_compare)
	{
		return [=](auto* obj) -> bool
			{
				return p_compare(p_extract(obj));
			};
	}

	template<typename T, typename U>
	std::vector<T> UseRule(std::vector<T>& p_elements, U p_compare)
	{
		std::vector<T> resultat;
		std::copy_if(p_elements.begin(), p_elements.end(), std::back_inserter(resultat), p_compare);
		return resultat;
	}

	template<typename T>
	void SwapValue(T& p_a, T& p_b)
	{
		std::swap(p_a, p_b);
	}

	template <typename T>
	std::size_t Type_Size()
	{
		return sizeof(T);
	}

	template<typename... Args>
	std::size_t Max_Arg_Size()
	{
		return std::max({ sizeof(Args)... });
	}
}