#pragma once
//Version 4.0.0

// La classe Vector2 est une classe de template qui représente un vecteur à deux dimensions,
// avec des composantes x et y de type T. Elle fournit des constructeurs pour initialiser les composantes du vecteur,
// ainsi que des opérateurs pour effectuer des opérations mathématiques sur les vecteurs, telles que l'addition, la soustraction,
// la multiplication par un scalaire, etc. La classe est conçue pour être utilisée dans des applications de jeux ou de graphiques où les vecteurs à deux dimensions sont couramment utilisés
// pour représenter des positions, des directions, des vitesses, etc.
template<typename T>
class Vector2
{
private:

	T m_x; // Composante x du vecteur
	T m_y; // Composante y du vecteur

public:

	// Constructeur par défaut
	// Ce constructeur initialise les composantes x et y du vecteur à zéro en utilisant la syntaxe d'initialisation de liste.
	// Par exemple, si vous créez un Vector2<int> sans fournir de valeurs, il sera initialisé à (0, 0).
	Vector2() : m_x(T(0)), m_y(T(0)) {}
	// Constructeur avec coordonnées
	// Ce constructeur permet de créer un Vector2<T> en spécifiant les valeurs de x et y. Les paramètres x_ et y_ sont utilisés pour initialiser les composantes du vecteur.
	// Par exemple, si vous créez un Vector2<float> avec les valeurs (3.5f, 4.5f), le vecteur sera initialisé à ces valeurs.
	Vector2(T p_x, T p_y) : m_x(p_x), m_y(p_y) {}
	// Constructeur de copie générique
	// Ce constructeur permet de créer un Vector2<T> à partir d'un Vector2<U>, en convertissant les types de données si nécessaire.
	// Par exemple, si vous avez un Vector2<int> et que vous voulez le convertir en Vector2<float>,
	// ce constructeur de copie générique permettra de faire cette conversion automatiquement.
	template <typename U>
	explicit Vector2(const Vector2<U>& vector);
	// Destructeur par défaut
	~Vector2() = default;

	//Définir le X de Vector2f
	void SetX(T p_x);
	//Définir le Y de Vector2f
	void SetY(T p_y);

	//Obtenir l'adresse de X de Vector2f
	T& GetX();
	//Obtenir l'adresse de Y de Vector2f
	T& GetY();

	//Obtenir la X de Vector2f non modifiable
	T GetX() const;
	//Obtenir la Y de Vector2f non modifiable
	T GetY() const;

	//Opérateurs d'indexation pour Vector2
	T& operator [] (int p_index);
	//Opérateurs d'indexation pour Vector2 non modifiable
	T operator [] (int p_index) const;

	//Opérateurs d'addition pour Vector2
	Vector2<T> operator + (const Vector2<T>& p_vector) const;
	//Opérateurs d'addition pour Vector2 avec un scalaire
	Vector2<T>& operator + (T p_scalar);
	//Opérateurs d'addition et d'affectation pour Vector2
	Vector2<T>& operator += (const Vector2<T>& p_vector);

	//Opérateurs de soustraction pour Vector2
	Vector2<T> operator - (const Vector2<T>& p_vector) const;
	//Opérateurs de soustraction pour Vector2 avec un scalaire
	Vector2<T>& operator - (T p_scalar);
	//Opérateurs de soustraction et d'affectation pour Vector2
	Vector2<T>& operator -= (const Vector2<T>& p_vector);

	//Opérateurs de multiplication pour Vector2
	Vector2<T> operator * (const Vector2<T>& p_vector) const;
	//Opérateurs de multiplication pour Vector2 avec un scalaire
	Vector2<T> operator * (T p_scalar) const;
	//Opérateurs de multiplication et d'affectation pour Vector2 avec un scalaire
	Vector2<T>& operator *= (T p_scalar);

	//Opérateurs de division pour Vector2
	Vector2<T> operator / (T scalar) const;
	//Opérateurs de division et d'affectation pour Vector2
	Vector2<T>& operator /= (T scalar);

	//Opérateur d'égalité pour Vector2
	bool operator == (const Vector2<T>& p_vector) const;
	//Opérateur de différence pour Vector2	
	bool operator != (const Vector2<T>& p_vector) const;

	//Opérateurs d'inversion pour Vector2
	Vector2<T> operator - () const;

	//Opérateur de comparaison pour Vector2 (compare la magnitude)
	bool operator < (const Vector2<T>& other) const;
	//Opérateur de comparaison pour Vector2 (compare la magnitude)
	bool operator > (const Vector2<T>& other) const;
	//Opérateur de comparaison pour Vector2 (compare la magnitude)
	bool operator <= (const Vector2<T>& other) const;
	//Opérateur de comparaison pour Vector2 (compare la magnitude)
	bool operator >= (const Vector2<T>& other) const;
};

//Opérateurs de multiplication pour Vector2 avec un scalaire (scalaire à gauche)
template<typename T>
inline Vector2<T> operator * (T p_scalar, const Vector2<T>& p_vector)
{
	return p_vector * p_scalar;
}

#include "Vector2.inl"

// Alias pour le type de vector à deux dimensions avec des int
using Vector2i = Vector2<int>;
// Alias pour le type de vector à deux dimensions avec des unsigned int
using Vector2u = Vector2<unsigned int>;
// Alias pour le type de vector à deux dimensions avec des float
using Vector2f = Vector2<float>;
