// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"



#declare solid_rod_1__m_cylLength = 210.000000;
#declare solid_rod_1__m_cylRadius = 25.000000;
#declare solid_rod_1__m_cylCapHeight = 0.000000;
#declare solid_rod_1__m_cylTopCtrPos = <0.000000, 155.000000, 0.000000>;
#declare solid_rod_1__m_cylBtmCtrPos = <0.000000, -55.000000, 0.000000>;
#declare solid_rod_1__m_rtzaRotationAngleAboutZAxis = 0.390625;
#declare solid_rod_1__m_rtzaOrgDirection = <0.000000, 1.000000, 0.000000>;
#declare solid_rod_1__m_rtzaRotateM00 = 0.999977;
#declare solid_rod_1__m_rtzaRotateM01 = -0.006818;
#declare solid_rod_1__m_rtzaRotateM02 = 0.000000;
#declare solid_rod_1__m_rtzaRotateM10 = 0.000000;
#declare solid_rod_1__m_rtzaRotateM11 = 0.000000;
#declare solid_rod_1__m_rtzaRotateM12 = 1.000000;
#declare solid_rod_1__m_rtzaRotateM20 = -0.006818;
#declare solid_rod_1__m_rtzaRotateM21 = -0.999977;
#declare solid_rod_1__m_rtzaRotateM22 = 0.000000;
#declare solid_rod_1__m_rtzaRotateM30 = 0.000000;
#declare solid_rod_1__m_rtzaRotateM31 = 0.000000;
#declare solid_rod_1__m_rtzaRotateM32 = 0.000000;
#declare solid_rod_1__m_rtzaInvRotM00 = 0.999977;
#declare solid_rod_1__m_rtzaInvRotM01 = 0.000000;
#declare solid_rod_1__m_rtzaInvRotM02 = -0.006818;
#declare solid_rod_1__m_rtzaInvRotM10 = -0.006818;
#declare solid_rod_1__m_rtzaInvRotM11 = 0.000000;
#declare solid_rod_1__m_rtzaInvRotM12 = -0.999977;
#declare solid_rod_1__m_rtzaInvRotM20 = 0.000000;
#declare solid_rod_1__m_rtzaInvRotM21 = 1.000000;
#declare solid_rod_1__m_rtzaInvRotM22 = 0.000000;
#declare solid_rod_1__m_rtzaInvRotM30 = 0.000000;
#declare solid_rod_1__m_rtzaInvRotM31 = 0.000000;
#declare solid_rod_1__m_rtzaInvRotM32 = 0.000000;

#declare solid_rod_2__m_cylLength = 20.000000;
#declare solid_rod_2__m_cylRadius = 120.000000;
#declare solid_rod_2__m_cylCapHeight = 0.000000;
#declare solid_rod_2__m_cylTopCtrPos = <0.000000, -35.000000, 0.000000>;
#declare solid_rod_2__m_cylBtmCtrPos = <0.000000, -55.000000, 0.000000>;
#declare solid_rod_2__m_rtzaRotationAngleAboutZAxis = -0.390625;
#declare solid_rod_2__m_rtzaOrgDirection = <0.000000, 1.000000, 0.000000>;
#declare solid_rod_2__m_rtzaRotateM00 = 0.999977;
#declare solid_rod_2__m_rtzaRotateM01 = 0.006818;
#declare solid_rod_2__m_rtzaRotateM02 = 0.000000;
#declare solid_rod_2__m_rtzaRotateM10 = -0.000000;
#declare solid_rod_2__m_rtzaRotateM11 = 0.000000;
#declare solid_rod_2__m_rtzaRotateM12 = 1.000000;
#declare solid_rod_2__m_rtzaRotateM20 = 0.006818;
#declare solid_rod_2__m_rtzaRotateM21 = -0.999977;
#declare solid_rod_2__m_rtzaRotateM22 = 0.000000;
#declare solid_rod_2__m_rtzaRotateM30 = 0.000000;
#declare solid_rod_2__m_rtzaRotateM31 = 0.000000;
#declare solid_rod_2__m_rtzaRotateM32 = 0.000000;
#declare solid_rod_2__m_rtzaInvRotM00 = 0.999977;
#declare solid_rod_2__m_rtzaInvRotM01 = -0.000000;
#declare solid_rod_2__m_rtzaInvRotM02 = 0.006818;
#declare solid_rod_2__m_rtzaInvRotM10 = 0.006818;
#declare solid_rod_2__m_rtzaInvRotM11 = 0.000000;
#declare solid_rod_2__m_rtzaInvRotM12 = -0.999977;
#declare solid_rod_2__m_rtzaInvRotM20 = 0.000000;
#declare solid_rod_2__m_rtzaInvRotM21 = 1.000000;
#declare solid_rod_2__m_rtzaInvRotM22 = 0.000000;
#declare solid_rod_2__m_rtzaInvRotM30 = 0.000000;
#declare solid_rod_2__m_rtzaInvRotM31 = 0.000000;
#declare solid_rod_2__m_rtzaInvRotM32 = 0.000000;

#declare square_slope_1__m_bssSlopeThickness = 50.000000;
#declare square_slope_1__m_bssSideLen = 10000.000000;
#declare square_slope_1__m_bssCtrPos = <0.000000, -51.000000, 0.000000>;
#declare square_slope_1__m_rtzaRotationAngleAboutZAxis = 0.000000;
#declare square_slope_1__m_rtzaOrgDirection = <0.000000, 1.000000, 0.000000>;
#declare square_slope_1__m_rtzaRotateM00 = 1.000000;
#declare square_slope_1__m_rtzaRotateM01 = 0.000000;
#declare square_slope_1__m_rtzaRotateM02 = 0.000000;
#declare square_slope_1__m_rtzaRotateM10 = 0.000000;
#declare square_slope_1__m_rtzaRotateM11 = 0.000000;
#declare square_slope_1__m_rtzaRotateM12 = 1.000000;
#declare square_slope_1__m_rtzaRotateM20 = 0.000000;
#declare square_slope_1__m_rtzaRotateM21 = -1.000000;
#declare square_slope_1__m_rtzaRotateM22 = 0.000000;
#declare square_slope_1__m_rtzaRotateM30 = 0.000000;
#declare square_slope_1__m_rtzaRotateM31 = 0.000000;
#declare square_slope_1__m_rtzaRotateM32 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM00 = 1.000000;
#declare square_slope_1__m_rtzaInvRotM01 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM02 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM10 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM11 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM12 = -1.000000;
#declare square_slope_1__m_rtzaInvRotM20 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM21 = 1.000000;
#declare square_slope_1__m_rtzaInvRotM22 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM30 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM31 = 0.000000;
#declare square_slope_1__m_rtzaInvRotM32 = 0.000000;



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

#declare srfc_Thickness = 10;                       

#declare rod = 
cylinder {
        <0.000000, 0.000000, 300.000000>, 
        <0.000000, 0.000000, -300.000000>,
        rod_m_cylRadius - srfc_Thickness
        material { My_Glass_Material }
}

object { rod }

/* Floor */
#declare Floor =
plane {
   y, m_tblCenter_Level - srfc_Thickness
   pigment {
        color rgb <0.5, 0.9, 0.5>
   }
}

object { Floor }               
