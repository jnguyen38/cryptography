#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>




int main(void) {
	char str[5000];
	strcpy(str, "43192948463641181942090577314551584651025255538733122061234447702636960034824773");
	strcat(str, "55885797807129879290928577803689478296466835207392280237659706922365798510232068");
	strcat(str, "69657001834751050149401001797187986054845949861871679993579210113503415502487211");
	strcat(str, "58989295278813768097258125503405814114085358966553755957702997358552797883361121");
	strcat(str, "50915097582151004668053395215483570227175184774860777330685041846744196582049775");
	strcat(str, "20631702571574531057585805866364841456138421456889927800288414662050181926724773");
	strcat(str, "48166501351749428339059745198436273929489799060791339383360502690058211546838392");
	strcat(str, "928193643065856362609720973778929485449586258569194767823");

	// Inititalize mpz vars, vars, and hardcoded values
	char buffer[BUFSIZ];
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

	// Find q and p such that p, q are odd primes and p = 2q + 1
	/*
	int i = 0;
	do {
		do {
			mpz_urandomb(q, rndstate, 2048);
		} while (!mpz_probab_prime_p(q, 15));
		
		mpz_add(p, q, q);
		mpz_add_ui(p, p, 1);
		
		if (++i % 50 == 0) printf("%d tries\n", i);
		
	} while (!mpz_probab_prime_p(p, 15));
	
	mpz_sub_ui(psub1, p, 1);	
	printf("%d total tries\n", i);
	*/
	mpz_set_str(p, str, 10);
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
