all: mutex multithread hello_omp forloop_omp section_omp section_omp2 sum_omp_bad

multithread: multithread.c
	g++ -pthread -o $@ $<
mutex: mutex.c
	g++ -pthread -o $@ $<
hello_omp: hello_omp.c
	g++ -fopenmp -o $@ $<
forloop_omp: forloop_omp.cpp
	g++ -std=c++11 -fopenmp -o $@ $<
section_omp: section_omp.cpp
	g++ -std=c++11 -fopenmp -o $@ $<
section_omp2: section_omp2.c
	g++ -std=c++11 -fopenmp -o $@ $<
sum_omp_bad: sum_omp_bad.c
	g++ -std=c++11 -fopenmp -o $@ $<
