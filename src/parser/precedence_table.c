//
// Created by Richa on 21-Nov-19.
//

#include "precedence_table.h"


precedence_table =
         {
        //   +-       /*     (       )       r       fc     var      $
/* +-  */ {REDUCE, SHIFT,  SHIFT,  REDUCE, XXXXXX, XXXXXX, SHIFT,  REDUCE},
/* /*  */ {REDUCE, REDUCE, SHIFT,  REDUCE, XXXXXX, XXXXXX, SHIFT,  REDUCE},
/*  (  */ {SHIFT,  XXXXXX, SHIFT,  EQUAL,  XXXXXX, XXXXXX, SHIFT,  ERROR },
/*  )  */ {REDUCE, XXXXXX, ERROR,  REDUCE, XXXXXX, XXXXXX, ERROR,  REDUCE},
/* rel */ {SHIFT,  XXXXXX, XXXXXX, XXXXXX, XXXXXX, XXXXXX, SHIFT,  REDUCE},
/* fc  */ {ERROR,  ERROR,  EQUAL,  ERROR,  ERROR,  ERROR,  ERROR,  REDUCE},
/* var */ {REDUCE, REDUCE, ERROR,  REDUCE, REDUCE, ERROR,  ERROR,  REDUCE},
/*  $  */ {SHIFT,  SHIFT,  SHIFT,  ERROR,  SHIFT,  ERROR,  SHIFT,  ERROR }};