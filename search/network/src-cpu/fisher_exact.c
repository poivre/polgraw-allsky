#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <gsl/gsl_linalg.h>
#include <time.h>
#include <dirent.h>

#include "auxi.h"
#include "settings.h"
#include "struct.h"
#include "init.h"

#include "arb.h"
#include "arb_mat.h" 

#ifndef CODEVER
#define CODEVER unknown
#endif

#ifndef DTAPREFIX
#define DTAPREFIX .
#endif


int fisher(
  Search_settings *sett,
  Command_line_opts *opts,
  Aux_arrays *aux);  

void modvir_da(
    double sinal, 
    double cosal, 
    double sindel, 
    double cosdel,	
    int Np, 
    Detector_settings *ifo, 
    Aux_arrays *aux);

void cleanup_fisher(
	Search_settings *sett,
	Command_line_opts *opts,
	Aux_arrays *aux,
  double *F);

void handle_opts_fisher (
  Search_settings *sett,
  Command_line_opts *opts,
  int argc,
  char* argv[]); 


int main (int argc, char* argv[]) {

  Command_line_opts opts;
  Search_settings sett;
  Search_range s_range; 
  Aux_arrays aux_arr;
  double *F; 			  // F-statistic array
  int i; 

#define QUOTE(x) #x
#define STR(macro) QUOTE(macro)
#define CVSTR STR(CODEVER)

  printf("Code version : " CVSTR "\n");

  // Command line options 
  handle_opts_fisher(&sett, &opts, argc, argv);  
	
  // Search settings
  search_settings(&sett); 

  // Detector network settings
  detectors_settings(&sett, &opts); 

  // Array initialization and reading the ephemerids 
  init_arrays(&sett, &opts, &aux_arr, &F);

  // Additional initialisation for derivatives 
  // of modulation functions aa and bb with respect 
  // to alpha and delta angles 

  for(i=0; i<sett.nifo; i++) {   

    ifo[i].sig.daaddel = (double *) calloc(sett.N, sizeof(double));
    ifo[i].sig.dbbddel = (double *) calloc(sett.N, sizeof(double));
    ifo[i].sig.daadalf = (double *) calloc(sett.N, sizeof(double));
    ifo[i].sig.dbbdalf = (double *) calloc(sett.N, sizeof(double));

  } 

  // Amplitude modulation functions for each detector  
  for(i=0; i<sett.nifo; i++)   
    rogcvir(&ifo[i]); 

  // Main job - Fisher matrix calculations   
  for(i=0; i<sett.nifo; i++) 
    fisher(&sett, &opts, &aux_arr);

  // Cleanup & memory free 
  cleanup_fisher(&sett, &opts, &aux_arr, F);

  return 0; 
	
}


int fisher(Search_settings *sett,
           Command_line_opts *opts, 
           Aux_arrays *aux) { 


  int i, j, k, l, m, n, reffr, dim=8, dim2=4, prec=128; 

  // Signal parameters: f, fdot, delta, alpha, a1, a2, a3, a4
  // (see Phys. Rev. D 82, 022005 2010, Eqs. 2.13a-d) 
  double sgnlo[dim]; 

  FILE *data; 

  // Reading signal parameters 
  if ((data=fopen (opts->addsig, "r")) != NULL) {

    // This read is made compatible with the format of the signal 
    // used by add_signal():init.c 
    // Skipping first three values 
 
    char amporsnr[3];
    double h0; 
    int gsize;   
    
    // These three values are not used  
    fscanf (data, "%s %le %d", amporsnr, &h0, &gsize);    
    
    fscanf (data, "%d", &reffr); 
    printf("Reference frame is %03d\n", reffr); 

    for(i=0; i<dim; i++)
      fscanf(data, "%le",i+sgnlo); 
    
    fclose (data);
                 
  } else {
    perror (opts->addsig);
  }
 
  double ma[dim][dim], mFl[dim][dim]; 
  double a[dim2], F[dim2][dim2][dim2], S[dim2][dim2][dim2][dim2];
  double omega0, omega1, domega, sumhsq;  
  double sindelt, cosdelt, sinalt, cosalt; 

  // signal frequency shifted w.r.t the reference segment 
  omega0 = sgnlo[0] - 2.*sgnlo[1]*(sett->N)*(reffr - opts->ident); 

  omega1 = sgnlo[1]; 

  domega = sett->oms; // 2*M_PI*sett->fpo*sett->dt; 

  sindelt = sin(sgnlo[2]); 
  cosdelt = cos(sgnlo[2]); 
  sinalt  = sin(sgnlo[3]); 
  cosalt  = cos(sgnlo[3]); 
  
  // a1 = a[0], a2 = a[1], a3 = a[2], a4 = a[3] 
  // in the signal amplitude model 
  // h = (a1*a + a2*b)*cos(psi) + (a3*a + a4*b)*sin(psi)
  a[0] = sgnlo[4]; 
  a[1] = sgnlo[5]; 
  a[2] = sgnlo[6]; 
  a[3] = sgnlo[7]; 


  // Main loop over the detectors 
  //-----------------------------

  for(n=0; n<sett->nifo; ++n) { 

    /* Amplitude modulation functions aa and bb 
     * for each detector (in signal sub-struct 
     * of _detector, ifo[n].sig.aa, ifo[n].sig.bb) 
     */

    modvir(sinalt, cosalt, sindelt, cosdelt, 
        sett->N, &ifo[n], aux);

    /* Derivatives of amplitude modulation functions 
     * aa and bb with respect to delta and alpha 
     * (in signal sub-struct of _detector, 
     * ifo[n].sig.daaddel, ifo[n].sig.dbbddel
     * ifo[n].sig.daadalf, ifo[n].sig.dbbdalf) 
     */

    modvir_da(sinalt, cosalt, sindelt, cosdelt, 
        sett->N, &ifo[n], aux);

    // Initial zeroing of matrices 
    for(k=0; k<dim; k++)
      for(j=0; j<dim; j++) { 
        ma[k][j] = 0; 
        mFl[k][j] = 0; 
      }

    for(l=0; l<dim2; l++)
      for(k=0; k<dim2; k++) 
        for(j=0; j<dim2; j++) 
          F[l][k][j] = 0; 

    for(m=0; m<dim2; m++)
      for(l=0; l<dim2; l++)
        for(k=0; k<dim2; k++) 
          for(j=0; j<dim2; j++) 
            S[m][l][k][j] = 0; 

    double sumhsq = 0; 


    // Loop over the data points 
    //--------------------------

    for(i=0; i<sett->N; ++i) {

      double psi, xet, yet, zet, aa, bb, amph, 
             dpdA[dim2], daadA[dim2], dbbdA[dim2], h[dim2], 
             dh[dim2][dim2]; 

      // Detector ephemerids at time step i
      xet = ifo[n].sig.DetSSB[i*3]; 
      yet = ifo[n].sig.DetSSB[i*3+1]; 
      zet = ifo[n].sig.DetSSB[i*3+2]; 

      // phase at time step i 
      psi = omega0*i + omega1*aux->t2[i] 
          + (cosalt*cosdelt*xet + sinalt*cosalt*yet + sindelt*zet)
           *(domega + omega0 + 2*omega1*i); 

      /* Phase derivatives w.r.t. intrinsic parameters A = (omega0, omega1, delta, alpha) 
       * freq = f = omega0, spindown = s = omega1, delta = d, alpha = a
       * 
       * dpdA[0] = dpdf, dpdA[1] = dpds, dpdA[2] = dpdd, dpdA[3] = dpda
       */     

      dpdA[0] = i + xet*cosalt*cosdelt + yet*cosdelt*sinalt + zet*sindelt;  
      dpdA[1] = aux->t2[i] + 2*i*(cosalt*cosdelt*xet + sinalt*cosalt*yet + sindelt*zet);   
      dpdA[2] = (domega + omega0 + 2*omega1*i)*(zet*cosdelt - (xet*cosalt + yet*sinalt)*sindelt); 
      dpdA[3] = (domega + omega0 + 2*omega1*i)*cosdelt*(yet*cosalt - xet*sinalt); 

      /* amplitude h = (a1*a + a2*b)*cos(psi) + (a3*a + a4*b)*sin(psi)
       * aa = a, bb = b 
       * 
       * h1 = h[0] = a*cos(psi)
       * h2 = h[1] = b*cos(psi) 
       * h3 = h[2] = a*sin(psi) 
       * h4 = h[3] = b*sin(psi) 
       */ 

      // amplitude modulation function at time step i 
      aa = ifo[n].sig.aa[i]; 
      bb = ifo[n].sig.bb[i]; 

      // derivatives of amplitude modulation functions 
      // with respect to f, s, d and a  

      daadA[0] = 0.; 
      daadA[1] = 0.; 
      daadA[2] = ifo[n].sig.daaddel[i]; 
      daadA[3] = ifo[n].sig.daadalf[i];
     
      dbbdA[0] = 0.; 
      dbbdA[1] = 0.; 
      dbbdA[2] = ifo[n].sig.dbbddel[i];
      dbbdA[3] = ifo[n].sig.dbbdalf[i];

      // Components of the GW amplitude 
      h[0] = aa*cos(psi); 
      h[1] = bb*cos(psi); 
      h[2] = aa*sin(psi); 
      h[3] = bb*sin(psi); 
    
      amph = a[0]*h[0] + a[1]*h[1] + a[2]*h[2] + a[3]*h[3];     

      // sum of amplitude h squares 
      sumhsq += amph*amph; 

      // Derivatives of h_i with respect to f, s, d and a 
      for(l=0; l<dim2; l++) { 
        dh[0][l] = -aa*sin(psi)*dpdA[l] + cos(psi)*daadA[l]; 
        dh[1][l] = -bb*sin(psi)*dpdA[l] + cos(psi)*dbbdA[l];
        dh[2][l] =  aa*cos(psi)*dpdA[l] + sin(psi)*daadA[l]; 
        dh[3][l] =  bb*cos(psi)*dpdA[l] + sin(psi)*dbbdA[l];
      } 
 
      // F(A)
      for(l=0; l<dim2; l++) 
        for(k=0; k<dim2; k++) 
          for(j=0; j<dim2; j++)  
            F[l][k][j] += h[k]*dh[j][l]; 

      // S(A, B) 
      for(m=0; m<dim2; m++)
        for(l=0; l<dim2; l++)
          for(k=0; k<dim2; k++) 
            for(j=0; j<dim2; j++) 
              S[m][l][k][j] += dh[k][m]*dh[j][l]; 
 

      // Fisher matrix elements 
      ma[0][0] = (aa*aa*a[0]*a[0]*dpdA[0]*dpdA[0])/2. + (aa*aa*a[2]*a[2]*dpdA[0]*dpdA[0])/2. + aa*a[0]*a[1]*bb*dpdA[0]*dpdA[0] + aa*a[2]*a[3]*bb*dpdA[0]*dpdA[0] + (a[1]*a[1]*bb*bb*dpdA[0]*dpdA[0])/2. + (a[3]*a[3]*bb*bb*dpdA[0]*dpdA[0])/2.; 
 
      ma[0][1] = (aa*aa*a[0]*a[0]*dpdA[1]*dpdA[0])/2 + (aa*aa*a[2]*a[2]*dpdA[1]*dpdA[0])/2. + aa*a[0]*a[1]*bb*dpdA[1]*dpdA[0] + aa*a[2]*a[3]*bb*dpdA[1]*dpdA[0] + (a[1]*a[1]*bb*bb*dpdA[1]*dpdA[0])/2. + (a[3]*a[3]*bb*bb*dpdA[1]*dpdA[0])/2.;
 
      ma[0][2] = (aa*aa*a[0]*a[0]*dpdA[0]*dpdA[2])/2 + (aa*aa*a[2]*a[2]*dpdA[0]*dpdA[2])/2. + aa*a[0]*a[1]*bb*dpdA[0]*dpdA[2] + aa*a[2]*a[3]*bb*dpdA[0]*dpdA[2] + (a[1]*a[1]*bb*bb*dpdA[0]*dpdA[2])/2. + (a[3]*a[3]*bb*bb*dpdA[0]*dpdA[2])/2.; 

      ma[0][3] = (aa*aa*a[0]*a[0]*dpdA[0]*dpdA[3])/2. + (aa*aa*a[2]*a[2]*dpdA[0]*dpdA[3])/2. + aa*a[0]*a[1]*bb*dpdA[0]*dpdA[3] + aa*a[2]*a[3]*bb*dpdA[0]*dpdA[3] + (a[1]*a[1]*bb*bb*dpdA[0]*dpdA[3])/2. + (a[3]*a[3]*bb*bb*dpdA[0]*dpdA[3])/2.;  

      ma[0][4] = (aa*aa*a[2]*dpdA[0])/2. + (aa*a[3]*bb*dpdA[0])/2.;  

      ma[0][5] = (aa*a[2]*bb*dpdA[0])/2. + (a[3]*bb*bb*dpdA[0])/2.;
 
      ma[0][6] = -(aa*aa*a[0]*dpdA[0])/2. - (aa*a[1]*bb*dpdA[0])/2.;
 
      ma[0][7] = -(aa*a[0]*bb*dpdA[0])/2. - (a[1]*bb*bb*dpdA[0])/2.; 

      ma[1][1] = (aa*aa*a[0]*a[0]*dpdA[1]*dpdA[1])/2. + (aa*aa*a[2]*a[2]*dpdA[1]*dpdA[1])/2. + aa*a[0]*a[1]*bb*dpdA[1]*dpdA[1] + aa*a[2]*a[3]*bb*dpdA[1]*dpdA[1] + (a[1]*a[1]*bb*bb*dpdA[1]*dpdA[1])/2. + (a[3]*a[3]*bb*bb*dpdA[1]*dpdA[1])/2.; 

      ma[1][2] = (aa*aa*a[0]*a[0]*dpdA[1]*dpdA[2])/2. + (aa*aa*a[2]*a[2]*dpdA[1]*dpdA[2])/2. + aa*a[0]*a[1]*bb*dpdA[1]*dpdA[2] + aa*a[2]*a[3]*bb*dpdA[1]*dpdA[2] + (a[1]*a[1]*bb*bb*dpdA[1]*dpdA[2])/2. + (a[3]*a[3]*bb*bb*dpdA[1]*dpdA[2])/2.;
 
      ma[1][3] = (aa*aa*a[0]*a[0]*dpdA[1]*dpdA[3])/2. + (aa*aa*a[2]*a[2]*dpdA[1]*dpdA[3])/2. + aa*a[0]*a[1]*bb*dpdA[1]*dpdA[3] + aa*a[2]*a[3]*bb*dpdA[1]*dpdA[3] + (a[1]*a[1]*bb*bb*dpdA[1]*dpdA[3])/2. + (a[3]*a[3]*bb*bb*dpdA[1]*dpdA[3])/2.;

      ma[1][4] = (aa*aa*a[2]*dpdA[1])/2. + (aa*a[3]*bb*dpdA[1])/2.;
 
      ma[1][5] = (aa*a[2]*bb*dpdA[1])/2. + (a[3]*bb*bb*dpdA[1])/2.; 
 
      ma[1][6] = -(aa*aa*a[0]*dpdA[1])/2. - (aa*a[1]*bb*dpdA[1])/2.;
 
      ma[1][7] = -(aa*a[0]*bb*dpdA[1])/2. - (a[1]*bb*bb*dpdA[1])/2.;
 
      ma[2][2] = (aa*aa*a[0]*a[0]*dpdA[2]*dpdA[2])/2. + (aa*aa*a[2]*a[2]*dpdA[2]*dpdA[2])/2. + aa*a[0]*a[1]*bb*dpdA[2]*dpdA[2] + aa*a[2]*a[3]*bb*dpdA[2]*dpdA[2] + (a[1]*a[1]*bb*bb*dpdA[2]*dpdA[2])/2. + (a[3]*a[3]*bb*bb*dpdA[2]*dpdA[2])/2.;
 
      ma[2][3] = (aa*aa*a[0]*a[0]*dpdA[2]*dpdA[3])/2. + (aa*aa*a[2]*a[2]*dpdA[2]*dpdA[3])/2. + aa*a[0]*a[1]*bb*dpdA[2]*dpdA[3] + aa*a[2]*a[3]*bb*dpdA[2]*dpdA[3] + (a[1]*a[1]*bb*bb*dpdA[2]*dpdA[3])/2. + (a[3]*a[3]*bb*bb*dpdA[2]*dpdA[3])/2.;
 
      ma[2][4] = (aa*aa*a[2]*dpdA[2])/2. + (aa*a[3]*bb*dpdA[2])/2.;
 
      ma[2][5] = (aa*a[2]*bb*dpdA[2])/2. + (a[3]*bb*bb*dpdA[2])/2.;
 
      ma[2][6] = -(aa*aa*a[0]*dpdA[2])/2. - (aa*a[1]*bb*dpdA[2])/2.;
 
      ma[2][7] = -(aa*a[0]*bb*dpdA[2])/2. - (a[1]*bb*bb*dpdA[2])/2.;
 
      ma[3][3] = (aa*aa*a[0]*a[0]*dpdA[3]*dpdA[3])/2. + (aa*aa*a[2]*a[2]*dpdA[3]*dpdA[3])/2. + aa*a[0]*a[1]*bb*dpdA[3]*dpdA[3] + aa*a[2]*a[3]*bb*dpdA[3]*dpdA[3] + (a[1]*a[1]*bb*bb*dpdA[3]*dpdA[3])/2. + (a[3]*a[3]*bb*bb*dpdA[3]*dpdA[3])/2.;
 
      ma[3][4] = (aa*aa*a[2]*dpdA[3])/2. + (aa*a[3]*bb*dpdA[3])/2.;
 
      ma[3][5] = (aa*a[2]*bb*dpdA[3])/2. + (a[3]*bb*bb*dpdA[3])/2.;
 
      ma[3][6] = -(aa*aa*a[0]*dpdA[3])/2. - (aa*a[1]*bb*dpdA[3])/2.;
 
      ma[3][7] = -(aa*a[0]*bb*dpdA[3])/2. - (a[1]*bb*bb*dpdA[3])/2.;
 
      ma[4][4] = aa*aa/2.;
 
      ma[4][5] = (aa*bb)/2.;
 
      ma[4][6] = 0;
 
      ma[4][7] = 0;
 
      ma[5][5] = bb*bb/2.;
 
      ma[5][6] = 0;
 
      ma[5][7] = 0;
 
      ma[6][6] = aa*aa/2.;
 
      ma[6][7] = (aa*bb)/2.;
 
      ma[7][7] = bb*bb/2.;


      // mFl 
      for(k=0; k<dim; k++) 
        for(j=0; j<dim; j++) 
          mFl[k][j] += ma[k][j];

    } /* end of loop over data points sett->N */  

    // Symmetrize mFl 
    for(k=1; k<dim; k++) 
      for(j=0; j<k; j++)
        mFl[k][j] = mFl[j][k]; 

/*
    printf("A0=omega0, A1=omega1, A2=delta, A3=alpha\n"); 

    for(l=0; l<dim2; l++) { 
      printf("\nF(A%d):\n", l); 
      for(k=0; k<dim2; k++) { 
        for(j=0; j<dim2; j++)  
          printf("%.16e ", F[l][k][j]);
        printf("\n");
      }
    } 

    for(m=0; m<dim2; m++)
      for(l=0; l<=m; l++) { 
        printf("\nS(A%d, A%d):\n", m, l); 
        for(k=0; k<dim2; k++) {  
          for(j=0; j<dim2; j++) 
            printf("%.16e ", S[m][l][k][j]);
          printf("\n");
        } 
      }

*/  

    //#mb printf("\nsumhsq, sqrt(sumhsq), N: %f %f %d\n", sumhsq, sqrt(sumhsq), sett->N); 

/*
    printf("\nThe Fisher matrix:\n"); 

*/ 

    int r;
    arb_t t; 
    arb_init(t); 

 
    arb_mat_t AA, T; 

    arb_mat_init(AA, dim, dim); 
    arb_mat_init(T, dim, dim); 


    for(k=0; k<dim; k++) { 
//      printf("[");
      for(j=0; j<dim; j++) { 
//        printf("%.16e ", mFl[k][j]);
        arb_set_d(arb_mat_entry(AA, k, j), mFl[k][j]);
      } 
//      printf("\n");
    }

//    printf("Inverting the Fisher matrix...\n"); 

    r = arb_mat_spd_inv(T, AA, prec);

    if(r) {
      
      printf("Diagonal elements of the covariance matrix:\n");  
      for(k=0; k<dim; k++) { 
        arb_set(t, arb_mat_entry(T, k, k)); 
        printf("%le ", arf_get_d(arb_midref(t), ARF_RND_DOWN));
      }
    } else { 
    
      printf("Failed to invert the Fisher matrix.\n"); 
    } 

    printf("\n"); 

    arb_mat_clear(AA);
    arb_mat_clear(T);  

    arb_mat_t mFl1, M1; 

    arb_mat_init(mFl1, dim2, dim2); 
    arb_mat_init(M1, dim2, dim2); 
    
    //printf("F(5:8,5:8) Fisher Matrix:\n"); 

    for(k=dim2; k<dim; k++) { 
      for(j=dim2; j<dim; j++) { 
        arb_set_d(arb_mat_entry(mFl1, k-dim2, j-dim2), mFl[k][j]);
        //printf("%.16le ", mFl[k][j]); 
      } 

      //printf("\n"); 

    }
   
    //printf("Inverting the mFL1 matrix...\n"); 

    r = arb_mat_inv(M1, mFl1, prec);

/*
    if(r) {

      for(k=0; k<dim2; k++) { 
        for(j=0; j<dim2; j++) { 
          arb_set(t, arb_mat_entry(M1, k, j)); 
          printf("%.16le ", arf_get_d(arb_midref(t), ARF_RND_DOWN));
        } 
        printf("\n");
      }

    } else { 
      printf("Failed to invert the mFl1 matrix.\n"); 
    } 

    printf("\n"); 
*/


    // Reduced Fisher matrix 
    //----------------------

    int A, B; 
    double redF[dim2][dim2];

    double Nd = (double)(sett->N);

    arb_t Narb;
    arb_init(Narb); 
    arb_set_d(Narb, Nd);   

    for(A=0; A<dim2; A++) { 
      for(B=0; B<=A; B++) { // loops over A, B

        arb_mat_t mA, mSAB, mFAT, mFB; 
  
        arb_mat_init(mA, dim2, dim2); 
        arb_mat_init(mSAB, dim2, dim2); 
        arb_mat_init(mFAT, dim2, dim2); 
        arb_mat_init(mFB, dim2, dim2); 

        for(k=0; k<dim2; k++) { 
          for(j=0; j<dim2; j++) {

            arb_set_d(arb_mat_entry(mSAB, k, j), S[A][B][k][j]);

            // matrix mFA transposed --> mFAT
            arb_set_d(arb_mat_entry(mFAT, k, j),  F[A][j][k]);
            arb_set_d(arb_mat_entry(mFB, k, j),  F[B][k][j]);

          } 
        }

        // mA = SAB - FA^T * M^-1 * FB
        // overwriting to avoid too many matrices  
        arb_mat_mul(mFAT, mFAT, M1, prec); 
        arb_mat_mul(mFAT, mFAT, mFB, prec);    
        arb_mat_sub(mA, mSAB, mFAT, prec); 

        arb_mat_mul(mA, mA, M1, prec);

        // trace(mA * M^-1)  
        arb_mat_trace(t, mA, prec);

        // normalization by N^2
        arb_div(t, t, Narb, prec); 
        arb_div(t, t, Narb, prec);
        // additional normalization for spindown  
        if(A==1)
         arb_div(t, t, Narb, prec); 
        if(B==1)
          arb_div(t, t, Narb, prec); 

        redF[A][B] = arf_get_d(arb_midref(t), ARF_RND_DOWN)/dim2;

        arb_mat_clear(mA); 
        arb_mat_clear(mSAB); 
        arb_mat_clear(mFAT);
        arb_mat_clear(mFB); 

      } /* end of loop over B */   

    } /* end of loop over A */ 

    // Symmetrize redF
    for(k=1; k<dim2; k++) 
      for(j=0; j<k; j++) 
        redF[j][k] = redF[k][j]; 

    printf("\nReduced Fisher matrix:\n"); 
    for(k=0; k<dim2; k++) { 
      for(j=0; j<dim2; j++) 
        printf("%.16le ", redF[k][j]);
      printf("\n");
    } 

    arb_mat_clear(mFl1);
    arb_mat_clear(M1); 

  } /* end of the detector loop */   

  return 0; 

} 

void modvir_da(
  double sinal, 
  double cosal, 
  double sindel, 
  double cosdel,
  int Np,
  Detector_settings *ifo, 
  Aux_arrays *aux) {

  int t;
  double cosalfr, sinalfr, s2d, c2d, c2sd, c, s, c2s, cs;

  double c1 = ifo->amod.c1,
         c2 = ifo->amod.c2,
         c3 = ifo->amod.c3,
         c4 = ifo->amod.c4,
         c5 = ifo->amod.c5,
         c6 = ifo->amod.c6,
         c7 = ifo->amod.c7,
         c8 = ifo->amod.c8,
         c9 = ifo->amod.c9;

  cosalfr = cosal*(ifo->sig.cphir) + sinal*(ifo->sig.sphir);
  sinalfr = sinal*(ifo->sig.cphir) - cosal*(ifo->sig.sphir);

  c2d = sqr(cosdel);
  s2d = sqr(sindel); 
  c2sd = sindel*cosdel;

  // Modulation factor for every data point 
  for (t=0; t<Np; t++) { 

    c = cosalfr*aux->cosmodf[t] + sinalfr*aux->sinmodf[t];
    s = sinalfr*aux->cosmodf[t] - cosalfr*aux->sinmodf[t];

    c2s = 2.*sqr(c);
    cs = c*s;

    // dcda = -s, dsda = c 

    /* // modulation factors aa and bb  
    ifo->sig.aa[t] = c1*(2.-c2d)*c2s + c2*(2.-c2d)*2.*cs +
           c3*c2sd*c + c4*c2sd*s - c1*(2.-c2d) + c5*c2d;

    ifo->sig.bb[t] = c6*sindel*c2s + c7*sindel*2.*cs + 
           c8*cosdel*c + c9*cosdel*s - c6*sindel;
    */ 
  
    // Derivatives of aa and bb with respect to delta and alpha 

    ifo->sig.daaddel[t] = 2.*c1*c2sd*c2s + 4.*c2*c2sd*cs  
          + c3*c*(c2d - s2d) + c4*s*(c2d - s2d) - 2.*c2sd*(c1 - c5);

    ifo->sig.dbbddel[t] = c6*cosdel*c2s + c7*cosdel*2.*cs 
          - c8*sindel*c - c9*sindel*s - c6*cosdel;

    ifo->sig.daadalf[t] = -4.*c1*(2.-c2d)*cs  
          + 2.*c2*(2.-c2d)*(c*c - s*s) - c3*c2sd*s + c4*c2sd*c;  

    ifo->sig.dbbdalf[t] = -4.*c6*sindel*cs 
          + 2.*c7*sindel*(c*c - s*s) - c8*cosdel*s + c9*cosdel*c;  

  } 

} // modvir_da 


  /* Cleanup & memory free (calculation 
   * of the Fisher matrix) 
	 */

void cleanup_fisher(
	Search_settings *sett,
	Command_line_opts *opts,
	Aux_arrays *aux, 
  double *F) {

  int i; 

  for(i=0; i<sett->nifo; i++) {
    free(ifo[i].sig.xDat);
    free(ifo[i].sig.xDatma);
    free(ifo[i].sig.xDatmb); 
    free(ifo[i].sig.DetSSB);
    free(ifo[i].sig.aa);
    free(ifo[i].sig.bb);

    free(ifo[i].sig.daaddel);
    free(ifo[i].sig.dbbddel);
    free(ifo[i].sig.daadalf);
    free(ifo[i].sig.dbbdalf);

    free(ifo[i].sig.shftf);
    free(ifo[i].sig.shft);
  } 
	
  free(aux->sinmodf);
  free(aux->cosmodf);
  free(aux->t2);
  free(F);  


} // end of cleanup & memory free 



/*  Command line options handling: fisher 
 */ 

void handle_opts_fisher( Search_settings *sett, 
		  Command_line_opts *opts,
		  int argc, 
		  char* argv[]) {

  strcpy (opts->prefix, TOSTR(PREFIX));
  strcpy (opts->dtaprefix, TOSTR(DTAPREFIX));

  opts->label[0]    = '\0';
  opts->range[0]    = '\0';
  opts->getrange[0] = '\0';
  opts->usedet[0]   = '\0';
  opts->addsig[0]   = '\0';
	
  // Initial value of starting frequency set to a negative quantity. 
  // If this is not changed by the command line value, fpo is calculated 
  // from the band number b (fpo = fpo = fstart + 0.96875*b/(2dt))
  sett->fpo = -1;

  // Default initial value of the data sampling time 
  sett->dt = 0.5; 

  // Initial value of the number of days is set to 0
  sett->nod = 0; 

  opts->help_flag=0;

  static int help_flag=0;

  // Reading arguments 

  while (1) {
    static struct option long_options[] = {
      {"help", no_argument, &help_flag, 1},
      // frame number
      {"ident", required_argument, 0, 'i'},
      // frequency band number
      {"band", required_argument, 0, 'b'},
      // input data directory
      {"data", required_argument, 0, 'd'},
      // fpo value
      {"fpo", required_argument, 0, 'p'},
      // add signal parameters
      {"addsig", required_argument, 0, 'x'},
      // number of days in the time-domain segment 
      {"nod", required_argument, 0, 'y'},
      // which detectors to use
      {"usedet", required_argument, 0, 'u'}, 
      // data sampling time 
      {"dt", required_argument, 0, 's'},
      {0, 0, 0, 0}
    };

    if (help_flag) {

      printf("polgraw-allsky periodic GWs: Fisher matrix calculation\n");
      printf("Usage: ./fisher -[switch1] <value1> -[switch2] <value2> ...\n") ;
      printf("Switches are:\n\n");
      printf("-data         Data directory (default is .)\n");
      printf("-ident        Frame number\n");
      printf("-band         Band number\n");
      printf("-fpo          Reference band frequency fpo value\n");
      printf("-dt           Data sampling time dt (default value: 0.5)\n");
      printf("-usedet       Use only detectors from string (default is use all available)\n");
      printf("-addsig       Add signal with parameters from <file>\n");
      printf("-nod          Number of days\n\n");


      printf("Also:\n\n");
      printf("--help            This help\n");

      exit(EXIT_SUCCESS);
    }

    int option_index = 0;
    int c = getopt_long_only(argc, argv, "i:b:dp:x:y:s:u:", 
			     long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 'i':
      opts->ident = atoi (optarg);
      break;
    case 'b':
      opts->band = atoi(optarg);
      break;
    case 'd':
      strcpy(opts->dtaprefix, optarg);
      break;
    case 'p':
      sett->fpo = atof(optarg);
      break;
    case 'x':
      strcpy(opts->addsig, optarg);
      break;
    case 'y':
      sett->nod = atoi(optarg);
      break;
    case 's':
      sett->dt = atof(optarg);
      break;
    case 'u':
      strcpy(opts->usedet, optarg);
      break;
    case '?':
      break;
    default:
      break ;
    } /* switch c */
  } /* while 1 */

  // Check if sett->nod was set up, if not, exit
  if(!(sett->nod)) { 
    printf("Number of days not set... Exiting\n"); 
    exit(EXIT_FAILURE); 
  } 

  printf("Number of days is %d\n", sett->nod); 

  printf("Input data directory is %s\n", opts->dtaprefix);
  printf("Frame and band numbers are %d and %d\n", opts->ident, opts->band);

  // Starting band frequency:
  // fpo_val is optionally read from the command line
  // Its initial value is set to -1
  if(!(sett->fpo >= 0))

    // The usual definition (multiplying the offset by B=1/(2dt))
    // !!! in RDC_O1 the fstart equals 10, not 100 like in VSR1 !!! 
    // 
    sett->fpo = 10. + 0.96875*opts->band*(0.5/sett->dt);

  printf("The reference frequency fpo is %f\n", sett->fpo);
  printf("The data sampling time dt is %f\n", sett->dt); 

  if (strlen(opts->addsig))
    printf ("Adding signal from '%s'\n", opts->addsig);

} // end of command line options handling: fisher  

