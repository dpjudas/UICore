#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.20789
//
//
///
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// image_sampler                     sampler      NA          NA    0        1
// mask_sampler                      sampler      NA          NA    1        1
// instance_data                     texture  float4          2d    0        1
// image_texture                     texture  float4          2d    1        1
// mask_texture                      texture  float4          2d    2        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0      POS   float       
// brush_data               1   xyzw        1     NONE   float   x zw
// brush_data               2   xyzw        2     NONE   float   xyzw
// vary_data                0   xyzw        3     NONE   float   xyzw
// mask_position            0   xy          4     NONE   float   xy  
// instance_offset          0   xy          5     NONE    uint   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
ps_4_0
dcl_sampler s0, mode_default
dcl_sampler s1, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_resource_texture2d (float,float,float,float) t2
dcl_input_ps linear v1.xzw
dcl_input_ps linear v2.xyzw
dcl_input_ps linear v3.xyzw
dcl_input_ps linear v4.xy
dcl_input_ps constant v5.xy
dcl_output o0.xyzw
dcl_temps 7
ftou r0.x, v1.x
switch r0.x
  case l(1)
  ftou r0.xy, v2.yzyy
  dp2 r0.z, v3.xyxx, v1.zwzz
  iadd r1.y, r0.x, v5.x
  mov r1.z, v5.y
  mov r1.w, l(0)
  ld r2.xyzw, r1.yzww, t0.xyzw
  iadd r1.x, r1.y, l(1)
  ld r3.xyzw, r1.xzww, t0.xyzw
  mov r4.xyzw, r2.xyzw
  mov r0.w, r3.x
  mov r3.y, r0.x
  loop 
    uge r3.z, r3.y, r0.y
    breakc_nz r3.z
    iadd r1.y, r3.y, v5.x
    ld r5.xyzw, r1.yzww, t0.xyzw
    iadd r1.x, r1.y, l(1)
    ld r6.xyzw, r1.xzww, t0.xyzw
    mad r1.x, r0.z, v2.x, -r0.w
    add r1.y, -r0.w, r6.x
    div_sat r1.x, r1.x, r1.y
    add r5.xyzw, -r4.xyzw, r5.xyzw
    mad r4.xyzw, r1.xxxx, r5.xyzw, r4.xyzw
    iadd r3.y, r3.y, l(2)
    mov r0.w, r6.x
  endloop 
  sample r0.xyzw, v4.xyxx, t2.xyzw, s1
  mul o0.xyzw, r0.xxxx, r4.xyzw
  ret 
  case l(2)
  ftou r0.xy, v2.yzyy
  dp2 r0.z, v3.xyxx, v3.xyxx
  sqrt r0.z, r0.z
  iadd r1.y, r0.x, v5.x
  mov r1.z, v5.y
  mov r1.w, l(0)
  ld r2.xyzw, r1.yzww, t0.xyzw
  iadd r1.x, r1.y, l(1)
  ld r3.xyzw, r1.xzww, t0.xyzw
  mov r4.xyzw, r2.xyzw
  mov r0.w, r3.x
  mov r3.y, r0.x
  loop 
    uge r3.z, r3.y, r0.y
    breakc_nz r3.z
    iadd r1.y, r3.y, v5.x
    ld r5.xyzw, r1.yzww, t0.xyzw
    iadd r1.x, r1.y, l(1)
    ld r6.xyzw, r1.xzww, t0.xyzw
    mad r1.x, r0.z, v2.x, -r0.w
    add r1.y, -r0.w, r6.x
    div_sat r1.x, r1.x, r1.y
    add r5.xyzw, -r4.xyzw, r5.xyzw
    mad r4.xyzw, r1.xxxx, r5.xyzw, r4.xyzw
    iadd r3.y, r3.y, l(2)
    mov r0.w, r6.x
  endloop 
  sample r0.xyzw, v4.xyxx, t2.xyzw, s1
  mul o0.xyzw, r0.xxxx, r4.xyzw
  ret 
  case l(3)
  sample r0.xyzw, v3.zwzz, t1.xyzw, s0
  sample r1.xyzw, v4.xyxx, t2.xyzw, s1
  mul o0.xyzw, r0.xyzw, r1.xxxx
  ret 
  default 
  sample r0.xyzw, v4.xyxx, t2.xyzw, s1
  mul o0.xyzw, r0.xxxx, v2.xyzw
  ret 
endswitch 
ret 
// Approximately 74 instruction slots used
#endif

const BYTE StandardPrograms::path_fragment[] =
{
     68,  88,  66,  67, 187,  35, 
    193,  90, 253,  57, 204, 225, 
    247, 181, 204, 172, 255, 196, 
    196, 214,   1,   0,   0,   0, 
     56,  10,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    112,   1,   0,   0,  80,   2, 
      0,   0, 132,   2,   0,   0, 
    188,   9,   0,   0,  82,  68, 
     69,  70,  52,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    255, 255,   1, 137,   0,   0, 
      0,   1,   0,   0, 188,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    202,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 215,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     12,   0,   0,   0, 229,   0, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  12,   0,   0,   0, 
    243,   0,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   2,   0,   0,   0, 
      1,   0,   0,   0,  12,   0, 
      0,   0, 105, 109,  97, 103, 
    101,  95, 115,  97, 109, 112, 
    108, 101, 114,   0, 109,  97, 
    115, 107,  95, 115,  97, 109, 
    112, 108, 101, 114,   0, 105, 
    110, 115, 116,  97, 110,  99, 
    101,  95, 100,  97, 116,  97, 
      0, 105, 109,  97, 103, 101, 
     95, 116, 101, 120, 116, 117, 
    114, 101,   0, 109,  97, 115, 
    107,  95, 116, 101, 120, 116, 
    117, 114, 101,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  57,  46,  51, 
     48,  46,  57,  50,  48,  48, 
     46,  50,  48,  55,  56,  57, 
      0, 171,  73,  83,  71,  78, 
    216,   0,   0,   0,   6,   0, 
      0,   0,   8,   0,   0,   0, 
    152,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
    164,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,  13,   0,   0, 
    164,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,  15,  15,   0,   0, 
    175,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,  15,   0,   0, 
    185,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   4,   0, 
      0,   0,   3,   3,   0,   0, 
    199,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   5,   0, 
      0,   0,   3,   3,   0,   0, 
     83,  86,  95,  80, 111, 115, 
    105, 116, 105, 111, 110,   0, 
     98, 114, 117, 115, 104,  95, 
    100,  97, 116,  97,   0, 118, 
     97, 114, 121,  95, 100,  97, 
    116,  97,   0, 109,  97, 115, 
    107,  95, 112, 111, 115, 105, 
    116, 105, 111, 110,   0, 105, 
    110, 115, 116,  97, 110,  99, 
    101,  95, 111, 102, 102, 115, 
    101, 116,   0, 171,  79,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  84, 
     97, 114, 103, 101, 116,   0, 
    171, 171,  83,  72,  68,  82, 
     48,   7,   0,   0,  64,   0, 
      0,   0, 204,   1,   0,   0, 
     90,   0,   0,   3,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     90,   0,   0,   3,   0,  96, 
     16,   0,   1,   0,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      1,   0,   0,   0,  85,  85, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   2,   0, 
      0,   0,  85,  85,   0,   0, 
     98,  16,   0,   3, 210,  16, 
     16,   0,   1,   0,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   2,   0,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   3,   0,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   4,   0,   0,   0, 
     98,   8,   0,   3,  50,  16, 
     16,   0,   5,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   7,   0, 
      0,   0,  28,   0,   0,   5, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,  16,  16,   0, 
      1,   0,   0,   0,  76,   0, 
      0,   3,  10,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   3,   1,  64,   0,   0, 
      1,   0,   0,   0,  28,   0, 
      0,   5,  50,   0,  16,   0, 
      0,   0,   0,   0, 150,  21, 
     16,   0,   2,   0,   0,   0, 
     15,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   3,   0, 
      0,   0, 230,  26,  16,   0, 
      1,   0,   0,   0,  30,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  16,  16,   0,   5,   0, 
      0,   0,  54,   0,   0,   5, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  26,  16,  16,   0, 
      5,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     45,   0,   0,   7, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    150,  15,  16,   0,   1,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  30,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  45,   0,   0,   7, 
    242,   0,  16,   0,   3,   0, 
      0,   0, 134,  15,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 242,   0, 
     16,   0,   4,   0,   0,   0, 
     70,  14,  16,   0,   2,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      3,   0,   0,   0,  54,   0, 
      0,   5,  34,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     48,   0,   0,   1,  80,   0, 
      0,   7,  66,   0,  16,   0, 
      3,   0,   0,   0,  26,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   3,   0,   4,   3, 
     42,   0,  16,   0,   3,   0, 
      0,   0,  30,   0,   0,   7, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      3,   0,   0,   0,  10,  16, 
     16,   0,   5,   0,   0,   0, 
     45,   0,   0,   7, 242,   0, 
     16,   0,   5,   0,   0,   0, 
    150,  15,  16,   0,   1,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  30,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   1,   0, 
      0,   0,  45,   0,   0,   7, 
    242,   0,  16,   0,   6,   0, 
      0,   0, 134,  15,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  10,  16,  16,   0, 
      2,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   8,  34,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     14,  32,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   8, 242,   0,  16,   0, 
      5,   0,   0,   0,  70,  14, 
     16, 128,  65,   0,   0,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   5,   0,   0,   0, 
     50,   0,   0,   9, 242,   0, 
     16,   0,   4,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      5,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     30,   0,   0,   7,  34,   0, 
     16,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      2,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   6,   0,   0,   0, 
     22,   0,   0,   1,  69,   0, 
      0,   9, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  16, 
     16,   0,   4,   0,   0,   0, 
     70, 126,  16,   0,   2,   0, 
      0,   0,   0,  96,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7, 242,  32,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   4,   0, 
      0,   0,  62,   0,   0,   1, 
      6,   0,   0,   3,   1,  64, 
      0,   0,   2,   0,   0,   0, 
     28,   0,   0,   5,  50,   0, 
     16,   0,   0,   0,   0,   0, 
    150,  21,  16,   0,   2,   0, 
      0,   0,  15,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      3,   0,   0,   0,  70,  16, 
     16,   0,   3,   0,   0,   0, 
     75,   0,   0,   5,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  10,  16, 
     16,   0,   5,   0,   0,   0, 
     54,   0,   0,   5,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     26,  16,  16,   0,   5,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  45,   0, 
      0,   7, 242,   0,  16,   0, 
      2,   0,   0,   0, 150,  15, 
     16,   0,   1,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     45,   0,   0,   7, 242,   0, 
     16,   0,   3,   0,   0,   0, 
    134,  15,  16,   0,   1,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 242,   0,  16,   0, 
      4,   0,   0,   0,  70,  14, 
     16,   0,   2,   0,   0,   0, 
     54,   0,   0,   5, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   3,   0, 
      0,   0,  54,   0,   0,   5, 
     34,   0,  16,   0,   3,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   1,  80,   0,   0,   7, 
     66,   0,  16,   0,   3,   0, 
      0,   0,  26,   0,  16,   0, 
      3,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      3,   0,   4,   3,  42,   0, 
     16,   0,   3,   0,   0,   0, 
     30,   0,   0,   7,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   3,   0, 
      0,   0,  10,  16,  16,   0, 
      5,   0,   0,   0,  45,   0, 
      0,   7, 242,   0,  16,   0, 
      5,   0,   0,   0, 150,  15, 
     16,   0,   1,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     45,   0,   0,   7, 242,   0, 
     16,   0,   6,   0,   0,   0, 
    134,  15,  16,   0,   1,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  10,  18,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   0,   0,   0,   0, 
     10,  16,  16,   0,   2,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,  14,  32, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   8, 
    242,   0,  16,   0,   5,   0, 
      0,   0,  70,  14,  16, 128, 
     65,   0,   0,   0,   4,   0, 
      0,   0,  70,  14,  16,   0, 
      5,   0,   0,   0,  50,   0, 
      0,   9, 242,   0,  16,   0, 
      4,   0,   0,   0,   6,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   5,   0, 
      0,   0,  70,  14,  16,   0, 
      4,   0,   0,   0,  30,   0, 
      0,   7,  34,   0,  16,   0, 
      3,   0,   0,   0,  26,   0, 
     16,   0,   3,   0,   0,   0, 
      1,  64,   0,   0,   2,   0, 
      0,   0,  54,   0,   0,   5, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      6,   0,   0,   0,  22,   0, 
      0,   1,  69,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      4,   0,   0,   0,  70, 126, 
     16,   0,   2,   0,   0,   0, 
      0,  96,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   4,   0,   0,   0, 
     62,   0,   0,   1,   6,   0, 
      0,   3,   1,  64,   0,   0, 
      3,   0,   0,   0,  69,   0, 
      0,   9, 242,   0,  16,   0, 
      0,   0,   0,   0, 230,  26, 
     16,   0,   3,   0,   0,   0, 
     70, 126,  16,   0,   1,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  69,   0, 
      0,   9, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  16, 
     16,   0,   4,   0,   0,   0, 
     70, 126,  16,   0,   2,   0, 
      0,   0,   0,  96,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7, 242,  32,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   1,   0, 
      0,   0,  62,   0,   0,   1, 
     10,   0,   0,   1,  69,   0, 
      0,   9, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  16, 
     16,   0,   4,   0,   0,   0, 
     70, 126,  16,   0,   2,   0, 
      0,   0,   0,  96,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7, 242,  32,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  30,  16,   0,   2,   0, 
      0,   0,  62,   0,   0,   1, 
     23,   0,   0,   1,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    116,   0,   0,   0,  74,   0, 
      0,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,  13,   0,   0,   0, 
     10,   0,   0,   0,   2,   0, 
      0,   0,   4,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  40,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
