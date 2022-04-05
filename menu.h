#include "objects.h"

class Menu {
public:
    std::vector<Object*> objects;
    bool created = false;
    virtual void create() {};
    virtual void update() = 0;
    virtual void key(int key,int mods) {};
    
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
                objects.erase(objects.begin() + i);
                break;
            }
        }
        delete ob;
    }
};

class MenuManager {
public:
    static Menu* currentMenu;

    static void switchMenu(Menu* ofMenu, bool checkForPrev = true)
    {
        printf("\nchanging menu");
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

class KeyboardManager {
public:
    static void disperseCallbacks(int key, int action, int mods)
    {
        MenuManager::currentMenu->key(key,mods);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(action == GLFW_PRESS)
        {
            disperseCallbacks(key,action,mods);
        }
    }
};

class EditorMenu : public Menu {
public:
    SpriteSheet* sheet;
    Sprite* highlight;

    std::vector<Sprite*> sprites;

    int lastx, lasty;

    int spriteIndex = 0;

    void key(int key,int mods)
    {
        printf("\nKey: %d", key);
        int max = sheet->w / 32;
        switch(key)
        {
            case GLFW_KEY_RIGHT_SHIFT:
                spriteIndex++;
                if (spriteIndex > max)
                    spriteIndex = 0;
                printf("\nNew index: %d",spriteIndex);
            break;
            case GLFW_KEY_LEFT_SHIFT:
                spriteIndex--;
                if (spriteIndex < 0)
                    spriteIndex = max;
                printf("\nNew index: %d",spriteIndex);
            break;
        }
    }

    void create() {
        sheet = new SpriteSheet("Assets/tileset.png");
        highlight = new Sprite(0,0,"Assets/highlight.png");
        createObject(highlight);
        created = true;
    }
    void update() {
        if (!created)
            return;
        double mx, my;
        glfw_backend::getMousePosition(&mx,&my);

        highlight->x = Helpers::Align(mx,32);
        highlight->y = Helpers::Align(my,32);

        if (glfw_backend::isMouseDown())
        {
            if (lastx != highlight->x && lasty != highlight->y)
            {
                bool keepgoin = true;
                for(Sprite* sprr : sprites)
                {
                    if (sprr->x == highlight->x && sprr->y == highlight->y)
                        keepgoin = false;
                }
                if (!keepgoin)
                    return;
                Sprite* spr = new Sprite(highlight->x,highlight->y, sheet, spriteIndex);
                sprites.push_back(spr);
                createObject(spr);
            }

        }
    }

    ~EditorMenu()
    {
        delete sheet;
    }

};

class Gameplay : public Menu {
public:
    SpriteSheet* sheet;
    std::vector<Sprite*> sprites;

    void key(int key,int mods)
    {
        switch(key)
        {
            case GLFW_KEY_1:
                MenuManager::switchMenu(new EditorMenu());
                break;
        }
    }

    void create() {
        sheet = new SpriteSheet("Assets/tileset.png");
        Sprite* spr = new Sprite(0,0,sheet);
        createObject(spr);

        Text* txt = new Text(40,40,"ARIAL.TTF",62,"test");
        createObject(txt);
    }
    void update() {

    }

    ~Gameplay()
    {
        delete sheet;
    }
};
