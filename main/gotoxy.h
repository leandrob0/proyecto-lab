#ifndef GOTOXY_H_INCLUDED
#define GOTOXY_H_INCLUDED

void color(int x);
void gotoxy(int X,int Y);
int whereX();
int whereY();
void hidecursor(int ver);
void dibujarCuadro(int x1,int y1,int x2,int y2);
#endif // GOTOXY_H_INCLUDED
