Agenda:
	g++ -o Agenda -I ./include -std=c++11 Agenda.cpp ./src/Date.cpp ./src/Meeting.cpp \
	./src/Storage.cpp ./src/User.cpp ./src/AgendaService.cpp ./src/AgendaUI.cpp ./src/Logger.cpp

clean:
	rm -rf Agenda