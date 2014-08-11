gccFlags = -O3 

all: expr 

expr: queue.o stack.o operations.o expr.o ; gcc $(gccFlags) -lm ../bin/expr.o ../bin/operations.o ../bin/queue.o ../bin/stack.o -o ../bin/expr

expr.o: expr.c ; gcc $(gccFlags) -c expr.c -o ../bin/expr.o
operations.o: operations.c ; gcc $(gccFlags) -c operations.c -o ../bin/operations.o
queue.o: queue.c ; gcc $(gccFlags) -c queue.c -o ../bin/queue.o
stack.o: stack.c ; gcc $(gccFlags) -c stack.c -o ../bin/stack.o

