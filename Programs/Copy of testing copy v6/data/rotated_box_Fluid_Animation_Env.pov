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
 
#declare My_Object_Texture =
texture {                                     
   T_Wood13           
//   pigment {
//        image_map {jpeg "Wood-Light-birch.jpg"}
//        scale <Object_Image_Scale, Object_Image_Scale, Object_Image_Scale>
//   }
}

#declare My_Fluid_Texture =
texture {                                     
   pigment {color <0.95, 0.95, 0.75>  filter 0.995 }
   finish {
      specular 0.95
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
    fade_distance 20
    fade_power 1
//    fade_color <1, 1, 1>
}

#declare My_Fluid_Material =
material {
        texture { My_Fluid_Texture  }
        interior { My_Fluid_Interior  }  
}

#declare My_Focus_Position = <0.0000, 0.0000, 0.0000>;
#declare My_Camera_Location = <0.0000, 0.0000, 300.0000>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source {
        <10000, 10000, 10000> colour White*0.01
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

#declare Standard_Box = 
box { <-0.5, -0.5, 0>, <0.5, 0.5, 1> }

#declare Moving_Box_Wall_Thickness = 15;                       
object {   
        Standard_Box
        scale <solid_box_1__m_boxWidthOnX + Moving_Box_Wall_Thickness,
                solid_box_1__m_boxWidthOnY + Moving_Box_Wall_Thickness,
                solid_box_1__m_boxHeightOnZ + Moving_Box_Wall_Thickness>
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
        texture { My_Object_Texture }
}

#declare Moving_Box_2 =
object {   
        Standard_Box
        scale <solid_box_2__m_boxWidthOnX + Moving_Box_Wall_Thickness,
                solid_box_2__m_boxWidthOnY + Moving_Box_Wall_Thickness,
                solid_box_2__m_boxHeightOnZ + Moving_Box_Wall_Thickness>
        matrix <solid_box_2__m_rtzaInvRotM00,
                solid_box_2__m_rtzaInvRotM01,
                solid_box_2__m_rtzaInvRotM02,
                solid_box_2__m_rtzaInvRotM10,
                solid_box_2__m_rtzaInvRotM11,
                solid_box_2__m_rtzaInvRotM12,
                solid_box_2__m_rtzaInvRotM20,
                solid_box_2__m_rtzaInvRotM21,
                solid_box_2__m_rtzaInvRotM22,
                solid_box_2__m_rtzaInvRotM30,
                solid_box_2__m_rtzaInvRotM31,
                solid_box_2__m_rtzaInvRotM32
                >
        translate solid_box_2__m_boxBtmCtrPos
        texture { My_Object_Texture }
}

object { Moving_Box_2 }

#declare Moving_Ball =
sphere {
        rigid_ball_1__m_ovalCtrPos,
        rigid_ball_1__m_rbRadius
        texture { My_Object_Texture }
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
#declare Floor_Image_Scale = 10000;
#declare Floor =
plane {
   y, floor__m_bssCtrPos.y
   pigment {
        image_map {jpeg "Wood-Light-birch.jpg"}
        rotate <0, -80, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish { Object_Finish }
}

object { Floor }               
