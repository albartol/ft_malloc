#ifndef _COLORS
# define _COLORS

// TEXT COLOR
# define BLACK "\033[1;30m"
# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN "\033[1;36m"
# define WHITE "\033[1;37m"
# define NC "\033[0m"

# define BRIGHT_BLACK  "\033[90m"
# define BRIGHT_RED    "\033[91m"
# define BRIGHT_GREEN  "\033[92m"
# define BRIGHT_YELLOW "\033[93m"
# define BRIGHT_BLUE   "\033[94m"
# define BRIGHT_MAGENTA "\033[95m"
# define BRIGHT_CYAN   "\033[96m"
# define BRIGHT_WHITE  "\033[97m"

// BACKGROUND COLOR
# define BG_BLACK      "\033[40m"
# define BG_RED        "\033[41m"
# define BG_GREEN      "\033[42m"
# define BG_YELLOW     "\033[43m"
# define BG_BLUE       "\033[44m"
# define BG_MAGENTA    "\033[45m"
# define BG_CYAN       "\033[46m"
# define BG_WHITE      "\033[47m"

# define BG_BRIGHT_BLACK   "\033[100m"
# define BG_BRIGHT_RED     "\033[101m"
# define BG_BRIGHT_GREEN   "\033[102m"
# define BG_BRIGHT_YELLOW  "\033[103m"
# define BG_BRIGHT_BLUE    "\033[104m"
# define BG_BRIGHT_MAGENTA "\033[105m"
# define BG_BRIGHT_CYAN    "\033[106m"
# define BG_BRIGHT_WHITE   "\033[107m"

// 256-Color
// Example usage macros for 256-color mode
# define COLOR_256_FG(N) "\033[38;5;" #N "m"
# define COLOR_256_BG(N) "\033[48;5;" #N "m"

// Example usage macros for 256-color mode
# define ORANGE_256  "\033[38;5;208m"
# define PINK_256    "\033[38;5;205m"
# define TEAL_256    "\033[38;5;44m"
# define PURPLE_256   "\033[38;5;14m"
# define RED_256     "\033[38;5;1m"
# define GREEN_256   "\033[38;5;74m"
# define BLUE_256    "\033[38;5;37m"
# define MAGENTA_256 "\033[38;5;147m"
# define CYAN_256    "\033[38;5;149m"
# define YELLOW_256   "\033[38;5;197m"
# define WHITE_256   "\033[38;5;15m"
# define BLACK_256   "\033[38;5;1m"

// True Color (16 million colors) using RGB values directly
// Format: \033[38;2;R;G;Bm for foreground
// Format: \033[48;2;R;G;Bm for background
# define TRUE_COLOR_FG(R, G, B) "\033[38;2;" #R ";" #G ";" #B "m"
# define TRUE_COLOR_BG(R, G, B) "\033[48;2;" #R ";" #G ";" #B "m"

// Example: Custom Gold
# define GOLD_TEXT "\033[38;2;255;215;0m"

#endif // !_COLORS
