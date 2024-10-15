/*
1.4 DA CONSEGNARE
come 1.3 con aggiunta di memorizzazione di ordinamenti
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CMAX 31
#define NMAX 1000

typedef struct {
    int anno, mese, giorno; 
} data; 

typedef struct {
    int ora, min, sec; 
} ora; 

typedef struct {
    char codice[CMAX], partenza[CMAX], destinazione[CMAX]; 
    data data_corsa; 
    ora ora_partenza, ora_arrivo; 
    int ritardo; 
} tratta; 

typedef enum {r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine, r_stampa,
                r_ord_data, r_ord_codice, r_ord_partenza, r_ord_capolinea, r_ricerca_partenza} comando_e; 

char comandi[][CMAX] = {"date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine", "stampa", 
                            "ord_data", "ord_codice", "ord_partenza", "ord_capolinea", "ricerca_partenza"};

tratta * p_ord_data[NMAX];
tratta * p_ord_codice[NMAX];
tratta * p_ord_partenza[NMAX];
tratta * p_ord_capolinea[NMAX];


comando_e leggiComando(); 
void selezionaDati(tratta * tratte_vet, comando_e cmd, int N); 
int datacmp(data d1, data d2); 
int oracmp(ora h1, ora h2); 
void stampaVet(tratta * tratte_vet[], int N); 

int main() {
    int N;
    FILE *in = fopen("corse.txt", "r"); 
    comando_e cmd; 

    fscanf(in, " %d", &N); 
    tratta tratte_vet[N]; 
    for(int i=0;i<N;i++){  // acquisizione vettore tratte  
        fscanf(in, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", tratte_vet[i].codice, tratte_vet[i].partenza, tratte_vet[i].destinazione,
                &tratte_vet[i].data_corsa.anno, &tratte_vet[i].data_corsa.mese, &tratte_vet[i].data_corsa.giorno,
                &tratte_vet[i].ora_partenza.ora, &tratte_vet[i].ora_partenza.min, &tratte_vet[i].ora_partenza.sec,
                &tratte_vet[i].ora_arrivo.ora, &tratte_vet[i].ora_arrivo.min, &tratte_vet[i].ora_arrivo.sec, &tratte_vet[i].ritardo);
    }

    do {
        cmd = leggiComando(comandi);
        selezionaDati(tratte_vet, cmd, N); 
    } while(cmd!=5); 

    printf("Terminazione programma."); 
    return 0;
}

comando_e leggiComando() {
    char cmd[CMAX];
    printf("Inserire comando: "); 
    scanf("%s", cmd); 

    for(int i=0;i<12;i++){
        if(strcmp(cmd, comandi[i])==0)
            return (comando_e) i; 
    }
    return 5; 
}

void selezionaDati(tratta * tratte_vet, comando_e cmd, int N) {
    data d1, d2; 
    comando_e cmd2; 
    FILE *out; 
    tratta temp; 
    tratta * temp_p;
    char str[CMAX], r_out; 
    int i, rit_tot=0, j=0, flag=0, str_len, k=0, min=0; 
    int v_flags[4] = {0};

    switch (cmd)
    {
    case r_stampa: 
        printf("Scegliere se stampare su file(f) o a schermo(s): "); 
        scanf(" %c", &r_out); // non controlla errori di inserimento
/*
        se si non si mette lo spazio prima di %c nella scanf il compilatore legge uno spazio vuoto lasciato da una scanf
        precedente nel programma e "salta" la riga. Con lo spazio si pulisce il buffer e legge correttamente.
*/
        out = (r_out=='f') ? fopen("out1.3.txt","w") : stdout; 
      
        cmd2 = leggiComando(); // riparte da fuori case r_stampa, leggi cmd2 senza funzione 

        switch (cmd2)
        { 
        case r_date:
            printf("Inserisci data 1 (formato aaa/mm/gg): \n"); 
            scanf("%d/%d/%d", &d1.anno, &d1.mese, &d1.giorno); 
            printf("Inserisci data 2 (formato aaa/mm/gg): \n"); 
            scanf("%d/%d/%d", &d2.anno, &d2.mese, &d2.giorno); 

            fprintf(out, "\nCorse che partono nell'intervallo di date richiesto: \n"); 
            for(i=0;i<N;i++) {
                if(datacmp(d1,tratte_vet[i].data_corsa)<=0 && datacmp(tratte_vet[i].data_corsa,d2)<=0)
                fprintf(out, "Corsa codice: %s\n", tratte_vet[i].codice); 
            }
            break;

        case r_partenza: 
            printf("Inserisci fermata di partenza: \n"); 
            scanf("%s", str); 

            fprintf(out, "\nCorse che partono dalla fermata richiesta: \n"); 
            for(i=0;i<N;i++){
                if(strcmp(str,tratte_vet[i].partenza)==0)
                fprintf(out, "Corsa codice: %s\n", tratte_vet[i].codice); 
            }
            break; 
        
        case r_capolinea: 
            printf("Inserisci fermata di capolinea: \n"); 
            scanf("%s", str); 

            fprintf(out, "\nCorse che terminano alla fermata richiesta: \n"); 
            for(i=0;i<N;i++){
                if(strcmp(str,tratte_vet[i].destinazione)==0)
                fprintf(out,"Corsa codice: %s\n", tratte_vet[i].codice); 
            }
            break; 

        case r_ritardo: 
            printf("Inserisci data 1 (formato aaa/mm/gg): \n"); 
            scanf("%d/%d/%d", &d1.anno, &d1.mese, &d1.giorno); 
            printf("Inserisci data 2 (formato aaa/mm/gg): \n"); 
            scanf("%d/%d/%d", &d2.anno, &d2.mese, &d2.giorno); 

            fprintf(out,"\nCorse che hanno fatto ritardo nell'intervallo di date richiesto: \n"); 
            for(i=0;i<N;i++) {
                if(datacmp(d1,tratte_vet[i].data_corsa)<=0 && datacmp(tratte_vet[i].data_corsa,d2)<=0 && tratte_vet[i].ritardo>0)
                fprintf(out,"Corsa codice: %s\n", tratte_vet[i].codice); 
            }
            break;

        case r_ritardo_tot: 
            printf("Inserisci codice tratta: \n"); 
            scanf("%s", str); 
            for(i=0;i<N;i++){
                if(strcmp(str, tratte_vet[i].codice)==0)
                rit_tot+=tratte_vet[i].ritardo; 
            }
            fprintf(out,"Ritardo totale su tratta di codice %s: %d\n", str, rit_tot); 
            break;

        default:
            break;
        }

        break; 

    case r_ord_data:
        if(v_flags[0]==0){
            for(i=0;i<N;i++)
                p_ord_data[i]=&tratte_vet[i];  
            for(i=0;i<N-1;i++) {
                flag=0;
                for(j=0;j<N-i-1;j++){
                    if(datacmp(p_ord_data[j]->data_corsa, p_ord_data[j+1]->data_corsa) > 0 || 
                        (datacmp(p_ord_data[j]->data_corsa, p_ord_data[j+1]->data_corsa) == 0 && 
                        oracmp(p_ord_data[j]->ora_partenza, p_ord_data[j+1]->ora_partenza) > 0)){
                        temp_p=p_ord_data[j]; 
                        p_ord_data[j]=p_ord_data[j+1]; 
                        p_ord_data[j+1]=temp_p;
                        flag=1; 
                    }
                }
                if(flag==0) break;              
                }
            v_flags[0]=1;
        }
        stampaVet(p_ord_data, N);
        break;

    case r_ord_codice: 
        if(v_flags[1]==0){
            for(i=0;i<N;i++)
                p_ord_codice[i]=&tratte_vet[i];  
            for(i=0;i<N-1;i++) {
                flag=0;
                for(j=0;j<N-i-1;j++){
                    if(atoi((p_ord_codice[j]->codice)+3)>atoi((p_ord_codice[j+1]->codice)+3)){
                        temp_p=p_ord_codice[j]; 
                        p_ord_codice[j]=p_ord_codice[j+1]; 
                        p_ord_codice[j+1]=temp_p;
                        flag=1; 
                    }
                }
                if(flag==0) break;              
                }
            v_flags[1]=1;
        }
        stampaVet(p_ord_codice, N);
        break;     

    case r_ord_partenza: 
        if(v_flags[2]==0){
            for(i=0;i<N;i++)
                p_ord_partenza[i]=&tratte_vet[i];  
            for(i=0;i<N-1;i++) {
                flag=0;
                for(j=0;j<N-i-1;j++){
                    if(strcmp(p_ord_partenza[j]->partenza,p_ord_partenza[j+1]->partenza)>0){
                        temp_p=p_ord_partenza[j]; 
                        p_ord_partenza[j]=p_ord_partenza[j+1]; 
                        p_ord_partenza[j+1]=temp_p;
                        flag=1; 
                    }
                }
                if(flag==0) break;              
                }
            v_flags[2]=1;
        }
        stampaVet(p_ord_partenza, N);
        break;

    case r_ord_capolinea: 
        if(v_flags[3]==0){
            for(i=0;i<N;i++)
                p_ord_capolinea[i]=&tratte_vet[i];  
            for(i=0;i<N-1;i++) {
                flag=0;
                for(j=0;j<N-i-1;j++){
                    if(strcmp(p_ord_capolinea[j]->destinazione,p_ord_capolinea[j+1]->destinazione)>0){
                        temp_p=p_ord_capolinea[j]; 
                        p_ord_capolinea[j]=p_ord_capolinea[j+1]; 
                        p_ord_capolinea[j+1]=temp_p;
                        flag=1; 
                    }
                }
                if(flag==0) break;              
                }
            v_flags[3]=1;
        }
        stampaVet(p_ord_capolinea, N);
        break;

    case r_ricerca_partenza: 
        printf("Inserisci nome (anche solo prefisso) della fermata di partenza: "); 
        scanf("%s",str); 
        str_len=strlen(str); 
        printf("Tratte che partono dalla fermata %s:\n", str); 
        for(i=0;i<N;i++){
            if(strncmp(tratte_vet[i].partenza,str,str_len)==0)
                printf("Corsa codice: %s. Da %s a %s\n", tratte_vet[i].codice, tratte_vet[i].partenza, tratte_vet[i].destinazione); 
        }
        break; 

    default: 
        break;
    }  
}

int datacmp(data d1, data d2) { 
    if(d1.anno!=d2.anno) return  d1.anno-d2.anno; 
    else if (d1.mese!=d2.mese) return d1.mese-d2.mese; 
    return d1.giorno-d2.giorno; 
}

int oracmp(ora h1, ora h2){
    if(h1.ora!=h2.ora) return h1.ora-h2.ora; 
    else if(h1.min!=h2.min) return h1.min-h2.min;
    return h1.sec-h2.sec; 
}

void stampaVet(tratta *tratte_vet[], int N){
    for(int i=0;i<N;i++)
    printf("V_ord[%d]: %s %s %s\n", i, tratte_vet[i]->codice, tratte_vet[i]->partenza, tratte_vet[i]->destinazione); 
}
