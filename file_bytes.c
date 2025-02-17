#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/random.h>
#include "hash.c"
#include "file_bytes.h"
#define MAX_PATH_LEN 255
#define SHA256_buf 32


struct dirent *ent;


int del_files(int paths_len){
	//rm *.slo buf
	char d_slo[MAX_PATH_LEN + 10];
	//rm *.fi buf
	char d_fi[MAX_PATH_LEN + 10];
	char c_rm[] = "rm ";
	char c_slo_slo[] = "*~.slo*";
	char c_fi_fi[] = "*~.fi*";
	
	memset(d_slo, 0x00, (MAX_PATH_LEN + 10));	
	memset(d_fi, 0x00, (MAX_PATH_LEN + 10));

	printf("%s d_fi\n", d_fi);
	for(int counter_p = 0; counter_p < paths_len; counter_p++){
		//execute commands for each path 
		strncat(d_slo, c_rm, strlen(c_rm));
		strncat(d_slo, path[counter_p], strlen(path[counter_p]));
		strncat(d_slo, c_slo_slo, strlen(c_slo_slo));
		//printf("%s \n", d_slo);
		system(d_slo);
		
		
		strncat(d_fi, c_rm, strlen(c_rm));
		strncat(d_fi, path[counter_p], strlen(path[counter_p]));
		strncat(d_fi, c_fi_fi, strlen(c_fi_fi));
		//printf("%s \n", d_fi);
		system(d_fi);
		
		memset(d_slo, 0x00, (MAX_PATH_LEN + 10));	
		memset(d_fi, 0x00, (MAX_PATH_LEN + 10));
		
	
	}

	
	return 0;
}


int mk_file(unsigned char bytes_form[buf_len], char *export_path){
	DIR *dir;
	bool flag_a = 0;
	unsigned char *hash_file = bytes_form;
	int paths_len = *(&path + 1) - path;
	char file_name[MAX_PATH_LEN];
	char cp_large_fi_cmd[60];
	char cp_cmd[] = "cp ";
	char space_char[] = " ";
	char dot[] = ".";
	char dotdot[] = "..";
	unsigned char *hash_from_file[SHA256_buf];	
	memset( cp_large_fi_cmd, 0x00, 60);
	printf("_----_\n");	
	
	calc_hash((unsigned char *)hash_file, buf_len, (unsigned char *)hash_from_file);
	for(int i_de = 0; i_de < SHA256_buf; i_de++){
		

		printf("%c", hash_from_file[i_de]);
		printf("\n %d i_de \n", i_de);
	} 
	
	del_files(paths_len);
	
	//cp command
	strncat(cp_large_fi_cmd, cp_cmd, strlen(cp_cmd));
	strncat(cp_large_fi_cmd, export_path, strlen(export_path));

	for(int c_p = 0; c_p < paths_len; c_p++){
		memset(file_name, 0, MAX_PATH_LEN);
		snprintf(file_name, sizeof(file_name), "%s", path[c_p]);
		dir = opendir(path[c_p]);

		while((ent = readdir(dir)) !=0){
			int counter = strlen(ent -> d_name) + strlen(path[c_p]) + strlen(out_file_ext);
			
			if((strlen(ent -> d_name) == 1 && strncmp(ent -> d_name, dot, sizeof(dot)) == 0) || strncmp(ent -> d_name, dotdot, sizeof(dotdot)) == 0 ){
				continue;	
			}

			printf("%s \n", ent -> d_name);
			//file path
			char n_path[counter];
			char cp_n_path[300];
			memset(cp_n_path, 0x00, 300);
			memset(n_path, 0x00, counter);
			//getting the file name
			strncat(n_path, path[c_p], strlen(path[c_p]));
			strncat(n_path, ent -> d_name, strlen(ent -> d_name));
			//getting the file name without the .slo ext, for cp cmd
			strncat(cp_n_path, cp_large_fi_cmd, strlen(cp_large_fi_cmd));
			strncat(cp_n_path, space_char, strlen(space_char));
			strncat(cp_n_path, n_path, strlen(n_path));
			strncat(cp_n_path, out_large_file_ext, strlen(out_large_file_ext));
			
			//outfile .slo
			strncat(n_path, out_file_ext, strlen(out_file_ext));

			FILE *f_p = fopen(n_path, "wb");
			//printf("%s \n", n_path);
			fwrite(bytes_form, buf_len, 1, f_p);
			fclose(f_p);
			
			FILE *f_p1 = fopen(n_path, "rb");
			hash_path(f_p1, hash_from_file);
			fclose(f_p1);

			
			
			//create the large_file
			system(cp_n_path);


		}

		closedir(dir);
	}
	return 0;
}



void shuffle(uint16_t * y_c, uint16_t * m_c, uint16_t * d_c, uint16_t * h_c, uint16_t*  min_c, uint16_t * s_c, time_t timeinfo){
	srand(timeinfo);
	*y_c = rand() % (400 + 1 - 20) + 20;
	*m_c = rand() % (500 + 1 - 400) + 400;
	*d_c = rand() % (900 + 1 - 500) + 500;
	*h_c = rand() % (1200 + 1 - 900) + 900;
	*min_c = rand() % (1800 + 1 - 1200) + 1200 ;
	*s_c = rand() % (1900 + 1 - 1800) + 1800;

}


int create_binary_file(int day, int month, int year, int hour, int min, int sec, time_t timeinfo, char *export_path){
	uint16_t y_c = 0;
	uint16_t m_c = 0;
	uint16_t d_c = 0;
	uint16_t h_c = 0;
	uint16_t min_c = 0;
	uint16_t s_c = 0;
	
	
	unsigned char bytes_form[buf_len];
	memset(bytes_form, 0, buf_len);
	
	//headers Bytes
	bytes_form[0] = 0x53;
	bytes_form[1] = 0x4C;
	bytes_form[2] = 0x4F;
	bytes_form[3] = 0x2E;
	
	bytes_form[4] = 0x08;
	bytes_form[5] = 0x00;
	
	// Getting paths for now just fillnig those 0 bytes with random bytes.
	for(int i = 6; i < buf_len; i++){
		getrandom(&bytes_form[i], 1, GRND_NONBLOCK);
		//printf("%.2x : %d \n", bytes_form[i], i);
	}

	//random offset locations for date| will be stored in offset < 6 AND offset > 20
	shuffle(&y_c, &m_c, &d_c, &h_c, &min_c, &s_c, timeinfo);	
        
	//the year offset 	
	bytes_form[6] = (y_c >> 8) & 0x00FF;
	bytes_form[7] = y_c & 0x00FF;
	//the month offset 	
	bytes_form[8] = (m_c >> 8) & 0x00FF;
	bytes_form[9] = m_c & 0x00FF;
	//the day offset 	
	bytes_form[10] = (d_c >> 8) & 0x00FF;
	bytes_form[11] = d_c & 0x00FF;
	//the hour offset 	
	bytes_form[12] = (h_c >> 8) & 0x00FF;
	bytes_form[13] = h_c & 0x00FF;
	//the min offset 	
	bytes_form[14] = (min_c >> 8) & 0x00FF;
	bytes_form[15] = min_c & 0x00FF;
	//the seconds offset 	
	bytes_form[16] = (s_c >> 8) & 0x00FF;
	bytes_form[17] = s_c & 0x00FF;
	//the date offsets
	bytes_form[y_c] = year;
	bytes_form[m_c] = month;
	bytes_form[d_c] = day;
	bytes_form[h_c] = hour;
	bytes_form[min_c] = min;
	bytes_form[s_c] = sec;

	mk_file(bytes_form, export_path);
	return 0;
}
