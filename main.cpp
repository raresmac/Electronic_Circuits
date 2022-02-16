#include <iostream>
#include <fstream>
#include <graphics.h>
#include <winbgim.h>
#include <windows.h>
#include <cmath>
#include <string>
#include <cstdlib>

using namespace std;

#define PI 3.14159265358979323846264338327950288

//ultimul buton apasat
int buton=0;
int main_window,input_window;

float rotire=0*PI/180;

//coordonatele ferestrelor
const int width=1500,height=800;
const int xlungbut=width/10,xlungspat=width/150+4,ylungbut1=0.125*height,ylungbut2=0.0928*height,ylungspat=0.0124*height;

//coordonatele butoanelor
int xspatiu1=0.119*width,yspatiu1=0.15*height,xspatiu2=0.8747*width,yspatiu2=0.979*height;//spatiul de lucru
int xsalveaza1,ysalveaza1,xsalveaza2,ysalveaza2;//SAVE
int xincarca1,yincarca1,xincarca2,yincarca2;//INCARCA
int xreset1,yreset1,xreset2,yreset2;//RESETARE
int xhelp1,yhelp1,xhelp2,yhelp2;//HELP
int xundo1,yundo1,xundo2,yundo2;//UNDO
int xredo1,yredo1,xredo2,yredo2;//REDO
int xedit1,yedit1,xedit2,yedit2;//EDIT
int xdelete1,ydelete1,xdelete2,ydelete2;//DELETE
int xtranzistor1,ytranzistor1,xtranzistor2,ytranzistor2;//TRANZISTOR
int xdioda1,ydioda1,xdioda2,ydioda2;//DIODA
int xbaterie1,ybaterie1,xbaterie2,ybaterie2;//BATERIE
int xsiguranta1,ysiguranta1,xsiguranta2,ysiguranta2;//SIGURANTA
int xcondensator1,ycondensator1,xcondensator2,ycondensator2;//CONDENSATOR
int xrezistenta1,yrezistenta1,xrezistenta2,yrezistenta2;//REZISTENTA
int xbec1,ybec1,xbec2,ybec2;//BEC
int xampermetru1,yampermetru1,xampermetru2,yampermetru2;//AMPERMETRU
int xvoltmetru1,yvoltmetru1,xvoltmetru2,yvoltmetru2;//VOLTMETRU
int xlegatura1,ylegatura1,xlegatura2,ylegatura2;//LEGATURA
int xnod1,ynod1,xnod2,ynod2;//NOD
int xzoomin1,yzoomin1,xzoomin2,yzoomin2;//ZOOM IN
int xzoomout1,yzoomout1,xzoomout2,yzoomout2;//ZOOM OUT
int xrotire1,yrotire1,xrotire2,yrotire2;//ROTIRE
int xslider1,xslider2,yslider1,yslider2;//SLIDER ROTIRE
int xsliderline1,ysliderline1,xsliderline2,ysliderline2; //BARA SLIDER ROTIRE
int xmove1,ymove1,xmove2,ymove2;//MOVE
int xinfo1,yinfo1,xinfo2,yinfo2;//PROPRIETATI
int xexit1,yexit1,xexit2,yexit2;//EXIT

struct punct{
    double x,y;
}mouse;//coordonatele mouse-ului

struct punctLegatura{
    int piesa;
    int nod;
};

struct legatura{
    int piesa1,piesa2;
    int nod1,nod2;
}vectorLegaturi[201];
int nrLegaturi=0;
int legx1[101],legy1[101],legx2[101],legy2[101];

struct piesa{
    int tip_piesa;
    long long valoare;
    double xmij,ymij;//coordonatele mijlocului
    double raza;//raza
    double x1,y1,x2,y2;//colturile dreptunghiului componentei
    unsigned orientare;
    float rotire=0*PI/180;
    int nrNod;
    punct noduri[5];
}vectorPiese[1001];
int nrPiese=0;

struct undo{
    int tip;//1-piesa ; 2-legatura
    piesa P;
    legatura L;
}vectorUndo[1001];
int nrUndo=0;

struct redo{
    int tip;//1-piesa ; 2-legatura
    piesa P;
    legatura L;
}vectorRedo[1001];
int nrRedo=0;

void actualizeazaColturi(piesa &P){
    int tip_piesa=P.tip_piesa, xmij=P.xmij, ymij=P.ymij, raza=P.raza;
    float sinrotire=sin(P.rotire);
    sinrotire*=1000000;
    sinrotire=int(sinrotire);
    sinrotire=sinrotire*1./1000000;
    float cosrotire=cos(P.rotire);
    cosrotire*=1000000;
    cosrotire=int(cosrotire);
    cosrotire=cosrotire*1./1000000;
    double sinr=sinrotire*P.raza;
    double cosr=cosrotire*P.raza;
    switch(tip_piesa){
        case 1:{//tranzistor
            P.x1=xmij-6*raza-2;
            P.y1=ymij-6*raza-2;
            P.x2=xmij+6*raza+2;
            P.y2=ymij+6*raza+2;
            /*P.x1=xmij-6*abs(cosr)-5*abs(sinr)+2*cosr+2*sinr-7;
            P.y1=ymij-5*abs(cosr)-6*abs(sinr)-7;
            P.x2=xmij+4*abs(cosr)+5*abs(sinr)+7;
            P.y2=ymij+5*abs(cosr)+4*abs(sinr)+7;*/
        } break;
        case 2:{//dioda
            P.x1=xmij-3*raza-2;
            P.y1=ymij-3*raza-2;
            P.x2=xmij+3*raza+2;
            P.y2=ymij+3*raza+2;
        } break;
        case 3:{//baterie
            P.x1=xmij-4*raza-2;
            P.y1=ymij-4*raza-2;
            P.x2=xmij+4*raza+2;
            P.y2=ymij+4*raza+2;
        } break;
        case 4:{//siguranta
            P.x1=xmij-5*raza-2;
            P.y1=ymij-5*raza-2;
            P.x2=xmij+5*raza+2;
            P.y2=ymij+5*raza+2;
        } break;
        case 5:{//condensator
            P.x1=xmij-2*raza-2;
            P.y1=ymij-2*raza-2;
            P.x2=xmij+2*raza+2;
            P.y2=ymij+2*raza+2;
        } break;
        case 6:{//rezistenta
            P.x1=xmij-5*raza-2;
            P.y1=ymij-5*raza-2;
            P.x2=xmij+5*raza+2;
            P.y2=ymij+5*raza+2;
        } break;
        case 7:{//bec
            P.x1=xmij-3*raza-2;
            P.y1=ymij-3*raza-2;
            P.x2=xmij+3*raza+2;
            P.y2=ymij+3*raza+2;
        } break;
        case 8:{//ampermetru
            P.x1=xmij-3*raza-2;
            P.y1=ymij-3*raza-2;
            P.x2=xmij+3*raza+2;
            P.y2=ymij+3*raza+2;
        }break;
        case 9:{//voltmetru
            P.x1=xmij-3*raza-2;
            P.y1=ymij-3*raza-2;
            P.x2=xmij+3*raza+2;
            P.y2=ymij+3*raza+2;
        } break;
        case 10:{//nod
            P.x1=xmij-2*raza;
            P.y1=ymij-2*raza;
            P.x2=xmij+2*raza;
            P.y2=ymij+2*raza;
        } break;
    }
}

void actualizeazaNoduri(piesa &P){
    float sinrotire=sin(P.rotire);
    sinrotire*=1000000;
    sinrotire=int(sinrotire);
    sinrotire=sinrotire*1./1000000;
    float cosrotire=cos(P.rotire);
    cosrotire*=1000000;
    cosrotire=int(cosrotire);
    cosrotire=cosrotire*1./1000000;
    double sinr=sinrotire*P.raza;
    double cosr=cosrotire*P.raza;
    setcolor(LIGHTGRAY);
    switch(P.tip_piesa){
        case 1:{//tranzistor
        P.nrNod=3;
        P.noduri[1].x=P.xmij-6*cosr,P.noduri[1].y=P.ymij-6*sinr;
        P.noduri[2].x=P.xmij+5*sinr,P.noduri[2].y=P.ymij-5*cosr;
        P.noduri[3].x=P.xmij-5*sinr,P.noduri[3].y=P.ymij+5*cosr;
    } break;
        case 2:{//dioda
        P.nrNod=2;
        P.noduri[1].x=P.xmij-3*cosr,P.noduri[1].y=P.ymij-3*sinr;
        P.noduri[2].x=P.xmij+3*cosr,P.noduri[2].y=P.ymij+3*sinr;
    } break;
        case 3:{//baterie
        P.nrNod=2;
        P.noduri[1].x=P.xmij-4*cosr,P.noduri[1].y=P.ymij-4*sinr;
        P.noduri[2].x=P.xmij+4*cosr,P.noduri[2].y=P.ymij+4*sinr;
    } break;
        case 4:{//siguranta
        P.nrNod=2;
        P.noduri[1].x=P.xmij-5*cosr,P.noduri[1].y=P.ymij-5*sinr;
        P.noduri[2].x=P.xmij+5*cosr,P.noduri[2].y=P.ymij+5*sinr;
    } break;
        case 5:{//condensator
        P.nrNod=2;
        P.noduri[1].x=P.xmij-2*cosr,P.noduri[1].y=P.ymij-2*sinr;
        P.noduri[2].x=P.xmij+2*cosr,P.noduri[2].y=P.ymij+2*sinr;
    } break;
        case 6:{//rezistenta
        P.nrNod=2;
        P.noduri[1].x=P.xmij-5*cosr,P.noduri[1].y=P.ymij-5*sinr;
        P.noduri[2].x=P.xmij+5*cosr,P.noduri[2].y=P.ymij+5*sinr;
    } break;
        case 7:{//bec
        P.nrNod=2;
        P.noduri[1].x=P.xmij-3*cosr,P.noduri[1].y=P.ymij-3*sinr;
        P.noduri[2].x=P.xmij+3*cosr,P.noduri[2].y=P.ymij+3*sinr;
    } break;
        case 8:{//ampermetru
        P.nrNod=2;
        P.noduri[1].x=P.xmij-3*cosr,P.noduri[1].y=P.ymij-3*sinr;
        P.noduri[2].x=P.xmij+3*cosr,P.noduri[2].y=P.ymij+3*sinr;
    } break;
        case 9:{//voltmetru
        P.nrNod=2;
        P.noduri[1].x=P.xmij-3*cosr,P.noduri[1].y=P.ymij-3*sinr;
        P.noduri[2].x=P.xmij+3*cosr,P.noduri[2].y=P.ymij+3*sinr;
    } break;
        case 10:{//nod
        P.nrNod=1;
        P.noduri[1].x=P.xmij,P.noduri[1].y=P.ymij;
    } break;
    }
}

void actualizeazaRotire(piesa P){
    //linie de 90 de pixeli => un pixel = 4 grade
    setfillstyle(1,RED); setbkcolor(RED);
    bar(xsliderline1-1,yslider1-1,xsliderline2+1,yslider2+1);
    line(xsliderline1,ysliderline1,xsliderline2,ysliderline2);
    xslider1=xsliderline1+P.rotire*45/PI;
    xslider2=xslider1+1;
    setfillstyle(1,WHITE); setbkcolor(WHITE);
    bar(xslider1-1,yslider1,xslider2,yslider2);
}

void deseneazaPiesa(piesa P){
    setcolor(WHITE);
    int tip_piesa=P.tip_piesa, x=P.xmij, y=P.ymij, raza=P.raza;
    float sinrotire=sin(P.rotire);
    sinrotire*=1000000;
    sinrotire=int(sinrotire);
    sinrotire=sinrotire*1./1000000;
    float cosrotire=cos(P.rotire);
    cosrotire*=1000000;
    cosrotire=int(cosrotire);
    cosrotire=cosrotire*1./1000000;
    double sinr=sinrotire*raza;
    double cosr=cosrotire*raza;
    switch(tip_piesa){
        case 1:{//tranzistor
            circle(x,y,4*raza);
            line(x-6*cosr,y-6*sinr,x-2*cosr,y-2*sinr);
            line(x-2*sinr-2*cosr,y-2*sinr+2*cosr,x+2*sinr-2*cosr,y-2*sinr-2*cosr);
            line(x-2*cosr+sinr,y-2*sinr-cosr,x+3*sinr,y-3*cosr);
            line(x-2*cosr-sinr,y-2*sinr+cosr,x-3*sinr,y+3*cosr);
            line(x+3*sinr,y-3*cosr,x+5*sinr,y-5*cosr);
            line(x-3*sinr,y+3*cosr,x-5*sinr,y+5*cosr);
        } break;
        case 2:{//dioda
            line(x-3*cosr,y-3*sinr,x-cosr,y-sinr);
            line(x+cosr,y+sinr,x+3*cosr,y+3*sinr);
            line(x-sinr-cosr,y-sinr+cosr,x+sinr-cosr,y-sinr-cosr);
            line(x+sinr-cosr,y-sinr-cosr,x+cosr,y+sinr);
            line(x-sinr-cosr,y-sinr+cosr,x+cosr,y+sinr);
            line(x-sinr+cosr,y+sinr+cosr,x+sinr+cosr,y+sinr-cosr);
        } break;
        case 3:{//baterie
            line(x-4*cosr,y-4*sinr,x-cosr,y-sinr);
            line(x,y,x+4*cosr,y+4*sinr);
            line(x-sinr-cosr,y-sinr+cosr,x+sinr-cosr,y-sinr-cosr);
            line(x-2*sinr,y+2*cosr,x+2*sinr,y-2*cosr);
        } break;
        case 4:{//siguranta
            line(x-5*cosr,y-5*sinr,x+5*cosr,y+5*sinr);
            line(x-3*cosr+sinr,y-3*sinr-cosr,x-3*cosr-sinr,y-3*sinr+cosr);
            line(x+3*cosr+sinr,y+3*sinr-cosr,x+3*cosr-sinr,y+3*sinr+cosr);
            line(x-3*cosr+sinr,y-3*sinr-cosr,x+3*cosr+sinr,y+3*sinr-cosr);
            line(x-3*cosr-sinr,y-3*sinr+cosr,x+3*cosr-sinr,y+3*sinr+cosr);
        } break;
        case 5:{//condensator
            line(x-2*cosr,y-2*sinr,x-cosr,y-sinr);
            line(x+2*cosr,y+2*sinr,x+cosr,y+sinr);
            line(x-sinr-cosr,y-sinr+cosr,x+sinr-cosr,y-sinr-cosr);
            line(x-sinr+cosr,y+sinr+cosr,x+sinr+cosr,y+sinr-cosr);
        } break;
        case 6:{//rezistenta
            line(x-5*cosr,y-5*sinr,x-3*cosr,y-3*sinr);
            line(x+3*cosr,y+3*sinr,x+5*cosr,y+5*sinr);
            line(x-3*cosr+sinr,y-3*sinr-cosr,x-3*cosr-sinr,y-3*sinr+cosr);
            line(x+3*cosr+sinr,y+3*sinr-cosr,x+3*cosr-sinr,y+3*sinr+cosr);
            line(x-3*cosr+sinr,y-3*sinr-cosr,x+3*cosr+sinr,y+3*sinr-cosr);
            line(x-3*cosr-sinr,y-3*sinr+cosr,x+3*cosr-sinr,y+3*sinr+cosr);
        } break;
        case 7:{//bec
            double liniecerc=0.692;
            circle(x,y,raza);
            line(x-3*cosr,y-3*sinr,x-cosr,y-sinr);
            line(x+cosr,y+sinr,x+3*cosr,y+3*sinr);
            cosr*=liniecerc;
            sinr*=liniecerc;
            line(x-sinr-cosr,y-cosr+sinr,x+cosr+sinr,y+cosr-sinr);
            line(x-cosr+sinr,y+cosr+sinr,x+cosr-sinr,y-cosr-sinr);
        } break;
        case 8:{//ampermetru
            circle(x,y,raza);
            line(x-3*cosr,y-3*sinr,x-cosr,y-sinr);
            line(x+cosr,y+sinr,x+3*cosr,y+3*sinr);
            line(x-cosr/2-sinr/2,y+cosr/2-sinr/2,x+sinr/2,y-cosr/2);
            line(x+sinr/2,y-cosr/2,x+cosr/2-sinr/2,y+cosr/2+sinr/2);
            line(x-cosr/2,y-sinr/2,x+cosr/2,y+sinr/2);
        } break;
        case 9:{//voltmetru
            circle(x,y,raza);
            line(x-3*cosr,y-3*sinr,x-cosr,y-sinr);
            line(x+cosr,y+sinr,x+3*cosr,y+3*sinr);
            line(x-cosr/2+sinr/2,y-cosr/2-sinr/2,x-sinr/2,y+cosr/2);
            line(x+cosr/2+sinr/2,y-cosr/2+sinr/2,x-sinr/2,y+cosr/2);
        } break;
    }
    for(int i=1;i<=P.nrNod;i++)
        circle(P.noduri[i].x,P.noduri[i].y,4);
}

void SelectareFigura(piesa P){
    int x1=P.x1,y1=P.y1,x2=P.x2,y2=P.y2;
    setcolor(RED);
    rectangle(x1,y1,x2,y2);
    setcolor(WHITE);
    setfillstyle(1,COLOR(60,60,60)); setbkcolor(COLOR(60,60,60));
    bar(xinfo1,yinfo1,xinfo2,yinfo2);
    //caracteristica
    settextstyle(3,HORIZ_DIR,1);
    switch(P.tip_piesa){
        case 1:{
            outtextxy(width*0.89,height*0.808,"Current Transfer");
        }break;
        case 2:{
            outtextxy(width*0.89,height*0.808,"Voltage Drop");
        }break;
        case 3:{
            outtextxy(width*0.89,height*0.808,"Voltage");
        }break;
        case 4:{
            outtextxy(width*0.89,height*0.808,"Current Rating");
        }break;
        case 5:{
            outtextxy(width*0.89,height*0.808,"Capacitance");
        }break;
        case 6:{
            outtextxy(width*0.89,height*0.808,"Resistance");
        }break;
        case 7:{
            outtextxy(width*0.89,height*0.808,"Voltage");
        }break;
        case 8:{
            outtextxy(width*0.89,height*0.808,"Nominal Current");
        }break;
        case 9:{
            outtextxy(width*0.89,height*0.808,"Voltage");
        }break;
    }

    //numar
    if(P.tip_piesa<10){
        settextstyle(3,HORIZ_DIR,2);
        string tmp = to_string(P.valoare);
        char const *num_char = tmp.c_str();
        char c[21];
        strcpy(c,num_char);
        outtextxy(width*0.89,height*0.836,c);
    }

    //marime fizica
    switch(P.tip_piesa){
        case 1:{
            outtextxy(width*0.89,height*0.865,"-constant-");
        }break;
        case 2:{
            outtextxy(width*0.89,height*0.865,"Volts");
        }break;
        case 3:{
            outtextxy(width*0.89,height*0.865,"Volts");
        }break;
        case 4:{
            outtextxy(width*0.89,height*0.865,"Amperes");
        }break;
        case 5:{
            outtextxy(width*0.89,height*0.865,"Farads");
        }break;
        case 6:{
            outtextxy(width*0.89,height*0.865,"Ohms");
        }break;
        case 7:{
            outtextxy(width*0.89,height*0.865,"Volts");
        }break;
        case 8:{
            outtextxy(width*0.89,height*0.865,"Amperes");
        }break;
        case 9:{
            outtextxy(width*0.89,height*0.865,"Volts");
        }break;
    }

    //rotire
    actualizeazaRotire(P);
}

void DeselectareFigura(double x1,double y1,double x2,double y2){
    setcolor(BLACK);
    rectangle(x1,y1,x2,y2);
}

bool InFereastra(int x,int y){
    if (x>xspatiu1 && x<xspatiu2 && y>yspatiu1 && y<yspatiu2)
        return true;
    return false;
}

bool IncapeFereastra(int x1,int y1,int x2,int y2){
    if(x1>xspatiu1 && x1<xspatiu2 && y1>yspatiu1 && y1<yspatiu2)
        return (x2>xspatiu1 && x2<xspatiu2 && y2>yspatiu1 && y2<yspatiu2);
    return 0;
}

void drawFereastraLucru(){
    int x1=xlungspat,x2=xlungspat+xlungbut,y1=ylungspat,y2=ylungspat+ylungbut1;
    setbkcolor(LIGHTGRAY); cleardevice(); setfillstyle(1,BLACK);
    //spatiul efectiv de desenare
    bar(xspatiu1,yspatiu1,xspatiu2,yspatiu2);
    setfillstyle(1,DARKGRAY); setbkcolor(DARKGRAY); settextstyle(3,HORIZ_DIR,1);
    vectorPiese[0].rotire=0;
//----------------------------
    {//tranzistor
    setcolor(WHITE);
    xtranzistor1=x1,xtranzistor2=x2,ytranzistor1=y1,ytranzistor2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.025*width,ytranzistor1+height/100,"TRANSISTOR");
    vectorPiese[0].tip_piesa=1;
    vectorPiese[0].raza=5;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=ytranzistor1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//dioda
    setcolor(WHITE);
    xdioda1=x1,xdioda2=x2,ydioda1=y1,ydioda2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.153*width,ydioda1+height/100,"DIODE");
    vectorPiese[0].tip_piesa=2;
    vectorPiese[0].raza=16;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=ytranzistor1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//baterie
    setcolor(WHITE);
    xbaterie1=x1,xbaterie2=x2,ybaterie1=y1,ybaterie2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.253*width,ybaterie1+height/100,"BATTERY");
    vectorPiese[0].tip_piesa=3;
    vectorPiese[0].raza=11;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=ybaterie1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//siguranta
    setcolor(WHITE);
    xsiguranta1=x1,xsiguranta2=x2,ysiguranta1=y1,ysiguranta2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.374*width,ysiguranta1+height/100,"FUSE");
    vectorPiese[0].tip_piesa=4;
    vectorPiese[0].raza=11;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=ysiguranta1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//condensator
    setcolor(WHITE);
    xcondensator1=x1,xcondensator2=x2,ycondensator1=y1,ycondensator2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.467*width,ycondensator1+height/100,"CAPACITOR");
    vectorPiese[0].tip_piesa=5;
    vectorPiese[0].raza=20;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=ycondensator1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//rezistenta
    setcolor(WHITE);
    xrezistenta1=x1,xrezistenta2=x2,yrezistenta1=y1,yrezistenta2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.58*width,yrezistenta1+height/100,"RESISTOR");
    vectorPiese[0].tip_piesa=6;
    vectorPiese[0].raza=11;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=yrezistenta1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//bec
    setcolor(WHITE);
    xbec1=x1,xbec2=x2,ybec1=y1,ybec2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.7*width,ybec1+height/100,"BULB");
    vectorPiese[0].tip_piesa=7;
    vectorPiese[0].raza=19;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=ybec1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//ampermetru
    setcolor(WHITE);
    xampermetru1=x1,xampermetru2=x2,yampermetru1=y1,yampermetru2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.8*width,yampermetru1+height/100,"AMMETER");
    vectorPiese[0].tip_piesa=8;
    vectorPiese[0].raza=19;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=yampermetru1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    x1+=xlungspat+xlungbut;
    x2+=xlungspat+xlungbut;
    }
    {//voltmetru
    setcolor(WHITE);
    xvoltmetru1=x1,xvoltmetru2=x2,yvoltmetru1=y1,yvoltmetru2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.903*width,yvoltmetru1+height/100,"VOLTMETER");
    vectorPiese[0].tip_piesa=9;
    vectorPiese[0].raza=19;
    vectorPiese[0].xmij=(x1+x2)/2;
    vectorPiese[0].ymij=yvoltmetru1+height/12;
    deseneazaPiesa(vectorPiese[0]);
    y1+=ylungspat+ylungbut1;
    y2+=ylungspat+ylungbut2;
    }
    {//nod
    xnod1=x1,xnod2=x2,ynod1=y1,ynod2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.921*width,ynod1+height/30,"NODE");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//legatura
    xlegatura1=x1,xlegatura2=x2,ylegatura1=y1,ylegatura2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.925*width,ylegatura1+height/30,"LINK");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
//----------------------------
    setfillstyle(1,RED); setbkcolor(RED);
    {//zoom in
    xzoomin1=x1,xzoomin2=x2,yzoomin1=y1,yzoomin2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.912*width,yzoomin1+height/30,"ZOOM IN");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//zoomout
    xzoomout1=x1,xzoomout2=x2,yzoomout1=y1,yzoomout2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.906*width,yzoomout1+height/30,"ZOOM OUT");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//rotire
    xrotire1=x1,xrotire2=x2,yrotire1=y1,yrotire2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.914*width,yrotire1+height/80,"ROTATE");
    xsliderline1=x1+(x2-x1)/5,ysliderline1=(y1+y2)/2+height/50,xsliderline2=x1+(x2-x1)*4/5,ysliderline2=(y1+y2)/2+height/50;
    setfillstyle(1,WHITE); setbkcolor(WHITE);
    line(xsliderline1,ysliderline1,xsliderline2,ysliderline2);
    xslider1=xsliderline1-1,xslider2=xsliderline1+1;
    yslider1=ysliderline1-(y2-y1)/6,yslider2=ysliderline2+(y2-y1)/6;
    bar(xslider1,yslider1,xslider2,yslider2);
    setfillstyle(1,RED); setbkcolor(RED);
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//move
    xmove1=x1,xmove2=x2,ymove1=y1,ymove2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.919*width,ymove1+height/30,"MOVE");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//proprietati
    y2+=ylungspat+ylungbut2-50;
    xinfo1=x1,xinfo2=x2,yinfo1=y1,yinfo2=y2;
    setfillstyle(1,COLOR(60,60,60)); setbkcolor(COLOR(60,60,60));
    bar(x1,y1,x2,y2); outtextxy(0.9*width,yinfo1+height/150,"PROPERTIES");
    yinfo1+=height/30;
    line(xinfo1,yinfo1,xinfo2,yinfo1);
    yinfo1++;
    y1+=ylungspat+ylungbut2+30;
    }
    {//exit
    y2=yspatiu2;
    xexit1=x1,xexit2=x2,yexit1=y1,yexit2=y2;
    setfillstyle(1,COLOR(60,0,0)); setbkcolor(COLOR(60,0,0));
    bar(x1,y1,x2,y2);
    outtextxy(0.922*width,yexit1+height/55,"EXIT");
    }
//-----------------------------
    setfillstyle(1,CYAN); setbkcolor(CYAN);
    x1=xtranzistor1,x2=xtranzistor2,y1=ynod1,y2=y1+ylungbut2;
    {//undo
    xundo1=x1,xundo2=x2,yundo1=y1,yundo2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.045*width,y1+width/60,"UNDO");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//redo
    xredo1=x1,xredo2=x2,yredo1=y1,yredo2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.045*width,y1+width/60,"REDO");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//edit
    xedit1=x1,xedit2=x2,yedit1=y1,yedit2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.048*width,y1+width/60,"EDIT");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//delete
    xdelete1=x1,xdelete2=x2,ydelete1=y1,ydelete2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.04*width,y1+width/60,"DELETE");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//salvare
    xsalveaza1=x1,xsalveaza2=x2,ysalveaza1=y1,ysalveaza2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.047*width,y1+width/60,"SAVE");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//incarca
    xincarca1=x1,xincarca2=x2,yincarca1=y1,yincarca2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.045*width,y1+width/60,"LOAD");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//reset
    xreset1=x1,xreset2=x2,yreset1=y1,yreset2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.042*width,y1+width/60,"RESET");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
    {//help
    y2=yspatiu2;
    xhelp1=x1,xhelp2=x2,yhelp1=y1,yhelp2=y2;
    bar(x1,y1,x2,y2); outtextxy(0.046*width,y1+width/50,"HELP");
    y1+=ylungspat+ylungbut2;
    y2+=ylungspat+ylungbut2;
    }
}

bool insideRectangle(double x,double y,double x1,double y1,double x2,double y2){
    return x>=x1-1 && x<=x2+1 && y>=y1-1 && y<=y2+1;
}

bool desenarePesteAltaPiesa(double x1,double y1,double x2,double y2,int indexPiesa){
    for(int i=1;i<=nrPiese;i++){
        if(indexPiesa==i) continue;
        if(insideRectangle(x1,y1,vectorPiese[i].x1,vectorPiese[i].y1,vectorPiese[i].x2,vectorPiese[i].y2)) return 1;
        if(insideRectangle(x2,y2,vectorPiese[i].x1,vectorPiese[i].y1,vectorPiese[i].x2,vectorPiese[i].y2)) return 1;
        if(insideRectangle(x1,y2,vectorPiese[i].x1,vectorPiese[i].y1,vectorPiese[i].x2,vectorPiese[i].y2)) return 1;
        if(insideRectangle(x2,y1,vectorPiese[i].x1,vectorPiese[i].y1,vectorPiese[i].x2,vectorPiese[i].y2)) return 1;
        if(insideRectangle(vectorPiese[i].x1,vectorPiese[i].y1,x1,y1,x2,y2)) return 1;
        if(insideRectangle(vectorPiese[i].x2,vectorPiese[i].y1,x1,y1,x2,y2)) return 1;
        if(insideRectangle(vectorPiese[i].x1,vectorPiese[i].y2,x1,y1,x2,y2)) return 1;
        if(insideRectangle(vectorPiese[i].x2,vectorPiese[i].y2,x1,y1,x2,y2)) return 1;
    }
    return 0;
}

void resetareCircuit(){
    setfillstyle(SOLID_FILL,BLACK);
    bar(xspatiu1,yspatiu1,xspatiu2,yspatiu2);
    nrPiese=0;
    nrLegaturi=0;
}

void new_x_y_fromFereastra(int &x,int &y){
    if(x>xspatiu2) x=xspatiu2-1;
    else if(x<xspatiu1) x=xspatiu1;
    if(y>yspatiu2) y=yspatiu2-1;
    else if(y<yspatiu1) y=yspatiu1;
}

void drawLine(int x1,int y1,int x2,int y2,int cul){
    setcolor(cul);
    if(y1<y2) swap(x1,x2),swap(y1,y2);
    line(x1,y1,x1,(y1+y2)/2),line(x1,(y1+y2)/2,x2,(y1+y2)/2),line(x2,(y1+y2)/2,x2,y2);
    //line(x1,y1,x1,(2*y1+y2)/3); line(x1,(2*y1+y2)/3,(x1+x2)/2,(2*y1+y2)/3); line((x1+x2)/2,(2*y1+y2)/3,(x1+x2)/2,(y1+2*y2)/3); line((x1+x2)/2,(y1+2*y2)/3,x2,(y1+2*y2)/3); line(x2,(y1+2*y2)/3,x2,y2);
}

int gaseste_piesa(int x,int y){
    for(int i=1;i<=nrPiese;i++)
        if(insideRectangle(x,y,vectorPiese[i].x1,vectorPiese[i].y1,vectorPiese[i].x2,vectorPiese[i].y2))
           return i;
    return 0;
}

void actualizare_coord_leg(int x1,int y1,int x2,int y2,int &nrcoord){
    line(x1,y1,x2,y2);
    legx1[nrcoord]=x1;
    legy1[nrcoord]=y1;
    legx2[nrcoord]=x2;
    legy2[nrcoord++]=y2;
}

void DesenareLegatura(int x1,int y1,int x2,int y2,int piesa1,int &nrcoord){
    //linia 1
    bool ok=0;
    int yMouse=y2,xMouse=x2;
    int x1c=x1,y1c=y1,x2c=x1,y2c=(y1+yMouse)/2;
    setcolor(LIGHTGRAY);
    //trec prin piesa initiala
    {/*if(x1c>vectorPiese[piesa1].x1 && x1c<vectorPiese[piesa1].x2){
        //de sus in jos
        if(y1c<y2c && y2c>vectorPiese[piesa1].y2){
            //drum in dreapta
            if(x2>x1c){
                actualizare_coord_leg(x1c,y1c,vectorPiese[piesa1].x2+1,y1c,nrcoord);
                //raman in piesa
                if(y2c<vectorPiese[piesa1].y2+1){
                    actualizare_coord_leg(vectorPiese[piesa1].x2+1,y1c,vectorPiese[piesa1].x2+1,y2c,nrcoord);
                    x1c=vectorPiese[piesa1].x2+1;
                    y1c=y2c;
                }
                //trec de piesa
                else{
                    actualizare_coord_leg(vectorPiese[piesa1].x2+1,y1c,vectorPiese[piesa1].x2+1,vectorPiese[piesa1].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[piesa1].x2+1,vectorPiese[piesa1].y2+1,x1c,vectorPiese[piesa1].y2+1,nrcoord);
                    y1c=vectorPiese[piesa1].y2+1;
                }
            }
            //drum in stanga
            else{
                actualizare_coord_leg(x1c,y1c,vectorPiese[piesa1].x1-1,y1c,nrcoord);
                //raman in piesa
                if(y2c<vectorPiese[piesa1].y2+1){
                    actualizare_coord_leg(vectorPiese[piesa1].x1-1,y1c,vectorPiese[piesa1].x1-1,y2c,nrcoord);
                    x1c=vectorPiese[piesa1].x1-1;
                    y1c=y2c;
                }
                //trec de piesa
                else{
                    actualizare_coord_leg(vectorPiese[piesa1].x1-1,y1c,vectorPiese[piesa1].x1-1,vectorPiese[piesa1].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[piesa1].x1-1,vectorPiese[piesa1].y2+1,x1c,vectorPiese[piesa1].y2+1,nrcoord);
                    y1c=vectorPiese[piesa1].y2+1;
                }
            }
        }
        //de jos in sus
        else if(y1c>y2c && y2c<vectorPiese[piesa1].y1){
            //drum in dreapta
            if(x2>x1c){
                actualizare_coord_leg(x1c,y1c,vectorPiese[piesa1].x2+1,y1c,nrcoord);
                //raman in piesa
                if(y2c<vectorPiese[piesa1].y1-1){
                    actualizare_coord_leg(vectorPiese[piesa1].x2+1,y1c,vectorPiese[piesa1].x2+1,y2c,nrcoord);
                    x1c=vectorPiese[piesa1].x2+1;
                    y1c=y2c;
                }
                //trec de piesa
                else{
                    actualizare_coord_leg(vectorPiese[piesa1].x2+1,y1c,vectorPiese[piesa1].x2+1,vectorPiese[piesa1].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[piesa1].x2+1,vectorPiese[piesa1].y1-1,x1c,vectorPiese[piesa1].y1-1,nrcoord);
                    y1c=vectorPiese[piesa1].y1-1;
                }
            }
            //drum in stanga
            else{
                actualizare_coord_leg(x1c,y1c,vectorPiese[piesa1].x1-1,y1c,nrcoord);
                //raman in piesa
                if(y2c<vectorPiese[piesa1].y1-1){
                    actualizare_coord_leg(vectorPiese[piesa1].x1-1,y1c,vectorPiese[piesa1].x1-1,y2c,nrcoord);
                    x1c=vectorPiese[piesa1].x1-1;
                    y1c=y2c;
                }
                //trec de piesa
                else{
                    actualizare_coord_leg(vectorPiese[piesa1].x1-1,y1c,vectorPiese[piesa1].x1-1,vectorPiese[piesa1].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[piesa1].x1-1,vectorPiese[piesa1].y1-1,x1c,vectorPiese[piesa1].y1-1,nrcoord);
                    y1c=vectorPiese[piesa1].y1-1;
                }
            }
        }
    }*/}
    for(int i=1;i<=nrPiese;i++){
        if(piesa1==i) continue;
        if(x1c>=vectorPiese[i].x1 && x1c<=vectorPiese[i].x2){
            //trece peste piesa
            //de sus in jos
            if(y1c<=vectorPiese[i].y1 && y2c>=vectorPiese[i].y2){
                actualizare_coord_leg(x1c,y1c,x1c,vectorPiese[i].y1-1,nrcoord);
                //la dreapta de mijloc
                if(x2>vectorPiese[i].xmij){
                    actualizare_coord_leg(x1c,vectorPiese[i].y1-1,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,x1c,vectorPiese[i].y2+1,nrcoord);
                }
                //la stanga de mijloc
                else{
                    actualizare_coord_leg(x1c,vectorPiese[i].y1-1,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,x1c,vectorPiese[i].y2+1,nrcoord);
                }
                y1c=vectorPiese[i].y2+1;
            }
            //de jos in sus
            else if(y1c>=vectorPiese[i].y2 && y2c<=vectorPiese[i].y1){
                actualizare_coord_leg(x1c,y1c,x1c,vectorPiese[i].y2+1,nrcoord);
                //la dreapta de mijloc
                if(x2>vectorPiese[i].xmij){
                    actualizare_coord_leg(x1c,vectorPiese[i].y2+1,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,x1c,vectorPiese[i].y1-1,nrcoord);
                }
                //la stanga de mijloc
                else{
                    actualizare_coord_leg(x1c,vectorPiese[i].y2+1,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,x1c,vectorPiese[i].y1-1,nrcoord);
                }
                y1c=vectorPiese[i].y1-1;
            }
            //se termina in piesa
            else if(y2c<=vectorPiese[i].y2 && y2c>=vectorPiese[i].y1){
                ok=1;
                //de sus in jos
                if(y1c<vectorPiese[i].y1){
                    actualizare_coord_leg(x1c,y1c,x1c,vectorPiese[i].y1-1,nrcoord);
                    //la dreapta de mijloc
                    if(x2>vectorPiese[i].xmij){
                        actualizare_coord_leg(x1c,vectorPiese[i].y1-1,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,vectorPiese[i].x2+1,y2c,nrcoord);
                        x1c=vectorPiese[i].x2+1;
                    }
                    //la stanga de mijloc
                    else{
                        actualizare_coord_leg(x1c,vectorPiese[i].y1-1,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,vectorPiese[i].x1-1,y2c,nrcoord);
                        x1c=vectorPiese[i].x1-1;
                    }
                }
                //de jos in sus
                else{
                    actualizare_coord_leg(x1c,y1c,x1c,vectorPiese[i].y2+1,nrcoord);
                    //la dreapta de mijloc
                    if(x2>vectorPiese[i].xmij){
                        actualizare_coord_leg(x1c,vectorPiese[i].y2+1,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,vectorPiese[i].x2+1,y2c,nrcoord);
                        x1c=vectorPiese[i].x2+1;
                    }
                    //la stanga de mijloc
                    else{
                        actualizare_coord_leg(x1c,vectorPiese[i].y2+1,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,vectorPiese[i].x1-1,y2c,nrcoord);
                        x1c=vectorPiese[i].x1-1;
                    }
                }
            }
        }
    }
    if(ok==0) actualizare_coord_leg(x1c,y1c,x2c,y2c,nrcoord);

    //linia 2
    ok=0;
    y1c=y2c,x2c=xMouse;
    setcolor(LIGHTGRAY);
    for(int i=1;i<=nrPiese;i++){
        if(y1c>vectorPiese[i].y1 && y1c<vectorPiese[i].y2){
            //trece peste piesa
            //de la stanga la dreapta
            if(x1c<=vectorPiese[i].x1 && x2c>=vectorPiese[i].x2){
                actualizare_coord_leg(x1c,y1c,vectorPiese[i].x1-1,y1c,nrcoord);
                //mai sus de centru
                if(y2<vectorPiese[i].ymij){
                    actualizare_coord_leg(vectorPiese[i].x1-1,y1c,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,vectorPiese[i].x2+1,y1c,nrcoord);
                }
                //mai jos de centru
                else{
                    actualizare_coord_leg(vectorPiese[i].x1-1,y1c,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,vectorPiese[i].x2+1,y1c,nrcoord);
                }
                x1c=vectorPiese[i].x2+1;
            }
            //de la dreapta la stanga
            else if(x1c>=vectorPiese[i].x2 && x2c<=vectorPiese[i].x1){
                actualizare_coord_leg(x1c,y1c,vectorPiese[i].x2+1,y1c,nrcoord);
                //mai sus de centru
                if(y2<vectorPiese[i].ymij){
                    actualizare_coord_leg(vectorPiese[i].x2+1,y1c,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,vectorPiese[i].x1-1,y1c,nrcoord);
                }
                //mai jos de centru
                else{
                    actualizare_coord_leg(vectorPiese[i].x2+1,y1c,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,vectorPiese[i].x1-1,y1c,nrcoord);
                }
                x1c=vectorPiese[i].x1-1;
            }
            //se termina in piesa
            if(x2c>=vectorPiese[i].x1 && x2c<=vectorPiese[i].x2){
                ok=1;
                //de la stanga la dreapta
                if(x1c<vectorPiese[i].x1){
                    actualizare_coord_leg(x1c,y1c,vectorPiese[i].x1-1,y1c,nrcoord);
                    //mai sus de centru
                    if(y2<vectorPiese[i].ymij){
                        actualizare_coord_leg(vectorPiese[i].x1-1,y1c,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,x2c,vectorPiese[i].y1-1,nrcoord);
                        y2c=vectorPiese[i].y1-1;
                    }
                    //mai jos de centru
                    else{
                        actualizare_coord_leg(vectorPiese[i].x1-1,y1c,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,x2c,vectorPiese[i].y2+1,nrcoord);
                        y2c=vectorPiese[i].y2+1;
                    }
                }
                //de la dreapta la stanga
                else if(x1c>vectorPiese[i].x2){
                    actualizare_coord_leg(x1c,y1c,vectorPiese[i].x2+1,y1c,nrcoord);
                    //mai sus de centru
                    if(y2<vectorPiese[i].ymij){
                        actualizare_coord_leg(vectorPiese[i].x2+1,y1c,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,x2c,vectorPiese[i].y1-1,nrcoord);
                        y2c=vectorPiese[i].y1-1;
                    }
                    //mai jos de centru
                    else{
                        actualizare_coord_leg(vectorPiese[i].x2+1,y1c,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                        actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,x2c,vectorPiese[i].y2+1,nrcoord);
                        y2c=vectorPiese[i].y2+1;
                    }
                }
            }
        }
    }
    if(ok==0) actualizare_coord_leg(x1c,y1c,x2c,y2c,nrcoord);

    //linia 3
    ok=0;
    x1c=x2c,x2c=xMouse,y1c=y2c,y2c=yMouse;
    setcolor(LIGHTGRAY);
    for(int i=1;i<=nrPiese;i++){
        if(x1c>vectorPiese[i].x1 && x1c<vectorPiese[i].x2){
            //trece peste piesa
            //de sus in jos
            if(y1c<=vectorPiese[i].y1 && y2c>=vectorPiese[i].y2){
                actualizare_coord_leg(x1c,y1c,x1c,vectorPiese[i].y1-1,nrcoord);
                //la dreapta de mijloc
                if(x2>vectorPiese[i].xmij){
                    actualizare_coord_leg(x1c,vectorPiese[i].y1-1,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,x1c,vectorPiese[i].y2+1,nrcoord);
                }
                //la stanga de mijloc
                else{
                    actualizare_coord_leg(x1c,vectorPiese[i].y1-1,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,x1c,vectorPiese[i].y2+1,nrcoord);
                }
                y1c=vectorPiese[i].y2+1;
            }
            //de jos in sus
            else if(y1c>=vectorPiese[i].y2 && y2c<=vectorPiese[i].y1){
                actualizare_coord_leg(x1c,y1c,x1c,vectorPiese[i].y2+1,nrcoord);
                //la dreapta de mijloc
                if(x2>vectorPiese[i].xmij){
                    actualizare_coord_leg(x1c,vectorPiese[i].y2+1,vectorPiese[i].x2+1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y2+1,vectorPiese[i].x2+1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x2+1,vectorPiese[i].y1-1,x1c,vectorPiese[i].y1-1,nrcoord);
                }
                //la stanga de mijloc
                else{
                    actualizare_coord_leg(x1c,vectorPiese[i].y2+1,vectorPiese[i].x1-1,vectorPiese[i].y2+1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y2+1,vectorPiese[i].x1-1,vectorPiese[i].y1-1,nrcoord);
                    actualizare_coord_leg(vectorPiese[i].x1-1,vectorPiese[i].y1-1,x1c,vectorPiese[i].y1-1,nrcoord);
                }
                y1c=vectorPiese[i].y1-1;
            }
        }
    }
    if(ok==0) actualizare_coord_leg(x1c,y1c,x2c,y2c,nrcoord);
}

void deseneazaTOT(){
    setfillstyle(SOLID_FILL,BLACK);
    bar(xspatiu1,yspatiu1,xspatiu2,yspatiu2);
    for(int i=1;i<=nrPiese;i++)
        deseneazaPiesa(vectorPiese[i]),actualizeazaNoduri(vectorPiese[i]);
    for(int i=1;i<=nrLegaturi;i++){
        int x1,x2,y1,y2;
        x1=vectorPiese[vectorLegaturi[i].piesa1].noduri[vectorLegaturi[i].nod1].x;
        y1=vectorPiese[vectorLegaturi[i].piesa1].noduri[vectorLegaturi[i].nod1].y;
        x2=vectorPiese[vectorLegaturi[i].piesa2].noduri[vectorLegaturi[i].nod2].x;
        y2=vectorPiese[vectorLegaturi[i].piesa2].noduri[vectorLegaturi[i].nod2].y;
        int s=0;
        DesenareLegatura(x1,y1,x2,y2,vectorLegaturi[i].piesa1,s);
    }
}

void selectareButon(int buton){
    setlinestyle(SOLID_LINE,0,1);
    if(buton==1)//tranzistor
        setcolor(YELLOW),rectangle(xtranzistor1,ytranzistor1,xtranzistor2,ytranzistor2);
    else
        setcolor(LIGHTGRAY),rectangle(xtranzistor1,ytranzistor1,xtranzistor2,ytranzistor2);
    if(buton==2)//dioda
        setcolor(YELLOW),rectangle(xdioda1,ydioda1,xdioda2,ydioda2);
    else
        setcolor(LIGHTGRAY),rectangle(xdioda1,ydioda1,xdioda2,ydioda2);
    if(buton==3)//baterie
        setcolor(YELLOW),rectangle(xbaterie1,ybaterie1,xbaterie2,ybaterie2);
    else
        setcolor(LIGHTGRAY),rectangle(xbaterie1,ybaterie1,xbaterie2,ybaterie2);
    if(buton==4)//siguranta
        setcolor(YELLOW),rectangle(xsiguranta1,ysiguranta1,xsiguranta2,ysiguranta2);
    else
        setcolor(LIGHTGRAY),rectangle(xsiguranta1,ysiguranta1,xsiguranta2,ysiguranta2);
    if(buton==5)//condensator
        setcolor(YELLOW),rectangle(xcondensator1,ycondensator1,xcondensator2,ycondensator2);
    else
        setcolor(LIGHTGRAY),rectangle(xcondensator1,ycondensator1,xcondensator2,ycondensator2);
    if(buton==6)//rezistenta
        setcolor(YELLOW),rectangle(xrezistenta1,yrezistenta1,xrezistenta2,yrezistenta2);
    else
        setcolor(LIGHTGRAY),rectangle(xrezistenta1,yrezistenta1,xrezistenta2,yrezistenta2);
    if(buton==7)//bec
        setcolor(YELLOW),rectangle(xbec1,ybec1,xbec2,ybec2);
    else
        setcolor(LIGHTGRAY),rectangle(xbec1,ybec1,xbec2,ybec2);
    if(buton==8)//ampermetru
        setcolor(YELLOW),rectangle(xampermetru1,yampermetru1,xampermetru2,yampermetru2);
    else
        setcolor(LIGHTGRAY),rectangle(xampermetru1,yampermetru1,xampermetru2,yampermetru2);
    if(buton==9)//voltmetru
        setcolor(YELLOW),rectangle(xvoltmetru1,yvoltmetru1,xvoltmetru2,yvoltmetru2);
    else
        setcolor(LIGHTGRAY),rectangle(xvoltmetru1,yvoltmetru1,xvoltmetru2,yvoltmetru2);
    if(buton==10)//nod
        setcolor(YELLOW),rectangle(xnod1,ynod1,xnod2,ynod2);
    else
        setcolor(LIGHTGRAY),rectangle(xnod1,ynod1,xnod2,ynod2);
    if(buton==11)//legatura
        setcolor(YELLOW),rectangle(xlegatura1,ylegatura1,xlegatura2,ylegatura2);
    else
        setcolor(LIGHTGRAY),rectangle(xlegatura1,ylegatura1,xlegatura2,ylegatura2);
    if(buton==12)//zoom in
        setcolor(YELLOW),rectangle(xzoomin1,yzoomin1,xzoomin2,yzoomin2);
    else
        setcolor(LIGHTGRAY),rectangle(xzoomin1,yzoomin1,xzoomin2,yzoomin2);
    if(buton==13)//zoom out
        setcolor(YELLOW),rectangle(xzoomout1,yzoomout1,xzoomout2,yzoomout2);
    else
        setcolor(LIGHTGRAY),rectangle(xzoomout1,yzoomout1,xzoomout2,yzoomout2);
    if(buton==14)//rotire
        setcolor(YELLOW),rectangle(xrotire1,yrotire1,xrotire2,yrotire2);
    else
        setcolor(LIGHTGRAY),rectangle(xrotire1,yrotire1,xrotire2,yrotire2);
    if(buton==15)//move
        setcolor(YELLOW),rectangle(xmove1,ymove1,xmove2,ymove2);
    else
        setcolor(LIGHTGRAY),rectangle(xmove1,ymove1,xmove2,ymove2);
    if(buton==16)//undo
        setcolor(YELLOW),rectangle(xundo1,yundo1,xundo2,yundo2);
    else
        setcolor(LIGHTGRAY),rectangle(xundo1,yundo1,xundo2,yundo2);
    if(buton==17)//redo
        setcolor(YELLOW),rectangle(xredo1,yredo1,xredo2,yredo2);
    else
        setcolor(LIGHTGRAY),rectangle(xredo1,yredo1,xredo2,yredo2);
    if(buton==18)//edit
        setcolor(YELLOW),rectangle(xedit1,yedit1,xedit2,yedit2);
    else
        setcolor(LIGHTGRAY),rectangle(xedit1,yedit1,xedit2,yedit2);
    if(buton==19)//delete
        setcolor(YELLOW),rectangle(xdelete1,ydelete1,xdelete2,ydelete2);
    else
        setcolor(LIGHTGRAY),rectangle(xdelete1,ydelete1,xdelete2,ydelete2);
    if(buton==20)//SAVE
        setcolor(YELLOW),rectangle(xsalveaza1,ysalveaza1,xsalveaza2,ysalveaza2);
    else
        setcolor(LIGHTGRAY),rectangle(xsalveaza1,ysalveaza1,xsalveaza2,ysalveaza2);
    if(buton==21)//incarca
        setcolor(YELLOW),rectangle(xincarca1,yincarca1,xincarca2,yincarca2);
    else
        setcolor(LIGHTGRAY),rectangle(xincarca1,yincarca1,xincarca2,yincarca2);
    if(buton==22)//reset
        setcolor(YELLOW),rectangle(xreset1,yreset1,xreset2,yreset2);
    else
        setcolor(LIGHTGRAY),rectangle(xreset1,yreset1,xreset2,yreset2);
    if(buton==23)//help
        setcolor(YELLOW),rectangle(xhelp1,yhelp1,xhelp2,yhelp2);
    else
        setcolor(LIGHTGRAY),rectangle(xhelp1,yhelp1,xhelp2,yhelp2);
    if(buton==0)
        deseneazaTOT();
}

bool deseneazaLegatura(int &piesa1,int &nod1,int &piesa2,int &nod2){
    bool click=false;
    int i,j,xMouse,yMouse;
    // aleg un punct de legatura (nod1) de la nodul 1 (piesa1)
    do{
        if(ismouseclick(WM_LBUTTONDOWN) && !click){
            getmouseclick(WM_LBUTTONDOWN,xMouse,yMouse);
            //xMouse=mousex(); yMouse=mousey();
            for(i=1;i<=nrPiese;i++){
                for(j=1;j<=vectorPiese[i].nrNod;j++){
                    if(abs(vectorPiese[i].noduri[j].x-xMouse)<=10 && abs(vectorPiese[i].noduri[j].y-yMouse)<=10){
                        piesa1=i;
                        nod1=j;
                        click=1;
                    }
                }
            }
        }
        else if(ismouseclick(WM_RBUTTONDOWN)){
            buton=0;
            selectareButon(buton);
            return 0;
        }
    }while(!click);
    // aleg un punct de legatura (nod2) de la nodul 2 (piesa2)
    // pana aleg, desenez mereu o linie
    click=0;
    int nrcoord;
    int x1,y1,x2,y2,x1c,y1c,x2c,y2c;
    x1=vectorPiese[piesa1].noduri[nod1].x;
    y1=vectorPiese[piesa1].noduri[nod1].y;
    xMouse=mousex(); yMouse=mousey();
    do{
        nrcoord=0;
        xMouse=mousex(); yMouse=mousey();
        if(InFereastra(xMouse,yMouse)==0) new_x_y_fromFereastra(xMouse,yMouse);

        DesenareLegatura(x1,y1,xMouse,yMouse,piesa1,nrcoord);

        //stergere linie
        delay(40);
        setcolor(BLACK);
        for(int i=0;i<nrcoord;i++) line(legx1[i],legy1[i],legx2[i],legy2[i]);

        if(ismouseclick(WM_LBUTTONDOWN) && !click){
            getmouseclick(WM_LBUTTONDOWN,xMouse,yMouse);
            //xMouse=mousex(); yMouse=mousey();
            for(i=1;i<=nrPiese;i++){
                if(i!=piesa1){
                    for(j=1;j<=vectorPiese[i].nrNod;j++){
                        if(abs(vectorPiese[i].noduri[j].x-xMouse)<=10 && abs(vectorPiese[i].noduri[j].y-yMouse)<=10){
                            piesa2=i;
                            nod2=j;
                            click=1;
                        }
                    }
                }
            }
        }
        else if(ismouseclick(WM_RBUTTONDOWN)){
            buton=0;
            selectareButon(buton);
            return 0;
        }
    }while(!click);
    x2=vectorPiese[piesa2].noduri[nod2].x;
    y2=vectorPiese[piesa2].noduri[nod2].y;
    setcolor(LIGHTGRAY);
    return 1;
}

void legatura(){
    nrLegaturi++;
    int piesa1,piesa2,nod1,nod2;
    if(deseneazaLegatura(piesa1,nod1,piesa2,nod2)==1){
        vectorLegaturi[nrLegaturi].piesa1=piesa1;
        vectorLegaturi[nrLegaturi].piesa2=piesa2;
        vectorLegaturi[nrLegaturi].nod1=nod1;
        vectorLegaturi[nrLegaturi].nod2=nod2;
        nrUndo++; vectorUndo[nrUndo].tip=2; vectorUndo[nrUndo].L=vectorLegaturi[nrLegaturi];
    }
    deseneazaTOT();
}

void zoom_in(int x,int y){
    int index=gaseste_piesa(x,y);
    struct piesa aux = vectorPiese[index];
    if(vectorPiese[index].tip_piesa==1 && vectorPiese[index].raza<=25)
        vectorPiese[index].raza++;
    else if(vectorPiese[index].tip_piesa==3 && vectorPiese[index].raza<=60)
        vectorPiese[index].raza+=2;
    else if((vectorPiese[index].tip_piesa==4 || vectorPiese[index].tip_piesa==6) && vectorPiese[index].raza<=25)
        vectorPiese[index].raza++;
    else if(vectorPiese[index].raza<=40) vectorPiese[index].raza++;
    actualizeazaColturi(vectorPiese[index]);
    actualizeazaNoduri(vectorPiese[index]);
    int x1,x2,y1,y2;
    x1=vectorPiese[index].x1;
    y1=vectorPiese[index].y1;
    x2=vectorPiese[index].x2;
    y2=vectorPiese[index].y2;
    if( IncapeFereastra(x1,y1,x2,y2) == false )
        vectorPiese[index] = aux;
    deseneazaTOT();
}

void zoom_out(int x,int y){
    int index=gaseste_piesa(x,y);
    if(vectorPiese[index].tip_piesa==1 && vectorPiese[index].raza>=4)
        vectorPiese[index].raza--;
    else if(vectorPiese[index].tip_piesa==3 && vectorPiese[index].raza>=20)
        vectorPiese[index].raza-=2;
    else if((vectorPiese[index].tip_piesa==4 || vectorPiese[index].tip_piesa==6) && vectorPiese[index].raza>=8)
        vectorPiese[index].raza--;
    else if(vectorPiese[index].raza>=13) vectorPiese[index].raza--;
    actualizeazaColturi(vectorPiese[index]);
    actualizeazaNoduri(vectorPiese[index]);
    deseneazaTOT();
}

void stergereNod(int x){
    for(;x<nrLegaturi;++x) vectorLegaturi[x]=vectorLegaturi[x+1];
    nrLegaturi--;
}

void stergerePiesa(int xmouse,int ymouse){
    int x = gaseste_piesa(xmouse,ymouse);
    if(x==0)
        return;
    nrRedo++; vectorRedo[nrRedo].tip=1; vectorRedo[nrRedo].P=vectorPiese[x];
    for(int i=x;i<nrPiese;++i) vectorPiese[i]=vectorPiese[i+1];
    for(int i=1;i<=nrLegaturi;++i)
        if(vectorLegaturi[i].piesa1==x || vectorLegaturi[i].piesa2==x) stergereNod(i--);
    for(int i=1;i<=nrLegaturi;++i){
        if(vectorLegaturi[i].piesa1>x) vectorLegaturi[i].piesa1--;
        if(vectorLegaturi[i].piesa2>x) vectorLegaturi[i].piesa2--;
    }
    nrPiese--;
    deseneazaTOT();
}

void salvareCircuit(){
    ofstream fout("circuit.txt");
    fout<<nrPiese<<'\n';
    for(int i=1;i<=nrPiese;i++)
        fout<<vectorPiese[i].tip_piesa<<'\n'<<vectorPiese[i].raza<<' '<<vectorPiese[i].xmij<<' '<<vectorPiese[i].ymij<<' '<<vectorPiese[i].rotire<<' '<<vectorPiese[i].valoare<<'\n';
    fout<<nrLegaturi<<'\n';
    for(int i=1;i<=nrLegaturi;i++)
        fout<<vectorLegaturi[i].piesa1<<' '<<vectorLegaturi[i].nod1<<' '<<vectorLegaturi[i].piesa2<<' '<<vectorLegaturi[i].nod2<<'\n';
    fout.close();
}

void incarcaCircuit(){
    resetareCircuit();
    punct p;
    ifstream fin("circuit.txt");
    //se citeste informatie de tipul numar de piese tip_piesa, raza, x, y, rotire, valoare
    //se presupune ca in fisier avem circuit "corect"
    fin>>nrPiese;
    for(int i=1;i<=nrPiese;i++){
        fin>>vectorPiese[i].tip_piesa;
        fin>>vectorPiese[i].raza;
        fin>>vectorPiese[i].xmij;
        fin>>vectorPiese[i].ymij;
        fin>>vectorPiese[i].rotire;
        fin>>vectorPiese[i].valoare;
        actualizeazaColturi(vectorPiese[i]);
        actualizeazaNoduri(vectorPiese[i]);
        deseneazaPiesa(vectorPiese[i]);
    }
    fin>>nrLegaturi;
    for(int i=1;i<=nrLegaturi;i++){
        fin>>vectorLegaturi[i].piesa1;
        fin>>vectorLegaturi[i].nod1;
        fin>>vectorLegaturi[i].piesa2;
        fin>>vectorLegaturi[i].nod2;
        int x1,x2,y1,y2;
        x1=vectorPiese[vectorLegaturi[i].piesa1].noduri[vectorLegaturi[i].nod1].x;
        y1=vectorPiese[vectorLegaturi[i].piesa1].noduri[vectorLegaturi[i].nod1].y;
        x2=vectorPiese[vectorLegaturi[i].piesa2].noduri[vectorLegaturi[i].nod2].x;
        y2=vectorPiese[vectorLegaturi[i].piesa2].noduri[vectorLegaturi[i].nod2].y;
        int s=0;
        DesenareLegatura(x1,y1,x2,y2,vectorLegaturi[i].piesa1,s);
    }
    fin.close();
}

void mutarePiesa(){
    bool click=0;
    int indexPiesa=0;
    int xMouse=0,yMouse=0;
    int xdif=0,ydif=0;
    do{
        if(ismouseclick(WM_LBUTTONDOWN) && !click){
            getmouseclick(WM_LBUTTONDOWN,xMouse,yMouse);
            indexPiesa=gaseste_piesa(xMouse,yMouse);
            if(indexPiesa>0)
                click=1,xdif=vectorPiese[indexPiesa].xmij-xMouse,ydif=vectorPiese[indexPiesa].ymij-yMouse;
        }
    }while(!click);
    if(ismouseclick(WM_LBUTTONUP)) clearmouseclick(WM_LBUTTONUP);
    bool inClick=0;
    do{
        if(ismouseclick(WM_MOUSEMOVE)){
            clearmouseclick(WM_MOUSEMOVE);
            xMouse=mousex(); yMouse=mousey();
            int x1,x2,y1,y2;
            vectorPiese[nrPiese+1]=vectorPiese[indexPiesa];
            vectorPiese[nrPiese+1].xmij=xMouse+xdif;
            vectorPiese[nrPiese+1].ymij=yMouse+ydif;
            actualizeazaColturi(vectorPiese[nrPiese+1]);
            actualizeazaNoduri(vectorPiese[nrPiese+1]);
            x1=vectorPiese[nrPiese+1].x1;
            y1=vectorPiese[nrPiese+1].y1;
            x2=vectorPiese[nrPiese+1].x2;
            y2=vectorPiese[nrPiese+1].y2;
            bool ok=desenarePesteAltaPiesa(x1,y1,x2,y2,indexPiesa);
            if(ok==0){
                vectorPiese[indexPiesa].xmij=xMouse+xdif;
                vectorPiese[indexPiesa].ymij=yMouse+ydif;
                actualizeazaColturi(vectorPiese[indexPiesa]);
                actualizeazaNoduri(vectorPiese[indexPiesa]);
                x1=vectorPiese[indexPiesa].x1;
                y1=vectorPiese[indexPiesa].y1;
                x2=vectorPiese[indexPiesa].x2;
                y2=vectorPiese[indexPiesa].y2;
                if(IncapeFereastra(x1,y1,x2,y2)==0 && ok==0){
                    int xmij=vectorPiese[indexPiesa].xmij;
                    int ymij=vectorPiese[indexPiesa].ymij;
                    if(InFereastra(xmij,ymij)==0){
                        new_x_y_fromFereastra(xmij,ymij);
                        vectorPiese[indexPiesa].xmij=xmij;
                        vectorPiese[indexPiesa].ymij=ymij;
                        actualizeazaColturi(vectorPiese[indexPiesa]);
                        actualizeazaNoduri(vectorPiese[indexPiesa]);
                    }
                    x1=vectorPiese[indexPiesa].x1;
                    y1=vectorPiese[indexPiesa].y1;
                    if(InFereastra(x1,y1)==0){
                        int dx=x1,dy=y1;
                        new_x_y_fromFereastra(x1,y1);
                        dx=dx-x1;dy=dy-y1;
                        xmij=xmij-dx;ymij=ymij-dy;
                        vectorPiese[indexPiesa].xmij=xmij;
                        vectorPiese[indexPiesa].ymij=ymij;
                        actualizeazaColturi(vectorPiese[indexPiesa]);
                        actualizeazaNoduri(vectorPiese[indexPiesa]);
                    }
                    x2=vectorPiese[indexPiesa].x2;
                    y2=vectorPiese[indexPiesa].y2;
                    if(InFereastra(x2,y2)==0){
                        int dx=x2,dy=y2;
                        new_x_y_fromFereastra(x2,y2);
                        dx=dx-x2;dy=dy-y2;
                        xmij=xmij-dx;ymij=ymij-dy;
                        vectorPiese[indexPiesa].xmij=xmij;
                        vectorPiese[indexPiesa].ymij=ymij;
                        actualizeazaColturi(vectorPiese[indexPiesa]);
                        actualizeazaNoduri(vectorPiese[indexPiesa]);
                    }
                }
                delay(40),deseneazaTOT();
            }
        }
        if(ismouseclick(WM_LBUTTONUP)){
            clearmouseclick(WM_LBUTTONUP);
            inClick=1;
        }
    }while(!inClick);
}

void rotirePiesa(){
    bool click=0;
    int indexPiesa=0;
    int xMouse=0,yMouse=0;
    int xdif=0,ydif=0;
    deseneazaTOT();
    do{
        if(ismouseclick(WM_LBUTTONDOWN) && !click){
            getmouseclick(WM_LBUTTONDOWN,xMouse,yMouse);
            indexPiesa=gaseste_piesa(xMouse,yMouse);
            if(indexPiesa>0)
                click=1;
        }
        else if(ismouseclick(WM_RBUTTONDOWN)){
            clearmouseclick(WM_RBUTTONDOWN);
            buton=0;
            selectareButon(buton);
            return;
        }
    }while(!click);
    SelectareFigura(vectorPiese[indexPiesa]);
    click=0;
    do{
        if(ismouseclick(WM_LBUTTONDOWN) && !click){
            getmouseclick(WM_LBUTTONDOWN,xMouse,yMouse);
            if(xMouse>=xsliderline1 && xMouse<=xsliderline2 && yMouse>=yslider1 && yMouse<=yslider2){
                click=1;
            }
        }
        else if(ismouseclick(WM_RBUTTONDOWN)){
            clearmouseclick(WM_RBUTTONDOWN);
            buton=0;
            selectareButon(buton);
            return;
        }
    }while(!click);
    click=0;
    xslider1=xMouse;
    vectorPiese[indexPiesa].rotire=(xslider1-xsliderline1)*4;
    vectorPiese[indexPiesa].rotire*=PI/180;
    bar(vectorPiese[indexPiesa].x1-3,vectorPiese[indexPiesa].y1-3,vectorPiese[indexPiesa].x2+3,vectorPiese[indexPiesa].y2+3);
    actualizeazaColturi(vectorPiese[indexPiesa]);
    actualizeazaNoduri(vectorPiese[indexPiesa]);
    actualizeazaRotire(vectorPiese[indexPiesa]);
    deseneazaTOT();
    SelectareFigura(vectorPiese[indexPiesa]);
    if(ismouseclick(WM_LBUTTONUP)) clearmouseclick(WM_LBUTTONUP);
    do{
        if(ismouseclick(WM_MOUSEMOVE)){
            clearmouseclick(WM_MOUSEMOVE);
            xMouse=mousex(),yMouse=mousey();
            setbkcolor(BLACK); setfillstyle(1,BLACK);
            if(xMouse<xsliderline1) xslider1=xsliderline1;
            else if(xMouse>xsliderline2) xslider1=xsliderline2;
            else xslider1=xMouse;
            vectorPiese[indexPiesa].rotire=(xslider1-xsliderline1)*4;
            vectorPiese[indexPiesa].rotire*=PI/180;
            bar(vectorPiese[indexPiesa].x1-3,vectorPiese[indexPiesa].y1-3,vectorPiese[indexPiesa].x2+3,vectorPiese[indexPiesa].y2+3);
            actualizeazaColturi(vectorPiese[indexPiesa]);
            actualizeazaNoduri(vectorPiese[indexPiesa]);
            actualizeazaRotire(vectorPiese[indexPiesa]);
            deseneazaTOT();
            SelectareFigura(vectorPiese[indexPiesa]);
            delay(40);
        }
        if(ismouseclick(WM_LBUTTONUP)){
            clearmouseclick(WM_LBUTTONUP);
            click=1;
        }
        else if(ismouseclick(WM_RBUTTONDOWN)){
            selectareButon(0);
            return;
        }
    }while(!click);
}

void makeUndo(){
    if(nrUndo>0)
    {
        nrRedo++;
        vectorRedo[nrRedo].tip=vectorUndo[nrUndo].tip;
        if(vectorUndo[nrUndo].tip==1 && nrPiese>0)
            vectorRedo[nrRedo].P=vectorPiese[nrPiese],nrPiese--;
        if(vectorUndo[nrUndo].tip==2 && nrLegaturi>0)
            vectorRedo[nrRedo].L=vectorLegaturi[nrLegaturi],nrLegaturi--;
        nrUndo--;
    }
    deseneazaTOT();
}

void makeRedo(){
    if(nrRedo>0)
    {
        nrUndo++;
        vectorUndo[nrUndo].tip = vectorRedo[nrRedo].tip;
        if(vectorRedo[nrRedo].tip==1)
            vectorUndo[nrUndo].P = vectorRedo[nrRedo].P,
            nrPiese++, vectorPiese[nrPiese] = vectorRedo[nrRedo].P;
        if(vectorRedo[nrRedo].tip==2)
            vectorUndo[nrUndo].L = vectorRedo[nrRedo].L,
            nrLegaturi++, vectorLegaturi[nrLegaturi] = vectorRedo[nrRedo].L;
        nrRedo--;
    }
    deseneazaTOT();
}

void HELP(){
    setbkcolor(BLACK);
    cleardevice();
    setbkcolor(RED);setfillstyle(SOLID_FILL,RED);
    const int xback1=5,yback1=5,xback2=85,yback2=35;
    bar(xback1,yback1,xback2,yback2); outtextxy(xback1+15,10,"BACK");
    //-----
    setcolor(LIGHTGRAY);
    setlinestyle(SOLID_LINE,0,3);
    for(int i=40;i<=800;i+=84) line(0,i,1500,i);
    line(90,0,90,40);
    line(150,40,150,796);
    line(300,40,300,796);
    setlinestyle(SOLID_LINE,0,1);
    outtextxy(700,10,"DESCRIPTION");
    char nume[][30]={"TRANSISTOR","DIODE","BATTERY","FUSE","CAPACITOR","RESISTOR","BULB","AMMETER","VOLTMETER"};
    setbkcolor(BLACK);
    for(int i=1;i<=9;i++)
        outtextxy(20,i*84,nume[i-1]);
    for(int i=1;i<=9;i++){
        piesa aux;
        aux.tip_piesa=i;
        aux.xmij=223;
        aux.ymij=i*84;
        aux.raza=20;
        if(i==1) aux.raza=5; //tranzistor
        else if(i==3) aux.raza=17; //baterie
        else if(i==4||i==6) aux.raza=12; //siguranta,rezistenta
        else if(i==10) aux.raza=2;
        actualizeazaColturi(aux);
        actualizeazaNoduri(aux);
        deseneazaPiesa(aux);
    }
    //tranzistor
    outtextxy(310,60, "A transistor is a semiconductor device used to amplify or switch electrical signals and power. The transistor is one of the basic building blocks");
    outtextxy(310,90, "of modern electronics. It is composed of semiconductor material, usually with at least three terminals for connection to an electronic circuit.");
    //dioda
    outtextxy(310,1*84+60,"A diode is a semiconductor device that essentially acts as a one-way switch for current. It allows current to flow easily in one direction, but");
    outtextxy(310,1*84+90,"severely restricts current from flowing in the opposite direction.");
    //baterie
    outtextxy(310,2*84+60,"An electrical cell is a power supply for a circuit, letting current flow from the positive terminal around the circuit to the negative terminal.");
    outtextxy(310,2*84+90,"The cell creates a potential difference across itself by supplying energy to charges.");
    //siguranta
    outtextxy(310,3*84+60,"A fuse is a device that allows current to flow while it is below a certain value, which depends on the rating of fuse. When the curent exceeds this");
    outtextxy(310,3*84+90,"value, the fuse will (melt or break), which breaks the circuit and stops current flowing altogether");
    //condensator
    outtextxy(310,4*84+75,"A capacitor is a device that stores electrical energy in an electric field. It is a passive electronic component with two terminals.");
    //rezistenta
    outtextxy(310,5*84+60,"A resistor is a passive two-terminal electrical component that implements electrical resistance as a circuit element. In electronic circuits, resistors");
    outtextxy(310,5*84+90,"are used to reduce current flow, adjust signal levels, to divide voltages and terminate transmission lines, among other uses.");
    //bec
    outtextxy(310,6*84+60,"A bulb is a component that transforms electrical energy into light energy. The brightness of the bulb depend on its power P, which has");
    outtextxy(310,6*84+90,"the units of watts, W.");
    //ampermetru
    outtextxy(310,7*84+60,"An ammeter is a device that measures current flowing between its ends. Current is the rate of flow of charge and is measured in amps");
    outtextxy(310,7*84+90,"with symbol A.");
    //voltmetru
    outtextxy(310,8*84+60,"A voltmeter is a device that measures the potential difference betweeb its ends. Potential difference is often called voltage and is measured");
    outtextxy(310,8*84+90,"in volts, V");
    //------
    bool inapoi=0;
    while(inapoi==0){
        if(ismouseclick(WM_LBUTTONDOWN)){
            int x,y;
            getmouseclick(WM_LBUTTONDOWN,x,y);
            if(x>xback1 && x<xback2 && y>yback1 && y<yback2) inapoi=1;
        }
    }
    drawFereastraLucru();
    deseneazaTOT();
    selectareButon(buton);
}

void initializare_fereastrainput(){
    int width2=3*height/8,height2=height/2;
    initwindow(3*height/8,height/2+height/8,"Input the value",width+10,50,0);
    input_window=getcurrentwindow();
    setcolor(WHITE);
    line(0,height/8,width2,height/8);
    line(width2/3,+height/8,width2/3,height2+height/8);
    line(2*width2/3,+height/8,2*width2/3,height2+height/8);
    line(0,height2/4+height/8,width,height2/4+height/8);
    line(0,height2/2+height/8,width,height2/2+height/8);
    line(0,3*height2/4+height/8,width,3*height2/4+height/8);
    settextstyle(3,HORIZ_DIR,8);
    outtextxy(width2*0.111,height2*0.028+height/8,"1");
    outtextxy(width2*0.452,height2*0.028+height/8,"2");
    outtextxy(width2*0.787,height2*0.028+height/8,"3");
    outtextxy(width2*0.1,height2*0.277+height/8,"4");
    outtextxy(width2*0.45,height2*0.277+height/8,"5");
    outtextxy(width2*0.79,height2*0.277+height/8,"6");
    outtextxy(width2*0.106,height2*0.527+height/8,"7");
    outtextxy(width2*0.45,height2*0.527+height/8,"8");
    outtextxy(width2*0.795,height2*0.527+height/8,"9");
    outtextxy(width2*0.451,height2*0.777+height/8,"0");
    settextstyle(3,HORIZ_DIR,4);
    outtextxy(width2*0.03,height2*0.833+height/8,"DEL");
    outtextxy(width2*0.7,height2*0.833+height/8,"ENT");
    setfillstyle(1,BLACK);
    bar(0,0,width2,height/8);
}

void IntroducereValori(piesa &P){
    P.valoare=0;
    int width2=3*height/8,height2=height/2;
    setcurrentwindow(input_window);
    setfillstyle(1,BLACK);
    bar(0,0,width2,height/8);
    int lastpress=0,nr_cifre=0;
    while(lastpress!=11){
        if(ismouseclick(WM_LBUTTONDOWN)){
            clearmouseclick(WM_LBUTTONDOWN);
            mouse.x=mousex();
            mouse.y=mousey();
            if(mouse.x<width2/3){
                if(mouse.y<height2/4+height/8) lastpress=1,outtextxy(nr_cifre*width2/14+5,height/80,"1"),nr_cifre++;
                else if(mouse.y<height2/2+height/8) lastpress=4,outtextxy(nr_cifre*width2/14+5,height/80,"4"),nr_cifre++;
                else if(mouse.y<3*height2/4+height/8) lastpress=7,outtextxy(nr_cifre*width2/14+5,height/80,"7"),nr_cifre++;
                else lastpress=10;
            }
            else if(mouse.x<2*width2/3){
                if(mouse.y<height2/4+height/8) lastpress=2,outtextxy(nr_cifre*width2/14+5,height/80,"2"),nr_cifre++;
                else if(mouse.y<height2/2+height/8) lastpress=5,outtextxy(nr_cifre*width2/14+5,height/80,"5"),nr_cifre++;
                else if(mouse.y<3*height2/4+height/8) lastpress=8,outtextxy(nr_cifre*width2/14+5,height/80,"8"),nr_cifre++;
                else lastpress=0,outtextxy(nr_cifre*width2/14+5,height/80,"0"),nr_cifre++;
            }
            else{
                if(mouse.y<height2/4+height/8) lastpress=3,outtextxy(nr_cifre*width2/14+5,height/80,"3"),nr_cifre++;
                else if(mouse.y<height2/2+height/8) lastpress=6,outtextxy(nr_cifre*width2/14+5,height/80,"6"),nr_cifre++;
                else if(mouse.y<3*height2/4+height/8) lastpress=9,outtextxy(nr_cifre*width2/14+5,height/80,"9"),nr_cifre++;
                else lastpress=11;
            }
            if(lastpress<10) P.valoare=P.valoare*10+lastpress;
            else if(lastpress==10) P.valoare=0,nr_cifre=0,bar(0,0,width2,height/8);
        }
    }
    bar(0,0,width2,height/8);
    setcurrentwindow(main_window);
}

void EditareValori(int x,int y){
    int index=gaseste_piesa(x,y);
    IntroducereValori(vectorPiese[index]);
}

void amplasarePiese(){
    int l=0,piesa_existenta=0;
    setcurrentwindow(main_window);
    while(buton!=44){
        double x1,y1,x2,y2;
        if(ismouseclick(WM_LBUTTONDOWN)){
            clearmouseclick(WM_LBUTTONDOWN);
            mouse.x=mousex();
            mouse.y=mousey();
            piesa_existenta=gaseste_piesa(mouse.x,mouse.y);
            if(l!=piesa_existenta){
                if(l) DeselectareFigura(vectorPiese[l].x1,vectorPiese[l].y1,vectorPiese[l].x2,vectorPiese[l].y2);
                deseneazaPiesa(vectorPiese[piesa_existenta]);
            }
            if(piesa_existenta)
                SelectareFigura(vectorPiese[piesa_existenta]);
            if(mouse.x>xtranzistor1 && mouse.y>ytranzistor1 && mouse.x<xtranzistor2 && mouse.y<ytranzistor2) buton=1,selectareButon(buton); //tranzistor
            else if(mouse.x>xdioda1 && mouse.x<xdioda2 && mouse.y>ydioda1 && mouse.y<ydioda2) buton=2,selectareButon(buton); //dioda
            else if(mouse.x>xbaterie1 && mouse.x<xbaterie2 && mouse.y>ybaterie1 && mouse.y<ybaterie2) buton=3,selectareButon(buton); //baterie
            else if(mouse.x>xsiguranta1 && mouse.x<xsiguranta2 && mouse.y>ysiguranta1 && mouse.y<ysiguranta2) buton=4,selectareButon(buton); //siguranta
            else if(mouse.x>xcondensator1 && mouse.x<xcondensator2 && mouse.y>ycondensator1 && mouse.y<ycondensator2) buton=5,selectareButon(buton); //condensator
            else if(mouse.x>xrezistenta1 && mouse.x<xrezistenta2 && mouse.y>yrezistenta1 && mouse.y<yrezistenta2) buton=6,selectareButon(buton); //rezistenta
            else if(mouse.x>xbec1 && mouse.x<xbec2 && mouse.y>ybec1 && mouse.y<ybec2) buton=7,selectareButon(buton); //bec
            else if(mouse.x>xampermetru1 && mouse.x<xampermetru2 && mouse.y>yampermetru1 && mouse.y<yampermetru2) buton=8,selectareButon(buton); //ampermetru
            else if(mouse.x>xvoltmetru1 && mouse.x<xvoltmetru2 && mouse.y>yvoltmetru1 && mouse.y<yvoltmetru2) buton=9,selectareButon(buton); //voltmetru
            else if(mouse.x>xnod1 && mouse.x<xnod2 && mouse.y>ynod1 &&mouse.y<ynod2) buton=10,selectareButon(buton); //nod
            else if(mouse.x>xlegatura1 && mouse.x<xlegatura2 && mouse.y>ylegatura1 && mouse.y<ylegatura2) buton=11,selectareButon(buton),legatura();//legatura
            else if(mouse.x>xzoomin1 && mouse.x<xzoomin2 && mouse.y>yzoomin1 && mouse.y<yzoomin2) buton=12,selectareButon(buton);//ZOOM IN
            else if(mouse.x>xzoomout1 && mouse.x<xzoomout2 && mouse.y>yzoomout1 && mouse.y<yzoomout2) buton=13,selectareButon(buton);//ZOOM OUT
            else if(mouse.x>xrotire1 && mouse.x<xrotire2 && mouse.y>yrotire1 && mouse.y<yrotire2) buton=14,selectareButon(buton),rotirePiesa();//ROTIRE
            else if(mouse.x>xmove1 && mouse.x<xmove2 && mouse.y>ymove1 && mouse.y<ymove2) buton=15,selectareButon(buton),mutarePiesa();//MOVE
            else if(mouse.x>xundo1 && mouse.x<xundo2 && mouse.y>yundo1 && mouse.y<yundo2) buton=16,selectareButon(buton),makeUndo(); //UNDO
            else if(mouse.x>xredo1 && mouse.x<xredo2 && mouse.y>yredo1 && mouse.y<yredo2) buton=17,selectareButon(buton),makeRedo(); //REDO
            else if(mouse.x>xedit1 && mouse.x<xedit2 && mouse.y>yedit1 && mouse.y<yedit2) buton=18,selectareButon(buton); //EDIT
            else if(mouse.x>xdelete1 && mouse.x<xdelete2 && mouse.y>ydelete1 && mouse.y<ydelete2) buton=19,selectareButon(buton); //DELETE
            else if(mouse.x>xsalveaza1 && mouse.x<xsalveaza2 && mouse.y>ysalveaza1 && mouse.y<ysalveaza2) buton=20,selectareButon(buton),salvareCircuit();//SAVE
            else if(mouse.x>xincarca1 && mouse.x<xincarca2 && mouse.y>yincarca1 && mouse.y<yincarca2) buton=21,selectareButon(buton),incarcaCircuit();//INCARCA
            else if(mouse.x>xreset1 && mouse.x<xreset2 && mouse.y>yreset1 && mouse.y<yreset2) buton=22,selectareButon(buton),resetareCircuit();//RESET
            else if(mouse.x>xhelp1 && mouse.x<xhelp2 && mouse.y>yhelp1 && mouse.y<yhelp2) buton=23,selectareButon(buton),delay(90),HELP(); //HELP
            else if(mouse.x>xexit1 && mouse.x<xexit2 && mouse.y>yexit1 && mouse.y<yexit2) buton=44;
            else if(buton==11) legatura();
            else if(buton==12) zoom_in(mouse.x,mouse.y);
            else if(buton==13) zoom_out(mouse.x,mouse.y);
            else if(buton==14) rotirePiesa();
            else if(buton==15) mutarePiesa();
            else if(buton==18) EditareValori(mouse.x,mouse.y);
            else if(buton==19) stergerePiesa(mouse.x,mouse.y);
            else if(buton>=1 && buton<=10 && InFereastra(mouse.x,mouse.y)){
                double raza=20;
                if(buton==1)//tranzistor
                    raza=7;
                else if(buton==3) //baterie
                    raza=10;
                else if(buton==4||buton==6)//siguranta,rezistenta
                    raza=9;
                else if(buton==10)
                    raza=3;
                vectorPiese[nrPiese+1].tip_piesa=buton;
                vectorPiese[nrPiese+1].raza=raza;
                vectorPiese[nrPiese+1].rotire=rotire;
                vectorPiese[nrPiese+1].xmij=mouse.x;
                vectorPiese[nrPiese+1].ymij=mouse.y;
                vectorPiese[nrPiese+1].valoare=0;
                actualizeazaColturi(vectorPiese[nrPiese+1]);
                actualizeazaNoduri(vectorPiese[nrPiese+1]);
                int x1=vectorPiese[nrPiese+1].x1;
                int y1=vectorPiese[nrPiese+1].y1;
                int x2=vectorPiese[nrPiese+1].x2;
                int y2=vectorPiese[nrPiese+1].y2;
                if(IncapeFereastra(x1,y1,x2,y2)){
                    if(!desenarePesteAltaPiesa(x1,y1,x2,y2,nrPiese+1)){
                        nrPiese++;
                        deseneazaPiesa(vectorPiese[nrPiese]);
                        nrUndo++; vectorUndo[nrUndo].tip=1; vectorUndo[nrUndo].P=vectorPiese[nrPiese];
                        if(buton<=9) IntroducereValori(vectorPiese[nrPiese]);
                    }
                }
            }
            l=piesa_existenta;
        }
        else if(ismouseclick(WM_RBUTTONDOWN)){
            buton=0;
            selectareButon(buton);
            clearmouseclick(WM_RBUTTONDOWN);
        }
    }
}

int main()
{
    initwindow(width,height,"ELECTRON - vizualizator de scheme electronice");
    main_window=getcurrentwindow();
    drawFereastraLucru();
    initializare_fereastrainput();
    amplasarePiese();
    return 0;
}
