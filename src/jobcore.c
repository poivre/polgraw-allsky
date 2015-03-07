#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <gsl/gsl_vector.h>
#include <complex.h>
#include <fftw3.h>

/* JobCore file */
#include "jobcore.h"
#include "auxi.h"
#include "settings.h"
#include "timer.h"


void save_array(complex double *arr, int N, const char* file) {
  int i;
  FILE *fc = fopen(file, "w");
  for (i=0; i<N; i++) {
    fprintf(fc, "%d %e + i %e\n", i, creal(arr[i]), cimag(arr[i]));
  }
  fclose(fc);
}

void save_array_double(double *arr, int N, const char* file) {
  int i;
  FILE *fc = fopen(file, "w");
  for (i=0; i<N; i++) {
    fprintf(fc, "%d %e\n", i, arr[i]);
  }
  fclose(fc);
}


//main searching function (loops inside)
void search(
  Search_settings *sett,
  Command_line_opts *opts,
  Search_range *s_range,
  FFTW_plans *plans,
  FFTW_arrays *fftw_arr,
  Aux_arrays *aux,
  int *FNum,
  double *F) {

  // struct stat buffer;
  struct flock lck;
 
  //	clock_t cstart, cend; //clock
  //	double time_elapsed; //for measuring time

  int pm, mm, nn;
  int sgnlc; //number of canditates
  double *sgnlv; //array with candidates data

  char outname[64], qname[64];
  int fd;
	
  FILE *state;
  if(opts->checkp_flag) {
    if(opts->hemi)
      sprintf (qname, "state_%03d_%03d%s_%d.dat", opts->ident, opts->band, opts->label, opts->hemi);
    else
      sprintf (qname, "state_%03d_%03d%s.dat", opts->ident, opts->band, opts->label);

    state = fopen (qname, "w");

  }
	
  //struct timespec tstart = get_current_time(), tend;
		
 
  /* Loop over hemispheres
   */ 

  for (pm=s_range->pst; pm<=s_range->pmr[1]; pm++) {	

    sprintf (outname, "%s/triggers_%03d_%03d%s_%d.bin", 
        opts->prefix, opts->ident, opts->band, opts->label, pm);

    /* Two main loops over sky positions 
     */ 

    for (mm=s_range->mst; mm<=s_range->mr[1]; mm++) {	
      for (nn=s_range->nst; nn<=s_range->nr[1]; nn++) {	

	      if(opts->checkp_flag) {
	        fprintf(state, "%d %d %d %d %d\n", 
              pm, mm, nn, s_range->sst, *FNum);
	        fflush(state);
	      }

	/* Loop over Spindows here */
	      sgnlv = job_core(
			          pm,          // hemisphere
			          mm,	      // grid 'sky position'
			          nn,	      // other grid 'sky position'
			          sett,        // search settings
			          opts,        // cmd opts
			          s_range,     // range for searching
                plans, 
			          fftw_arr,   // arrays for fftw
			          aux, 	      // auxiliary arrays
			          F,	        // F-statistics array
			          &sgnlc,     // reference to array with the parameters
			                      // of the candidate signal
			                      // (used below to write to the file)
			          FNum);	    // Candidate signal number
				
	//get back to regular spin-down range
	s_range->sst = s_range->spndr[0];
	/* Add trigger parameters to a file */

	//if any signals found (Fstat>Fc)
	if (sgnlc) {
	  if ((fd = open (outname, O_WRONLY|O_CREAT|O_APPEND,
			  S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0) {
	    perror (outname);
	    return;
	  }
	  lck.l_type = F_WRLCK;
	  lck.l_whence = 0;
	  lck.l_start = 0L;
	  lck.l_len = 0L;
	  if (fcntl (fd, F_SETLKW, &lck) < 0) perror ("fcntl()");

    /* //#mb debugging  
	  int jj, kk;
	  printf("\nParameters to save:\n");
	  for (jj=0; jj<sgnlc; jj++) {
	    for(kk=0; kk<NPAR; kk++) {
	      printf("%lf ", sgnlv[jj*NPAR+kk]);
	    }
	    printf("\n");
	  }
	  */
										
	  write (fd, (void *)(sgnlv), sgnlc*NPAR*sizeof (double));

	  if (close (fd) < 0) perror ("close()");

	} /* if sgnlc */
	free (sgnlv);
      } // for nn
      s_range->nst = s_range->nr[0];
    } // for mm
    s_range->mst = s_range->mr[0];
  } // for pm

  if(opts->checkp_flag) {
    fclose (state);
  }

  //tend = get_current_time();
	
  // printf("tstart = %d . %d\ntend = %d . %d\n", tstart.tv_sec, tstart.tv_usec, tend.tv_sec, tend.tv_usec);
	
  //double time_elapsed = get_time_difference(tstart, tend);
  //printf("Time elapsed: %e s\n", time_elapsed);

}


  /* Main job 
   */ 

double* job_core(
  int pm,		                // Hemisphere
  int mm,		                // Grid 'sky position'
  int nn,		                // Second grid 'sky position'
  Search_settings *sett,    // Search settings
  Command_line_opts *opts,  // Search options 
  Search_range *s_range,	  // Range for searching
  FFTW_plans *plans,        // Plans for fftw
  FFTW_arrays *fftw_arr,    // Arrays for fftw
  Aux_arrays *aux, 	        // Auxiliary arrays
  double *F,		            // F-statistics array
  int *sgnlc,		            // Candidate trigger parameters 
  int *FNum) {	            // Candidate signal number

  int i, j, n;
  int smin = s_range->sst, smax = s_range->spndr[1];
  double al1, al2, sinalt, cosalt, sindelt, cosdelt, sgnlt[NPAR], 
    nSource[3], het0, sgnl0, *sgnlv, ft;

  /* Matrix	M(.,.) (defined on page 22 of PolGrawCWAllSkyReview1.pdf file)
     defines the transformation form integers (bin, ss, nn, mm) determining
     a grid point to linear coordinates omega, omegadot, alpha_1, alpha_2),
     where bin is the frequency bin number and alpha_1 and alpha_2 are
     defined on p. 22 of PolGrawCWAllSkyReview1.pdf file.

     [omega]													[bin]
     [omegadot]	 			= M(.,.) \times [ss]
     [alpha_1/omega]									[nn]
     [alpha_2/omega]									[mm]

     Array M[.] is related to matrix M(.,.) in the following way;

                 [ M[0] M[4] M[8]	M[12] ]
      M(.,.) =	 [ M[1] M[5] M[9]	M[13] ]
                 [ M[2] M[6] M[10] M[14] ]
                 [ M[3] M[7] M[11] M[15] ]

     and

     M[1] = M[2] = M[3] = M[6] = M[7] = 0
  */

  // Grid positions
  al1 = nn*sett->M[10] + mm*sett->M[14];
  al2 = nn*sett->M[11] + mm*sett->M[15];

  sgnlv = NULL;
  *sgnlc = 0;

  // check if the search is in an appropriate region of the grid
  // if not, returns NULL
  if((sqr(al1)+sqr(al2))/sqr(sett->oms) > 1.) return NULL ;

  int ss;
  double shft1, phase, cp, sp;
  complex double exph;

  //	fftw_arr->xDb = fftw_arr->xDa + sett->nfft; //position of 'b' array
  //	fftw_arr->rDb = fftw_arr->rDa + sett->Ninterp; //same
  //	fftw_arr->xb = fftw_arr->xbo =


  //!
  // Switch for different interpolation methods
  // (set while calling JobCore() from JobNAllSky.c)
  //	switch (opts->fftinterp) {
  //	case INT:
  //		fftw_arr->xb = fftw_arr->xa + sett->nfft;
  //		fftw_arr->xbo = fftw_arr->xao + sett->nfft;
  //		break;
  //	case FFT:
  fftw_arr->xb = fftw_arr->xa + fftw_arr->arr_len;// + sett->fftpad * sett->nfft;
  //	} /* case fftinterp */

  //change linear (grid) coordinates to real coordinates
  lin2ast(al1/sett->oms, al2/sett->oms, 
      pm, sett->sepsm, sett->cepsm,
	    &sinalt, &cosalt, &sindelt, &cosdelt);

  // calculate declination and right ascention
  // written in file as candidate signal sky positions
  sgnlt[2] = asin(sindelt);
  sgnlt[3] = fmod(atan2(sinalt, cosalt) + 2.*M_PI, 2.*M_PI);

  het0 = fmod(nn*sett->M[8] + mm*sett->M[12], sett->M[0]);

  // Loop for each detector 
  for(n=0; n<sett->nifo;n++) { 

    /* Amplitude modulation functions aa and bb 
     * for each detector (in signal sub-struct 
     * of _detector, ifo[n].sig.aa, ifo[n].sig.bb) 
     */

    modvir(sinalt, cosalt, sindelt, cosdelt,
	         sett->N, &ifo[n], aux);
	
    // Calculate detector positions with respect to baricenter
    nSource[0] = cosalt*cosdelt;
    nSource[1] = sinalt*cosdelt;
    nSource[2] = sindelt;

    shft1 = 0.;
    for (j=0; j<3; j++)
      shft1 += nSource[j]*(ifo[n].sig.DetSSB[j]);

    for(i=0; i<sett->N; i++) {
      ifo[n].sig.shft[i] = 0.;

      for(j=0; j<3; j++)
        ifo[n].sig.shft[i] += nSource[j]*(ifo[n].sig.DetSSB[i*3+j]);
    
      ifo[n].sig.shftf[i] = ifo[n].sig.shft[i] - shft1;

      // Phase modulation 
      phase = het0*i + sett->oms*(ifo[n].sig.shft[i]);
      cp = cos(phase);
      sp = sin(phase);
      exph = cp - I*sp;

      // Matched filter 
      ifo[n].sig.xDatma[i] = 
        ifo[n].sig.xDat[i]*ifo[n].sig.aa[i]*exph;
      ifo[n].sig.xDatmb[i] = 
        ifo[n].sig.xDat[i]*ifo[n].sig.bb[i]*exph;
  
    } 


    /* Resampling using spline interpolation:
     * This will double the sampling rate 
     */ 
  
    for(i=0; i < sett->N; i++) { 
      fftw_arr->xa[i] = ifo[n].sig.xDatma[i];
      fftw_arr->xb[i] = ifo[n].sig.xDatmb[i];
    }
 
    // Zero-padding (filling with 0s up to sett->nfft, 
    // the nearest power of 2)
    for (i=sett->N; i<sett->nfft; i++) { 
      fftw_arr->xa[i] = 0.;
      fftw_arr->xb[i] = 0.;
    }

    fftw_execute(plans->pl_int);  //forward fft (len nfft)
    fftw_execute(plans->pl_int2); //forward fft (len nfft)

    int nyqst = (sett->nfft+2)/2; // Nyquist frequency
	
    // move frequencies from second half of spectrum; 
    // loop length: nfft - nyqst = nfft - nfft/2 - 1 = nfft/2 - 1
    for(i=nyqst + sett->Ninterp - sett->nfft, j=nyqst; 
        i<sett->Ninterp; i++, j++) {
      fftw_arr->xa[i] = fftw_arr->xa[j];
      fftw_arr->xb[i] = fftw_arr->xb[j];
    }
	
    //  zero frequencies higher than nyquist
    for (i=nyqst; i<nyqst + sett->Ninterp - sett->nfft; i++) {
      fftw_arr->xa[i] = 0;
      fftw_arr->xb[i] = 0;
    }

    // Backward fft (len Ninterp = nfft*interpftpad)
    fftw_execute (plans->pl_inv);     
    fftw_execute (plans->pl_inv2); 

    ft = (double)sett->interpftpad / sett->Ninterp; //scale FFT
    for (i=0; i < sett->Ninterp; i++) {
      fftw_arr->xa[i] *= ft;
      fftw_arr->xb[i] *= ft;
    }

	  //	struct timeval tstart = get_current_time(), tend;

    // Spline interpolation to xDatma, xDatmb arrays
    splintpad(fftw_arr->xa, ifo[n].sig.shftf, sett->N, 
      sett->interpftpad, ifo[n].sig.xDatma);   
    splintpad(fftw_arr->xb, ifo[n].sig.shftf, sett->N, 
      sett->interpftpad, ifo[n].sig.xDatmb);

  //	tend = get_current_time();
	
  //	double time_elapsed = get_time_difference(tstart, tend);
  //	printf("Time elapsed, 2 splines: %e s\n", time_elapsed);

  } // end of detector loop 


  // square sums of modulation factors 
  double aa = 0.; 
  double bb = 0.; 

  for(n=0; n<sett->nifo;n++) {

    double aatemp = 0.; 
    double bbtemp = 0.;
 
    for(i=0; i<sett->N; i++) {
      aatemp += sqr(ifo[n].sig.aa[i]);
      bbtemp += sqr(ifo[n].sig.bb[i]);
    }

    aa += aatemp/ifo[n].sig.sig2; 
    bb += bbtemp/ifo[n].sig.sig2;   
  }


  /* Spindown loop 
   */

  //struct timespec tstart, tend;
  //double spindown_timer=0;
  //int spindown_counter = 0;

  printf ("\n>>%d\t%d\t%d\t[%d..%d]\n", *FNum, mm, nn, smin, smax);

  // if no-spindown
  if(opts->s0_flag) smin = smax;
  // if spindown parameter is taken into account,
  // smin != smax
  for(ss=smin; ss<=smax; ss++) {
    //tstart = get_current_time();
    // Spindown parameter
    sgnlt[1] = (opts->s0_flag ? 0. : ss*sett->M[5] + nn*sett->M[9] + mm*sett->M[13]);

    if(sgnlt[1] >= -sett->Smax && sgnlt[1] <= 0.) { //look only for negative-valued spindowns
      int ii;
      double Fc, het1;

      //print a 'dot' every new spindown
      printf ("."); fflush (stdout);

      het1 = fmod(ss*sett->M[4], sett->M[0]);
      if(het1<0) het1 += sett->M[0];

      sgnl0 = het0 + het1;

      // phase modulation before fft

      for(i=0; i<sett->N; i++) {
	      phase = het1*i + sgnlt[1]*(aux->t2[i] + 2.*i*ifo[0].sig.shft[i]);  

        cp = cos(phase);
	      sp = sin(phase);
	      exph = cp - I*sp;

        fftw_arr->xa[i] = ifo[0].sig.xDatma[i]*exph/(ifo[0].sig.sig2);
        fftw_arr->xb[i] = ifo[0].sig.xDatmb[i]*exph/(ifo[0].sig.sig2);    
        
      }

      for(n=1; n<sett->nifo; n++) {

        for(i=0; i<sett->N; i++) {
	        phase = het1*i + sgnlt[1]*(aux->t2[i] + 2.*i*ifo[n].sig.shft[i]);  
          cp = cos(phase);
	        sp = sin(phase);
	        exph = cp - I*sp;

          fftw_arr->xa[i] += ifo[n].sig.xDatma[i]*exph/(ifo[n].sig.sig2);
          fftw_arr->xb[i] += ifo[n].sig.xDatmb[i]*exph/(ifo[n].sig.sig2);    
        }
      } 

      //#mb currently only one option: FFT
      //			switch (opts->fftinterp) { 
      // Padding by fftpad times zeros
      for(i = sett->N; i<sett->fftpad*sett->nfft; i++)
	      fftw_arr->xa[i] = fftw_arr->xb[i] = 0.; 

      fftw_execute (plans->plan);
      fftw_execute (plans->plan2);

      (*FNum)++;

      // Computing F-statistic 
      for (i=sett->nmin; i<sett->nmax; i++) {
	      F[i] = (sqr(creal(fftw_arr->xa[i])) 
             + sqr(cimag(fftw_arr->xa[i])))/aa  
             + (sqr(creal(fftw_arr->xb[i])) 
             + sqr(cimag(fftw_arr->xb[i])))/bb;
      }

      // Normalize F-statistics 
      if(!(opts->white_flag))	// if the noise is not white noise
	      FStat(F + sett->nmin, sett->nmax - sett->nmin, NAV, 0);

      for(i=sett->nmin; i<sett->nmax; i++) {
	      if ((Fc = F[i]) > opts->trl) { //if F-stat exceeds trl (critical value)
	        // Find local maximum for neighboring signals 
	        ii = i;

	        while (++i < sett->nmax && F[i] > opts->trl) {
      	    if(F[i] >= Fc) {
	            ii = i;
	            Fc = F[i];
	          } // if F[i] 
	        } // while i 
	  // Candidate signal frequency
					
	  sgnlt[0] = 2.*M_PI*ii/((double) sett->fftpad*sett->nfft) + sgnl0;
	  // Signal-to-noise ratio
	  sgnlt[4] = sqrt(2.*(Fc-sett->nd));

    //#mb
    /* 
	  printf("\n%lf %lf %lf %lf %lf\n", 
      sgnlt[0], sgnlt[1], sgnlt[2], sgnlt[3], sgnlt[4]);
    */ 

	  (*sgnlc)++; // increase found number

	  // Add new parameters to output array 
	  sgnlv = (double *)realloc(sgnlv, NPAR*(*sgnlc)*sizeof (double));

	  for (j=0; j<NPAR; j++) // save new parameters
	    sgnlv[NPAR*(*sgnlc-1)+j] = sgnlt[j];

	  printf ("\nSignal %d: %d %d %d %d %d \tsnr=%.2f\n", 
		  *sgnlc, pm, mm, nn, ss, ii, sgnlt[4]);
	} // if Fc > trl 
      } // for i 

    
      /* //#mb for debugging 

      tend = get_current_time();
      spindown_timer += get_time_difference(tstart, tend);
      spindown_counter++;

      if(spindown_counter %10 == 0 && spindown_counter > 0 ) {
        printf("\nTotal spindown loop time: %e s, mean spindown time: %e s (%d runs)\n", spindown_timer, spindown_timer/spindown_counter, spindown_counter);

      }

      */ 

    } // if sgnlt[1] 
  } // for ss 

  //printf("\nTotal spindown loop time: %e s, mean spindown time: %e s (%d runs)\n",
	// spindown_timer, spindown_timer/spindown_counter, spindown_counter);

  printf("\n"); 

  return sgnlv;

} // jobcore

