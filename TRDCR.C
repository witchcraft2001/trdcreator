/* This is TRDCR utility to create empty TRD files */
/* Written for SOLiD C by Hard (D.Mikhaltchenkov) in March 2021 */
/* mikhaltchenkov@gmail.com */
/* This software is PUBLIC DOMAIN */
/* Use it one your own RISK ! */

#include <io.h>
#include <stdio.h>
#include <dos.h>
#include <malloc.h>
                      
char info[] = { 0x00, 0x01, 0x16, 0x00, 
		0xF0, 0x09, 0x10, 0x00, 
		0x00, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x20, 0x20,
		0x20, 0x20, 0x00, 0x00,
		0x53, 0x50, 0x52, 0x49,
		0x4E, 0x54, 0x45, 0x52,
		0x00, 0x00, 0x00};

/* ====================================
        Check file by filename
   ====================================*/

BOOL is_file_exists(filename)
char *filename;
{
	FIND p;
	return ffirst(filename,&p,FA_NORMAL|FA_ARCH) != -1;
}

/* ====================================
              Create file
   ====================================*/

BOOL create(filename)
char *filename;
{
	FD fd;
	char *clear_sectors;
	char *info_sector;
	int i;        

	printf("Creating TRD-file: %s ...", filename);
	if ((fd=open(filename, O_CREAT|O_WRONLY)) == -1)
	{
		printf("ERROR: Can't create file: %s", filename);
		return FALSE;
	}
	clear_sectors = calloc(256,1);
	info_sector = calloc(256,1);
	
	//writing catalogue
	for(i=0;i<8;i++)
	{
		write(fd,clear_sectors, 256);
	}
                     
	for(i=0;i<sizeof(info);i++)
	{
		info_sector[i+225] = info[i];
	}

	//writing info sector
	write(fd,info_sector,256);
	
	//write tail of file             
	for(i=0;i<2551;i++)
	{
		write(fd,clear_sectors,256);
	}
	free(clear_sectors);
	free(info_sector);
	close(fd);
	printf("OK\n");
	return TRUE;
}

void prt_hello()
{
	printf("TRDCReator v.0.1 by D.Mikhaltchenkov\n\n");
}

main(argc, argv)
int argc;
char *argv[];
{
	if(argc==1)
	{
		prt_hello();
		printf("Usage:\t\tTRDCR <TRD file>\n");
		printf("Example:\tTRDCR FILE.TRD\n");
		exit(0);
	}

	if (is_file_exists(argv[1])) {
		printf("File %s is exists. Aborted.\n", argv[1]);
		exit(-2);
	}

	if (!create(argv[1])) exit(-1);
}

