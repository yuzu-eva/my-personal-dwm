/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"
#define TERMCLASS "st-256color"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int minwsz    = 20;		/* Minimal height of a client for smfact */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Hack Nerd Font:size=11", "JoyPixels:pixelsize:10:antialias=true:autohint=true"};
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
static const char *tags[] = { "1", "2", "3", "4" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
 	{ "Gimp",          NULL,     NULL,           0,         1,          0,           0,        -1 },
 	{ "Firefox",       NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "floating",	   NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "steam",         NULL,     NULL,           0,         1,          0,          -1,        -1 },
 	{ TERMCLASS,       NULL,     NULL,           0,         0,          1,           0,        -1 },
 	{ NULL,            NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const float smfact	 = 0.00; /* factor of tiled chlients [0.00..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
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
	{ MODKEY|ShiftMask|ControlMask, XK_b,              togglebar,      {0} },
	{ MODKEY,                       XK_j,              focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,              focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,              incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,              incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,              setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,              setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,              setsmfact,      {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_k,              setsmfact,      {.f = +0.05} },
	{ MODKEY,                       XK_Return,         zoom,           {0} },
	{ MODKEY,                       XK_Tab,            view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,              killclient,     {0} },
	{ MODKEY,                       XK_b,              setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,              setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,              setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_space,          togglefloating, {0} },
	{ MODKEY,                       XK_comma,          focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,         focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,          tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,         tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_f,              spawn,          {.v = (const char*[]){ "firefox", "-P", "default-release", NULL } } },
	{ MODKEY|ShiftMask,             XK_b,              spawn,          {.v = (const char*[]){ "firefox", "-P", "less-strict",  NULL } } },
	{ MODKEY|ShiftMask,             XK_e,              spawn,          {.v = (const char*[]){ "emacsclient", "-c", "-a", "emacs", NULL } } },
	{ MODKEY|ShiftMask,             XK_n,              spawn,          {.v = (const char*[]){ TERMINAL, "newsboat", NULL } } },
	{ MODKEY|ShiftMask,             XK_m,              spawn,          {.v = (const char*[]){ "mpdinfo", NULL } } },
	{ MODKEY|ShiftMask,             XK_v,              spawn,          {.v = (const char*[]){ TERMINAL, "pulsemixer", NULL } } },
	{ MODKEY|ControlMask,           XK_c,              spawn,          {.v = (const char*[]){ "mpdmenu", NULL } } },
	{ MODKEY|ControlMask,           XK_b,              spawn,          SHCMD("$BROWSER $(grep -v '^#' ~/.config/bookmarks | dmenu -i -l 50 | cut -d' ' -f1)") },
	{ MODKEY|ControlMask,           XK_q,              spawn,          SHCMD("~/.local/bin/qrshare") },
	{ MODKEY,                       XK_Print,          spawn,          SHCMD("sleep 0.3s; scrot -s -e 'mv $f ~/pics/'") },
	{ 0,                            XK_Print,          spawn,          SHCMD("scrot -u -e 'mv $f ~/pics/'") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	{ MODKEY|ShiftMask|ControlMask, XK_q,      quit,          {0} },

	{ 0,    XF86XK_AudioRaiseVolume,    spawn, {.v = upvol} },
	{ 0,	XF86XK_AudioLowerVolume,    spawn, {.v = downvol} },
	{ 0,	XF86XK_AudioMute,           spawn, {.v = mutevol} },
	{ 0,	XF86XK_AudioNext,           spawn, {.v = (const char*[]){ "/usr/bin/mpc", "next", NULL } } },
	{ 0,	XF86XK_AudioPrev,           spawn, {.v = (const char*[]){ "/usr/bin/mpc", "prev", NULL } } },
	{ 0,	XF86XK_AudioPlay,           spawn, {.v = (const char*[]){ "/usr/bin/mpc", "toggle", NULL } } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

