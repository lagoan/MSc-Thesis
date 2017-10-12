// Persistence Of Vision raytracer version 3.5 sample file.
// File by Dan Farmer
// Wine glass and chessboard
// Updated October, 1996
// Updated January, 1998  DCB

global_settings { assumed_gamma 1.0 number_of_waves 3 max_trace_level 5 }

#include "colors.inc"
#include "shapes.inc"
#include "textures.inc"
//#include "glass.inc"
#include "metals.inc"

camera {
   location <50.0, 5.0, -75.0>
   direction z
   up y
   right x*4/3
   look_at <-10.0, 18.0, 0.0>
}

light_source { <10.0, 50.0, 35.0> colour White }
light_source { <-35.0, 30.0, -150.0> colour White }


#declare I_Glass1 =                  // Use with Liquid
   interior{
      ior 1.45
      caustics 2
      fade_distance 40                   // only for this scene
      fade_power 1
   }


//#declare Wine = color red 1.0 filter 0.85;
#declare Wine = rgb 1.0 filter 0.85;

#declare Liquid = finish { reflection 0.95 }


#declare Wine2 =
union {
   sphere { 2*y, 1
      texture {
         finish { Liquid }
         pigment { Wine }
      }
   }
   scale <14.9, 9.5, 14.9>
   translate 14.0*y
   interior { I_Glass1}
}

object { Wine2 }

/* Ground plane */
plane {
   y, 0.0
   texture {
      pigment {
         checker color red 1.0 green 0.1 blue 0.1
                 color red 0.8 green 0.8 blue 0.8
         scale 5
      }
   }
/*   texture {
      pigment { RichBlue }
      normal {
         quilted 0.45
         control0 1 control1 1
         scale <0.45, 1, 0.45>
      }
   }
*/
}

