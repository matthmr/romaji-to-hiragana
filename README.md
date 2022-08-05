# romaji.c - Romaji to hiragana converter

Simple romaji to hiragana converter written in C.

## Building

Run `make romaji` to build the program. `make` variables are:

- `CC` : the C compiler (gcc)
- `CFLAGS` : additional flags to pass to the compiler ()

No further dependencies are needed besides `libc`.

## Using

I made this with the intent of integrating it to [libkkc](https://github.com/ueno/libkkc).
With this setup, I can write an ordinary, space-separated romaji japanese sentence
and get kana+kanji output, which is really useful for studying the language.

If you want to have a system like mine, compile this program and
write the following shell script:

```sh
[[ -z $2 ]] && LVL=2 || LVL=$2
romaji "$1" | sed "s/\$/ $LVL/g" | kkc |\
awk -F: '/^([0-9]+:|>> 0:) <[^mcQ]/ {
	line=""
	split($2, splitter, "/")
	lim=length(splitter)
	for (i = 0; i < lim; i++) {
		sub(/^([ \n]*<|.*<)/, "", splitter[i])
		line=(line splitter[i])
	}
	print line
}'
```

I wrap this shell code in a function that gets sourced by my `~/.zshrc` so I can
call this by passing the function name, instead of the script name but whichever
implementation works. The awk script will be in the repository root as `kkc.awk`,
it can be used in the output of piped kkc.

Usage alongside `kkc.awk` *should* look as such:

```txt
$ romaji "kore ha nani" | sed 's/$/ 4/' | kkc | kkc.awk
コレハナに
コレは何
これは何
これはなに
```

This repository also includes a sed script for common transliteration of misleading
particles like `wa` (は) as in (kimi wa - 君は) & `e` as in (nihon e - 日本へ).
The script is `norm.sed`.

## License

This repository is licensed under the [GPL-v3.0 License](https://opensource.org/licenses/GPL-3.0).
