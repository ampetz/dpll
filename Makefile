main:
	clang++ -std=c++1y ./src/main5.cc -I ./include -I /opt/local/include -o ./bin/dpll

clean:
	rm ./bin/*
