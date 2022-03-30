#include "objects.h"


class Menu {
public:
    std::vector<Object> objects;
    void create() {};
    void update() {};

    void draw() {
        for(Object obj : objects)
            obj.draw();
    };

    void createObject(Object ob) {
        objects.push_back(ob);
    }
    void deleteObject(Object ob) {
        for(int i = 0; i < objects.size(); i++)
        {
            if (objects[i].id == ob.id)
            {
                // auto freed since this a stack variable
                objects.erase(objects.begin() + i);
                break;
            }
        }
    }
};

class MenuManager {
public:
    static Menu currentMenu;

    static void switchMenu(Menu ofMenu, bool checkForPrev = true)
    {
        KeyboardManager::removeAllCallbacks();
        if (checkForPrev)
        {   
            for(Object ob : currentMenu.objects)
                currentMenu.deleteObject(ob);
        }
        currentMenu = ofMenu;
        ofMenu.create();
    }
};

Menu MenuManager::currentMenu;

class Editor : public Menu {
public:
    void update()
    {
        
    }
};

class Gameplay : public Menu {
public:
    SpriteSheet* sheet;
    std::vector<Sprite> sprites;

    void key(int key, int action, int mods)
    {

    }

    void create() {
        sheet = new SpriteSheet("Assets/tileset.png");
        Sprite spr = Sprite(0,0,sheet);
        createObject(spr);

        KeyboardManager::registerCallback((keyPress)&key);
    }
    void update() {
 
    }

    ~Gameplay()
    {
        delete sheet;
    }
};