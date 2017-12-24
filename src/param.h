#ifndef GF_H
#define GF_H
#endif

#define m_val 2
// Extension degree 12
#define gf_extd 12
// Field size 2^12
#define gf_card (1 << gf_extd)
// Field Group order 2^12 - 1
#define gf_ord ((gf_card)-1)

// Subfield degree is 6
#define gf_extd_sf gf_extd / m_val
// Subfield size 2^6
#define gf_card_sf (1 << gf_extd_sf)
// Subfield Group order 2^6 -1
#define gf_ord_sf ((gf_card_sf)-1)

#define primitive_element_in_f_q 36

// Define Field value
#define u_val 64
// Define Poly primitive subfield: X^6 + x + 1
#define poly_primitive_subfield 67

/*
 // DAG 96
 #define pol_deg 11 //block number 11
 #define code_length 1216
 #define code_dimension 512
 #define order  (1<<5)  // number of line of each block matrix
 #define n0_val 38 // number of constant block
 #define k_prime 32
 #define k_sec 480
 #define n0_w 176
 #define ss_length 64*/

// DAG 128
#define pol_deg 11 //block number 11
#define code_length 2112
#define code_dimension 704
#define order  (1<<6)  // number of line of each block matrix
#define n0_val 33 // number of constant block
#define k_prime  43
#define k_sec 661
#define n0_w 352
#define ss_length 64

/*
 // DAG 64
 #define pol_deg 8 //block number 11
 #define code_length 80
 #define code_dimension 48
 #define order  (1<<1)  // number of line of each block matrix
 #define n0_val 40 // number of constant block
 */

