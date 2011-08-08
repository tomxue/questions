#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

main(int argc, char *argv[])
{
	#define NumofDays 31
	#define NumofMons 12
	char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};	
	char myDate[9];	//20110431*
	char myCmd1[57] = "find /mnt/hgfs/Dropbox/ScrapBook/data/ -maxdepth 1 -name ";
	char myCmd2[8] = " | wc -l";
	char myCmd[75];
	char myCmd3[] = "mutt -s \"statistics\" tom.xue@nokia.com < asterisk.txt"; 
	int i_mon, j_day, k, ret, bigNum;
	time_t timep;
	struct tm *p;
	FILE *streamCmd;
	int bufCmd[1];
	int numOneDay = 0;
	float numTotal = 0;
	float numOfDay = 0;
	int fd;
	char lineEnd[]="\n";
	char plus[]="+";
	int times=0;

	myCmd[74] = 0;
	time(&timep);	/* Return the current time and put it in *TIMER if TIMER is not NULL.  */
	p=gmtime(&timep);	/* Return the `struct tm' representation of *TIMER in Universal Coordinated Time */
	printf("Press Crl+Z to break the execution.\n");
	fd=open("asterisk.txt",O_RDWR |  O_TRUNC | O_APPEND,0777);
	if(fd<0)
		printf("Open file asterisk.txt failed!\n");
	
	for(i_mon=0;i_mon<NumofMons;i_mon++)
	{
		for(j_day=0;j_day<NumofDays;j_day++)
			{
				if( (p->tm_mon > i_mon) && (p->tm_mday > j_day) && (i_mon == 0) )
					sprintf(myDate, "%d%.2d%.2d", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday - j_day);		
				else if( (p->tm_mon > i_mon) && (i_mon > 0) )
					sprintf(myDate, "%d%.2d%.2d", (1900+p->tm_year), (1+p->tm_mon-i_mon), 31 - j_day);	
				else
					break;				
				
				ret = strcmp(myDate, "20110431");	//start day
				if( ret == 0 )
					{
						printf("\nThe total learnt number is: %d\n", (int)numTotal);
						printf("The time span is: %d days\n", (int)numOfDay);
						printf("The average learnt number is: %f\n\n", numTotal/numOfDay);
						return;	
					}

				myDate[8] = 42;//"*";
				//printf("Date: %.8s  - The number of items I learnt is below\n",myDate);	
				//printf("Date: %s\n",myDate);	
				sprintf(myCmd, "%.57s%.9s%.8s", myCmd1, myDate, myCmd2);		
				printf("\n");	//it is used to make the myCmd run in 1 line
				if(argc > 1)	//"1" ascii code number
					{
						system(myCmd);
						printf("Date: %s\n",myDate);
					}
				
				/* to read the shell result back to this program */
				streamCmd = popen( myCmd, "r" ); 	
				//printf( "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk" );	
				memset(bufCmd, 0, sizeof(bufCmd));		
				fread( bufCmd, sizeof(int), sizeof(bufCmd), streamCmd);
				//printf( "cmd result: %d\n", bufCmd[0] );
				numOneDay = atoi(bufCmd);
				numTotal += numOneDay;	
				numOfDay += 1;		
				//printf( "cmd result: %d\n", numOneDay );
				
				for(k=0;k<numOneDay;k++)
				{
					printf( "+" );
					if(write(fd,plus,strlen(plus)) < 0)
						printf("write error!\n");				
				}
				write(fd,lineEnd,strlen(lineEnd));	
				pclose(streamCmd);
				
				times = times + 1;
				if(times > 20)
					goto end;
		}
				
	}	

	end:	printf("The end.\n");
		close(fd);
		system(myCmd3);
		return;
}
