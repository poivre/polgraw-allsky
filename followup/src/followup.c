/* Ver. 1.0
Main followup programm. Required files
(all should be in directory 'data' - given 
as argument) :
'candidates.coi' - File with candidates from coincidences
'list.txt'
Ver. 2.0
Functions glue and neigh added!
Ver. 3.0
Mesh adaptive direct search (MADS) added (to find real
maximum)
TO DO: compare MADS with simplex and ...

MS
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <math.h>
#include <complex.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <fcntl.h>
#include <getopt.h>
#include <gsl/gsl_linalg.h>
#include <time.h>
#include <dirent.h>
#include <omp.h>

#include "auxi.h"
#include "settings.h"
#include "struct.h"
#include "init.h"
//#include "timer.h"

#include "glue.h"
#include "neigh.h"

#include <assert.h>
#if defined(SLEEF)
//#include "sleef-2.80/purec/sleef.h"
#include <sleefsimd.h>
#elif defined(YEPPP)
#include <yepLibrary.h>
#include <yepCore.h>
#include <yepMath.h>
#endif

// Default output and data directories

#ifndef PREFIX
#define PREFIX ./FSTAT_OUT
#endif

#ifndef DTAPREFIX
#define DTAPREFIX .
#endif

// Allocation of memory for martix with given number of rows and columns
float** matrix(int rows, int cols) {

  	int k;
	float **m;
  	m = (float **)malloc(rows*sizeof(float *));
  
  	for (k=0; k<rows; k++)
    		m[k] = (float *)calloc(cols, sizeof(float));
  
  	return m;
}

// Fstat function declaration
double* Fstatnet(Search_settings *sett, Command_line_opts *opts, Aux_arrays *aux, double *F, double *sgnlo, double *nSource){


	double xa_real = 0., xa_imag = 0., xb_real = 0., xb_imag = 0., xasum_real = 0., xasum_imag = 0., xbsum_real = 0., xbsum_imag = 0.;
	double shft1, cosPH, sinPH, phase[sett->N];
  	double sinalt, cosalt, sindelt, cosdelt;
	int i = 0, n = 0; 
	static double fstat_out[10]; //output
	double aa = 0., bb = 0., aaa = 0., bbb = 0.;
#ifdef YEPPP
//#define VLEN 2048
    int VLEN = sett->N;
    yepLibrary_Init();

    Yep64f _sph[VLEN];
    Yep64f _cph[VLEN];
    enum YepStatus status;

#endif

//From jobcore.c, line 237 
//Loop for each detector 
  	for(n=0; n < sett->nifo; ++n) { 

// Calculate detector positions with respect to baricenter
// Copied from jobcore.c, line 248

		xa_real = 0.;	
		xa_imag = 0.;
		xb_real = 0.;	
		xb_imag = 0.;
		aa = 0.;
		bb = 0.;
//Inside loop
    		for(i=0; i<sett->N; ++i) {

      			ifo[n].sig.shft[i] = nSource[0]*ifo[n].sig.DetSSB[i*3]
		         	+ nSource[1]*ifo[n].sig.DetSSB[i*3+1]
		         	+ nSource[2]*ifo[n].sig.DetSSB[i*3+2];
    
// Phase modulation function
// Copied from jobcore.c, line 265

			phase[i] = sgnlo[0]*(i + ifo[n].sig.shft[i]) 
				+ sgnlo[1]*i*i + (sett->oms 
				+ 2*sgnlo[1]*i)*ifo[n].sig.shft[i];

		}			

		status = yepMath_Cos_V64f_V64f(phase, _cph, VLEN);
		assert(status == YepStatusOk);
		status = yepMath_Sin_V64f_V64f(phase, _sph, VLEN);
		assert(status == YepStatusOk);

// Matched filter 
// Copied from jobcore.c, line 276 and 337

		for (i = 0; i<sett->N; ++i){
			xa_real = xa_real + ifo[n].sig.xDat[i]*ifo[n].sig.aa[i]*_cph[i];
			xa_imag = xa_imag - ifo[n].sig.xDat[i]*ifo[n].sig.aa[i]*_sph[i];
			xb_real = xb_real + ifo[n].sig.xDat[i]*ifo[n].sig.bb[i]*_cph[i];
			xb_imag = xb_imag - ifo[n].sig.xDat[i]*ifo[n].sig.bb[i]*_sph[i];		
			
     			aa += sqr(ifo[n].sig.aa[i]);
      			bb += sqr(ifo[n].sig.bb[i]);
	 
		}	// End of inside loop

    		aaa += aa/ifo[n].sig.sig2; 
    		bbb += bb/ifo[n].sig.sig2;

		xasum_real += xa_real/ifo[n].sig.sig2;	
		xasum_imag += xa_imag/ifo[n].sig.sig2;
		xbsum_real += xb_real/ifo[n].sig.sig2;
		xbsum_imag += xb_imag/ifo[n].sig.sig2;

	}		// End of detector loop

// F - statistic
	fstat_out[5] = - ((( sqr(xasum_real) + sqr(xasum_imag))/aaa)
			+ ((sqr(xbsum_real) +sqr(xbsum_imag))/bbb));

// Amplitude estimates
	fstat_out[0] = 2*xasum_real/aaa;
	fstat_out[1] = 2*xbsum_real/bbb;
	fstat_out[2] = -2*xasum_imag/aaa;
	fstat_out[3] = -2*xbsum_imag/bbb;

// Signal-to-noise ratio
	fstat_out[4] = sqrt(2*(-fstat_out[5]-2));

	fstat_out[6] = sgnlo[0];
	fstat_out[7] = sgnlo[1];
	fstat_out[8] = sgnlo[2];
	fstat_out[9] = sgnlo[3];		


	return fstat_out;
}

//mesh adaptive direct search (MADS) maximum search declaration

double* MADS(Search_settings *sett, Command_line_opts *opts, Aux_arrays *aux, double *F, double* in){
	double p[4];
	static double out[10]; //output
	int i, j, k, l, m, n, o;
  	double sinalt, cosalt, sindelt, cosdelt;
	double nSource[3];
	double *res;
	double extr[10];
	float param = 0.009; //initial size of mesh
	float step = 0.001;  //how much mesh should be resized


	for(i = 0; i < 4; i++) p[i] = in[6+i];
	for(i = 0; i < 10; i++) extr[i] = in[i];
	while(param >= 0.001){  //when to end computations
		k = 0;
		for(j = 0; j < 8; j++){
			if(j < 4){
				p[k] = in[6+k] + param*in[6+k];
				k++;
			}
			else { 
				k--;
				p[k] = in[6+k] - param*in[6+k];
			}
			sinalt = sin(p[3]);
			cosalt = cos(p[3]);
			sindelt = sin(p[2]);
			cosdelt = cos(p[2]);

			nSource[0] = cosalt*cosdelt;
			nSource[1] = sinalt*cosdelt;
			nSource[2] = sindelt;

			for (o = 0; o < sett->nifo; ++o){
				modvir(sinalt, cosalt, sindelt, cosdelt, 
			   		sett->N, &ifo[o], aux);  
			}
			res = Fstatnet(sett, opts, aux, F, p, nSource); //Fstat function for mesh points
			if (res[5] < extr[5]){
				for (l = 0; l < 10; l++){ 
					extr[l] = res[l];
				}
			}

		}
		if (extr[5] == in[5]){
			param = param - step;
		}
		else{
			for(m = 0; m < 4; m++) p[m] = extr[6+m];
			in[5] = extr[5];
		}
	}
	for (n= 0; n < 10; n++){
		out[n] = extr[n];
	}
	return out;
}

int main (int argc, char *argv[]) {
	Search_settings sett;	
	Command_line_opts opts;
  	Search_range s_range; 
  	Aux_arrays aux_arr;
  	double *F; 			// F-statistic array
  	int i, j, r, c, a, b;	 	// myrank, num_threads; 
	int d, o, m;
	int bins = 10, ROW;		// neighbourhood of point will be divide into defined number of bins
	double *results;		// Vector with results from Fstatnet function
	double *maximum;		// True maximum of Fstat
	double results_max[10];		  
	double s1, s2, s3, s4;
	double sgnlo[4]; //  arr[ROW][COL], arrg[ROW][COL]; 
	float **arr, **arrg;
	double nSource[3];
  	double sinalt, cosalt, sindelt, cosdelt;
	char path[512];
	ROW = bins*bins;

// Command line options 
	handle_opts(&sett, &opts, argc, argv);  
	
	sprintf(path, "%s/candidates.coi", opts.dtaprefix);

//Glue function
	glue(opts.prefix, opts.dtaprefix, opts.label);

	sprintf(opts.dtaprefix, "./data_total");
	sprintf(opts.dtaprefix, "%s/followup_total_data", opts.prefix); 
	opts.ident = 000;
	
	arr = matrix(ROW, 2);
	arrg = matrix(ROW, 2);

	FILE *coi;
	int z;
//	int ops[256]; 
//    	unsigned short int w, fra[256];
	float mean[4];
	if ((coi = fopen(path, "r")) != NULL) {
//		while(!feof(coi)) {

/*			if(!fread(&w, sizeof(unsigned short int), 1, coi)) { break; } 
		  	fread(&mean, sizeof(float), 5, coi);
		  	fread(&fra, sizeof(unsigned short int), w, coi); 
		  	fread(&ops, sizeof(int), w, coi);

			if((fread(&mean, sizeof(float), 4, coi)) == 4){
*/
			while(fscanf(coi, "%f %f %f %f", &mean[0], &mean[1], &mean[2], &mean[3]) == 4){

//Function neighbourhood - generating grid around point
				arr = neigh(mean[0], mean[1], bins);
				arrg = neigh(mean[2], mean[3], bins);
// Output data handling
/*  				struct stat buffer;

  				if (stat(opts.prefix, &buffer) == -1) {
    					if (errno == ENOENT) {
// Output directory apparently does not exist, try to create one
      						if(mkdir(opts.prefix, S_IRWXU | S_IRGRP | S_IXGRP 
          						| S_IROTH	| S_IXOTH) == -1) {
	      						perror (opts.prefix);
	      						return 1;
      						}
    					} 
					else { // can't access output directory
			      			perror (opts.prefix);
			      			return 1;
			    		}
  				}
*/
// Search settings
  				search_settings(&sett); 

// Detector network settings
  				detectors_settings(&sett, &opts); 

// Array initialization
  				init_arrays(&sett, &opts, &aux_arr, &F);

// Amplitude modulation functions for each detector  
				for(i=0; i<sett.nifo; i++) rogcvir(&ifo[i]); 

// Setting number of using threads (not required)
//omp_set_num_threads(32);

				results_max[5] = 0.;

// F - statistic with parallelisation 
#pragma omp parallel private(d, sinalt, cosalt, sindelt, cosdelt, m, o, sgnlo, nSource, results) shared(results_max)
{
#pragma omp for 

				for (d = 0; d < ROW; ++d){

					sgnlo[2] = arrg[d][0];
					sgnlo[3] = arrg[d][1];	

					sinalt = sin(sgnlo[3]);
					cosalt = cos(sgnlo[3]);
					sindelt = sin(sgnlo[2]);
					cosdelt = cos(sgnlo[2]);

					nSource[0] = cosalt*cosdelt;
					nSource[1] = sinalt*cosdelt;
					nSource[2] = sindelt;

					for (o = 0; o < sett.nifo; ++o){
						modvir(sinalt, cosalt, sindelt, cosdelt, 
					   		sett.N, &ifo[o], &aux_arr);  
					}

					for (m = 0; m < ROW; ++m){
						sgnlo[0] = arr[m][0];
						sgnlo[1] = arr[m][1];

						results = Fstatnet(&sett, &opts, &aux_arr, F, sgnlo, nSource);

// Maximum value in points searching
						if(results[5] < results_max[5]){
							for (i = 0; i < 10; i++){
								results_max[i] = results[i];
							}

						}
					}
				}
}
				maximum = MADS(&sett, &opts, &aux_arr, F, results_max);


			}
//		}
	}
	else {
		
		perror (path);
		return 1;
	}
// Output information
	puts("**********************************************************************");
	printf("***	Maximum value of F-statistic for grid is : (-)%lf	***\n", -results_max[5]);
	printf("Sgnlo: %le %le %le %le\n", results_max[6], results_max[7], results_max[8], results_max[9]);
	printf("Amplitudes: %le %le %le %le\n", results_max[0], results_max[1], results_max[2], results_max[3]);
	printf("Signal-to-noise ratio: %le\n", results_max[4]); 
	puts("**********************************************************************");
	printf("***	True maximum is : (-)%lf				***\n", -maximum[5]);
	printf("Sgnlo for true maximum: %le %le %le %le\n", maximum[6], maximum[7], maximum[8], maximum[9]);
	printf("Amplitudes for true maximum: %le %le %le %le\n", maximum[0], maximum[1], maximum[2], maximum[3]);
	printf("Signal-to-noise ratio for true maximum: %le\n", maximum[4]); 
	puts("**********************************************************************");

// Cleanup & memory free 
//  	cleanup(&sett, &opts, &aux_arr, F);
	
	return 0;

}

//old test
//time LD_LIBRARY_PATH=lib/yeppp-1.0.0/binaries/linux/x86_64 ./followup -data ./data -ident 001 -band 100 -fpo 199.21875 
// new test: 
//time LD_LIBRARY_PATH=/home/msieniawska/tests/polgraw-allsky/search/network/src-cpu/lib/yeppp-1.0.0/binaries/linux/x86_64/ ./followup -data /home/msieniawska/tests/polgraw-allsky/followup/src/testdata/ -output /home/msieniawska/tests/polgraw-allsky/followup/src/output -label J0000+1902 -band 1902
//test for basic testdata:
//time LD_LIBRARY_PATH=/home/msieniawska/tests/polgraw-allsky/search/network/src-cpu/lib/yeppp-1.0.0/binaries/linux/x86_64/ ./followup -data /home/msieniawska/tests/polgraw-allsky/followup/src/testdata/ -output /home/msieniawska/tests/polgraw-allsky/followup/src/output -band 100 -ident 10 -fpo 199.21875

