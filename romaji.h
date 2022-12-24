#ifndef LOCK_ROMAJI
#  define LOCK_ROMAJI

#  define JLEN 3 // the amount of bytes per hiragana character
#  define INS(x) ((x) | 0x20) // always lowercase
#  define ascii - 95
#  define ASCII + 95

#  define VOWEL(x) (\
    ((x) == 'a') || \
    ((x) == 'i') || \
    ((x) == 'u') || \
    ((x) == 'e') || \
    ((x) == 'o'))
#  define ALPHA(x) (((x) >= 'a' && (x) <= 'z') || ((x) >= 'A' && (x) <= 'Z'))

#  define DOUBLE "っ\000\000"
#  define DONT '\0'
#  define LVL(x) ((x)*5)

#  define true '\1'
#  define false '\0'

typedef char jchar[JLEN];

enum JA_VOWELS {
	_ = -95, _ALT = -94,
	_A = 'a' ascii,
	_I = 'i' ascii,
	_U = 'u' ascii,
	_E = 'e' ascii,
	_O = 'o' ascii,
};
typedef enum JA_VOWELS jvow;

enum JA_CONSONANTS {
	_N = 'n' ascii,
	_Y = 'y' ascii,
	_W = 'w' ascii,
	_C = 'c' ascii,
	_F = 'f' ascii,
	_M = 'm' ascii,
	_R = 'r' ascii,
	_T = 't' ascii,
	_H = 'h' ascii,
	_P = 'p' ascii,
	_B = 'b' ascii,
	_S = 's' ascii,
	_J = 'j' ascii,
	_D = 'd' ascii,
	_G = 'g' ascii,
	_K = 'k' ascii,
	_Z = 'z' ascii,
	_Q = 'q' ascii,
	_X = 'x' ascii,
	_L = 'l' ascii,
	_V = 'v' ascii,
};
typedef enum JA_CONSONANTS jconson;

enum JA_PONCTUATION {
	_POINT, _COMMA, //_LONG, //_SPACE,
};
typedef enum JA_PONCTUATION jponct;

typedef char bool;
typedef char jchar2[JLEN*2]; // double the size to accomodate the compounding hiragana
typedef struct {
	const jchar2 _, _a, _i, _u, _e, _o, _ALT;
} jtableman;
typedef struct {
	const int it, base, ign;
	const jtableman* as;
} jcompconson;
typedef struct {
	const int it;
	const jtableman* as;
} jcompy;
typedef struct {
	const jcompy y;
	const jcompconson conson;
	const bool it;
} jcomp;
typedef struct {
	const jcomp comp;
	const jtableman man;
} jtable;

const jtableman jyman = {
	._ = DONT,
	._a = "ゃ",
	._i = "ぃ",
	._u = "ゅ",
	._e = "ぇ",
	._o = "ょ",
	._ALT = DONT,
};

const jtableman jvman = {
	._ = DONT,
	._a = "ぁ",
	._i = "ぃ",
	._u = "ぅ",
	._e = "ぇ",
	._o = "ぉ",
	._ALT = DONT,
};

const jtableman jkman = {
	._ = DONT,
	._a = "ぁ",
	._i = "ぃ",
	._u = "ぅ",
	._e = "ぇ",
	._o = "ぉ",
	._ALT = DONT,
};

const jtable jman[] = {
	[_ ASCII] = {
		.comp = {
			.it = true,
			.y = {
				.it = DONT,
				.as = NULL,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			}
		},
		.man = {
			._ = DONT,
			._a = "あ",
			._i = "い",
			._u = "う",
			._e = "え",
			._o = "お",
			._ALT = DONT,
		},
	},

	[_X] = {
		.comp = {
			.it = true,
			.y = {
				.it = _ALT,
				.as = &jyman,
			},
			.conson = {
				.it = _K,
				.base = _ALT,
				.ign = DONT,
				.as = &jkman,
			},
		},
		.man = {
			._ = DONT,
			._a = "ぁ",
			._i = "ぃ",
			._u = "ぅ",
			._e = "ぇ",
			._o = "ぉ",
			._ALT = DONT,
		},
	},

	[_L] = {
		.comp = {
			.it = true,
			.y = {
				.it = _ALT,
				.as = &jyman,
			},
			.conson = {
				.it = _K,
				.base = _ALT,
				.ign = DONT,
				.as = &jkman,
			},
		},
		.man = {
			._ = DONT,
			._a = "ぁ",
			._i = "ぃ",
			._u = "ぅ",
			._e = "ぇ",
			._o = "ぉ",
			._ALT = DONT,
		},
	},

	[_V] = {
		.comp = {
			.it = true,
			.y = {
				.it = _U,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "ゔぁ",
			._i = "ゔぃ",
			._u = "ゔ",
			._e = "ゔぇ",
			._o = "ゔぉ",
			._ALT = DONT,
		},
	},

	[_Y] = {
		.comp = {
			.it = true,
			.y = {
				.it = DONT,
				.as = NULL,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "や",
			._i = DONT,
			._u = "ゆ",
			._e = "いぇ",
			._o = "よ",
			._ALT = DONT,
		},
	},

	[_W] = {
		.comp = {
			.it = true,
			.y = {
				.it = DONT,
				.as = NULL,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "わ",
			._i = "ゐ",
			._u = DONT,
			._e = "ゑ",
			._o = "を",
			._ALT = DONT,
		},
	},

	[_R] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "ら",
			._i = "り",
			._u = "る",
			._e = "れ",
			._o = "ろ",
			._ALT = DONT,
		},
	},

	[_T] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = _S,
				.base = _U,
				.ign = _U,
				.as = &jvman,
			},
		},
		.man = {
			._ = DONT,
			._a = "た",
			._i = "ち",
			._u = "つ",
			._e = "て",
			._o = "と",
			._ALT = DONT,
		},
	},

	[_F] = {
		.comp = {
			.it = true,
			.y = {
				.it = _U,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "ふぁ",
			._i = "ふぃ",
			._u = "ふ",
			._e = "ふぇ",
			._o = "ふぉ",
			._ALT = DONT,
		},
	},

	[_H] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "は",
			._i = "ひ",
			._u = "ふ",
			._e = "へ",
			._o = "ほ",
			._ALT = DONT,
		},
	},

	[_P] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "ぱ",
			._i = "ぴ",
			._u = "ぷ",
			._e = "ぺ",
			._o = "ぽ",
			._ALT = DONT,
		},
	},

	[_B] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "ば",
			._i = "び",
			._u = "ぶ",
			._e = "べ",
			._o = "ぼ",
			._ALT = DONT,
		},
	},

	[_S] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = _H,
				.base = _I,
				.ign = _I,
				.as = &jyman,
			},
		},
		.man = {
			._ = DONT,
			._a = "さ",
			._i = "し",
			._u = "す",
			._e = "せ",
			._o = "そ",
			._ALT = DONT,
		},
	},

	[_D] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = _Z,
				.base = _U,
				.ign = _U,
				.as = &jvman,
			},
		},
		.man = {
			._ = DONT,
			._a = "だ",
			._i = "じ",
			._u = "づ",
			._e = "で",
			._o = "ど",
			._ALT = DONT,
		},
	},

	[_G] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "が",
			._i = "ぎ",
			._u = "ぐ",
			._e = "げ",
			._o = "ご",
			._ALT = DONT,
		},
	},

	[_K] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "か",
			._i = "き",
			._u = "く",
			._e = "け",
			._o = "こ",
			._ALT = DONT,
		},
	},

	[_C] = {
		.comp = {
			.it = true,
			.y = {
				.it = _ALT,
				.as = &jyman,
			},
			.conson = {
				.it = _H,
				.base = _ALT,
				.ign = _I,
				.as = &jyman,
			},
		},
		.man = {
			._ = DONT,
			._a = "か",
			._i = "し",
			._u = "く",
			._e = "せ",
			._o = "こ",
			._ALT = "ち",
		},
	},

	[_Z] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "ざ",
			._i = "じ",
			._u = "ず",
			._e = "ぜ",
			._o = "ぞ",
			._ALT = DONT,
		},
	},

	[_N] = {
		.comp = {
			.it = false,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = "ん",
			._a = "な",
			._i = "に",
			._u = "ぬ",
			._e = "ね",
			._o = "の",
			._ALT = DONT,
		},
	},

	[_M] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "ま",
			._i = "み",
			._u = "む",
			._e = "め",
			._o = "も",
			._ALT = DONT,
		},
	},

	[_Q] = {
		.comp = {
			.it = true,
			.y = {
				.it = DONT,
				.as = NULL,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "くぁ",
			._i = "くぃ",
			._u = "く",
			._e = "くぇ",
			._o = "くぉ",
			._ALT = DONT,
		},
	},

	[_J] = {
		.comp = {
			.it = true,
			.y = {
				.it = _I,
				.as = &jyman,
			},
			.conson = {
				.it = DONT,
				.base = DONT,
				.ign = DONT,
				.as = NULL,
			},
		},
		.man = {
			._ = DONT,
			._a = "じゃ",
			._i = "じ",
			._u = "じゅ",
			._e = "じぇ",
			._o = "じょ",
			._ALT = DONT,
		},
	},
};

const jchar2 jpman[] = {
	[_POINT] = "。\000\000",
	[_COMMA] = "、\000\000",
	//[_LONG] = "ー\000\000",
	//[_SPACE] = "　",
};

#endif
