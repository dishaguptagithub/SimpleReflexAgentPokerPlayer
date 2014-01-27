all:
	clang++ -stdlib=libc++ main.cpp -o main

clean:
	rm -f main *.txt
