#ifndef MELODY_H
#define MELODY_H

#include "tones.h"

int getCoin[][2] =
{
    {tones[4][B],tones[5][E]}, // tones
    {8,8}  // beats
};

int bonusUp[][6] =
{
    {tones[4][G],tones[4][A], tones[5][E],tones[5][C],tones[5][D],tones[5][G]}, // tones
    {8          ,8          ,8           ,8          ,8          ,8}  // beats
};


int superMario3_1[][15] =
{
    {tones[4][FS],tones[4][G],tones[4][G],tones[4][FS],tones[4][G],tones[4][G],tones[4][FS],tones[4][G],tones[4][A],tones[4][GS],tones[4][A],tones[4][B],tones[4][A],tones[4][G],tones[4][FS]},
    {4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,2           ,4           ,4   ,4           },
};

int superMario3_2[][15] =
{
    {tones[4][FS],tones[4][G],tones[4][G],tones[4][FS],tones[4][G],tones[4][G],tones[4][FS],tones[4][G],tones[4][A],tones[4][GS],tones[4][A],tones[4][B],tones[5][D],tones[5][G],REST},
    {4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,3           ,4           ,4   ,5           },
};

int superMario3_3[][15] =
{
    {tones[4][FS],tones[4][G],tones[4][G],tones[4][FS],tones[4][G],tones[4][G],tones[4][FS],tones[4][G],tones[4][A],tones[4][GS],tones[4][A],tones[4][B],tones[4][A],tones[4][G],tones[4][F]},
    {4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,4           ,2           ,4           ,4   ,4           },
};

int superMario3_4[][14] =
{
    {tones[4][E],tones[4][F],tones[4][G],tones[4][A],tones[4][G],tones[4][F],tones[4][E],tones[4][D],tones[4][E],tones[4][F],tones[4][G],tones[4][F],tones[4][E],tones[4][D]},
    {4           ,4           ,4           ,2           ,4           ,4           ,4           ,4           ,4           ,4           ,2           ,2           ,4           ,2   },
};

int superMario3_5[][12] =
{
    {tones[4][C],tones[4][D],tones[4][E],tones[4][F],tones[4][C],tones[4][D],tones[3][B],tones[4][C],tones[2][F],tones[2][G],tones[2][E]},
    {4           ,4           ,4           ,2           ,4           ,4           ,4           ,4           ,4           ,4           ,2 },
};


int getStar[][2][10] =
{
    {
        {tones[5][C],tones[5][C],tones[5][C],tones[4][D],tones[5][C],tones[5][C],tones[4][D],tones[5][C],tones[4][D],tones[5][C]},
        {4          ,4          ,3          ,8           ,4          ,4          ,3          ,8         ,8          ,8          }
    },
    {
        {tones[4][B],tones[4][B],tones[4][B],tones[4][C],tones[4][B],tones[4][B],tones[4][C],tones[4][B],tones[4][C],tones[4][B]},
        {4          ,4          ,3         ,8           ,4          ,4          ,3          ,8         ,8          ,8          }
    },
};

int bubbleBubble[][2][8] =
{
    {
        {tones[5][F],tones[5][G],tones[5][A],tones[5][AS],tones[5][A],tones[5][G],tones[5][F],tones[5][A]},
        {2          ,2          ,2          ,4           ,4          ,2          ,8          ,4          }
    },
    {
        {tones[5][G],tones[5][F],tones[5][EF],tones[5][G],tones[5][F],tones[5][EF],tones[5][D],tones[5][F]},
        {4          ,4          ,4          ,4           ,4          ,8          ,4          ,2          }
    },
    {
        {tones[5][D],tones[5][C],tones[4][AS],tones[5][C],tones[5][D],tones[5][EF],tones[5][C],tones[5][D]},
        {8          ,4          ,4          ,4           ,2          ,8          ,4          ,2          }
    },
    {
        {tones[5][EF],tones[5][F],tones[5][F],tones[5][G],tones[5][A],tones[5][G],tones[5][F],tones[5][G]},
        {4          ,4          ,4          ,4           ,8          ,4          ,4          ,4          }
    },
    {
        {tones[5][A],tones[5][AS],tones[5][A],tones[5][G],tones[5][F],tones[5][A],tones[5][G],tones[5][F]},
        {4          ,4          ,4          ,2           ,8          ,4          ,4          ,4          }
    },
    {
        {tones[5][EF],tones[5][G],tones[5][F],tones[5][EF],tones[5][D],tones[5][F],tones[5][D],tones[5][C]},
        {4          ,4          ,4          ,8           ,4          ,2          ,8          ,4          }
    },
    {
        {tones[4][AS],tones[5][C],tones[5][D],tones[5][EF],tones[5][C],tones[5][D],tones[5][EF],tones[5][F]},
        {4          ,4          ,2          ,8           ,4          ,4          ,4          ,4          }
    },
    {
        {tones[5][F],tones[5][G],tones[5][A],tones[5][G],tones[5][F],tones[5][G],tones[5][A],tones[5][AS]},
        {4          ,4          ,8          ,4           ,4          ,4          ,4          ,1          }
    },
    /*
    {
        {tones[5][F],tones[5][G],tones[5][A],tones[6][C],tones[5][F],tones[5][G],tones[5][A],tones[6][D]},
        {4          ,4          ,4          ,1           ,4          ,4          ,4          ,1          }
    },
    {
        {tones[5][AS],tones[6][C],tones[6][D],tones[6][EF],tones[6][EF],tones[6][D],tones[6][C],tones[6][D]},
        {4          ,4          ,4          ,2           ,2          ,8          ,4          ,1          }
    },
    {
        {tones[6][D],tones[6][C],tones[5][F],tones[6][D],tones[4][F],tones[6][D],tones[5][AS],tones[5][AS]},
        {4          ,2          ,2          ,4           ,2          ,4          ,2          ,2          }
    },
*/
};






#endif // MELODY_H
