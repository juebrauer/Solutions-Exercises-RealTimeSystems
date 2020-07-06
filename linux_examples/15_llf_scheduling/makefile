scheduling_simulation: scheduling_simulation.o scheduler.o scheduler_llf.o
	g++ scheduling_simulation.o scheduler.o scheduler_llf.o -o scheduling_simulation

scheduling_simulation.o: scheduling_simulation.cpp math_functions.h
	g++ -c scheduling_simulation.cpp -o scheduling_simulation.o

scheduler.o: scheduler.h scheduler.cpp
	g++ -c scheduler.cpp -o scheduler.o

scheduler_llf.o: scheduler_llf.h scheduler_llf.cpp
	g++ -c scheduler_llf.cpp -o scheduler_llf.o

