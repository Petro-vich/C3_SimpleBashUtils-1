all: s21_grep

s21_grep:
	gcc -Wall -Werror -g -Wextra -std=c11  s21_grep.c -o s21_grep -D_POSIX_C_SOURCE=201709L

clear:
	rm s21_grep

clean:
	rm s21_grep

rebuild: clear build

test:
	bash grep_test.sh

big_test:
	bash tests/tests.sh

memory:
	bash tests/test_leak.sh
