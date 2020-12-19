#ifndef COLOR_H
#define COLOR_H
#include <string.h>
#define C_BLK "\e[0;30m"
#define C_RED "\e[0;31m"
#define C_GRN "\e[0;32m"
#define C_YEL "\e[0;33m"
#define C_BLU "\e[0;34m"
#define C_MAG "\e[0;35m"
#define C_CYN "\e[0;36m"
#define C_WHT "\e[0;37m"

//Regular bold text
#define C_BBLK "\e[1;30m"
#define C_BRED "\e[1;31m"
#define C_BGRN "\e[1;32m"
#define C_BYEL "\e[1;33m"
#define C_BBLU "\e[1;34m"
#define C_BMAG "\e[1;35m"
#define C_BCYN "\e[1;36m"
#define C_BWHT "\e[1;37m"

//Regular underline text
#define C_UBLK "\e[4;30m"
#define C_URED "\e[4;31m"
#define C_UGRN "\e[4;32m"
#define C_UYEL "\e[4;33m"
#define C_UBLU "\e[4;34m"
#define C_UMAG "\e[4;35m"
#define C_UCYN "\e[4;36m"
#define C_UWHT "\e[4;37m"

//Regular background
#define C_BLKB "\e[40m"
#define C_REDB "\e[41m"
#define C_GRNB "\e[42m"
#define C_YELB "\e[43m"
#define C_BLUB "\e[44m"
#define C_MAGB "\e[45m"
#define C_CYNB "\e[46m"
#define C_WHTB "\e[47m"

//High intensty background
#define C_BLKHB "\e[0;100m"
#define C_REDHB "\e[0;101m"
#define C_GRNHB "\e[0;102m"
#define C_YELHB "\e[0;103m"
#define C_BLUHB "\e[0;104m"
#define C_MAGHB "\e[0;105m"
#define C_CYNHB "\e[0;106m"
#define C_WHTHB "\e[0;107m"

//High intensty text
#define C_HBLK "\e[0;90m"
#define C_HRED "\e[0;91m"
#define C_HGRN "\e[0;92m"
#define C_HYEL "\e[0;93m"
#define C_HBLU "\e[0;94m"
#define C_HMAG "\e[0;95m"
#define C_HCYN "\e[0;96m"
#define C_HWHT "\e[0;97m"

//Bold high intensity text
#define C_BHBLK "\e[1;90m"
#define C_BHRED "\e[1;91m"
#define C_BHGRN "\e[1;92m"
#define C_BHYEL "\e[1;93m"
#define C_BHBLU "\e[1;94m"
#define C_BHMAG "\e[1;95m"
#define C_BHCYN "\e[1;96m"
#define C_BHWHT "\e[1;97m"

#define C_BLINK "\033[5m"
//Reset
#define C_RESET "\e[0m"
#endif

char* strfmt(char* cmt, char* content);