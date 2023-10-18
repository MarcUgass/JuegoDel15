
extern "C" char carac;

// Matriu 4x4 per inicialitzar el joc
extern "C" char puzzle[4][4] = { {'A', 'B', 'C', 'D'},
								 {'E', 'J', 'F', 'H'}, 
							     {'I', ' ', 'G', 'K'},
								 {'M', 'N', 'O', 'L'}};

extern "C" char carac2;     //caràcter llegit de teclat i per a escriure a pantalla.
extern "C" int row;			//fila per a accedir a la matriu puzzle [1..4]
extern "C" char col;		//columna per a accedir a la matriu puzzle [A..D]
extern "C" int indexMat; 	//índex per a accedir a la matriu puzzle (index=row*4+col [0..15].
extern "C" int rowEmpty;	//fila on es troba el forat.
extern "C" char colEmpty;	//columna on es troba el forat.
extern "C" int moves;
extern "C" int indexMatIni;
extern "C" int victory;

extern "C" int rowScreen;	   //fila on volem posicionar el cursor a la pantalla.
extern "C" int colScreen;	   //columna on volem posicionar el cursor a la pantalla.

extern "C" int RowScreenIni;
extern "C" int ColScreenIni;

extern "C" int rowIni;
extern "C" char colIni;

extern "C" int opc;

