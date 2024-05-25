FLAGS =-Wall -Werror -Wextra

s21_cat: 
	gcc $(FLAGS) s21_cat.c s21_cat.h -o s21_cat
	cp ../../materials/linters/.clang-format ./.clang-format
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c
	clang-format -n s21_cat.c s21_cat.h
clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf s21_cat *.dSYM
	rm -rf .clang-format
	rm -rf test_cat/*.o
	rm -rf test_cat/s21_cat *.dSYM