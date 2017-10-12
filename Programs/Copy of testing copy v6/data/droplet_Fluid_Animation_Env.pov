// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"

#declare My_Glass_Texture =
texture {                                     
   pigment {rgb < 0.85, 0.85, 0.85> }
   finish {
      specular 0.05
      roughness 0.001
      ambient 0.05
      diffuse 0.05
      reflection {
         0.05
      }
   }
}

#declare My_Glass_Interior =
interior {
    ior 1.05
    fade_distance 1000
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Glass_Material =
material {
        texture { My_Glass_Texture  }
        interior { My_Glass_Interior  }  
}

#declare My_Fluid_Texture =
texture {                                     
   pigment {rgb < 0.5, 0.75, 0.5> }
   finish {
      specular 0.25
      roughness 0.02
      ambient 0.75
      diffuse 0.75
      reflection {
         0.05
      }
   }
}

#declare My_Fluid_Interior =
interior {
    ior 1.33
    fade_distance 5
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Fluid_Material =
material {
        texture { My_Fluid_Texture  }
        interior { My_Fluid_Interior  }  
}

#declare Dislocation_X = 0;
#declare My_Focus_Position = <0, 140.0000, 0.0000>;
#declare My_Camera_Location = <Dislocation_X, 200.0000, -500.0000>;
              
fog {
        fog_type   2
        fog_offset 1000
        fog_alt    1
        distance 1000
        color rgb <0, 0, 0>
}

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

#declare FadeDistance = 3000;
light_source {
        <-1000.0, 1000.0, -1500.0> colour White
        spotlight
        point_at My_Focus_Position
        radius 40
        falloff 50
        fade_distance FadeDistance
        fade_power 1            
        area_light <150, 0, 0>, <0, 0, 150>, 8, 8
        adaptive 1
}
/*
light_source {
        <1000.0, 1000.0, -2000.0> colour White
        spotlight
        point_at My_Focus_Position
        radius 40
        falloff 50
        fade_distance FadeDistance
        fade_power 1            
        area_light <150, 0, 0>, <0, 0, 150>, 8, 8
        adaptive 1
}
*/
/* Floor */
#declare FloorThickness = 10;
#declare Floor =
plane {
   y, floor__m_bssCtrPos.y - FloorThickness
   pigment {
        color rgb <0.5, 0.9, 0.5>
   }
}

object { Floor }               
