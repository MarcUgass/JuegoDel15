#include <stdio.h>
#include <conio.h>

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "globals.h"

extern "C" {
	// Subrutines en ASM
	void posCurScreen();
	void getMove();
	void moveCursor();
	void moveCursorContinuo();
	void moveTile();
	void playTile();
	void playBlock();


	void printChar_C(char c);
	int clearscreen_C();
	int printMenu_C();
	int gotoxy_C(int row_num, int col_num);
	char getch_C();
	void victory_C();
	int printBoard_C(int tries);
	void continue_C();
}


#define DimMatrix 4


int row = 1;			//fila de la pantalla
char col = 'A';   		//columna actual de la pantalla*/
int rowIni;
char colIni;
int rowEmpty = 3;
char colEmpty = 'B';

char carac, carac2;
int moves = 0;
int victory = 0;

int opc;
int indexMat;
int indexMatIni;
int rowScreen;
int colScreen;
int RowScreenIni;
int ColScreenIni;

//Mostrar un caràcter
//Quan cridem aquesta funció des d'assemblador el paràmetre s'ha de passar a traves de la pila.
void printChar_C(char c) {
	putchar(c);
}

//Esborrar la pantalla
int clearscreen_C() {
	system("CLS");
	return 0;
}

int migotoxy(int x, int y) { //USHORT x,USHORT y) {
	COORD cp = { y,x };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cp);
	return 0;
}

//Situar el cursor en una fila i columna de la pantalla
//Quan cridem aquesta funció des d'assemblador els paràmetres (row_num) i (col_num) s'ha de passar a través de la pila
int gotoxy_C(int row_num, int col_num) {
	migotoxy(row_num, col_num);
	return 0;
}

void victory_C()
{
	int col_central = 39, row_central = 11;
	int row, col;
	for (int i = 0; i < 5; i++)
	{
		//Dibuixar la primera fila
		gotoxy_C(row_central - i, col_central - i);
		printf("+----");
		for (int j = 0; j < i; j++)
		{
			printf("--");
		}
		printf("---+");

		//Dibuixar files superiors buides
		for (int j = 1; j < i; j++)
		{
			gotoxy_C(row_central - j, col_central - i);
			printf("|    ");
			for (int k = 0; k < i; k++)
			{
				printf("  ");
			}
			printf("   |");
		}

		//Dibuixar la línia de VICTORY
		gotoxy_C(row_central, col_central - i);
		printf("|");
		for (int k = 0; k < i; k++)
		{
			printf(" ");
		}
		printf("VICTORY");
		for (int k = 0; k < i; k++)
		{
			printf(" ");
		}
		printf("|");

		//Dibuixar files inferiors buides
		for (int j = 1; j < i; j++)
		{
			gotoxy_C(row_central + j, col_central - i);
			printf("|    ");
			for (int k = 0; k < i; k++)
			{
				printf("  ");
			}
			printf("   |");
		}

		//Dibuixar la última fila
		gotoxy_C(row_central + i, col_central - i);
		printf("+----");
		for (int j = 0; j < i; j++)
		{
			printf("--");
		}
		printf("---+");

		Sleep(150);
	}
}


//Imprimir el menú del joc
int printMenu_C() {

	clearscreen_C();
	gotoxy_C(1, 1);
	printf("______________________________________________________________________________\n");
	printf("|                                                                             |\n");
	printf("|                                 MENU PUZZLE                                 |\n");
	printf("|_____________________________________________________________________________|\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                               1. Show cursor                                |\n");
	printf("|                               2. Move cursor                                |\n");
	printf("|                               3. Move cursor continuos                      |\n");
	printf("|                               4. Move a tile                                |\n");
	printf("|                               5. Play (tile movement)                       |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                                                                             |\n");
	printf("|                               0. Exit                                       |\n");
	printf("|                                                                             |\n");
	printf("|_____________________________________________________________________________|\n");
	printf("|                                                                             |\n");
	printf("|                               OPTION:                                       |\n");
	printf("|_____________________________________________________________________________|\n");
	return 0;
}


//Llegir una tecla sense espera i sense mostrar-la per pantalla
char getch_C() {
	DWORD mode, old_mode, cc;
	HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
	if (h == NULL) {
		return 0; // console not found
	}
	GetConsoleMode(h, &old_mode);
	mode = old_mode;
	SetConsoleMode(h, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));
	TCHAR c = 0;
	ReadConsole(h, &c, 1, &cc, NULL);
	SetConsoleMode(h, old_mode);

	return c;
}


/**
 * Mostrar el tauler de joc a la pantalla. Les li­nies del tauler.
 *
 * Aquesta funcio es crida des de C i des d'assemblador,
 * i no hi ha definida una subrutina d'assemblador equivalent.
 * No hi ha pas de parametres.
 */
void printBoard_C() {
	int i, j, r = 1, c = 25;

	clearscreen_C();
	gotoxy_C(r++, 25);
	printf("===================================");
	gotoxy_C(r++, c); 	      //Ti­tol
	printf("              PUZZLE      moves: %02d", moves);
	gotoxy_C(r++, c);
	gotoxy_C(r++, 25);
	printf("===================================");
	gotoxy_C(r++, c); 	      
	printf("                                   ");
	gotoxy_C(r++, c); 	      //Coordenades
	printf("            A   B   C   D           ");
	for (i = 0; i < DimMatrix; i++) {
		gotoxy_C(r++, c);
		printf("\t   +"); 	      // "+" cantonada inicial
		for (j = 0; j < DimMatrix; j++) {
			printf("---+");   //segment horitzontal	
		}
		gotoxy_C(r++, c);
		printf("\t%d  |", i + 1);     //Coordenades
		for (j = 0; j < DimMatrix; j++) {
			printf(" %c |", puzzle[i][j]);
		}
	}
	gotoxy_C(r++, c);
	printf("\t   +");
	for (j = 0; j < DimMatrix; j++) {
		printf("---+");
	}
}

int main(void) {
	opc = 1;
	RowScreenIni = 8;
	ColScreenIni = 37;
	moves = 0;

	while (opc != '0') {
		printMenu_C();					//Mostrar menú
		gotoxy_C(18, 40);				//Situar el cursor
		opc = getch_C();				//Llegir una opció
		row = 3;
		col = 'B';
		switch (opc) {
		case '1':					//Show cursor
			clearscreen_C();  		//Esborra la pantalla
			printBoard_C();			//Mostrar el tauler

			gotoxy_C(17, 37);		//Situar el cursor a sota del tauler
			printf("Press any key ");
			
			posCurScreen();		//Posicionar el cursor a pantalla.

			getch_C();				//Esperar que es premi una tecla
			break;

		case '2':                //Move Cursor
			clearscreen_C();  	 //Esborra la pantalla
			printBoard_C();   	 //Mostrar el tauler.

			posCurScreen();		//Posicionar el cursor a pantalla.
			getMove();			//Llegir tecla i comprovar que sigui vàlida
			moveCursor();		//Moure el cursor

			gotoxy_C(17, 37);		//Situar el cursor a sota del tauler
			printf("Press any key ");

			posCurScreen();		//Posicionar el cursor a pantalla.

			getch_C();
			break;

		case '3':				//Move Cursor Continuos
			clearscreen_C();  	 //Esborra la pantalla
			printBoard_C();   	 //Mostrar el tauler.

			posCurScreen();			//Posicionar el cursor a pantalla.
			moveCursorContinuo();	//Moure el cursor múltiples cops fins pulsar 's' o 'm'.

			gotoxy_C(17, 37);		//Situar el cursor a sota del tauler
			printf("Press any key ");

			getch_C();
			break;
		
		case '4':				//Move Tile
			clearscreen_C();  	 //Esborra la pantalla
			printBoard_C();   	 //Mostrar el tauler.

			posCurScreen();			//Posicionar el cursor a pantalla.
			moveCursorContinuo();	//Moure el cursor múltiples cops fins pulsar 's' o 'm'.
			if (carac2 == 'm')
			{
				moveTile();
			}

			gotoxy_C(17, 37);		//Situar el cursor a sota del tauler
			printf("Press any key ");

			posCurScreen();

			getch_C();
			break;
		case '5':				//Play Tile
			clearscreen_C();  	 //Esborra la pantalla
			printBoard_C();   	 //Mostrar el tauler.

			posCurScreen();			//Posicionar el cursor a pantalla.
			playTile();

			gotoxy_C(17, 37);		//Situar el cursor a sota del tauler
			printf("Press any key ");

			getch_C();
			break;
		}
	}
	gotoxy_C(19, 1);						//Situar el cursor a la fila 19
	return 0;
}