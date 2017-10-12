/* average times: interact=0.662500 s/f, motion=2.313664 s/f, downsample=0.004321 s/f, upsample=0.262470 s/f, and total=8.130597 s/f. */

#declare My_Focus_Position = <0.000000, 0.000000, 0.000000>;
#declare My_Camera_Position = <0.000000, 0.000000, 300.000000>;


#declare bowl__m_bbBigRadius = 200.000000;
#declare bowl__m_bbRimRadius = 200.000000;
#declare bowl__m_bbHeight = 200.000000;
#declare bowl__m_bbBtmPos = <0.000000, -280.000000, 0.000000>;
#declare bowl__m_ovalCtrPos = <0.000000, -80.000000, 0.000000>;
#declare bowl__m_ovalRadiusOnX = 200.000000;
#declare bowl__m_ovalRadiusOnY = 200.000000;
#declare bowl__m_ovalRadiusOnZ = 200.000000;
#declare bowl__m_rtzaRotationAngleAboutZAxis = 0.000000;
#declare bowl__m_rtzaOrgDirection = <0.000000, 1.000000, 0.000000>;
#declare bowl__m_rtzaRotateM00 = 1.000000;
#declare bowl__m_rtzaRotateM01 = 0.000000;
#declare bowl__m_rtzaRotateM02 = 0.000000;
#declare bowl__m_rtzaRotateM10 = 0.000000;
#declare bowl__m_rtzaRotateM11 = 0.000000;
#declare bowl__m_rtzaRotateM12 = 1.000000;
#declare bowl__m_rtzaRotateM20 = 0.000000;
#declare bowl__m_rtzaRotateM21 = -1.000000;
#declare bowl__m_rtzaRotateM22 = 0.000000;
#declare bowl__m_rtzaRotateM30 = 0.000000;
#declare bowl__m_rtzaRotateM31 = 0.000000;
#declare bowl__m_rtzaRotateM32 = 0.000000;
#declare bowl__m_rtzaInvRotM00 = 1.000000;
#declare bowl__m_rtzaInvRotM01 = 0.000000;
#declare bowl__m_rtzaInvRotM02 = 0.000000;
#declare bowl__m_rtzaInvRotM10 = 0.000000;
#declare bowl__m_rtzaInvRotM11 = 0.000000;
#declare bowl__m_rtzaInvRotM12 = -1.000000;
#declare bowl__m_rtzaInvRotM20 = 0.000000;
#declare bowl__m_rtzaInvRotM21 = 1.000000;
#declare bowl__m_rtzaInvRotM22 = 0.000000;
#declare bowl__m_rtzaInvRotM30 = 0.000000;
#declare bowl__m_rtzaInvRotM31 = 0.000000;
#declare bowl__m_rtzaInvRotM32 = 0.000000;

#include "Fluid_Animation_Env.pov"

#if (display_particles_1)
	#include "prtl_eggwhite_0134.txt"
#else
	#include "surface_mesh_eggwhite_0134.inc"
	object { eggwhite translate Translate_eggwhite material { My_Fluid_Material_1 } }
#end

#if (display_particles_2)
	#include "prtl_yolk0_0134.txt"
#else
	#include "surface_mesh_yolk0_0134.inc"
	object { yolk0 translate Translate_yolk0 material { My_Fluid_Material_2 } }
#end
