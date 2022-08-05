// Made by mH (https://github.com/matthmr)
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "romaji.h"

#define drop(_lock) \
	lock = false; \
	throwr = '\0'; \
	(_lock) = 0
#define BIT(x) 1 << (x)
#define USAGE(x) \
	(x), sizeof ((x))

static bool lock = false;
static char* bufout = NULL;

typedef enum {
	comp = BIT (0),
	y = BIT (1),
	tsu = BIT (2),
} slock;

static const inline jchar2* jmindex (const jtableman* syl, char c) {
	switch (c) {
	case 'a': return &syl->_a;
	case 'i': return &syl->_i;
	case 'u': return &syl->_u;
	case 'e': return &syl->_e;
	case 'o': return &syl->_o;
	case '\0': return &syl->_;
	case '\1':  return &syl->_ALT;
	}
	return NULL;
}

// situational `bufout' append
static unsigned int bondx = 0;
static void inline abufappend (const char c) {
	bufout[bondx] = c;
	bondx++;
}
static void inline jbufappend (const jchar2 c) {
	*(int*)(bufout+bondx) = *(int*)c;
	*(short*)(bufout+bondx+4) = *(short*)(c+4);
	unsigned int i = bondx, lim = (bondx + sizeof (jchar2));
	while (bufout[i] && i <= lim) i++;
	bondx = i;
}

static char wabuf[4];
int wandx = 0;
static void inline wabufappend (char c) {
	wabuf[wandx] = c;
	wandx++;
}
static void inline waflush (void) {
	*(int*)wabuf = 0;
	wandx = 0;
}
static void inline wawrite (void) {
	*(int*)(bufout+bondx) = *(int*)wabuf;
	bondx += sizeof (int);
	waflush ();
}

static char throwr = '\0';
static jtable* ccr = NULL;
static slock jlock = 0;

static void inline delim (char c, const jchar2 jc) {
	if (throwr && ! jman[throwr ascii].comp.it) {
		waflush ();
		jbufappend (jman[throwr ascii].man._);
	}
	else if (throwr) {
		wawrite ();
	}
	drop (jlock);
	if (jc) jbufappend (jc);
	return;
}

void throw (char c) {
	char cc, yc;

	if (!lock) {
		lock = true;
		throwr = c;
		wabufappend (c);
		return;
	}

	{ // <resolve>
		if (jlock & y) {
			if (! VOWEL (c)) {
				wawrite ();
				goto y_out;
			}
			jtableman* jtman = (jtableman*) ccr->comp.y.as;
			jchar2* jmbc = (jchar2*) jmindex (&ccr->man, ccr->comp.y.it ASCII);
			jbufappend (*jmbc);
			jmbc = (jchar2*) jmindex (jtman, c);
			jbufappend (*jmbc);
			y_out: {
				ccr = NULL;
				waflush ();
				drop (jlock);
				return;
			}
		}
		else if (jlock & comp) {
			if (! VOWEL (c)) {
				wawrite ();
				goto comp_out;
			}
			jtableman* jtman = (jtableman*) ccr->comp.conson.as;
			jchar2* jmbc = (jchar2*) jmindex (&ccr->man, ccr->comp.conson.base ASCII);
			if (c != ccr->comp.conson.ign ASCII) {
				jbufappend (*jmbc);
				jmbc = (jchar2*) jmindex (jtman, c);
			}
			jbufappend (*jmbc);
			comp_out: {
				ccr = NULL;
				waflush ();
				drop (jlock);
				return;
			}
		}
		else if (jlock & tsu) {
			if (! VOWEL (c)) {
				if (ccr->comp.conson.it ASCII != DONT && c == ccr->comp.conson.it ASCII) {
					wabufappend (c);
					jlock = comp;
					return;
				}
				else if (ccr->comp.y.it ASCII != DONT && c == 'y') {
					wabufappend (c);
					jlock = y;
					return;
				}
				else {
					wawrite ();
					goto tsu_out;
				}
			}
			const jchar2* jmc = jmindex (&jman[throwr ascii].man, c);
			jbufappend (*jmc);
			tsu_out: {
				ccr = NULL;
				waflush ();
				drop (jlock);
				return;
			}
		}
		else if (VOWEL (c)) {
			const jchar2* jmc = jmindex (&jman[throwr ascii].man, c);
			jbufappend (*jmc);
			ccr = NULL;
			waflush ();
			drop (jlock);
			return;
		}
	} // </resolve>

	{ // <set>
		if (c == throwr) {
			if (jman[throwr ascii].comp.it) {
				jlock |= tsu;
				jbufappend (DOUBLE);
				ccr = (jtable*) &jman[throwr ascii];
			}
			else {
				jbufappend (jman[throwr ascii].man._);
			}
			return;
		}

		cc = jman[throwr ascii].comp.conson.it ASCII;
		if ((cc ascii) != DONT) {
			if (c == cc) {
				wabufappend (c);
				ccr = (jtable*) &jman[throwr ascii];
				jlock |= comp;
			}
			else {
				wabufappend (c);
				wawrite ();
				drop (jlock);
			}
			return;
		}

		yc = jman[throwr ascii].comp.y.it ASCII;
		if ((yc ascii) != DONT) {
			if (c == 'y') {
				wabufappend (c);
				ccr = (jtable*) &jman[throwr ascii];
				jlock |= y;
				return;
			}
		}

		if (! jman[throwr ascii].comp.it) {
			waflush ();
			jbufappend (jman[throwr ascii].man._);
			throwr = c;
			wabufappend (c);
		}
		else {
			wawrite ();
			drop (jlock);
		}
		return;
	} // </set>
}

// convert romaji to hiragana
void convert (char c, jchar2* jc) {
	if (VOWEL (c)) {
		if (lock) goto throw;
		const jchar2* jmc = jmindex (&jman[_ ASCII].man, c);
		jbufappend (*jmc);
	}
	else throw: throw (c);
	return;
}

void resolve (char c) {
	if (! wabuf[0] || ! throwr) return;
	if (c == throwr) {
		waflush ();
		if (! jman[throwr ascii].comp.it) jbufappend (jman[throwr ascii].man._);
		else goto dump;
	}
	else dump: {
		abufappend (c);
	}
	return;
}

void parse (char* buf) {
	static int plen = 0;

	char c;
	jchar2 jc;
	unsigned int j;
	unsigned int len = strlen (buf);

	if (!plen) goto alloc;
	else if (len < plen) {
		bzero (bufout, bondx);
		goto loop;
	}
	else alloc: {
		if (bufout) free (bufout);
		plen = len;
		bufout = calloc (JLEN, len);
	}

	bondx = 0;

	loop: for (j = 0; buf[j] != '\0'; j++) {
		c = buf[j];
		if (ALPHA (c)) {
			c = INS (c);
			convert (c, &jc);
		}
		else switch (c) {
			case ' ': case '\t': case '\r': case '\n': case '-':
				delim (c, DONT); break;
			case '.': delim (c, jpman[_POINT]); break;
			case ',': delim (c, jpman[_COMMA]); break;
			default:  abufappend (c); continue;
		}
	}
	resolve (c);
}

int main (int argc, char** argv) {
	for (unsigned int i = 1; i < argc; i++) {
		parse (argv[i]);
		write (1, bufout, bondx);
	}

	if (argc > 1) {
		write (1, "\n", 2);
		free (bufout);
	}
	else {
		write (2, USAGE ("Usage: romaji \"romaji sentence\"\n"));
		return 1;
	}

	return 0;
}
