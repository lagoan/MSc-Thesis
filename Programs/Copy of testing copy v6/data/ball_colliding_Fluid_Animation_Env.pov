// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "woods.inc"

#declare display_particles_1 = 0; 
#declare display_particles_2 = 0;

#declare Object_Finish =
finish {
      specular 0.05
      ambient 0.25
      diffuse 0.25
}

#declare My_Fluid_Texture_1 =
texture {                                     
   pigment {color <0.25, 0.45, 0.25>  filter 0.995 }
   finish {
      specular 0.75
      roughness 0.001
      ambient 0.95
      diffuse 0.95
      reflection {
         0.01
      }
   }
}

#declare My_Fluid_Texture_2 =
texture {                                     
   pigment {color <0.75, 0.25, 0.25>  filter 0.5 }
   finish {
      specular 0.75
      roughness 0.001
      ambient 0.95
      diffuse 0.95
      reflection {
         0.01
      }
   }
}

#declare My_Fluid_Interior =
interior {
    ior 1.33
    fade_distance 50
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Fluid_Material_1 =
material {
        texture { My_Fluid_Texture_1  }
        interior { My_Fluid_Interior  }  
}

#declare My_Fluid_Material_2 =
material {
        texture { My_Fluid_Texture_2  }
        interior { My_Fluid_Interior  }  
}

#declare My_Focus_Position = <0, 0, 0>;
#declare My_Camera_Location = <0, 0, 500.0000>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source {
        <-10000, 10000, 10000> colour White*0.01
}

light_source {
        <400, 400, 400> colour White
        spotlight
        point_at My_Focus_Position
        radius 200000
        falloff 500
        fade_distance 10000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
}

/* Sky */
sky_sphere {
    pigment {
        gradient y
        color_map {
            [0 color White]//rgb <0.05, 0.05, 0.05>]
            [1 color Blue]
        }
        scale 2
        translate -1
    }
}

/* Floor */
#declare Floor_Thickness = -2;
#declare Floor_Image_Scale = 1000;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y - Floor_Thickness
   pigment {
        image_map {jpeg "Wood-Light-birch.jpg"}
        rotate <0, 80, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish { Object_Finish }
}

object { Floor }               
