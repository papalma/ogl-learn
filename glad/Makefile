CC=clang

src/glad.o: src/glad.c
	$(CC) -o $@ -c $^ -I include

.PHONY:
clean:
	rm -f src/*.o
