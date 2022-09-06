#include <stdio.h>
#define NbrColonne 7
#define NbrLigne 6
#include <stdlib.h>
#include <time.h>

void afficheClair(int plateau[NbrLigne][NbrColonne])
{
  /* affichage du plateau */
  int x, y;
  printf("| C1 | C2 | C3 | C4 | C5 | C6 | C7 |\n");
  for (y=0; y<NbrLigne; y++) {
    printf("| ");
    for (x=0; x<NbrColonne; x++) {
      if(plateau[y][x]==1){
	printf("O  | ");
      }else if(plateau[y][x]==2){
	printf("X  | ");
      }else if(plateau[y][x]==0){
	printf("-  | ");
      }else{
	printf("%d  | ",plateau[y][x]);
      }
    }
    printf("\n|----|----|----|----|----|----|----|");
    printf("\n");
  }
}
void initialiseVide(int plateau[NbrLigne][NbrColonne])
{
  /* Initialisation du plateau */
  int x, y;
  for (y=0; y<NbrLigne; y++) {
    for (x=0; x<NbrColonne; x++) {
      plateau[y][x]= 0;
    }
  }
}
int position(int plateau[NbrLigne][NbrColonne], int choix, int idcheck)
{
  int suivant, y;
  /* Premier cas : Recherche s'il y a déjà des jetons dans la colonne ? */
  for(y=0;y<NbrLigne-1;y++){
    suivant = plateau[y+1][choix];
    if(suivant!=0){
      if (idcheck == 1){
	plateau[y][choix] = 1;
	return y;
      }else if(idcheck == 2) {
	plateau[y][choix] = 2;
	return y;
      }
    }
  }
  /* Second cas : Aucun jeton donc on met le jeton dans la dernière ligne*/
  if(idcheck == 1) {
    plateau[y][choix] = 1;
    return y;
  }else if(idcheck == 2){
    plateau[y][choix] = 2;
    return y;
  }
  return 0;
}

int plateauPlein(int plateau[NbrLigne][NbrColonne])
{
  /* Reste-t-il des cases vides dans le plateau ? */
  int x, y;
  for (y=0; y<NbrLigne; y++) {
    for (x=0; x<NbrColonne; x++) {
      if(plateau[y][x] == 0){
	return 0;
      }
    }
  }
  return 10;
}

int plateauVictoire(int plateau[NbrLigne][NbrColonne], int position_x, int position_y,int idcheck)
{
  /*pions fantômes*/
  int piond, piong, pionb,i,j;
  piond = 0;
  piong = 0;
  pionb = 0;
  i = position_x;
  j = position_y;
  /*vérification ligne*/
  while(plateau[j][i+1]==idcheck && i+1<NbrColonne){
    piond=piond+1;
    i++;
  }
  i = position_x;
  while(plateau[j][i-1]==idcheck && j-1 >=0){
    piong=piong+1;
    i--;
  }
  if((piong+piond+1)>=4){
    return idcheck;
  }
  /*vérification colonne*/
  while(plateau[j+1][i]==idcheck && j+1<NbrLigne){
    pionb=pionb+1;
    j++; 
  }
  j = position_y;
  while(plateau[j-1][i]==idcheck && j-1>=0){
    pionb=pionb+1;
    j++;
  }
  if(pionb+1>=4){
    return idcheck;
  }
  /*vérification diagonale: ascendante et descendante*/
  piong=0;
  piond=0;
  i = position_x;
  j = position_y;
  /*On commence avec la diagonale ascendante*/
  while(plateau[j+1][i+1]==idcheck && j+1<NbrLigne && i+1<NbrColonne){
    piond=piond+1;
    j++;
    i++;
  }
  i = position_x;
  j = position_y;
  while(plateau[j-1][i-1]==idcheck && j-1 >=0 && i-1>=0){
    piong=piong+1;
    j--;
    i--;
  }
  if((piong+piond+1)>=4){
    return idcheck;
  }
  /*et maintenant la diagonale descendante*/
  piong=0;
  piond=0;
  i = position_x;
  j = position_y;
  while(plateau[j+1][i-1]==idcheck && j+1<NbrLigne && i-1>=0){
    piond=piond+1;
    j++;
    i--;
  }
  i = position_x;
  j = position_y;
  while(plateau[j-1][i+1]==idcheck && j-1>=0 && i+1<NbrColonne){
    piong=piong+1;
    j--;
    i++;
  }
  if((piong+piond+1)>=4){
    return idcheck;
  }
  return 0;
}

int extensionGo(int plateau[NbrLigne][NbrColonne],int x, int y, int idjoueur)
{
  int pionf,flag;
  flag =0;
  pionf=0;
  /* Cas n°1 : le pion est au dessus du pion à convertir */
  if(plateau[y][x]==plateau[y+1][x+1] && y+1<NbrLigne && x+1<NbrColonne){
    pionf=pionf+1;
    if(plateau[y][x]==plateau[y+1][x-1] && y+1<NbrLigne && x-1>=0){
      pionf=pionf+1;
      if(plateau[y][x]==plateau[y+2][x] && y+2<NbrLigne){
	pionf=pionf+1;
      }}}
  if(pionf==3){
    plateau[y+1][x]=idjoueur;
    /* Cette modification a-t-elle permit une victoire ? */
    flag=plateauVictoire(plateau,x,y,idjoueur);
    if(flag !=0){
      return flag;
    }
  }
  /* Cas n°2: le pion est à droite du pion à convertir */
  pionf=0;
  if(plateau[y][x]==plateau[y-1][x-1] && y-1>=0 && x-1>=0){
    pionf=pionf+1;
    if(plateau[y][x]==plateau[y][x-2] && x-2>=0){
      pionf=pionf+1;
      if(plateau[y][x]==plateau[y+1][x-1] && y+1<NbrLigne && x-1>=0){
	pionf=pionf+1;
      }}}
  if(pionf==3){
    plateau[y][x-1]=idjoueur;
    /* Cette modification a-t-elle permit une victoire ? */
    flag=plateauVictoire(plateau,x,y,idjoueur);
    if(flag !=0){
      return flag;
    }
  }
  /* Cas n°3 : le pion est à gauche du pion à convertir */
  pionf=0;
  if(plateau[y][x]==plateau[y-1][x+1] && y-1>=0 && x+1<NbrColonne){
    pionf=pionf+1;
    if(plateau[y][x]==plateau[y][x+2] && x+2<NbrColonne){
      pionf=pionf+1;
      if(plateau[y][x]==plateau[y+1][x+1] && y+1<NbrLigne && x+1<NbrColonne){
	pionf=pionf+1;
      }}}
  if(pionf==3){
    plateau[y][x+1]=idjoueur;
    /* Cette modification a-t-elle permit une victoire ? */
    flag=plateauVictoire(plateau,x,y,idjoueur);
    if(flag !=0){
      return flag;
    }
  }
  return 0;
}

int joueurContrejoueur(int plateau[NbrLigne][NbrColonne])
{
  int flag1, flag2, choix, idjoueur,position_x, position_y, go,extension;
  flag1 = 0;
  flag2 = 0;
  /* Extension Go ? */
  printf("Souhaitez vous rajouter l'extion de Go (Un pion encerclé par 4 pions adverse se verra convertit en un pion adverse ?\n(1)oui\n(0)non\n");
  scanf("%d",&go);
  while(go != 1 && go != 0 ){
    printf("Vous n'avez pas définit si vous souhaitiez l'extension Go ou non ?\n(1) oui\n(0) non\n");
    scanf("%d",&go);
  }

  printf("Choisissez qui commence la partie :\nRond (tapez 1) \nCroix(tapez 2)\n");
  scanf("%d",&idjoueur);
  printf("\n");

  /* L'IDjoueur existe-t-elle ? */
  while(idjoueur != 1 && idjoueur != 2 ){
    printf("Le numéro de joueur doit etre 1 ou 2, tapez donc 1 (Rond) ou 2(Croix).\n");
    scanf("%d",&idjoueur);
  }
  while (flag1 == 0 && flag2 ==0) { 
    printf("C'est le tour du joueur n°%d, voici le plateau : \n",idjoueur);
    afficheClair(plateau);
    printf("\nDans quelle colonne souhaitez vous jouer ?\n");
    scanf("%d",&choix);
    choix = choix -1;

    /* La colonne choisie existe-t-elle sur le plateau ? */
    while (choix> NbrColonne-1 || choix<0) {
      printf("La colonne demandé n'est pas dans le plateau.Choisissez en une autre. \n");
      scanf("%d",&choix);
      choix = choix -1;
    }

    /* La colonne choisie est-elle pleine ? */
    while (plateau[0][choix] != 0) {
      printf("La colonne est déjà pleine. Choisissez en une autre.\n");
      scanf("%d",&choix);
      choix = choix - 1;
    }
    position_x = choix;
    position_y = position(plateau,choix,idjoueur);
    if(go == 1){
      extension=extensionGo(plateau,position_x,position_y, idjoueur);
      if(extension!=0){
	return idjoueur;
      }
    }
    /* Quelqu'un a-t-il gagné ? */
    position_x = choix ;
    flag2 = plateauVictoire(plateau,position_x,position_y,idjoueur);
    if(flag2 !=0){
      return idjoueur;
    }
    /* Le tableau est-il plein ? */
    flag1 = plateauPlein(plateau);
    if(flag1 != 0){
      return flag1;
    }
    /* A la fin du tour, inversion des ID */
    if (idjoueur == 1) {
      idjoueur=2;
    }else if(idjoueur == 2) {
      idjoueur=1;
    }
  } 
  return 0;
}

int intelligenceArtificielle1(int plateau[NbrLigne][NbrColonne])
{
  int choix, max;
  srand(time(NULL));
  max = NbrColonne;
  choix = rand() %max;
  while (plateau[0][choix] != 0) {
    choix = rand() %max;
  }
  return choix;
}
int positionPotentielle ( int plateau[NbrLigne][NbrColonne], int coupPotentiel_x){
  //on cherche à simuler le tomber d'un jeton pour IA sournoise
  int suivant, y;
  for (y=0; y<NbrLigne-1; y++){
    suivant = plateau[y+1][coupPotentiel_x];
    if (suivant != 0){
      return y;
    }
  }
  return y;
}

int IAsournois(int plateau[NbrLigne][NbrColonne],int idadversaire, int idordi){
  /*même principe que pour la victoire, sauf que si il détecte 3 pions alignés du joueur adverse, il le bloque, sinon jeu aléatoire. A traiter comme un joueur !*/
  int max, i;
  int coupPotentiel_x;
  int coupPotentiel_y;
  i =0;
  /* Premier cas : Peut-elle jouer un coups gagnant ?*/
  while(i<NbrColonne){
    coupPotentiel_x=i;
    /* Dernière ligne pleine */
    if (plateau[0][coupPotentiel_x]!=0){
      i++;
    }else{
      /* On regarde la position du jeton qu'on jouerait dans cette colonne */
      coupPotentiel_y = positionPotentielle(plateau, coupPotentiel_x);
      if(plateauVictoire(plateau, coupPotentiel_x, coupPotentiel_y, idordi) !=0){
	/* Victoire possible donc on joue ce coups.*/
	return coupPotentiel_x;
      }else {
	/* Sinon on change de colonne */
	i++;
      }}}
  /*Second cas : Peut-on bloquer le prochain coups de l'adversaire ? */
  i=0;
  while(i<NbrColonne){
    coupPotentiel_x=i;
    /* Dernière colonne pleine ?*/
    if (plateau[0][coupPotentiel_x]!=0){
      i++;
    }else{
      /* Si le joueur adverse jouait dans cette colonne */
      coupPotentiel_y =positionPotentielle(plateau, coupPotentiel_x);
      if(plateauVictoire(plateau, coupPotentiel_x,coupPotentiel_y, idadversaire)!=0){
	/* Victoire possible du joueur donc l'IA doit le bloquer */
	printf("choix intelligent : contrer.\n");
	return coupPotentiel_x;
      }else {
	i++;
      }}}
  /* Troisième cas : Rien à faire d'intelligent soyons idiot */
  srand(time(NULL));
  max = NbrColonne;
  coupPotentiel_x = rand () %max;
  while (plateau[0][coupPotentiel_x] !=0) {
    coupPotentiel_x = rand () %max;
  }
  printf("choix idiot.\n");
  return coupPotentiel_x;
}

int joueurContreordi(int plateau[NbrLigne][NbrColonne])
{
  int flag1, flag2, choix, idjoueur, idordi, diff, position_y;
  flag1 = 0;
  flag2 = 0;
  printf("Choisissez le niveau de difficulté de votre adversaire :\n1. No brain\n2. Facile mais sournois\n");
  scanf("%d",&diff);
  while(diff != 1 && diff != 2 && diff !=3){
    printf("Vous devez choisir un niveau existant : \n(1) : Très Facile\n(2) : Facile\n");
    scanf("%d",&diff);
  }
  printf("Choisissez votre symbole : \nRond (tapez 1) \nCroix (tapez 2)\n");
  scanf("%d",&idjoueur);
  printf("\n");
  while(idjoueur != 1 && idjoueur != 2 ){
    printf("Vous devez choisir un symbole : \n(1) : Rond\n(2) : Croix\n");
    scanf("%d",&idjoueur);
  }
  if (idjoueur == 1) {
    idordi=2;
  }else if(idjoueur == 2) {
    idordi=1;
  }
  while (flag1 == 0 && flag2 ==0) { 
    /* Tour du Joueur */
    printf("C'est votre tour de jouer, voici le plateau : \n");
    afficheClair(plateau);
    printf("\nDans quelle colonne souhaitez vous jouer ?\n");
    scanf("%d",&choix);
    choix = choix -1;
    while (choix> NbrColonne-1 || choix<0) {
      printf("La colonne demandé n'est pas dans le plateau.Choisissez en une autre. \n");
      scanf("%d",&choix);
      choix = choix -1;
    }
    while (plateau[0][choix] != 0) {
      printf("La colonne est déjà pleine. Choissiez en une autre.\n");
      scanf("%d",&choix);
      choix = choix - 1;
    }
    position_y = position(plateau,choix,idjoueur);
    afficheClair(plateau);
    printf("\n");
    /* Le joueur a-t-il gagné ?*/
    flag2 = plateauVictoire(plateau,choix,position_y,idjoueur);
    if(flag2 !=0){
      return 5;
    }
    /* Le tableau est-il plein ? */
    flag1 = plateauPlein(plateau);
    if(flag1 != 0){
      return flag1;
    }
    /* C'est le tour de l'IA */
    printf("C'est au tour de votre adversaire.\n");
    if(diff==1){
      choix=intelligenceArtificielle1(plateau);
      position_y= position(plateau,choix,idordi);
    }else if(diff==2){
      choix=IAsournois(plateau,idjoueur, idordi);
      position_y = position(plateau,choix,idordi);
    }
    /* L'IA a-t-elle gagné ? */
    flag2 = plateauVictoire(plateau,choix,position_y,idordi);
    if(flag2 !=0){
      return 4;
    }
    /* Le tableau est-il plein ? */
    flag1 = plateauPlein(plateau);
    if(flag1 != 0){
      return flag1;
    }	
  } 
  return 0;
}

int OrdicontreOrdi(int plateau[NbrLigne][NbrColonne])
{
  int diff1, diff2, flag1, flag2, idordi, position_x, position_y, timer1, timer2;
  flag1 =0;
  flag2 =0;
  position_x=0;
  position_y=0;
  printf("Choisissez le niveau de difficulté du premier joueur (rond) :\n1. No brain\n2. Facile mais sournois\n");
  scanf("%d",&diff1);
  while(diff1 != 1 && diff1 != 2 && diff1 !=3){
    printf("Vous devez choisir un niveau existant pour le premier joueur (rond): \n(1) : Très Facile\n(2) : Facile\n");
    scanf("%d",&diff1);
  }
  printf("Maintenant choisissez la difficulté de son adversaire (croix) :\n1. No brain\n2. Facile mais sournois\n");
  scanf("%d",&diff2);
  while(diff2 != 1 && diff2 != 2 && diff2 !=3){
    printf("Vous devez choisir un niveau existant pour le second joueur (croix): \n(1) : Très Facile\n(2) : Facile\n");
    scanf("%d",&diff2);
  }
  while (flag1 == 0 && flag2 ==0) { 
    /* Tour de la Première IA */
    printf("C'est le tour du joueur n°1. \n");
    idordi=1;
    if(diff1==1){
      position_x=intelligenceArtificielle1(plateau);
      position_y= position(plateau,position_x,idordi);
    }else if(diff1==2){
      position_x=IAsournois(plateau, idordi+1, idordi);
      position_y = position(plateau,position_x,idordi);
    }
    /* L'IA a-t-elle gagné ? */
    flag2 = plateauVictoire(plateau,position_x,position_y,idordi);
    if(flag2 !=0){
      printf("flag2 : %d\n",flag2);
      return flag2;
    }
    /* Le tableau est-il plein ? */
    flag1 = plateauPlein(plateau);
    if(flag1 != 0){
      printf("flag1 : %d\n",flag1);
      return flag1;
    }	
    afficheClair(plateau);
    printf("\n");
    /* Temps d'attente visuel entre les deux tours */
    timer1 = time(NULL);
    while(timer2 - timer1 !=3){
      timer2 = time(NULL);
    }
    /* Tour du second ordinateur */
    printf("C'est au tour du joueur n°2.\n");
    idordi= 2;
    if(diff2==1){
      position_x=intelligenceArtificielle1(plateau);
      position_y= position(plateau,position_x,idordi);
    }else if(diff2==2){
      position_x=IAsournois(plateau, idordi-1, idordi);
      position_y = position(plateau,position_x,idordi);
    }
    /* L'IA a-t-elle gagné ? */
    flag2 = plateauVictoire(plateau,position_x,position_y,idordi);
    if(flag2 !=0){
      printf("flag2: %d\n",flag2);
      return flag2;
    }
    /* Le tableau est-il plein ? */
    flag1 = plateauPlein(plateau);
    if(flag1 != 0){
      printf("flag1 : %d\n",flag1);
      return flag1;
    }	
    afficheClair(plateau);
    printf("\n");
    timer1 = time(NULL);
    while(timer2 - timer1 !=3){
      timer2 = time(NULL);
    }
  }
  return 0;
} 
    
    
int 
main()
{
  int plateau[NbrLigne][NbrColonne], mode, result1, result2, result3;
  initialiseVide(plateau);
  result1=0;
  result2=0;
  result3=0;

  printf("Dans quel mode souhaitez vous jouer ?\n 1 : Mode Joueur contre Joueur \n 2 : Mode Joueur contre Ordinateur\n 3 : Mode Démo\n");
  scanf("%d",&mode);
  if(mode == 1){
    result1 = joueurContrejoueur(plateau);
  }else if(mode == 2){
    result2 = joueurContreordi(plateau);
  }else if(mode == 3){
    result3 = OrdicontreOrdi(plateau);
  }else {
    /* Si le mode saisie n'est pas 1, 2 ou 3 ? */
    do {
      printf("Entrez un mode conforme aux instructions du menu svp.\n 1 : Mode Joueur contre Joueur \n 2 : Mode Joueur contre Ordinateur\n 3 : Mode Démo\n");
      scanf("%d",&mode);
      if(mode == 1){
	result1=joueurContrejoueur(plateau);
      }else if(mode == 2){
	result2=joueurContreordi(plateau);
      }else if(mode == 3){
	result3=OrdicontreOrdi(plateau);
      }
    }while(mode !=1 && mode !=2 && mode !=3);
  }
  /* Résultats rendu par les fonctions de jeu pour déterminer à qui revient la victoire */
  /* Pour chaque mode le tableau est plein */
  if(result1 ==10 || result2 ==10 || result3 ==10){
    afficheClair(plateau);
    printf("Pas de Victoire, le tableau est plein.\n");
    /* Pour le mode jcj le joueur vainqueur */
  }else if( result1 == 1 || result1 == 2){
    afficheClair(plateau);
    printf("Victoire pour le joueur n°%d.\n",result1);
    /* Pour le mode jco si le joueur a gagné */
  }else if(result2 == 5){
    afficheClair(plateau);
    printf("Vous avez gagné ! Félicitation.\n");
    /* Pour le mode jco si l'IA a gagné */
  }else if(result2 == 4){
    afficheClair(plateau);
    printf("Victoire pour votre adversaire ! \n");
    /* Pour le oco quel est l'ordinateur qui a gagné ? */
  }else if(result3 == 1 || result3 == 2){
    afficheClair(plateau);
    printf("Victoire pour le joueur n°%d \n",result3);
  }
  return 0;
}


