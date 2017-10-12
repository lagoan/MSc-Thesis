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
                                   
#declare Translate_pull_up_fluid = <0, 0, 0>;

#declare Fluid_Color = <0.25, 0.75, 0.25>;
#declare Fluid_Finish =
finish {
        specular 0.5
        roughness 0.005
        ambient 0.5
        diffuse 0.5
        reflection { 0.001 }
}

#declare My_Fluid_Material_1 = material {}

#declare My_Focus_Position = <0, -75, 50>;
#declare My_Camera_Location = <-900, -75, 400>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source { <-10000, 0, -5000> colour White*0.75 }

#declare Light_Color = White*0.9;
light_source {
        <-300, 400, 400> colour White
        spotlight
        point_at My_Focus_Position
        radius 120
        falloff 140
        fade_distance 10000
        fade_power 1            
        area_light <100, 0, 0>, <0, 0, 100>, 8, 8
        adaptive 1
}
                                   
#declare My_Object_Material1 =
material {                                     
        texture {                                     
                pigment { color <2, 2, 0> }
                finish {
                        specular 0.4
                        roughness 0.02
                        diffuse 0.3
                        ambient 0.3
                }
        }
}

#declare Plate_Surface_Thickness = -2;
#declare Standard_Box = 
box { <-0.5, -0.5, 0>, <0.5, 0.5, 1> }
object {   
        Standard_Box
        scale <solid_box_1__m_boxWidthOnX + Plate_Surface_Thickness*2,
                solid_box_1__m_boxWidthOnY + Plate_Surface_Thickness*2,
                solid_box_1__m_boxHeightOnZ + Plate_Surface_Thickness*2>
        matrix <solid_box_1__m_rtzaInvRotM00,
                solid_box_1__m_rtzaInvRotM01,
                solid_box_1__m_rtzaInvRotM02,
                solid_box_1__m_rtzaInvRotM10,
                solid_box_1__m_rtzaInvRotM11,
                solid_box_1__m_rtzaInvRotM12,
                solid_box_1__m_rtzaInvRotM20,
                solid_box_1__m_rtzaInvRotM21,
                solid_box_1__m_rtzaInvRotM22,
                solid_box_1__m_rtzaInvRotM30,
                solid_box_1__m_rtzaInvRotM31,
                solid_box_1__m_rtzaInvRotM32
                >
        translate solid_box_1__m_boxBtmCtrPos
        material { My_Object_Material1 }
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
              specular 0.4
              roughness 0.01
              ambient 0.3
              diffuse 0.3
        //      reflection { 0.001 }
        }
}

object { Floor }               
