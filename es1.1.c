#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *cercaRegexp(char *src, char *regexp);

int main() {


    return 0; 
}

// nonrec

char *cercaRegexp(char *src, char *regexp) {
    int i, j, lens, lenr, lenr_in, cont=0, flag=0; 
    lens = strlen(src);
    lenr = strlen(regexp); 
    char * temp = src; 
    lenr_in=lenr;

    if(lens<lenr) return NULL; 

    while(lenr>0 && lens>0) { 
        lenr = strlen(regexp);
        lens = strlen(src);

        if(*regexp==*src){
            regexp++; 
            src++; 
            cont++; 
        }
        else{
            if(isalpha(*regexp)!=0) {
                regexp = temp;
                cont=0; 
            }
            else{
                switch (*regexp) {
                    case '.':
                        regexp++; 
                        src++;
                        cont++; 
                        break;
                    
                    case '[':
                        regexp++; 
                        if(*regexp=='^'){
                            regexp++; 
                            while(*regexp!=']'){  //MODIFICABILE: in regexp dentro a [^...] c'è sempre solo una lettera
                                if(*regexp==*src) regexp++; 
                                else{
                                    src++; 
                                    cont++; 
                                    while(*regexp!=']') regexp++; 
                                    regexp++;
                                }
                            }
                        }
                        else {
                            while(*regexp!=']'){  
                                if(*regexp!=*src) regexp++; 
                                else{
                                    src++; 
                                    cont++; 
                                    while(*regexp!=']') regexp++; 
                                    regexp++;
                                }
                            }
                        } 
                        break;

                    case '\\': //stesso modo di prima, porta avanti i puntatori, controlla con a islower 
                        break; 
                    
                }
            }
        } 
    }

    if(lenr==0 && cont==lenr_in)  return (char*)&temp; 
    return NULL; 
}