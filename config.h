/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"
#define TERMCLASS "st-256color"

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows; 3 is min pixels needed for smooth rounded corners */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel		= 0;
static const char *fonts[]          = { "Hack Nerd Font:size=11", "JoyPixels:pixelsize:11:antialias=true:autohint=true"};
static const char dmenufont[]       = "Hack Nerd Font:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
 	/* class           instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
 	{ "Gimp",          NULL,     NULL,           0,         1,          0,           0,        -1 },
 	{ "Firefox",       NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
 	{ TERMCLASS,       NULL,     NULL,           0,         0,          1,           0,        -1 },
 	{ NULL,            NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *upvol[] = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "5%+", NULL };
static const char *downvol[] = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "5%-", NULL };
static const char *mutevol[] = { "/usr/bin/wpctl", "set-mute", "@DEFAULT_AUDIO_SINK@", "toggle", NULL };

static Key keys[] = {
	/* modifier                     key                function        argument */
	{ MODKEY,                       XK_p,              spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,         spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,              togglebar,      {0} },
	{ MODKEY,                       XK_j,              focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,              focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,              incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,              incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,              setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,              setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_minus,          incrgaps,       {.i = +1 } },
	{ MODKEY,                       XK_equal,          incrgaps,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_minus,          incrogaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,          incrogaps,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_minus,          incrigaps,      {.i = +1 } },
	{ MODKEY|ControlMask,           XK_equal,          incrigaps,      {.i = -1 } },
	{ MODKEY,                       XK_bracketleft,    setborderpx, {.i = +1 } },
	{ MODKEY,                       XK_bracketright,   setborderpx, {.i = -1 } },
	{ MODKEY,                       XK_backslash,      setborderpx, {.i = 0 } },
	{ MODKEY,                       XK_0,              togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_0,              defaultgaps,    {0} },
	{ MODKEY,                       XK_Return,         zoom,           {0} },
	{ MODKEY,                       XK_Tab,            view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,              killclient,     {0} },
	{ MODKEY,                       XK_t,              setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,              setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,              setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,              setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,              setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_space,          togglefloating, {0} },
	{ MODKEY,                       XK_comma,          focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,         focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,          tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,         tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask,           XK_b,              spawn,          SHCMD("xdotool type $( grep -v '^#' ~/docs/random/bookmarks.txt | dmenu -i -l 50 | cut -d' ' -f1 )") },
	{ MODKEY|ControlMask,           XK_l,              spawn,          SHCMD("xdotool type \"cat $( /usr/bin/ls -d ~/docs/lyrics/* | dmenu -i -l 50)\"") },
	{ MODKEY,                       XK_b,              spawn,          SHCMD("firefox") },
	{ MODKEY,                       XK_e,              spawn,          SHCMD("emacsclient -c -a 'emacs'") },
	{ MODKEY,                       XK_v,              spawn,          SHCMD("st pulsemixer") },
	{ MODKEY,                       XK_n,              spawn,          SHCMD("st newsboat") },
	{ MODKEY,                       XK_Print,          spawn,          SHCMD("sleep 0.3s; scrot -s -e 'mv $f ~/pics/'") },
	{ 0,                            XK_Print,          spawn,          SHCMD("scrot -u -e 'mv $f ~/pics/'") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	{ 0,    XF86XK_AudioRaiseVolume,    spawn, {.v = upvol} },
	{ 0,	XF86XK_AudioLowerVolume,    spawn, {.v = downvol} },
	{ 0,	XF86XK_AudioMute,           spawn, {.v = mutevol} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

