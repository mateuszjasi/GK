//////////////////////////////////////////////////////////////////////////////////////////
//
// Program wyswietlajacy szescian w rzucie perspektywicznym. Dlugosc boku szescianu
// moze byc interaktywnie zmieniana za pomoca klwiszy '+' i '-'.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <glut.h>


// Definicja stalych
#define DLUGOSC_BOKU            5.0
#define OBSERWATOR_ODLEGLOSC    20.0
#define OBSERWATOR_OBROT_X      20.0
#define OBSERWATOR_OBROT_Y      20.0
#define OBSERWATOR_FOV_Y        30.0  

// Zmienne globalne
double  bok           = DLUGOSC_BOKU; // Dlugosc boku szescianu
int     szerokoscOkna = 800;
int     wysokoscOkna  = 600;


// Prototypy funkcji
void RysujSzescian(double a);
void UstawParametryWidoku(int szer, int wys);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca szescian o boku "a" w trybie GL_QUAD_STRIP.
// Srodek szescianu znajduje siê w punkcie (0,0,0).
void RysujSzescian(double a)
{
  // Sciany boczne
  glBegin(GL_QUAD_STRIP); 
    glVertex3f(a/2.0, a/2.0, a/2.0);
    glVertex3f(a/2.0, -a/2.0, a/2.0);
    glVertex3f(a/2.0, a/2.0, -a/2.0);
    glVertex3f(a/2.0, -a/2.0, -a/2.0);

    glVertex3f(-a/2.0, a/2.0, -a/2.0);
    glVertex3f(-a/2.0, -a/2.0, -a/2.0);

    glVertex3f(-a/2.0, a/2.0, a/2.0);
    glVertex3f(-a/2.0, -a/2.0, a/2.0);

    glVertex3f(a/2.0, a/2.0, a/2.0);
    glVertex3f(a/2.0, -a/2.0, a/2.0);
  glEnd(); 

  // Gorna sciana
  glBegin(GL_QUAD_STRIP); 
    glVertex3f(-a/2.0, a/2.0, a/2.0);
    glVertex3f(a/2.0, a/2.0, a/2.0);
    glVertex3f(-a/2.0, a/2.0, -a/2.0);
    glVertex3f(a/2.0, a/2.0, -a/2.0);
  glEnd();

  // Dolna sciana
  glBegin(GL_QUAD_STRIP); 
    glVertex3f(-a/2.0, -a/2.0, a/2.0);
    glVertex3f(a/2.0, -a/2.0, a/2.0);
    glVertex3f(-a/2.0, -a/2.0, -a/2.0);
    glVertex3f(a/2.0, -a/2.0, -a/2.0);
  glEnd();

}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu. Powinna
// byæ wywolywana kazdorazowo po zmianie rozmiarow okna programu.
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
  gluPerspective(OBSERWATOR_FOV_Y, (float)szerokoscOkna/(float)wysokoscOkna, 1.0, 1000.0);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja wyswietlajaca pojedyncza klatke animacji
void WyswietlObraz(void)
{
  // Wyczyszczenie bufora koloru i bufora glebokosci
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
  glMatrixMode(GL_MODELVIEW);

  // Zastapienie aktywnej macierzy macierza jednostkowa
  glLoadIdentity();

  // Ustalenie polozenia obserwatora
  glTranslatef(0, 0, -OBSERWATOR_ODLEGLOSC);
  glRotatef(OBSERWATOR_OBROT_X, 1, 0, 0);
  glRotatef(OBSERWATOR_OBROT_Y, 0, 1, 0);

  // Narysowanie szescianu
  RysujSzescian(bok);

  // Przelaczenie buforow ramki
  glutSwapBuffers();
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
  if(klawisz == '+')
      bok *= 2.0;
  else if (klawisz == '-')
      bok /= 2.0;
  else if (klawisz == 27)
      exit(0);      
}


//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char **argv)
{
  // Zainicjowanie biblioteki GLUT
  glutInit(&argc, argv);

  // Ustawienie trybu wyswietlania
  glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

  // Ustawienie polozenia dolenego lewego rogu okna
  glutInitWindowPosition(100, 100);

  // Ustawienie rozmiarow okna
  glutInitWindowSize(szerokoscOkna, wysokoscOkna);

  // Utworzenie okna
  glutCreateWindow("Szescian");

  // Odblokowanie bufora glebokosci
  glEnable(GL_DEPTH_TEST);

  // Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
  glClearDepth(1000.0);

  // Ustawienie koloru czyszczenia bufora ramki
  glClearColor (0.0f, 0.0f, 0.3f, 0.0f);

  // Wlaczenie wyswietlania wielokatow w postaci obrysow (przydatne w celach diagnostycznych).
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  
  // Zarejestrowanie funkcji (callback) odpowiedzialnej za 
  glutDisplayFunc(WyswietlObraz);

  // Zarejestrowanie funkcji (callback) wywolywanej za kazdym razem kiedy
  // zmieniane sa rozmiary okna
  glutReshapeFunc(UstawParametryWidoku);

  // Zarejestrowanie funkcji wykonywanej gdy okno nie obsluguje
  // zadnych zadan
  glutIdleFunc(WyswietlObraz);

  // Zarejestrowanie funkcji obslugi klawiatury
  glutKeyboardFunc(ObslugaKlawiatury);

  // Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
  // w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
  glutMainLoop();

  return 0;
}
