demo: cli srv

cli: common.o cli.cc
	g++ cli.cc common.o -o cli

srv: common.o srv.cc
	g++ srv.cc common.o -o srv

common.o: common.h common.cc
	g++ -I ./common.h -c common.cc -o common.o

clean:
	rm cli srv common.o
