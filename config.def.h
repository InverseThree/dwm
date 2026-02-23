/* See LICENSE file for copyright and license details. */

static const char thunderbird[]       = "org.mozilla.Thunderbird";

/* appearance */
static unsigned int borderpx          = 3;        /* border pixel of windows */
static unsigned int snap              = 32;       /* snap pixel */
static unsigned int gappih            = 10;       /* horiz inner gap between windows */
static unsigned int gappiv            = 10;       /* vert inner gap between windows */
static unsigned int gappoh            = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov            = 10;       /* vert outer gap between windows and screen edge */
static int swallowfloating            = 0;        /* 1 means swallow floating windows by default */
static const int swterminheritfs      = 1;        /* 1 terminal inherits fullscreen on unswallow, 0 otherwise */
static int smartgaps                  = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar              = 1;        /* 0 means no bar */
static const int topbar               = 1;        /* 0 means bottom bar */
static const char *fonts[]            = { "monospace:size=12", "fontawesome:size=36" };
static const char dmenufont[]         = "monospace:size=12, fontawesome:size=36";
static unsigned int baralpha        = 0xd0;
static unsigned int borderalpha     = OPAQUE;
static const char col_gray1[]         = "#222222";
static const char col_gray2[]         = "#444444";
static const char col_gray3[]         = "#bbbbbb";
static const char col_gray4[]         = "#eeeeee";
static const char col_cyan[]          = "#9500ff";
static const char urgbordercolor[]    = "#ff0000";

#include <X11/XF86keysym.h>

#include "colors-wal-dwm.h"

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance     title       tags mask    switchtotag    isfloating    isterminal    noswallow    monitor */
	{ "Gimp",            NULL,      NULL,       1 << 8,          3,             1,            0,            0,          -1 },
	{ "librewolf",       NULL,      NULL,       1 << 1,          3,             0,            0,           -1,          -1 },
	{ "steam",           NULL,      "Steam",    1 << 2,          0,             0,            0,            0,          -1 },
	{ "steam_app_0",     NULL,      NULL,       1 << 3,          0,             0,            0,            0,          -1 },
	{ "steam_app_1",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_2",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_3",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_4",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_5",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_6",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_7",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_8",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ "steam_app_9",     NULL,      NULL,       1 << 4,          3,             0,            0,            0,          -1 },
	{ thunderbird,       NULL,      NULL,       1 << 5,          3,             0,            0,            0,          -1 },
	{ "TelegramDesktop", NULL,      NULL,       1 << 5,          4,             0,            0,            0,          -1 },
	{ "Signal",          NULL,      NULL,       1 << 5,          4,             0,            0,            0,          -1 },
	{ "Alacritty",       NULL,      NULL,       0,               0,             0,            1,            0,          -1 },
	{ "kitty",           NULL,      NULL,       0,               0,             0,            1,            0,          -1 },
	{ NULL,              NULL, "Event Tester",  0,               0,             0,            0,            1,          -1 },
};

/* layout(s) */
static const float mfact        = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0;    /* 1 will force focus on the fullscreen window */
static const int refreshrate    = 120;  /* refresh rate (per second) for client move/resize */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile }, /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "D[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid }, 
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG)												\
	{1, {{MODKEY, KEY}},								view,           {.ui = 1 << TAG} },	\
	{1, {{MODKEY|ControlMask, KEY}},					toggleview,     {.ui = 1 << TAG} }, \
	{1, {{MODKEY|ShiftMask, KEY}},						tag,            {.ui = 1 << TAG} }, \
	{1, {{MODKEY|ControlMask|ShiftMask, KEY}},			toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "kitty", NULL };

static Keychord keychords[] = {
	/* Keys                                       function          argument */
	{ 1, {{ MODKEY, XK_r }},                      spawn,            {.v = (const char*[]){ "rofi-drun", NULL } } },
	{ 1, {{ MODKEY, XK_t }},                      spawn,            {.v = termcmd } },
	{ 1, {{ MODKEY|ControlMask, XK_b }},          togglebar,        {0} },
	{ 1, {{ MODKEY|ControlMask, XK_g }},          togglegaps,       {0} },
	{ 1, {{ MODKEY, XK_j }},                      focusstack,       {.i = +1 } },
	{ 1, {{ MODKEY, XK_k }},                      focusstack,       {.i = -1 } },
	{ 1, {{ MODKEY|ControlMask, XK_j }},          incnmaster,       {.i = +1 } },
	{ 1, {{ MODKEY|ControlMask, XK_k }},          incnmaster,       {.i = -1 } },
	{ 1, {{ MODKEY, XK_h }},                      setmfact,         {.f = -0.05} },
	{ 1, {{ MODKEY, XK_l }},                      setmfact,         {.f = +0.05} },
	{ 1, {{ MODKEY, XK_o }},                      setmfact,         {.f = 1.0 + mfact} },
	{ 1, {{ MODKEY|ControlMask, XK_h }},          setcfact,         {.f = +0.25} },
	{ 1, {{ MODKEY|ControlMask, XK_l }},          setcfact,         {.f = -0.25} },
	{ 1, {{ MODKEY|ControlMask, XK_o }},          setcfact,         {.f = 0.00} },
	{ 1, {{ MODKEY, XK_Return }},                 zoom,             {0} },
	{ 1, {{ MODKEY, XK_Tab }},                    view,             {0} },
	{ 1, {{ MODKEY|ControlMask, XK_c }},          killclient,       {0} },
	{ 1, {{ MODKEY|ControlMask, XK_t }},          setlayout,        {.v = &layouts[0]} },
	{ 1, {{ MODKEY|ControlMask, XK_m }},          setlayout,        {.v = &layouts[1]} },
	{ 1, {{ MODKEY, XK_space }},                  setlayout,        {0} },
	{ 1, {{ MODKEY|ControlMask, XK_space }},      togglefloating,   {0} },
	{ 1, {{ MODKEY, XK_0 }},                      view,             {.ui = ~0 } },
	{ 1, {{ MODKEY, XK_comma }},                  focusmon,         {.i = -1 } },
	{ 1, {{ MODKEY, XK_period }},                 focusmon,         {.i = +1 } },
	{ 1, {{ MODKEY|ControlMask, XK_comma }},      tagmon,           {.i = -1 } },
	{ 1, {{ MODKEY|ControlMask, XK_period }},     tagmon,           {.i = +1 } },
	TAGKEYS(                                      XK_1,             0)
	TAGKEYS(                                      XK_2,             1)
	TAGKEYS(                                      XK_3,             2)
	TAGKEYS(                                      XK_4,             3)
	TAGKEYS(                                      XK_5,             4)
	TAGKEYS(                                      XK_6,             5)
	TAGKEYS(                                      XK_7,             6)
	TAGKEYS(                                      XK_8,             7)
	TAGKEYS(                                      XK_9,             8)
	{ 1, {{ MODKEY|ControlMask, XK_Left }},       cyclelayout,      {.i = +1 } },
	{ 1, {{ MODKEY|ControlMask, XK_Right }},      cyclelayout,      {.i = -1 } },
	{ 1, {{ Mod1Mask, XK_Tab }},                  shiftviewclients, { .i = +1 } },
	{ 1, {{ Mod1Mask|ShiftMask, XK_Tab }},        shiftviewclients, { .i = -1 } },
	{ 1, {{ MODKEY|Mod1Mask, XK_c }},             killclient,       {.ui = 1} },  // kill unselect
    { 1, {{ MODKEY|Mod1Mask|ControlMask, XK_c }}, killclient,       {.ui = 2} },  // killall
	{ 1, {{ MODKEY|ControlMask, XK_s }},          togglesticky,     {0} },
	{ 1, {{ MODKEY|Mod1Mask|ControlMask, XK_q }}, quit,             {1} }, 
	{ 1, {{ MODKEY, XK_f }},                      spawn,            {.v = (const char*[]){ "kitty", "-e", "lf", NULL } } },
	{ 1, {{ MODKEY, XK_m }},                      spawn,            {.v = (const char*[]){ "kitty", "-e", "rmpc", NULL } } },
	{ 1, {{ MODKEY, XK_v }},                      spawn,            {.v = (const char*[]){ "kitty", "-e", "pavucontrol", NULL } } },
	{ 1, {{ MODKEY, XK_b }},                      spawn,            {.v = (const char*[]){ "kitty", "-e", "bluetui", NULL } } },
	{ 1, {{ MODKEY, XK_w }},                      spawn,            {.v = (const char*[]){ "librewolf", NULL } } },
	{ 1, {{ MODKEY|ControlMask, XK_w }},          spawn,            {.v = (const char*[]){ "librewolf", "--private-window", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask, XK_m }},             spawn,            {.v = (const char*[]){ "thunderbirdSucks", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask, XK_t }},             spawn,            {.v = (const char*[]){ "Telegram", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask, XK_s }},             spawn,            {.v = (const char*[]){ "signal-desktop", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask, XK_v }},             spawn,            {.v = (const char*[]){ "mullvad-vpn", NULL } } },
	{ 1, {{ MODKEY, XK_equal }},                  spawn,            {.v = (const char*[]){ "qalculate-gtk", NULL } } },
	{ 1, {{ MODKEY, XK_i }},                      spawn,            {.v = (const char*[]){ "ronema", NULL } } },
	{ 1, {{ MODKEY, XK_c }},                      spawn,            {.v = (const char*[]){ "rofi-clip", NULL } } },
	{ 1, {{ MODKEY, XK_u }},                      spawn,            {.v = (const char*[]){ "rofi-usb", NULL } } },
	{ 1, {{ MODKEY, XK_n }},                      spawn,            {.v = (const char*[]){ "rofi_notes", NULL } } },
	{ 1, {{ MODKEY|ShiftMask, XK_q }},            spawn,            {.v = (const char*[]){ "rofi-power", NULL } } },
	{ 1, {{ MODKEY|ControlMask, XK_a }},          spawn,            {.v = (const char*[]){ "recording-auto", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask|ControlMask, XK_m }}, spawn,            {.v = (const char*[]){ "recording-merge", NULL } } },
	{ 1, {{ MODKEY|ControlMask, XK_r }},          spawn,            {.v = (const char*[]){ "recording-start", NULL } } },
	{ 1, {{ MODKEY|ShiftMask, XK_r }},            spawn,            {.v = (const char*[]){ "recording-window", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask, XK_r }},             spawn,            {.v = (const char*[]){ "recording-audio", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask|ControlMask, XK_r }}, spawn,            {.v = (const char*[]){ "recording-display-audio", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask|ShiftMask, XK_r }},   spawn,            {.v = (const char*[]){ "recording-window-audio", NULL } } },
	{ 1, {{ 0, XK_Print }},                       spawn,            {.v = (const char*[]){ "screenshot", "color", NULL } } },
	{ 1, {{ ControlMask, XK_Print }},             spawn,            {.v = (const char*[]){ "screenshot", NULL } } },
	{ 1, {{ ShiftMask, XK_Print }},               spawn,            {.v = (const char*[]){ "screenshot", "window", NULL } } },
	{ 1, {{ ShiftMask|ControlMask, XK_Print }},   spawn,            {.v = (const char*[]){ "screenshot", "full", NULL } } },
	{ 1, {{ Mod1Mask, XK_Print }},                spawn,            {.v = (const char*[]){ "farmXV", NULL } } },
	{ 1, {{ MODKEY|ControlMask, XK_c }},          spawn,            {.v = (const char*[]){ "clip", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask, XK_c }},             spawn,            {.v = (const char*[]){ "clip", NULL } } },
	{ 1, {{ MODKEY|Mod1Mask|ControlMask, XK_c }}, spawn,            {.v = (const char*[]){ "clip", NULL } } },
	{ 1, {{ 0, XF86XK_AudioPlay }},               spawn,            {.v = (const char*[]){ "playerctl", "play", NULL } } },
	{ 1, {{ 0, XF86XK_AudioPause }},              spawn,            {.v = (const char*[]){ "playerctl", "pause", NULL } } },
	{ 1, {{ 0, XF86XK_AudioNext }},               spawn,            {.v = (const char*[]){ "playerctl", "next", NULL } } },
	{ 1, {{ 0, XF86XK_AudioPrev }},               spawn,            {.v = (const char*[]){ "playerctl", "previous", NULL } } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask          button          function        argument */
	{ ClkLtSymbol,          0,                  Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,                  Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                  Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                  Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,                  Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,                  Button3,        sigstatusbar,   {.i = 3} },
	{ ClkStatusText,        0,                  Button4,        sigstatusbar,   {.i = 4} },
	{ ClkStatusText,        0,                  Button5,        sigstatusbar,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,          Button1,        sigstatusbar,   {.i = 6} },
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,             Button1,        moveorplace,    {.i = 1} },
	{ ClkClientWin,         MODKEY,             Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,             Button3,        resizeorfacts,  {0} },
	{ ClkClientWin,         MODKEY|ControlMask, Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,                  Button1,        view,           {0} },
	{ ClkTagBar,            0,                  Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,             Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,             Button3,        toggletag,      {0} },
};

