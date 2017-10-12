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
        
#declare Moving_Cylinder_Wall_Thickness = 10;                       
#declare Moving_Cylinder_End_Thickness = 10;                       

#declare Moving_Cylinder_1 = 
cylinder {
        solid_rod_1__m_cylTopCtrPos,
        <solid_rod_1__m_cylBtmCtrPos.x - solid_rod_1__m_cylCapHeight,
         solid_rod_1__m_cylBtmCtrPos.y - Moving_Cylinder_End_Thickness,
         solid_rod_1__m_cylBtmCtrPos.z>   
        solid_rod_1__m_cylRadius + Moving_Cylinder_Wall_Thickness
}

object {
        Moving_Cylinder_1
        texture { My_Object_Texture }
}

#declare Moving_Cylinder_2 = 
cylinder {
        solid_rod_2__m_cylTopCtrPos,
        <solid_rod_2__m_cylBtmCtrPos.x + solid_rod_2__m_cylCapHeight,
         solid_rod_2__m_cylBtmCtrPos.y + Moving_Cylinder_End_Thickness,
         solid_rod_2__m_cylBtmCtrPos.z>   
        solid_rod_2__m_cylRadius + Moving_Cylinder_Wall_Thickness
}

object {
        Moving_Cylinder_2
        texture { My_Object_Texture }
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
   y, floor__m_bssCtrPos.y
   pigment {
        image_map {jpeg "Wood-Light-birch.jpg"}
        rotate <0, -80, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish { Object_Finish }
}

object { Floor }               
