#include "input_manager.h"

breene::KeyboardManager::KeyboardManager()
{
	for (GLuint i = 0; i < BREENE_KEYCOUNT; ++i)
		_keys.insert(KeyPair(static_cast<Key>(i), BREENE_KEY_RELEASED));
}


breene::KeyboardManager::KeyPair breene::KeyboardManager::Input(GLint key, GLint action)
{
	Key keypress;
	switch (key)
	{
	case GLFW_KEY_GRAVE_ACCENT:
		keypress = BREENE_KEY_GRAVE_ACCENT;
		break;
	case GLFW_KEY_1:
		keypress = BREENE_KEY_ONE;
		break;
	case GLFW_KEY_2:
		keypress = BREENE_KEY_TWO;
		break;
	case GLFW_KEY_3:
		keypress = BREENE_KEY_THREE;
		break;
	case GLFW_KEY_4:
		keypress = BREENE_KEY_FOUR;
		break;
	case GLFW_KEY_5:
		keypress = BREENE_KEY_FIVE;
		break;
	case GLFW_KEY_6:
		keypress = BREENE_KEY_SIX;
		break;
	case GLFW_KEY_7:
		keypress = BREENE_KEY_SEVEN;
		break;
	case GLFW_KEY_8:
		keypress = BREENE_KEY_EIGHT;
		break;
	case GLFW_KEY_9:
		keypress = BREENE_KEY_NINE;
		break;
	case GLFW_KEY_0:
		keypress = BREENE_KEY_ZERO;
		break;
	case GLFW_KEY_MINUS:
		keypress = BREENE_KEY_MINUS;
		break;
	case GLFW_KEY_EQUAL:
		keypress = BREENE_KEY_EQUAL;
		break;
	case GLFW_KEY_BACKSPACE:
		keypress = BREENE_KEY_BACKSPACE;
		break;
	case GLFW_KEY_Q:
		keypress = BREENE_KEY_Q;
		break;
	case GLFW_KEY_W:
		keypress = BREENE_KEY_W;
		break;
	case GLFW_KEY_E:
		keypress = BREENE_KEY_E;
		break;
	case GLFW_KEY_R:
		keypress = BREENE_KEY_R;
		break;
	case GLFW_KEY_T:
		keypress = BREENE_KEY_T;
		break;
	case GLFW_KEY_Y:
		keypress = BREENE_KEY_Y;
		break;
	case GLFW_KEY_U:
		keypress = BREENE_KEY_U;
		break;
	case GLFW_KEY_I:
		keypress = BREENE_KEY_I;
		break;
	case GLFW_KEY_O:
		keypress = BREENE_KEY_O;
		break;
	case GLFW_KEY_P:
		keypress = BREENE_KEY_P;
		break;
	case GLFW_KEY_LEFT_BRACKET:
		keypress = BREENE_KEY_SQUARE_BRACKET_OPEN;
		break;
	case GLFW_KEY_RIGHT_BRACKET:
		keypress = BREENE_KEY_SQUARE_BRACKET_CLOSE;
		break;
	case GLFW_KEY_A:
		keypress = BREENE_KEY_A;
		break;
	case GLFW_KEY_S:
		keypress = BREENE_KEY_S;
		break;
	case GLFW_KEY_D:
		keypress = BREENE_KEY_D;
		break;
	case GLFW_KEY_F:
		keypress = BREENE_KEY_F;
		break;
	case GLFW_KEY_G:
		keypress = BREENE_KEY_G;
		break;
	case GLFW_KEY_H:
		keypress = BREENE_KEY_H;
		break;
	case GLFW_KEY_J:
		keypress = BREENE_KEY_J;
		break;
	case GLFW_KEY_K:
		keypress = BREENE_KEY_K;
		break;
	case GLFW_KEY_L:
		keypress = BREENE_KEY_L;
		break;
	case GLFW_KEY_SEMICOLON:
		keypress = BREENE_KEY_SEMICOLON;
		break;
	case GLFW_KEY_APOSTROPHE:
		keypress = BREENE_KEY_APOSTROPHE;
		break;
	case GLFW_KEY_BACKSLASH:
		keypress = BREENE_KEY_BACKSLASH;
		break;
	case GLFW_KEY_ENTER:
		keypress = BREENE_KEY_ENTER;
		break;
	case GLFW_KEY_Z:
		keypress = BREENE_KEY_Z;
		break;
	case GLFW_KEY_X:
		keypress = BREENE_KEY_X;
		break;
	case GLFW_KEY_C:
		keypress = BREENE_KEY_C;
		break;
	case GLFW_KEY_V:
		keypress = BREENE_KEY_V;
		break;
	case GLFW_KEY_B:
		keypress = BREENE_KEY_B;
		break;
	case GLFW_KEY_N:
		keypress = BREENE_KEY_N;
		break;
	case GLFW_KEY_M:
		keypress = BREENE_KEY_M;
		break;
	case GLFW_KEY_COMMA:
		keypress = BREENE_KEY_COMMA;
		break;
	case GLFW_KEY_PERIOD:
		keypress = BREENE_KEY_PERIOD;
		break;
	case GLFW_KEY_SLASH:
		keypress = BREENE_KEY_FORWARD_SLASH;
		break;
	case GLFW_KEY_TAB:
		keypress = BREENE_KEY_TAB;
		break;
	case GLFW_KEY_CAPS_LOCK:
		keypress = BREENE_KEY_CAPS;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		keypress = BREENE_KEY_LSHIFT;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		keypress = BREENE_KEY_LCTRL;
		break;
	case GLFW_KEY_LEFT_ALT:
		keypress = BREENE_KEY_LALT;
		break;
	case GLFW_KEY_SPACE:
		keypress = BREENE_KEY_SPACE;
		break;
	case GLFW_KEY_RIGHT_ALT:
		keypress = BREENE_KEY_RALT;
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		keypress = BREENE_KEY_RCTRL;
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		keypress = BREENE_KEY_RSHIFT;
		break;
	case GLFW_KEY_ESCAPE:
		keypress = BREENE_KEY_ESC;
		break;
	case GLFW_KEY_F1:
		keypress = BREENE_KEY_F1;
		break;
	case GLFW_KEY_F2:
		keypress = BREENE_KEY_F2;
		break;
	case GLFW_KEY_F3:
		keypress = BREENE_KEY_F3;
		break;
	case GLFW_KEY_F4:
		keypress = BREENE_KEY_F4;
		break;
	case GLFW_KEY_F5:
		keypress = BREENE_KEY_F5;
		break;
	case GLFW_KEY_F6:
		keypress = BREENE_KEY_F6;
		break;
	case GLFW_KEY_F7:
		keypress = BREENE_KEY_F7;
		break;
	case GLFW_KEY_F8:
		keypress = BREENE_KEY_F8;
		break;
	case GLFW_KEY_F9:
		keypress = BREENE_KEY_F9;
		break;
	case GLFW_KEY_F10:
		keypress = BREENE_KEY_F10;
		break;
	case GLFW_KEY_F11:
		keypress = BREENE_KEY_F11;
		break;
	case GLFW_KEY_F12:
		keypress = BREENE_KEY_F12;
		break;
	case GLFW_KEY_PRINT_SCREEN:
		keypress = BREENE_KEY_PRT_SCR;
		break;
	case GLFW_KEY_SCROLL_LOCK:
		keypress = BREENE_KEY_SCROLL_LOCK;
		break;
	case GLFW_KEY_PAUSE:
		keypress = BREENE_KEY_PAUSE;
		break;
	case GLFW_KEY_INSERT:
		keypress = BREENE_KEY_INS;
		break;
	case GLFW_KEY_HOME:
		keypress = BREENE_KEY_HOME;
		break;
	case GLFW_KEY_PAGE_UP:
		keypress = BREENE_KEY_PG_UP;
		break;
	case GLFW_KEY_PAGE_DOWN:
		keypress = BREENE_KEY_PG_DOWN;
		break;
	case GLFW_KEY_END:
		keypress = BREENE_KEY_END;
		break;
	case GLFW_KEY_UP:
		keypress = BREENE_KEY_UP;
		break;
	case GLFW_KEY_DOWN:
		keypress = BREENE_KEY_DOWN;
		break;
	case GLFW_KEY_LEFT:
		keypress = BREENE_KEY_LEFT;
		break;
	case GLFW_KEY_RIGHT:
		keypress = BREENE_KEY_RIGHT;
		break;
	case GLFW_KEY_DELETE:
		keypress = BREENE_KEY_DEL;
		break;
	case GLFW_KEY_NUM_LOCK:
		keypress = BREENE_KEY_NUM_LOCK;
		break;
	case GLFW_KEY_KP_0:
		keypress = BREENE_KEY_NUM_0;
		break;
	case GLFW_KEY_KP_1:
		keypress = BREENE_KEY_NUM_1;
		break;
	case GLFW_KEY_KP_2:
		keypress = BREENE_KEY_NUM_2;
		break;
	case GLFW_KEY_KP_3:
		keypress = BREENE_KEY_NUM_3;
		break;
	case GLFW_KEY_KP_4:
		keypress = BREENE_KEY_NUM_4;
		break;
	case GLFW_KEY_KP_5:
		keypress = BREENE_KEY_NUM_5;
		break;
	case GLFW_KEY_KP_6:
		keypress = BREENE_KEY_NUM_6;
		break;
	case GLFW_KEY_KP_7:
		keypress = BREENE_KEY_NUM_7;
		break;
	case GLFW_KEY_KP_8:
		keypress = BREENE_KEY_NUM_8;
		break;
	case GLFW_KEY_KP_9:
		keypress = BREENE_KEY_NUM_9;
		break;
	case GLFW_KEY_KP_DIVIDE:
		keypress = BREENE_KEY_NUM_DIVIDE;
		break;
	case GLFW_KEY_KP_MULTIPLY:
		keypress = BREENE_KEY_NUM_MULTIPLY;
		break;
	case GLFW_KEY_KP_SUBTRACT:
		keypress = BREENE_KEY_NUM_SUBTRACT;
		break;
	case GLFW_KEY_KP_ADD:
		keypress = BREENE_KEY_NUM_ADD;
		break;
	case GLFW_KEY_KP_ENTER:
		keypress = BREENE_KEY_NUM_ENTER;
		break;
	default:
		keypress = BREENE_KEY_UNKNOWN;
		break;
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		_keys[keypress] = BREENE_KEY_PRESSED;
	else
		_keys[keypress] = BREENE_KEY_RELEASED;

	return KeyPair(keypress, _keys[keypress]);
}

breene::Key breene::KeyboardManager::ConvertKey(GLint glfw_key)
{
	Key keypress;
	switch (glfw_key)
	{
	case GLFW_KEY_GRAVE_ACCENT:
		keypress = BREENE_KEY_GRAVE_ACCENT;
		break;
	case GLFW_KEY_1:
		keypress = BREENE_KEY_ONE;
		break;
	case GLFW_KEY_2:
		keypress = BREENE_KEY_TWO;
		break;
	case GLFW_KEY_3:
		keypress = BREENE_KEY_THREE;
		break;
	case GLFW_KEY_4:
		keypress = BREENE_KEY_FOUR;
		break;
	case GLFW_KEY_5:
		keypress = BREENE_KEY_FIVE;
		break;
	case GLFW_KEY_6:
		keypress = BREENE_KEY_SIX;
		break;
	case GLFW_KEY_7:
		keypress = BREENE_KEY_SEVEN;
		break;
	case GLFW_KEY_8:
		keypress = BREENE_KEY_EIGHT;
		break;
	case GLFW_KEY_9:
		keypress = BREENE_KEY_NINE;
		break;
	case GLFW_KEY_0:
		keypress = BREENE_KEY_ZERO;
		break;
	case GLFW_KEY_MINUS:
		keypress = BREENE_KEY_MINUS;
		break;
	case GLFW_KEY_EQUAL:
		keypress = BREENE_KEY_EQUAL;
		break;
	case GLFW_KEY_BACKSPACE:
		keypress = BREENE_KEY_BACKSPACE;
		break;
	case GLFW_KEY_Q:
		keypress = BREENE_KEY_Q;
		break;
	case GLFW_KEY_W:
		keypress = BREENE_KEY_W;
		break;
	case GLFW_KEY_E:
		keypress = BREENE_KEY_E;
		break;
	case GLFW_KEY_R:
		keypress = BREENE_KEY_R;
		break;
	case GLFW_KEY_T:
		keypress = BREENE_KEY_T;
		break;
	case GLFW_KEY_Y:
		keypress = BREENE_KEY_Y;
		break;
	case GLFW_KEY_U:
		keypress = BREENE_KEY_U;
		break;
	case GLFW_KEY_I:
		keypress = BREENE_KEY_I;
		break;
	case GLFW_KEY_O:
		keypress = BREENE_KEY_O;
		break;
	case GLFW_KEY_P:
		keypress = BREENE_KEY_P;
		break;
	case GLFW_KEY_LEFT_BRACKET:
		keypress = BREENE_KEY_SQUARE_BRACKET_OPEN;
		break;
	case GLFW_KEY_RIGHT_BRACKET:
		keypress = BREENE_KEY_SQUARE_BRACKET_CLOSE;
		break;
	case GLFW_KEY_A:
		keypress = BREENE_KEY_A;
		break;
	case GLFW_KEY_S:
		keypress = BREENE_KEY_S;
		break;
	case GLFW_KEY_D:
		keypress = BREENE_KEY_D;
		break;
	case GLFW_KEY_F:
		keypress = BREENE_KEY_F;
		break;
	case GLFW_KEY_G:
		keypress = BREENE_KEY_G;
		break;
	case GLFW_KEY_H:
		keypress = BREENE_KEY_H;
		break;
	case GLFW_KEY_J:
		keypress = BREENE_KEY_J;
		break;
	case GLFW_KEY_K:
		keypress = BREENE_KEY_K;
		break;
	case GLFW_KEY_L:
		keypress = BREENE_KEY_L;
		break;
	case GLFW_KEY_SEMICOLON:
		keypress = BREENE_KEY_SEMICOLON;
		break;
	case GLFW_KEY_APOSTROPHE:
		keypress = BREENE_KEY_APOSTROPHE;
		break;
	case GLFW_KEY_BACKSLASH:
		keypress = BREENE_KEY_BACKSLASH;
		break;
	case GLFW_KEY_ENTER:
		keypress = BREENE_KEY_ENTER;
		break;
	case GLFW_KEY_Z:
		keypress = BREENE_KEY_Z;
		break;
	case GLFW_KEY_X:
		keypress = BREENE_KEY_X;
		break;
	case GLFW_KEY_C:
		keypress = BREENE_KEY_C;
		break;
	case GLFW_KEY_V:
		keypress = BREENE_KEY_V;
		break;
	case GLFW_KEY_B:
		keypress = BREENE_KEY_B;
		break;
	case GLFW_KEY_N:
		keypress = BREENE_KEY_N;
		break;
	case GLFW_KEY_M:
		keypress = BREENE_KEY_M;
		break;
	case GLFW_KEY_COMMA:
		keypress = BREENE_KEY_COMMA;
		break;
	case GLFW_KEY_PERIOD:
		keypress = BREENE_KEY_PERIOD;
		break;
	case GLFW_KEY_SLASH:
		keypress = BREENE_KEY_FORWARD_SLASH;
		break;
	case GLFW_KEY_TAB:
		keypress = BREENE_KEY_TAB;
		break;
	case GLFW_KEY_CAPS_LOCK:
		keypress = BREENE_KEY_CAPS;
		break;
	case GLFW_KEY_LEFT_SHIFT:
		keypress = BREENE_KEY_LSHIFT;
		break;
	case GLFW_KEY_LEFT_CONTROL:
		keypress = BREENE_KEY_LCTRL;
		break;
	case GLFW_KEY_LEFT_ALT:
		keypress = BREENE_KEY_LALT;
		break;
	case GLFW_KEY_SPACE:
		keypress = BREENE_KEY_SPACE;
		break;
	case GLFW_KEY_RIGHT_ALT:
		keypress = BREENE_KEY_RALT;
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		keypress = BREENE_KEY_RCTRL;
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		keypress = BREENE_KEY_RSHIFT;
		break;
	case GLFW_KEY_ESCAPE:
		keypress = BREENE_KEY_ESC;
		break;
	case GLFW_KEY_F1:
		keypress = BREENE_KEY_F1;
		break;
	case GLFW_KEY_F2:
		keypress = BREENE_KEY_F2;
		break;
	case GLFW_KEY_F3:
		keypress = BREENE_KEY_F3;
		break;
	case GLFW_KEY_F4:
		keypress = BREENE_KEY_F4;
		break;
	case GLFW_KEY_F5:
		keypress = BREENE_KEY_F5;
		break;
	case GLFW_KEY_F6:
		keypress = BREENE_KEY_F6;
		break;
	case GLFW_KEY_F7:
		keypress = BREENE_KEY_F7;
		break;
	case GLFW_KEY_F8:
		keypress = BREENE_KEY_F8;
		break;
	case GLFW_KEY_F9:
		keypress = BREENE_KEY_F9;
		break;
	case GLFW_KEY_F10:
		keypress = BREENE_KEY_F10;
		break;
	case GLFW_KEY_F11:
		keypress = BREENE_KEY_F11;
		break;
	case GLFW_KEY_F12:
		keypress = BREENE_KEY_F12;
		break;
	case GLFW_KEY_PRINT_SCREEN:
		keypress = BREENE_KEY_PRT_SCR;
		break;
	case GLFW_KEY_SCROLL_LOCK:
		keypress = BREENE_KEY_SCROLL_LOCK;
		break;
	case GLFW_KEY_PAUSE:
		keypress = BREENE_KEY_PAUSE;
		break;
	case GLFW_KEY_INSERT:
		keypress = BREENE_KEY_INS;
		break;
	case GLFW_KEY_HOME:
		keypress = BREENE_KEY_HOME;
		break;
	case GLFW_KEY_PAGE_UP:
		keypress = BREENE_KEY_PG_UP;
		break;
	case GLFW_KEY_PAGE_DOWN:
		keypress = BREENE_KEY_PG_DOWN;
		break;
	case GLFW_KEY_END:
		keypress = BREENE_KEY_END;
		break;
	case GLFW_KEY_DELETE:
		keypress = BREENE_KEY_DEL;
		break;
	case GLFW_KEY_UP:
		keypress = BREENE_KEY_UP;
		break;
	case GLFW_KEY_DOWN:
		keypress = BREENE_KEY_DOWN;
		break;
	case GLFW_KEY_LEFT:
		keypress = BREENE_KEY_LEFT;
		break;
	case GLFW_KEY_RIGHT:
		keypress = BREENE_KEY_RIGHT;
		break;
	case GLFW_KEY_NUM_LOCK:
		keypress = BREENE_KEY_NUM_LOCK;
		break;
	case GLFW_KEY_KP_0:
		keypress = BREENE_KEY_NUM_0;
		break;
	case GLFW_KEY_KP_1:
		keypress = BREENE_KEY_NUM_1;
		break;
	case GLFW_KEY_KP_2:
		keypress = BREENE_KEY_NUM_2;
		break;
	case GLFW_KEY_KP_3:
		keypress = BREENE_KEY_NUM_3;
		break;
	case GLFW_KEY_KP_4:
		keypress = BREENE_KEY_NUM_4;
		break;
	case GLFW_KEY_KP_5:
		keypress = BREENE_KEY_NUM_5;
		break;
	case GLFW_KEY_KP_6:
		keypress = BREENE_KEY_NUM_6;
		break;
	case GLFW_KEY_KP_7:
		keypress = BREENE_KEY_NUM_7;
		break;
	case GLFW_KEY_KP_8:
		keypress = BREENE_KEY_NUM_8;
		break;
	case GLFW_KEY_KP_9:
		keypress = BREENE_KEY_NUM_9;
		break;
	case GLFW_KEY_KP_DIVIDE:
		keypress = BREENE_KEY_NUM_DIVIDE;
		break;
	case GLFW_KEY_KP_MULTIPLY:
		keypress = BREENE_KEY_NUM_MULTIPLY;
		break;
	case GLFW_KEY_KP_SUBTRACT:
		keypress = BREENE_KEY_NUM_SUBTRACT;
		break;
	case GLFW_KEY_KP_ADD:
		keypress = BREENE_KEY_NUM_ADD;
		break;
	case GLFW_KEY_KP_ENTER:
		keypress = BREENE_KEY_NUM_ENTER;
		break;
	default:
		keypress = BREENE_KEY_UNKNOWN;
		break;
	}

	return keypress;
}

breene::KeyStatus breene::KeyboardManager::GetKeyStatus(Key key)
{
	return _keys[key];
}

std::string breene::KeyboardManager::KeyToString(Key key)
{
	switch (key)
	{
	case BREENE_KEY_GRAVE_ACCENT:
		return "`";
	case BREENE_KEY_ONE:
		return "1";
	case BREENE_KEY_TWO:
		return "2";
	case BREENE_KEY_THREE:
		return "3";
	case BREENE_KEY_FOUR:
		return "4";
	case BREENE_KEY_FIVE:
		return "5";
	case BREENE_KEY_SIX:
		return "6";
	case BREENE_KEY_SEVEN:
		return "7";
	case BREENE_KEY_EIGHT:
		return "8";
	case BREENE_KEY_NINE:
		return "9";
	case BREENE_KEY_ZERO:
		return "0";
	case BREENE_KEY_MINUS:
		return "-";
	case BREENE_KEY_EQUAL:
		return "=";
	case BREENE_KEY_BACKSPACE:
		return "backspace";
	case BREENE_KEY_Q:
		return "Q";
	case BREENE_KEY_W:
		return "W";
	case BREENE_KEY_E:
		return "E";
	case BREENE_KEY_R:
		return "R";
	case BREENE_KEY_T:
		return "T";
	case BREENE_KEY_Y:
		return "Y";
	case BREENE_KEY_U:
		return "U";
	case BREENE_KEY_I:
		return "I";
	case BREENE_KEY_O:
		return "O";
	case BREENE_KEY_P:
		return "P";
	case BREENE_KEY_SQUARE_BRACKET_OPEN:
		return "[";
	case BREENE_KEY_SQUARE_BRACKET_CLOSE:
		return "]";
	case BREENE_KEY_A:
		return "A";
	case BREENE_KEY_S:
		return "S";
	case BREENE_KEY_D:
		return "D";
	case BREENE_KEY_F:
		return "F";
	case BREENE_KEY_G:
		return "G";
	case BREENE_KEY_H:
		return "H";
	case BREENE_KEY_J:
		return "J";
	case BREENE_KEY_K:
		return "K";
	case BREENE_KEY_L:
		return "L";
	case BREENE_KEY_SEMICOLON:
		return ";";
	case BREENE_KEY_APOSTROPHE:
		return "'";
	case BREENE_KEY_BACKSLASH:
		return "\\";
	case BREENE_KEY_ENTER:
		return "enter";
	case BREENE_KEY_Z:
		return "Z";
	case BREENE_KEY_X:
		return "X";
	case BREENE_KEY_C:
		return "C";
	case BREENE_KEY_V:
		return "V";
	case BREENE_KEY_B:
		return "B";
	case BREENE_KEY_N:
		return "N";
	case BREENE_KEY_M:
		return "M";
	case BREENE_KEY_COMMA:
		return ",";
	case BREENE_KEY_PERIOD:
		return ".";
	case BREENE_KEY_FORWARD_SLASH:
		return "/";
	case BREENE_KEY_TAB:
		return "tab";
	case BREENE_KEY_CAPS:
		return "caps lock";
	case BREENE_KEY_LSHIFT:
		return "lshift";
	case BREENE_KEY_LCTRL:
		return "lctrl";
	case BREENE_KEY_LALT:
		return "lalt";
	case BREENE_KEY_SPACE:
		return "lspace";
	case BREENE_KEY_RALT:
		return "ralt";
	case BREENE_KEY_RCTRL:
		return "rctrl";
	case BREENE_KEY_RSHIFT:
		return "rshift";
	case BREENE_KEY_ESC:
		return "esc";
	case BREENE_KEY_F1:
		return "F1";
	case BREENE_KEY_F2:
		return "F2";
	case BREENE_KEY_F3:
		return "F3";
	case BREENE_KEY_F4:
		return "F4";
	case BREENE_KEY_F5:
		return "F5";
	case BREENE_KEY_F6:
		return "F6";
	case BREENE_KEY_F7:
		return "F7";
	case BREENE_KEY_F8:
		return "F8";
	case BREENE_KEY_F9:
		return "F9";
	case BREENE_KEY_F10:
		return "F10";
	case BREENE_KEY_F11:
		return "F11";
	case BREENE_KEY_F12:
		return "F12";
	case BREENE_KEY_PRT_SCR:
		return "print screen";
	case BREENE_KEY_SCROLL_LOCK:
		return "scroll lock";
	case BREENE_KEY_PAUSE:
		return "pause";
	case BREENE_KEY_INS:
		return "insert";
	case BREENE_KEY_HOME:
		return "home";
	case BREENE_KEY_PG_UP:
		return "page up";
	case BREENE_KEY_PG_DOWN:
		return "page down";
	case BREENE_KEY_END:
		return "end";
	case BREENE_KEY_DEL:
		return "delete";
	case BREENE_KEY_UP:
		return "arrow up";
	case BREENE_KEY_DOWN:
		return "arrow down";
	case BREENE_KEY_LEFT:
		return "arrow left";
	case BREENE_KEY_RIGHT:
		return "arrow right";
	case BREENE_KEY_NUM_LOCK:
		return "num lock";
	case BREENE_KEY_NUM_0:
		return "num 0";
	case BREENE_KEY_NUM_1:
		return "num 1";
	case BREENE_KEY_NUM_2:
		return "num 2";
	case BREENE_KEY_NUM_3:
		return "num 3";
	case BREENE_KEY_NUM_4:
		return "num 4";
	case BREENE_KEY_NUM_5:
		return "num 5";
	case BREENE_KEY_NUM_6:
		return "num 6";
	case BREENE_KEY_NUM_7:
		return "num 7";
	case BREENE_KEY_NUM_8:
		return "num 8";
	case BREENE_KEY_NUM_9:
		return "num 9";
	case BREENE_KEY_NUM_DIVIDE:
		return "num /";
	case BREENE_KEY_NUM_MULTIPLY:
		return "num *";
	case BREENE_KEY_NUM_SUBTRACT:
		return "num -";
	case BREENE_KEY_NUM_ADD:
		return "num +";
	case BREENE_KEY_NUM_ENTER:
		return "num enter";
	default:
		return "unknown key";
	}
}

breene::MouseManager::MouseManager()
{
	for (GLuint i = 0; i < BREENE_MOUSE_KEYCOUNT; ++i)
		_keys.insert(KeyPair(static_cast<MouseKey>(i), BREENE_KEY_RELEASED));
}

breene::MouseManager::KeyPair breene::MouseManager::Input(GLint key, GLint action)
{
	MouseKey keypress;
	switch (key)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		keypress = BREENE_MOUSE_KEY_LEFT;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		keypress = BREENE_MOUSE_KEY_RIGHT;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		keypress = BREENE_MOUSE_KEY_MIDDLE;
		break;
	case GLFW_MOUSE_BUTTON_4:
		keypress = BREENE_MOUSE_KEY_4;
		break;
	case GLFW_MOUSE_BUTTON_5:
		keypress = BREENE_MOUSE_KEY_5;
		break;
	case GLFW_MOUSE_BUTTON_6:
		keypress = BREENE_MOUSE_KEY_6;
		break;
	case GLFW_MOUSE_BUTTON_7:
		keypress = BREENE_MOUSE_KEY_7;
		break;
	case GLFW_MOUSE_BUTTON_8:
		keypress = BREENE_MOUSE_KEY_8;
		break;
	default:
		keypress = BREENE_MOUSE_KEY_UNKNOWN;
		break;
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		_keys[keypress] = BREENE_KEY_PRESSED;
	else
		_keys[keypress] = BREENE_KEY_RELEASED;

	return KeyPair(keypress, _keys[keypress]);
}

breene::MouseKey breene::MouseManager::ConvertKey(GLint glfw_key)
{
	MouseKey keypress;
	switch (glfw_key)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		keypress = BREENE_MOUSE_KEY_LEFT;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		keypress = BREENE_MOUSE_KEY_RIGHT;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		keypress = BREENE_MOUSE_KEY_MIDDLE;
		break;
	case GLFW_MOUSE_BUTTON_4:
		keypress = BREENE_MOUSE_KEY_4;
		break;
	case GLFW_MOUSE_BUTTON_5:
		keypress = BREENE_MOUSE_KEY_5;
		break;
	case GLFW_MOUSE_BUTTON_6:
		keypress = BREENE_MOUSE_KEY_6;
		break;
	case GLFW_MOUSE_BUTTON_7:
		keypress = BREENE_MOUSE_KEY_7;
		break;
	case GLFW_MOUSE_BUTTON_8:
		keypress = BREENE_MOUSE_KEY_8;
		break;
	default:
		keypress = BREENE_MOUSE_KEY_UNKNOWN;
		break;
	}

	return keypress;
}

breene::KeyStatus breene::MouseManager::GetKeyStatus(MouseKey mouse_key)
{
	return _keys[mouse_key];
}

std::string breene::MouseManager::KeyToString(MouseKey mouse_key)
{
	switch (mouse_key)
	{
	case BREENE_MOUSE_KEY_LEFT:
		return "left mouse button";
	case BREENE_MOUSE_KEY_RIGHT:
		return "right mouse button";
	case BREENE_MOUSE_KEY_MIDDLE:
		return "middle mouse button";
	case BREENE_MOUSE_KEY_4:
		return "mouse button 4";
	case BREENE_MOUSE_KEY_5:
		return "mouse button 5";
	case BREENE_MOUSE_KEY_6:
		return "mouse button 6";
	case BREENE_MOUSE_KEY_7:
		return "mouse button 7";
	case BREENE_MOUSE_KEY_8:
		return "mouse button 8";
	default:
		return "unknown mouse button";
	}
}
