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
      specular 0.5
      roughness 0.02
      ambient 0.95
      diffuse 0.95
      reflection {
         0.25
      }
   }
}

#declare My_Fluid_Interior =
interior {
    ior 1.33
    fade_distance 10
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Fluid_Material =
material {
        texture { My_Fluid_Texture  }
        interior { My_Fluid_Interior  }  
}

#declare Dislocation_X = 50;
#declare My_Focus_Position = <0, -100.0000, 0.0000>;
#declare My_Camera_Location = <Dislocation_X, -20.0000, -700.0000>;
              
fog {
        fog_type   2
        fog_offset 100
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

light_source {
        <-100.0, 1000.0, -2500.0> colour White
        spotlight
        point_at My_Focus_Position
        radius 40
        falloff 50
        fade_distance 4000
        fade_power 1            
        area_light <150, 0, 0>, <0, 0, 150>, 8, 8
        adaptive 1
}

/* Floor */
#declare Floor =
plane {
   z, 0
        matrix <square_slope__m_trzaTranMatrix00,
                square_slope__m_trzaTranMatrix01,
                square_slope__m_trzaTranMatrix02,
                square_slope__m_trzaTranMatrix10,
                square_slope__m_trzaTranMatrix11,
                square_slope__m_trzaTranMatrix12,
                square_slope__m_trzaTranMatrix20,
                square_slope__m_trzaTranMatrix21,
                square_slope__m_trzaTranMatrix22,
                square_slope__m_trzaTranMatrix30,
                square_slope__m_trzaTranMatrix31,
                square_slope__m_trzaTranMatrix32
                >
   pigment {
        color rgb <0.5, 0.9, 0.5>
   }
}

object { Floor translate square_slope__m_bssCtrPos}               
