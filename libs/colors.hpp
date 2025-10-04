#ifndef __COLORS_HPP__
#define __COLORS_HPP__

// Cores para o terminal
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

// Estilos de texto
#define STYLE_BOLD      "\033[1m"
#define STYLE_DIM       "\033[2m"

// Cores específicas para logs
#define LOG_INFO_COLOR    COLOR_CYAN
#define LOG_WARN_COLOR    COLOR_YELLOW
#define LOG_DEBUG_COLOR   COLOR_MAGENTA
#define LOG_SUCCESS_COLOR COLOR_GREEN
#define LOG_ERROR_COLOR   COLOR_RED

// Configuração - defina como 0 para desativar cores
#ifndef LOG_COLORS_ENABLED
#define LOG_COLORS_ENABLED 1
#endif

// Macros para aplicar cores condicionalmente
#if LOG_COLORS_ENABLED
    #define APPLY_INFO_COLOR(x)    LOG_INFO_COLOR x COLOR_RESET
    #define APPLY_WARN_COLOR(x)    LOG_WARN_COLOR x COLOR_RESET
    #define APPLY_DEBUG_COLOR(x)   LOG_DEBUG_COLOR x COLOR_RESET
    #define APPLY_SUCCESS_COLOR(x) LOG_SUCCESS_COLOR x COLOR_RESET
    #define APPLY_ERROR_COLOR(x)   LOG_ERROR_COLOR x COLOR_RESET
    #define APPLY_TIME_COLOR(x)    COLOR_WHITE STYLE_DIM x COLOR_RESET
#else
    #define APPLY_INFO_COLOR(x)    x
    #define APPLY_WARN_COLOR(x)    x
    #define APPLY_DEBUG_COLOR(x)   x
    #define APPLY_SUCCESS_COLOR(x) x
    #define APPLY_ERROR_COLOR(x)   x
    #define APPLY_TIME_COLOR(x)    x
#endif

#endif