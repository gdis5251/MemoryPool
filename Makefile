Main:Main.cpp
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm Main
