CPP = clang++
OPTS = -std=c++17 -L /System/Library/Frameworks -framework GLUT -framework OpenGL
GLFW = /opt/homebrew/Cellar/glfw/3.3.9/include/

main:
	$(CPP) $(OPTS) -o main main.cpp
	./main
	rm main
	
clean:
	rm main