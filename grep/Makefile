CC = gcc
CCFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize=null -fno-sanitize=alignment

all: s21_grep

s21_grep:
	$(CC) $(CCFLAGS) grep.c -o s21_grep

clean: clear_test
	find ./../ -type f -name 's21_*'  		-delete
	find ./../ -type f -name '*.gch'  			-delete
	find ./../ -type f -name '*.o'    			-delete
	find ./../ -type f -name 'a'      			-delete
	find ./../ -name '*.dSYM'      				-exec rm -rv {} +
	find ./../ -type f -name '*.out'  			-delete

format_project:
	find ../ -iname '*.c' | xargs clang-format -i
	find ../ -iname '*.h' | xargs clang-format -i
	cppcheck --check-config --enable=all --suppress=missingIncludeSystem ../

tests:
	$(CC) $(CCFLAGS) test_grep.c -o s21_test_grep
	./s21_test_grep

clear_test:
	find ./../ -type f -name 'tt*'  		-delete
	find ./../ -type f -name 'tt'  		-delete