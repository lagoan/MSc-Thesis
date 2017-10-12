// Persistence Of Vision raytracer version 3.5 sample file.
// Utah Teapot w/ Bezier patches
// adapted by Alexander Enzmann


global_settings { assumed_gamma 1.0 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

#declare My_Focus = <0.0000, 0.0000, 0.0000>;

#declare My_Camera_Location = <2.0, 1.5, -15.0>;
                     
#declare Lava = color Orange filter 0.05;

#declare My_Lava_Texture =
texture {
        pigment {Lava}
    finish {                
//        phong 0.9 phong_size 100
        specular 0
        ambient 0.95
        diffuse 0.95
        conserve_energy
    }
}

#declare My_Lava_Int =
interior {
    ior 1.33
    fade_distance 2
    fade_power 2
//    fade_color <1, 1, 1>
}

#declare My_Lava_Material =
material {
    texture {My_Lava_Texture}
    interior {My_Lava_Int}
}
                                    
#declare Camera_Focus_Up = 1.0;
#declare Camera_Zoom_In = 0;
camera {
   location  <My_Camera_Location.x,
              My_Camera_Location.y+Camera_Focus_Up,
              My_Camera_Location.z+Camera_Zoom_In>
                 //<0.0, 0.0, -10.0>
//   direction <0.0, 0.0,  1.0>
//   up        <0.0, 1.0,  0.0>
   up        <0.0, 1.0,  0.0>
   right     <2/5 , 0.0,  0.0>
   look_at   <My_Focus.x, My_Focus.y+Camera_Focus_Up, My_Focus.z>
}

#declare LampCapHeight = 1.5;
#declare LampWaistHeight = 0.4;
#declare LampBaseTopHeight = 1.2;
#declare LampBaseNeckHeight = 1.0;
#declare LampBaseBtmHeight = 1.5;

#declare LampTopPos = <0, 5.2, 0>;
#declare LampCenterPos = <0, 0, 0>;
#declare LampBtmPos = <0, -1.5, 0>;
#declare LampCapPos = <0, LampTopPos.y+LampCapHeight, 0>;
#declare LampBaseTopPos = <0, LampBtmPos.y+LampBaseTopHeight, 0>;
#declare LampBaseNeckPos = <0, LampBtmPos.y-LampBaseNeckHeight, 0>;
#declare LampBaseBtmPos = <0, LampBaseNeckPos.y-LampBaseBtmHeight, 0>;
                          
#declare LampWaistTopPosY = LampCenterPos.y + 0.5*LampWaistHeight;
#declare LampWaistBtmPosY = LampCenterPos.y - 0.5*LampWaistHeight;

#declare LampTopRadius = 1.2;
#declare LampWaistRadius = 1.8;
#declare LampBtmRadius = 1.3;
#declare LampBaseBtmRadius = 1.8;
                              
#declare LampBaseThickness = 0.02;                             
#declare LampUpperHeight = LampTopPos.y-LampWaistTopPosY;
#declare LampLowerHeight = LampWaistBtmPosY-LampBtmPos.y;
#declare LampCapRadius = LampWaistRadius - (LampWaistRadius-LampTopRadius)*(LampUpperHeight+LampCapHeight)/LampUpperHeight;
#declare LampBaseTopRadius = LampBaseThickness+LampWaistRadius - (LampWaistRadius-LampBtmRadius)*(LampLowerHeight-LampBaseTopHeight)/LampLowerHeight;
#declare LampBaseNeckRadius = LampBaseThickness+LampWaistRadius - (LampWaistRadius-LampBtmRadius)*(LampLowerHeight+LampBaseNeckHeight)/LampLowerHeight;

#declare LampGlassTexture =
texture {
        pigment {rgbf < 1.0, 1.0, 1.0, 0.95> }
        finish {
                ambient 0.0
                diffuse 0.0
                reflection 0.1
                phong 0.3
                phong_size 90
        }
/*   finish {
      specular 1
      roughness 0.001
      ambient 0
      diffuse 0
      reflection {
         0,1
         fresnel
      }         
   }
*/
}             
#declare LampGlassInterior =
   interior{
      ior 1.1
      caustics 2
      fade_distance 1.5
      fade_power 1
   }
#declare LampGlassThickness = 0.05;
#declare LampGlassOut = 
  sor {        
        6,
        <LampBaseNeckRadius, LampBaseNeckPos.y>
        <LampBtmRadius, LampBtmPos.y>
        <LampWaistRadius, LampWaistBtmPosY>
        <LampWaistRadius, LampWaistTopPosY>
        <LampTopRadius, LampTopPos.y>
        <LampCapRadius, LampTopPos.y+LampCapHeight>
//        <0, LampTopPos.y+LampCapHeight+LampCapHeight*LampCapRadius/(LampTopRadius-LampCapRadius)-0.2>
        open          
        sturm
        pigment {rgbf 0.95}
        texture {LampGlassTexture}
        interior {LampGlassInterior}
   }     

#declare LampLiquidPigment = color red 1.0 green 0.75 blue 0.75 filter 0.75;
#declare LampLiquidFinish =
        finish {
                ambient 0.0
                diffuse 0.0   
                specular 1
                roughness 0.0001
                reflection 0.1         
               }
#declare LampLiquidInterior =
   interior{
      ior 1.0
      caustics 2
      fade_distance 40
      fade_power 1
   }
#declare LampLiquid =
sor {        
        6,
        <LampBaseNeckRadius-LampGlassThickness, LampBaseNeckPos.y>
        <LampBtmRadius-LampGlassThickness, LampBtmPos.y>
        <LampWaistRadius-LampGlassThickness, LampWaistBtmPosY>
        <LampWaistRadius-LampGlassThickness, LampWaistTopPosY>
        <LampTopRadius-LampGlassThickness, LampTopPos.y>
        <LampCapRadius-LampGlassThickness, LampTopPos.y+LampCapHeight>
//        <0, LampTopPos.y+LampCapHeight+LampCapHeight*LampCapRadius/(LampTopRadius-LampCapRadius)-0.2>
        open          
        sturm
        texture {
                pigment {LampLiquidPigment}
                finish {LampLiquidFinish}
        }
        interior {LampLiquidInterior}
}     
#declare Lamp =
union {
        object {LampGlassOut}
        object {LampLiquid}
}
object {Lamp}

#declare LampMetalTexture =
texture {
        pigment {rgbf < 1.0, 1.0, 1.0, 0.0> }
        finish {
                ambient 0.05
                diffuse 0.05
                specular 1
                reflection 0.9
                phong 0.3
                phong_size 90
        }      
}

#declare LampCap = 
  cone {
      LampCapPos, LampCapRadius, LampTopPos, LampTopRadius
      texture {LampMetalTexture}
   }    
   
#declare LampUpperBase = 
  cone {
      LampBaseTopPos, LampBaseTopRadius, LampBaseNeckPos, LampBaseNeckRadius
      open
      texture {LampMetalTexture}
   }    
   
#declare LampLowerBase = 
  cone {
      LampBaseNeckPos, LampBaseNeckRadius, LampBaseBtmPos, LampBaseBtmRadius
      texture {LampMetalTexture}
   }    

object {LampCap}
object {LampUpperBase} 
object {LampLowerBase} 


#declare Sphere_Lava = 
sphere {
    My_Focus, 1
    material {My_Lava_Material}
}

sky_sphere {
    pigment {
        gradient y
        color_map {
            [0 color CornflowerBlue]
            [1 color MidnightBlue]
        }
        scale 10
        translate -3.65
    }
}
    
/* Floor */
plane { y, LampBaseNeckPos.y-LampBaseBtmHeight
   pigment { color NeonPink*1.2}
}
                                             
#declare top_light_magnitude = 0.05;                                             
#declare base_light_magnitude = 10;                                             
light_source { <100.0, 15.0, -10.0> colour White}
//light_source { <0.0, 200.0, -200.0> colour Gray05 shadowless}
light_source { <0.0, 5.75, 0.0> colour top_light_magnitude}
light_source { <0.0, -2, 0.0> colour base_light_magnitude}

/*
#declare LampBaseFront =
   superellipsoid {
      < 0.05, 0.05>
      scale <1.21, LampBaseHeight, 0.02>
      translate <0, Lampbasetranslatey, -1.2>
      texture {Bright_Bronze}
   }
object {LampBaseFront}
object {LampBaseBack}
object {LampBaseLeft} 
object {LampBaseRight}
*/

/* Background */    
/*
plane {
   z, 10

   texture {
      pigment {
        color rgb <0.95 , 0.25, 0.25>
      }
   }
}
*/
/*                          
#declare LampTopRadius = 0.8;
#declare LampCapRadius = 1.0;
#declare LampWaistRadius = 1.7;
#declare LampBaseTopRadius = 1.5;
#declare LampBaseNeckRadius = 0.8;
#declare LampBaseBtmRadius = 1.7;
#declare LampWaistTopPos = <0, 0.4, 0>;
#declare LampWaistBtmPos = <0, -0.4, 0>;
#declare LampBaseTopPos = <0, -1.0, 0>;
#declare LampBaseNeckPos = <0, -3.1, 0>;
#declare LampBaseBtmPos = <0, -4.5, 0>;
#declare UpperLamp = 
  cone {
      LampCapPos, LampCapRadius , LampWaistTopPos, LampWaistRadius
      open
      pigment {rgbf 0.75}
      texture {LampGlass}
      interior {
         ior 1.5
         fade_distance 5
         fade_power 1
      }
   }    

#declare MiddleLamp = 
  sor {        
      6,
      <LampBaseNeckRadius, LampBaseNeckPos.y>
      <LampBaseTopRadius, LampBaseTopPos.y>
      <LampWaistRadius, LampWaistBtmPos.y>
      <LampWaistRadius, LampWaistTopPos.y>
      <LampCapRadius, LampCapPos.y>
      <LampTopRadius, LampTopPos.y>
      open          
      sturm
      pigment {rgbf 0.75}
      texture {LampGlass}
      interior {
         ior 1.5
         fade_distance 5
         fade_power 1
      }
   }     
//object {MiddleLamp}
   
#declare LowerLamp = 
  cone {
      LampWaistBtmPos, LampWaistRadius, LampBaseTopPos, LampBaseTopRadius
      open
      pigment {rgbf 0.75}
      texture {LampGlass}
      interior {
         ior 1.5
         fade_distance 5
         fade_power 1
      }
   }    
*/   

