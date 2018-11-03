main:
	g++ -o main -I ./include -std=c++11 main.cpp ./src/Date.cpp ./src/Meeting.cpp \
	./src/Storage.cpp ./src/User.cpp ./src/AgendaService.cpp

clean:
	rm -rf main