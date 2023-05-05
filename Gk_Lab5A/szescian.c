#include <math.h>
#include <string.h>
#include <stdio.h>
#include <gl/glut.h>


// Definicja stalych
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
#define M_PI         3.1415926535
#define WYSOKOSC_STOZEK         3
#define PROMIEN_STOZEK          1


// Makro przeliczajace stopnie na radiany
#define DEG2RAD(x)     ((long double)(x)*M_PI/180.0)


int     sIndeks;           // Wybrana pozycja w tablicy parametrow swiatla
int     mIndeks;           // Wybrana pozycja w tablicy parametrow materialu
int     menu;              // Identyfikator wybranego menu (menu swiatla lub menu materialu) 
int     malowanie = 0;
int     cieniowanie = 0;
int     normalne = 0;
int     wlacznik1 = 0;
int     wlacznik2 = 0;
int     szerokoscOkna = 800;
int     wysokoscOkna = 600;
int		l_podz = 4;

GLfloat kat_orbityX = 0;
GLfloat kat1 = 0;
GLfloat promien_reflektora = 20;
GLfloat predkosc_orbity = 0.25;
GLUquadricObj* Okrag;

GLint kolor = 0;
GLdouble i = 0.0;
GLdouble j = 0.0;
GLdouble M = 12.0;
GLdouble ilosc_podzialow = 64.0;
GLfloat rotObsY = 315.0;
GLfloat rotObsX = 20.0;
GLfloat rotObsZ = 0.0;
GLfloat odleglosc = -20.0;
GLdouble szybkosc_swiatla = 0.0;
GLdouble nachylenie_swiatla = 0.0;
GLdouble promien_swiatla = 10.0;
GLdouble kat_swiatla = 7.0;


// Tablica parametrow swiatla
GLfloat swiatlo[10][4];
GLfloat swiatlo2[10][4];

// Tablica parametrow materialu z jakiego wykonany jest walec
GLfloat material[6][4];
GLfloat material2[6][4];
GLfloat material3[6][4];


// Prototypy funkcji
void UstawDomyslneWartosciParametrow(void);
void UstawKolorPozycji(int m, int indeks);
void WlaczOswietlenie(void);
void DefiniujMaterial(void);
void UstawParametryOswietlenia(int indeks, char operacja);
void UstawParametryMaterialu(int indeks, char operacja);
void UstawParametryWidoku(int szer, int wys);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);
void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y);


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca domyslne parametry walca, materialu i zrodla swiatla
void UstawDomyslneWartosciParametrow(void)
{
    GLfloat param_materialu[6][4] = {//niebieski blyszczacy widziany w bialym swietle
    {0.0, 0.0, 1.0, 1.0},  // [0] wspolczynnik odbicia swiatla otoczenia
    {0.0, 0.0, 1.0, 1.0},  // [1] wspolczynnik odbicia swiatla rozproszonego
    {1.0, 1.0, 1.0, 1.0},  // [2] wspolczynnik odbicia swiatla lustrzanego
    {50.0, 0.0, 0.0, 0.0},  // [3] polysk 
    {0.0, 0.0, 0.0, 0.0} }; // [4] kolor swiatla emitowanego

    GLfloat param_materialu2[6][4] = {//szary matowy widziany w bialym swietle
    {0.5, 0.5, 0.5, 1.0},  // [0] wspolczynnik odbicia swiatla otoczenia
    {0.5, 0.5, 0.5, 1.0},  // [1] wspolczynnik odbicia swiatla rozproszonego
    {0.0, 0.0, 0.0, 1.0},  // [2] wspolczynnik odbicia swiatla lustrzanego
    {0.0, 0.0, 0.0, 0.0},  // [3] polysk 
    {0.0, 0.0, 0.0, 0.0} }; // [4] kolor swiatla emitowanego

    GLfloat param_materialu3[6][4] = {//polerowane zloto
    {0.247250, 0.224500, 0.064500, 1.000000},  // [0] wspolczynnik odbicia swiatla otoczenia
    {0.346150, 0.314300, 0.090300, 1.000000},  // [1] wspolczynnik odbicia swiatla rozproszonego
    {0.797357, 0.723991, 0.208006, 1.000000},  // [2] wspolczynnik odbicia swiatla lustrzanego
    {83.2, 0.0, 0.0, 0.0},  // [3] polysk 
    {0.0, 0.0, 0.0, 0.0} }; // [4] kolor swiatla emitowanego

    GLfloat param_swiatla[10][4] = {
    {0.5, 0.5, 0.0, 1.0},   // [0] otoczenie
    {0.5, 0.5, 0.0, 1.0},   // [1] rozproszenie
    {0.5, 0.5, 0.0, 1.0},   // [2] lustrzane
    {0.0, 0.0, 0.0, 0.0},   // [3] polozenie
    {0.0, 0.0, 0.0, 0.0},   // [4] kierunek swiecenia
    {0.0, 0.0, 0.0, 0.0},   // [5] tlumienie katowe swiatla
    {45.0, 0.0, 0.0, 0.0},  // [6] kat odciecia swiatla
    {1.0, 0.0, 0.0, 0.0},   // [7] stale tlumienie
    {0.0, 0.0, 0.0, 0.0},   // [8] tlumienie liniowe
    {0.0, 0.0, 0.0, 0.0} }; // [9] tlumienie kwadratowe 

    GLfloat param_swiatla2[10][4] = {
    {0.5, 0.0, 0.5, 0.7},     // [0] otoczenie
    {0.5, 0.0, 0.5, 0.7},     // [1] rozproszenie
    {0.5, 0.0, 0.5, 0.7},     // [2] lustrzane
    {10.0, -5.0, 10.0, 1.0},  // [3] polozenie
    {-1.0, 1.0, -1.0, 1.0},     // [4] kierunek swiecenia
    {0.0, 0.0, 0.0, 0.0},     // [5] tlumienie katowe swiatla
    {180.0, 0.0, 0.0, 0.0},   // [6] kat odciecia swiatla
    {1.0, 0.0, 0.0, 0.0},     // [7] stale tlumienie
    {0.0, 0.0, 0.0, 0.0},     // [8] tlumienie liniowe
    {0.0, 0.0, 0.0, 0.0} }; // [9] tlumienie kwadratowe 

     // Skopiowanie zawartosci tablic param_* do tablic 
    memcpy(swiatlo, param_swiatla, LPOZ_MENU_SWIATLA * 4 * sizeof(GLfloat));
    memcpy(swiatlo2, param_swiatla2, LPOZ_MENU_SWIATLA * 4 * sizeof(GLfloat));
    memcpy(material, param_materialu, LPOZ_MENU_MATERIALU * 4 * sizeof(GLfloat));
    memcpy(material2, param_materialu2, LPOZ_MENU_MATERIALU * 4 * sizeof(GLfloat));
    memcpy(material3, param_materialu3, LPOZ_MENU_MATERIALU * 4 * sizeof(GLfloat));


    // Pozostale parametry
    sIndeks = 0;        // Wybrana pozycja w tablicy parametrow swiatla
    mIndeks = 0;        // Wybrana pozycja w tablicy parametrow materialu
    menu = ID_MENU_SWIATLA;

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

//Wlaczenie  oswietlenia sceny
void WlaczOswietlenie(void)
{
    GLfloat pozycja_swiatla[] = { promien_swiatla * cos(DEG2RAD(kat_swiatla)) , 0.01 , promien_swiatla * sin(DEG2RAD(kat_swiatla)), 1.0 };
    GLfloat kierunek_swiatla[] = { -(promien_swiatla * cos(DEG2RAD(kat_swiatla))) , 0.0 , -(promien_swiatla * sin(DEG2RAD(kat_swiatla))), 0.0 };

    // Odblokowanie zerowego zrodla swiatla
    if (wlacznik1)glEnable(GL_LIGHT0);
    else  glDisable(GL_LIGHT0);
    if (wlacznik2) glEnable(GL_LIGHT1);
    else  glDisable(GL_LIGHT1);


    // Inicjowanie swiatla 1 + mala sferka
    glPushMatrix();
    glRotatef(nachylenie_swiatla, 1, 0, 0);
    glTranslatef(promien_swiatla * cos(DEG2RAD(kat_swiatla)), 0.01, promien_swiatla * sin(DEG2RAD(kat_swiatla)));
    glColor3f(0.0, 0.0, 0.0);
    glutSolidSphere(0.1, 20, 20);

    glLightfv(GL_LIGHT0, GL_AMBIENT, swiatlo[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, swiatlo[1]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, swiatlo[2]);
    glLightfv(GL_LIGHT0, GL_POSITION, pozycja_swiatla);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, kierunek_swiatla);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, swiatlo[5][0]);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, swiatlo[6][0]);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, swiatlo[7][0]);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, swiatlo[8][0]);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, swiatlo[9][0]);

    glPopMatrix();

    // Inicjowanie  wiat a 2 + ma a sferka

    glPushMatrix();
    glTranslatef(-10.0, 10.0, 10.0);
    glColor3f(0.0, 0.0, 0.0);
    //glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    glLightfv(GL_LIGHT1, GL_AMBIENT, swiatlo2[0]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, swiatlo2[1]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, swiatlo2[2]);
    glLightfv(GL_LIGHT1, GL_POSITION, swiatlo2[3]);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, swiatlo2[4]);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, swiatlo2[5][0]);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, swiatlo2[6][0]);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, swiatlo2[7][0]);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, swiatlo2[8][0]);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, swiatlo2[9][0]);

}



//////////////////////////////////////////////////////////////////////////////////////////
// Zdefiniowanie walasciwosci materialu walca na podstawie zapisanych w tablcy 'material'
// parametrow (material obowiazuje tylko do scian skierowanych przodem do obserwatora)
void Material1(void) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, material[0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material[1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material[2]);
    glMaterialfv(GL_FRONT, GL_SHININESS, material[3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, material[4]);
}

void Material2(void) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, material2[0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material2[1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material2[2]);
    glMaterialfv(GL_FRONT, GL_SHININESS, material2[3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, material2[4]);
}

void Material3(void) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, material3[0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material3[1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material3[2]);
    glMaterialfv(GL_FRONT, GL_SHININESS, material3[3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, material3[4]);
}

void RysujTekstRastrowy(void* font, char* tekst)
{
    for (int i = 0; i < (int)strlen(tekst); i++)
        glutBitmapCharacter(font, tekst[i]);
}

void RysujNakladke(void)//slasz piekne menu, nie trzaeba interaktywnie
{
    char buf[255];
    // Zmiana typu rzutu z perspektywicznego na ortogonalny
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, szerokoscOkna, 0.0, wysokoscOkna, -100.0, 100.0);

    // Modelowanie sceny 2D (zawartosci nakladki)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Zablokowanie oswietlenia (mialoby ono wplyw na kolor tekstu)
    glDisable(GL_LIGHTING);

    // Okreslenie koloru tekstu
    glColor3f(1.0, 1.0, 1.0);

    // RYSOWANIE MENU 
    glColor3f(1.0, 1.0, 1.0);

    sprintf(buf, " 1, 2, 3 - zmien material (%d)", kolor + 1);
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 150);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " -/_ - zmien liczbe podzialow pionowych (%.f)", ilosc_podzialow);
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 160);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    if (cieniowanie == 0) sprintf(buf, " 4 - cieniowanie (plaskie)");
    else  sprintf(buf, " 4 - cieniowanie (gladkie)");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 170);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    if (malowanie == 0) sprintf(buf, " 5 - wypelnienie - ON");
    else  sprintf(buf, " 5 - wypelnienie - OFF");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 180);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    if (normalne == 1) sprintf(buf, " 6 - malowanie normalnych - ON");
    else  sprintf(buf, " 6 - malowanie normalnych - OFF");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 190);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " =/+ - odleglosc od obiektu");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 200);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " <ARROW_UP>/<ARROW_DOWN> - obrot osi x");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 210);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " <ARROW_LEFT>/<ARROW_RIGHT> - obrot osi y");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 220);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " Swiatlo ON/OFF:");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 230);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    if (wlacznik1 == 1) sprintf(buf, " F1 - swiatlo 1 ON");
    else sprintf(buf, " F1 - swiatlo 1 OFF");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 240);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    if (wlacznik2 == 1) sprintf(buf, " F2 - swiatlo 2 ON");
    else sprintf(buf, " F2 - swiatlo 2 OFF");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 250);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " Ruchome swiatlo:");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 260);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " w/W - zmien promien orbity swiatla 1 (%.f)", promien_swiatla);
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 270);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    if (szybkosc_swiatla == 0) sprintf(buf, " q/Q - zmien predkosc swiatla 1 (1.0)");
    else if (szybkosc_swiatla == 1) sprintf(buf, " q/Q - zmien predkosc swiatla 1 (0.5)");
    else if (szybkosc_swiatla == 2) sprintf(buf, " q/Q - zmien predkosc swiatla 1 (0.125)");
    else sprintf(buf, " q/Q - zmien predkosc swiatla 1 (STOP)");
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 280);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    sprintf(buf, " a/A - zmien nachylenie swiatla 1 (%.f)", nachylenie_swiatla);
    glRasterPos2i(X_OFFSET_OBIEKT, Y_OFFSET_OBIEKT - 290);
    RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

    // Przywrocenie macierzy sprzed wywolania funkcji
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Odblokowanie oswietlenia
    glEnable(GL_LIGHTING);
}

double r = 5;
double kat = 270.0;
double h = 4.0;
double alfa;
double beta;
double delta;

//////////////////////////////////////////////////////////////////////////////////////////
// Walec
void RysujWalec() {
    delta = r / ilosc_podzialow;
    beta = h / ilosc_podzialow;
    alfa = kat / ilosc_podzialow;
    double temp_r, temp_r_2, temp_h, temp_h_2;

    //dolna podstawa
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 0.0);
    for (int i = 0.0; i * alfa <= kat; i++)
    {
        glNormal3f(0, -1, 0);
        glVertex3f(r * cos(DEG2RAD(i * alfa)), 0, r * sin(DEG2RAD(i * alfa)));
    }
    glEnd();

    //gorna podstawa
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, h, 0.0);
    for (int i = 0.0; i * alfa <= kat; i++)
    {
        glNormal3f(0, 1, 0);
        glVertex3f(r * cos(DEG2RAD(i * alfa)), h, r * sin(DEG2RAD(i * alfa)));
    }
    glEnd();

    //boki
    glColor3f(1.0, 1.0, 0.0);
    for (int j = 0; j < ilosc_podzialow; j++)
    {
        temp_h = j * beta;
        temp_h_2 = (j + 1) * beta;
        glBegin(GL_TRIANGLES);
        for (int i = 0.0; i * alfa < kat; i++)
        {
            glNormal3f(cos(DEG2RAD(i * alfa)), 0, sin(DEG2RAD(i * alfa)));
            glVertex3f(r * cos(DEG2RAD(i * alfa)), temp_h, r * sin(DEG2RAD(i * alfa)));
            glNormal3f(cos(DEG2RAD(i * alfa)), 0, sin(DEG2RAD(i * alfa)));
            glVertex3f(r * cos(DEG2RAD(i * alfa)), temp_h_2, r * sin(DEG2RAD(i * alfa)));
            glNormal3f(cos(DEG2RAD((i + 1) * alfa)), 0, sin(DEG2RAD((i + 1) * alfa)));
            glVertex3f(r * cos(DEG2RAD((i + 1) * alfa)), temp_h, r * sin(DEG2RAD((i + 1) * alfa)));
            glNormal3f(cos(DEG2RAD(i * alfa)), 0, sin(DEG2RAD(i * alfa)));
            glVertex3f(r * cos(DEG2RAD(i * alfa)), temp_h_2, r * sin(DEG2RAD(i * alfa)));
            glNormal3f(cos(DEG2RAD((i + 1) * alfa)), 0, sin(DEG2RAD((i + 1) * alfa)));
            glVertex3f(r * cos(DEG2RAD((i + 1) * alfa)), temp_h, r * sin(DEG2RAD((i + 1) * alfa)));
            glNormal3f(cos(DEG2RAD((i + 1) * alfa)), 0, sin(DEG2RAD((i + 1) * alfa)));
            glVertex3f(r * cos(DEG2RAD((i + 1) * alfa)), temp_h_2, r * sin(DEG2RAD((i + 1) * alfa)));
        }
        glEnd();
    }

    //domkniecie z
    glColor3f(1.0, 1.0, 0.0);
    for (int j = 0; j < ilosc_podzialow; j++)
    {
        temp_h = j * beta;
        temp_h_2 = (j + 1) * beta;
        glBegin(GL_TRIANGLES);
        for (int i = 0.0; i * alfa < kat; i++)
        {
            glNormal3f(-1, 0, 0);
            temp_r = i * delta;
            temp_r_2 = (i + 1) * delta;
            glVertex3f(0, temp_h, -temp_r_2);
            glVertex3f(0, temp_h_2, -temp_r_2);
            glVertex3f(0, temp_h, -temp_r);
            glVertex3f(0, temp_h_2, -temp_r);
            glVertex3f(0, temp_h, -temp_r);
            glVertex3f(0, temp_h_2, -temp_r_2);
        }
        glEnd();
    }
    //domkniecie x
    for (int j = 0; j < ilosc_podzialow; j++)
    {
        temp_h = j * beta;
        temp_h_2 = (j + 1) * beta;
        glBegin(GL_TRIANGLES);
        for (int i = 0.0; i * alfa < kat; i++)
        {
            glNormal3f(0, 0, -1);
            temp_r = i * delta;
            temp_r_2 = (i + 1) * delta;
            glVertex3f(temp_r_2, temp_h, 0);
            glVertex3f(temp_r_2, temp_h_2, 0);
            glVertex3f(temp_r, temp_h, 0);
            glVertex3f(temp_r, temp_h_2, 0);
            glVertex3f(temp_r, temp_h, 0);
            glVertex3f(temp_r_2, temp_h_2, 0);
        }
        glEnd();
    }
}

void CzyNormalne() {
    delta = r / ilosc_podzialow;
    beta = h / ilosc_podzialow;
    alfa = kat / ilosc_podzialow;
    double temp_r, temp_r_2, temp_h, temp_h_2;
    //zaobrazowanie wektorow normalmych sciany bocznej
    glColor3f(0.0, 0.0, 0.0);
    for (int j = 0; j < ilosc_podzialow; j++)
    {
        temp_h = j * beta;
        temp_h_2 = (j + 1) * beta;
        glBegin(GL_LINES);
        for (int i = 0.0; i * alfa < kat; i++)
        {
            glVertex3f(r * cos(DEG2RAD(i * alfa)), temp_h, r * sin(DEG2RAD(i * alfa)));
            glVertex3f(1.5 * r * cos(DEG2RAD(i * alfa)), temp_h, 1.5 * r * sin(DEG2RAD(i * alfa)));

            glVertex3f(r * cos(DEG2RAD(i * alfa)), temp_h_2, r * sin(DEG2RAD(i * alfa)));
            glVertex3f(1.5 * r * cos(DEG2RAD(i * alfa)), temp_h_2, 1.5 * r * sin(DEG2RAD(i * alfa)));

            glVertex3f(r * cos(DEG2RAD((i + 1) * alfa)), temp_h, r * sin(DEG2RAD((i + 1) * alfa)));
            glVertex3f(1.5 * r * cos(DEG2RAD((i + 1) * alfa)), temp_h, 1.5 * r * sin(DEG2RAD((i + 1) * alfa)));

            glVertex3f(r * cos(DEG2RAD(i * alfa)), temp_h_2, r * sin(DEG2RAD(i * alfa)));
            glVertex3f(1.5 * r * cos(DEG2RAD(i * alfa)), temp_h_2, 1.5 * r * sin(DEG2RAD(i * alfa)));

            glVertex3f(r * cos(DEG2RAD((i + 1) * alfa)), temp_h, r * sin(DEG2RAD((i + 1) * alfa)));
            glVertex3f(1.5 * r * cos(DEG2RAD((i + 1) * alfa)), temp_h, 1.5 * r * sin(DEG2RAD((i + 1) * alfa)));

            glVertex3f(r * cos(DEG2RAD((i + 1) * alfa)), temp_h_2, r * sin(DEG2RAD((i + 1) * alfa)));
            glVertex3f(1.5 * r * cos(DEG2RAD((i + 1) * alfa)), temp_h_2, 1.5 * r * sin(DEG2RAD((i + 1) * alfa)));
        }
        //wektory podstaw do zaobrazowania
        glVertex3f(0, -5, 0);
        glVertex3f(0, 9, 0);

        //zaobrazowanie wektorow scianek lezacych na osi x/z
        glVertex3f(0, 2, -2.5);
        glVertex3f(12, 2, -2.5);
        glVertex3f(2.5, 2, -12);
        glVertex3f(2.5, 2, 0);

        glEnd();
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu
void UstawParametryWidoku(int szer, int wys)
{
    // Zapamietanie wielkosci widoku
    szerokoscOkna = szer;
    wysokoscOkna = wys;

    // Ustawienie parametrow viewportu
    glViewport(0, 0, szerokoscOkna, wysokoscOkna);

    // Przejscie w tryb modyfikacji macierzy rzutowania
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (float)szerokoscOkna / (float)wysokoscOkna, 1.0, 1000.0);

    // Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
    glMatrixMode(GL_MODELVIEW);

    // Zmiana macierzy znajdujacej sie na wierzcholku stosu na macierz jednostkowa 
    glLoadIdentity();
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja generujaca pojedyncza klatke animacji
void WyswietlObraz(void)
{
    // Wyczyszczenie bufora ramki i bufora glebokosci
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Okreslenie wielkosci widoku, wlaczenie rzutowania perspektywicznego
    // i zainicjowanie stosu macierzy modelowania
    UstawParametryWidoku(szerokoscOkna, wysokoscOkna);

    // Ustalenie polozenia obserwatora
    glTranslatef(0, 0, odleglosc);
    glRotatef(rotObsX, 1, 0, 0);
    glRotatef(rotObsY, 0, 1, 0);
    glRotatef(rotObsZ, 0, 0, 1);

    // Generacja obrazu
    if (kolor == 0) Material1();
    else if (kolor == 1) Material2();
    else Material3();
    
    RysujWalec();

    if (normalne == 1) CzyNormalne();
    WlaczOswietlenie();

    // Narysowanie tekstow z opisem parametrow oswietlenia i materialu
    RysujNakladke();

    // Przelaczenie buforow ramki
    glutSwapBuffers();

    // Wy wietlanie siatki/materialow 
    if (malowanie == 1) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (cieniowanie == 1) glShadeModel(GL_SMOOTH);
    else glShadeModel(GL_FLAT);

    if (szybkosc_swiatla == 0) kat_swiatla = kat_swiatla + 0.5;
    else if (szybkosc_swiatla == 1) kat_swiatla = kat_swiatla + 0.25;
    else if (szybkosc_swiatla == 2) kat_swiatla = kat_swiatla + 0.125;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y)
{
    switch (klawisz)
    {
    case GLUT_KEY_UP:
        rotObsX = (rotObsX < 10000.0) ? rotObsX + 1.0 : rotObsX;
        break;

    case GLUT_KEY_DOWN:
        rotObsX = (rotObsX > -10000) ? rotObsX - 1.0 : rotObsX;
        break;

    case GLUT_KEY_LEFT:
        rotObsY = (rotObsY > -10000.0) ? rotObsY - 1.0 : rotObsY;
        break;

    case GLUT_KEY_RIGHT:
        rotObsY = (rotObsY < 10000.0) ? rotObsY + 1.0 : rotObsY;
        break;

    case GLUT_KEY_HOME: // Nachylenie
        rotObsZ = (rotObsZ < 10000.0) ? rotObsZ + 1.0 : rotObsZ;
        break;

    case GLUT_KEY_END: // Nachylenie w druga
        rotObsZ = (rotObsZ > -10000.0) ? rotObsZ - 1.0 : rotObsZ;
        break;

    case GLUT_KEY_F1: // Przelacznik swiatlo 1
        wlacznik1 = (wlacznik1 > 0) ? 0 : 1;
        break;

    case GLUT_KEY_F2: // Przelacznik swiatlo 2
        wlacznik2 = (wlacznik2 > 0) ? 0 : 1;
        break;
    }
}
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{

    switch (klawisz)
    {
    case '1': // Zmiana materialu
        kolor = 0;
        break;

    case '2': // Zmiana materialu
        kolor = 1;
        break;

    case '3': // Zmiana materialu
        kolor = 2;
        break;

    case '=': // Blizej
        odleglosc = (odleglosc < -5.0) ? odleglosc + 1.0 : odleglosc;
        break;

    case '+': // Dalej
        odleglosc = (odleglosc > -150.0) ? odleglosc - 1.0 : odleglosc;
        break;

    case '-': // Wiecej podzialow
        ilosc_podzialow = (ilosc_podzialow < 64.0) ? ilosc_podzialow + 2.0 : ilosc_podzialow;
        break;

    case '_': // Mniej podzialow
        ilosc_podzialow = (ilosc_podzialow > 4.0) ? ilosc_podzialow - 2.0 : ilosc_podzialow;
        break;

    case 'w': // Dalej zrodlo
        promien_swiatla = (promien_swiatla < 120.0) ? promien_swiatla + 1.0 : promien_swiatla;
        break;

    case 'W': // Blizej zrodlo
        promien_swiatla = (promien_swiatla > 2.0) ? promien_swiatla - 1.0 : promien_swiatla;
        break;

    case 'a': // Nachylenie zrodlo
        if (nachylenie_swiatla < 360) nachylenie_swiatla++;
        else nachylenie_swiatla = 0;
        break;

    case 'A': // Nachylenie zrodlo w druga
        if (nachylenie_swiatla > 0) nachylenie_swiatla--;
        else nachylenie_swiatla = 360;
        break;

    case 'Q': // Wolniej zrodlo
        szybkosc_swiatla = (szybkosc_swiatla < 3.0) ? szybkosc_swiatla + 1.0 : szybkosc_swiatla;
        break;

    case 'q': // Szybciej zrodlo
        szybkosc_swiatla = (szybkosc_swiatla > 0.0) ? szybkosc_swiatla - 1.0 : szybkosc_swiatla;
        break;

    case '4': // Przelacznik cieniowanie
        cieniowanie = (cieniowanie > 0) ? 0 : 1;
        break;

    case '5': // Przelacznik linie - wypelnienie
        malowanie = (malowanie > 0) ? 0 : 1;
        break;

    case '6': // Przelacznik pokazywanie wektorow
        normalne = (normalne == 1) ? 0 : 1;
        break;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char** argv)
{
    // Zainicjowanie biblioteki GLUT
    glutInit(&argc, argv);

    // Ustawienie trybu wyswietlania
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // Ustawienie polozenia dolenego lewego rogu okna
    glutInitWindowPosition(100, 100);

    // Ustawienie rozmiarow okna
    glutInitWindowSize(szerokoscOkna, wysokoscOkna);

    // Utworzenie okna
    glutCreateWindow("Oswietlony walec");

    // Zarejestrowanie funkcji obslugi klawiatury
    glutKeyboardFunc(ObslugaKlawiatury);

    // Zarejestrowanie funkcji obslugi klawiszy specjalnych
    glutSpecialFunc(ObslugaKlawiszySpecjalnych);

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
    glClearColor(0.0, 0.0, 0.0, 0.0);


    // Zarejestrowanie funkcji (callback) wywolywanej za kazdym razem kiedy
    // zmieniane sa rozmiary okna
    glutReshapeFunc(UstawParametryWidoku);

    // Ustawienie domyslnych wartosci parametrow
    UstawDomyslneWartosciParametrow();


    // Zarejestrowanie funkcji (callback) wyswietlajacej
    glutDisplayFunc(WyswietlObraz);


    // Zarejestrowanie funkcji wykonywanej gdy okno nie obsluguje
    // zadnych zadan
    glutIdleFunc(WyswietlObraz);


    // Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
    // w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
    glutMainLoop();

    return 0;
}