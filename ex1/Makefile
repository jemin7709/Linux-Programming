all : print_func
	
print_func : main.o func_one.o func_two.o
	gcc -o print_func main.o func_one.o func_two.o
main.o : header.h main.c
	gcc main.c -c
func_one.o : header.h func_one.c
	gcc func_one.c -c
func_two.o : header.h func_two.c
	gcc func_two.c -c
clean : 
	rm -f *.o print_func
