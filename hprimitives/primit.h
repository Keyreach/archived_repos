#include <SDL2/SDL.h>
#include <string>
using namespace std;

class Base {
	public:
		virtual void hide()=0;
		virtual void show()=0;
		virtual void move(int dx, int dy)=0;
		virtual string toString()=0; 
		SDL_Renderer* ren;
};

class Pixel : public Base {
	public:
		int x, y, colour;
		Pixel(SDL_Renderer* _ren, int _x, int _y, int _c);
		virtual void hide();
		virtual void show();
		virtual void move(int dx, int dy);
		virtual string toString();
};

class Line : public Base {
	public:
		int x1, y1, x2, y2;
		Line(SDL_Renderer* _ren, int a, int b, int c, int d);
		virtual void hide();
		virtual void show();
		virtual void move(int dx, int dy);
		virtual string toString();
};

class Rect : public Line {
	public:
		Rect(SDL_Renderer* _ren, int _x1, int _y1, int _x2, int _y2);
		virtual void hide();
		virtual void show();
		virtual void move(int dx, int dy);
		virtual string toString();
};

class Circle : public Pixel {
	public:
		int radius;
		Circle(SDL_Renderer* _ren, int _x, int _y, int _r, int _c);
		void draw(SDL_Renderer* _ren, int _x, int _y, int _r);
		virtual void hide();
		virtual void show();
		virtual void move(int dx, int dy);
		virtual string toString();
};

class Figure : public Base {
	public:
		int n;
		Base* shapes[100];
		Figure(SDL_Renderer* _ren);
		void add(Base* b);
		void show();
		void hide();
		void move(int dx, int dy);
		void saveToFile(char* filename);
		void loadFromFile(char* filename);
		string toString();
};
