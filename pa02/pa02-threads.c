#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREADS 8

int found = 0;

void *thread_handler(void *arg) {
	unsigned long thread_num = (unsigned long) arg;
	unsigned long rand = time(NULL) * thread_num * thread_num;

	mpz_t p, q;
	mpz_inits(p, q, NULL);
	
	gmp_randstate_t rndstate;
    gmp_randinit_default(rndstate);
    gmp_randseed_ui(rndstate, rand);

	// Find q and p such that p, q are odd primes and p = 2q + 1
	int i = 0;
	do {
		if (found) return (void *)"-1";
		do {
			if (found) return (void *)"-1";
			mpz_urandomb(q, rndstate, 500);
		} while (!mpz_probab_prime_p(q, 15));
		
		mpz_add(p, q, q);
		mpz_add_ui(p, p, 1);
		
		if (++i % 50 == 0) printf("Thread %ld: %d tries\n", thread_num, i);
		
	} while (!mpz_probab_prime_p(p, 15));
	
	found = 1;
	char *pstr = mpz_get_str(NULL, 10, p); 
	
	printf("Thread %ld found p: %s\n", thread_num, pstr);
	found = 1;
	
	return (void *)pstr;
}

int main(void) {
	// Inititalize mpz vars, vars, and hardcoded values
	char buffer[BUFSIZ];
	char *res;
	mpz_t c1,c2,x,p,q,m,h,g,g1,g2,gq,gp,psub1,tmp,r,hr,qsize;
    mpz_inits(c1,c2,x,p,q,m,h,g,g1,g2,gq,gp,psub1,tmp,r,hr,qsize,NULL);
	mpz_set_str(x, "1234567890123456789012345678901234567890", 10);

	// Read input file and store in m
	FILE *input = fopen("./input", "r");
	fread(buffer, 1, 5000, input);
	mpz_set_str(m, buffer, 0);
	
	// Init randstate
	gmp_randstate_t rndstate;
    gmp_randinit_default(rndstate);
    gmp_randseed_ui(rndstate, (unsigned long)time(NULL));
	
	// Find p with threads
	pthread_t threads[MAX_THREADS];
	for (size_t i = 0; i < MAX_THREADS; i++) {
		pthread_create(&threads[i], NULL, thread_handler, (void *)i);
	}
	
	for (size_t i = 0; i < MAX_THREADS; i++) {
		pthread_join(threads[i], (void **)&res);
		if (strcmp(res, "-1")) {
			mpz_set_str(p, res, 10);
		}
	}
	
	mpz_sub_ui(psub1, p, 1);	
	mpz_divexact_ui(q, psub1, 2);
	
	
	// Find the generator and compute h
	do {
		mpz_urandomm(g, rndstate, p);
		mpz_powm_ui(g1, g, 1, p);
		mpz_powm_ui(g2, g, 2, p);
		mpz_powm(gq, g, q, p);
		mpz_powm(gp, g, psub1, p);
	} 	while(g == 0 || mpz_cmp_ui(g1,1) == 0 || mpz_cmp_ui(g2,1) == 0 || mpz_cmp_ui(gq,1) == 0 || mpz_cmp_ui(gp, 1) != 0);
	
	mpz_mul(g, g, g); // g = g^2 mod p
	mpz_mod(g, g, p); 
	
	mpz_powm(h, g, x, p); // h = g^x mod p
	
	
	// Encrypt 3 times and write to output string
	char omsg[BUFSIZ];
	strcpy(omsg, "");
	for (int i = 0; i < 3; i++) {
		mpz_urandomm(r, rndstate, p); // r = random int from 0 to (p - 1)
		
		mpz_powm(hr, h, r, p); // hr = h^r mod p
		
		mpz_powm(c1, g, r, p); // c1 = g^r
		
		mpz_mul(c2, hr, m); // c2 = h^r * m mod p
		mpz_mod(c2, c2, p);
		
		gmp_sprintf(buffer, "%Zd,%Zd,%Zd\n", c1, c2, p);
		strcat(omsg, buffer);
	}	

	FILE *output = fopen("./output", "w+");
	fprintf(output, "%s", omsg);
	
	fclose(output);
	
	return 0;
}
