#ifndef KeyCode_H
#define KeyCode_H


enum KeyCode {

	//#define GLFW_KEY_UNKNOWN            -1
	unknown = -1,

	//#define GLFW_KEY_SPACE              32
	space = 32,
	//#define GLFW_KEY_APOSTROPHE         39
	apostrope = 39,
	//#define GLFW_KEY_COMMA              44
	comma = 44,
	//#define GLFW_KEY_MINUS              45
	minus,
	//#define GLFW_KEY_PERIOD             46
	period,
	//#define GLFW_KEY_SLASH              47
	slash,
	//#define GLFW_KEY_0                  48
	number_0,
	//#define GLFW_KEY_1                  49
	number_1,
	//#define GLFW_KEY_2                  50
	number_2,
	//#define GLFW_KEY_3                  51
	number_3,
	//#define GLFW_KEY_4                  52
	number_4,
	//#define GLFW_KEY_5                  53
	number_5,
	//#define GLFW_KEY_6                  54
	number_6,
	//#define GLFW_KEY_7                  55
	number_7,
	//#define GLFW_KEY_8                  56
	number_8,
	//#define GLFW_KEY_9                  57
	number_9,
	//#define GLFW_KEY_SEMICOLON          59
	semicolon = 59,
	//#define GLFW_KEY_EQUAL              61
	equal = 61,
	//#define GLFW_KEY_A                  65
	a = 65,
	//#define GLFW_KEY_B                  66
	b,
	//#define GLFW_KEY_C                  67
	c,
	//#define GLFW_KEY_D                  68
	d,
	//#define GLFW_KEY_E                  69
	e,
	//#define GLFW_KEY_F                  70
	f,
	//#define GLFW_KEY_G                  71
	g,
	//#define GLFW_KEY_H                  72
	h,
	//#define GLFW_KEY_I                  73
	i,
	//#define GLFW_KEY_J                  74
	j,
	//#define GLFW_KEY_K                  75
	k,
	//#define GLFW_KEY_L                  76
	l,
	//#define GLFW_KEY_M                  77
	m,
	//#define GLFW_KEY_N                  78
	n,
	//#define GLFW_KEY_O                  79
	o,
	//#define GLFW_KEY_P                  80
	p,
	//#define GLFW_KEY_Q                  81
	q,
	//#define GLFW_KEY_R                  82
	r,
	//#define GLFW_KEY_S                  83
	s,
	//#define GLFW_KEY_T                  84
	t,
	//#define GLFW_KEY_U                  85
	u,
	//#define GLFW_KEY_V                  86
	v,
	//#define GLFW_KEY_W                  87
	w,
	//#define GLFW_KEY_X                  88
	x,
	//#define GLFW_KEY_Y                  89
	y,
	//#define GLFW_KEY_Z                  90
	z,
	//#define GLFW_KEY_LEFT_BRACKET       91
	bracket_left,
	//#define GLFW_KEY_BACKSLASH          92
	backslash,
	//#define GLFW_KEY_RIGHT_BRACKET      93
	bracket_right,
	//#define GLFW_KEY_GRAVE_ACCENT       96
	grave_accent = 96,
	//#define GLFW_KEY_WORLD_1            161
	//#define GLFW_KEY_WORLD_2            162
	//
	//#define GLFW_KEY_ESCAPE             256
	escape = 256,
	//#define GLFW_KEY_ENTER              257
	enter,
	//#define GLFW_KEY_TAB                258
	tab,
	//#define GLFW_KEY_BACKSPACE          259
	backspace,
	//#define GLFW_KEY_INSERT             260
	insert,
	//#define GLFW_KEY_DELETE             261
	del,
	//#define GLFW_KEY_RIGHT              262
	right,
	//#define GLFW_KEY_LEFT               263
	left,
	//#define GLFW_KEY_DOWN               264
	down,
	//#define GLFW_KEY_UP                 265
	up,
	//#define GLFW_KEY_PAGE_UP            266
	page_up,
	//#define GLFW_KEY_PAGE_DOWN          267
	page_down,
	//#define GLFW_KEY_HOME               268
	home,
	//#define GLFW_KEY_END                269
	end,
	//#define GLFW_KEY_CAPS_LOCK          280
	caps_lock,
	//#define GLFW_KEY_SCROLL_LOCK        281
	scroll_lock,
	//#define GLFW_KEY_NUM_LOCK           282
	num_lock,
	//#define GLFW_KEY_PRINT_SCREEN       283
	print_screen,
	//#define GLFW_KEY_PAUSE              284
	pause,
	//#define GLFW_KEY_F1                 290
	f1 = 290,
	//#define GLFW_KEY_F2                 291
	f2,
	//#define GLFW_KEY_F3                 292
	f3,
	//#define GLFW_KEY_F4                 293
	f4,
	//#define GLFW_KEY_F5                 294
	f5,
	//#define GLFW_KEY_F6                 295
	f6,
	//#define GLFW_KEY_F7                 296
	f7,
	//#define GLFW_KEY_F8                 297
	f8,
	//#define GLFW_KEY_F9                 298
	f9,
	//#define GLFW_KEY_F10                299
	f10,
	//#define GLFW_KEY_F11                300
	f12,
	//#define GLFW_KEY_F12                301
	//#define GLFW_KEY_F13                302
	//#define GLFW_KEY_F14                303
	//#define GLFW_KEY_F15                304
	//#define GLFW_KEY_F16                305
	//#define GLFW_KEY_F17                306
	//#define GLFW_KEY_F18                307
	//#define GLFW_KEY_F19                308
	//#define GLFW_KEY_F20                309
	//#define GLFW_KEY_F21                310
	//#define GLFW_KEY_F22                311
	//#define GLFW_KEY_F23                312
	//#define GLFW_KEY_F24                313
	//#define GLFW_KEY_F25                314
	//#define GLFW_KEY_KP_0               320
	keypad_number_0 = 320,
	//#define GLFW_KEY_KP_1               321
	keypad_number_1,
	//#define GLFW_KEY_KP_2               322
	keypad_number_2,
	//#define GLFW_KEY_KP_3               323
	keypad_number_3,
	//#define GLFW_KEY_KP_4               324
	keypad_number_4,
	//#define GLFW_KEY_KP_5               325
	keypad_number_5,
	//#define GLFW_KEY_KP_6               326
	keypad_number_6,
	//#define GLFW_KEY_KP_7               327
	keypad_number_7,
	//#define GLFW_KEY_KP_8               328
	keypad_number_8,
	//#define GLFW_KEY_KP_9               329
	keypad_number_9,
	//#define GLFW_KEY_KP_DECIMAL         330
	keypad_decimal,
	//#define GLFW_KEY_KP_DIVIDE          331
	keypad_divide,
	//#define GLFW_KEY_KP_MULTIPLY        332
	keypad_multiplay,
	//#define GLFW_KEY_KP_SUBTRACT        333
	keypad_subtract,
	//#define GLFW_KEY_KP_ADD             334
	keypad_add,
	//#define GLFW_KEY_KP_ENTER           335
	keypad_enter,
	//#define GLFW_KEY_KP_EQUAL           336
	keypad_equal,
	//#define GLFW_KEY_LEFT_SHIFT         340
	shift_left = 340,
	//#define GLFW_KEY_LEFT_CONTROL       341
	control_left,
	//#define GLFW_KEY_LEFT_ALT           342
	alt_left,
	//#define GLFW_KEY_LEFT_SUPER         343
	super_left,
	//#define GLFW_KEY_RIGHT_SHIFT        344
	shift_right,
	//#define GLFW_KEY_RIGHT_CONTROL      345
	control_right,
	//#define GLFW_KEY_RIGHT_ALT          346
	alt_right,
	//#define GLFW_KEY_RIGHT_SUPER        347
	super_right,
	//#define GLFW_KEY_MENU               348
	menu,
	//#define GLFW_KEY_LAST               GLFW_KEY_MENU
	LAST_KEY

};

enum KeyModifier {

	//#define 	GLFW_MOD_SHIFT   0x0001
	shift = 0x0001,
	//#define 	GLFW_MOD_CONTROL   0x0002
	control = 0x0002,
	//#define 	GLFW_MOD_ALT   0x0004
	alt = 0x0004,
	//#define 	GLFW_MOD_SUPER   0x0008
	super = 0x0005,

};

#endif