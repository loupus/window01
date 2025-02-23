#pragma once

// referance rectangle for surface area (width,height,top,height)
class  cRefRect
{
public:
	cRefRect() {}
	cRefRect(int awidth, int aheight) : width(awidth), height(aheight) {}
	cRefRect(int aleft, int atop, int awidth, int aheight) :left(aleft), top(atop), width(awidth), height(aheight) {}
	cRefRect(const  cRefRect& arect) :left(arect.left), top(arect.top), width(arect.width), height(arect.height) {}

public:
	int width = 0, height = 0, left = 0, top = 0;
};