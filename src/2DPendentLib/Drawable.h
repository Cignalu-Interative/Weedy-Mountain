#pragma once
//Version 1.0.1

class Render;
class Window;
class Camera;
class Drawable
{
public:
	//Fonction virtuelle pure pour dessiner l'objet
	virtual void Draw(Render* f_render, Window* f_window, Camera* p_camera) = 0;
};