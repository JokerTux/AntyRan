#include <openssl/sha.h>
#include <malloc.h>
#define SHA256_buf 32
#define MAX_BUF 2048


unsigned char hash_exp[SHA256_buf];
bool flag_b = false;

size_t file_sz(FILE *f_p){
	size_t size = fseek(f_p, 0L, SEEK_END);
	
	size = ftell(f_p);
	rewind(f_p);

	return size;
}


void calc_hash(unsigned char *buf, size_t buf_len, unsigned char *hash){
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buf, buf_len);
    SHA256_Final(hash, &sha256);
    
    //print buff.
    for(int i = 0; i < SHA256_buf; i++){
    	printf("%02x", hash[i]);
    }
    printf("\n");
}


int hash_path(FILE *f_p, unsigned char **hash_from_file){
	unsigned char hash[SHA256_buf];
	memset(hash, 0x00, SHA256_buf);
	
	size_t f_sz = file_sz(f_p);
	if(f_sz != MAX_BUF){
		printf("Error MAX_BUF != f_sz\n");
	}
	uint8_t *f_buf_p = malloc(f_sz);
	printf("%ld f_sz\n", f_sz);
	
	if(fread(f_buf_p, 1, f_sz, f_p) != f_sz){
		printf("Error file size \n");
	}
	calc_hash(f_buf_p, f_sz, hash);
	free(f_buf_p);
	printf("%ld sizeof hash_exp\n", sizeof(hash_exp));	
	if(flag_b == false){	
		for(int a_j = 0; a_j < SHA256_buf; a_j++){
			hash[a_j] = hash_exp[a_j];
			printf("%02x", hash[a_j]);
		}
	}
	printf("\n\n");
	if(&hash_from_file == hash){
		printf("Successs !!\n");
		
	}	

	return 0;
}
