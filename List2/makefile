helloworld: hello_world.cpp
	g++ hello_world.cpp -o helloworld -fopenmp -lpthread

loop_paralleism: loop_paralleism.cpp
	g++ loop_paralleism.cpp -o loop_paralleism -fopenmp -lpthread

firstprivate: firstprivate.cpp
	g++ firstprivate.cpp -o firstprivate -fopenmp -lpthread

lastprivate: lastprivate.cpp
	g++ lastprivate.cpp -o lastprivate -fopenmp -lpthread

ordered: ordered.cpp
	g++ ordered.cpp -o ordered -fopenmp -lpthread

scheduling: scheduling.cpp
	g++ scheduling.cpp -o scheduling -fopenmp -lpthread

pgi_firstprivate: firstprivate.cpp
	pgc++ firstprivate.cpp -g -o firstprivate -mp -lpthread