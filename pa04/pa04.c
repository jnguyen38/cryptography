#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pbc.h>

int main(int argc, const char* argv[]){
    pairing_t pairing;
    element_t x,pk,g1,h,sigma;
    char s[3000] = "type a\nq 8780710799663312522437781984754049815806883199414208211028653399266475630880222957078625179422662221423155858769582317459277713367317481324925129998224791\nh 12016012264891146079388821366740534204802954401251311822919615131047207289359704531102844802183906537786776\nr 730750818665451621361119245571504901405976559617\nexp2 159\nexp1 107\nsign1 1\nsign0 1\n";


    if (pairing_init_set_buf(pairing,s,strlen(s))) pbc_die("Pairing initialization failed.");
    if (!pairing_is_symmetric(pairing)) pbc_die("pairing must be symmetric");

    element_init_Zr(x, pairing);
    element_init_G1(g1, pairing);
    element_init_G1(pk, pairing);
    element_init_G1(h, pairing);
    element_init_G1(sigma, pairing);
    
    /* READING INPUT */
    
    FILE *fp = fopen("./input", "r");
    char buffer[5000];
    char message[5000];
    
    // G1
    fgets(buffer, 5000, fp);
    //printf("g1 (buffer): %s\n", buffer);
    element_set_str(g1, buffer, 10);
    //element_printf("g1: %B\n", g1);
    
    // Message
    fgets(message, 5000, fp);
    //printf("msg: %ld, %s\n", strlen(message), message);
    
    // Secret Key
    fgets(buffer, 5000, fp);
    //printf("sk (buffer): %s\n", buffer);
    
    element_set_str(x, buffer, 10);
    //element_printf("sk: %B\n", x);
    
    element_pow_zn(pk, g1, x);
    //element_printf("pk: %B\n", pk);
    
    element_from_hash(h, message, strlen(message));
    //element_printf("h: %B\n", h);
    
    element_pow_zn(sigma, h, x);
    //element_printf("output: %B\n", sigma);
    
    
    fp = fopen("./output", "w");
    
    element_fprintf(fp, "%B\n", sigma); 
    
    
    
    

	return 0;
}
