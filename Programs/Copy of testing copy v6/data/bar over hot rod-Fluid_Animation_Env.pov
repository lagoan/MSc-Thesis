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
                                   
#declare Translate_fluid_cylinder = <0, 0, 0>;

#declare Fluid_Finish =
finish {
        specular 0.5
        roughness 0.01
        ambient 0.5
        diffuse 0.5
        reflection { 0.001 }
}

#declare My_Fluid_Material_1 =
material {
        texture {}
        interior {}  
}

#declare My_Focus_Position = <0, 0, 0>;
#declare My_Camera_Location = <-300, 0, 600>;

light_source { <-1000, 300, -1000> colour White*0.5 }
 
light_source {
        <200, 200, 400> colour White
        spotlight
        point_at My_Focus_Position
        radius 120
        falloff 140
        fade_distance 100000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
}
                                   
#declare My_Object_Material =
material {                                     
        texture {                                     
                pigment { color <2, 2, 0> }
                finish {
                      specular 0.4
                      roughness 0.02
                      ambient 0.3
                      diffuse 0.3
                }
        }
}

#declare Rod_Surface_Thickness = 0;
#declare RodRadius = solid_rod_1__m_cylRadius + Rod_Surface_Thickness;
#declare RodBtmCtrPos = solid_rod_1__m_cylBtmCtrPos;
#declare RodTopCtrPos = solid_rod_1__m_cylTopCtrPos;
#declare RodLenBtmCut = 50;
#declare RodLenTopCut = 50;
#declare HeatingRod = 
cylinder {
        <RodBtmCtrPos.x, RodBtmCtrPos.y, RodBtmCtrPos.z + RodLenBtmCut>
        <RodTopCtrPos.x, RodTopCtrPos.y, RodTopCtrPos.z - RodLenTopCut>
        RodRadius
        material { My_Object_Material }
}
object { HeatingRod }          

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
#declare Floor_Image_Scale = 200;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y + Floor_Thickness
   pigment {
        image_map {jpeg "jade_green.jpg"}
        rotate <90, 0, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish {
      specular 0.001
      ambient 0.3
      diffuse 0.3
   }
}

object { Floor }               

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}
