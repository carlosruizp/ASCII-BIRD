CC = gcc 
#-ansi -pedantic
#CFLAGS = -Wall
EXE = ascii-birds

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)
	

$(EXE) : % : %.o Interface.o scwindow.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o Interface.o scwindow.o -lpthread

	mv *.o build/

%.o : %.c Interface.c
	@echo "#---------------------------"
	@echo "# Generando $@"
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -c $< Interface.c

