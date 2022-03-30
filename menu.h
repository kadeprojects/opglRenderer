#include "objects.h"


class Menu {
public:
    std::vector<Object*> objects;
    virtual void create() {};
    virtual void update() = 0;

    void draw() {
        for(Object* obj : objects)
            obj->draw();
    };

    void createObject(Object* ob) {
        objects.push_back(ob);
    }
    void deleteObject(Object* ob) {
        for(int i = 0; i < objects.size(); i++)
        {
            if (objects[i]->id == ob->id)
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
    static Menu* currentMenu;

    static void switchMenu(Menu* ofMenu, bool checkForPrev = true)
    {
        printf("\nchanging menu");
        KeyboardManager::removeAllCallbacks();
        if (checkForPrev)
        {   
            for(Object* ob : currentMenu->objects)
                currentMenu->deleteObject(ob);
            delete currentMenu;
        }
        currentMenu = ofMenu;
        currentMenu->create();
        printf("\nswitched!");
    }
};

Menu* MenuManager::currentMenu;

class Gameplay : public Menu {
public:
    SpriteSheet* sheet;
    std::vector<Sprite*> sprites;

    void key(int key, int action, int mods)
    {

    }

    void create() {
        sheet = new SpriteSheet("Assets/tileset.png");
        Sprite* spr = new Sprite(0,0,sheet);
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