// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "woods.inc"

#declare Object_Image_Scale = 1000;
#declare Object_Finish =
finish {
      specular 0.05
      ambient 0.25
      diffuse 0.25
}
 
#declare My_Object_Material =
material {                                     
        texture {                                     
           pigment {color <0.95, 0.25, 0.25>}
           finish {
                specular 0.5
                roughness 0.01
                ambient 0.5
                diffuse 0.5
                reflection { 0.005 }
           }
        }
        interior {
           ior 1.5
           fade_distance 20.0
           fade_power 1
        }
}

#declare My_Fluid_Texture =
texture {                                     
   pigment {color <0.5, 0.8, 0.5>  filter 0.995 }
   finish {
      specular 0.5
      roughness 0.01
      ambient 0.75
      diffuse 0.75
      reflection {
         0.01
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

#declare Ball_Srfc_Thickness = 2;
#declare Moving_Ball = 
sphere {     
        rigid_ball_1__m_ovalCtrPos,
        rigid_ball_1__m_rbRadius - Ball_Srfc_Thickness
        material { My_Object_Material }
}
                                                            
object { Moving_Ball }
                                                            
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
#declare Floor_Thickness = 0;
#declare Floor_Image_Scale = 1000;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y - Floor_Thickness
   pigment {
        image_map {jpeg "Wood-Light-birch.jpg"}
        rotate <0, -80, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish { Object_Finish }
}

object { Floor }               
