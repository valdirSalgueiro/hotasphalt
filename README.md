# Green juice team engine
Homemade cross platform Open GLES 2.0 engine codenamed "greenjuice team engine". Its C based syntax for the core provides simple of use but still powerful API.

# Current supported platforms
![alt](https://raw.githubusercontent.com/valdirSalgueiro/sgsCrossPlatform/master/promotion/dl_android.png)
![alt](https://raw.githubusercontent.com/valdirSalgueiro/sgsCrossPlatform/master/promotion/dl_apple.png)
![alt](https://raw.githubusercontent.com/valdirSalgueiro/sgsCrossPlatform/master/promotion/dl_html5.gif)
![alt](https://raw.githubusercontent.com/valdirSalgueiro/sgsCrossPlatform/master/promotion/dl_uwp.png)
![alt](https://raw.githubusercontent.com/valdirSalgueiro/sgsCrossPlatform/master/promotion/xbox-icon-21.png)

# Quick start

## Requirements
Visual studio 2017 with c++ and mobile native modules installed

## Project structure 

The Visual studio 2017 solution file comes with projects for android, UWP(Windows and xbox) and ios platforms. These projects contain specifics for each platform. There is also a project that contains shared content for all these projects **"GreenJuiceTeam.Shared"** where you should put your game logic. It already contains some libraries to ease some common activities like font rendering, sound and low level rendering. 

The shared project contains a *Engine.cpp* class that we will use to build our first example. These class is instantiated in each of the platform example.

## Drawing a sprite

Engine cpp contains 3 main methods:
1. init() 
  Contains all your initialization code here.
2. update()
  Contains frame update logic.
3. render()
  Contains frame render logic.
  
Lets draw the green juice team logo just for fun. First lets load the logo image file (for now only PNG support is given) inside our *init* method.

```cpp
void Engine::init(int width_, int height_)
{
	startBatch(width_, height_); //important! need for rendering
	logo = new glImage(); //initialize our image object
	glLoadSprite("logo.png", logo); //load png file, important: files must be power of two images!
}
```

now we can render it at (0,0) in our render method:

```cpp
void Engine::render(float time) {
	spriteBatchBegin();
	glSprite(0, 0, GL2D_NONE, logo); // glsprite(x,y,MODE,image)
	spriteBatchEnd();
}
```

and thats it! :)

To see it running, set one of the application platforms(UWP, IOS or android) as your initiliazing project:

# Libraries
- Freetype
- Libpng
- Opengl Es 2
- EGL
- GLFW
- Zlib
- Tinyxml
- Emscripten

# Credits
- Valdir Salgueiro - Developer

# License
- Source code : MIT
