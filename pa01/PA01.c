#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(const int argc, const char* argv[]) {
	printf("reading...\n");
	// Load file stream from input
	FILE *input;
	input = fopen("./input", "r");
	
	// Read from input file
	char string[BUFSIZ];
	fgets(string, BUFSIZ, input);
	
	printf("initializing...\n");
	// Initialize GMP variables
	mpz_t m, c, e, d, n, p, q, m_, c_, d_, n_, p_, q_, tmp, phi;
	mpz_inits(m, c, e, d, n, p, q, m_, c_, d_, n_, p_, q_, tmp, phi, NULL);
	
	printf("splitting...\n");
	// Split input string by commas into GMP variables
	char *str = strtok(string, ",");
	mpz_set_str(m, str, 0);
	str = strtok(NULL, ",");
	mpz_set_str(c_, str, 0);
	str = strtok(NULL, ",");
	mpz_set_str(d_, str, 0);
	str = strtok(NULL, ",");
	mpz_set_str(p_, str, 0);
	str = strtok(NULL, ",");
	mpz_set_str(q_, str, 0);
	fclose(input);
	
	/*****************************************/
	/* Encrypt m by generating random e and n*/	
	/*****************************************/
	printf("encrypting...\n");
	gmp_randstate_t rndstate;
    gmp_randinit_default(rndstate);
    
    mpz_set_ui(tmp, (unsigned long)time(NULL) % 10000);

    gmp_randseed(rndstate, tmp);

    mpz_urandomb(p, rndstate, 512);

    mpz_nextprime(p, p);
    mpz_nextprime(q, p);

    mpz_mul(n, p, q);
    mpz_sub_ui(q, q, 1);
	mpz_sub_ui(p, p, 1);
	
    mpz_mul(phi, p, q);
    
    mpz_urandomm(e, rndstate, phi);
    
    while(mpz_invert(d, e, phi) == 0){
        mpz_urandomm(e, rndstate, phi);
    }
    
    mpz_powm(c, m, e, n);
	
	/***********************************/
	/* Decrypt c' using d', p', and q' */
	/***********************************/
	printf("decrypting...\n");
	// Calculate n' using p' and q'
	mpz_set_ui(n_, 0);
	mpz_mul(n_, p_, q_);
	
	/*
	// Calculate phi(n) = (p-1)(q-1)
	mpz_t phi_n;
	mpz_init_set_ui(phi_n, 0);
	mpz_sub_ui(p_, p_, 1);
	mpz_sub_ui(q_, q_, 1);
	mpz_mul(phi_n, p_, q_);
	
	printf("Phi(n):\n");
	mpz_out_str(stdout, 10, phi_n);
	printf("\nn:\n");
	mpz_out_str(stdout, 10, n_);
	printf("\nd:\n");
	mpz_out_str(stdout, 10, d_);
	
	// Calculate d mod phi(n)
	mpz_mod(d_, d_, phi_n);
	printf("\nd (after):\n");
	mpz_out_str(stdout, 10, d_);
	*/

	// Calculate c'^(d') mod n'
	mpz_set_ui(m_, 0);
	mpz_powm(m_, c_, d_, n_);
	printf("\nc:\n");
	mpz_out_str(stdout, 10, c_);
	printf("\n");
	printf("\nd:\n");
	mpz_out_str(stdout, 10, d_);
	printf("\n");
	printf("\np:\n");
	mpz_out_str(stdout, 10, p_);
	printf("\n");
	printf("\nq:\n");
	mpz_out_str(stdout, 10, q_);
	printf("\n");
	printf("\nn:\n");
	mpz_out_str(stdout, 10, n_);
	printf("\n");
	printf("\nm:\n");
	mpz_out_str(stdout, 10, m_);
	printf("\n");

	// Create output file
	FILE *output;
	output = fopen("./output", "w+");
	mpz_out_str(output, 10, c);
	fprintf(output, ",");
	mpz_out_str(output, 10, e);
	fprintf(output, ",");
	mpz_out_str(output, 10, d);
	fprintf(output, ",");
	mpz_out_str(output, 10, n);
	fprintf(output, ",");
	mpz_out_str(output, 10, m_);
	fclose(output);
	
	
	// Free GMP variables
	mpz_clear(m);
	mpz_clear(c_);
	mpz_clear(d_);
	mpz_clear(p_);
	mpz_clear(q_);
	
	return 0;
}
