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

    std::vector<Object*> sprites;

    int lastx, lasty;

    int spriteIndex = 0;

    int selectInd = 0;

    void key(int key,int mods)
    {

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
        bool hovered = false;

        // Editor imgui
        ImGui::Begin("Object Properties");

        ImGui::SetWindowSize(ImVec2(400, 200));
        if (sprites.size() != 0)
        {
            Object* spr = sprites[selectInd];
            if (spr)
            {
                if (spr->specialTag == 0)
                {
                    Rect src = sheet->returnSrc(((Sprite*)spr)->spriteIndex);

                    float tl = src.x;
                    float br = tl + src.w;
                    ImGui::Image((ImTextureID)sheet->sheet->textureID, ImVec2(32, 32), ImVec2(tl, 0), ImVec2(br, 1));
                }
                ImGui::Text("X:");
                ImGui::InputInt("##InputDoublePropX", &spr->x, 32, 64);
                ImGui::Text("Y:");
                ImGui::InputInt("##InputDoublePropY", &spr->y, 32, 64);

                spr->x = Helpers::Align(spr->x, 32);
                spr->y = Helpers::Align(spr->y, 32);

                switch (spr->specialTag)
                {
                case 1: // text
                    Text* text = (Text*)spr;
                    ImGui::Text("Text:");
                    char* buf = (char*)malloc(sizeof(char*) * 620);
                    ImGui::InputText("##InputText", buf, 620);
                    text->text = std::string(buf);
                    break;
                }
            }
        }

        if (ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered())
            hovered = true;

        ImGui::End();

        ImGui::Begin("Assets", 0, ImGuiWindowFlags_NoResize);
        ImGui::SetWindowSize(ImVec2(sheet->w + 256, 128));

        for (int i = 0; i < sheet->w / 32; i++)
        {
            Rect src = sheet->returnSrc(i);

            float tl = src.x;
            float br = tl + src.w;

            ImGui::PushID(i * 200);
            if (ImGui::ImageButton((ImTextureID)sheet->sheet->textureID, ImVec2(32, 32), ImVec2(tl, 0), ImVec2(br, 1)))
            {
                spriteIndex = i;
                printf("\nSeleted: %d", i);
            }
            ImGui::PopID();
            ImGui::SameLine();
        }

        ImGui::NewLine();
        ImGui::Text("Selected: %d | Cursor: %d,%d", spriteIndex, highlight->x, highlight->y);

        if (ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered())
            hovered = true;

        ImGui::End();

        if (glfw_backend::isMouseDown() && !hovered)
        {
            if (lastx != highlight->x && lasty != highlight->y)
            {
                bool keepgoin = true;
                for (Object* sprr : sprites)
                {
                    if (sprr->x == highlight->x && sprr->y == highlight->y)
                    {
                        keepgoin = false;
                        break;
                    }
                    selectInd++;
                }
                if (keepgoin)
                {
                    Sprite* spr = nullptr;
                    Text* text = nullptr;
                    switch (spriteIndex)
                    {
                    default:
                        spr = new Sprite(highlight->x, highlight->y, sheet, spriteIndex);
                        sprites.push_back(spr);
                        createObject(spr);
                        break;
                    case 3:
                        text = new Text(highlight->x, highlight->y, "ARIAL.TTF", 16, "defaultText");
                        sprites.push_back(text);
                        createObject(text);
                        break;
                    }
                    
                }
            }

        }

        if (glfw_backend::isRightMouseDown() && !hovered)
        {
            for (Object* sprr : sprites)
            {
                if (sprr->x == highlight->x && sprr->y == highlight->y)
                {
                    deleteObject(sprr);
                    break;
                }
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
        created = true;
    }
    void update() {

    }

    ~Gameplay()
    {
        delete sheet;
    }
};
