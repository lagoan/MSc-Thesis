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
                rotate <0, -80, 0>       
                scale <Object_Image_Scale, Object_Image_Scale, Object_Image_Scale>
        }
        finish {
              specular 0.01
              roughness 0.001
              ambient 0.25
              diffuse 0.25
              reflection { 0.001 }
              }
        }
   interior {
        ior 1.5
        fade_distance 50
        fade_power 1
   }
}

#declare My_Fluid_Texture =
texture {                                     
   pigment {color <0.35, 0.75, 0.35>  filter 0.25 }
   finish {
      specular 0.75
      roughness 0.005
      ambient 0.5
      diffuse 0.75
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

#declare My_Focus_Position = <0.0000, -20.0000, 0.0000>;
#declare My_Camera_Location = <200.0000, -20.0000, 700.0000>;

camera {
   location  My_Camera_Location
/*   direction <0.0, 0.0,  1.0>*/
   up        <0.0, 1.0,  0.0>
   right     <-4/4, 0.0,  0.0>
   look_at   My_Focus_Position
}

light_source { <10000, -10, 10000> colour White*0.01 }

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

#declare Outer_Radius = nozzle_outlet__m_trsOuterRadius + 8;
#declare Inner_Radius = nozzle_outlet__m_trsInnerRadius + 8;

#declare Torus_Major_Radius = 0.5*(Outer_Radius + Inner_Radius);
#declare Torus_Minor_Radius = 0.5*(Outer_Radius - Inner_Radius);
#declare Standard_Torus =
torus { Torus_Major_Radius, Torus_Minor_Radius }

#declare Outlet = 
object {
        Standard_Torus
        rotate <90, 0, 0>
        translate <0, 0, -1>
        matrix <nozzle_outlet__m_rtzaInvRotM00,
                nozzle_outlet__m_rtzaInvRotM01,
                nozzle_outlet__m_rtzaInvRotM02,
                nozzle_outlet__m_rtzaInvRotM10,
                nozzle_outlet__m_rtzaInvRotM11,
                nozzle_outlet__m_rtzaInvRotM12,
                nozzle_outlet__m_rtzaInvRotM20,
                nozzle_outlet__m_rtzaInvRotM21,
                nozzle_outlet__m_rtzaInvRotM22,
                nozzle_outlet__m_rtzaInvRotM30,
                nozzle_outlet__m_rtzaInvRotM31,
                nozzle_outlet__m_rtzaInvRotM32
                >
        translate nozzle_outlet__m_trsCtrPos
        }

#declare Standard_Cylinder = 
cylinder { <0, 0, 0>, <0, 0, 1>, 1 }
        
#declare Length = nozzle__m_cylLength;

#declare Solid_Cylinder = 
difference {
        cylinder {<0, 0, -0.25>, <0, 0, 1>, Outer_Radius scale <1, 1, Length> }
        cylinder {<0, 0, 0>, <0, 0, 1>, Inner_Radius scale <1, 1, Length*2> }
        matrix <nozzle__m_rtzaInvRotM00,
                nozzle__m_rtzaInvRotM01,
                nozzle__m_rtzaInvRotM02,
                nozzle__m_rtzaInvRotM10,
                nozzle__m_rtzaInvRotM11,
                nozzle__m_rtzaInvRotM12,
                nozzle__m_rtzaInvRotM20,
                nozzle__m_rtzaInvRotM21,
                nozzle__m_rtzaInvRotM22,
                nozzle__m_rtzaInvRotM30,
                nozzle__m_rtzaInvRotM31,
                nozzle__m_rtzaInvRotM32
                >
        translate nozzle__m_cylBtmCtrPos
}

object {
        union {
                object { Solid_Cylinder }
                object { Outlet }
              }
        material { My_Object_Material }
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
   y, square_slope_1__m_bssCtrPos.y - 4
   pigment {
        image_map {jpeg "Wood-Light-birch.jpg"}
        rotate <0, -80, 0>       
        scale <Floor_Image_Scale, Floor_Image_Scale, Floor_Image_Scale>
   }
   finish {
         specular 0.05
         ambient 0.25
         diffuse 0.25
              reflection {
                 0.001
              }
   }
}

object { Floor }               
