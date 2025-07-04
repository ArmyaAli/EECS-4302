#include "include/hash_table.h"

#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 127
#define DEFAULT_LOAD 0.75
#define DEFAULT_FUNC hash_string


struct hash_table *hash_table_create(int bucket_count, hash_func_t func)
{
	struct hash_table *h;

	h = (struct hash_table *) malloc(sizeof(struct hash_table));
	if(!h)
		return 0;

	if(bucket_count < 1)
		bucket_count = DEFAULT_SIZE;
	if(!func)
		func = DEFAULT_FUNC;

	h->size = 0;
	h->hash_func = func;
	h->bucket_count = bucket_count;
	h->buckets = (struct entry **) calloc(bucket_count, sizeof(struct entry *));
	if(!h->buckets) {
		free(h);
		return 0;
	}

	return h;
}

void hash_table_clear(struct hash_table *h)
{
	struct entry *e, *f;
	int i;

	for(i = 0; i < h->bucket_count; i++) {
		e = h->buckets[i];
		while(e) {
			f = e->next;
			free(e->key);
			free(e);
			e = f;
		}
	}

	for(i = 0; i < h->bucket_count; i++) {
		h->buckets[i] = 0;
	}
}


void hash_table_delete(struct hash_table *h)
{
	hash_table_clear(h);
	free(h->buckets);
	free(h);
}

void *hash_table_lookup(struct hash_table *h, const char *key)
{
	struct entry *e;
	unsigned hash, index;

	hash = h->hash_func(key);
	index = hash % h->bucket_count;
	e = h->buckets[index];

	while(e) {
		if(hash == e->hash && !strcmp(key, e->key)) {
			return e->value;
		}
		e = e->next;
	}

	return 0;
}

int hash_table_size(struct hash_table *h)
{
	return h->size;
}

static int hash_table_double_buckets(struct hash_table *h)
{
	struct hash_table *hn = hash_table_create(2 * h->bucket_count, h->hash_func);

	if(!hn)
		return 0;

	/* Move pairs to new hash */
	char *key;
	void *value;
	hash_table_firstkey(h);
	while(hash_table_nextkey(h, &key, &value))
		if(!hash_table_insert(hn, key, value))
		{
			hash_table_delete(hn);
			return 0;
		}

	/* Delete all old pairs */
	struct entry *e, *f;
	int i;
	for(i = 0; i < h->bucket_count; i++) {
		e = h->buckets[i];
		while(e) {
			f = e->next;
			free(e->key);
			free(e);
			e = f;
		}
	}

	/* Make the old point to the new */
	free(h->buckets);
	h->buckets      = hn->buckets;
	h->bucket_count = hn->bucket_count;
	h->size         = hn->size;

	/* Delete reference to new, so old is safe */
	free(hn);

	return 1;
}

int hash_table_insert(struct hash_table *h, const char *key, const void *value)
{
	struct entry *e;
	unsigned hash, index;

	if( ((float) h->size / h->bucket_count) > DEFAULT_LOAD )
		hash_table_double_buckets(h);

	hash = h->hash_func(key);
	index = hash % h->bucket_count;
	e = h->buckets[index];

//NOTE(Ali): If key exists, we will overrite that. For markers.
//	while(e) {
//		if(hash == e->hash && !strcmp(key, e->key)) {
//      printf("bob\n");
//    }
//			return 0;
//		e = e->next;
//	}

	e = (struct entry *) malloc(sizeof(struct entry));
	if(!e)
		return 0;

	e->key = strdup(key);
	if(!e->key) {
		free(e);
		return 0;
	}

	e->value = (void *) value;
	e->hash = hash;
	e->next = h->buckets[index];
	h->buckets[index] = e;
	h->size++;

	return 1;
}

void *hash_table_remove(struct hash_table *h, const char *key)
{
	struct entry *e, *f;
	void *value;
	unsigned hash, index;

	hash = h->hash_func(key);
	index = hash % h->bucket_count;
	e = h->buckets[index];
	f = 0;

	while(e) {
		if(hash == e->hash && !strcmp(key, e->key)) {
			if(f) {
				f->next = e->next;
			} else {
				h->buckets[index] = e->next;
			}
			value = e->value;
			free(e->key);
			free(e);
			h->size--;
			return value;
		}
		f = e;
		e = e->next;
	}

	return 0;
}

void hash_table_firstkey(struct hash_table *h)
{
	h->ientry = 0;
	for(h->ibucket = 0; h->ibucket < h->bucket_count; h->ibucket++) {
		h->ientry = h->buckets[h->ibucket];
		if(h->ientry)
			break;
	}
}

int hash_table_nextkey(struct hash_table *h, char **key, void **value)
{
	if(h->ientry) {
		*key = h->ientry->key;
		*value = h->ientry->value;

		h->ientry = h->ientry->next;
		if(!h->ientry) {
			h->ibucket++;
			for(; h->ibucket < h->bucket_count; h->ibucket++) {
				h->ientry = h->buckets[h->ibucket];
				if(h->ientry)
					break;
			}
		}
		return 1;
	} else {
		return 0;
	}
}

typedef unsigned long int ub4;	/* unsigned 4-byte quantities */
typedef unsigned char ub1;	/* unsigned 1-byte quantities */

#define hashsize(n) ((ub4)1<<(n))
#define hashmask(n) (hashsize(n)-1)

/*
--------------------------------------------------------------------
mix -- mix 3 32-bit values reversibly.
For every delta with one or two bits set, and the deltas of all three
  high bits or all three low bits, whether the original value of a,b,c
  is almost all zero or is uniformly distributed,
* If mix() is run forward or backward, at least 32 bits in a,b,c
  have at least 1/4 probability of changing.
* If mix() is run forward, every bit of c will change between 1/3 and
  2/3 of the time.  (Well, 22/100 and 78/100 for some 2-bit deltas.)
mix() was built out of 36 single-cycle latency instructions in a
  structure that could supported 2x parallelism, like so:
	  a -= b;
	  a -= c; x = (c>>13);
	  b -= c; a ^= x;
	  b -= a; x = (a<<8);
	  c -= a; b ^= x;
	  c -= b; x = (b>>13);
	  ...
  Unfortunately, superscalar Pentiums and Sparcs can't take advantage
  of that parallelism.  They've also turned some of those single-cycle
  latency instructions into multi-cycle latency instructions.  Still,
  this is the fastest good hash I could find.  There were about 2^^68
  to choose from.  I only looked at a billion or so.
--------------------------------------------------------------------
*/

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

/*
--------------------------------------------------------------------
hash() -- hash a variable-length key into a 32-bit value
  k       : the key (the unaligned variable-length array of bytes)
  len     : the length of the key, counting by bytes
  initval : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Every 1-bit and 2-bit delta achieves avalanche.
About 6*len+35 instructions. The best hash table sizes are powers of 2.  There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.  For example, if you need only 10 bits, do
  h = (h & hashmask(10));
In which case, the hash table should have hashsize(10) elements. If you are hashing n strings (ub1 **)k, do it like this:
  for (i=0, h=0; i<n; ++i) h = hash( k[i], len[i], h);

By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this code any way you wish, private, educational, or commercial.  It's free. See http://burtleburtle.net/bob/hash/evahash.html
Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
--------------------------------------------------------------------
*/

static ub4 jenkins_hash(k, length, initval)
	 register const ub1 *k;	/* the key */
	 register ub4 length;	/* the length of the key */
	 register ub4 initval;	/* the previous hash, or an arbitrary value */
{
	register ub4 a, b, c, len;	/* Set up the internal state */
	len = length;
	a = b = 0x9e3779b9;	/* the golden ratio; an arbitrary value */
	c = initval;					 /* the previous hash value *//*---------------------------------------- handle most of the key */
	while(len >= 12) {
		a += (k[0] + ((ub4) k[1] << 8) + ((ub4) k[2] << 16) + ((ub4) k[3] << 24));
		b += (k[4] + ((ub4) k[5] << 8) + ((ub4) k[6] << 16) + ((ub4) k[7] << 24));
		c += (k[8] + ((ub4) k[9] << 8) + ((ub4) k[10] << 16) + ((ub4) k[11] << 24));
		mix(a, b, c);
		k += 12;
		len -= 12;
	}
	/*------------------------------------- handle the last 11 bytes */
	c += length;
	switch (len) {		/* all the case statements fall through */
	case 11:
		c += ((ub4) k[10] << 24);
	case 10:
		c += ((ub4) k[9] << 16);
	case 9:
		c += ((ub4) k[8] << 8);
		/* the first byte of c is reserved for the length */
	case 8:
		b += ((ub4) k[7] << 24);
	case 7:
		b += ((ub4) k[6] << 16);
	case 6:
		b += ((ub4) k[5] << 8);
	case 5:
		b += k[4];
	case 4:
		a += ((ub4) k[3] << 24);
	case 3:
		a += ((ub4) k[2] << 16);
	case 2:
		a += ((ub4) k[1] << 8);
	case 1:
		a += k[0];
		/* case 0: nothing left to add */
	}
	mix(a, b, c);
   /*-------------------------------------------- report the result */
	return c;
}

unsigned hash_string(const char *s)
{
	return jenkins_hash((const ub1 *) s, strlen(s), 0);
}
