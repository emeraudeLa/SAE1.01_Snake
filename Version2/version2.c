/**
* @file version2.c
* @brief Programme snake version 2
* @author Esmeralda MARTIN
* @version version 2
* @date 08/11/2024
*
* Programme snake, version 2, serpent de taille TAILLE qui peut se déplacer dans les quatre directions.
* le programme s'arrete quand la touche 'a' est pressee
* L’utilisateur peut changer la direction du serpent grâce aux touches zqsd
* position initiale pas demandée à l’utilisateur au début de l’exécution
* le serpent ne peut pas faire demi-tour sur lui-même
* on autorise le serpent à se croiser
*/

/* Fichiers inclus */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

/*Constantes symboliques*/
#define HAUT 'z'
#define GAUCHE 'q'
#define BAS 's'
#define DROITE 'd'


/* Déclaration des constantes*/
const char TETE = 'O', CORPS = 'X';
const char STOP = 'a';
const int TEMPS = 200000;
const int TAILLE = 10;
const int LIM_MAX = 40;
const int LIM_MIN = 0;
const int DEPART = 20;

/* Déclaration des procédures */
void gotoXY(int x, int y);
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], char direction);
void disableEcho();
void enableEcho();
int kbhit();



int main ()
{	
    int lesX[TAILLE];//tab coordonees en x
    int lesY[TAILLE];//tab coordonees en y
    char touchePrecedente;//touche frappée par l'user
    char toucheSaisie;
    int coordX,coordY;//valeurs des tableaux de coordonnées
    int frappe;//si touche frappée, frappe=1

	//début programme

    coordX = DEPART;
    coordY = DEPART;

    for (int i = 0; i < TAILLE; i++)//calculer et enregistrer dans les tab les positions de tous les élem du serpent
    {
        lesX[i] = coordX;
        coordX = coordX - 1;
        lesY[i] = coordY;
    }

    system("clear");//vide la console
    dessinerSerpent(lesX,lesY);//dessiner le serpent
    touchePrecedente=DROITE;//serpent va vers la droite
	disableEcho();
	
    while (toucheSaisie != STOP) {//tant que touche != 'a' faire
        progresser(lesX,lesY,touchePrecedente);//faire progresser le serpent d'une position vers la direction souhaitée (=calculer sa nouvelle position)
        usleep(TEMPS);//temporiser
        frappe = kbhit();//regarde si touche frappee
        if (frappe == 1)//si une touche a été frappée alors
        {	
            scanf("%c",&toucheSaisie);//lire cette touche
        }//fin si
        
        if (((toucheSaisie == DROITE) && (touchePrecedente != GAUCHE))||//si le serpent veut aller vers la droite et qu'il ne va pas déjà a gauche
			((toucheSaisie == GAUCHE) && (touchePrecedente != DROITE))||//si le serpent veut aller vers la gauche et qu'il ne va pas déjà a droite
			((toucheSaisie == HAUT) && (touchePrecedente != BAS))||//si le serpent veut aller vers le haut et qu'il ne va pas déjà en bas
			((toucheSaisie == BAS) && (touchePrecedente != HAUT)))//si le serpent veut aller vers le bas et qu'il ne va pas déjà en haut
        {
			touchePrecedente = toucheSaisie;//la touche prend la valeur rentrée par l'user
		}//fin_si
		
    }//fin tant que
    enableEcho();
    
    printf("\n");

    return EXIT_SUCCESS;
}

void gotoXY(int x, int y)
{
	//procedure donnee sur moodle
	/**
	* @brief procédure gotoXY, permet de se placer a un certain endroit de la console
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	*/
    printf("\033[%d;%df", y, x);
}

void afficher(int x, int y, char c)
{
	/**
	* @brief procédure afficher, affiche le caractère c à la position (x, y), où x représente le numéro de colonne
    et y le numéro de ligne.
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	* @param c de type caractere, entrée : caractère a afficher
	*/
    gotoXY(x,y);
    printf("%c",c);
    gotoXY(LIM_MIN,LIM_MAX);//place le curseur hors du serpent
}

void effacer(int x, int y)
{
    /**
	* @brief procédure effacer, affiche un espace à la position (x, y), où x représente le numéro de colonne et y
    le numéro de ligne.
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	*/
    gotoXY(x,y);
    printf(" ");
}


void dessinerSerpent(int lesX[], int lesY[])
{
    
    /**
	* @brief procédure dessinerSerpent, affiche à l’écran un à un les éléments du serpent dont les coordonnées
    sont fournies dans le(s) tableau(x) en paramètre(s).
	* @param lesX tableau de type entier, entrée : coordonées en x
	* @param lesY tableau de type entier, entrée : coordonées en y
	*/

    for (int i = 0; i < TAILLE; i++)
    {
		if ((lesX[i] >= 0) && (lesY[i] >= 0))//affiche le serpent seulement s'il est dans la console
											//s'il sort de la console, on ne l'affiche pas
		{
			if (i==0)
			{
				afficher(lesX[i],lesY[i],TETE);//affiche la tete du serpent
			}
			else
			{
				afficher(lesX[i],lesY[i],CORPS);//affiche le corps du serpent
			}//fin_si
		}//fin_si
    }//fin boucle for
}

void progresser(int lesX[], int lesY[], char direction)
{
    /**
	* @brief procédure progresser, calcule et d’affiche la prochaine position du serpent. Par exemple, si
    le serpent a sa tête en (12, 3), la procédure doit faire en sorte que le serpent évolue d’une position vers
    la droite et donc l’affiche avec la tête en position (13, 3)
	* @param lesX tableau de type entier, entrée : coordonées en x
	* @param lesY tableau de type entier, entrée : coordonées en y
	* @param direction de type caractère, entrée : direction du serpent 'z' pour haut, 's' pour bas, 'q' pour gauche, 'd' pour droite
	*/

    // efface la position actuelle du serpent
    for (int i = 0; i < TAILLE; i++)
    {
        effacer(lesX[i], lesY[i]);
    }
    
    //décale les valeurs des tableaux d'une case pour permettre a la tete d'avoir ses nouvelles coordonnées
	for (int i = TAILLE; i > 0; i--)
    {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }
    
    
    // recalcule la position de la tete en fonction de la direction
    switch (direction)
    {
        case HAUT: // Haut
            lesY[0] = lesY[0] - 1;
            break;
            
        case BAS: // Bas
            lesY[0] = lesY[0] + 1;
            break;
            
        case GAUCHE: // Gauche
            lesX[0] = lesX[0] - 1;
            break;
            
        case DROITE: // Droite
            lesX[0] = lesX[0] + 1;
            break;
            
        default:
            break; // Si touche != de z,q,s,d, fait rien
    }

	
    // Dessine le serpent avec sa nouvelle position
    dessinerSerpent(lesX, lesY);
}

void disableEcho()
{
	//procedure donnee sur moodle
	/**
	* @brief procédure disableEcho, pour éviter que les caractères utilisés pour diriger le serpent s’affichent à l’écran
	*/
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho() 
{	
	//procedure donnee sur moodle
	/**
	* @brief procédure enableEcho, pour afficher à nouveau les caractères sur la console
	*/
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}



int kbhit()
{	
	/**
	* @brief fonction kbhit, permet de savoir si une touche a été frappée sur le clavier
	* @return unCaractere, 1 si un caractere est present, 0 si pas de caractere present
	*/
	//fonction donnee sur moodle
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}


