#include "primit.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

/*
 * PIXEL
 */

Pixel::Pixel(SDL_Renderer* _ren, int _x, int _y, int _c){
	x = _x;
	y = _y;
	colour = _c;
	ren = _ren;
}
void Pixel::hide(){
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderDrawPoint(ren, x, y);
}
void Pixel::show(){
	SDL_SetRenderDrawColor(ren, 0, colour, 0, 255);
	SDL_RenderDrawPoint(ren, x, y);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}
void Pixel::move(int dx, int dy){
	hide();
	x += dx;
	y += dy;
	show();
}
string Pixel::toString(){
	string ret;
	char buff[100];
	sprintf(buff, "pixel %d %d %d\n", x, y, colour);
	ret = buff;
	return ret;
}
/*
 * LINE
 */

Line::Line(SDL_Renderer* _ren, int a, int b, int c, int d){
	x1 = a;
	y1 = b;
	x2 = c;
	y2 = d;
	ren = _ren;
}
void Line::hide(){
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderDrawLine(ren, x1, y1, x2, y2);
}
void Line::show(){
	SDL_SetRenderDrawColor(ren, 0, 192, 0, 255);
	SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}
void Line::move(int dx, int dy){
	hide();
	x1 += dx;
	y1 += dy;
	x2 += dx;
	y2 += dy;
	show();
}
string Line::toString(){
	string ret;
	char buff[100];
	sprintf(buff, "line %d %d %d %d\n", x1, y1, x2, y2);
	ret = buff;
	return ret;
}

/*
 * RECT
 */

Rect::Rect(SDL_Renderer* _ren, int _x1, int _y1, int _x2, int _y2):Line(_ren, _x1, _y1, _x2, _y2){

}
void Rect::hide(){
	SDL_Rect rectangle;
	rectangle.x = x1;
	rectangle.y = y1;
	rectangle.w = x2 - x1;
	rectangle.h = y2 - y1;
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderDrawRect(ren, &rectangle);
}
void Rect::show(){
	SDL_Rect rectangle;
	rectangle.x = x1;
	rectangle.y = y1;
	rectangle.w = x2 - x1;
	rectangle.h = y2 - y1;
	SDL_SetRenderDrawColor(ren, 0, 192, 0, 255);
	SDL_RenderDrawRect(ren, &rectangle);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}
void Rect::move(int dx, int dy){
	hide();
	x1 += dx;
	y1 += dy;
	x2 += dx;
	y2 += dy;
	show();
}
string Rect::toString(){
	string ret;
	char buff[100];
	sprintf(buff, "rect %d %d %d %d\n", x1, y1, x2, y2);
	ret = buff;
	return ret;
}

/*
 * FIGURE
 */

Figure::Figure(SDL_Renderer* _ren){
    n = 0;
    ren  = _ren;
}
void Figure::add(Base* b){
    if(n < 100)
        shapes[n++] = b;
}
void Figure::show(){
    for(int i=0; i<n; i++)
        shapes[i]->show();
}
void Figure::hide(){
    for(int i=0; i<n; i++)
        shapes[i]->hide();
}
void Figure::move(int dx, int dy){
    for(int i=0; i<n; i++)
        shapes[i]->move(dx, dy);
}
void Figure::saveToFile(char* filename){
	ofstream outfile;
	outfile.open(filename);
	for(int i=0; i<n; i++)
		outfile << shapes[i]->toString();
	outfile.close();
}
string Figure::toString(){
	string ret="STUB";
	return ret;
}
void Figure::loadFromFile(char* filename){
	ifstream infile;
	string iline;
	infile.open(filename);
	while(getline(infile, iline)){
		istringstream iss(iline);
		string shape;
		iss >> shape;
		if(shape == "pixel"){
			int x, y, c;
			iss >> x >> y >> c;
			add((Base*)(new Pixel(ren, x, y, c)));
		} else if(shape == "line"){
			int x1, y1, x2, y2;
			iss >> x1 >> y1 >> x2 >> y2;
			add((Base*)(new Line(ren, x1, y1, x2, y2)));
		} else if(shape == "circle"){
			int x, y, r, c;
			iss >> x >> y >> r >> c;
			add((Base*)(new Circle(ren, x, y, r, c)));
		} else if(shape == "rect"){
			int x1, y1, x2, y2;
			iss >> x1 >> y1 >> x2 >> y2;
			add((Base*)(new Rect(ren, x1, y1, x2, y2)));
		}
	}
	infile.close();
}

/*
 * CIRCLE
 */
 
Circle::Circle(SDL_Renderer* _ren, int _x, int _y, int _r, int _c):Pixel(_ren, _x, _y, _c){
	radius = _r;
}

void Circle::hide(){
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	draw(ren, x, y, radius);
}

void Circle::show(){
	SDL_SetRenderDrawColor(ren, 0, 192, 0, 255);
	draw(ren, x, y, radius);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
}

void Circle::move(int dx, int dy){
	hide();
	x += dx;
	y += dy;
	show();
}

void Circle::draw(SDL_Renderer* _ren, int _x, int _y, int _r){
	/*
	 * Bresenham Circle Drawing algorithm
	 */
	int x = 0;
	int y = _r;
	int d = 1 - 2 * _r;
	int err = 0;
	while(y >= 0){
		// 4
		SDL_RenderDrawPoint(ren, _x + x, _y + y);
		// 1
		SDL_RenderDrawPoint(ren, _x + x, _y - y);
		// 3
		SDL_RenderDrawPoint(ren, _x - x, _y + y);
		// 2
		SDL_RenderDrawPoint(ren, _x - x, _y - y);
		err = 2 * (d + y) - 1;
		if((d < 0) && (err <= 0)){
			d += 2 * ++x + 1;
			continue;
		}
		err = 2 * (d - x) - 1;
		if((d > 0) && (err > 0)){
			d += 1 - 2 * --y;
			continue;
		}
		x++;
		d += 2 * (x - y);
		y--;
	}
}
string Circle::toString(){
	string ret;
	char buff[100];
	sprintf(buff, "circle %d %d %d %d\n", x, y, radius, colour);
	ret = buff;
	return ret;
}
