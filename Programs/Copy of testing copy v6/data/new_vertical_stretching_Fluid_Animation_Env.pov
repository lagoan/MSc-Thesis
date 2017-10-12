// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "woods.inc"

#declare display_particles = 0;

#declare Object_Image_Scale = 1000;
 
#declare My_Object_Material =
material {                                     
   texture {
        pigment {
                image_map {jpeg "Wood-Light-birch.jpg"}
                rotate <0, 0, 0>       
                scale <Object_Image_Scale, Object_Image_Scale, Object_Image_Scale>
        }
        finish {
              specular 0.005
              ambient 0.35
              diffuse 0.35
        }
   }
}

#declare My_Fluid_Texture =
texture {                                     
   pigment {color <0.25, 0.45, 0.25>  filter 0.995 }
   finish {
      specular 0.85
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

#declare My_Fluid_Material =
material {
        texture { My_Fluid_Texture  }
        interior { My_Fluid_Interior  }  
}

#declare My_Focus_Position = <0.0000, 125.0000, 0.0000>;
#declare My_Camera_Location = <0.0000, 50.0000, -600.0000>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source {
        <-10000, 10000, -10000> colour White*0.01
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

#declare Standard_Rod = 
cylinder { <0, 0, -0.2>, <0, 0, 1>, 1 }
        
#declare Moving_Cylinder_Wall_Thickness = 10;                       
#declare Moving_Cylinder_End_Thickness = 10;                       

#declare Top_Rod_Len = solid_rod_2__m_cylLength;
#declare Top_Rod_Radius = solid_rod_2__m_cylRadius + Moving_Cylinder_Wall_Thickness;

#declare Btm_Rod_Len = solid_rod_1__m_cylLength;
#declare Btm_Rod_Radius = solid_rod_1__m_cylRadius + Moving_Cylinder_Wall_Thickness;

object {
        Standard_Rod
        scale <Top_Rod_Radius, Top_Rod_Radius, Top_Rod_Len>
        material { My_Object_Material }
        matrix <solid_rod_2__m_rtzaInvRotM00,
                solid_rod_2__m_rtzaInvRotM01,
                solid_rod_2__m_rtzaInvRotM02,
                solid_rod_2__m_rtzaInvRotM10,
                solid_rod_2__m_rtzaInvRotM11,
                solid_rod_2__m_rtzaInvRotM12,
                solid_rod_2__m_rtzaInvRotM20,
                solid_rod_2__m_rtzaInvRotM21,
                solid_rod_2__m_rtzaInvRotM22,
                solid_rod_2__m_rtzaInvRotM30,
                solid_rod_2__m_rtzaInvRotM31,
                solid_rod_2__m_rtzaInvRotM32
                >
        translate solid_rod_2__m_cylBtmCtrPos
}

object {
        Standard_Rod
        scale <Btm_Rod_Radius, Btm_Rod_Radius, Btm_Rod_Len>
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
#declare Floor_Image_Scale = 10000;
#declare Floor =
plane {
   y, square_slope_1__m_bssCtrPos.y
   pigment {
        image_map {jpeg "Wood-Light-birch.jpg"}
        rotate <0, -80, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish {
         specular 0.05
         ambient 0.25
         diffuse 0.25
   }
}

object { Floor }               
