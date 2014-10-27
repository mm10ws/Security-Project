/*
assign2
author: MayurM
date: sept 18

issues: network mode and HMAC not working 

*/

#include<stdio.h>
#include<stdlib.h>
#include<gcrypt.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char **argv)
{
	char * filename = malloc(strlen(argv[1])); //holds the name of the file to be used
	char arg2[] = ""; //holds second arg
	char password[20]; //holds user entered password
	strcpy(filename, argv[1]);	
	strcpy(arg2, argv[2]);	

	if (strcmp(arg2, "-d") == 0) //network mode
		printf("-d\n"); //unable to implement
	else if(strcmp(arg2, "-l") == 0){	//local mode	

		printf("Enter password:\n");
		scanf("%s", password); //user enters password
		//printf("%d\n", strlen(password));

		gpg_error_t err;
		unsigned char outbuf[16] = {0}; //holds generated key
		unsigned long iter = 4096; //num of iterations
		
		//generate key
		err = gcry_kdf_derive(password, strlen(password), GCRY_KDF_PBKDF2, GCRY_MD_SHA512, "NaCl", 4, iter, 16, outbuf);
		if(err){
			printf("%s\n", "there was a problem generating key");
		}

		//print out key
		printf("Key: ");
		int i = 0;
		for (; i < 16; i++){	
			printf("%02X ", outbuf[i]);
		}
		printf("\n");	
		
		//open files
		FILE *in = fopen(argv[1], "rb");
		FILE *pout;  	
				
		//long inlen = ftell(in);		
		//char *inbuff = malloc(inlen +1);
		//fread(inbuff, inlen, 1, in);
		//fclose(in);
		
		
		//printf("%s\n", inbuff);
		gcry_cipher_hd_t enh;
		gcry_error_t     enerror;
    		char *encrybuff = malloc(16); //output after encryption
		int b = GCRY_CIPHER_AES128;		

    		size_t keysize = gcry_cipher_get_algo_keylen(GCRY_CIPHER_AES128);
    		size_t blocksize = gcry_cipher_get_algo_blklen(GCRY_CIPHER_AES128);		
		
		int iv = 5844;
		
		//open cipher
		enerror = gcry_cipher_open(&enh, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
		if(enerror){
			printf("error opening cypher\n");
		}
		
		//setting key
		enerror = gcry_cipher_setkey(enh, outbuf, keysize);
		if(enerror){
			printf("error setting key\n");

		}

		
    		//setting iv
		enerror = gcry_cipher_setiv(enh, &iv, blocksize);
		if(enerror){
			printf("error setting iv\n");

		}

		
		//appending .uf to output file name
		char *outfilename = malloc(5 + strlen(argv[1]) );
		strcpy(outfilename, argv[1]);
		strcat(outfilename, ".uf");
		
		if(access(outfilename, F_OK) != -1){ //check if file already exists
			return 33; //file already exists
		}
		else{ //open the file
				
  			pout = fopen (outfilename, "wb");
  			
		}

		//encrypting
		while(!feof(in)){
			b = fread(encrybuff, 1, 16, in);
			if(!b){
				break; // empty
			}
			while(b < 16){
				encrybuff[b++] = 0x0; //padding
			}
			enerror = gcry_cipher_encrypt(enh, encrybuff, 16, NULL, 0); 
			if(enerror){
				printf("error encry\n");
				//printf("%d\n", strlen(inbuff));
				//printf("%d\n", blocksize);
				//problem with encryption dont know how to fix
			}
			b = fwrite(encrybuff, 1, 16, pout); //write to file			
		}
		
		//finalize
		gcry_cipher_close(enh); 
		fclose(in);
		fclose(pout);
		free(filename);
		filename = NULL;
		free(encrybuff);
		encrybuff = NULL;
		free(outfilename);
		outfilename = NULL;
		
		
		
	
	}
	else
		printf("incorrect\n");	
		
        
        exit(EXIT_SUCCESS);
	
}

