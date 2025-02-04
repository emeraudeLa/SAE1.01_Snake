/**
* @file version1.c
* @brief Programme snake version 1
* @author Esmeralda MARTIN
* @version version 1
* @date 27/TAILLE/2024
*
* Programme snake, version 1, serpent de taille TAILLE qui avance uniquement vers la droite
* le programme s'arrete quand la touche 'a' est pressee
* demande avant a l'user où se situe le serpent au début du programme
*
*/

/* Fichiers inclus */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>


/* Déclaration des constantes*/
const char TETE='O', CORPS='X';
const char STOP='a';
const int TEMPS=500000;
const int TAILLE=10;
const int LIM_MAX=LIM_MAX;
const int LIM_MIN=0;


/* Déclaration des procédures */
void gotoXY(int x, int y);
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[]);
int kbhit();



int main (void)
{	
    int position_x, position_y, n;//varaibles a compléter par l'user, coordonées de départ et taille du serpent
    int lesX[TAILLE];//tab coordonees en x
    int lesY[TAILLE];//tab coordonees en y
    char touche;//touche frappée par l'user
    int a = 0;
    int x1,y1;//valeurs des tableaux de coordonnées
    int frappe;//si touche frappée, frappe=1

	//début programme
	
	do {
		printf("Position en x :");
		scanf("%d",&position_x);//lire le num de colonne de la tete
		if ((position_x <= LIM_MIN) || (position_x >=LIM_MAX))
		{
			printf("Nombre non compris entre 0 et LIM_MAX, redonner nombre\n");//message d'erreur
		}
	}while((position_x <= LIM_MIN) || (position_x >=LIM_MAX));
    
    do {
		printf("Position en y :");
		scanf("%d",&position_y);//lire le num de colonne de la tete
		if ((position_y <= LIM_MIN) || (position_y >=LIM_MAX))
		{
			printf("Nombre non compris entre 0 et LIM_MAX, redonner nombre\n");//message d'erreur
		}
	}while((position_y <= LIM_MIN) || (position_y >=LIM_MAX));

    printf("Taille N du serpent :");
    scanf("%d",&n);

    x1 = position_x;
    y1 = position_y;

    for (a = 0; a < TAILLE; a++)//calculer et enregistrer dans les tab les positions de tous les élem du serpent
    {
        lesX[a] = x1;
        x1 = x1 - 1;
        lesY[a] = y1;
    }

    system("clear");//vide la console
    dessinerSerpent(lesX,lesY);//dessiner le serpent

    while (touche != STOP) {//tant que touche != 'a' faire 
        progresser(lesX,lesY);//faire progresser le serpent d'une position vers la droite (=calculer sa nouvelle position)
        usleep(TEMPS);//temporiser
        frappe = kbhit();//regarde si touche frappee
        if (frappe == 1)//si une touche a été frappée alors
        {
            scanf("%c",&touche);//lire cette touche
        }//fin si
    }//fin tant que
    
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
	
    int a=0;

    for (a=0; a<TAILLE-1; a++)
    {
        afficher(lesX[a],lesY[a],CORPS);//affiche le corps du serpent
    }
    afficher(lesX[0],lesY[0],TETE);//affiche la tete du serpent
}

void progresser(int lesX[], int lesY[])
{
    /**
	* @brief procédure progresser, calcule et d’affiche la prochaine position du serpent. Par exemple, si
    le serpent a sa tête en (12, 3), la procédure doit faire en sorte que le serpent évolue d’une position vers
    la droite et donc l’affiche avec la tête en position (13, 3)
	* @param lesX tableau de type entier, entrée : coordonées en x
	* @param lesY tableau de type entier, entrée : coordonées en y
	*/

    int a=0;
    
    for (a=0; a<TAILLE; a++)
    {
		effacer(lesX[a],lesY[a]);//efface tout le serpent
	}
    for (a=0; a<TAILLE; a++)
    {
		lesX[a]=lesX[a]+1;//recalcule sa nouvelle position
	}
    dessinerSerpent(lesX,lesY);//dessiner le serpent
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


