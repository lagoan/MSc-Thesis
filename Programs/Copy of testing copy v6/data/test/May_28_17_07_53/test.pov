
#include "colors.inc"

    sphere {
         < -2.5, 2.5, 2.5, > 1.5
         pigment {
              color  Red
         }
         finish {
              phong  1
         }
    }


    sphere {
         < 2, 1, -1 >, 0.6
         pigment {
              color  Yellow
         }
         finish {
              phong  1
         }
    }


    sphere {
         0*x, 1
         pigment {
              color  Blue
         }
         finish {
              phong  1
         }
    }


    sphere {
         < 3, 5, 9 >, 2.5
         pigment {
              color  Pink
         }
         finish {
              phong  1
         }
    }


    box {
         < -4, -1.5, 0.5 >, < -2, -0.5, 1.5 >
         pigment {
              color  Yellow
         }
         finish {
              phong  1
         }
    }

 
    box {
         < 2.5, -2, -4 >, < 4, -1, 4 >    
            rotate      30*x
         pigment {
              color  Magenta
         }
         finish {
              phong  1
         }
    }


    plane {
         y, -2
         pigment {
              color  Cyan
         }
         finish {
              phong  1
         }
    }


    light_source {
         < 0, 10, -3 >
         color  White 
         rotate x*-90
    }

    camera {
         location  < 0, 1, -8 >
         direction z
         up        y
         right     4/3*x
    }

