#include <SFML/Graphics.hpp>
#include <time.h>
#include<bits/stdc++.h>

using namespace std;
using namespace sf;

int N=16,M=16;//dimensiunea terenului
int size=40;//dimensiunea unui patratel
int w = 600;//w si h -> dimensiunea jocului
int h = 600;

int di[5]={0,0,1,-1,0};
int dj[5]={0,-1,0,0,1};//directiile

int dir,num=1,score,scoremax;//directie, scor, scor maxim
int k,antk;//folosite pentru a determina schimbarea unei directii
bool gamestart=false;//a inceput jocul?

struct Snake//pozitia jucatorului,rotatia si c-> folosit daca suntem intr-un loc unde am schimbat directia pentru a crea un "colt"
{ int x,y,r,c;}  s[100];

struct Fruct
{ int x,y;} f;//pozitia marului

int opus(int x)//opusul miscarii
{
    if(x==1)return 4;
    else if(x==4)return 1;
    else if(x==2)return 3;
    else if(x==3)return 2;
    return 0;
}

int rotation(int x)//rotirea in functie de directie
{
    switch(x)
    {
    case 0:
        return -1;
    case 1:
        return 0;
    case 3:
        return 90;
    case 2:
        return-90;
    case 4:
        return 180;
    }
}

int getDir(int i,int j)//obtinerea directiei
{
    for(int q=1;q<5;q++)
        if(di[q]==i&&dj[q]==j)
            return q;
}

void Tick()//miscarea jucatorului
 {
     antk=getDir(s[num].y-s[num-1].y,s[num].x-s[num-1].x);//directia cozii
    for (int i=num;i>0;i--)//de la coada la cap
	 {
     k=getDir(s[i].y-s[i-1].y,s[i].x-s[i-1].x);//directia actuala
     if(antk==0)antk=k;
     s[i].x=s[i-1].x;
	 s[i].y=s[i-1].y;//mutam jucatorul
	 s[i].c=-1;//resetam colturile
	 s[i].r=rotation(opus(k));//rotirea
	 if(antk!=k&&antk>0)//daca schimbam directia->avem un colt
        {
            if((antk==3&&k==4)||(antk==1&&k==2))
                s[i+1].c=180;
            else if((antk==1&&k==3)||(antk==2&&k==4))
                s[i+1].c=90;
            else if((antk==4&&k==3)||(antk==2&&k==1))
                s[i+1].c=0;
            else if((antk==3&&k==1)||(antk==4&&k==2))
                s[i+1].c=-90;
            //rotirea coltului in functie de directia veche si cea noua
            antk=k;
        }
	 }

    if (dir==4) s[0].x+=1;
    if (dir==1) s[0].x-=1;
    if (dir==2) s[0].y+=1;
    if (dir==3) s[0].y-=1;
    //miscarea capului

    antk=opus(k);
    if(antk!=dir&&antk!=opus(dir)&&num>1)//verificam daca s[1] este colt
        {
            cout<<antk<<" "<<dir<<endl;
            if((antk==3&&dir==4)||(antk==1&&dir==2))
                s[1].c=0;
            else if((antk==1&&dir==3)||(antk==2&&dir==4))
                s[1].c=-90;
            else if((antk==4&&dir==3)||(antk==2&&dir==1))
                s[1].c=180;
            else if((antk==3&&dir==1)||(antk==4&&dir==2))
                s[1].c=90;
            antk=dir;
        }
    else s[1].c=-1;//daca nu il resetam
    s[0].r=rotation(dir);//rotirea capului
    s[num].r=s[num-1].r;//rotirea cozii

	if ((s[0].x==f.x) && (s[0].y==f.y))//daca am luat un mar, adaugam in coada, cream un mar nou, crestem scorul si il comparam cu scorul maxim
      { num++;
        int cv=getDir(s[num-1].y-s[num-2].y,s[num-1].x-s[num-2].x);
        s[num].x=s[num-1].x+dj[cv];
        s[num].y=s[num-1].y+di[cv];
        s[num].r=s[num-1].r;
    	f.x=rand() % 15+1;
        f.y=rand() % 15+1;
        score++;
        if(score>scoremax)scoremax=score;}

    if (s[0].x>N) s[0].x=0;  if (s[0].x<0) s[0].x=N;
    if (s[0].y>M) s[0].y=0;  if (s[0].y<0) s[0].y=M;
    //verificam daca parasim terenul; daca da intram pe partea cealalta

    for (int i=1;i<num;i++)
     if (s[0].x==s[i].x && s[0].y==s[i].y)  {score=score-(num-i);num=i;}
     //daca ne intersectam, taiem din corp si din scor
 }

int main()
{
	srand(time(0));

    RenderWindow window(VideoMode(w, h), "Snake");

    //luam texturile si font-ul textului
	Texture sarpili,backT,marT;
    sarpili.loadFromFile("snake.png");
    backT.loadFromFile("teren.png");
    marT.loadFromFile("mar.png");

    Font font;
    font.loadFromFile("font.TTF");
    Text text;
    text.setFont(font);


    Sprite player(sarpili);Sprite point(marT);Sprite background(backT); Sprite endplayer(sarpili); Sprite mijloc(sarpili);Sprite colt1(sarpili);

    //cream fiecare sprite

    background.setScale(0.83,0.83);
    player.setTextureRect(IntRect(192,64,64,64));
    player.setOrigin(32,32);
    colt1.setTextureRect(IntRect(0,0,64,64));
    colt1.setOrigin(32,32);
    colt1.setScale(0.625,0.625);
    endplayer.setOrigin(32,32);
    mijloc.setOrigin(32,32);
    player.setScale(0.625,0.625);
    endplayer.setScale(0.625,0.625);
    endplayer.setTextureRect(IntRect(192,192,64,64));
    mijloc.setScale(0.625,0.625);
    mijloc.setTextureRect(IntRect(64,0,64,64));
    point.setScale(0.09,0.09);

	Clock clock;
    float timer=0, delay=0.35;

    //initializam pozitia sarpelui si a marului
	f.x=10;
    f.y=10;
    s[0].x=7;
    s[0].y=7;
    s[1].x=8;
    s[1].y=7;


    while (window.isOpen())
    {
        //cronometru pentru a seta viteza
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            if(e.type == Event::KeyPressed)//input-ul de la jucator
                switch(e.key.code)
                {
                default:
                    break;
                case Keyboard::Escape:
                    gamestart=!gamestart;
                case Keyboard::Left:
                    if(dir!=4&&dir!=1){gamestart=true;dir=1;}
                    break;
                case Keyboard::Right:
                    if(dir!=1&&dir!=4){gamestart=true;dir=4;}
                    break;
                case Keyboard::Up:
                    if(dir!=2&&dir!=3){gamestart=true;dir=3;}
                    break;
                case Keyboard::Down:
                    if(dir!=3&&dir!=2){gamestart=true;dir=2;}
                    break;
                }
		}

		//if (Keyboard::isKeyPressed(Keyboard::Left)&&dir!=4) {dir=1;gamestart=true;}
	    //if (Keyboard::isKeyPressed(Keyboard::Right)&&dir!=1) {dir=4;gamestart=true;}
	    //if (Keyboard::isKeyPressed(Keyboard::Up)&&dir!=2) {dir=3;gamestart=true;}
		//if (Keyboard::isKeyPressed(Keyboard::Down)&&dir!=3) {dir=2;gamestart=true;}

		if (timer>(delay-((float)score/100))&&gamestart==true) {timer=0; Tick();}

   ////// draw  ///////
    window.clear();
    window.draw(background);

    player.setPosition(s[0].x*size-20,s[0].y*size-20);
    player.setRotation(s[0].r);
    window.draw(player);
	for (int i=1;i<num;i++)
	    if(s[i].c==-1){ mijloc.setPosition(s[i].x*size-20, s[i].y*size-20);  mijloc.setRotation(s[i].r);  window.draw(mijloc); }
	    else{ colt1.setPosition(s[i].x*size-20, s[i].y*size-20);  colt1.setRotation(s[i].c);  window.draw(colt1); }

    if(num>0){
    endplayer.setPosition(s[num].x*size-20,s[num].y*size-20);
    endplayer.setRotation(s[num].r);
    window.draw(endplayer);}
	point.setPosition(f.x*size-40,  f.y*size-40);  window.draw(point);

	if(gamestart==true)
    {
         char s[50];
        char s1[100]="score: ";
        strcpy (s,to_string(score).c_str());
        strcat(s1,s);
        text.setString(s1);
        text.setPosition(360,0);
        text.setCharacterSize(30);

        window.draw(text);

    }
    else
    {
         char s[50];
        char s1[121]="Your highest\nscore was: ";
        strcpy (s,to_string(scoremax).c_str());
        strcat(s1,s);
        text.setPosition(120,180);
        text.setCharacterSize(40);
        text.setString(s1);

        window.draw(text);

    }

	window.display();
	}

    return 0;
}
