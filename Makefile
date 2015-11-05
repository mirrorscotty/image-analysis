CC=g++
CFLAGS=`pkg-config --cflags opencv` -std=c++11
LDFLAGS=`pkg-config --libs opencv`
SRC=$(wildcard *.cpp)

all: calc-area

calc-area: hough-lines.o merge-rows.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

doc: Doxyfile
	doxygen Doxyfile

clean:
	rm -rf doc kF gab fitdiff modulus
	rm -rf $(SRC:.c=.o)
	rm -rf $(SRC:.c=.d)
	rm -rf *.a
	$(MAKE) -C material-data clean
	$(MAKE) -C matrix clean

%.o: %.cpp
	$(CC) -c $(CFLAGS) $*.cpp -o $*.o
	$(CC) -MM $(CFLAGS) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
          sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

-include $(SRC:.cpp=.d)

