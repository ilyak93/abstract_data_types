# Macros
CC = gcc
COMP_FLAG = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
LIB_FLAG = -L. -lmtm -lex1

# Main target

#tests
mtm_yad3: yad3.o realtor.o customer.o mtm_yad3.o
	$(CC) $(COMP_FLAG) yad3.o realtor.o customer.o \
mtm_yad3.o $(LIB_FLAG) -o  $@
yad3.o: yad3.c yad3.h customer.h realtor.h \
list.h apartment_service.h apartment.h mtm_ex2.h list.h map.h
	$(CC) -c $(COMP_FLAG) $(LIB_FLAG) $*.c
realtor.o: realtor.c realtor.h apartment_service.h apartment.h map.h
	$(CC) -c $(COMP_FLAG) $(LIB_FLAG) $*.c
customer.o: customer.c map.h customer.h
	$(CC) -c $(COMP_FLAG) $(LIB_FLAG) $*.c
mtm_yad3.o: mtm_yad3.c mtm_ex2.h yad3.h customer.h \
realtor.h apartment_service.h apartment.h
	$(CC) -c $(COMP_FLAG) $(LIB_FLAG) $*.c
	
#Tests	
tests: yad3_test realtor_test customer_test list_mtm_test

#yad3_test	
yad3_test: tests/yad3_test.o yad3.o realtor.o customer.o
	$(CC) $(COMP_FLAG) tests/yad3_test.o yad3.o \
realtor.o customer.o $(LIB_FLAG) -o $@
tests/yad3_test.o : tests/yad3_test.c 
	$(CC) $(COMP_FLAG) $(LIB_FLAG) $*.c -c -o $@
#realtor_test
realtor_test: tests/realtor_test.o realtor.o
	$(CC) $(COMP_FLAG) tests/realtor_test.o realtor.o $(LIB_FLAG) -o $@
tests/realtor_test.o : tests/realtor_test.c 
	$(CC) $(COMP_FLAG) $(LIB_FLAG) $*.c -c -o $@
#customer_test
customer_test: tests/customer_test.o customer.o
	$(CC) $(COMP_FLAG) tests/customer_test.o customer.o $(LIB_FLAG) -o $@
tests/customer_test.o : tests/customer_test.c 
	$(CC) $(COMP_FLAG) $(LIB_FLAG) $*.c -c -o $@
#list_mtm_test
list_mtm_test: list_mtm/list_mtm.o tests/list_mtm_test.o
	$(CC) $(COMP_FLAG) tests/list_mtm_test.o \
list_mtm/list_mtm.o $(LIB_FLAG) -o $@
tests/list_mtm_test.o : list_mtm/list_mtm.o tests/list_mtm_test.c 
	$(CC) $(COMP_FLAG) $(LIB_FLAG) $*.c -c -o $@
list_mtm/list_mtm.o: list_mtm/list_mtm.c list_mtm/list_mtm.h
	$(CC) -c $(COMP_FLAG) $(LIB_FLAG) $*.c -o $@