//////////////////////////////////////////////////////////////////////////////////////////
// Program demonstruje  skladanie transformacji przy modelowaniu zlozonych obiektow
// skladajacych sie z wielu elementow ruchomych (ramie robota). 
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>

// Wskazniki do wykorzystywanych kwadryk
GLUquadricObj* podstawaSciany;
GLUquadricObj* podstawaDyskG;
GLUquadricObj* przegubSciany;
GLUquadricObj* przegubDyskG;
GLUquadricObj* przegubDyskD;
GLUquadricObj* glowicaSciany;
GLUquadricObj* glowicaDyskG;
GLUquadricObj* glowicaDyskD;

//-1-
GLfloat rotObsZ = 0.0;
GLfloat rotObsY = 40.0;
GLfloat rotObsX = 40.0;
GLfloat min_distance = -100.0;
GLfloat max_distance = -300.0;
GLfloat distance = -150.0;

GLfloat rotPodstawy = 0.0;
GLfloat rotRamienia1 = 40.0;
GLfloat rotRamienia2 = -40.0;
GLfloat rotGlowicy = 20.0;
GLfloat rozUchwytow = 0.5;

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja inicjujaca elementy skladowe ramienia robota zamodelowane jako kwadryki
// 
void InicjujRamieRobota(void)
{
    // Zainicjowanie scian bocznych walca bedacego podstawa ramienia
    podstawaSciany = gluNewQuadric();
    gluQuadricDrawStyle(podstawaSciany, GLU_LINE);

    // Zainicjowanie gornej podstawy walca 
    podstawaDyskG = gluNewQuadric();
    gluQuadricDrawStyle(podstawaDyskG, GLU_LINE);

    // Zainicjowanie scian bocznych cylindrycznego przegubu ramienia
    przegubSciany = gluNewQuadric();
    gluQuadricDrawStyle(przegubSciany, GLU_LINE);

    // Zainicjowanie gornej podstawy walca 
    przegubDyskG = gluNewQuadric();
    gluQuadricDrawStyle(przegubDyskG, GLU_LINE);

    // Zainicjowanie dolnej podstawy walca 
    przegubDyskD = gluNewQuadric();
    gluQuadricDrawStyle(przegubDyskD, GLU_LINE);

    // Zainicjowanie scian bocznych cylindra glowicy 
    glowicaSciany = gluNewQuadric();
    gluQuadricDrawStyle(glowicaSciany, GLU_LINE);

    // Zainicjowanie gornej podstawy walca 
    glowicaDyskG = gluNewQuadric();
    gluQuadricDrawStyle(glowicaDyskG, GLU_LINE);

    // Zainicjowanie dolnej podstawy walca 
    glowicaDyskD = gluNewQuadric();
    gluQuadricDrawStyle(glowicaDyskD, GLU_LINE);

}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca obraz sceny widzianej z biezacej pozycji obserwatora
// Zalozenie: Funkcja glutWireSpehere moze ryswac tylko sfere o promieniu 1  
void RysujRamieRobota(GLfloat obrotPodstawy, GLfloat obrotRamienia1,
    GLfloat obrotRamienia2, GLfloat obrotGlowicy,
    GLfloat rozstawUchwytow)
{
    // Pocztaek tworzenia ukladu wspolrzednych
    glBegin(GL_LINES);

    // Os X
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-100.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);

    // Os Y
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, -100.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);

    // Os Z
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, -100.0);
    glVertex3f(0.0, 0.0, 100.0);

    // Koniec tworzenia ukladu wspolrzednych
    glEnd();

    glColor3f(1.0, 1.0, 0);




    // Przygotowanie stosu macierzy modelowania
    glPushMatrix();
    // Przesuniecie robota
    glTranslatef(0.0, 60.0, 0.0);
    // Rysowanie podstawy ramienia (cylinder bez dolnej podstawy)
    glPushMatrix();
    // - sciany boczne
    glRotatef(-90.0, 1, 0, 0);
    gluCylinder(podstawaSciany, 3.0, 3.0, 1.0, 20, 4);

    // - gorna podstawa
    glTranslatef(0.0, 0.0, 1.0);
    gluDisk(podstawaDyskG, 0.0, 3.0, 20, 4);
    glPopMatrix();


    // Rysowanie dwoch wspornikow ramienia (prostopadlosciany)
    glPushMatrix();
    glScalef(3.0, 4.0, 1.0);
    glTranslatef(0.0, 3.0 / 4.0, 1.0);
    glutWireCube(1);
    glTranslatef(0.0, 0.0, -2.0);
    glutWireCube(1);
    glPopMatrix();

    // Wyznaczenie osi obrotu ramienia w plaszczyznie pionowej
    glTranslatef(0.0, 4.0, 0.0);

    // Obrot ramienia w plaszczyznie pionowej
    glRotatef(obrotRamienia1, 0, 0, 1);

    // Modelowanie ramienia nr 1
    glPushMatrix();
    glScalef(8.0, 1.0, 1.0);
    glTranslatef(3.5 / 8.0, 0.0, 0.0);
    glutWireCube(1);
    glPopMatrix();

    // Wyznaczenie osi obrotu ramienia 2 w plaszczyznie pionowej
    glTranslatef(7.5, 0.0, 0.0);

    // Obrot ramienia 2 wzgledem ramienia 1
    glRotatef(obrotRamienia2, 0, 0, 1);

    // Modelowanie przegubu (walca z obiema podstawami)
    glPushMatrix();
    // - sciany boczne
    glTranslatef(0.0, 0.0, -0.5);
    gluCylinder(podstawaSciany, 1.0, 1.0, 1.0, 20, 4);

    // - gorna podstawa
    gluDisk(podstawaDyskG, 0.0, 1.0, 20, 4);

    // - dolna podstawa
    glTranslatef(0.0, 0.0, 1.0);
    gluDisk(podstawaDyskG, 0.0, 1.0, 20, 4);
    glPopMatrix();

    // Modelowanie ramienia nr 2
    glPushMatrix();
    glScalef(4.0, 1.0, 1.0);
    glTranslatef(2.0 / 4.0, 0.0, 0.0);
    glutWireCube(1);
    glPopMatrix();

    // Wyznaczenie osi obrotu glowicy
    glTranslatef(4.0, 0.0, 0.0);
    glRotatef(90, 0, 1, 0);
    glRotatef(obrotGlowicy, 0, 0, 1);

    // Modelowanie glowicy (walca z oboma podstawami)
    glPushMatrix();
    // - sciany boczne
    gluCylinder(podstawaSciany, 1.0, 1.0, 1.0, 20, 4);

    // - gorna podstawa
    gluDisk(podstawaDyskG, 0.0, 1.0, 20, 4);

    // - dolna podstawa
    glTranslatef(0.0, 0.0, 1.0);
    gluDisk(podstawaDyskG, 0.0, 1.0, 20, 4);
    glPopMatrix();

    // Modelowanie uchwytu (dwoch prostopadloscianow, ktore sie rozsuwaja i zsuwaja)
    glTranslatef(0.0, 0.0, 1.5);
    glScalef(0.5, 0.5, 2.0);
    glTranslatef(-rozstawUchwytow, 0.0, 0.25);
    glutWireCube(1);
    glTranslatef(rozstawUchwytow * 2.0, 0.0, 0.0);
    glutWireCube(1);



    // Posprzatanie na stosie macierzy modelowania
    glPopMatrix();
}

//-2-
// Rysowanie wie¿y
void Wieza()
{
    // Przygotowanie stosu macierzy modelowania
    glPushMatrix();
    // Ustawienie wiezy w po³o¿eniu
    glTranslatef(0.0, 80.0, 0.0);
    glColor3f(0.7, 0.2, 0.2);

    // Modelowanie podstawy wie¿y
    glPushMatrix();
    glTranslatef(0.0, 5.0, 0.0);
    glRotatef(45.0, 0, 1, 0);
    glScalef(4.2, 10.0, 4.2);
    glutSolidCube(1);
    glPopMatrix();

    // Modelowanie nadbudówki
    glColor3f(0.2, 1.0, 0.5);
    glPushMatrix();
    glTranslatef(0.0, 11.5, 0.0);
    glScalef(8.0, 3.0, 8.0);
    glutSolidCube(1);
    glPopMatrix();

    glColor3f(0.2, 0.2, 1.0);

    // Modelowanie wie¿yczek
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslatef(1.5, 7.0, -1.5);
    glutSolidCube(1);
    glTranslatef(-3.0, 0.0, 0.0);
    glutSolidCube(1);
    glTranslatef(0.0, 0.0, 3.0);
    glutSolidCube(1);
    glTranslatef(3.0, 0.0, 0.0);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

// -3-
GLUquadricObj* orbita;
GLfloat rotacjaZiemia = 0;
GLfloat rotacjaKsiezyc = 0;
GLfloat rotacjaPl1 = 0;
GLfloat rotacjaPl2 = 0;



void Uklad_Planet() {
    //Zadanie 3
    glPushMatrix();
    // S³oñce   
    glColor3f(1.0, 1.0, 0.0);
    glRotatef(90, 1, 0, 0);
    
    // Modelowanie s³oñce
    glutWireSphere(5.0, 20.0, 20.0);
    orbita = gluNewQuadric();
    gluQuadricDrawStyle(orbita, GLU_LINE);
    
    //orbita Ziemi
    glColor3f(0.0, 1.0, 1.0);
    gluDisk(orbita, 20, 20, 100, 100);

    // Zadanie 3a
    // orbita Planety1
    glPushMatrix();
    glColor3f(1.0 , 0.0, 1.0);
    glRotatef(30, 1, 0, 0);
    gluDisk(orbita, 60, 60, 100, 100);
    glPopMatrix();

    // Zadanie 3b
    // orbita Planety2
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glRotatef(-15, 1, 0, 0);
    gluDisk(orbita, 80, 80, 100, 100);
    glPopMatrix();

    glRotatef(-90, 1, 0, 0);

    // Ziemia
    glPushMatrix();
    glRotatef(rotacjaZiemia, 0, 1, 0);
    // rotacja CW[-]
    rotacjaZiemia -= 0.25;
    glRotatef(90, 1, 0, 0);

    glTranslatef(20, 0, 0);
    glColor3f(0.0, 1.0, 1.0);
    glutWireSphere(2, 20, 20);
    glColor3f(0.0, 0.5, 0.5);
    gluDisk(orbita, 5, 5, 100, 100);

    glRotatef(-90, 1, 0, 0);
    
    //Ksiezyc
    glPushMatrix();
    glRotatef(rotacjaKsiezyc, 0, 1, 0);
    //rotacja CCW[+]
    rotacjaKsiezyc += 0.5;
    glRotatef(90, 1, 0, 0);
    glTranslatef(5, 0, 0);
    glutWireSphere(0.5, 20, 20);
    glRotatef(-90, 1, 0, 0);
    glPopMatrix();

    glPopMatrix();


    //Zadanie 3a
    //Planeta1
    glPushMatrix();
    glRotatef(30, 1, 0, 0);
    glRotatef(rotacjaPl1, 0, 1, 0);
    //rotacja CCW[+]
    rotacjaPl1 += 0.15;
    glRotatef(90, 1, 0, 0);
    glTranslatef(60, 0, 0);
    glColor3f(1.0, 0.0, 1.0);
    glutWireSphere(3, 20, 20);

    glRotatef(-90, 1, 0, 0);
    glPopMatrix();


    // Zadanie 3b
    //Planeta2
    glPushMatrix();
    glRotatef(-15, 1, 0, 0);
    glRotatef(rotacjaPl2, 0, 1, 0);
    //rotacja CW[-]
    rotacjaPl2 -= 0.25;
    glRotatef(90, 1, 0, 0);

    glTranslatef(80, 0, 0);
    glColor3f(1.0, 1.0, 1.0);
    glutWireSphere(2, 20, 20);

    glRotatef(90, 1, 0, 0);
    glPopMatrix();

    glPopMatrix();
}

//-Indywidualne-
GLUquadricObj* cyliderWiatraka;
GLUquadricObj* dyskWiatraka;
GLfloat speed = 0.0;
GLfloat angle = 0.0;

void wiatrak() {
    angle += speed;
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    dyskWiatraka = gluNewQuadric();
    gluQuadricDrawStyle(dyskWiatraka, GLU_LINE);
    cyliderWiatraka = gluNewQuadric();
    gluQuadricDrawStyle(cyliderWiatraka, GLU_LINE);

    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.3, 0.3, 0.3);

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    gluCylinder(cyliderWiatraka, 10, 5, 5, 20, 20);

    glColor3f(0.0, 1.0, 0.0);
    gluDisk(dyskWiatraka, 0, 10, 20, 20);
    glTranslatef(0, 0, 5);

    glColor3f(1.0, 0.0, 0.0);
    gluDisk(dyskWiatraka, 0, 5, 20, 20);
    
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(cyliderWiatraka, 2.5, 2.5, 10, 20, 20);
    glTranslatef(0, 0, 10);

    glColor3f(0.0, 1.0, 0.0);
    gluDisk(dyskWiatraka, 0, 20, 20, 20);
    
    glColor3f(1.0, 1.0, 1.0);
    gluCylinder(cyliderWiatraka, 20, 20, 10, 20, 20);
    glTranslatef(0, 0, 10);

    glColor3f(1.0, 0.0, 0.0);
    gluDisk(dyskWiatraka, 0, 20, 20, 20);
    glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();

    glTranslatef(0, -25, 0);
    glRotatef(angle, 0, 1, 0);

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    gluCylinder(cyliderWiatraka, 8, 8, 10, 20, 20);
    glTranslatef(0, 0, 10);
    glColor3f(1.0, 0.0, 0.0);
    gluDisk(dyskWiatraka, 0, 8, 20, 20);
    glColor3f(1.0, 1.0, 1.0);
    glPopMatrix();
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glRotatef(i * 72, 0, 1, 0);
        glTranslatef(0, -4, 11);
        // ³¹cznik
        glPushMatrix();
        glScalef(2, 2, 6);
        glutWireCube(1);
        glPopMatrix();
        // p³at
        glPushMatrix();
        glTranslatef(0, 0, 33);
        glScalef(16, 2, 60);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja generujaca pojedyncza klatke animacji
void WyswietlObraz(void)
{
    // Wyczyszczenie bufora ramki i bufora glebokosci
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Powielenie macierzy na wierzcholku stosu 
    glPushMatrix();

    // Wyznaczenie polozenia obserwatora (przeksztalcenie uladu wspolrzednych 
    // sceny do ukladu wspolrzednych obserwatora). 
    glTranslatef(0, 0, distance);
    glRotatef(rotObsX, 1, 0, 0);
    glRotatef(rotObsY, 0, 1, 0);
    glRotatef(rotObsZ, 0, 0, 1);

    // Generacja obrazu sceny w niewidocznym buforze ramki
    //RysujRamieRobota(rotPodstawy, rotRamienia1, rotRamienia2, rotGlowicy, rozUchwytow);
    //Wieza();
    //Uklad_Planet();
    wiatrak();
    // Usuniecie macierzy lezacej na  wierzcholku stosu (powrot do stanu
    // sprzed wywolania funkcji)
    glPopMatrix();

    // Przelaczenie buforow ramki
    glutSwapBuffers();


}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu
void UstawParametryWidoku(int szerokosc, int wysokosc)
{
    // Ustawienie parametrow viewportu
    glViewport(0, 0, szerokosc, wysokosc);

    // Przejscie w tryb modyfikacji macierzy rzutowania
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (float)szerokosc / (float)wysokosc, 1.0, 1000.0);

    // Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
    glMatrixMode(GL_MODELVIEW);

    // Zmiana macierzy znajdujacej sie na wierzcholku stosu na macierz jednostkowa 
    glLoadIdentity();
}



//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja klawiszy specjalnych
void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y)
{
    switch (klawisz)
    {
    case GLUT_KEY_UP:
        rotObsX = rotObsX + 1.0;
        break;

    case GLUT_KEY_DOWN:
        rotObsX = rotObsX - 1.0;
        break;

    case GLUT_KEY_LEFT:
        rotObsY = rotObsY - 1.0;
        break;

    case GLUT_KEY_RIGHT:
        rotObsY = rotObsY + 1.0;
        break;

    case GLUT_KEY_HOME:
        rotObsZ = rotObsZ + 1.0;
        break;

    case GLUT_KEY_END:
        rotObsZ = rotObsZ - 1.0;
        break;

    case GLUT_KEY_PAGE_UP:
        speed = speed < 1 ? speed + 0.25 : speed;
        break;

    case GLUT_KEY_PAGE_DOWN:
        speed = speed > -1 ? speed - 0.25 : speed;
        break;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{

    switch (klawisz)
    {
    case '2':
        rotRamienia1 = (rotRamienia1 < 90.0) ? rotRamienia1 + 1.0 : rotRamienia1;
        break;

    case '@':
        rotRamienia1 = (rotRamienia1 > 0.0) ? rotRamienia1 - 1.0 : rotRamienia1;
        break;

    case '3':
        rotRamienia2 = (rotRamienia2 < 0.0) ? rotRamienia2 + 1.0 : rotRamienia2;
        break;

    case '#':
        rotRamienia2 = (rotRamienia2 > -90.0) ? rotRamienia2 - 1.0 : rotRamienia2;
        break;

    case '4':
        rotGlowicy = (rotGlowicy < 360.0) ? rotGlowicy + 1.0 : rotGlowicy;
        break;

    case '$':
        rotGlowicy = (rotGlowicy > 0.0) ? rotGlowicy - 1.0 : rotGlowicy;
        break;

    case '5':
        rozUchwytow = (rozUchwytow < 1.5) ? rozUchwytow + 0.1 : rozUchwytow;
        break;

    case '%':
        rozUchwytow = (rozUchwytow > 0.5) ? rozUchwytow - 0.1 : rozUchwytow;
        break;

    case '-':
        if (distance - 1 < max_distance) break;
        else distance--;
        break;

    case '+':
        if (distance + 1 > min_distance) break;
        else distance++;
        break;
    }

    if (klawisz == 27)
        exit(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char** argv)
{
    // Zainicjowanie biblioteki GLUT
    glutInit(&argc, argv);

    // Ustawienie trybu wyswietlania
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Ustawienie polozenia dolenego lewego rogu okna
    glutInitWindowPosition(100, 100);

    // Ustawienie rozmiarow okna
    glutInitWindowSize(400, 400);

    // Utworzenie okna
    glutCreateWindow("Zadanie z lab3");

    // Odblokowanie bufora glebokosci
    glEnable(GL_DEPTH_TEST);

    // Ustawienie funkcji wykonywanej na danych w buforze glebokosci
    glDepthFunc(GL_LEQUAL);

    // Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
    glClearDepth(1000.0);

    // Ustawienie koloru czyszczenia bufora ramki
    glClearColor(0.0, 0.0, 0.0, 0.0);

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


    // Zainicjowanie kwadryk tworzacych ramie robota
    InicjujRamieRobota();

    // Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
    // w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
    glutMainLoop();

    return 0;
}
