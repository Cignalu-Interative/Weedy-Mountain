#include "pch.h"
#include "Window.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

Window::~Window()
{
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	if (m_glContext)
	{
		SDL_GL_DeleteContext(m_glContext);
		m_glContext = nullptr;
	}
	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Window::CreateWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_Init/SDL_Init non reussie : %s", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/IMG_Init/IMG_Init non reussie : %s", IMG_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/TTF_Init/TTF_Init non reussie : %s", TTF_GetError());
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/Mix_OpenAudio/Mix_OpenAudio non reussie : %s", Mix_GetError());
		return false;
	}
	Mix_AllocateChannels(24);

	V_Sync();

	m_windowDimension.SetX((int)(m_displayMode.w / SCREEN_SCALE));
	m_windowDimension.SetY((int)(m_displayMode.h / SCREEN_SCALE));

	//Définition de la version d'OpenGL
	// OpenGL 4.5 est une version majeure d'OpenGL qui a été publiée en août 2014.
	// Elle a introduit de nombreuses fonctionnalités et améliorations par rapport aux versions précédentes, notamment :
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_CONTEXT_MAJOR_VERSION non reussie : %s", SDL_GetError());
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_CONTEXT_MINOR_VERSION non reussie : %s", SDL_GetError());
		return false;
	}

	//Définition du profil d'OpenGL
	// Le profil de compatibilité (compatibility profile) est conçu pour être compatible avec les anciennes versions d'OpenGL.
	// Il inclut toutes les fonctionnalités des versions précédentes, y compris les fonctionnalités obsolètes.
	// Ce profil est généralement utilisé pour les applications qui ont besoin de prendre en charge du code OpenGL plus ancien
	// ou qui utilisent des fonctionnalités qui ont été supprimées dans les versions plus récentes.
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_CONTEXT_PROFILE_MASK non reussie : %s", SDL_GetError());
		return false;
	}

	//Définition du contexte de debug pour OpenGL
	// Le contexte de débogage (debug context) est une fonctionnalité d'OpenGL qui permet aux développeurs de recevoir des messages d'erreur
	// et des avertissements détaillés lors du développement d'applications graphiques. Lorsque le contexte de débogage est activé,
	// OpenGL peut fournir des informations sur les erreurs de rendu, les problèmes de performance, les violations de spécifications, etc.
	// Cela peut être extrêmement utile pour identifier et résoudre les problèmes dans le code OpenGL.
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_CONTEXT_FLAGS non reussie : %s", SDL_GetError());
		return false;
	}

	// Activer le double buffering
	// Le double buffering est une technique qui utilise deux tampons pour le rendu :
	// un tampon de dessin (back buffer) et un tampon d'affichage (front buffer). Le rendu est effectué sur le back buffer,
	// tandis que le front buffer est affiché à l'écran. Une fois que le rendu est terminé, les tampons sont échangés,
	// ce qui permet d'éviter les artefacts visuels tels que le tearing.
	if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_DOUBLEBUFFER non reussie : %s", SDL_GetError());
		return false;
	}

	// Activer le multisampling (antialiasing)
	// Le multisampling est une technique d'anti-aliasing qui permet de réduire les effets d'aliasing (bords dentelés) dans les images rendues.
	// En activant le multisampling, vous demandez à OpenGL de prendre plusieurs échantillons par pixel lors du rendu, ce qui peut améliorer la qualité visuelle
	// en lissant les bords des objets.
	// En définissant le nombre de buffers de multisampling à 1, vous indiquez que vous souhaitez utiliser un seul tampon de multisampling pour le rendu.
	// En définissant le nombre d'échantillons de multisampling à 4, vous demandez à OpenGL de prendre 4 échantillons par pixel,
	// ce qui peut fournir une amélioration significative de la qualité visuelle tout en restant performant pour la plupart des applications graphiques.
	// En résumé, ces paramètres de contexte OpenGL sont configurés pour créer un environnement de rendu moderne et performant,
	// avec des fonctionnalités de débogage, un double buffering pour éviter les artefacts visuels, un stencil buffer pour des opérations de masquage avancées,
	// et le multisampling pour améliorer la qualité visuelle en réduisant les effets d'aliasing.
	// Ces paramètres sont couramment utilisés dans les applications graphiques modernes pour garantir une expérience de rendu fluide et de haute qualité.
	if (SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 2) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_MULTISAMPLEBUFFERS non reussie : %s", SDL_GetError());
		return false;
	}

	// Demander un contexte OpenGL accéléré par le matériel
	// En demandant un contexte OpenGL accéléré par le matériel, vous indiquez que vous souhaitez
	// que le rendu soit effectué par la carte graphique plutôt que par le processeur central (CPU).
	// Cela peut améliorer considérablement les performances de rendu, en particulier pour les applications graphiques complexes ou les jeux.
	// En activant cette option, vous permettez à OpenGL d'utiliser les capacités de la carte graphique pour effectuer le rendu,
	// ce qui peut entraîner une expérience de rendu plus fluide et plus rapide, en particulier lorsque vous utilisez des fonctionnalités graphiques
	// avancées ou que vous travaillez avec des scènes 3D complexes.
	// En résumé, en demandant un contexte OpenGL accéléré par le matériel, vous optimisez les performances de rendu en tirant parti des capacités de la carte graphique,
	// ce qui peut être particulièrement bénéfique pour les applications graphiques exigeantes ou les jeux.
	// En activant cette option, vous permettez à OpenGL d'utiliser les capacités de la carte graphique pour effectuer le rendu,
	// ce qui peut entraîner une expérience de rendu plus fluide et plus rapide,
	// en particulier lorsque vous utilisez des fonctionnalités graphiques avancées ou que vous travaillez avec des scènes 3D complexes.
	// En résumé, en demandant un contexte OpenGL accéléré par le matériel,
	// vous optimisez les performances de rendu en tirant parti des capacités de la carte graphique,
	// ce qui peut être particulièrement bénéfique pour les applications graphiques exigeantes ou les jeux.
	if (SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_ACCELERATED_VISUAL non reussie : %s", SDL_GetError());
		return false;
	}

	// En définissant le comportement de libération du contexte OpenGL sur SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH,
	// vous indiquez que vous souhaitez que les commandes de rendu en attente soient exécutées avant que le contexte ne soit libéré.
	// Cela peut être utile pour garantir que toutes les opérations de rendu sont terminées avant que le contexte ne soit détruit,
	// ce qui peut aider à éviter les problèmes de rendu ou les artefacts visuels lors de la fermeture de l'application ou du changement de contexte.
	// En activant cette option, vous permettez à OpenGL de s'assurer que toutes les commandes de rendu en attente sont traitées avant que le contexte ne soit libéré,
	// ce qui peut contribuer à une expérience de rendu plus fluide et plus cohérente, en particulier lors de la fermeture de l'application ou du changement de contexte.
	// En résumé, en définissant le comportement de libération du contexte OpenGL sur SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH,
	// vous garantissez que toutes les commandes de rendu en attente sont exécutées avant que le contexte ne soit libéré,
	// ce qui peut aider à éviter les problèmes de rendu ou les artefacts visuels lors de la fermeture de l'application ou du changement de contexte.
	// Cela contribue à une expérience de rendu plus fluide et plus cohérente pour les utilisateurs.
	// En activant cette option, vous permettez à OpenGL de s'assurer que toutes les commandes de rendu en attente sont traitées avant que le contexte ne soit libéré,
	// ce qui peut contribuer à une expérience de rendu plus fluide et plus cohérente, en particulier lors de la fermeture de l'application ou du changement de contexte.
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH) <0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_CONTEXT_RELEASE_BEHAVIOR non reussie : %s", SDL_GetError());
		return false;
	}

	// En définissant le comportement de contexte sans erreur sur 0, vous indiquez que vous souhaitez que les erreurs OpenGL soient signalées normalement.
	// Cela signifie que si une erreur se produit lors de l'exécution de commandes OpenGL, elle sera signalée et pourra être détectée par votre application.
	// En activant cette option, vous permettez à OpenGL de signaler les erreurs de manière standard,
	// ce qui peut être utile pour le débogage et la gestion des erreurs dans votre application.
	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, 0) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_CONTEXT_NO_ERROR non reussie : %s", SDL_GetError());
		return false;
	}

	// Définition de la taille des tampons de couleur (RGBA)
	if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_RED_SIZE non reussie : %s", SDL_GetError());
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_GREEN_SIZE non reussie : %s", SDL_GetError());
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_BLUE_SIZE non reussie : %s", SDL_GetError());
		return false;
	}
	if (SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8) < 0)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_SetAttribute/SDL_GL_ALPHA_SIZE non reussie : %s", SDL_GetError());
		return false;
	}

	m_window = SDL_CreateWindow("Cignalu Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_windowDimension.GetX(),
		m_windowDimension.GetY(),
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (m_window == NULL)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_CreateWindow/SDL_CreateWindow non reussie : %s", SDL_GetError());
		return false;
	}

	//////////////////////////////////////////

	m_glContext = SDL_GL_CreateContext(m_window);
	if (m_glContext == NULL)
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/SDL_GL_CreateContext/SDL_GL_CreateContext non reussie : %s", SDL_GetError());
		return false;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		SDL_Log("Lib/Window.cpp/CreateWindow/gladLoadGLLoader/gladLoadGLLoader non reussie : %s", SDL_GetError());
		return false;
	}

	glViewport(0, 0, m_windowDimension.GetX(), m_windowDimension.GetY());

	SDL_Log("OpenGL version : %s", glGetString(GL_VERSION));
	SDL_Log("GPU : %s", glGetString(GL_RENDERER));
	SDL_Log("GLSL : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if(m_windowDimension.GetX() < m_displayMode.w && m_windowDimension.GetY() < m_displayMode.h)
	{
		m_isFullScreen = false;
	}
	else
	{
		m_isFullScreen = true;
	}

	SDL_Surface* icon = IMG_Load("../../res/2DPendentEngine/Assets/Textures/IconWindow/CignaluEngineIconeV2.png");

	if (icon != nullptr) {
		SDL_SetWindowIcon(m_window, icon);
		SDL_FreeSurface(icon);
	}
	else {
		SDL_Log("Lib/Window.cpp/CreateWindow/IMG_Load/IMG_Load non reussie : %s", IMG_GetError());
		return false;
	}
	return true;
}

int Window::V_Sync()
{
	if (SDL_GetCurrentDisplayMode(0, &m_displayMode) < 0)
	{
		SDL_Log("Lib/Window.cpp/V_Sync/SDL_GetCurrentDisplayMode/SDL_GetCurrentDisplayMode non reussie : %s", SDL_GetError());
	}
	return m_displayMode.refresh_rate;
}

void Window::BoolFullScreen()
{
	Uint32 flags = SDL_GetWindowFlags(m_window);
	if (m_isFullScreen == true)
	{
		if (SDL_SetWindowFullscreen(m_window, 0) < 0)
		{
			SDL_Log("Lib/Window.cpp/BoolFullScreen/SDL_SetWindowFullscreen/SDL_SetWindowFullscreen 1 non reussie : %s", SDL_GetError());
			return;
		}
		m_isFullScreen = false;
	}
	else
	{
		if (SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0)
		{
			SDL_Log("Lib/Window.cpp/BoolFullScreen/SDL_SetWindowFullscreen/SDL_SetWindowFullscreen 2 non reussie : %s", SDL_GetError());
			return;
		}
		m_isFullScreen = true;
	}
}

void Window::SetWindowSize(Vector2i p_dimention)
{
	SDL_SetWindowSize(m_window, p_dimention.GetX(), p_dimention.GetY());
}

Vector2i Window::GetWindowSize()
{
	SDL_GetWindowSize(m_window, &m_windowDimension.GetX(), &m_windowDimension.GetY());
	Vector2i dimention = { m_windowDimension.GetX(), m_windowDimension.GetY() };
	return dimention;
}

Vector2i Window::GetHalfWindowSize()
{
	Vector2i halfSizewindow = GetWindowSize() / 2;
	return halfSizewindow;
}
