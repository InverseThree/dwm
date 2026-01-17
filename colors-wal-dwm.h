static const char norm_fg[] = "#bd7eff";
static const char norm_bg[] = "#270042";
static const char norm_border[] = "#693faa";

static const char sel_fg[] = "#bd7eff";
static const char sel_bg[] = "#55007f";
static const char sel_border[] = "#9500ff";

static const char urg_fg[] = "#bd7eff";
static const char urg_bg[] = "#4a24aa";
static const char urg_border[] = "#4a24aa";

static const char *colors[][3]      = {
    /*               fg           bg         border                         */
    [SchemeNorm] = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]  = { sel_fg,      sel_bg,    sel_border },  // the focused win
    [SchemeUrg] =  { urg_fg,      urg_bg,    urg_border },
};
