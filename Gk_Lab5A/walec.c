//////////////////////////////////////////////////////////////////////////////////////////
// Program demonstruje wplyw zmiany parametrow zrodla swiatla i wlasnosci materialu
// na umieszczony w scenie obiekt (walec).Uzytkownik interakcyjnie za pomoca
// klawiatury moze wprowadzac nastepujace zmiany (w nawiasach <> podano nazwy klawiszy):
//   <p>/<P>    - zwiekszenie/zmniejszenie o 1 promienia walca w ustalonym zakresie,
//   <w>/<W>    - zwiekszenie/zmniejszenie o 1 wysokosci walca w ustalonym zakresie,
//   <h>/<H>    - zwiekszenie/zmniejszenie o 1 liczby podzialow poziomych 
//                w ustalonym zakresie,
//   <v>/<V>    - zwiekszenie/zmniejszenie o 1 liczby podzialow pionowych 
//                w ustalonym zakresie,
//   <m>        - wybor aktywnego menu (menu swiatla lub menu materialu)
//   <KEY_DOWN> - przejscie do nastepnej pozycji w aktywnym menu,
//   <Key_UP>   - przejscie do poprzedniej pozycji w aktywnym menu,
//   <1>/<!>    - zmiana pierwszej skladowej wybranej pozycji menu,
//   <2>/<@>    - zmiana drugiej skladowej wybranej pozycji menu (o ile istnieje),  
//   <3>/<#>    - zmiana trzeciej skladowej wybranej pozycji menu (o ile istnieje),  
//   <4>/<$>    - zmiana czwartej skladowej wybranej pozycji menu (o ile istnieje),  
//   <r>        - ustawienie domyslnych wartosci wszystkich parametrow,
//   <ESC>      - koniec programu.
//
//   Autor: Wojciech Satala
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <GL/glut.h>
#include "materials.h"

// Definicja stalych
#define LPOZ_MAX            100
#define LPOZ_MIN            3
#define LPION_MAX           100
#define LPION_MIN           3
#define R_MAX               10.0
#define WYS_MAX             10.0
#define KROK_FLOAT          0.1
#define X_OFFSET_SWIATLO    10
#define Y_OFFSET_SWIATLO    120
#define X_OFFSET_MAT        10
#define Y_OFFSET_MAT        220
#define X_OFFSET_OBIEKT     10
#define Y_OFFSET_OBIEKT     300
#define ID_MENU_SWIATLA     0
#define ID_MENU_MATERIALU   1
#define LPOZ_MENU_SWIATLA   10
#define LPOZ_MENU_MATERIALU 5
#define M_PI				3.14159

// Makro przeliczajace stopnie na radiany
#define DEG2RAD(x)       ((float)(x)*M_PI/180.0)

// Zmienne globalne
double  kat = 0.0;         // Kat obrotu obserwatora wokol osi OY [stopnie]
int     lPionowych;        // Liczba podzialow pionowych 
int     lPoziomych;        // Liczba podzialow poziomych
double  promien;           // Promien walca
double  wysokosc;          // Wysokosc walca
int     sIndeks;           // Wybrana pozycja w tablicy parametrow swiatla
int     mIndeks;           // Wybrana pozycja w tablicy parametrow materialu
int     menu;              // Identyfikator wybranego menu (menu swiatla lub menu materialu) 
int     szerokoscOkna = 800;
int     wysokoscOkna  = 600;

// Zerowy parametr Emission
const GLfloat Emission[4] = {0.0 , 0.0, 0.0, 0.0};

// Tablica parametrow swiatla
GLfloat swiatlo[10][4];

// Tablica parametrow materialu z jakiego wykonany jest walec
GLfloat material[6][4];

// Prototypy funkcji
void UstawDomyslneWartosciParametrow(void);
void RysujTekstRastrowy(void *font, char *tekst);
void UstawKolorPozycji(int m, int indeks);
void WlaczOswietlenie(void);
void DefiniujMaterial(void);
void UstawParametryOswietlenia(int indeks, char operacja);
void UstawParametryMaterialu(int indeks, char operacja);
void RysujWalec(double h, double r, int nv, int nh);
void UstawParametryWidoku(int szer, int wys);
void RysujNakladke(void);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);
void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y);

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca domyslne parametry walca, materialu i zrodla swiatla
void UstawDomyslneWartosciParametrow(void)
{
  // Tablica parametrow materialu
  GLfloat param_materialu[6][4] = {
    {0.2, 0.2, 0.2, 1.0},  // [0] wspolczynnik odbicia swiatla otoczenia
    {0.8, 0.8, 0.8, 1.0},  // [1] wspolczynnik odbicia swiatla rozproszonego
    {0.0, 0.0, 0.0, 1.0},  // [2] wspolczynnik odbicia swiatla lustrzanego
    {0.0, 0.0, 0.0, 0.0},  // [3] polysk 
    {0.0, 0.0, 0.0, 1.0}}; // [4] kolor swiatla emitowanego  
	
  // Tablica parametrow swiatla
  GLfloat param_swiatla[10][4] = {
    {0.0, 0.0, 0.0, 1.0},  // [0] otoczenie
    {1.0, 1.0, 1.0, 1.0},  // [1] rozproszenie
    {1.0, 1.0, 1.0, 1.0},  // [2] lustrzane
    {0.0, 0.0, 1.0, 1.0},  // [3] polozenie
    {0.0, 0.0, -1.0, 0.0}, // [4] kierunek swiecenia
    {0.0, 0.0, 0.0, 0.0},  // [5] tlumienie katowe swiatla
    {180.0, 0.0, 0.0, 0.0},// [6] kat odciecia swiatla
    {1.0, 0.0, 0.0, 0.0},  // [7] stale tlumienie
    {0.0, 0.0, 0.0, 0.0},  // [8] tlumienie liniowe
    {0.0, 0.0, 0.0, 0.0}}; // [9] tlumienie kwadratowe
    
  // Skopiowanie zawartosci tablic param_* do tablic globalnych
  memcpy(swiatlo, param_swiatla, LPOZ_MENU_SWIATLA*4*sizeof(GLfloat));
  memcpy(material, param_materialu, LPOZ_MENU_MATERIALU*4*sizeof(GLfloat));
    
  // Skopiowanie W³aœciwoœci materia³u - z³oto
  memcpy(material[0], PolishedGoldAmbient, 4*sizeof(GLfloat));
  memcpy(material[1], PolishedGoldDiffuse, 4*sizeof(GLfloat));
  memcpy(material[2], PolishedGoldSpecular, 4*sizeof(GLfloat));
  memcpy(material[3], &PolishedGoldShininess, 4*sizeof(GLfloat));
  memcpy(material[4], Emission, 4*sizeof(GLfloat));
  
  // Parametry walca
  lPionowych = 20;         // Liczba podzialow pionowych 
  lPoziomych = 20;         // Liczba podzialow poziomych
  promien    = 3.0;        // Promien walca
  wysokosc   = 3.0;        // Wysokosc walca  

  // Pozostale parametry
  sIndeks = 0;        // Wybrana pozycja w tablicy parametrow swiatla
  mIndeks = 0;        // Wybrana pozycja w tablicy parametrow materialu
  menu    = ID_MENU_SWIATLA;

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca tekst rastrowy <tekst> za pomca fontow bitmapowych <font>.Rysowanie
// tekstu wykonywane jest poczawszy od biezacej pozycji ekranu.
void RysujTekstRastrowy(void *font, char *tekst)
{
  int i;

  for (i = 0; i < (int)strlen(tekst); i++)
    glutBitmapCharacter(font, tekst[i]);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawia kolor dla pozycji <indeks> w  menu <m>. Aktualnie wybrana pozycja 
// jest rysowana w kolorze zoltym.
void UstawKolorPozycji(int m, int indeks)
{
  if (m == menu) 
    if((m == ID_MENU_SWIATLA) && (indeks == sIndeks)
       || (m == ID_MENU_MATERIALU) && (indeks == mIndeks))

    // Pozycja podswietlona wyswietlana jest w kolkorze zoltym
    glColor3f(1.0, 1.0, 0.0);
  else

    // Pozostale na bialo
    glColor3f(1.0, 1.0, 1.0);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Wlaczenie  oswietlenia sceny
void WlaczOswietlenie(void)
{
  // Odblokowanie oswietlenia
  glEnable(GL_LIGHTING);

  // Odblokowanie zerowego zrodla swiatla
  glEnable(GL_LIGHT0);

  // Inicjowanie zrodla swiatla
  glLightfv(GL_LIGHT0, GL_AMBIENT, swiatlo[0]);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, swiatlo[1]);
  glLightfv(GL_LIGHT0, GL_SPECULAR, swiatlo[2]);
  glLightfv(GL_LIGHT0, GL_POSITION, swiatlo[3]);  
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, swiatlo[4]);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, swiatlo[5][0]);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, swiatlo[6][0]);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, swiatlo[7][0]);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, swiatlo[8][0]);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, swiatlo[9][0]);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Zdefiniowanie walasciwosci materialu walca na podstawie zapisanych w tablcy 'material'
// parametrow (material obowiazuje tylko do scian skierowanych przodem do obserwatora)
void DefiniujMaterial(void)
{
  glMaterialfv(GL_FRONT, GL_AMBIENT, material[0]);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, material[1]);
  glMaterialfv(GL_FRONT, GL_SPECULAR, material[2]);
  glMaterialfv(GL_FRONT, GL_SHININESS, material[3]);
  glMaterialfv(GL_FRONT, GL_EMISSION, material[4]);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Ustawienie parametrow oswietlenia sceny
void UstawParametryOswietlenia(int indeks, char operacja)
{
  switch(indeks)
    {
      // Parametry swiatla otoczenia, rozproszenia i odbicia lustrzanego
    case 0:
    case 1:
    case 2:
      switch(operacja)
	{
	  // Zwiekszenie skladowej R o wartosc KROK_FLOAT
	case '1':
	  swiatlo[indeks][0] = (swiatlo[indeks][0] >= 1.0) ? 1.0 : swiatlo[indeks][0]+ KROK_FLOAT;
	  break;

	  // Zmniejszenie skladowej R o wartosc KROK_FLOAT
	case '!':
	  swiatlo[indeks][0] = (swiatlo[indeks][0] <= 0.1) ? 0.0 : swiatlo[indeks][0]- KROK_FLOAT;
	  break;
	  
	  // Zwiekszenie skladowej G o wartosc KROK_FLOAT
	case '2':
	  swiatlo[indeks][1] = (swiatlo[indeks][1] >= 1.0) ? 1.0 : swiatlo[indeks][1]+ KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie skladowej G o wartosc KROK_FLOAT
	case '@':
	  swiatlo[indeks][1] = (swiatlo[indeks][1] <= 0.01) ? 0.0 : swiatlo[indeks][1]- KROK_FLOAT;
	  break;
	  
	  // Zwiekszenie skladowej B o wartosc KROK_FLOAT
	case '3':
       	  swiatlo[indeks][2] = (swiatlo[indeks][2] >= 1.0) ? 1.0 : swiatlo[indeks][2]+ KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie skladowej B o wartosc KROK_FLOAT
	case '#':
	  swiatlo[indeks][2] = (swiatlo[indeks][2] <= 0.1) ? 0.0 : swiatlo[indeks][2]- KROK_FLOAT;
	  break;

	  // Zwiekszenie skladowej A o wartosc KROK_FLOAT
	case '4':
       	  swiatlo[indeks][3] = (swiatlo[indeks][3] >= 1.0) ? 1.0 : swiatlo[indeks][3]+ KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie skladowej B o wartosc KROK_FLOAT
	case '$':
	  swiatlo[indeks][3] = (swiatlo[indeks][3] <= 0.1) ? 0.0 : swiatlo[indeks][3]- KROK_FLOAT;
	  break;
	}
      break;

      // Parametry opisujace polozenie i orientacje swiatla
    case 3:
    case 4:
    case 5:
    case 6:
      switch(operacja)
	{
	  // Zwiekszenie wspolrzednej X o wartosc KROK_FLOAT
	case '1':
	  swiatlo[indeks][0] += KROK_FLOAT;
	  break;

     	  // Zmniejszenie wspolrzednej X o wartosc KROK_FLOAT
	case '!':
	  swiatlo[indeks][0] -= KROK_FLOAT;
	  break;
	  
	  // Zwiekszenie wspolrzednej Y o  wartosc KROK_FLOAT
	case '2':
	  swiatlo[indeks][1] += KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie wspolrzednej Y o  KROK_FLOAT    
	case '@':
	  swiatlo[indeks][1] -= KROK_FLOAT;
	  break;
	  
	  // Zwiekszenie wspolrzednej Z o wartosc KROK_FLOAT
	case '3':
	  swiatlo[indeks][2] += KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie wspolrzednej Z o wartosc KROK_FLOAT
	case '#':
	  swiatlo[indeks][2] -= KROK_FLOAT;
	  break;

	  // Zwiekszenie wspolrzednej W o wartosc KROK_FLOAT
	case '4':
	  swiatlo[indeks][3] += KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie wspolrzednej W o wartosc KROK_FLOAT
	case '$':
	  swiatlo[indeks][3] -= KROK_FLOAT;
	  break;
	}
	break;
    case 7:
    case 8:
    case 9:
      switch(operacja)
	{
	  // Zwiekszenie skladowych tlumienia wartosc KROK_FLOAT/10
	case '1':
	  swiatlo[indeks][0] += KROK_FLOAT/10.0;
	  break;

     	  // Zmniejszenie stalych tlumienia o wartosc KROK_FLOAT/10
	case '!':
	  swiatlo[indeks][0] -= KROK_FLOAT/10.0;
	  break;
	}
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Ustawienie parametrow materoalu
void UstawParametryMaterialu(int indeks, char operacja)
{
  switch(indeks)
    {
      // Parametry odbicia swiatla swiatla otoczenia, swiatla rozproszonego, swiatla 
      // kierunkowego i emitowanego
    case 0:
    case 1:
    case 2:
    case 4:
      switch(operacja)
	{
	  // Zwiekszenie skladowej R o wartosc KROK_FLOAT
	case '1':
	  material[indeks][0] = (material[indeks][0] >= 1.0) ? 1.0 : material[indeks][0]+ KROK_FLOAT;
	  break;

	  // Zmniejszenie skladowej R o wartosc KROK_FLOAT
	case '!':
	  material[indeks][0] = (material[indeks][0] <= 0.1) ? 0.0 : material[indeks][0]- KROK_FLOAT;
	  break;
	  
	  // Zwiekszenie skladowej G o wartosc KROK_FLOAT
	case '2':
	  material[indeks][1] = (material[indeks][1] >= 1.0) ? 1.0 : material[indeks][1]+ KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie skladowej G o wartosc KROK_FLOAT
	case '@':
	  material[indeks][1] = (material[indeks][1] <= 0.1) ? 0.0 : material[indeks][1]- KROK_FLOAT;
	  break;
	  
	  // Zwiekszenie skladowej B o wartosc KROK_FLOAT
	case '3':
	  material[indeks][2] = (material[indeks][2] >= 1.0) ? 1.0 : material[indeks][2]+ KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie skladowej B o wartosc KROK_FLOAT
	case '#':
	  material[indeks][2] = (material[indeks][2] <= 0.1) ? 0.0 : material[indeks][2]- KROK_FLOAT;
	  break;

	  // Zwiekszenie skladowej A o wartosc KROK_FLOAT
	case '4':
	  material[indeks][3] = (material[indeks][3] >= 1.0) ? 1.0 : material[indeks][3]+ KROK_FLOAT;
	  break;
	  
	  // Zmniejszenie skladowej A o wartosc KROK_FLOAT
	case '$':
	  material[indeks][3] = (material[indeks][3] <= 0.1) ? 0.0 : material[indeks][3]- KROK_FLOAT;
	  break;
	}
      break;

      // Parametr opisujacy polysk powierzchni
    case 3:
      switch(operacja)
	{
	  // Zwiekszenie skladowej R o wartosc KROK_FLOAT*10
	case '1':
	  material[indeks][0] = (material[indeks][0] >= 100.0) ? 100.0 : material[indeks][0]+ KROK_FLOAT * 10.0;
	  break;
	  
	  // Zmniejszenie skladowej R o wartosc KROK_FLOAT*10
	case '!':
	  material[indeks][0] = (material[indeks][0] <= 0.0) ? 0.0 : material[indeks][0]- KROK_FLOAT * 10.0;
	  break;
	}
      break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca walec o zadanej wysokosci, promieniu oraz liczbie podzialow
// pionowych i poziomych. Tryb modelowania powierzchni bocznej  - GL_TRIANGLE_STRIP,
// podstaw - GL_TRIANGLE_FAN.
void RysujWalec(double h, double r, int nv, int nh)
{
  double dH, dAlfa;
  int   i, j;


  // Wyznaczenie kata wyznaczajacego pojedynczy wycinek pionowy
  dAlfa = 360.0L/(double)nh;

  // Wyznaczenie wysokosci pojedynczego wycinka poziomego
  dH = h/(double)nv;


  // Wyznaczanie wierzcholkow i wektorow normalnych powierzchni bocznych
  for (i = 0; floor((i+1)*dH*1.0E10) <= floor(h*1.0E10); i++)
    {
      glBegin(GL_TRIANGLE_STRIP); 
      glNormal3f(0.0, 0.0, 1.0);
      glVertex3f(0.0, (i + 1)*dH, r);
      glVertex3f(0.0, i*dH, r);

      for (j = 1; j*dAlfa <= 360.0L + dAlfa; j++)
	{
	  glNormal3f(sin(DEG2RAD(j*dAlfa)), 0.0, cos(DEG2RAD(j*dAlfa)));
	  glVertex3f(r*sin(DEG2RAD(j*dAlfa)), (i + 1)*dH, r*cos(DEG2RAD(j*dAlfa)));
	  glVertex3f(r*sin(DEG2RAD(j*dAlfa)), i*dH, r*cos(DEG2RAD(j*dAlfa)));
	}
      glEnd();
    }

  
  // Wyznaczenie wierzcholkow i wektorow normalnych dolnej podstawy 
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0, -1.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  for (i = 0; i * dAlfa <= 360.0L + dAlfa; i++)
    glVertex3f(r*sin(DEG2RAD(i*dAlfa)), 0.0, r*cos(DEG2RAD(i*dAlfa)));
  glEnd();  

  // Wyznaczenie wierzcholkow i wektorow normalnych gornej podstawy 
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, h, 0.0);
  for (i = 0; i * dAlfa <= 360.0L + dAlfa; i++)
    glVertex3f(r*sin(DEG2RAD(i*dAlfa)), h, r*cos(DEG2RAD(i*dAlfa)));
  glEnd();  
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu
void UstawParametryWidoku(int szer, int wys)
{
  // Zapamietanie wielkosci widoku
  szerokoscOkna = szer;
  wysokoscOkna  = wys;

  // Ustawienie parametrow viewportu
  glViewport(0, 0, szerokoscOkna, wysokoscOkna);

  // Przejscie w tryb modyfikacji macierzy rzutowania
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (float)szerokoscOkna/(float)wysokoscOkna, 1.0, 1000.0);

  // Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
  glMatrixMode(GL_MODELVIEW);

  // Zmiana macierzy znajdujacej sie na wierzcholku stosu na macierz jednostkowa 
  glLoadIdentity();
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca na wygenerowanym obrazie walca nkladke z tekstem opisujacym
// aktualne parametry zrodla swiatla i materialu.
void RysujNakladke(void)
{
 char buf[255];

  // Zmiana typu rzutu z perspektywicznego na ortogonalny
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, szerokoscOkna,  0.0, wysokoscOkna,-100.0, 100.0); 
 
  // Modelowanie sceny 2D (zawartosci nakladki)
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Zablokowanie oswietlenia (mialoby ono wplyw na kolor tekstu)
  glDisable(GL_LIGHTING);

  // Okreslenie koloru tekstu
  glColor3f(1.0, 1.0, 1.0);

 // RYSOWANIE MENU PARAMETROW ZRODLA SWIATLA

  sprintf(buf, "Swiatlo");
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,0);
  sprintf(buf, " - otaczajace  (%.1f, %.1f, %.1f, %.1f)", swiatlo[0][0], swiatlo[0][1], swiatlo[0][2],
	  swiatlo[0][3]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 10);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,1);
  sprintf(buf, " - rozproszone (%.1f, %.1f, %.1f, %.1f)", swiatlo[1][0], swiatlo[1][1], swiatlo[1][2],
	  swiatlo[1][3]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 20);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,2);
  sprintf(buf, " - lustrzane   (%.1f, %.1f, %.1f, %.1f)", swiatlo[2][0], swiatlo[2][1], swiatlo[2][2],
	  swiatlo[2][3]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 30);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,3);
  sprintf(buf, " - polozenie   (%.1f, %.1f, %.1f, %.1f)", swiatlo[3][0], swiatlo[3][1], swiatlo[3][2],
	  swiatlo[3][3]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 40);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,4);
  sprintf(buf, " - kierunek    (%.1f, %.1f, %.1f)", swiatlo[4][0], swiatlo[4][1], swiatlo[4][2]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 50);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,5);
  sprintf(buf, " - tl. katowe  (%.1f)", swiatlo[5][0]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 60);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,6);
  sprintf(buf, " - kat odciecia(%.1f)", swiatlo[6][0]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 70);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,7);
  sprintf(buf, " - tl. stale   (%.2f)", swiatlo[7][0]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 80);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,8);
  sprintf(buf, " - tl. liniowe (%.2f)", swiatlo[8][0]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 90);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_SWIATLA,9);
  sprintf(buf, " - tl. kwadrat.(%.2f)", swiatlo[9][0]);
  glRasterPos2i(X_OFFSET_SWIATLO, Y_OFFSET_SWIATLO - 100);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);


 // RYSOWANIE MENU  PARAMETROW MATERIALU

  glColor3f(1.0, 1.0, 1.0);
  sprintf(buf, "Material");
  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_MATERIALU,0);
  sprintf(buf, " - otaczajace  (%.6f, %.6f, %.6f, %.6f)", material[0][0], material[0][1], 
	  material[0][2], material[0][3]);
  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 10);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_MATERIALU,1);
  sprintf(buf, " - rozproszone (%.6f, %.6f, %.6f, %.6f)", material[1][0], material[1][1], 
	  material[1][2], material[1][3]);
  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 20);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_MATERIALU,2);
  sprintf(buf, " - kierunkowe  (%.6f, %.6f, %.6f, %.6f)", material[2][0], material[2][1], 
	  material[2][2], material[2][3]);
  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 30);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_MATERIALU,3);
  sprintf(buf, " - polysk      (%.2f)", material[3][0]);
  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 40);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  UstawKolorPozycji(ID_MENU_MATERIALU,4);
  sprintf(buf, " - emitowane   (%.6f, %.6f, %.6f, %.6f)", material[4][0], material[4][1], 
	  material[4][2], material[4][3]);
  glRasterPos2i(X_OFFSET_MAT, Y_OFFSET_MAT - 50);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

 // RYSOWANIE MENU PARAMETROW WALCA

  glColor3f(1.0, 1.0, 1.0);

  sprintf(buf, "Walec:");
  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, " - promien (%.1f)", promien);
  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 10);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, " - wysokosc (%.1f)", wysokosc);
  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 20);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, " - podzialow pionowych (%d)", lPionowych);
  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 30);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, " - podzialow poziomych (%d)", lPoziomych);
  glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 40);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  // Przywrocenie macierzy sprzed wywolania funkcji
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();  

  // Odblokowanie oswietlenia
  glEnable(GL_LIGHTING);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja generujaca pojedyncza klatke animacji
void WyswietlObraz(void)
{
  // Wyczyszczenie bufora ramki i bufora glebokosci
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Okreslenie wielkosci widoku, wlaczenie rzutowania perspektywicznego
  // i zainicjowanie stosu macierzy modelowania
  UstawParametryWidoku(szerokoscOkna, wysokoscOkna);

  // Ustawienie oswietlenia sceny (polozenie zrodla swiatla wyznaczane jest w ukladzie
  // wspolrzednych obserwatora)
  WlaczOswietlenie();

  // Wyznaczenie polozenia obserwatora (przeksztalcenie uladu wspolrzednych 
  // sceny do ukladu wspolrzednych obserwatora). Obserwator obraca sie wokol punktu 0.0
  // na osi OY. Promien obrotu = 20, pochylenie = 20 stopni,
  // predkosc obrotu = 0.25 stopnia/ramke.
  glTranslatef(0, 0, -20);
  glRotatef(20, 1, 0, 0);
  glRotatef(kat, 0, 1, 0);

  // Zdefiniowanie materialu walca
  DefiniujMaterial();

  // Generacja obrazu walca
  RysujWalec(wysokosc, promien, lPoziomych, lPionowych);

  // Narysowanie tekstow z opisem parametrow oswietlenia i materialu
  RysujNakladke();

  // Przelaczenie buforow ramki
  glutSwapBuffers();


  // Modyfikacja kata obrotu
  kat += 0.25; 
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
  switch(klawisz)
    {
    case 'v':
      lPionowych = (lPionowych == LPION_MAX)? LPION_MAX : lPionowych + 1;
      break;

    case 'V':
      lPionowych = (lPionowych == LPION_MIN)? LPION_MIN : lPionowych - 1;
      break;

    case 'h':
      lPoziomych = (lPoziomych == LPOZ_MAX)? LPOZ_MAX : lPoziomych + 1;
      break;

    case 'H':
      lPoziomych = (lPoziomych == LPOZ_MIN)? LPOZ_MIN : lPoziomych - 1;
      break;

    case 'w':
      wysokosc = (wysokosc == WYS_MAX) ? WYS_MAX : wysokosc + 1;
      break;

    case 'W':
      wysokosc = (wysokosc == 1) ? wysokosc : wysokosc - 1;
      break;

    case 'p':
      promien = (promien == R_MAX) ? R_MAX : promien + 1;   
      break;

    case 'P':
      promien = (promien == 1) ? promien : promien - 1;
      break;
      
    case 'r':
      UstawDomyslneWartosciParametrow();
      break;

    case 'm':
      menu = (menu == ID_MENU_SWIATLA) ? ID_MENU_MATERIALU : ID_MENU_SWIATLA;
      break;
      
    case '1':
    case '!':
    case '2':
    case '@':
    case '3':
    case '#':
    case '4':
    case '$':
      if (menu == ID_MENU_SWIATLA)
	UstawParametryOswietlenia(sIndeks, klawisz);
      else
	UstawParametryMaterialu(mIndeks, klawisz);
      break;

      // Wcisniecie klawisza ESC powoduje wyjscie z programu
    case 27:
      exit(0);      
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja klawiszy specjalnych
void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y)
{
  switch(klawisz)
    {
    case GLUT_KEY_UP:
      if (menu == ID_MENU_SWIATLA)
	sIndeks = (sIndeks > 0) ? sIndeks - 1 : 0;
      else
	mIndeks = (mIndeks > 0) ? mIndeks - 1 : 0;
      break;

    case GLUT_KEY_DOWN:
      if (menu == ID_MENU_SWIATLA)
	sIndeks = (sIndeks < LPOZ_MENU_SWIATLA - 1) ? sIndeks + 1 : LPOZ_MENU_SWIATLA - 1;
      else
	mIndeks = (mIndeks < LPOZ_MENU_MATERIALU - 1) ? mIndeks + 1 : LPOZ_MENU_MATERIALU - 1;
      break;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char **argv)
{
  // Zainicjowanie biblioteki GLUT
  glutInit(&argc, argv);

  // Ustawienie trybu wyswietlania
  glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);

  // Ustawienie polozenia dolenego lewego rogu okna
  glutInitWindowPosition(100, 100);

  // Ustawienie rozmiarow okna
  glutInitWindowSize(szerokoscOkna, wysokoscOkna);

  // Utworzenie okna
  glutCreateWindow("Oswietlony walec");

  // Odblokowanie bufora glebokosci
  glEnable(GL_DEPTH_TEST);

  // Ustawienie funkcji wykonywanej na danych w buforze glebokosci
  glDepthFunc(GL_LEQUAL);

  // Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
  glClearDepth(1000.0);

  // Odblokowanie wykonywania operacji na skladowych "alfa"
  glEnable(GL_BLEND);

  // Wybor funkcji mieszajacej kolory
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Ustawienie koloru czyszczenia bufora ramki
  glClearColor (0.0, 0.0, 0.0, 1.0);
  
  // Zarejestrowanie funkcji (callback) wyswietlajacej
  glutDisplayFunc(WyswietlObraz);

  // Zarejestrowanie funkcji (callback) wywolywanej za kazdym razem kiedy
  // zmieniane sa rozmiary okna
  glutReshapeFunc(UstawParametryWidoku);

  // Zarejestrowanie funkcji wykonywanej gdy okno nie obsluguje
  // zadnych zadan
  glutIdleFunc(WyswietlObraz);

  // Zarejestrowanie funkcji obslugi klawiatury
  glutKeyboardFunc(ObslugaKlawiatury);

  // Zarejestrowanie funkcji obslugi klawiszy specjalnych
  glutSpecialFunc(ObslugaKlawiszySpecjalnych);

  // Ustawienie domyslnych wartosci parametrow
  UstawDomyslneWartosciParametrow();

  // Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
  // w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
  glutMainLoop();

  return 0;
}

