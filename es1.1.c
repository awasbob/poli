/*
es 1.1 DA CONSEGNARE
*/

/*
+ attualmente funzionante con stringhe nel main preimpostate
- quando si incrementa regexp controllare che non sia alla fine della stringa -> es. abcd\a darebbe errore
- aggiornare tutti i casi di FAIL come il primo
- sistemare main (prendere src da input)
- aggiungere stampa finale (tutta la parola non solo la lettera)
- FACOLTATIVO aggiungere controlli 
*/

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define CMAX 100

char *cercaRegexp(char *src, char *regexp);

int main() {
    char *src, *regexp; 
    char * p; 

    src="ciao cici"; 
    regexp="c.ci"; 
    /*
    printf("Inserisci stringa src: \n"); 
    scanf("%[^\n]", src); // deve leggere più parole
    printf("Inserisci stringa regexp: \n"); 
    scanf("%s\n", regexp); 
    */
    if ((p = cercaRegexp(src, regexp)) == NULL) printf("Non trovato"); 
    else printf("Primo carattere: %c", *p);  

    return 0; 
}

char *cercaRegexp(char *src, char *regexp) {
    int lens, lenr, lenr_in, cont=0, flag=0; 
    char * regexp_in = regexp, * src_ret = src; 
    lens = strlen(src);
    lenr = strlen(regexp); 
    lenr_in=lenr;

    while(lenr>1 && lens>1) { 
        lenr = strlen(regexp);
        lens = strlen(src);

        if(*regexp==*src){
            regexp++; 
            src++; 
            cont++; 
        }
        else{
            if(isalpha(*regexp)!=0) { // FAIL
                regexp = regexp_in;
                for (; *src != '\0'; src++) {
                    if (*src == ' ') {
                        src++;  // Salta lo spazio
                        break;
                    }
                }
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
                            if(*regexp!=*src) { 
                                cont++; 
                                regexp++; // punta a ']'
                                regexp++;
                                src++; 
                            }
                            else{ // FAIL
                                for(int i=0;i<lens && *src!=' ';i++, src++); 
                                src++;
                                src_ret=src;
                                regexp = regexp_in; 
                                cont=0;
                            }
                        }
                        else {
                            while(*regexp!=']'){ 
                                if(*regexp!=*src) regexp++; 
                                else{
                                    src++; 
                                    cont++; 
                                    while(*regexp!=']'){
                                        regexp++;
                                        flag=1; 
                                    }  
                                }
                            }
                            regexp++;
                            if(flag==0) { // FAIL
                                for(int i=0;i<lens && *src!=' ';i++, src++); 
                                src++;
                                src_ret=src;
                                regexp = regexp_in; 
                                cont=0;
                            }
                            flag=0;
                        } 
                        break;

                    case '\\': 
                        regexp++; 
                        if(*regexp=='a') { 
                            if(iswlower(*src)!=0) {
                                src++;
                                regexp++;
                                cont++; 
                            }
                            else { // FAIL
                                for(int i=0;i<lens && *src!=' ';i++, src++); 
                                src++;
                                src_ret=src;
                                regexp = regexp_in; 
                                cont=0;
                            }
                        }
                        else{ //sa da per scontato o 'a' o 'A'
                            if(islower(*src)==0) {
                                src++;
                                regexp++;
                                cont++; 
                            }
                            else{ // FAIL
                                for(int i=0;i<lens && *src!=' ';i++, src++); 
                                src++;
                                src_ret=src;
                                regexp = regexp_in; 
                                cont=0;
                            }
                        }
                        break;                     
                }
            }
        } 
    }

    if(lenr==1 && cont==lenr_in)  return (char*)&src_ret[0]; //function return address of local variable
    return NULL; 
}
