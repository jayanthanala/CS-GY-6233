#include "types.h"
#include "stat.h"
#include "user.h"

int stringcmp(const char* s1, const char* s2)
{
  for (; *s1 && *s2 && *s1==*s2; s1++, s2++);
  return *(unsigned char*)s1-*(unsigned char*)s2;
}
int stringcasecmp(const char* s1, const char* s2)
{
  for (; *s1 && *s2 && ( (*s1==*s2) || (*s1-*s2==32) || (*s2-*s1==32) ); s1++, s2++);
  return *(unsigned char*)s1-*(unsigned char*)s2;
}

int processCmdArgs(int argc, char **argv,int *flagc,int *flagd,int *flagi)
{
    char c;
    int temp = 0;
    while((++argv)[0] && argv[0][0] == '-')
    {
        while((c = *++argv[0]))
        {
            switch(c) {
            case 'c':
                *flagc = 1;
                temp++;
                break;
            case 'd':
                *flagd=1;
                temp++;
                break;
            case 'i':
                *flagi=1;
                temp++;
                break;
            default:
            printf(1,"\n Invalid Option provided. Only -i,-c,-d are allowed");
            exit() ;
            }
        }
    }
    if(*flagc==1 && *flagd==1)
    {
        printf(1,"\n -c and -d together are not allowed");
        exit();
    }
    return temp;
}
void calOutput(int fd,int flagc,int flagi, int flagd)
{
    char s[1024];
    int n=0;
    char curr[1024];
    char prev[1024];
    int i=0;
    int word_len = 0;
    int dup_line_count = 1;
    int total_line_count = 0;
    while((n = read(fd, s, sizeof(s))) > 0)
     {
        for(i=0;i<=n;i++) // Run the loop one extra time to process the last line
        {
            if(i<n && s[i]!='\n') // The first condition will fail when file length is crossed so goes to else
            {
                curr[word_len] = s[i]; // Store the line untill new line is reached
                word_len++; // Keep the word length count
            }
            else
            {
                word_len = 0; //Make the line length 0
                if( (flagi==0) ? stringcmp(prev,curr) == 0 : stringcasecmp(prev,curr)==0)
                {
                    if(flagd==1 && dup_line_count == 1)
                        printf(1,"%s\n",prev);                  
                    dup_line_count++;
                }
                else
                {
                    /*Check the flags and print the output accordingly*/
                    if(total_line_count !=0) // An empty line is generated at start, need to be discarded
                    (flagc==1) ? printf(1,"%d %s\n",dup_line_count,prev) :
                    ( (flagd==0) ? printf(1,"%s\n",prev) : printf(1,"%s",""));
                    dup_line_count = 1;
                    strcpy(prev,curr);
                }
                memset(curr,'\0',1024);
                total_line_count++;
            }
        }
        
        (flagc==1) ? printf(1,"%d %s\n",dup_line_count,prev) : 
        (flagd == 0) ? printf(1,"%s\n",prev) : printf(1,"%s","");
        
        
     }
     close(fd);
     return;

}


int main(int argc, char **argv)
{
    int flagc =0;
    int flagi = 0;
    int flagd = 0;
    int fd = 0;
    int count_params = processCmdArgs(argc,argv,&flagc,&flagd,&flagi);
    if(argc - count_params -1 == 1)
    {
        fd = open(argv[count_params+1],0);
        if(fd  < 0){
            printf(1, "uniq: cannot open %s\n", "testfile");
            exit();
        }
    }
    calOutput(fd,flagc,flagi,flagd);
    exit();
    return 0;
}

