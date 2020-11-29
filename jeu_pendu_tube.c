#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

// les constatntes
#define R 0
#define W 1
#define len 15



// permet de voir si le mot saisi existe dans la chaine
int rechercheLettre(char lettre, char motSecret[], int lettreTrouvee[]);


// verifie si le joueur a gagné
int gagne(int lettreTrouvee[]);


// choisir un chiffre aleatoirement
int ligneChiffre();

// retourne la taile d'un mot
int tailleMot(char mot[]);



// main
int main(int argc, char* argv[])
{	

	// declarations des variables
	char dico[6][11] = {"BONJOUR","MONSIEUR","KONE","VAKARAMOKO","ESATIC","ABIDJAN"};
	
	int PversF[2],FversP[2];
	char message[11] ;
	int message1[11];
	int tailleMotSecret;
	
	char lettre = 0; // Stocke la lettre proposée par l'utilisateur (retour du scanf)
        char motSecret[11] ; // C'est le mot à trouver
	
       int i = 0; 
	char tableauLettres[26]; // contient toutes les lettres entrées par le fils
	
	strcpy(motSecret,dico[ligneChiffre()]); // copie le mot aleatoirement dans dico dans motSecret
        tailleMotSecret = tailleMot(motSecret); // retourne la taile du mot secret
	
	
	
	
	if(pipe(PversF) ==-1 ){ // si erreur lors de la creation du tube PversF
	
		puts("erreur lors de la creation du tube PversF\n");
		exit(0);
	}
	if( pipe(FversP)== -1 ){// si erreur lors de la creation du tube FversP
	
		puts("erreur lors de la creation du tube FversP\n");
		exit(0);
	}

	switch(fork()){ // creation du processus fils
	
	case -1:
		{
			puts("erreur de creation du processs fils\n");
			exit(1);
		};break;
	case 0:
		{	// le processus fils
				
       			int lettreTrouvee[11] = {0};
       			int coupsRestants = 10;
       			
				close(PversF[W]); // on empeche le pere d'ecrire
				close(FversP[R]);// on empeche le fils de lire
				
				read(PversF[R],message,tailleMotSecret);
				
				srand(time(NULL));
				
				while (coupsRestants > 0 && !gagne(lettreTrouvee))
				{
					printf("\n\nIl vous reste %d coups a jouer", coupsRestants);
					printf("\nQuel est le mot secret ? ");


					for (i = 0 ; i < tailleMotSecret ; i++)
					{
					    if (lettreTrouvee[i]) // Si on a trouvé la lettre n° i
						printf("%c", message[i]); // On l'affiche
					    else
						printf("*"); // Sinon, on affiche une étoile pour les lettres non trouvées
					}
					
					printf("\nProposez une lettre : ");
					
					
					lettre = rand()%26+'A'; // generation du mot aleatoire
					
					printf(" %c  ",lettre);
					
					if (!rechercheLettre(lettre, message, lettreTrouvee))
					    {
						coupsRestants--; // On enlève un coup au joueur
					    }
				}
				
				
				write(FversP[W],lettreTrouvee,sizeof(tableauLettres) + 1);
				
				
		};break;
			
	default:
			{ // le processus pere
			
				close(FversP[W]); //on empeche le fils de d'ecrire
				close(PversF[R]); // on empeche le pere de lire
				
				write(PversF[W],motSecret, sizeof(tableauLettres) + 1);
				read(FversP[R],message1,tailleMotSecret) ;
				
				if (gagne(message1))
			   	 printf("\n\nGagne ! Le mot secret etait bien : %s \n\n", motSecret);
				else
			   	 printf("\n\nPerdu ! Le mot secret etait : %s \n\n", motSecret);

				
			}
		}
	

    return 0;
}


// permet de voir si le mot saisi existe dans la chaine
int rechercheLettre(char lettre, char motSecret[], int lettreTrouvee[])
{
    int i = 0;
    int bonneLettre = 0;

    // On parcourt motSecret pour vérifier si la lettre proposée y est 
    for (i = 0 ; motSecret[i] != '\0' ; i++)
    {
        if (lettre == motSecret[i]) // Si la lettre y est
        {
            bonneLettre = 1; // On mémorise que c'était une bonne lettre
            lettreTrouvee[i] = 1; // On met à 1 la case du tableau de booléens correspondant à la lettre actuelle
        }
    }

    return bonneLettre;
}



// verifie si le joueur a gagné
int gagne(int lettreTrouvee[])
{
    int i = 0;
    int joueurGagne = 1;

    for (i = 0 ; i < 6 ; i++)
    {
        if (lettreTrouvee[i] == 0)
            joueurGagne = 0;
    }

    return joueurGagne;
}


// choisir un chiffre aleatoirement
int ligneChiffre()
{
	int choix ;
	
	// generation d'un nombre aletoire entre 1 et 6
	srand(time(NULL));
	choix = rand()%6; 
	
	return choix;
}


// retourne la taile d'un mot
int tailleMot(char mot[])
{
	int i ;
	for(i=0;mot[i] != '\0';i++){}
	
	return i;
}
