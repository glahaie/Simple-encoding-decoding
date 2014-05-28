/* tp1.c
 * Par Guillaume Lahaie et Sylvain Labranche
 * LAHG04077707            LABS02059007
 *
 * Dernière modification: 11 novembre 2012
 *
 * Remise: 13 novembre 2012
 *
 * Utilisation du programme: Le programme peut encrypter ou décrypter une
 * chaine de caractères de toute longueur. Il est possible d'encrypter ou de
 * décrypter les chaines de caractères en 5, 7 ou 8 bits.
 *
 * Pour l'encryption / la décryption en 5 bits, les seules caractères légaux
 * sont les lettres minuscules et les blancs. Pour 7 bits, tous les caractères
 * ASCII 7 bits sont autorisés et pour 8 bits, tous les caractères ASCII 8 bits
 * sont autorisés.
 *
 * Pour l'encryption: Le message encrypté est affiché à l'aide d'une chaine de
 * caractère. Les caractères en majuscule représente une valeur binaire 1, et
 * un caractère minuscule est une valeur 0. Les espaces ne veulent rien dire.
 *
 * Pour la décryption, l'utilisateur doit seulement entrer des lettres majuscules
 * et minuscules, et des espaces blancs. Les espaces blancs sont ignorés. Le
 * programme affiche alors le message décrypté.
 *
 * Si le programme rencontre une erreur, il arrête le traitement de
 * l'encryption ou de la décryption et affiche un message d'erreur. Les erreurs
 * traitées sont les erreurs d'argument: mauvais nombre d'arguments, mauvaises
 * valeurs d'arguments, mauvais nom d'arguments, les erreurs d'encryption:
 * caractère illégal pour le nombre de bits choisi, et pour la décryption:
 * caractère illégal pour la décryption, nombre de caractères qui n'est pas
 * un multiple du nombre de bits.
 *
 * Utilisation du programme: <nom_programme> -(d ou e) -taille=(5, 7 ou 8)
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ERR_ARG 2	//Erreur avec -taille ou -d / -e
#define ERR_DEC 3   //Caractère illégal dans l'encryptage
#define ERR_ENC 4   //ou décryptage

#define MESS_ERR_DEC "Caractere illegal pour le décryptage.\n"\
                     "Usage: Lettres ou espaces seulement. "\
                     "Chaque caractere doit etre represente\n"\
                     "par le nombre de bits fourni dans l'argument "\
                     "taille sans tenir compte des espaces.\n"

#define MESS_ERR_ENC "Caractère illegal pour l'encodage.\n"\
                     "Usage: 5 bits: espaces et lettres "\
                     "minuscules seulement;\n"\
                     "7 bits: caracteres ASCII seulement;\n"\
                     "8 bits: caracteres ASCII Latin-1 seulement.\n"

#define TAILLE_ETRE 39
const char ETRE[] = "To be or not to be that is the question";


//Affiche un message d'erreur approprié incluant le nom du fichier
//et le numéro d'erreur.
void afficherErreur(int noErreur, const char *nomFichier);

//Encrypter le message entré par l'utilisateur, et affiche le message
//encrypté sur la sortie standard. Vérifie si le nombre de bits entré
//est permis et si les caractères entrés sont permis pour ce nombre de
//bits. La fonction retourne 0 si tout se déroule bien, et une valeur
//différente sinon.
int encrypter(const char *taille);

//Décrypter le message entré par l'utilisateur, vérifie si le nombre de
//bits est alloué, si les caractères sont alloués pour le décryptage
//(espaces ou lettres sans accents) et si le nombre de caractères correspond
//à un nombre de caractères décryptés complet. Affiche le message
//décrypté sur la sortie standard. La fonction retourne 0 si tout s'est bien
//déroulé et une valeur différente sinon.
int decrypter(const char *taille);

//Vérifie si le caractère est alloué pour le nombre de bits, et au besoin
//transforme le caractère. Si le caractère est illégal, la valeur retournée
//est -1.
int ajusterCar(int choixBits, int car);

//Vérifie si le nombre de bits est légal. Retourne le nombre de bits
//s'il est alloué, sinon retourne -1.
int validerTaille(const char *taille);


int main(int argc, char *argv[]) {

    int i, erreur = ERR_ARG;
    //Vérifier si on veut encrypter / décrypter
    //Vérifier aussi les arguments
    if(argc == 3) {
	    for(i = 1; i < argc; i++) {
	        if(strlen(argv[i]) == 2 && argv[i][0] == '-') {
	    	    switch(argv[i][1]) {
		            case 'd': erreur = decrypter((i==1?argv[2]:argv[1]));
                              break;
		            case 'e': erreur = encrypter((i==1?argv[2]:argv[1]));
                              break;
		            default:  erreur = ERR_ARG;
			                  break;
		        }
	        }
        }
    }
    if (erreur != 0) {
        afficherErreur(erreur, argv[0]);
    }

    return erreur;
}

int encrypter (const char *taille) {

	int i = 0, j= 0,
        car_en_bits[8], //Tableau contenant le caractère en binaire
        choixBits,      //Nombre de bits choisi par l'utilisateur
	    prochCar;       //Caractère lu

	//Vérifie que le choix de bits est 5, 7 ou 8.
    if((choixBits = validerTaille(taille)) < 0)    
        return ERR_ARG;

	//Début de l'encryption
	while((prochCar = getchar()) != EOF) {
	    //Transforme la valeur selon choixBits
        if((prochCar = ajusterCar(choixBits, prochCar)) < 0)
            return ERR_ENC;

    	for(i = choixBits-1; i >=0; i--) {
		    car_en_bits[i] = prochCar % 2;
		    prochCar = prochCar / 2;
	    }

	    //Afficher le caractère encrypté.
        i = 0;
	    while(i < choixBits) {

		    if(j >= TAILLE_ETRE || ETRE[j] == ' ') {
		        printf(" ");
		        j = (j >= TAILLE_ETRE?0:j+1);
		    } else {
		        printf("%c", (car_en_bits[i]==0?tolower(ETRE[j]):toupper(ETRE[j])));
			    i++;
			    j++;
		    }
	    }
	}
	printf("\n");
	return 0;
}

int decrypter(const char *taille) {

	int i = 0, j= 0,
        total,
        car_en_bits[8], //Tableau contenant la valeur binaire du caractère
        choixBits,      //Choix de bits par l'utilisateur
	    prochCar,       //Caractère lu
        puissanceDeux,
        finCar = 1;     //1 si on a traité un nombre de caractère qui est
                        // un multiple de choixBits, 0 sinon

	//Vérifie que le choix de bits est 5, 7 ou 8.
    if((choixBits = validerTaille(taille)) < 0)
        return ERR_ARG;

	while((prochCar = getchar()) != EOF) {
	    //Vérifie si c'est un caractere valide
	    if(!(isalpha(prochCar) || isspace(prochCar))) {
	    	return ERR_DEC;
	    }
	    if(islower(prochCar) || isupper(prochCar)) {
		    car_en_bits[i] = islower(prochCar)?0:1;
		    i++;
		    finCar = 0;
	    }

        //Décrypte le caractère.
	    if(i == choixBits) {

		    total = 0;
		    puissanceDeux = 1;
		    for(j = choixBits -1; j >= 0; j--) {
		        total += car_en_bits[j]*puissanceDeux;
		        puissanceDeux *= 2;
		    }

		    if(total == 0) {
			    printf(" ");
            } else {
                if(choixBits == 5) {
		            if (total > 26 || total < 1) {
			            return ERR_DEC;
                    } else {
			            printf("%c", total + 'a' -1);
                    }
                } else {
                    printf("%c", total);
                }
            }
		    i = 0;
		    finCar = 1;
	    }
	}
	printf("\n");
	if(!finCar) {
	    return ERR_DEC;
	}
	return 0;
}

void afficherErreur(int noErreur, const char *nomFichier) {

    printf("\n");
	fprintf(stderr, "%s: Erreur %d\n", nomFichier, noErreur);
	switch(noErreur) {
	    case ERR_ARG: fprintf(stderr, "Mauvais arguments.\n");
                      fprintf(stderr, "Usage: %s -(d ou e) -taille=(5 , 7 ou 8).\n",
                              nomFichier);
			          break;
	    case ERR_DEC: fprintf(stderr, "%s", MESS_ERR_DEC);
        			  break;
        case ERR_ENC: fprintf(stderr, "%s", MESS_ERR_ENC);
                      break;
	    default:	  fprintf(stderr, "Erreur inconnue.\n");
			          break;
	}
    return;
}

int ajusterCar(int choixBits, int car) {

    if(isspace(car)) {
	    return 0;
    } else {
	    if(choixBits ==5) {
	        if(!isalpha(car) || isupper(car))
		        return -1;
	        else
		        return car -'a' + 1;
	    } else if (choixBits== 7 && (car < 1 || car > 127)) {
		    return -1;
        }
    }
        return car;
}

int validerTaille(const char *taille) {

    int choixBits;
    if(!strncmp(taille, "-taille=", 8)
	    && strlen(taille) == 9) {

        switch(choixBits = taille[8] - '0') {

            case 5:
		    case 7:
		    case 8:  return choixBits;
			    	 break;
		}
    }
    return -1;
}
