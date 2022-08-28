#include <stdio.h>

int main(int argc, char *argv[]){



    FILE *SourceFile,*TargetFile;
    char buffer[1024];
    printf("argc:%d\n",argc);
    printf("argv0:%s\n",argv[0]);
    printf("argv1:%s\n",argv[1]);
    printf("argv2:%s\n",argv[2]);
    SourceFile = fopen( argv[1],"rb" );
    TargetFile = fopen( argv[2],"w" );
    //char buffer[]={ 'H','e','y' };

    //pFile = fopen( "write.txt","w" );

    if( NULL == SourceFile || NULL == TargetFile){

        printf( "open failure\n" );

        return 1;

    }else{
        
        
        while (!(EOF==fscanf(SourceFile,"%s",buffer)))
        {
            printf("%s\n",buffer);
            fprintf(TargetFile,"%s%s",buffer,"\n");
        }
        //fwrite(buffer,1,sizeof(buffer),pFile);
    }



    //fclose(pFile);
    fclose(SourceFile);
    fclose(TargetFile);
    return 0;

}