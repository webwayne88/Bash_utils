FLAGS =-Wall -Werror -Wextra

s21_grep: 
	gcc $(FLAGS) s21_grep.c s21_grep.h -o s21_grep
	cp ../../materials/linters/.clang-format ./.clang-format
	cppcheck --enable=all --suppress=missingIncludeSystem s21_grep.c
	clang-format -n s21_grep.c s21_grep.h
clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf s21_grep *.dSYM
	rm -rf .clang-format
	rm -rf test_grep/*.o
	rm -rf test_grep/s21_grep *.dSYM