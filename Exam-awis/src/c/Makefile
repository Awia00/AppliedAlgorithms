CFLAGS = -O2 -Wall -Wa,-q -g -std=c99 -fopenmp
SDS = sds.c
HASH = hash.c
REPRESENTATION = representation.c


default: sds 

sds: $(SDS)

hash: $(HASH) 

representation: $(REPRESENTATION) 

clean:
	$(RM) sds hash