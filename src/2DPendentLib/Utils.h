#pragma once
//#include "glad/glad.h"
//#include "glm/glm.hpp"
#include <SDL3/SDL.h>
#include <cmath>
#include <numbers>
#include <initializer_list>
#include <string>
#include <functional>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <array>
#include <utility>
#include <cstddef>
#include "Vector2.h"

namespace Utils
{
	//PI
	// C'est une constante mathématique qui représente le rapport entre la circonférence d'un cercle et son diamètre.
	// Elle est utilisée dans de nombreux domaines des mathématiques et de la physique, notamment pour les calculs liés aux cercles,
	// aux sphères, aux ondes, et à la trigonométrie.
	//Sert a calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques
	static constexpr float PI = std::numbers::pi_v<float>;
	//PI2
	// C'est simplement deux fois la valeur de PI, soit 2 * 3.14 etc.... Elle est souvent utilisée dans les calculs impliquant des cercles complets,
	// tels que les rotations à 360 degrés ou les fonctions périodiques.
	//Sert a calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques
	static constexpr float PI2 = 2.0 * PI;
	// HALF_PI
	// C'est la moitié de la valeur de PI, soit 3.14 etc.... / 2. Elle est souvent utilisée dans les calculs impliquant des angles droits ou des rotations à 90 degrés.
	//Sert a calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques
	static constexpr float HALF_PI = 0.5 * PI;
	// DEG2RAD
	// C'est une constante de conversion qui permet de convertir des angles exprimés en degrés en radians.
	// La formule de conversion est la suivante : radians = degrés * (PI / 180).
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques
	static constexpr float DEG2RAD = PI / 180.0f;
	// RAD2DEG
	// C'est une constante de conversion qui permet de convertir des angles exprimés en radians en degrés.
	// La formule de conversion est la suivante : degrés = radians * (180 / PI).
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques
	static constexpr float RAD2DEG = 180.0f / PI;

	// Comparateur Croissant
	// Exemple de fonctionnement 0, 5, 4, 2, 1, 3, 5, 2, 4, 1
	// Avec le comparateur croissant, les éléments seraient triés dans l'ordre suivant : 0, 1, 1, 2, 2, 3, 4, 4, 5, 5.
	template<typename T>
	static constexpr std::less<T> LESS = std::less<T>();

	// Comparateur Décroissant
	// Exemple de fonctionnement 0, 5, 4, 2, 1, 3, 5, 2, 4, 1
	// Avec le comparateur décroissant, les éléments seraient triés dans l'ordre suivant : 5, 5, 4, 4, 3, 2, 2, 1, 1, 0.
	template<typename T>
	static constexpr std::greater<T> GREATER = std::greater<T>();

	// Comparateur croissant basé sur la valeur absolue
	// Exemple de fonctionnement -3, 2, -1, 4, 0
	// Avec le comparateur croissant basé sur la valeur absolue, les éléments seraient triés dans l'ordre suivant : 0, -1, 2, -3, 4.
	template<typename T>
	static auto AbsLess = [](const T& p_a, const T& p_b) { return std::abs(p_a) < std::abs(p_b); };
	// Comparateur décroissant basé sur la valeur absolue
	// Exemple de fonctionnement -3, 2, -1, 4, 0
	// Avec le comparateur décroissant basé sur la valeur absolue, les éléments seraient triés dans l'ordre suivant : 4, -3, 2, -1, 0.
	template<typename T>
	static auto AbsGreater = [](const T& p_a, const T& p_b) { return std::abs(p_a) > std::abs(p_b); };

	// Comparateur croissant basé sur la distance au carré
	// Exemple de fonctionnement (1, 2), (3, 4), (0, 0), (2, 2)
	// Avec le comparateur croissant basé sur la distance au carré, les éléments seraient triés dans l'ordre suivant : (0, 0), (1, 2), (2, 2), (3, 4).
	template<typename T>
	static auto Dist2DLess = [](const Vector2<T>& p_a, const Vector2<T>& p_b) { return p_a.GetX() * p_a.GetX() + p_a.GetY() * p_a.GetY() < p_b.GetX() * p_b.GetX() + p_b.GetY() * p_b.GetY(); };
	// Comparateur décroissant basé sur la distance au carré
	// Exemple de fonctionnement (1, 2), (3, 4), (0, 0), (2, 2)
	// Avec le comparateur décroissant basé sur la distance au carré, les éléments seraient triés dans l'ordre suivant : (3, 4), (2, 2), (1, 2), (0, 0).
	template<typename T>
	static auto Dist2DGreater = [](const Vector2<T>& p_a, const Vector2<T>& p_b) { return p_a.GetX() * p_a.GetX() + p_a.GetY() * p_a.GetY() > p_b.GetX() * p_b.GetX() + p_b.GetY() * p_b.GetY(); };

	// Comparateur croissant basé sur la distance au carré par rapport à un point donné
	// Exemple de fonctionnement avec le point de référence (1, 1) : (2, 2), (3, 4), (0, 0), (1, 2)
	// Avec le comparateur croissant basé sur la distance au carré par rapport à un point donné, les éléments seraient triés dans l'ordre suivant : (1, 2), (0, 0), (2, 2), (3, 4).
	//Pour l'utiliser, vous pouvez créer une instance de ce comparateur en fournissant un point de référence, par exemple : auto comparator = Utils::DistToPointLess(Vector2<float>(1.0f, 1.0f));
	// Ensuite, vous pouvez utiliser ce comparateur pour trier une liste de points en fonction de leur distance au carré par rapport au point de référence,
	// par exemple : std::sort(points.begin(), points.end(), comparator);
	// Cela triera les points dans l'ordre croissant de leur distance au carré par rapport au point de référence (1, 1).
	template<typename T>
	static auto DistToPointLess = [](const Vector2<T>& p_point)
	{
		return [p_point](const Vector2<T>& p_a, const Vector2<T>& p_b)
		{
			return (p_a.GetX() - p_point.GetX()) * (p_a.GetX() - p_point.GetX()) + (p_a.GetY() - p_point.GetY()) * (p_a.GetY() - p_point.GetY()) < (p_b.GetX() - p_point.GetX()) * (p_b.GetX() - p_point.GetX()) + (p_b.GetY() - p_point.GetY()) * (p_b.GetY() - p_point.GetY());
		};
	};
	// Comparateur décroissant basé sur la distance au carré par rapport à un point donné
	// Exemple de fonctionnement avec le point de référence (1, 1) : (2, 2), (3, 4), (0, 0), (1, 2)
	// Avec le comparateur décroissant basé sur la distance au carré par rapport à un point donné, les éléments seraient triés dans l'ordre suivant : (3, 4), (2, 2), (0, 0), (1, 2).
	//Pour l'utiliser, vous pouvez créer une instance de ce comparateur en fournissant un point de référence, par exemple : auto comparator = Utils::DistToPointGreater(Vector2<float>(1.0f, 1.0f));
	// Ensuite, vous pouvez utiliser ce comparateur pour trier une liste de points en fonction de leur distance au carré par rapport au point de référence,
	// par exemple : std::sort(points.begin(), points.end(), comparator);
	// Cela triera les points dans l'ordre décroissant de leur distance au carré par rapport au point de référence (1, 1).
	template<typename T>
	static auto DistToPointGreater = [](const Vector2<T>& p_point)
	{
		return [p_point](const Vector2<T>& p_a, const Vector2<T>& p_b)
		{
			return (p_a.GetX() - p_point.GetX()) * (p_a.GetX() - p_point.GetX()) + (p_a.GetY() - p_point.GetY()) * (p_a.GetY() - p_point.GetY()) > (p_b.GetX() - p_point.GetX()) * (p_b.GetX() - p_point.GetX()) + (p_b.GetY() - p_point.GetY()) * (p_b.GetY() - p_point.GetY());
		};
	};

	// Comparateur d'angle croissant
	// Exemple de fonctionnement (1, 0), (0, 1), (-1, 0), (0, -1)
	// Avec le comparateur d'angle croissant, les éléments seraient triés dans l'ordre suivant : (1, 0), (0, 1), (-1, 0), (0, -1).
	template<typename T>
	static auto AngleLess = [](const Vector2<T>& p_a, const Vector2<T>& p_b)
	{
		return std::atan2(p_a.GetY(), p_a.GetX()) < std::atan2(p_b.GetY(), p_b.GetX());
	};

	// Comparateur d'angle décroissant
	// Exemple de fonctionnement (1, 0), (0, 1), (-1, 0), (0, -1)
	// Avec le comparateur d'angle décroissant, les éléments seraient triés dans l'ordre suivant : (0, -1), (-1, 0), (0, 1), (1, 0).
	template<typename T>
	static auto AngleGreater = [](const Vector2<T>& p_a, const Vector2<T>& p_b)
	{
		return std::atan2(p_a.GetY(), p_a.GetX()) > std::atan2(p_b.GetY(), p_b.GetX());
	};

	// Convertit un angle de degrés en radians
	// La formule de conversion est la suivante : radians = degrés * (3.14 etc.... / 180).
	//Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques
	static float DegToRad(float p_degree) { return p_degree * DEG2RAD; }
	// Convertit un angle de radians en degrés
	// La formule de conversion est la suivante : degrés = radians * (180 / 3.14 etc....).
	//Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques
	static float RadToDeg(float p_radian) { return p_radian * RAD2DEG; }

	// Calcule l'angle entre deux vecteurs en utilisant la fonction atan2, qui retourne l'angle en radians entre le vecteur formé par les deux points et l'axe des x.
	// La formule pour calculer l'angle entre deux vecteurs a et b est la suivante : angle = atan2(b.y - a.y, b.x - a.x).
	// Utilisée pour calculer les angles entre les vecteurs, les projections, les réflexions, etc. dans les jeux,
	// en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez deux vecteurs de direction pour des personnages dans un jeu,
	// vous pouvez calculer l'angle entre ces deux vecteurs pour déterminer la direction relative des personnages l'un par rapport à l'autre.
	template<typename T>
	static float Angle(const Vector2<T>& p_a, const Vector2<T>& p_b);

	// Normalise un vecteur en le divisant par sa propre longueur, ce qui donne un vecteur de longueur 1 qui pointe dans la même direction que le vecteur d'origine.
	// La formule de normalisation est la suivante : v_normalized = v / ||v||, où ||v|| est la longueur du vecteur v.
	// Utilisée pour calculer les directions, les normales, les réflexions, etc. dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez un vecteur de direction pour un personnage dans un jeu,
	// vous pouvez le normaliser pour obtenir une direction unitaire, puis multiplier ce vecteur normalisé par la vitesse du personnage pour obtenir la vélocité du personnage.
	// Par exemple, si vous avez un vecteur de direction (3, 4), sa longueur est 5, donc le vecteur normalisé serait (3/5, 4/5) = (0.6, 0.8).
	template<typename T>
	static Vector2<float> NormalizeVector(const Vector2<T>& p_vector);

	// Calcule et renvoie le vecteur perpendiculair au vecteur en paramètre
	// Exemple d'utilité : dans le déplacement d'une IA on peux vouloir que si elle rencontre un obstacle elle essaie de la contourner en passant par le côté,
	// elle prendra alors en direction le vecteur perpendiculaire de son vecteur de direction initial
	template<typename T>
	static Vector2<T> Perpendicular(const Vector2<T>& p_v);

	// Calcule la réflexion d'un vecteur incident par rapport à une normale.
	// La formule de réflexion est la suivante : r = i - 2 * dot(i, n) * n, où i est le vecteur incident, n est la normale, et r est le vecteur réfléchi.
	// Utilisée pour calculer les réflexions dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez un vecteur de direction pour un personnage qui rebondit sur une surface dans un jeu,
	// vous pouvez calculer la réflexion de ce vecteur de direction par rapport à la normale de la surface pour obtenir la nouvelle direction du personnage après le rebond.
	// Par exemple, si vous avez un vecteur de direction (0.6, 0.8) et une normale (0, 1) pour une surface horizontale,
	// le vecteur réfléchi serait (0.6, -0.8), ce qui indique que le personnage rebondit vers le bas après avoir touché la surface.
	// Notez que la normale doit être un vecteur unitaire pour que la formule de réflexion fonctionne correctement.
	// Si la normale n'est pas unitaire, vous devez la normaliser avant de l'utiliser dans la formule de réflexion.
	// Par exemple, si vous avez une normale (0, 2) pour une surface horizontale, vous devez la normaliser à (0, 1) avant de l'utiliser dans la formule de réflexion.
	// Si la normale n'est pas unitaire, la formule de réflexion ne donnera pas le résultat correct, car elle suppose que la normale a une longueur de 1.
	// Par exemple, si vous utilisez une normale non unitaire (0, 2) dans la formule de réflexion avec un vecteur incident (0.6, 0.8),
	// le vecteur réfléchi serait (0.6, -1.6), ce qui n'est pas correct, car il ne reflète pas correctement la direction du rebond.
	template<typename T>
	static Vector2<T> Reflect(const Vector2<T>& p_incident, const Vector2<T>& p_normal);

	// Calcule la vraie distance entre deux points représentés par des vecteurs.
	// La formule de distance est la suivante : DistanceSquared(a, b) = sqrt((b.x - a.x)^2 + (b.y - a.y)^2), où a et b sont les deux points.
	// Utilisée pour calculer les distances de manière précise dans les jeux, en particulier pour les affichages, les calculs précis
	template<typename T>
	static float DistanceSquared(const Vector2<T>& p_a, const Vector2<T>& p_b);

	// Compare la distance au carré entre deux points représentés par des vecteurs.
	// La formule de distance au carré est la suivante : distance(a, b) = (b.x - a.x)^2 + (b.y - a.y)^2, où a et b sont les deux points.
	// Utilisée pour comparer les distances de manière plus rapide dans les jeux, en particulier pour les collisions, les range etc.,
	// sans avoir besoin de calculer la racine carrée, ce qui peut être coûteux en termes de performance.
	template<typename T>
	static float Distance(const Vector2<T>& p_a, const Vector2<T>& p_b);

	// Calcule le produit scalaire (dot product) de deux vecteurs.
	// La formule du produit scalaire est la suivante : dot(a, b) = a.x * b.x + a.y * b.y, où a et b sont les deux vecteurs.
	template <typename T>
	static float Dot(const Vector2<T>& p_a, const Vector2<T>& p_b);

	// Calcule le produit vectoriel (cross product) de deux vecteurs en 2D.
	// En 2D, le produit vectoriel de deux vecteurs a et b est un scalaire donné par la formule : cross(a, b) = a.x * b.y - a.y * b.x.
	template <typename T>
	static float Cross(const Vector2<T>& p_a, const Vector2<T>& p_b);

	// Calcule l'hypoténuse d'un triangle rectangle à partir de ses deux côtés adjacents.
	// La formule de l'hypoténuse est la suivante : hypotenuse(a, b) = sqrt(a^2 + b^2), où a et b sont les longueurs des deux côtés adjacents du triangle rectangle.
	// Utilisée pour calculer les distances, les longueurs, etc. dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez les longueurs des deux côtés adjacents d'un triangle rectangle dans un jeu,
	// vous pouvez utiliser la fonction Hypotenuse pour calculer la longueur de l'hypoténuse,
	// qui peut être utilisée pour déterminer la distance entre deux points, la longueur d'un vecteur, etc.
	template<typename T>
	static T Hypotenuse(T p_a, T p_b);

	// Calcule la longueur du côté opposé d'un triangle rectangle à partir de l'hypoténuse et du côté adjacent.
	// La formule pour calculer le côté opposé est la suivante : opposite = sqrt(hypotenuse^2 - side^2),
	// où hypotenuse est la longueur de l'hypoténuse, side est la longueur du côté adjacent, et opposite est la longueur du côté opposé.
	// Utilisée pour calculer les distances, les longueurs, etc. dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez la longueur de l'hypoténuse et la longueur du côté adjacent d'un triangle rectangle dans un jeu,
	// vous pouvez utiliser la fonction OppositeSide pour calculer la longueur du côté opposé,
	// qui peut être utilisée pour déterminer la distance entre deux points, la longueur d'un vecteur, etc.
	template<typename T>
	static T OppositeSide(T p_hypotenuse, T p_side);

	// Calcule la longueur de l'hypoténuse d'un triangle rectangle à partir de ses trois côtés.
	// La formule pour calculer l'hypoténuse à partir de trois côtés est la suivante : hypotenuse = sqrt(a^2 + b^2 - c^2),
	// où a et b sont les longueurs des deux côtés adjacents du triangle rectangle, c est la longueur du côté opposé, et hypotenuse est la longueur de l'hypoténuse.
	// Utilisée pour calculer les distances, les longueurs, etc. dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez les longueurs des deux côtés adjacents et du côté opposé d'un triangle rectangle dans un jeu,
	// vous pouvez utiliser la fonction FindHypotenuse pour calculer la longueur de l'hypoténuse,
	// qui peut être utilisée pour déterminer la distance entre deux points, la longueur d'un vecteur, etc.
	template<typename T>
	static T FindHypotenuse(T p_a, T p_b, T p_c);

	// Vérifie si les trois côtés donnés peuvent former un triangle rectangle en utilisant le théorème de Pythagore.
	// La formule pour vérifier si trois côtés peuvent former un triangle rectangle est la suivante : a^2 + b^2 = c^2,
	// où a et b sont les longueurs des deux côtés adjacents du triangle rectangle, et c est la longueur du côté opposé.
	// Utilisée pour vérifier les conditions de formation d'un triangle rectangle dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez les longueurs de trois côtés dans un jeu, vous pouvez utiliser la fonction IsRightTriangle
	// pour vérifier si ces côtés peuvent former un triangle rectangle,
	// ce qui peut être utile pour déterminer les angles, les directions, etc. dans le jeu.
	template<typename T>
	bool IsRightTriangle(T p_a, T p_b, T p_c);

	// Trouve l'angle droit formé par les trois côtés donnés en utilisant le théorème de Pythagore.
	// La formule pour trouver l'angle droit formé par trois côtés est la suivante : c^2 = a^2 + b^2,
	// où a et b sont les longueurs des deux côtés adjacents du triangle rectangle, c est la longueur du côté opposé, et l'angle droit est formé par les côtés a et b.
	// Utilisée pour trouver les angles droits formés par trois côtés dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : si vous avez les longueurs de trois côtés dans un jeu, vous pouvez utiliser la fonction FindAngleRight
	// pour trouver l'angle droit formé par ces côtés, ce qui peut être utile pour déterminer les angles, les directions, etc. dans le jeu.
	// Notez que cette fonction retourne une chaîne de caractères indiquant les côtés qui forment l'angle droit, ou "non_rectangle" si les côtés ne forment pas un triangle rectangle.
	// Par exemple, si vous avez les côtés 3, 4, et 5, la fonction FindAngleRight retournerait "CB", indiquant que les côtés C et B forment l'angle droit,
	// car 5^2 = 3^2 + 4^2.
	// Si vous avez les côtés 1, 2, et 3, la fonction FindAngleRight retournerait "non_rectangle", car ces côtés ne forment pas un triangle rectangle,
	// car 3^2 != 1^2 + 2^2.
	// En résumé, la fonction FindAngleRight vérifie si les trois côtés donnés peuvent former un triangle rectangle,
	template<typename T>
	static std::string FindAngleRight(T p_a, T p_b, T p_c);

	// Applique le théorème de Thales pour trouver la longueur d'un segment dans un triangle similaire.
	// La formule du théorème de Thales est la suivante : (a / b) = (c / d), où a et b sont les longueurs des côtés correspondants dans les triangles similaires,
	// et c et d sont les longueurs des segments correspondants dans les triangles similaires.
	// Utilisée pour trouver les longueurs de segments dans les triangles similaires dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : avec comme valeurs a = 3, b = 6, c = 2, la fonction ThalesTheorem retournerait 4, car (3 / 6) = (2 / 4).
	template<typename T>
	static T ThalesTheorem(T p_a, T p_b, T p_c);

	// Applique la réciproque du théorème de Thales pour vérifier si les segments donnés sont proportionnels, ce qui indiquerait que les triangles sont parallèles.
	// La formule de la réciproque du théorème de Thales est la suivante : (a / b) = (c / d), où a et b sont les longueurs des côtés correspondants dans les triangles parallèles,
	// et c et d sont les longueurs des segments correspondants dans les triangles parallèles.
	// Utilisée pour vérifier les conditions de similarité des triangles dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	template<typename T>
	static bool ReciprocalThales(T p_a, T p_b, T p_c, T p_d);

	// Applique la réciproque du théorème de Thales sur des vecteurs pour vérifier si les segments donnés sont proportionnels, ce qui indiquerait que les triangles sont parallèles.
	// La formule de la réciproque du théorème de Thales pour les vecteurs est la suivante
	// : (A / D) = (B / E) = (C / F), ou A, B, C sont les longueurs des côtés correspondants dans les triangles parallèles,
	// et D, E, F sont les longueurs des segments correspondants dans les triangles parallèles.
	// Utilisée pour vérifier les conditions de similarité des triangles dans les jeux, en particulier pour les mouvements, les collisions, et les effets de lumière.
	// Exemple d'utilisation : avec comme valeurs A = (3, 4), D = (6, 8), B = (2, 3), E = (4, 6), C = (1, 2), la fonction ReciprocalThalesVector retournerait true,
	// car (3 / 6) = (2 / 4) = (1 / 2).
	template<typename T>
	static bool ReciprocalThalesVector(const Vector2<T>& p_A, const Vector2<T>& p_D, const Vector2<T>& p_B, const Vector2<T>& p_E, const Vector2<T>& p_C);

	// Cosinus
	// La formule pour calculer le cosinus d'un angle dans un triangle rectangle est la suivante : cos(angle) = adjacent / hypotenuse,
	// où adjacent est la longueur du côté adjacent à l'angle, et hypotenuse est la longueur de l'hypoténuse du triangle rectangle.
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques dans les jeux.
	template<typename T>
	static float Cos(T p_adjacent, T p_hypotenuse);

	// Cosinus
	// La formule pour calculer le cosinus d'un angle dans un triangle rectangle est la suivante : cos(angle) = adjacent / hypotenuse,
	// où adjacent est la longueur du côté adjacent à l'angle, et hypotenuse est la longueur de l'hypoténuse du triangle rectangle.
	template<typename T>
	static T Cos(T p_angle);

	// Sinus
	// La formule pour calculer le sinus d'un angle dans un triangle rectangle est la suivante : sin(angle) = opposite / hypotenuse,
	// où opposite est la longueur du côté opposé à l'angle, et hypotenuse est la longueur de l'hypoténuse du triangle rectangle.
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques dans les jeux.
	template<typename T>
	static float Sin(T p_opposite, T p_hypotenuse);

	// Sinus
	// La formule pour calculer le sinus d'un angle dans un triangle rectangle est la suivante : sin(angle) = opposite / hypotenuse,
	// où opposite est la longueur du côté opposé à l'angle, et hypotenuse est la longueur de l'hypoténuse du triangle rectangle.
	template<typename T>
	static T Sin(T p_angle);

	// Tangente
	// La formule pour calculer la tangente d'un angle dans un triangle rectangle est la suivante : tan(angle) = opposite / adjacent,
	// où opposite est la longueur du côté opposé à l'angle, et adjacent est la longueur du côté adjacent à l'angle dans le triangle rectangle.
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques dans les jeux.
	template<typename T>
	static float Tan(T p_opposite, T p_adjacent);

	// Tangente
	// La formule pour calculer la tangente d'un angle dans un triangle rectangle est la suivante : tan(angle) = opposite / adjacent,
	// où opposite est la longueur du côté opposé à l'angle, et adjacent est la longueur du côté adjacent à l'angle dans le triangle rectangle.
	template<typename T>
	static T Tan(T p_angle);

	// ArcSinus
	// La formule pour calculer l'arc sinus d'un angle dans un triangle rectangle est la suivante : angle = arcsin(opposite / hypotenuse),
	// où opposite est la longueur du côté opposé à l'angle, et hypotenuse est la longueur de l'hypoténuse du triangle rectangle.
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques dans les jeux.
	template<typename T>
	static float ArcSin(T p_opposite, T p_hypotenuse);

	// ArcCosinus
	// La formule pour calculer l'arc cosinus d'un angle dans un triangle rectangle est la suivante : angle = arccos(adjacent / hypotenuse),
	// où adjacent est la longueur du côté adjacent à l'angle, et hypotenuse est la longueur de l'hypoténuse du triangle rectangle.
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques dans les jeux.
	template<typename T>
	static float ArcCos(T p_adjacent, T p_hypotenuse);

	// ArcTangente
	// La formule pour calculer l'arc tangente d'un angle dans un triangle rectangle est la suivante : angle = arctan(opposite / adjacent),
	// où opposite est la longueur du côté opposé à l'angle, et adjacent est la longueur du côté adjacent à l'angle dans le triangle rectangle.
	// Utilisée pour calculer les angles et les rotations, ainsi que pour les fonctions trigonométriques dans les jeux.
	template<typename T>
	static float ArcTan(T p_opposite, T p_adjacent);

	// Normalise un angle en le ramenant dans la plage de 0 à 360 degrés.
	// La formule pour normaliser un angle est la suivante : normalized_angle = angle mod 360, où angle est l'angle à normaliser,
	// et normalized_angle est l'angle normalisé dans la plage de 0 à 360 degrés.
	// Utilisée pour normaliser les angles dans les jeux, en particulier pour les rotations, les mouvements, et les effets de lumière.
	// Exemple d'utilisation : si vous avez un angle de 450 degrés dans un jeu, vous pouvez utiliser la fonction NormalizeAngle pour le normaliser à 90 degrés,
	// car 450 mod 360 = 90.
	template<typename T>
	static float NormalizeAngle(T p_angle);

	// Interpole linéairement entre deux angles en tenant compte de la nature cyclique des angles.
	// La formule pour interpoler linéairement entre deux angles a et b avec un facteur de interpolation t est la suivante :
	// angle = a + t * (b - a), où a et b sont les deux angles, t est le facteur d'interpolation entre 0 et 1, et angle est l'angle interpolé.
	// Cependant, pour les angles, il est important de prendre en compte la nature cyclique des angles, ce qui signifie que l'angle 0 degrés est équivalent à l'angle 360 degrés.
	// Par conséquent, la formule pour interpoler linéairement entre deux angles a et b avec un facteur de interpolation t en tenant compte de la nature cyclique des angles est la suivante :
	// angle = a + t * ((b - a + 180) mod 360 - 180), où a et b sont les deux angles, t est le facteur d'interpolation entre 0 et 1, et angle est l'angle interpolé.
	// Utilisée pour interpoler entre deux angles dans les jeux, pour les transitions douces entre les angles.
	template<typename T>
	static float AngleLerp(T p_start, T p_end, float p_t);

	// AngleDiff
	// Calcule la différence entre deux angles en tenant compte de la nature cyclique des angles.
	// La formule pour calculer la différence entre deux angles a et b en tenant compte de la nature cyclique des angles est la suivante :
	// angle_diff = (b - a + 180) mod 360 - 180, où a et b sont les deux angles, et angle_diff est la différence entre les deux angles dans la plage de -180 à 180 degrés.
	// Exemple d'utilisation : si vous avez deux angles de 30 degrés et 350 degrés dans un jeu, vous pouvez utiliser la fonction AngleDiff pour calculer la différence entre ces deux angles,
	template<typename T>
	static float AngleDiff(T p_a, T p_b);

	// Limite une valeur entre une valeur minimale et une valeur maximale
	// Si la valeur est inférieure à la valeur minimale, elle retourne la valeur minimale.
	// Si la valeur est supérieure à la valeur maximale, elle retourne la valeur maximale.
	// Sinon, elle retourne la valeur elle-même.
	// Utilisée pour limiter les valeurs dans les jeux, par exemple pour limiter la vitesse d'un personnage, la position d'un objet, etc.
	template<typename T>
	static T Clamp(T p_value, T p_min, T p_max);

	// Limite une valeur à une valeur minimale
	// Si la valeur est inférieure à la valeur minimale, elle retourne la valeur minimale.
	// Sinon, elle retourne la valeur elle-même.
	// Utilisée pour limiter les valeurs dans les jeux, par exemple pour limiter la vitesse d'un personnage, la position d'un objet, etc.
	template<typename T>
	static T LimitMin(T p_value, T p_min);

	// Limite une valeur à une valeur maximale
	// Si la valeur est supérieure à la valeur maximale, elle retourne la valeur maximale.
	// Sinon, elle retourne la valeur elle-même.
	// Utilisée pour limiter les valeurs dans les jeux, par exemple pour limiter la vitesse d'un personnage, la position d'un objet, etc.
	template<typename T>
	static T LimitMax(T p_value, T p_max);

	// Retourne la valeur maximale parmi une liste de valeurs.
	// Utilisée pour trouver la valeur maximale dans une liste de valeurs, par exemple pour trouver le score le plus élevé, la vitesse maximale, etc. dans les jeux.
	// Exemple d'utilisation : si vous avez une liste de scores dans un jeu, vous pouvez utiliser la fonction Max pour trouver le score le plus �lev� parmi cette liste.
	template<typename T>
	static T Max(std::initializer_list<T> p_values);
	// Retourne la valeur minimale parmi une liste de valeurs.
	// Utilisée pour trouver la valeur minimale dans une liste de valeurs, par exemple pour trouver le score le plus bas, la vitesse minimale, etc. dans
	// les jeux.
	// Exemple d'utilisation : si vous avez une liste de scores dans un jeu, vous pouvez utiliser la fonction Min pour trouver le score le plus bas parmi cette liste.
	template<typename T>
	static T Min(std::initializer_list<T> p_values);

	// Retourne la valeur absolue d'un nombre, c'est-à-dire sa distance par rapport à zéro, sans tenir compte de son signe.
	// Par exemple, la valeur absolue de -5 est 5, et la valeur absolue de 3 est également 3.
	// Utilisée pour calculer les distances, les vitesses, etc. dans les jeux, sans se soucier du signe des valeurs.
	template<typename T>
	static T AbsValue(T p_value);

	// Trie une collection d'éléments en utilisant un comparateur personnalisé.
	// T représente le type des éléments dans la collection, et U représente le type du comparateur.
	template<typename T, typename U>
	static void Sort(T p_first, T p_last, U p_comparator);

	// Vérifie si deux valeurs sont égales en utilisant une tolérance absolue et relative 1e-3f = 0,001 et 1e-4f = 0,0001.
	// La formule pour vérifier si deux valeurs a et b sont égales en utilisant une tolérance absolue et relative est la suivante :
	// epsilon_equal = abs(a - b) <= max(abs_eps, rel_eps * max(abs(a), abs(b))), où a et b sont les deux valeurs à comparer,
	// abs_eps est la tolérance absolue, rel_eps est la tolérance relative, et epsilon_equal est un booléen qui indique si les deux valeurs sont égales en tenant compte de la tolérance.
	// Exemple de calcul : si vous avez deux valeurs de 0.1 et 0.10001, une tolérance absolue de 0.0001, et une tolérance relative de 0.0001,
	// la fonction EpsilonEqual retournerait true, car abs(0.1 - 0.10001) = 0.00001 <= max(0.0001, 0.0001 * max(abs(0.1), abs(0.10001))) = 0.0001.
	template<typename T>
	static bool EpsilonEqual(T p_a, T p_b, float p_abs_eps = 1e-3f, float p_rel_eps = 1e-4f);
	
	// Crée une règle de tri personnalisée en combinant une fonction d'extraction et un comparateur.
	// F représente la fonction d'extraction qui extrait une clé de tri à partir d'un élément, et C représente le comparateur qui compare les clés de tri extraites.
	// La fonction d'extraction est utilisée pour extraire une clé de tri à partir d'un élément, et le comparateur est utilisé pour comparer les clés de tri extraites.
	// Exemple d'utilisation : si vous avez une collection d'objets avec une propriété "age",
	// vous pouvez créer une règle de tri personnalisée pour trier les objets en fonction de leur âge en utilisant la fonction CreateRegle,
	// par exemple : auto age_comparator = Utils::CreateRegle([](const MyObject& obj) { return obj.age; }, std::less<int>());
	template<typename T, typename U>
	static auto CreateRule(T p_extract, U p_compare);
	
	template<typename T, typename U>
	static std::vector<T> UseRule(std::vector<T>& p_elements, U p_compare);
	
	//Fonction statique pour extraire le nom du fichier à partir de son chemin
	//Exemple d'utilisation : si vous avez un chemin de fichier "C:/Users/Username/Documents/file.txt", la fonction GetNameFromPath retournerait "file".
	static std::string GetNameFromPath(const std::string& p_path)
	{
		return std::filesystem::path(p_path).stem().string();
	}

	// Fonction qui échange les valeur des variable
	// Exemple int a = 0 et int b = 1 Swap(a, b) -> a = 1 et b = 0
	// avec des Vector -> Vector2f a = {0.0f, 1.0f} et Vector2f b = {2.0f, 3.0f} Swap(a, b) -> a = {2.0f, 3.0f) et b = {0.0f, 1.0f}
	template<typename T>
	static void SwapValue(T& p_a, T& p_b);

	//Cette fonction permet d'obtenir les coins d'un rectangle à partir de son collider,
	// elle retourne un tableau de 4 Vector2f représentant les positions des coins du rectangle
	// Exemple d'utilisation : si vous avez un collider de rectangle avec une position de (100, 100) et une dimension de (50, 50),
	// la fonction GetCorners retournerait un tableau contenant les positions des coins du rectangle : [(100, 100), (150, 100), (150, 150), (100, 150)].
	static std::array<Vector2f, 4> GetCorners(Vector2f p_position, Vector2i p_dimension, float p_angle, Vector2f p_rotationPoint)
	{
		// 1. Pré-calculs des limites locales (évite les multiplications répétées)
		float xMin = -p_dimension.GetX() * p_rotationPoint.GetX();
		float xMax = p_dimension.GetX() * (1.0f - p_rotationPoint.GetX());
		float yMin = -p_dimension.GetY() * p_rotationPoint.GetY();
		float yMax = p_dimension.GetY() * (1.0f - p_rotationPoint.GetY());

		// 2. Rotation
		float angle = p_angle * Utils::DEG2RAD;
		float cosA = Utils::Cos(angle);
		float sinA = Utils::Sin(angle);

		// 3. Construction directe (évite le tableau intermédiaire 'local' et la boucle)
		// On calcule les composantes réutilisables
		float xMinCos = xMin * cosA;
		float xMinSin = xMin * sinA;
		float xMaxCos = xMax * cosA;
		float xMaxSin = xMax * sinA;

		float yMinCos = yMin * cosA;
		float yMinSin = yMin * sinA;
		float yMaxCos = yMax * cosA;
		float yMaxSin = yMax * sinA;

		return {
			Vector2f{ p_position.GetX() + xMinCos - yMinSin, p_position.GetY() + xMinSin + yMinCos }, // Haut-Gauche
			Vector2f{ p_position.GetX() + xMaxCos - yMinSin, p_position.GetY() + xMaxSin + yMinCos }, // Haut-Droite
			Vector2f{ p_position.GetX() + xMaxCos - yMaxSin, p_position.GetY() + xMaxSin + yMaxCos }, // Bas-Droite
			Vector2f{ p_position.GetX() + xMinCos - yMaxSin, p_position.GetY() + xMinSin + yMaxCos }  // Bas-Gauche
		};
	}

	// Fonction pour générer une liste de nombres entiers entre un nombre de départ et un nombre de fin avec une augmentation spécifiée
	static std::vector<int> GenerateIntNumber(int p_numberStart, int p_numberEnd, int p_increase)
	{
		std::vector<int> numbers;
		int nb = (p_numberEnd - p_numberStart) / p_increase + 1;

		numbers.resize(nb);

		for (int i = 0; i < nb; i++)
		{
			numbers[i] = p_numberStart + (i * p_increase);
		}
		return numbers;
	}

	// Fonction pour vérifier si un texte est présent dans une chaîne
	static bool TextInString(const std::string& p_text, const std::string& p_string)
	{
		return p_string.find(p_text) != std::string::npos;
	}

	template <typename T>
	static std::size_t Type_Size();

	template<typename... Args>
	static std::size_t Max_Arg_Size();




	static void MakeOrtho(float p_mat[16],
		float left, float right,
		float bottom, float top,
		float nearZ = -1.0f, float farZ = 1.0f)
	{
		float rL = 1.0f / (right - left);
		float tB = 1.0f / (top - bottom);
		float fN = 1.0f / (farZ - nearZ);

		// Zéro d'abord
		for (int i = 0; i < 16; ++i) p_mat[i] = 0.0f;

		p_mat[0] = 2.0f * rL;
		p_mat[5] = 2.0f * tB;
		p_mat[10] = -2.0f * fN;
		p_mat[12] = -(right + left) * rL;
		p_mat[13] = -(top + bottom) * tB;
		p_mat[14] = -(farZ + nearZ) * fN;
		p_mat[15] = 1.0f;
	}

	// Matrice modèle 2D (position + taille + rotation)
	static void MakeModel2D(float p_mat[16],
		float px, float py,         // position
		float sx, float sy,         // scale (taille du sprite)
		float angle = 0.0f)         // rotation en radians
	{
		float c = cosf(angle);
		float s = sinf(angle);

		for (int i = 0; i < 16; ++i) p_mat[i] = 0.0f;

		p_mat[0] = c * sx;
		p_mat[1] = s * sx;
		p_mat[4] = -s * sy;
		p_mat[5] = c * sy;
		p_mat[10] = 1.0f;
		p_mat[12] = px;
		p_mat[13] = py;
		p_mat[15] = 1.0f;
	}

	//static void CheckGLError(const char* p_location)
	//{
	//	GLenum err;
	//	while ((err = glGetError()) != GL_NO_ERROR)
	//	{
	//		const char* msg = "INCONNU";
	//		switch (err)
	//		{
	//		case GL_INVALID_ENUM:      msg = "INVALID_ENUM";      break;
	//		case GL_INVALID_VALUE:     msg = "INVALID_VALUE";     break;
	//		case GL_INVALID_OPERATION: msg = "INVALID_OPERATION"; break;
	//		case GL_OUT_OF_MEMORY:     msg = "OUT_OF_MEMORY";     break;
	//		}
	//		SDL_Log("[GL] Erreur %s à %s", msg, p_location);
	//	}
	//}

	//static void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id,
	//	GLenum severity, GLsizei length,
	//	const GLchar* message, const void* userParam)
	//{
	//	// Ignorer les infos non critiques
	//	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

	//	const char* srcStr = "?";
	//	const char* typeStr = "?";
	//	const char* sevStr = "?";

	//	switch (source) {
	//	case GL_DEBUG_SOURCE_API:             srcStr = "API";        break;
	//	case GL_DEBUG_SOURCE_SHADER_COMPILER: srcStr = "SHADER";     break;
	//	case GL_DEBUG_SOURCE_APPLICATION:     srcStr = "APP";        break;
	//	}
	//	switch (type) {
	//	case GL_DEBUG_TYPE_ERROR:             typeStr = "ERREUR";    break;
	//	case GL_DEBUG_TYPE_PERFORMANCE:       typeStr = "PERF";      break;
	//	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED"; break;
	//	}
	//	switch (severity) {
	//	case GL_DEBUG_SEVERITY_HIGH:   sevStr = "HAUTE";   break;
	//	case GL_DEBUG_SEVERITY_MEDIUM: sevStr = "MOYENNE"; break;
	//	case GL_DEBUG_SEVERITY_LOW:    sevStr = "FAIBLE";  break;
	//	}

	//	SDL_Log("[GL DEBUG] [%s][%s][%s] ID=%u : %s", srcStr, typeStr, sevStr, id, message);
	//}


	//static void EnableGLDebug()
	//{
	//	glEnable(GL_DEBUG_OUTPUT);
	//	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // erreurs synchrones (plus facile à debugger)
	//	glDebugMessageCallback(GLDebugCallback, nullptr);
	//	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
	//		0, nullptr, GL_FALSE); // silence les notifications
	//}

};

#include "Utils.inl"