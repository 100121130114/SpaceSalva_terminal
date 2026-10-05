#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdkey.h"
#include <unistd.h>

//var conf
unsigned int monA=30,monB=60;//tamaño de el monitor
unsigned int omniA=3, omniB=5;//cantidad de omnis
#define cvd 10000 //cantidad de veces que se prueba la detecion
#define cpp 1000 //cantidad de posibles proyectiles
#define kbt 5 //keyboard tempo
	#define omt 10000 //omnis tempo
	#define prt 10000 //proyectiles tempo
#define ado 5 //altura a la que estan los omnis
#define jdo monA-5 //altura a la que esta la nave
#define sepA 2 //separacion entre omnis
#define sepB 3 //separacion entre omnis
#define ppo 7999 //porcentaje de probabailidad de que un omni lanze un proyectil
#define spo 100 //score points por omni
#define ebm 2 //extra de blockes para el muerte de los misiles
#define ab 1 //almacen de balas
#define tr 5 //tiempo de recarga
char* paint=" X!#|/";
//var save
char* pmemo;
bool* pbd;
unsigned int trash=0;

//functions filter
char* memo(unsigned int a,int b){
 if(a>=monA){return (char*)&trash;}
 if(b>=monB){return (char*)&trash;}
 if(b<0){return (char*)&trash;}
 trash=0;
 return &pmemo[a*monB+b];
}
bool* bd(unsigned int a,int b){
 if(a>=monA){return (bool*)&trash;}
 if(b>=monB){return (bool*)&trash;}
 if(b<0){return (bool*)&trash;}
 trash=0;
 return &pbd[a*monB+b];
}

//functions system
int main(){
 printf("SEED ");
 scanf("%u",&trash);
 srand(trash);
 //inicializador
 pmemo=(char*)calloc(monA*monB,sizeof(char));
 if(pmemo==NULL){printf("Error de memoria. PMEMO\n");return 0;}
 pbd=(bool*)calloc(monA*monB,sizeof(bool));
 if(pbd==NULL){printf("Error de memoria. PBD\n");return 0;}
 for(unsigned int a=15;a<20;a++){
  for(unsigned int b=3;b<monB-3;b++){
   if(!((b%8>=0)&&(b%8<3))){
    *bd(a,b)=1;
   }
  }
 }
 for(unsigned int a=0;a<monA;a++){
  for(unsigned int b=0;b<monB;b++){
   *memo(a,b)=*bd(a,b)?paint[3]:paint[0];
  }
 }
 
 //var system
 unsigned int lives=3,score=0,deom=0;//puntos, vidas y omnis muertos
 bool omnis[omniA][omniB];//si estan los omnis
 unsigned int ubio;//ubicacion de los omnis
 unsigned int ubi[2];//ubicacion de la nave
 unsigned int pro[cpp][3];//lista de proyectiles
 bool bul=0;//intento de disparar
 unsigned int ejeglobal=0;//cantidad de ejecuciones de forma global
 unsigned int bullets=0;//conte de las balas
 //limpieza
 for(unsigned int a=0;a<cpp;a++){
  pro[a][0]=0;
  pro[a][1]=0;
  pro[a][2]=0;
 } 

 //systema de juego
 while(true){
  ejeglobal++;
  ubi[0]=ubi[1];
  //print
  system("clear");
  for(unsigned int a=0;a<monA;a++){
   for(unsigned int b=0;b<monB;b++){
    printf("%c",*memo(a,b));
   }
   printf("\n");
  }
  printf("LIVES %u\tSCORE %u\n",lives,score);
  //sistema de ejecuciones
  for(unsigned int t=0;t<cvd;t++){
   for(unsigned int a=0;a<monA;a++){
    for(unsigned int b=0;b<monB;b++){
     *memo(a,b)=*bd(a,b)?paint[3]:paint[0];
    }
   }
   //detecion de key board
   if((kbhit())&&(t%kbt==0)){
    switch(getch()){
     case 'h':{
      ubi[0]--;
     }break;
     case 'k':{
      ubi[0]++;
     }break;
     case 'j':{
      bul=1;
     }break;
     case 'p':{
      printf("\n\t>PAUSE<");
      while(true){
       if((kbhit())&&(getch()=='p')){
        goto exitpause;
       }
       if((kbhit())&&(getch()=='q')){
        free(pmemo);
        system("clear");
        return 0;
       }
      }
      exitpause:
     }break;
     case 'q':{
      free(pmemo);
      system("clear");
      return 0;
     }break;
    };
   }
   if(t%omt==0){
    ubio=(ubio+1)%(2*(((omniB-1)*sepB)+1)+monB);
   }
   //movimiento de los proyectiles
   for(unsigned int a=0;a<cpp;a++){
    if(pro[a][2]==2){
     if(*bd(pro[a][0],pro[a][1])){
      *bd(pro[a][0],pro[a][1])=0;
      pro[a][2]=0;
      goto projmp1;
     }
     for(int d=0;d<=2;d++){
      if(*memo(pro[a][0]+d,pro[a][1])==paint[5]){
       for(unsigned int c=0;c<cpp;c++){
        if((pro[c][2]==1)&&((pro[c][0]==pro[a][0]+d)&&(pro[c][1]==pro[a][1]))){
         pro[c][2]=0;
         pro[a][2]=0;
         goto projmp1;
        }
       }
      }
     }     
     *memo(pro[a][0],pro[a][1])=paint[4];
     if(pro[a][0]<=0){pro[a][2]=0;}
     if(t%prt==0){pro[a][0]--;}
     projmp1:
     continue;
    }
    if(pro[a][2]==1){
     if(*bd(pro[a][0],pro[a][1])){
      *bd(pro[a][0],pro[a][1])=0;
      pro[a][2]=0;
      goto projmp1;
     }
     for(int d=-2;d<=0;d++){
      if(*memo(pro[a][0]+d,pro[a][1])==paint[4]){
       for(unsigned int c=0;c<cpp;c++){
        if((pro[c][2]==2)&&((pro[c][0]==pro[a][0]+d)&&(pro[c][1]==pro[a][1]))){
         pro[c][2]=0;
         pro[a][2]=0;
         goto projmp2;
        }
       }
      }
     }
     *memo(pro[a][0],pro[a][1])=paint[5];
     if(pro[a][0]>monA){pro[a][2]=0;}
     if(t%prt==0){pro[a][0]++;}
     projmp2:
     continue;
    }
   }
   //movimiento de los omnis
   for(unsigned int a=0;a<omniA;a++){
    for(unsigned int b=0;b<omniB;b++){
     if(!(omnis[a][b])){
      if(*memo(ado+a*sepA,(ubio-((omniB-1)*sepB)-1)+b*sepB)==paint[4]){
       for(unsigned int c=0;c<cpp;c++){
        if((pro[c][2]==2)&&((pro[c][0]==ado+a*sepA)&&(pro[c][1]==(ubio-((omniB-1)*sepB)-1)+b*sepB))){
         omnis[a][b]=1;
	 score+=spo;
	 deom++;
	 pro[c][2]=0;
	 break;
	}
       }
       continue;
      }
      *memo(ado+a*sepA,(ubio-((omniB-1)*sepB)-1)+b*sepB)=paint[2];
      //generador de proyectiles
      if((rand()<ppo)&&(*memo(ado+a*sepA+1,(ubio-((omniB-1)*sepB)-1)+b*sepB)!=paint[5])){
       for(unsigned int c=0;c<cpp;c++){
        if(pro[c][2]==0){
         pro[c][0]=ado+a*sepA+1;
         pro[c][1]=(ubio-((omniB-1)*sepB)-1)+b*sepB;
         pro[c][2]=1;
         break;
        }
       }
      }
     }
    }
   }
   //movimiento de la nave y sus disparos
   if(ubi[0]<monB){ubi[1]=ubi[0];}
   if((bul)&&(bullets<ab)){
    bullets=(bullets+1)%(ab+tr);
    for(unsigned int c=0;c<cpp;c++){
     if(pro[c][2]==0){
      pro[c][0]=jdo-1;
      pro[c][1]=ubi[1];
      pro[c][2]=2;
      break;
     }
    }
   }
   bul=0;
   if(*memo(jdo,ubi[1])==paint[5]){
    for(unsigned int c=0;c<cpp;c++){
     if((pro[c][2]==1)&&((pro[c][0]==jdo)&&(pro[c][1]==ubi[1]))){
      lives--;
      pro[c][2]=0;
      break;
     }
    }
   }
   if(lives==0){
    free(pmemo);
    system("clear");
    printf("SCORE %u.\n",score);
    printf("GAME OVER.\n");
    return 0;
   }
   *memo(jdo,ubi[1])=paint[1];
  }
  if(bullets>=ab){bullets=(bullets+1)%(ab+tr);}
 }
 return 0;
}
