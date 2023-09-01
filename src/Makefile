CC = g++ -Werror -Wextra -Wall -std=c++17
SRCS = models_lib/*.cc
HEADERS = models_lib/*.h
TESTS = calculation_tests.cc
LIB = s21_model.a
TESTEX = calculation_tests
PKG = `pkg-config --cflags --libs gtest`
DISTNAME = SmartCalc_V_2_0.tar.gz

all: install clean dvi

s21_model.a:
	$(CC) -c $(SRCS)
	ar rc $(LIB) *.o
	ranlib $(LIB)
	
install: uninstall $(LIB)
	mkdir build
	cd build && cmake ../SmartCalc_V_2_0/ && cmake --build . && rm -rf Makefile cmake_install.cmake CMakeFiles CMakeCache.txt SmartCalc_V_2_0_autogen

uninstall:
	rm -rf build

dist: all
	rm -rf $(DISTNAME)
	tar -cvzf $(DISTNAME) build/


gcov_report: clean
	$(CC) -fprofile-arcs -ftest-coverage $(SRCS) $(TESTS) -o gcovreport \
	$(PKG)
	./gcovreport
	lcov -t "gcovreport" --no-external -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info
	open report/index.html

test: clean $(LIB)
	$(CC) $(TESTS) $(LIB) -o $(TESTEX) \
	$(PKG)
	./$(TESTEX)

style:
	clang-format -n $(SRCS) $(TESTS) $(HEADERS)


leaks: clean $(LIB)
	$(CC) $(TESTS) $(LIB) -o $(TESTEX) \
	$(PKG)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TESTEX)

check: style leaks clean

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info $(TESTEX) gcovreport report 

rebuild: clean all
