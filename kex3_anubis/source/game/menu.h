//
// Copyright(C) 2014-2015 Samuel Villarreal
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

#ifndef __MENU_H__
#define __MENU_H__

typedef enum
{
    MENU_QUITCONFIRM    = 0,
    MENU_BINDINGS,
    MENU_TRAVEL,
    MENU_STARTUP_NOTICE,
    MENU_PAUSE,
    MENU_OPTIONS,
    MENU_MAINCONFIRM,
    MENU_INPUT,
    MENU_AUDIO,
    MENU_MOUSE,
    MENU_JOYSTICK,
    NUMMENUS
} menus_t;

//-----------------------------------------------------------------------------
//
// kexMenuObject
//
//-----------------------------------------------------------------------------

class kexMenu;
class kexMenuObject;

typedef void(kexMenu::*selectCallback_t)(kexMenuObject*);

BEGIN_EXTENDED_KEX_CLASS(kexMenuObject, kexObject);
public:
    kexMenuObject(void);

    virtual void                    Draw(void) = 0;
    virtual void                    Tick(void) = 0;
    virtual bool                    CheckMouseSelect(const float mx, const float my) = 0;
    virtual void                    Reset(void);

    void                            DrawLabel(void);

    typedef enum
    {
        MITA_LEFT   = 0,
        MITA_CENTER,
        MITA_RIGHT
    } menuItemTextAlign_t;

    float                           x;
    float                           y;
    float                           w;
    float                           h;
    float                           labelWidth;
    float                           labelHeight;
    kexStr                          label;
    menuItemTextAlign_t             textAlignment;
    uint                            index;
    bool                            bSelected;
    kexMenu                         *menu;

    selectCallback_t                Callback;
END_KEX_CLASS();

BEGIN_EXTENDED_KEX_CLASS(kexMenu, kexObject);
public:
    kexMenu(void);

    virtual void                Init(void);
    virtual void                Display(void);
    virtual void                Reset(void);
    virtual void                Update(void);
    virtual bool                ProcessInput(inputEvent_t *ev);

    kexMenuObject               *AllocateMenuObject(const char *className);
    void                        DrawItems(void);
    void                        UpdateItems(void);

    kexArray<kexMenuObject*>    menuObjects;
    uint                        itemIndex;
    int                         selectedItem;
END_KEX_CLASS();

#define DEFINE_MENU_CLASS(name)   \
BEGIN_EXTENDED_KEX_CLASS(name, kexMenu);  \
public: \
name(void)

#define END_MENU_CLASS  END_KEX_CLASS

#define DECLARE_MENU_CLASS(name, id)    \
DECLARE_KEX_CLASS(name, kexMenu)  \
static name local_ ## name;   \
name::name(void)    \
{   \
kexGameLocal::menus[id] = this;   \
}

class kexMenuItem;
typedef void(*menuItemLerpDone_t)(kexMenuItem*);

//-----------------------------------------------------------------------------
//
// kexMenuItem
//
//-----------------------------------------------------------------------------

BEGIN_EXTENDED_KEX_CLASS(kexMenuItem, kexObject);
public:
    kexMenuItem(void);
    kexMenuItem(const char *label, const float x, const float y, const float scale,
                menuItemLerpDone_t callback = NULL);
    ~kexMenuItem(void);

    virtual void                    Draw(void);
    virtual void                    Tick(void);
    virtual void                    Select(const bool b) { bSelected = b; }

    void                            DrawSmallString(const char *string, float x, float y, float scale,
                                                    bool center, bool flash);
    void                            DrawBigString(const char *string, float x, float y, float scale,
                                                  bool center, bool flash);
    void                            LerpTo(const float destx, const float desty);
    void                            LerpTo(const float destx);

    const bool                      IsHighlighted(void) const { return bHighLighted; }
    void                            Toggle(const bool b) { bDisabled = !b; }

    virtual kexMenuItem             &operator=(const kexMenuItem &item);

    float                           x;
    float                           y;
    float                           scale;

    menuItemLerpDone_t              lerpCallback;

protected:
    void                            Move(void);
    virtual bool                    OnCursor(void);

    float                           highlightTime;
    kexStr                          label;
    bool                            bDisabled;
    bool                            bSelected;
    bool                            bInteract;
    bool                            bLerping;
    bool                            bHighLighted;
    float                           time;
    float                           startX;
    float                           startY;
    float                           destX;
    float                           destY;
END_KEX_CLASS();

#endif
