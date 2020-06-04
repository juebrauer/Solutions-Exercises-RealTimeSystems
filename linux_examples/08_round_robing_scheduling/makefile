scheduling_simulation: scheduling_simulation.o scheduler.o scheduler_roundrobin.o
	g++ scheduling_simulation.o scheduler.o scheduler_roundrobin.o -o scheduling_simulation

scheduling_simulation.o: scheduling_simulation.cpp
	g++ -c scheduling_simulation.cpp -o scheduling_simulation.o

scheduler.o: scheduler.h scheduler.cpp
	g++ -c scheduler.cpp -o scheduler.o

scheduler_roundrobin.o: scheduler_roundrobin.h scheduler_roundrobin.cpp
	g++ -c scheduler_roundrobin.cpp -o scheduler_roundrobin.o

