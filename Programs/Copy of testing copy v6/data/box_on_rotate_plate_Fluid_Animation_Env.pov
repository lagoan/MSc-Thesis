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

#declare Translate_fluid_box = <0, 0, 0>;

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
        texture {  }
        interior {  }  
}

#declare My_Focus_Position = <0, 0, 0>;
#declare My_Camera_Location = <700, 250, -300>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source {
        <10000, 1000, 10000> colour White*0.5
}

light_source {
        <400, 400, -400> colour White
        spotlight
        point_at My_Focus_Position
        radius 200000
        falloff 500
        fade_distance 10000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
}

#declare Object_Image_Scale = 1000;

#declare My_Object_Material =
material {                                     
   texture {
        pigment {
                image_map {jpeg "Wood-Light-birch.jpg"}
                scale <Object_Image_Scale, Object_Image_Scale, Object_Image_Scale>
        }
        finish {
              specular 0.005
              ambient 0.35
              diffuse 0.35
        }
   }
}

#declare Standard_Rod = 
cylinder { <0, 0, 0>, <0, 0, 1>, 1 }

#declare Top_Rod_Len = solid_rod_1__m_cylLength - 5;
#declare Top_Rod_Radius = solid_rod_1__m_cylRadius - 50;
                
object {   
        Standard_Rod
        scale <Top_Rod_Radius, Top_Rod_Radius, Top_Rod_Len>
        material { My_Object_Material }
        matrix <solid_rod_1__m_rtzaInvRotM00,
                solid_rod_1__m_rtzaInvRotM01,
                solid_rod_1__m_rtzaInvRotM02,
                solid_rod_1__m_rtzaInvRotM10,
                solid_rod_1__m_rtzaInvRotM11,
                solid_rod_1__m_rtzaInvRotM12,
                solid_rod_1__m_rtzaInvRotM20,
                solid_rod_1__m_rtzaInvRotM21,
                solid_rod_1__m_rtzaInvRotM22,
                solid_rod_1__m_rtzaInvRotM30,
                solid_rod_1__m_rtzaInvRotM31,
                solid_rod_1__m_rtzaInvRotM32
                >
        translate solid_rod_1__m_cylBtmCtrPos
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
#declare Floor_Thickness = 0;
#declare Floor_Image_Scale = 300;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y + Floor_Thickness
   pigment {
        image_map {jpeg "jade_green.jpg"}
        rotate <90, 0, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish {
         specular 0.05
         ambient 0.25
         diffuse 0.25
   }
}

object { Floor }               
