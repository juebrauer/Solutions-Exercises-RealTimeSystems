scheduling_simulation: scheduling_simulation.o scheduler.o scheduler_edf.o
	g++ scheduling_simulation.o scheduler.o scheduler_edf.o -o scheduling_simulation

scheduling_simulation.o: scheduling_simulation.cpp math_functions.h
	g++ -c scheduling_simulation.cpp -o scheduling_simulation.o

scheduler.o: scheduler.h scheduler.cpp
	g++ -c scheduler.cpp -o scheduler.o

scheduler_edf.o: scheduler_edf.h scheduler_edf.cpp
	g++ -c scheduler_edf.cpp -o scheduler_edf.o

