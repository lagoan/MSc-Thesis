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
   pigment {rgbf < 0.5, 0.75, 0.75, 0.025> }
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
      specular 0.95
      roughness 0.02
      ambient 0.95
      diffuse 0.95
      reflection {
         0.99
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

#declare My_Focus_Position = <0.0000, -50.0000, 0.0000>;
#declare My_Camera_Location = <0.0000, -40.0000, -650.0000>;
              
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
        <0.0, 1000.0, -1000.0> colour White
        spotlight
        point_at My_Focus_Position
        radius 40
        falloff 50
        fade_distance 1000
        fade_power 1            
        area_light <150, 0, 0>, <0, 0, 150>, 8, 8
        adaptive 1
}
        
#declare Moving_Cylinder_Thickness = 15;                       
#declare Moving_Cylinder_Cover_Len = 5;                       

#declare Left_Moving_Cylinder = 
cylinder {
        <left_cyl__m_cylTopCtrPos.x + Moving_Cylinder_Cover_Len,
         left_cyl__m_cylTopCtrPos.y, left_cyl__m_cylTopCtrPos.z>   
        <left_cyl__m_cylBtmCtrPos.x - left_cyl__m_cylCapHeight - Moving_Cylinder_Cover_Len,
         left_cyl__m_cylBtmCtrPos.y, left_cyl__m_cylBtmCtrPos.z>   
        left_cyl__m_cylRadius + Moving_Cylinder_Thickness
}

object {
        Left_Moving_Cylinder
        material { My_Glass_Material }
}

#declare Right_Moving_Cylinder = 
cylinder {
        <right_cyl__m_cylTopCtrPos.x - Moving_Cylinder_Cover_Len,
         right_cyl__m_cylTopCtrPos.y, right_cyl__m_cylTopCtrPos.z>   
        <right_cyl__m_cylBtmCtrPos.x + right_cyl__m_cylCapHeight + Moving_Cylinder_Cover_Len,
         right_cyl__m_cylBtmCtrPos.y, right_cyl__m_cylBtmCtrPos.z>   
        right_cyl__m_cylRadius + Moving_Cylinder_Thickness
}

object {
        Right_Moving_Cylinder
        material { My_Glass_Material }
}

#declare box_m_bsbMinPos = <-20, -120, -20>;
#declare box_m_bsbMaxPos = <20, -80, 20>;
#declare nozzle_m_cylLength = 50.000000;
#declare nozzle_m_cylRadius = 40.000000;
#declare nozzle_m_bcOpenOutletWidth = 20.000000;
#declare nozzle_m_cylTopCtrPos = <-27.000000, 25.000000, 5.000000>;
#declare nozzle_m_cylBtmCtrPos = <-71.721360, 2.639320, 5.000000>;
#declare nozzle_m_cylDirection = <0.894427, 0.447214, 0.000000>;
  
#declare Inner_Cylinder = 
cylinder {
        nozzle_m_cylTopCtrPos,
        nozzle_m_cylBtmCtrPos,   
        nozzle_m_cylRadius
}

#declare Outer_Cylinder = 
cylinder {
        nozzle_m_cylTopCtrPos,
        nozzle_m_cylBtmCtrPos,   
        nozzle_m_cylRadius+10
}

#declare Mug = 
difference
{
  object { Outer_Cylinder }
  object { Inner_Cylinder }
  material { My_Glass_Material }
}

// object { Mug }

#declare Ball = 
sphere {
        nozzle_m_cylTopCtrPos, nozzle_m_cylRadius
        material { My_Fluid_Material }
}  

//object { Ball }

#declare Box = 
box {
        box_m_bsbMinPos, box_m_bsbMaxPos
        material { My_Fluid_Material }
}  

//object { Box }

/* Floor */
#declare Floor =
plane {
   y, -200
   pigment {
        color rgb <0.5, 0.9, 0.5>
   }
}

object { Floor }               
