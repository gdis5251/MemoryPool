Main:Main.cpp
	g++ -o $@ $^ -std=c++11 -g

.PHONY:clean
clean:
	rm Main
