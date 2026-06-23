#pragma once
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
//Version 1.0.1

// Touche Virtuel
enum KeyboardKeycodeKeys
{
    // Touche inconnue
    K_UNKNOWN = 0,

	// Touche de retour
    K_RETURN = '\r',
    // Touche d'échappement
    K_ESCAPE = '\x1B',
    // Touche de suppression
    K_BACKSPACE = '\b',
    // Touche de tabulation
    K_TAB = '\t',
    // Touche d'espace
    K_SPACE = ' ',
    // Touche d'exclamation
    K_EXCLAIM = '!',
    // Touche de guillemet double
    K_QUOTEDBL = '"',
    // Touche dièse
    K_HASH = '#',
    // Touche de pourcentage
    K_PERCENT = '%',
    // Touche de dollar
    K_DOLLAR = '$',
    // Touche esperluette
    K_AMPERSAND = '&',
    // Touche de guillemet simple
    K_QUOTE = '\'',
    // Touche de parenthèse gauche
    K_LEFTPAREN = '(',
    // Touche de parenthèse droite
    K_RIGHTPAREN = ')',
    // Touche d'astérisque
    K_ASTERISK = '*',
    // Touche de plus
    K_PLUS = '+',
    // Touche de virgule
    K_COMMA = ',',
    // Touche de moins
    K_MINUS = '-',
    // Touche de point
    K_PERIOD = '.',
    // Touche de barre oblique
    K_SLASH = '/',
	// Touche du chiffre 0
    K_0 = '0',
	// Touche du chiffre 1
    K_1 = '1',
    // Touche du chiffre 2
    K_2 = '2',
    // Touche du chiffre 3
    K_3 = '3',
    // Touche du chiffre 4
    K_4 = '4',
    // Touche du chiffre 5
    K_5 = '5',
    // Touche du chiffre 6
    K_6 = '6',
    // Touche du chiffre 7
    K_7 = '7',
    // Touche du chiffre 8
    K_8 = '8',
    // Touche du chiffre 9
    K_9 = '9',
	// Touche de deux points
    K_COLON = ':',
	// Touche de point-virgule
    K_SEMICOLON = ';',
	// Touche de moins
    K_LESS = '<',
	// Touche de égal
    K_EQUALS = '=',
	// Touche de plus grand que
    K_GREATER = '>',
	// Touche de point d'interrogation
    K_QUESTION = '?',
	// Touche de arobase
    K_AT = '@',

	// Touche de crochets gauche
    K_LEFTBRACKET = '[',
	// Touche de barre oblique inversée
    K_BACKSLASH = '\\',
	// Touche de crochets droite
    K_RIGHTBRACKET = ']',
	// Touche de chapeau
    K_CARET = '^',
	// Touche de soulignement
    K_UNDERSCORE = '_',
	// Touche de backquote
    K_BACKQUOTE = '`',
	// Touche de lettre a
    K_a = 'a',
	// Touche de lettre b
    K_b = 'b',
	// Touche de lettre c
    K_c = 'c',
	// Touche de lettre d
    K_d = 'd',
	// Touche de lettre e
    K_e = 'e',
	// Touche de lettre f
    K_f = 'f',
		// Touche de lettre g
    K_g = 'g',
	// Touche de lettre h
    K_h = 'h',
	// Touche de lettre i
    K_i = 'i',
	// Touche de lettre j
    K_j = 'j',
	// Touche de lettre k
    K_k = 'k',
	// Touche de lettre l
    K_l = 'l',
	// Touche de lettre m
    K_m = 'm',
	// Touche de lettre n
    K_n = 'n',
	// Touche de lettre o
    K_o = 'o',
	// Touche de lettre p
    K_p = 'p',
	// Touche de lettre q
    K_q = 'q',
	// Touche de lettre r
    K_r = 'r',
	// Touche de lettre s
    K_s = 's',
	// Touche de lettre t
    K_t = 't',
	// Touche de lettre u
    K_u = 'u',
	// Touche de lettre v
    K_v = 'v',
	// Touche de lettre w
    K_w = 'w',
	// Touche de lettre x
    K_x = 'x',
	// Touche de lettre y
    K_y = 'y',
	// Touche de lettre z
    K_z = 'z',


	// Touche de verrouillage des majuscules
    K_CAPSLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),

	    // Touches de fonction F1
    K_F1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1),
		// Touches de fonction F2
    K_F2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2),
		// Touches de fonction F3
    K_F3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3),
		// Touches de fonction F4
    K_F4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4),
		// Touches de fonction F5
    K_F5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5),
		// Touches de fonction F6
    K_F6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6),
		// Touches de fonction F7
    K_F7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7),
		// Touches de fonction F8
    K_F8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8),
		// Touches de fonction F9
    K_F9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9),
		// Touches de fonction F10
    K_F10 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10),
		// Touches de fonction F11
    K_F11 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11),
		// Touches de fonction F12
    K_F12 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12),

	// Touche d'impression écran
    K_PRINTSCREEN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
	// Touche de verrouillage du défilement
    K_SCROLLLOCK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
	// Touche de pause
    K_PAUSE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE),
		// Touche d'insertion
    K_INSERT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT),
		// Touche de suppression
    K_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME),
		// Touche de page précédente
    K_PAGEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP),
		// Touche de suppression
    K_DELETE = '\x7F',
		// Touche de fin
    K_END = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END),
		// Touche de page suivante
    K_PAGEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
		// Touches de direction
    K_RIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT),
		// Touches de direction
    K_LEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT),
		// Touches de direction
    K_DOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN),
		// Touches de direction
    K_UP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP),
		// Touche de verrouillage numérique

		// Touche de verrouillage numérique
    K_NUMLOCKCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
		// Touches du pavé numérique /
    K_KP_DIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
		// Touches du pavé numérique *
    K_KP_MULTIPLY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
		// Touches du pavé numérique -
    K_KP_MINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
		// Touches du pavé numérique +
    K_KP_PLUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
		// Touches du pavé numérique Entrée
    K_KP_ENTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER),

	// Touches du pavé numérique 1
    K_KP_1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1),
	// Touches du pavé numérique 2
    K_KP_2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2),
	// Touches du pavé numérique 3
    K_KP_3 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3),
	// Touches du pavé numérique 4
    K_KP_4 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4),
	// Touches du pavé numérique 5
    K_KP_5 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5),
	// Touches du pavé numérique 6
    K_KP_6 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6),
	// Touches du pavé numérique 7
    K_KP_7 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7),
	// Touches du pavé numérique 8
    K_KP_8 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8),
	// Touches du pavé numérique 9
    K_KP_9 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9),
	// Touches du pavé numérique 0
    K_KP_0 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0),

		// Touches du pavé numérique .
    K_KP_PERIOD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),

	// Touche d'application
    K_APPLICATION = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION),
	// Touche de puissance
    K_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER),
	// Touches du pavé numérique =
    K_KP_EQUALS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
		// Touches de fonction F13
    K_F13 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13),
	// Touches de fonction F14
    K_F14 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14),
		// Touches de fonction F15
    K_F15 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15),
		// Touches de fonction F16
    K_F16 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16),
		// Touches de fonction F17
    K_F17 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17),
		// Touches de fonction F18
    K_F18 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18),
		// Touches de fonction F19
    K_F19 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19),
		// Touches de fonction F20
    K_F20 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20),
		// Touches de fonction F21
    K_F21 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21),
		// Touches de fonction F22
    K_F22 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22),
		// Touches de fonction F23
    K_F23 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23),
		// Touches de fonction F24
    K_F24 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24),
		// Touche de exécution
    K_EXECUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE),
		// Touche de exécution
    K_HELP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP),
		// Touche de menu
    K_MENU = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU),
	// Touche de sélection
    K_SELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT),
	// Touche de arrêt
    K_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP),
	// Touche de redémarrage
    K_AGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN),
	// Touche d'annulation
    K_UNDO = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO),
	// Touche de coupe
    K_CUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT),
	// Touche de copie
    K_COPY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY),
	// Touche de coller
    K_PASTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE),
	// Touche de trouver
    K_FIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND),
	// Touche de mute
    K_MUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE),
	// Touches de volume
    K_VOLUMEUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP),
	// Touches de volume
    K_VOLUMEDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN),
	// Touches du pavé numérique ,
    K_KP_COMMA = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
	// Touches du pavé numérique =
    K_KP_EQUALSAS400 =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

		// Touche d'altération
    K_ALTERASE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE),
		// Touche de sysreq
    K_SYSREQ = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ),
		// Touche de cancel
    K_CANCEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL),
		// Touche de clear
    K_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR),
		// Touche de prior
    K_PRIOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR),
		// Touche de retour
    K_RETURN2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2),
		// Touche de séparation
    K_SEPARATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
		// Touche de sortie
    K_OUT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT),
		// Touche de opérateur
    K_OPER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER),
		// Touche de Clear/Again
    K_CLEARAGAIN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
		// Touche de CrSel
    K_CRSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL),
		// Touche de ExSel
    K_EXSEL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL),
        
		// Touches du pavé numérique 00
    K_KP_00 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00),
		// Touches du pavé numérique 000
    K_KP_000 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000),
		// Touche de séparateur de milliers
    K_THOUSANDSSEPARATOR =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
		// Touche de séparateur décimal
    K_DECIMALSEPARATOR =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
		// Touche de séparateur de monétaire
    K_CURRENCYUNIT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
		// Touche de séparateur de monétaire
    K_CURRENCYSUBUNIT =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
		// Touches du pavé numérique (
    K_KP_LEFTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
		// Touches du pavé numérique )
    K_KP_RIGHTPAREN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
		// Touches du pavé numérique {
    K_KP_LEFTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
		// Touches du pavé numérique }
    K_KP_RIGHTBRACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
		// Touches du pavé numérique Tab
    K_KP_TAB = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB),
		// Touches du pavé numérique Backspace
    K_KP_BACKSPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
		// Touches du pavé numérique A
    K_KP_A = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A),
		// Touches du pavé numérique B
    K_KP_B = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B),
		// Touches du pavé numérique C
    K_KP_C = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C),
		// Touches du pavé numérique D
    K_KP_D = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D),
		// Touches du pavé numérique E
    K_KP_E = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E),
		// Touches du pavé numérique F
    K_KP_F = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F),
		// Touches du pavé numérique XOR
    K_KP_XOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR),
		// Touches du pavé numérique ^
    K_KP_POWER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER),
		// Touches du pavé numérique %
    K_KP_PERCENT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
		// Touches du pavé numérique &
    K_KP_LESS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS),
		// Touches du pavé numérique >
    K_KP_GREATER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
		// Touches du pavé numérique &
    K_KP_AMPERSAND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
		// Touche de double esperluette du pavé numérique
    K_KP_DBLAMPERSAND =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
		// Touche de barre verticale du pavé numérique
    K_KP_VERTICALBAR =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
		// Touche de double barre verticale du pavé numérique
    K_KP_DBLVERTICALBAR =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
		// Touches du pavé numérique :
    K_KP_COLON = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON),
		// Touches du pavé numérique #
    K_KP_HASH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH),
		// Touches du pavé numérique espace
    K_KP_SPACE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
		// Touches du pavé numérique @
    K_KP_AT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT),
		// Touches du pavé numérique !
    K_KP_EXCLAM = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
		// Touches du pavé numérique mémoire store
    K_KP_MEMSTORE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
		// Touches du pavé numérique mémoire recall
    K_KP_MEMRECALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
		// Touches du pavé numérique mémoire clear
    K_KP_MEMCLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
		// Touches du pavé numérique mémoire addition
    K_KP_MEMADD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
		// Touche de soustraction de mémoire du pavé numérique
    K_KP_MEMSUBTRACT =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
		// Touche de multiplication de mémoire du pavé numérique
    K_KP_MEMMULTIPLY =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
		// Touche de division de mémoire du pavé numérique
    K_KP_MEMDIVIDE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
		// Touches du pavé numérique +/- du pavé numérique
    K_KP_PLUSMINUS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
		// Touches du pavé numérique Clear du pavé numérique
    K_KP_CLEAR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
		// Touches du pavé numérique Clear Entry du pavé numérique
    K_KP_CLEARENTRY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
		// Touches du pavé numérique Binary du pavé numérique
    K_KP_BINARY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
		// Touches du pavé numérique Octal du pavé numérique
    K_KP_OCTAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
		// Touches du pavé numérique Décimal du pavé numérique
    K_KP_DECIMAL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
		// Touches du pavé numérique Hexadecimal du pavé numérique
    K_KP_HEXADECIMAL =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

		// Touche de contrôle gauche
    K_LCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL),
		// Touche de majuscule gauche
    K_LSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT),
		// Touche d'altération gauche
    K_LALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT),
		// Touche de logo gauche
    K_LGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI),
		// Touche de contrôle droite
    K_RCTRL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL),
		// Touche de majuscule droite
    K_RSHIFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT),
		// Touche d'altération droite
    K_RALT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT),
		// Touche de logo droite
    K_RGUI = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI),

		// Touche de mode
    K_MODE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE),

		// Touches de média
    K_AUDIONEXT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
		// Touches de média
    K_AUDIOPREV = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
		// Touches de média
    K_AUDIOSTOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
		// Touches de média
    K_AUDIOPLAY = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
		// Touches de média
    K_AUDIOMUTE = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
		// Touches de média
    K_MEDIASELECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
		// Touches de média
    K_WWW = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WWW),
		// Touches de média
    K_MAIL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MAIL),
		// Touches de média
    K_CALCULATOR = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
		// Touches de média
    K_COMPUTER = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COMPUTER),
		// Touches de média
    K_AC_SEARCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
		// Touches de média
    K_AC_HOME = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME),
		// Touches de média
    K_AC_BACK = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK),
		// Touches de média
    K_AC_FORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
		// Touches de média
    K_AC_STOP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP),
		// Touches de média
    K_AC_REFRESH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
		// Touches de média
    K_AC_BOOKMARKS = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

		// Touches de luminosité
    K_BRIGHTNESSDOWN =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
		// Touches de luminosité
    K_BRIGHTNESSUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
		// Touches de luminosité
    K_DISPLAYSWITCH = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
		// Touches de luminosité
    K_KBDILLUMTOGGLE =
    SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
		// Touches de luminosité
    K_KBDILLUMDOWN = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
		// Touches de luminosité
    K_KBDILLUMUP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
		// Touches de luminosité
    K_EJECT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EJECT),
		// Touches de luminosité
    K_SLEEP = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP),
		// Touches de média
    K_APP1 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP1),
		// Touches de média
    K_APP2 = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APP2),

		// Touches de média
    K_AUDIOREWIND = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOREWIND),
		// Touches de média
    K_AUDIOFASTFORWARD = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AUDIOFASTFORWARD),

		// Touches de média
    K_SOFTLEFT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT),
		// Touches de média
    K_SOFTRIGHT = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT),
		// Touches de média
    K_CALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL),
		// Touches de média
    K_ENDCALL = SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL)
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