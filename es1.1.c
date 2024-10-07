#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *cercaRegexp(char *src, char *regexp);

int main() {
    // MAIN
    
    return 0; 
}

// nonrec

char *cercaRegexp(char *src, char *regexp) {
    int lens, lenr, lenr_in, cont=0; 
    char * regexp_in = regexp, * src_ret = src; 
    lens = strlen(src);
    lenr = strlen(regexp); 
    lenr_in=lenr;

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
                regexp = regexp_in;
                src++;          // MODIFICABILE: portare src al prossimo carattere dopo spazio vuoto invece che al carattere successivo
                src_ret=src;
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
                            while(*regexp!=']'){  // serve aggiungere un flag di controllo all'uscita del while
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
                        regexp++; 
                        if(*regexp=='a') { 
                            if(iswlower(*src)!=0) {
                                src++;
                                regexp++;
                                cont++; 
                            }
                            else {
                                regexp = regexp_in; 
                                src++; // MODIFICABILE come sopra src fino a dopo ' '
                                cont=0;
                            }
                        }
                        else{ //sa da per scontato o 'a' o 'A'
                            if(islower(*src)==0) {
                                src++;
                                regexp++;
                                cont++; 
                            }
                            else{
                                regexp = regexp_in; 
                                src++; // MODIFICABILE come sopra src fino a dopo ' '
                                cont=0;
                            }
                            }
                        break; 
                    
                }
            }
        } 
    }

    if(lenr==0 && cont==lenr_in)  return (char*)&src_ret; 
    return NULL; 
}
