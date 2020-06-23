scheduling_simulation: scheduling_simulation.o scheduler.o scheduler_rms.o
	g++ scheduling_simulation.o scheduler.o scheduler_rms.o -o scheduling_simulation

scheduling_simulation.o: scheduling_simulation.cpp
	g++ -c scheduling_simulation.cpp -o scheduling_simulation.o

scheduler.o: scheduler.h scheduler.cpp
	g++ -c scheduler.cpp -o scheduler.o

scheduler_rms.o: scheduler_rms.h scheduler_rms.cpp
	g++ -c scheduler_rms.cpp -o scheduler_rms.o

