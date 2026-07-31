#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
//Version 1.0.1

// Touche Virtuel
enum KeyboardKeycodeKeys : Uint32
{
    K_UNKNOWN = 0x00000000u, // Aucune touche / inconnue
    K_RETURN = 0x0000000du, // Entrée / Return
    K_ESCAPE = 0x0000001bu, // Échap
    K_BACKSPACE = 0x00000008u, // Retour arrière
    K_TAB = 0x00000009u, // Tabulation
    K_SPACE = 0x00000020u, // Espace
    K_EXCLAIM = 0x00000021u, // !
    K_DBLAPOSTROPHE = 0x00000022u, // "
    K_SUPERSCRIPT_TWO = 0x000000b2u, //²
    K_HASH = 0x00000023u, // #
    K_DOLLAR = 0x00000024u, // $
    K_PERCENT = 0x00000025u, // %
    K_AMPERSAND = 0x00000026u, // &
    K_APOSTROPHE = 0x00000027u, // '
    K_LEFTPAREN = 0x00000028u, // (
    K_RIGHTPAREN = 0x00000029u, // )
    K_ASTERISK = 0x0000002au, // *
    K_PLUS = 0x0000002bu, // +
    K_COMMA = 0x0000002cu, // ,
    K_MINUS = 0x0000002du, // -
    K_PERIOD = 0x0000002eu, // .
    K_SLASH = 0x0000002fu, // /
    K_0 = 0x00000030u, // 0
    K_1 = 0x00000031u, // 1
    K_2 = 0x00000032u, // 2
    K_3 = 0x00000033u, // 3
    K_4 = 0x00000034u, // 4
    K_5 = 0x00000035u, // 5
    K_6 = 0x00000036u, // 6
    K_7 = 0x00000037u, // 7
    K_8 = 0x00000038u, // 8
    K_9 = 0x00000039u, // 9
    K_COLON = 0x0000003au, // :
    K_SEMICOLON = 0x0000003bu, // ;
    K_LESS = 0x0000003cu, // <
    K_EQUALS = 0x0000003du, // =
    K_GREATER = 0x0000003eu, // >
    K_QUESTION = 0x0000003fu, // ?
    K_AT = 0x00000040u, // @
    K_LEFTBRACKET = 0x0000005bu, // [
    K_BACKSLASH = 0x0000005cu, // \
        K_RIGHTBRACKET = 0x0000005du, // ]
    K_CARET = 0x0000005eu, // ^
    K_UNDERSCORE = 0x0000005fu, // _
    K_GRAVE = 0x00000060u, // `
    K_A = 0x00000061u, // a
    K_B = 0x00000062u, // b
    K_C = 0x00000063u, // c
    K_D = 0x00000064u, // d
    K_E = 0x00000065u, // e
    K_F = 0x00000066u, // f
    K_G = 0x00000067u, // g
    K_H = 0x00000068u, // h
    K_I = 0x00000069u, // i
    K_J = 0x0000006au, // j
    K_K = 0x0000006bu, // k
    K_L = 0x0000006cu, // l
    K_M = 0x0000006du, // m
    K_N = 0x0000006eu, // n
    K_O = 0x0000006fu, // o
    K_P = 0x00000070u, // p
    K_Q = 0x00000071u, // q
    K_R = 0x00000072u, // r
    K_S = 0x00000073u, // s
    K_T = 0x00000074u, // t
    K_U = 0x00000075u, // u
    K_V = 0x00000076u, // v
    K_W = 0x00000077u, // w
    K_X = 0x00000078u, // x
    K_Y = 0x00000079u, // y
    K_Z = 0x0000007au, // z
    K_LEFTBRACE = 0x0000007bu, // {
    K_PIPE = 0x0000007cu, // |
    K_RIGHTBRACE = 0x0000007du, // }
    K_TILDE = 0x0000007eu, // ~
    K_DELETE = 0x0000007fu, // Suppr / Delete
    K_PLUSMINUS = 0x000000b1u, // ±

    K_CAPSLOCK = 0x40000039u, // Verr Maj
    K_F1 = 0x4000003au, // F1
    K_F2 = 0x4000003bu, // F2
    K_F3 = 0x4000003cu, // F3
    K_F4 = 0x4000003du, // F4
    K_F5 = 0x4000003eu, // F5
    K_F6 = 0x4000003fu, // F6
    K_F7 = 0x40000040u, // F7
    K_F8 = 0x40000041u, // F8
    K_F9 = 0x40000042u, // F9
    K_F10 = 0x40000043u, // F10
    K_F11 = 0x40000044u, // F11
    K_F12 = 0x40000045u, // F12
    K_PRINTSCREEN = 0x40000046u, // Impr écran
    K_SCROLLLOCK = 0x40000047u, // Arrêt défil
    K_PAUSE = 0x40000048u, // Pause
    K_INSERT = 0x40000049u, // Insertion
    K_HOME = 0x4000004au, // Début / Home
    K_PAGEUP = 0x4000004bu, // Page précédente
    K_END = 0x4000004du, // Fin
    K_PAGEDOWN = 0x4000004eu, // Page suivante
    K_RIGHT = 0x4000004fu, // Flèche droite
    K_LEFT = 0x40000050u, // Flèche gauche
    K_DOWN = 0x40000051u, // Flèche bas
    K_UP = 0x40000052u, // Flèche haut
    K_NUMLOCKCLEAR = 0x40000053u, // Verr Num
    K_KP_DIVIDE = 0x40000054u, // Pavé numérique /
    K_KP_MULTIPLY = 0x40000055u, // Pavé numérique *
    K_KP_MINUS = 0x40000056u, // Pavé numérique -
    K_KP_PLUS = 0x40000057u, // Pavé numérique +
    K_KP_ENTER = 0x40000058u, // Entrée du pavé numérique
    K_KP_1 = 0x40000059u, // Pavé numérique 1
    K_KP_2 = 0x4000005au, // Pavé numérique 2
    K_KP_3 = 0x4000005bu, // Pavé numérique 3
    K_KP_4 = 0x4000005cu, // Pavé numérique 4
    K_KP_5 = 0x4000005du, // Pavé numérique 5
    K_KP_6 = 0x4000005eu, // Pavé numérique 6
    K_KP_7 = 0x4000005fu, // Pavé numérique 7
    K_KP_8 = 0x40000060u, // Pavé numérique 8
    K_KP_9 = 0x40000061u, // Pavé numérique 9
    K_KP_0 = 0x40000062u, // Pavé numérique 0
    K_KP_PERIOD = 0x40000063u, // Pavé numérique .
    K_APPLICATION = 0x40000065u, // Menu contextuel / Application
    K_POWER = 0x40000066u, // Touche alimentation
    K_KP_EQUALS = 0x40000067u, // Pavé numérique =
    K_F13 = 0x40000068u, // F13
    K_F14 = 0x40000069u, // F14
    K_F15 = 0x4000006au, // F15
    K_F16 = 0x4000006bu, // F16
    K_F17 = 0x4000006cu, // F17
    K_F18 = 0x4000006du, // F18
    K_F19 = 0x4000006eu, // F19
    K_F20 = 0x4000006fu, // F20
    K_F21 = 0x40000070u, // F21
    K_F22 = 0x40000071u, // F22
    K_F23 = 0x40000072u, // F23
    K_F24 = 0x40000073u, // F24
    K_EXECUTE = 0x40000074u, // Exécuter
    K_HELP = 0x40000075u, // Aide
    K_MENU = 0x40000076u, // Menu
    K_SELECT = 0x40000077u, // Sélectionner
    K_STOP = 0x40000078u, // Stop
    K_AGAIN = 0x40000079u, // Refaire
    K_UNDO = 0x4000007au, // Annuler
    K_CUT = 0x4000007bu, // Couper
    K_COPY = 0x4000007cu, // Copier
    K_PASTE = 0x4000007du, // Coller
    K_FIND = 0x4000007eu, // Rechercher
    K_MUTE = 0x4000007fu, // Muet
    K_VOLUMEUP = 0x40000080u, // Volume +
    K_VOLUMEDOWN = 0x40000081u, // Volume -
    K_KP_COMMA = 0x40000085u, // Pavé numérique ,
    K_KP_EQUALSAS400 = 0x40000086u, // Pavé numérique = (AS400)
    K_ALTERASE = 0x40000099u, // Alt + Effacement
    K_SYSREQ = 0x4000009au, // SysReq
    K_CANCEL = 0x4000009bu, // Annuler
    K_CLEAR = 0x4000009cu, // Effacer
    K_PRIOR = 0x4000009du, // Page précédente
    K_RETURN2 = 0x4000009eu, // Entrée 2
    K_SEPARATOR = 0x4000009fu, // Séparateur
    K_OUT = 0x400000a0u, // Sortie
    K_OPER = 0x400000a1u, // Opérateur
    K_CLEARAGAIN = 0x400000a2u, // Effacer encore
    K_CRSEL = 0x400000a3u, // Select CR
    K_EXSEL = 0x400000a4u, // Select EX
    K_KP_00 = 0x400000b0u, // Pavé numérique 00
    K_KP_000 = 0x400000b1u, // Pavé numérique 000
    K_THOUSANDSSEPARATOR = 0x400000b2u, // Séparateur des milliers
    K_DECIMALSEPARATOR = 0x400000b3u, // Séparateur décimal
    K_CURRENCYUNIT = 0x400000b4u, // Unité monétaire
    K_CURRENCYSUBUNIT = 0x400000b5u, // Sous-unité monétaire
    K_KP_LEFTPAREN = 0x400000b6u, // Pavé numérique (
    K_KP_RIGHTPAREN = 0x400000b7u, // Pavé numérique )
    K_KP_LEFTBRACE = 0x400000b8u, // Pavé numérique {
    K_KP_RIGHTBRACE = 0x400000b9u, // Pavé numérique }
    K_KP_TAB = 0x400000bau, // Tab du pavé numérique
    K_KP_BACKSPACE = 0x400000bbu, // Retour arrière du pavé numérique
    K_KP_A = 0x400000bcu, // Pavé numérique A
    K_KP_B = 0x400000bdu, // Pavé numérique B
    K_KP_C = 0x400000beu, // Pavé numérique C
    K_KP_D = 0x400000bfu, // Pavé numérique D
    K_KP_E = 0x400000c0u, // Pavé numérique E
    K_KP_F = 0x400000c1u, // Pavé numérique F
    K_KP_XOR = 0x400000c2u, // Pavé numérique XOR
    K_KP_POWER = 0x400000c3u, // Pavé numérique ^
    K_KP_PERCENT = 0x400000c4u, // Pavé numérique %
    K_KP_LESS = 0x400000c5u, // Pavé numérique <
    K_KP_GREATER = 0x400000c6u, // Pavé numérique >
    K_KP_AMPERSAND = 0x400000c7u, // Pavé numérique &
    K_KP_DBLAMPERSAND = 0x400000c8u, // Pavé numérique &&
    K_KP_VERTICALBAR = 0x400000c9u, // Pavé numérique |
    K_KP_DBLVERTICALBAR = 0x400000cau, // Pavé numérique ||
    K_KP_COLON = 0x400000cbu, // Pavé numérique :
    K_KP_HASH = 0x400000ccu, // Pavé numérique #
    K_KP_SPACE = 0x400000cdu, // Pavé numérique espace
    K_KP_AT = 0x400000ceu, // Pavé numérique @
    K_KP_EXCLAM = 0x400000cfu, // Pavé numérique !
    K_KP_MEMSTORE = 0x400000d0u, // Pavé numérique stocker en mémoire
    K_KP_MEMRECALL = 0x400000d1u, // Pavé numérique rappeler la mémoire
    K_KP_MEMCLEAR = 0x400000d2u, // Pavé numérique effacer la mémoire
    K_KP_MEMADD = 0x400000d3u, // Pavé numérique ajouter à la mémoire
    K_KP_MEMSUBTRACT = 0x400000d4u, // Pavé numérique soustraire de la mémoire
    K_KP_MEMMULTIPLY = 0x400000d5u, // Pavé numérique multiplier la mémoire
    K_KP_MEMDIVIDE = 0x400000d6u, // Pavé numérique diviser la mémoire
    K_KP_PLUSMINUS = 0x400000d7u, // Pavé numérique +/- 
    K_KP_CLEAR = 0x400000d8u, // Pavé numérique effacer
    K_KP_CLEARENTRY = 0x400000d9u, // Pavé numérique effacer l'entrée
    K_KP_BINARY = 0x400000dau, // Pavé numérique binaire
    K_KP_OCTAL = 0x400000dbu, // Pavé numérique octal
    K_KP_DECIMAL = 0x400000dcu, // Pavé numérique décimal
    K_KP_HEXADECIMAL = 0x400000ddu, // Pavé numérique hexadécimal
    K_LCTRL = 0x400000e0u, // Ctrl gauche
    K_LSHIFT = 0x400000e1u, // Shift gauche
    K_LALT = 0x400000e2u, // Alt gauche
    K_LGUI = 0x400000e3u, // Touche Windows / Cmd gauche
    K_RCTRL = 0x400000e4u, // Ctrl droite
    K_RSHIFT = 0x400000e5u, // Shift droite
    K_RALT = 0x400000e6u, // Alt droite
    K_RGUI = 0x400000e7u, // Touche Windows / Cmd droite
    K_MODE = 0x40000101u, // Mode
    K_SLEEP = 0x40000102u, // Veille
    K_WAKE = 0x40000103u, // Réveil
    K_CHANNEL_INCREMENT = 0x40000104u, // Canal suivant
    K_CHANNEL_DECREMENT = 0x40000105u, // Canal précédent
    K_MEDIA_PLAY = 0x40000106u, // Lecture média
    K_MEDIA_PAUSE = 0x40000107u, // Pause média
    K_MEDIA_RECORD = 0x40000108u, // Enregistrement média
    K_MEDIA_FAST_FORWARD = 0x40000109u, // Avance rapide média
    K_MEDIA_REWIND = 0x4000010au, // Retour rapide média
    K_MEDIA_NEXT_TRACK = 0x4000010bu, // Piste suivante
    K_MEDIA_PREVIOUS_TRACK = 0x4000010cu, // Piste précédente
    K_MEDIA_STOP = 0x4000010du, // Stop média
    K_MEDIA_EJECT = 0x4000010eu, // Éjecter
    K_MEDIA_PLAY_PAUSE = 0x4000010fu, // Lecture / pause média
    K_MEDIA_SELECT = 0x40000110u, // Sélection média
    K_AC_NEW = 0x40000111u, // Nouveau
    K_AC_OPEN = 0x40000112u, // Ouvrir
    K_AC_CLOSE = 0x40000113u, // Fermer
    K_AC_EXIT = 0x40000114u, // Quitter
    K_AC_SAVE = 0x40000115u, // Sauvegarder
    K_AC_PRINT = 0x40000116u, // Imprimer
    K_AC_PROPERTIES = 0x40000117u, // Propriétés
    K_AC_SEARCH = 0x40000118u, // Rechercher
    K_AC_HOME = 0x40000119u, // Accueil
    K_AC_BACK = 0x4000011au, // Retour
    K_AC_FORWARD = 0x4000011bu, // Avancer
    K_AC_STOP = 0x4000011cu, // Stop
    K_AC_REFRESH = 0x4000011du, // Rafraîchir
    K_AC_BOOKMARKS = 0x4000011eu, // Favoris
    K_SOFTLEFT = 0x4000011fu, // Soft left
    K_SOFTRIGHT = 0x40000120u, // Soft right
    K_CALL = 0x40000121u, // Appel
    K_ENDCALL = 0x40000122u, // Fin d'appel

    K_LEFT_TAB = 0x20000001u, // Tabulation gauche
    K_LEVEL5_SHIFT = 0x20000002u, // Shift niveau 5
    K_MULTI_KEY_COMPOSE = 0x20000003u, // Compose multi-touche
    K_LMETA = 0x20000004u, // Meta gauche
    K_RMETA = 0x20000005u, // Meta droite
    K_LHYPER = 0x20000006u, // Hyper gauche
    K_RHYPER = 0x20000007u // Hyper droite
};

// Touche Modificateur
enum class KeyboardKeymodKeys : Uint16
{
    K_NONE = 0x0000u, // Aucun modificateur
    K_LSHIFT = 0x0001u, // Shift gauche
    K_RSHIFT = 0x0002u, // Shift droit
    K_LEVEL5 = 0x0004u, // Level 5 Shift
    K_LCTRL = 0x0040u, // Ctrl gauche
    K_RCTRL = 0x0080u, // Ctrl droit
    K_LALT = 0x0100u, // Alt gauche
    K_RALT = 0x0200u, // Alt droit
    K_LGUI = 0x0400u, // Touche Windows / Cmd gauche
    K_RGUI = 0x0800u, // Touche Windows / Cmd droite
    K_NUM = 0x1000u, // Verr Num
    K_CAPS = 0x2000u, // Verr Maj
    K_MODE = 0x4000u, // AltGr / mode
    K_SCROLL = 0x8000u, // Verr Défil

    K_CTRL = K_LCTRL | K_RCTRL,   // N’importe quel Ctrl
    K_SHIFT = K_LSHIFT | K_RSHIFT, // N’importe quel Shift
    K_ALT = K_LALT | K_RALT,     // N’importe quel Alt
    K_GUI = K_LGUI | K_RGUI      // N’importe quelle touche Windows / Cmd
};

// Touche Physique
enum KeyboardScancodeKeys
{
	// Touche inconnue
    SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
     /* @{ */

	// Touche A
    SCANCODE_A = 4,
	// Touche B
    SCANCODE_B = 5,
	// Touche C
    SCANCODE_C = 6,
	// Touche D
    SCANCODE_D = 7,
    // Touche E
    SCANCODE_E = 8,
	// Touche F
    SCANCODE_F = 9,
	// Touche G
    SCANCODE_G = 10,
	// Touche H
    SCANCODE_H = 11,
	// Touche I
    SCANCODE_I = 12,
	// Touche J
    SCANCODE_J = 13,
	// Touche K
    SCANCODE_K = 14,
	// Touche L
    SCANCODE_L = 15,
	// Touche M
    SCANCODE_M = 16,
	// Touche N
    SCANCODE_N = 17,
	// Touche O
    SCANCODE_O = 18,
	// Touche P
    SCANCODE_P = 19,
	// Touche Q
    SCANCODE_Q = 20,
	// Touche R
    SCANCODE_R = 21,
	// Touche S
    SCANCODE_S = 22,
	// Touche T
    SCANCODE_T = 23,
	// Touche U
    SCANCODE_U = 24,
	// Touche V
    SCANCODE_V = 25,
	// Touche W
    SCANCODE_W = 26,
	// Touche X
    SCANCODE_X = 27,
	// Touche Y
    SCANCODE_Y = 28,
	// Touche Z
    SCANCODE_Z = 29,

	// Touche du chiffre 1
    SCANCODE_1 = 30,
	// Touche du chiffre 2
    SCANCODE_2 = 31,
	// Touche du chiffre 3
    SCANCODE_3 = 32,
	// Touche du chiffre 4
    SCANCODE_4 = 33,
	// Touche du chiffre 5
    SCANCODE_5 = 34,
	// Touche du chiffre 6
    SCANCODE_6 = 35,
	// Touche du chiffre 7
    SCANCODE_7 = 36,
	// Touche du chiffre 8
    SCANCODE_8 = 37,
	// Touche du chiffre 9
    SCANCODE_9 = 38,
	// Touche du chiffre 0
    SCANCODE_0 = 39,

	// Touche Entrée
    SCANCODE_RETURN = 40,
	// Touche d'échappement
    SCANCODE_ESCAPE = 41,
	// Touche de retour arrière
    SCANCODE_BACKSPACE = 42,
	// Touche de tabulation
    SCANCODE_TAB = 43,
	// Touche d'espace
    SCANCODE_SPACE = 44,

	// Touche de moins
    SCANCODE_MINUS = 45,
	// Touche d'égalité
    SCANCODE_EQUALS = 46,
	// Touche de crochet gauche
    SCANCODE_LEFTBRACKET = 47,
	// Touche de crochet droit
    SCANCODE_RIGHTBRACKET = 48,

		// Touche de barre oblique inversée
    SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                   *   key on ISO keyboards and at the right end
                                   *   of the QWERTY row on ANSI keyboards.
                                   *   Produces REVERSE SOLIDUS (backslash) and
                                   *   VERTICAL LINE in a US layout, REVERSE
                                   *   SOLIDUS and VERTICAL LINE in a UK Mac
                                   *   layout, NUMBER SIGN and TILDE in a UK
                                   *   Windows layout, DOLLAR SIGN and POUND SIGN
                                   *   in a Swiss German layout, NUMBER SIGN and
                                   *   APOSTROPHE in a German layout, GRAVE
                                   *   ACCENT and POUND SIGN in a French Mac
                                   *   layout, and ASTERISK and MICRO SIGN in a
                                   *   French Windows layout.
                                   */
								   // Touche de barre oblique inversée
    SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                   *   instead of 49 for the same key, but all
                                   *   OSes I've seen treat the two codes
                                   *   identically. So, as an implementor, unless
                                   *   your keyboard generates both of those
                                   *   codes and your OS treats them differently,
                                   *   you should generateSCANCODE_BACKSLASH
                                   *   instead of this code. As a user, you
                                   *   should not rely on this code because SDL
                                   *   will never generate it with most (all?)
                                   *   keyboards.
                                   */
								   // Touche de point-virgule
    SCANCODE_SEMICOLON = 51,
		// Touche d'apostrophe
    SCANCODE_APOSTROPHE = 52,
		// Touche de grave
    SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                               *   and ISO keyboards). Produces GRAVE ACCENT and
                               *   TILDE in a US Windows layout and in US and UK
                               *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                               *   and NOT SIGN in a UK Windows layout, SECTION
                               *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                               *   layouts on ISO keyboards, SECTION SIGN and
                               *   DEGREE SIGN in a Swiss German layout (Mac:
                               *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                               *   DEGREE SIGN in a German layout (Mac: only on
                               *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                               *   French Windows layout, COMMERCIAL AT and
                               *   NUMBER SIGN in a French Mac layout on ISO
                               *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                               *   SIGN in a Swiss German, German, or French Mac
                               *   layout on ANSI keyboards.
                               */
							   // Touche de virgule
    SCANCODE_COMMA = 54,
		// Touche de point
    SCANCODE_PERIOD = 55,
		// Touche de barre oblique
    SCANCODE_SLASH = 56,
		// Touche de verrouillage des majuscules
    SCANCODE_CAPSLOCK = 57,
		// Touches de fonction F1
    SCANCODE_F1 = 58,
	// Touches de fonction F2
    SCANCODE_F2 = 59,
	// Touches de fonction F3
    SCANCODE_F3 = 60,
	// Touches de fonction F4
    SCANCODE_F4 = 61,
	// Touches de fonction F5
    SCANCODE_F5 = 62,
	// Touches de fonction F6
    SCANCODE_F6 = 63,
	// Touches de fonction F7
    SCANCODE_F7 = 64,
	// Touches de fonction F8
    SCANCODE_F8 = 65,
	// Touches de fonction F9
    SCANCODE_F9 = 66,
	// Touches de fonction F10
    SCANCODE_F10 = 67,
	// Touches de fonction F11
    SCANCODE_F11 = 68,
	// Touches de fonction F12
    SCANCODE_F12 = 69,

	// Touche d'impression écran
    SCANCODE_PRINTSCREEN = 70,
	// Touche de verrouillage du défilement
    SCANCODE_SCROLLLOCK = 71,
	// Touche de pause
    SCANCODE_PAUSE = 72,
	// Touche d'insertion
    SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                    does send code 73, not 117) */
									// Touche de suppression
    SCANCODE_HOME = 74,
	// Touche de page précédente
    SCANCODE_PAGEUP = 75,
	// Touche de suppression
    SCANCODE_DELETE = 76,
	// Touche de fin
    SCANCODE_END = 77,
	// Touche de page suivante
    SCANCODE_PAGEDOWN = 78,
		// Touches de direction
    SCANCODE_RIGHT = 79,
		// Touches de direction
    SCANCODE_LEFT = 80,
		// Touches de direction
    SCANCODE_DOWN = 81,
		// Touches de direction
    SCANCODE_UP = 82,
		// Touche de verrouillage numérique
    SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                      */
									  // Touches du pavé numérique /
    SCANCODE_KP_DIVIDE = 84,
		// Touches du pavé numérique *
    SCANCODE_KP_MULTIPLY = 85,
		// Touches du pavé numérique -
    SCANCODE_KP_MINUS = 86,
		// Touches du pavé numérique +
    SCANCODE_KP_PLUS = 87,
		// Touches du pavé numérique Entrée
    SCANCODE_KP_ENTER = 88,
		// Touches du pavé numérique 1
    SCANCODE_KP_1 = 89,
		// Touches du pavé numérique 2
    SCANCODE_KP_2 = 90,
		// Touches du pavé numérique 3
    SCANCODE_KP_3 = 91,
	// Touches du pavé numérique 4
    SCANCODE_KP_4 = 92,
	// Touches du pavé numérique 5
    SCANCODE_KP_5 = 93,
	// Touches du pavé numérique 6
    SCANCODE_KP_6 = 94,
		// Touches du pavé numérique 7
    SCANCODE_KP_7 = 95,
		// Touches du pavé numérique 8
    SCANCODE_KP_8 = 96,
		// Touches du pavé numérique 9
    SCANCODE_KP_9 = 97,
		// Touches du pavé numérique 0
    SCANCODE_KP_0 = 98,
		// Touches du pavé numérique .
    SCANCODE_KP_PERIOD = 99,

		// Touche de barre oblique inversée non américaine
    SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                         *   keyboards have over ANSI ones,
                                         *   located between left shift and Y.
                                         *   Produces GRAVE ACCENT and TILDE in a
                                         *   US or UK Mac layout, REVERSE SOLIDUS
                                         *   (backslash) and VERTICAL LINE in a
                                         *   US or UK Windows layout, and
                                         *   LESS-THAN SIGN and GREATER-THAN SIGN
                                         *   in a Swiss German, German, or French
                                         *   layout. */
										 // Touche d'application
    SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
	// Touche de puissance
    SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                                *   not a physical key - but some Mac keyboards
                                *   do have a power key. */
								// Touches du pavé numérique =
    SCANCODE_KP_EQUALS = 103,
	// Touches de fonction F13
    SCANCODE_F13 = 104,
	// Touches de fonction F14
    SCANCODE_F14 = 105,
	// Touches de fonction F15
    SCANCODE_F15 = 106,
	// Touches de fonction F16
    SCANCODE_F16 = 107,
	//  Touches de fonction F17
    SCANCODE_F17 = 108,
	// Touches de fonction F18
    SCANCODE_F18 = 109,
	// Touches de fonction F19
    SCANCODE_F19 = 110,
	// Touches de fonction F20
    SCANCODE_F20 = 111,
	// Touches de fonction F21
    SCANCODE_F21 = 112,
	// Touches de fonction F22
    SCANCODE_F22 = 113,
		// Touches de fonction F23
    SCANCODE_F23 = 114,
		// Touches de fonction F24
    SCANCODE_F24 = 115,
		// Touche d'exécution
    SCANCODE_EXECUTE = 116,
		// Touche d'aide
    SCANCODE_HELP = 117,    /**< AL Integrated Help Center */
	// Touche de menu
    SCANCODE_MENU = 118,    /**< Menu (show menu) */
	// Touche de sélection
    SCANCODE_SELECT = 119,
	// Touche d'arrêt
    SCANCODE_STOP = 120,    /**< AC Stop */
	// Touche de redémarrage
    SCANCODE_AGAIN = 121,   /**< AC Redo/Repeat */
	// Touche d'annulation
    SCANCODE_UNDO = 122,    /**< AC Undo */
	// Touche de coupe
    SCANCODE_CUT = 123,     /**< AC Cut */
	// Touche de copie
    SCANCODE_COPY = 124,    /**< AC Copy */
	// Touche de coller
    SCANCODE_PASTE = 125,   /**< AC Paste */
	// Touche de trouver
    SCANCODE_FIND = 126,    /**< AC Find */
	// Touche de mute
    SCANCODE_MUTE = 127,
		// Touches de volume
    SCANCODE_VOLUMEUP = 128,
		// Touches de volume
    SCANCODE_VOLUMEDOWN = 129,
    /* not sure whether there's a reason to enable these */
    /*    SCANCODE_LOCKINGCAPSLOCK = 130,  */
    /*    SCANCODE_LOCKINGNUMLOCK = 131, */
    /*    SCANCODE_LOCKINGSCROLLLOCK = 132, */
		// Touches du pavé numérique ,
    SCANCODE_KP_COMMA = 133,
		// Touches du pavé numérique =
    SCANCODE_KP_EQUALSAS400 = 134,

    // Touches internationales
    SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                             footnotes in USB doc */
											 // Touches internationales
    SCANCODE_INTERNATIONAL2 = 136,
		// Touches internationales
    SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
		// Touches internationales
    SCANCODE_INTERNATIONAL4 = 138,
		// Touches internationales
    SCANCODE_INTERNATIONAL5 = 139,
		// Touches internationales
    SCANCODE_INTERNATIONAL6 = 140,
		// Touches internationales
    SCANCODE_INTERNATIONAL7 = 141,
		// Touches internationales
    SCANCODE_INTERNATIONAL8 = 142,
		// Touches internationales
    SCANCODE_INTERNATIONAL9 = 143,
		// Touches de langue
    SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
	// Touches de langue
    SCANCODE_LANG2 = 145, /**< Hanja conversion */
		// Touches de langue
    SCANCODE_LANG3 = 146, /**< Katakana */
		// Touches de langue
    SCANCODE_LANG4 = 147, /**< Hiragana */
		// Touches de langue
    SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
		// Touches de langue
    SCANCODE_LANG6 = 149, /**< reserved */
		// Touches de langue
    SCANCODE_LANG7 = 150, /**< reserved */
		// Touches de langue
    SCANCODE_LANG8 = 151, /**< reserved */
		// Touches de langue
    SCANCODE_LANG9 = 152, /**< reserved */

	// Touche d'effacement alternatif
    SCANCODE_ALTERASE = 153,    /**< Erase-Eaze */
	// Touche de demande système
    SCANCODE_SYSREQ = 154,
	// Touche d'annulation alternative
    SCANCODE_CANCEL = 155,      /**< AC Cancel */
	// Touche d'effacement
    SCANCODE_CLEAR = 156,
	// Touche de page précédente alternative
    SCANCODE_PRIOR = 157,
	// Touche d'entrée alternative
    SCANCODE_RETURN2 = 158,
	// Touche de séparation
    SCANCODE_SEPARATOR = 159,
	// Touche de sortie
    SCANCODE_OUT = 160,
	// Touche de opérateur
    SCANCODE_OPER = 161,
	// Touche de Clear/Again
    SCANCODE_CLEARAGAIN = 162,
	// Touche de CrSel
    SCANCODE_CRSEL = 163,
	// Touche de ExSel
    SCANCODE_EXSEL = 164,

	// Touches du pavé numérique 00
    SCANCODE_KP_00 = 176,
		// Touches du pavé numérique 000
    SCANCODE_KP_000 = 177,
		// Touche de séparateur de milliers
    SCANCODE_THOUSANDSSEPARATOR = 178,
		// Touche de séparateur décimal
    SCANCODE_DECIMALSEPARATOR = 179,
		// Touche de séparateur de monétaire
    SCANCODE_CURRENCYUNIT = 180,
		// Touche de séparateur de monétaire
    SCANCODE_CURRENCYSUBUNIT = 181,
		// Touches du pavé numérique (
    SCANCODE_KP_LEFTPAREN = 182,
		// Touches du pavé numérique )
    SCANCODE_KP_RIGHTPAREN = 183,
		// Touches du pavé numérique {
    SCANCODE_KP_LEFTBRACE = 184,
		// Touches du pavé numérique }
    SCANCODE_KP_RIGHTBRACE = 185,
		// Touches du pavé numérique Tab
    SCANCODE_KP_TAB = 186,
		// Touches du pavé numérique Backspace
    SCANCODE_KP_BACKSPACE = 187,
		// Touches du pavé numérique A
    SCANCODE_KP_A = 188,
		// Touches du pavé numérique B
    SCANCODE_KP_B = 189,
		// Touches du pavé numérique C
    SCANCODE_KP_C = 190,
		// Touches du pavé numérique D
    SCANCODE_KP_D = 191,
		// Touches du pavé numérique E
    SCANCODE_KP_E = 192,
		// Touches du pavé numérique F
    SCANCODE_KP_F = 193,
		// Touches du pavé numérique XOR
    SCANCODE_KP_XOR = 194,
	// Touches du pavé numérique ^
    SCANCODE_KP_POWER = 195,
		// Touches du pavé numérique %
    SCANCODE_KP_PERCENT = 196,
		// Touches du pavé numérique <
    SCANCODE_KP_LESS = 197,
		// Touches du pavé numérique >
    SCANCODE_KP_GREATER = 198,
		// Touches du pavé numérique &
    SCANCODE_KP_AMPERSAND = 199,
		// Touches du pavé numérique &&
    SCANCODE_KP_DBLAMPERSAND = 200,
		// Touches du pavé numérique |
    SCANCODE_KP_VERTICALBAR = 201,
		// Touches du pavé numérique ||
    SCANCODE_KP_DBLVERTICALBAR = 202,
		// Touches du pavé numérique :
    SCANCODE_KP_COLON = 203,
		// Touches du pavé numérique #
    SCANCODE_KP_HASH = 204,
		// Touches du pavé numérique espace
    SCANCODE_KP_SPACE = 205,
		// Touches du pavé numérique @
    SCANCODE_KP_AT = 206,
		// Touches du pavé numérique !
    SCANCODE_KP_EXCLAM = 207,
		// Touches du pavé numérique mémoire store
    SCANCODE_KP_MEMSTORE = 208,
		// Touches du pavé numérique mémoire recall
    SCANCODE_KP_MEMRECALL = 209,
		// Touches du pavé numérique mémoire clear
    SCANCODE_KP_MEMCLEAR = 210,
		// Touches du pavé numérique mémoire addition
    SCANCODE_KP_MEMADD = 211,
		// Touche de soustraction de mémoire du pavé numérique
    SCANCODE_KP_MEMSUBTRACT = 212,
		// Touche de multiplication de mémoire du pavé numérique
    SCANCODE_KP_MEMMULTIPLY = 213,
	// Touche de division de mémoire du pavé numérique
    SCANCODE_KP_MEMDIVIDE = 214,
	// Touches du pavé numérique +/- du pavé numérique
    SCANCODE_KP_PLUSMINUS = 215,
	// Touches du pavé numérique Clear du pavé numérique
    SCANCODE_KP_CLEAR = 216,
	// Touches du pavé numérique Clear Entry du pavé numérique
    SCANCODE_KP_CLEARENTRY = 217,
	// Touches du pavé numérique Binary du pavé numérique
    SCANCODE_KP_BINARY = 218,
	// Touches du pavé numérique Octal du pavé numérique
    SCANCODE_KP_OCTAL = 219,
	// Touches du pavé numérique Décimal du pavé numérique
    SCANCODE_KP_DECIMAL = 220,
	// Touches du pavé numérique Hexadecimal du pavé numérique
    SCANCODE_KP_HEXADECIMAL = 221,

		// Touche de contrôle gauche
    SCANCODE_LCTRL = 224,
		// Touche de majuscule gauche
    SCANCODE_LSHIFT = 225,
		// Touche d'altération gauche
    SCANCODE_LALT = 226, /**< alt, option */
		// Touche de logo gauche
    SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
	// Touche de contrôle droite
    SCANCODE_RCTRL = 228,
	// Touche de majuscule droite
    SCANCODE_RSHIFT = 229,
	// Touche d'altération droite
    SCANCODE_RALT = 230, /**< alt gr, option */
	// Touche de logo droite
    SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

		// Touche de mode
    SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                  *   by any of the above, but since there's a
                                  *   special KMOD_MODE for it I'm adding it here
                                  */

                                  /* @} *//* Usage page 0x07 */

                                  /**
                                   *  \name Usage page 0x0C
                                   *
                                   *  These values are mapped from usage page 0x0C (USB consumer page).
                                   *  See https://usb.org/sites/default/files/hut1_2.pdf
                                   *
                                   *  There are way more keys in the spec than we can represent in the
                                   *  current scancode range, so pick the ones that commonly come up in
                                   *  real world usage.
                                   */
                                   /* @{ */

		// Touches de média
    SCANCODE_AUDIONEXT = 258,
		// Touches de média
    SCANCODE_AUDIOPREV = 259,
		// Touches de média
    SCANCODE_AUDIOSTOP = 260,
		// Touches de média
    SCANCODE_AUDIOPLAY = 261,
		// Touches de média
    SCANCODE_AUDIOMUTE = 262,
		// Touches de média
    SCANCODE_MEDIASELECT = 263,
		// Touches de média
    SCANCODE_WWW = 264,             /**< AL Internet Browser */
		// Touches de média
    SCANCODE_MAIL = 265,
		// Touches de média
    SCANCODE_CALCULATOR = 266,      /**< AL Calculator */
		// Touches de média
    SCANCODE_COMPUTER = 267,
		// Touches de média
    SCANCODE_AC_SEARCH = 268,       /**< AC Search */
		// Touches de média
    SCANCODE_AC_HOME = 269,         /**< AC Home */
		// Touches de média
    SCANCODE_AC_BACK = 270,         /**< AC Back */
		// Touches de média
    SCANCODE_AC_FORWARD = 271,      /**< AC Forward */
		// Touches de média
    SCANCODE_AC_STOP = 272,         /**< AC Stop */
		// Touches de média
    SCANCODE_AC_REFRESH = 273,      /**< AC Refresh */
		// Touches de média
    SCANCODE_AC_BOOKMARKS = 274,    /**< AC Bookmarks */

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
     /* @{ */

		// Touches de luminosité
    SCANCODE_BRIGHTNESSDOWN = 275,
		// Touches de luminosité
    SCANCODE_BRIGHTNESSUP = 276,
		// Touches de luminosité
    SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                            switch, video mode switch */
											// Touches de luminosité
    SCANCODE_KBDILLUMTOGGLE = 278,
	// Touches de luminosité
    SCANCODE_KBDILLUMDOWN = 279,
	// Touches de luminosité
    SCANCODE_KBDILLUMUP = 280,
	// Touches de luminosité
    SCANCODE_EJECT = 281,
	// Touches de luminosité
    SCANCODE_SLEEP = 282,           /**< SC System Sleep */

		// Touches de média
    SCANCODE_APP1 = 283,
	// Touches de média
    SCANCODE_APP2 = 284,

    /* @} *//* Walther keys */

    /**
     *  \name Usage page 0x0C (additional media keys)
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
     /* @{ */

		// Touches de média
    SCANCODE_AUDIOREWIND = 285,
	// Touches de média
    SCANCODE_AUDIOFASTFORWARD = 286,

    /* @} *//* Usage page 0x0C (additional media keys) */

    /**
     *  \name Mobile keys
     *
     *  These are values that are often used on mobile phones.
     */
     /* @{ */

		// Touches de média
    SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                       used as a multi-function feature key for selecting
                                       a software defined function shown on the bottom left
                                       of the display. */
									   // Touches de média
    SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                        used as a multi-function feature key for selecting
                                        a software defined function shown on the bottom right
                                        of the display. */
										// Touches de média
    SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
		// Touches de média
    SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */

    /* @} *//* Mobile keys */

    /* Add any other keys here. */
		// Touche de scancode maximale
    NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

// Bouton de manette
enum ControllerButton : uint8_t
{
	// Bouton de manette invalide
    CONTROLLER_BUTTON_INVALID = -1,
	// Bouton de manette A
    CONTROLLER_BUTTON_A,
	// Bouton de manette B
    CONTROLLER_BUTTON_B,
	// Bouton de manette X
    CONTROLLER_BUTTON_X,
	// Bouton de manette Y
    CONTROLLER_BUTTON_Y,
	// Bouton de manette BACK
    CONTROLLER_BUTTON_BACK,
	// Bouton de manette GUIDE
    CONTROLLER_BUTTON_GUIDE,
	// Bouton de manette START
    CONTROLLER_BUTTON_START,
	// Bouton de manette gauche
    CONTROLLER_BUTTON_LEFTSTICK,
	// Bouton de manette droit
    CONTROLLER_BUTTON_RIGHTSTICK,
	// Bouton de manette d'épaule gauche
    CONTROLLER_BUTTON_LEFTSHOULDER,
	// Bouton de manette d'épaule droite
    CONTROLLER_BUTTON_RIGHTSHOULDER,
	// Bouton de manette de direction haut
    CONTROLLER_BUTTON_DPAD_UP,
	// Bouton de manette de direction bas
    CONTROLLER_BUTTON_DPAD_DOWN,
	// Bouton de manette de direction gauche
    CONTROLLER_BUTTON_DPAD_LEFT,
	// Bouton de manette de direction droite
    CONTROLLER_BUTTON_DPAD_RIGHT,
	// Bouton de manette de menu
    CONTROLLER_BUTTON_MISC1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
	// Bouton de manette de menu
    CONTROLLER_BUTTON_PADDLE1,  /* Xbox Elite paddle P1 (upper left, facing the back) */
	// Bouton de manette de menu
    CONTROLLER_BUTTON_PADDLE2,  /* Xbox Elite paddle P3 (upper right, facing the back) */
	// Bouton de manette de menu
    CONTROLLER_BUTTON_PADDLE3,  /* Xbox Elite paddle P2 (lower left, facing the back) */
	// Bouton de manette de menu
    CONTROLLER_BUTTON_PADDLE4,  /* Xbox Elite paddle P4 (lower right, facing the back) */
	// Bouton de manette de menu
    CONTROLLER_BUTTON_TOUCHPAD, /* PS4/PS5 touchpad button */
	// Nombre de bouton de manette
    CONTROLLER_BUTTON_MAX
};

// Axes de manette
enum ControllerAxis : uint8_t
{
	// Axe de manette invalide
    CONTROLLER_AXIS_INVALID = -1,
	// L'axe de manette gauche X
    CONTROLLER_AXIS_LEFTX,
    // L'axe de manette gauche Y
    CONTROLLER_AXIS_LEFTY,
    // L'axe de manette droite X
    CONTROLLER_AXIS_RIGHTX,
    // L'axe de manette droite Y
    CONTROLLER_AXIS_RIGHTY,
    // L'axe de manette gauche de la gâchette
    CONTROLLER_AXIS_TRIGGERLEFT,
    // L'axe de manette droite de la gâchette
    CONTROLLER_AXIS_TRIGGERRIGHT,
	// Nombre max d'axes de manette
    CONTROLLER_AXIS_MAX
};

// Joystick de manette
enum JoyStick : uint8_t
{
	// Joystick Gauche
    JOYSTICK_LEFT,   // LEFTX + LEFTY
	// Joystick Droit
	JOYSTICK_RIGHT,   // RIGHTX + RIGHTY
	// Nombre de Joystick
	JOYSTICK_MAX  // Nombre de Joystick
};

// Bouton de souris
#define BUTTON(X)       (1 << ((X)-1))
#define BUTTON_LEFT     1 // Bouton de souris gauche
#define BUTTON_MIDDLE   2 // Bouton de souris du milieu
#define BUTTON_RIGHT    3 // Bouton de souris droit
#define BUTTON_X1       4 // Bouton de souris X1
#define BUTTON_X2       5 // Bouton de souris X2
#define BUTTON_LMASK    BUTTON(BUTTON_LEFT) // Masque de bouton de souris gauche
#define BUTTON_MMASK    BUTTON(BUTTON_MIDDLE) // Masque de bouton de souris du milieu
#define BUTTON_RMASK    BUTTON(BUTTON_RIGHT) // Masque de bouton de souris droit
#define BUTTON_X1MASK   BUTTON(BUTTON_X1) // Masque de bouton de souris X1
#define BUTTON_X2MASK   BUTTON(BUTTON_X2) // Masque de bouton de souris X2

enum MouseScroll : uint8_t
{
    // Scroll vers le haut
	SCROLL_UP,
    // Scroll vers le bas
	SCROLL_DOWN,
    // Scroll vers la gauche
	SCROLL_LEFT,
    // Scroll vers la droite
	SCROLL_RIGHT,

    // Nombre de scroll de souris
    SCROLL_MAX
};

enum class SwitchButtonController : uint8_t
{
	// Nombre de bouton Max d'une manette Switch
    BUTTON_MAX
};
enum class SwitchAxisController : uint8_t
{
    // Nombre d'axe Max d'une manette Switch
    AXIS_MAX
};

enum class XboxButtonController : uint8_t
{
    // Nombre de bouton Max d'une manette Xbox
    BUTTON_MAX
};
enum class XboxAxisController : uint8_t
{
	// Nombre d'axe Max d'une manette Xbox
};

enum class PlayStationButtonController : uint8_t
{
    // Nombre de bouton Max d'une manette PlayStation
    BUTTON_MAX
};
enum class PlayStationAxisController : uint8_t
{
    // Nombre d'axe Max d'une manette PlayStation
    AXIS_MAX
};