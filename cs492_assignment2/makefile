NAME=Assignment2exec
SRC=main.c CLOCK.c FIFO.c LRU.c
SRCPATH=./
OBJ=$(addprefix $(SRCPATH), $(SRC: .c=.o))

RM=rm -f
INCPATH=includes
CFLAGS += -std=c99 -I $(INCPATH)

all: $(OBJ)
	gcc -std=c99 $(OBJ) -o $(NAME)

clean:
	-$(RM) *~
	-$(RM) *#*
	-$(RM) *swp
	-$(RM) *.core
	-$(RM) *.stackdump
	-$(RM) $(SRCPATH)*.o
	-$(RM) $(SRCPATH)*.obj
	-$(RM) $(SRCPATH)*~
	-$(RM) $(SRCPATH)*#*
	-$(RM) $(SRCPATH)*swp
	-$(RM) $(SRCPATH)*.core
	-$(RM) $(SRCPATH)*.stackdump

fclean: clean
	-$(RM) $(NAME)

re: fclean all