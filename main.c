/*
 * make the file extensions universal.
 * learn how do the includes work.
 * remove the ~.slo and large.fi files. 
 * set max len of bytes in strings, check if the str len is < bytes buffers.
 * error handling
 * client and serv
 * make .slo file structure.
 */



#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "file_bytes.c"
#define MAX_BYTES 50
#define SHA256_buf 32


int chk_dir(char *large_file_name, char *dir_name, char *export_path){
	char cmd_buf[100];
	char tmp_buf[40];
	char tmp_dir_path[] = "/tmp/";
	char slash[1] = "/";
	char dd_1[] = "dd if=/dev/zero of=";
	char dd_2[] = " bs=1 count=0 seek=1T";
	char mk_d[] = "mkdir /tmp/";
	int str_len_dir_name = strlen(dir_name);


	/* tmp path where large.file will be saved also the command for creating the
	  tmp directory & export buf. Check if the file is equals to . or .. .	  
	  */


	memset(cmd_buf, 0x00, 100);
	memset(tmp_buf, 0x00, 40);
	
	strncat(cmd_buf, mk_d, strlen(mk_d));
	strncat(tmp_buf, tmp_dir_path, strlen(tmp_dir_path));
	for(int offset_i = 2; offset_i < str_len_dir_name; offset_i++){
		strncat(cmd_buf, &dir_name[offset_i], 1);
		strncat(tmp_buf, &dir_name[offset_i], 1);
	}
	strncat(tmp_buf, slash, 1);
	system(cmd_buf);
	
	strncat(export_path, tmp_buf, strlen(tmp_buf));
	strncat(export_path, large_file_name, strlen(large_file_name));



	/* create large.file command. */
	memset(cmd_buf, 0x00, 100);

	strncat(cmd_buf, dd_1, strlen(dd_1));
	strncat(cmd_buf, tmp_buf, strlen(tmp_buf));
	strncat(cmd_buf, large_file_name, strlen(large_file_name));
	strncat(cmd_buf, dd_2, strlen(dd_2));

	system(cmd_buf);
	
	
	return 0;
}

int time_export(char *export_path){
	time_t timeinfo;
	timeinfo = time(NULL);
	struct tm *tm_l = localtime(&timeinfo);
	
	int day = tm_l -> tm_mday;	
	int month = tm_l -> tm_mon + 1;
	int year = tm_l -> tm_year - 100;	
	int hour = tm_l -> tm_hour;
	int min = tm_l -> tm_min;
	int sec = tm_l -> tm_sec;
	/////////////////////////////
	
	//signed short year_B = year;
	//printf("%d; %d; %d; %d; %d; %d \n", day, month, year, hour, min, sec);
	
	create_binary_file(day, month, year, hour, min, sec, timeinfo, export_path);
	
	//printf("%s tmp buf \n", export_path);
	return 0;
}


int main(){
	
	char export_path[MAX_BYTES];
	
	memset(export_path, 0x00, MAX_BYTES);
	
	//hash_fn();
	chk_dir(large_file_name, dir_name, export_path);	
	time_export(export_path);
	return 0;
}
