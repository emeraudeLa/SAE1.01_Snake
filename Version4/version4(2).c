/**
* @file version4.c
* @brief Programme snake version 4
* @author Esmeralda MARTIN
* @version version 4
* @date 24/11/2024
*
* Programme snake, version 4, serpent de taille TAILLE qui peut se déplacer dans les quatre directions.
* le programme s'arrete quand la touche 'a' est pressee
* L’utilisateur peut changer la direction du serpent grâce aux touches zqsd
* position initiale pas demandée à l’utilisateur au début de l’exécution
* le serpent ne peut pas faire demi-tour sur lui-même
* on n'autorise pas le serpent à se croiser
* le serpent se déplacera à l’intérieur d’un plateau délimité
* obstacles sur le plateau
* le serpent doit manger des pommes (représentée par le caractère ‘6’)
* les pommes apparaissent aléatoirement à l’intérieur du plateau sur une case non déjà occupée par un élément de pavé ou de serpent
* Dès que le nombre de pommes mangées par le serpent atteint 10, le programme se termine et la partie est gagnée.
* plateau "percé" d’une sortie au centre de chacun de ses côtés, quand le serpent sort du cadre, il réapparaît sur le côté opposé.
*/

/* Fichiers inclus */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <stdbool.h>

/*Constantes symboliques*/

//Directions zqsd
#define HAUT 'z'
#define GAUCHE 'q'
#define BAS 's'
#define DROITE 'd'

//Taille du plateau
#define LARGEUR 80
#define HAUTEUR 40

//Taille du serpent
#define TAILLE_MAX 20



/* Déclaration des constantes*/
const char TETE = 'O', CORPS = 'X';
const char STOP = 'a';
const int TEMPS = 200000;
const int TAILLE = 10;
const int LIM_MAX = 40;
const int LIM_MIN = 1;
const int DEPART_X = 40;
const int DEPART_Y = 20;
const char MUR = '#';
const char VIDE = ' ';
const int NB_BLOC = 4;
const int TAILLE_BLOC = 5;
const int MOITIE_LARGEUR = 40;
const int MOITIE_HAUTEUR = 40;
const int MIN_LARGEUR = 1;
const int MIN_HAUTEUR = 1;
const char POMME = '6';
const float AUGMENTE_TEMPS = 0.9;
const int TROU_DROITE = 79;
const int TROU_GAUCHE = 1;
const int TROU_HAUT = 1;
const int TROU_BAS = 39;

/*Déclarations variables globales*/
int tailleSerpent = TAILLE;

/*Structures*/
typedef char aireDeJeu[LARGEUR+1][HAUTEUR+1];

/* Variables globales*/
aireDeJeu plateau;  // Déclaration globale de l'aireDeJeu

/* Déclaration des procédures */
void gotoXY(int x, int y);
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], char direction, bool *collision , bool *mangePomme);
void disableEcho();
void enableEcho();
void initPlateau(aireDeJeu plateau, int lesX[], int lesY[]);
void afficherPlateau(aireDeJeu plateau);
void initPaves(aireDeJeu plateau, int lesX[], int lesY[]);
void teleportation (int lesX[], int lesY[]);
void ajouterPomme(aireDeJeu plateau, int lesX[], int lesY[], int tailleSerpent);

/* Déclaration des fonctions */
int kbhit();
bool estObstacle(int x, int y);



int main()
{
    int tailleSerpent = TAILLE;  // Taille initiale du serpent
    int lesX[TAILLE_MAX]; // Tab coordonnées en x
    int lesY[TAILLE_MAX]; // Tab coordonnées en y
    char touchePrecedente; // Touche frappée par l'utilisateur
    char toucheSaisie;	
    int coordX, coordY; // Valeurs des tableaux de coordonnées
    bool collision; // Si serpent touche une bordure, lui-même ou un pavé
    bool mangePomme;//Si le serpent mange une pomme
    int temps=TEMPS;//Vitesse de délpacement du serpent
	
	//Début programme
	
    srand(time(NULL));

    coordX = DEPART_X;
    coordY = DEPART_Y;
    collision = false;

    for (int i = 0; i < tailleSerpent; i++) {//Calculer et enregistrer dans les tab les positions de tous les élem du serpent
        lesX[i] = coordX;
        coordX = coordX - 1;
        lesY[i] = coordY;
    }

    system("clear"); // Vide la console
    initPlateau(plateau, lesX, lesY); // Initialise le plateau de jeu
    ajouterPomme(plateau, lesX, lesY, tailleSerpent);//Ajoute une pomme sur le plateau
    afficherPlateau(plateau); // Affiche le plateau
    dessinerSerpent(lesX, lesY); // Dessine le serpent
    touchePrecedente = DROITE; // Serpent va vers la droite
    
    disableEcho();//Enlève les caractères frappés à l'écran pour qu'ils ne soient pas visibles

    while (toucheSaisie != STOP && !collision && tailleSerpent!=TAILLE_MAX) { // Tant que touche != 'a'  et qu'il n'y a pas de collision et que le serpent n'a pas mangé 10 pommes faire
		
        usleep(temps); // Temporisation
        int frappe = kbhit(); // Regarde si touche frappée
        if (frappe == 1) {//Si une touche a été frappée alors
            scanf("%c", &toucheSaisie); // Lire cette touche
        }//Fin si
		
        if (((toucheSaisie == DROITE) && (touchePrecedente != GAUCHE))||//Si le serpent veut aller vers la droite et qu'il ne va pas déjà a gauche
			((toucheSaisie == GAUCHE) && (touchePrecedente != DROITE))||//Si le serpent veut aller vers la gauche et qu'il ne va pas déjà a droite
			((toucheSaisie == HAUT) && (touchePrecedente != BAS))||//Si le serpent veut aller vers le haut et qu'il ne va pas déjà en bas
			((toucheSaisie == BAS) && (touchePrecedente != HAUT)))//Si le serpent veut aller vers le bas et qu'il ne va pas déjà en haut
        {
            touchePrecedente = toucheSaisie; // La touche prend la valeur rentrée par l'utilisateur
        }//Fin si

        progresser(lesX, lesY, touchePrecedente, &collision, &mangePomme);// Faire progresser le serpent d'une position vers la direction souhaitée (=calculer sa nouvelle position), vérifie si collision, vérifie si mange pomme

        if (mangePomme)//Si le serpent a mangé une pomme
        {
            ajouterPomme(plateau, lesX, lesY, tailleSerpent);// Ajoute une nouvelle pomme
            tailleSerpent++; // Augmente la taille du serpent
			temps=temps*AUGMENTE_TEMPS;//Augmente la vitesse du serpent
        }//Fin_si

        mangePomme = false;  // Réinitialisation de la variable mangePomme

    }//Fin tant_que

    enableEcho();//Rend à nouveau visibles les caractères frappés à l'écran
    printf("\n");

    return EXIT_SUCCESS;
    //Fin
}

void gotoXY(int x, int y)
{
	//Procedure donnée sur moodle
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
    gotoXY(LIM_MIN,LIM_MAX);//Place le curseur hors du serpent
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
    for (int i = 0; i < TAILLE_MAX; i++)
    {
		if ((lesX[i] >= LIM_MIN) && (lesY[i] >= LIM_MIN))//Affiche le serpent seulement s'il est dans la console
											//S'il sort de la console, on ne l'affiche pas
		{
			if (i==0)
			{
				afficher(lesX[i],lesY[i],TETE);//Affiche la tete du serpent
			}
			else
			{
				afficher(lesX[i],lesY[i],CORPS);//Affiche le corps du serpent
			}//Fin si
		}//Fin si
    }//Fin boucle for
    
}

bool estObstacle(int x, int y)
{	
	/**
	* @brief fonction estObstacle, permet de savoir si un caractère '#' est présent aux coordonnée x,y
	* @param x de type entier, entrée : coordonées en x
	* @param y de type entier, entrée : coordonées en y
	* @return true ou false si caractère présent ou non
	*/
    return plateau[x][y] == MUR;  // Renvoie true si il y a un '#' à la case x,y du plateau
}


void progresser(int lesX[], int lesY[], char direction, bool *collision, bool *mangePomme)
{	
    /**
	* @brief procédure progresser, calcule et affiche la prochaine position du serpent. 
	* Par exemple, si le serpent a sa tête en (12, 3), la procédure doit faire en sorte que 
	* le serpent évolue d’une position vers la droite et donc l’affiche avec la tête en position (13, 3)
	* @param lesX tableau de type entier, entrée : coordonnées en x
	* @param lesY tableau de type entier, entrée : coordonnées en y
	* @param direction de type caractère, entrée : direction du serpent 'z' pour haut, 's' pour bas, 'q' pour gauche, 'd' pour droite
	* @param collision de type booléen, sortie : true ou false si le serpent arrive sur une bordure, un obstacle ou lui-même
	* @param mangePomme de type booléen, sortie : true ou false si le serpent mange une pomme
	*/
    
    // Effacer la position actuelle du serpent
    for (int i = 0; i < tailleSerpent; i++)
    {
        effacer(lesX[i], lesY[i]);
    }

    // Déplacer le serpent (téléportation pour la tête et déplacement des autres éléments)
    teleportation(lesX, lesY);

    // Vérifie si le serpent mange une pomme
    if (plateau[lesX[0]][lesY[0]] == POMME)
    {
        *mangePomme = true;
        plateau[lesX[0]][lesY[0]] = VIDE;  // Efface la pomme du plateau

        // Ajouter un nouvel élément au serpent à la fin
        lesX[tailleSerpent] = lesX[tailleSerpent - 1];  // La nouvelle case X prend la position de l'élément précédent
        lesY[tailleSerpent] = lesY[tailleSerpent - 1];  // La nouvelle case Y prend la position de l'élément précédent
        tailleSerpent++;  // Augmente la taille du serpent
    }
    else
    {
        *mangePomme = false;//Renvoie false si pomme non mangée
    }

    //Décale les valeurs des tableaux d'une case pour permettre à la tête d'avoir ses nouvelles coordonnées
    for (int i = tailleSerpent - 1; i > 0; i--) {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    // Recalcule la position de la tête en fonction de la direction
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

    // Vérifie si collision : si la prochaine position de la tête est un obstacle
    if (estObstacle(lesX[0], lesY[0])) {
        *collision = true;  // Collision
    }
    else
    {
        *collision = false; // Pas de collision
    }

    // Vérifie si la tête se retrouve sur son propre corps
    for (int i = 1; i < tailleSerpent; i++)
    {
        if (lesX[0] == lesX[i] && lesY[0] == lesY[i])//Si la tête arrive sur un des éléments du serpent
        {
            *collision = true; // Collision
        }
    }

    // Redessine le serpent à sa nouvelle position
    dessinerSerpent(lesX, lesY);
}






void teleportation (int lesX[], int lesY[])
{
	/**
	* @brief procédure teleportation, calcule et affiche la prochaine position du serpent s'il passe par un des trous du plateau 
	* @param lesX tableau de type entier, entrée : coordonnées en x
	* @param lesY tableau de type entier, entrée : coordonnées en y
	*/
	
	if (lesX[0]==TROU_DROITE)//Passe par le trou à droite
	{
		lesX[0]=TROU_GAUCHE;//Met la tête du serpent dans le trou a gauche
	}
	else if (lesX[0]==TROU_GAUCHE)//Passe par le trou à gauche
	{
		lesX[0]=TROU_DROITE;//Met la tête du serpent dans le trou a droite
	}
	else if (lesY[0]==TROU_HAUT)//Passe par le trou en haut
	{
		lesY[0]=TROU_BAS;//Met la tête du serpent dans le trou en bas
	}
	else if (lesY[0]==TROU_BAS)//Passe par le trou en bas
	{
		lesY[0]=TROU_HAUT;//Met la tête du serpent dans le trou en haut
	}
	
}


void disableEcho()
{
	//Procédure donnée sur moodle
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
	//Procédure donnée sur moodle
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

void afficherPlateau(aireDeJeu plateau)
{
	/**
	* @brief procédure afficherPlateau, affiche tout le plateau et ses bords
	* @param plateau tableau de type aireDeJeu, entrée : éléments à placer sur la console
	*/
    for (int i = 0; i < LARGEUR; i++)
    {
        for (int j = 0; j < HAUTEUR; j++)
        {
            afficher(i, j, plateau[i][j]);  // Afficher chaque case du plateau
        }
        printf("\n");//Retour à la ligne après avoir fini une ligne
    }
}

void initPlateau(aireDeJeu plateau, int lesX[], int lesY[])
{
	/**
	* @brief procédure initPlateau, initialise tout le plateau et ses bords
	* @param plateau tableau de type aireDeJeu, entrée : éléments à placer sur la console
	* @param lesX tableau de type entier, entrée : coordonées en x
	* @param lesY tableau de type entier, entrée : coordonées en y
	*/ 
    // Initialise les bords du plateau
    for (int i = 0; i < LARGEUR; i++)
    {
        for (int j = 0; j < HAUTEUR; j++)
        {
            if (i == MIN_LARGEUR || i == LARGEUR - 1 || j == MIN_HAUTEUR || j == HAUTEUR - 1) {
                plateau[i][j] = MUR;  //Remplir les bords du plateau avec des '#'
            } else {
                plateau[i][j] = VIDE;  //Remplir l'intérieur du plateau avec des ' '
            }
        }
    }
    
    for (int i = 0; i < NB_BLOC; i++) { //Ajouter NB_BLOC obstacles
        initPaves(plateau,lesX,lesY);  // Ajouter des obstacles à des endroits aléatoires
    }
    
    //Trous sur les cotés
    plateau[TROU_GAUCHE][HAUTEUR/2]=VIDE;// sortie a gauche
    plateau[LARGEUR/2][TROU_HAUT]=VIDE;// sortie en haut
    plateau[LARGEUR/2][TROU_BAS]=VIDE;// sortie en bas
    plateau[TROU_DROITE][HAUTEUR/2]=VIDE;// sortie a droite
}

void initPaves(aireDeJeu plateau, int lesX[], int lesY[]) 
{
    /**
    * @brief procédure initPaves, initialise les obstacles du plateau pour ensuite les placer
    * @param plateau tableau de type aireDeJeu, entrée : éléments à placer sur la console
    * @param lesX tableau de type entier, entrée : coordonées en x
    * @param lesY tableau de type entier, entrée : coordonées en y
    */ 
    bool estSurSerpent = true; // Si l'obstacle est sur un élément du serpent
    int x, y; // Coordonnées de l'obstacle

    // Vérifier si la position générée pour le pavé est sur le serpent ou trop proche de la tête du serpent
    while (estSurSerpent) {
        // Position aléatoire pour le pavé 
        x = rand() % (LARGEUR - TAILLE_BLOC - 6) + 3;  // Décalage de 3 cases pour les bords(marge de sécurité)
        y = rand() % (HAUTEUR - TAILLE_BLOC - 6) + 3; // Décalage de 3 cases pour les bords(marge de sécurité)

        estSurSerpent = false;// Réinitialise estSurSerpent à false à chaque nouvelle tentative

        for (int i = 0; i < TAILLE && !estSurSerpent; i++)// Vérifie si l'obstacle est dans une zone de 3 cases autour de la tête du serpent
        {
            for (int j = x; j < x + TAILLE_BLOC && !estSurSerpent; j++)
            {
                for (int k = y; k < y + TAILLE_BLOC && !estSurSerpent; k++)
                {
                    //La fonction abs() permet de comparer la distance en coordonnées X et Y entre la tête du serpent et la position du pavé
                    if (abs(lesX[0] - j) <= 3 && abs(lesY[0] - k) <= 3)// Vérifie de la zone autour de la tête du serpent
                    {
                        estSurSerpent = true;  // Pavé trop proche de la tête
                    }//Fin si
                    
                    if (!estSurSerpent && lesX[i] == j && lesY[i] == k)// Vérifie si le pavé ne touche pas une partie du serpent
                    {
                        estSurSerpent = true;  // L'obstacle touche le serpent
                    }//Fin si
                }//Fin boucle For
            }//Fin boucle For
        }//Fin boucle For
    }//Fin boucle tant_que

   
    for (int i = x; i < x + TAILLE_BLOC; i++)
    {
        for (int j = y; j < y + TAILLE_BLOC; j++)
        {
            plateau[i][j] = MUR;   // Ajout du pavé dans le plateau si l'emplacement est pas sur serpent ou à côté
        }//Fin boucle For
    }//Fin boucle For
}

int kbhit()
{	
	/**
	* @brief fonction kbhit, permet de savoir si une touche a été frappée sur le clavier
	* @return unCaractere, 1 si un caractere est present, 0 si pas de caractere present
	*/
	//Fonction donnée sur moodle
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

void ajouterPomme(aireDeJeu plateau, int lesX[], int lesY[], int tailleSerpent) 
{
    /**
	* @brief procédure ajouterPomme, ajoute une pomme sur le plateau
	* @param plateau tableau de type aireDeJeu, entrée : éléments à placer sur la console
	* @param lesX tableau de type entier, entrée : coordonnées en x du serpent
	* @param lesY tableau de type entier, entrée : coordonnées en y du serpent
	* @param tailleSerpent de type entier, entrée : taille actuelle du serpent
	*/
    
    srand(time(NULL));
    bool estSurSerpent = true; // Si la pomme est sur un élément du serpent
    int x, y; // Coordonnées de la pomme

    // Vérifier si la position générée pour la pomme est sur le serpent ou un obstacle
    while (estSurSerpent) {
        // Position aléatoire pour la pomme
        x = rand() % (LARGEUR - 2) + 1;  // Génère une position x entre 1 et LARGEUR-2
        y = rand() % (HAUTEUR - 2) + 1;  // Génère une position y entre 1 et HAUTEUR-2

        estSurSerpent = false; // Réinitialise estSurSerpent à false à chaque nouvelle tentative

        // Vérifie si la position générée est sur un élément du serpent ou un obstacle
        if (plateau[x][y] == MUR)
        {
            estSurSerpent = true;  // La position est un obstacle, réessaie
        }

        // Vérifie si la position est sur le serpent
        for (int i = 0; i < tailleSerpent; i++)
        {
            if (lesX[i] == x && lesY[i] == y)
            {
                estSurSerpent = true;  // La pomme est sur le serpent, réessaie
            }
        }
    }

    plateau[x][y] = POMME;   // Ajoute la pomme à la position valide
    afficher(x,y,POMME);//Affiche la pomme sur le plateau
}




