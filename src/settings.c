#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#include "settings.h"
#include "auxi.h"
#include "struct.h"


/* Search settings: 
 * FFT lenghts & other details, bandwidth and Earth parameters
 */

void search_settings(
  Search_settings* sett) {

  double dt, B, oms, omr, Smin, Smax;
  int nod, N, nfft, s, nd, interpftpad;


  dt = sett->dt;                    // data sampling time:  
                                    // set in handle_opts() from the command line
                                    // (the default value is dt=0.5)

  B = 0.5/dt;                       // Bandwidth
  oms = 2.*M_PI*(sett->fpo)*dt;     // Dimensionless angular frequency

  omr = C_OMEGA_R*dt;

  nod = 2;                          // Observation time in days
  N = round (nod*C_SIDDAY/dt);      // No. of data points

  nfft = 1 << (int)ceil(log(N)/log(2.));    // length of FFT
  s = 1;                                    // No. of spindowns

  Smin = 1000.*C_YEARSEC;                   // Minimum spindown time 
                                            // [sec.]

  // Maximum spindown (1000 years) [angular, dimensionless]
  Smax = 2.*M_PI*(sett->fpo + B)*dt*dt/(2.*Smin);   

  nd = 2;     // Degree of freedom, 
              // (2*nd = deg. no ofrees of freedom for chi^2)

  interpftpad = 2;

  sett->B=B;          	// bandwidth
  sett->oms=oms;      	// dimensionless angular frequency
  sett->omr=omr;      	// C_OMEGA_R * dt
  sett->nod=nod;      	// number of days of observation
  sett->N=N;          	// number of data points
  sett->nfft=nfft;    	// length of fft
  sett->s=s;          	// number of spindowns
  sett->Smin=Smin;    	// minimum spindown
  sett->Smax=Smax;    	// maximum spindown
  sett->nd=nd;        	// degrees of freedom
  sett->interpftpad=interpftpad;

  // Because of frequency-domain filters, we search
  // F-statistic in range (nmin+1, nmax) of data points
  // 
  // The value of sett->fftpad (zero padding - original grids: 2, new grids: 1) 
  // is read from the grid.bin file in read_grid() (see init.c) 

  sett->nmin = sett->fftpad*NAV*sett->B;
  sett->nmax = (sett->nfft/2 - NAV*sett->B)*sett->fftpad;

} // search settings  



/* Network of detectors' discovery: 
 * finds subdirectories in the main input directory, 
 * which by convention should be named like V1, L1, H1 
 * and which contain input data and ephemerids; 
 * writes appropriate detector-related data into structs. 
 */ 

void detectors_settings(
  Search_settings* sett, 
  Command_line_opts *opts) {

  int i=0; 

  char dirname[512], x[512];
  // Main input directory name 
  sprintf (dirname, "%s/%03d", opts->dtaprefix, opts->ident); 

  DIR *dp;
  struct dirent *ep;

  char **detnames  = malloc(MAX_DETECTORS*sizeof(char*));   
  char **xnames = malloc(MAX_DETECTORS*sizeof(char*));

  dp = opendir (dirname);
  if (dp != NULL) {
    while ((ep = readdir (dp))) { 

      // Subdirectory names: 2 char long
      if((ep->d_type == DT_DIR) && 
        (strlen(ep->d_name)==DETNAME_LENGTH) && 
        strncmp(&ep->d_name[0],".",1)) { 

          FILE *data;

          // Input time-domain data handling
          // 
          // We assume that in each subdirectory corresponding 
          // to the detector the input data will look as following: 
          sprintf(x, "%s/%03d/%s/xdatc_%03d_%03d%s.bin",
          opts->dtaprefix, opts->ident, ep->d_name,
          opts->ident, opts->band, opts->label);

          if((data = fopen(x, "r")) != NULL) {

            xnames[i]   = calloc(strlen(x)+1, sizeof(char));
            detnames[i] = calloc(DETNAME_LENGTH+1, sizeof(char));

            strncpy(xnames[i], x, strlen(x));
            strncpy(detnames[i], ep->d_name, DETNAME_LENGTH);
            i++;

          } else { 
            printf("Directory %s exists, but no input file found:\n%s missing...\n", 
              ep->d_name, x);  
            //perror (x);
          }

          memset(x, 0, sizeof(x));
      }
    } 
      
    (void) closedir(dp);

  } else perror ("Couldn't open the input directory...");

  sett->nifo=i;      // number of detectors  
  if(sett->nifo) { 
    printf("Settings - number of detectors: %d\n", sett->nifo); 

  } else { 
    printf("No subdirectories with detector data found. Exiting...\n"); 
    exit(EXIT_FAILURE);
  }  

  for(i=0; i<sett->nifo; i++) { 

    // Virgo detector
    if(!strcmp("V1", detnames[i])) {

      strncpy(ifo[i].xdatname, xnames[i], strlen(xnames[i]));
      strncpy(ifo[i].name, detnames[i], DETNAME_LENGTH);

      // Geographical latitude phi in radians
      ifo[i].ephi = (43.+37./60.+53.0880/3600.)/RAD_TO_DEG;
      // Geographical longitude in radians
      ifo[i].elam = (10.+30./60.+16.1885/3600.)/RAD_TO_DEG;
      // Height h above the Earth ellipsoid in meters
      ifo[i].eheight = 53.238;
      // Orientation of the detector gamma
      ifo[i].egam = (135. - (19.0+25./60.0+57.96/3600.))/RAD_TO_DEG;

      printf("Using %s IFO as detector #%d... %s as input time series data\n", 
        ifo[i].name, i, ifo[i].xdatname);

    // Hanford H1 detector
    } else if(!strcmp("H1", detnames[i])) {

      strncpy(ifo[i].xdatname, xnames[i], strlen(xnames[i]));
      strncpy(ifo[i].name, detnames[i], DETNAME_LENGTH);

      // Geographical latitude phi in radians
      ifo[i].ephi = (46+(27+18.528/60.)/60.)/RAD_TO_DEG;
      // Geographical longitude in radians
      ifo[i].elam = -(119+(24+27.5657/60.)/60.)/RAD_TO_DEG;
      // Height h above the Earth ellipsoid in meters
      ifo[i].eheight = 142.554;
      // Orientation of the detector gamma
      ifo[i].egam = 170.9994/RAD_TO_DEG;

      printf("Using %s IFO as detector #%d... %s as input time series data\n", 
        ifo[i].name, i, ifo[i].xdatname);
 
    // Livingston L1 detector
    } else if(!strcmp("L1", detnames[i])) {

      strncpy(ifo[i].xdatname, xnames[i], strlen(xnames[i]));
      strncpy(ifo[i].name, detnames[i], DETNAME_LENGTH);

      // Geographical latitude phi in radians
      ifo[i].ephi = (30+(33+46.4196/60.)/60.)/RAD_TO_DEG;
      // Geographical longitude in radians
      ifo[i].elam = -(90+(46+27.2654/60.)/60.)/RAD_TO_DEG;
      // Height h above the Earth ellipsoid in meters
      ifo[i].eheight = -6.574;
      // Orientation of the detector gamma
      ifo[i].egam = 242.7165/RAD_TO_DEG;

      printf("Using %s IFO as detector #%d... %s as input time series data\n", 
        ifo[i].name, i, ifo[i].xdatname);

    } else {

      printf("Meh, unknown detector %s (see settings.c) Exiting...\n", 
              detnames[i]);
      exit(EXIT_FAILURE);
    }

  } 

  // memory free for detnames and xdatnames
  for(i=0; i<sett->nifo; i++) { 
    free(detnames[i]);
    free(xnames[i]); 
  } 
   

  free(detnames); 
  free(xnames); 

} // detectors settings


  /* Coefficients of the amplitude modulation functions
   * of the Virgo detector
   */ 

void rogcvir(Detector_settings *ifo) {

  /* In the notation of Phys. Rev. D 58, 063001 (1998):
   * ephi = lambda (geographical latitude phi in radians)
   * egam = gamma (orientation of the detector)
   * 
   * (see modvir function in jobcore.c for Eqs. 12 and 13)
   */ 

  ifo->amod.c1 = .25*sin(2.*ifo->egam)*(1+sqr(sin(ifo->ephi)));
  ifo->amod.c2 = -.5*cos(2.*ifo->egam)*sin(ifo->ephi);
  ifo->amod.c3 = .5*sin(2.*ifo->egam)*sin(2.*ifo->ephi);
  ifo->amod.c4 = -cos(2.*ifo->egam)*cos(ifo->ephi);
  ifo->amod.c5 = .75*sin(2.*ifo->egam)*sqr(cos(ifo->ephi));
  ifo->amod.c6 = cos(2.*ifo->egam)*sin(ifo->ephi);
  ifo->amod.c7 = .5*sin(2.*ifo->egam)*(1.+sqr(sin(ifo->ephi)));
  ifo->amod.c8 = cos(2.*ifo->egam)*cos(ifo->ephi);
  ifo->amod.c9 = .5*sin(2.*ifo->egam)*sin(2.*ifo->ephi);

} // rogcvir


  /* Amplitude modulation of the signal
   */ 

void modvir(
  double sinal, 
  double cosal, 
  double sindel, 
  double cosdel,
  int Np,
  Detector_settings *ifo, 
  Aux_arrays *aux) {

  int t;
  double cosalfr, sinalfr, c2d, c2sd, c, s, c2s, cs;

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
  c2sd = sindel*cosdel;

  // Modulation factor for every data point 
  for (t=0; t<Np; t++) { 

    c = cosalfr*aux->cosmodf[t] + sinalfr*aux->sinmodf[t];
    s = sinalfr*aux->cosmodf[t] - cosalfr*aux->sinmodf[t];
    c2s = 2.*sqr(c);
    cs = c*s;

    // modulation factors aa and bb  
    ifo->sig.aa[t] = c1*(2.-c2d)*c2s + c2*(2.-c2d)*2.*cs +
           c3*c2sd*c + c4*c2sd*s - c1*(2.-c2d) + c5*c2d;

    ifo->sig.bb[t] = c6*sindel*c2s + c7*sindel*2.*cs + 
           c8*cosdel*c + c9*cosdel*s - c6*sindel;

  } 

} // modvir



static int colnum = 0; // columns to compare when searching coincidences
int way2compare(const void *a, const void *b){

    int** x = (int**) a;
	int** y = (int**) b;

	int xval, yval;

	xval = *(*(x) + colnum);
	yval = *(*(y) + colnum);

	if(xval < yval) return 1;
    else if(xval > yval) return -1;
	else return 0;

}

// Allocation of memory for martix with given number of rows and columns
int** matrix(int rows, int cols) {

	int k, **m;
	m = (int **)malloc(rows*sizeof(int *));

	for (k=0; k<rows; k++){
		m[k] = (int *)calloc(cols, sizeof(int));
	}

	return m;

}

    /* Obtaining the trigger files list from a given directory
     * and reading their content 
     */ 

void read_trigger_files(
  Search_settings *sett, 
  Command_line_opts_coinc *opts, 
  Candidate_triggers *trig) {

  int i, j, shift[4], candsize=INICANDSIZE, allcandsize=INICANDSIZE, goodcands=0, current_frame=0, frcount=0;  
  double sqrN, omsN, v[4][4], be[2];
  FLOAT_TYPE tmp[4], c[5];

  char dirname[512], filename[512]; 
  // Trigger files directory name 
  sprintf (dirname, "%s", opts->dtaprefix); 

  DIR *dp;
  struct dirent *ep;
  FILE *data; 

  int **candi = malloc(candsize*sizeof(int *)); 
  int **ti; 
  // 7 columns in a row: fi, si, di, ai, orgpos, fr, i
  for(i=0; i<candsize; i++) 
    candi[i] = malloc(7*sizeof(int));

  int **allcandi = malloc(allcandsize*sizeof(int *)); 
  // 7 columns in a row: fi, si, di, ai, orgpos, fr, i
  for(i=0; i<allcandsize; i++) 
    allcandi[i] = malloc(7*sizeof(int));

  FLOAT_TYPE **candf = malloc(candsize*sizeof(FLOAT_TYPE *)); 
  FLOAT_TYPE **tf; 
  // 5 columns in a row: f, s, d, a, SNR
  for(i=0; i<candsize; i++) 
    candf[i] = malloc(5*sizeof(FLOAT_TYPE));

  FLOAT_TYPE **allcandf = malloc(allcandsize*sizeof(FLOAT_TYPE *));
  // 5 columns in a row: f, s, d, a, SNR
  for(i=0; i<allcandsize; i++)
    allcandf[i] = malloc(5*sizeof(FLOAT_TYPE));


  dp = opendir (dirname);
  if (dp != NULL) {

    // Calculating the shifts from opts->shift 
    int val = opts->shift;
    for(i=0; i<4; i++) shift[i] = 0; 
    i=3; 
    while (val > 0) { 
      if(val%10) shift[i] = val%10; 
      i--; val /= 10;
    }

    sqrN = pow(sett->N, 2);
    omsN = sett->oms*sett->N; 

    // Preparing the transformation matrix elements: division by the corresponding 
    // scale factors outside the loop over candidates to save computational time  
    for(j=0; j<4; j++)  
      for(i=0; i<4; i++)
        v[i][j] = (sett->vedva[i][j])/(opts->scale[j]);   

    while ((ep = readdir (dp))) { 

      if((ep->d_type == DT_REG) &&
        (strstr(ep->d_name, opts->trigname) != NULL)) { 

          sprintf(filename, "%s/%s", opts->dtaprefix, ep->d_name); 

          // This part looks for the first number in the trigger file name, 
          // under the assumption that this is the frame number
          char *fr, *epdname;
          epdname = strdup(ep->d_name);  
          while((fr = strsep(&epdname, "_"))!=NULL) {
            if(fr[0] >= '0' && fr[0] <= '9') {
              current_frame = atoi(fr);
              printf("Reading %s... Frame %d: ", ep->d_name, current_frame); 
              break; 
            } 
          }  

          if((data = fopen(filename, "r")) != NULL) {

            FLOAT_TYPE finband;             
            // Original candidate number (position in the trigger file) 
            int orgpos=0;
            // Counter for 'good' candidates i.e. these that are in band
            i=0; 
            frcount++; 

            // Each candidate is represented by 5 FLOAT_TYPE (double or float) numbers
            // c[0]=f, c[1]=s, c[2]=d, c[3]=a, c[4]=snr
            while(fread((void *)c, sizeof(FLOAT_TYPE), 5, data)==5) {  

              // shifting c[0] (=frequency) to opts->refr reference frame 
              c[0] = c[0] + 2.*c[1]*(sett->N)*(opts->refr - current_frame); 

              // #mb todo: deal with the out-of-band candidates 
              // if frequency is in band 
              if((c[0]>0) && (c[0]<M_PI)) { 

                // Conversion to linear parameters
                //--------------------------------

                tmp[0] = c[0]*sett->N; 
                tmp[1] = c[1]*sqrN; 

                // Transformation of astronomical to linear coordinates;  
                // C_EPSMA, an average value of epsm, is defined in settings.h  
                ast2lin(c[3], c[2], C_EPSMA, be);

                // tmp[2] corresponds to declination (d), tmp[3] to right ascension (a) 
                tmp[2] = omsN*be[0]; 
                tmp[3] = omsN*be[1]; 

                // Saving candidate values 
                for(j=0; j<4; j++) { 

                  // Integer values (0=fi, 1=si, 2=di, 3=ai)  
                  candi[i][j] = round(tmp[0]*v[0][j] + tmp[1]*v[1][j] 
                              + tmp[2]*v[2][j] + tmp[3]*v[3][j] 
                              + 0.5*shift[j]);
 
                  // Astrophysical values (0=f, 1=s, 2=d, 3=a)
                  // f is shifted to opts->refr time frame
                  candf[i][j] = c[j]; 

                } 

                // Saving the original position, frame number and current index
                candi[i][4] = orgpos; 
                candi[i][5] = current_frame;
                candi[i][6] = i; 
                // Saving the SNR value 
                candf[i][4] = c[4]; 
                i++; 

              }   

              orgpos++;

              // Resizing the candidates' array, if the previous limit is reached 
              // (realloc by a factor of 2) 
              if(i==candsize) {

                candsize *= 2; 

                ti = realloc(candi, candsize*sizeof(int *)); 
                if(ti!=NULL) { 
                  candi = ti; 
                    for(j=candsize/2; j<candsize; j++)
                        candi[j] = malloc(7*sizeof(int));
                } else { 
                  printf("Problem with memory realloc for candidates array (int)... exiting...\n"); 
                  exit(EXIT_FAILURE);        
                } 

                tf = realloc(candf, candsize*sizeof(FLOAT_TYPE *)); 
                if(tf!=NULL) { 
                  candf = tf; 
                    for(j=candsize/2; j<candsize; j++)
                        candf[j] = malloc(5*sizeof(FLOAT_TYPE));
                } else { 
                  printf("Problem with memory realloc for candidates array (astro)... exiting...\n"); 
                  exit(EXIT_FAILURE);        
                } 


              } // candsize realloc 

            } // while fread 

            // Frame number  
            trig->frameinfo[frcount][0] = current_frame;
            // Number of candidates in band for a given frame 
            trig->frameinfo[frcount][1] = i;  


            // Looking for duplicates and selecting the one with highest SNR
            //--------------------------------------------------------------

            // Sorting the first 4 columns of candi
            for (i=0; i<4; i++) {
              colnum = i;
              qsort(candi, trig->frameinfo[frcount][1], sizeof(int *), way2compare);
            }

            int maxsnridx=0, frgoodcands=0; 
            double candsnr=0;  
            for (i=0; i<trig->frameinfo[frcount][1]; i++) {
 
              int idx, idx1, maxi, diff=0;  
              for(j=0; j<4; j++)
                //#mb optimize          
                diff += (candi[i][j] - candi[i+1][j])*(candi[i][j] - candi[i+1][j]);

              idx = candi[i][6]; 

              if(diff) {

                int k=i, kidx=idx; 
                if(maxsnridx) { k=maxi; kidx=maxsnridx; }

                // Writing to array containing all candidates 
                for(j=0; j<6; j++) 
                    allcandi[goodcands][j] = candi[k][j]; 
                allcandi[goodcands][6] = goodcands; 

                for(j=0; j<5; j++)
                    allcandf[goodcands][j] = candf[kidx][j];
                 
                maxsnridx=0;
                goodcands++;  
                frgoodcands++; 

                if(goodcands==allcandsize) {

                  allcandsize *= 2; 

                  ti = realloc(allcandi, allcandsize*sizeof(int *)); 
                  if(ti!=NULL) { 
                    allcandi = ti; 
                    for(j=allcandsize/2; j<allcandsize; j++)
                      allcandi[j] = malloc(7*sizeof(int));
                  } else { 
                    printf("Problem with memory realloc for ALL candidates array (int)... exiting...\n"); 
                    exit(EXIT_FAILURE);        
                  }
 
                  tf = realloc(allcandf, allcandsize*sizeof(FLOAT_TYPE *)); 
                  if(tf!=NULL) { 
                    allcandf = tf; 
                    for(j=allcandsize/2; j<allcandsize; j++)
                      allcandf[j] = malloc(5*sizeof(FLOAT_TYPE));
                  } else { 
                    printf("Problem with memory realloc for ALL candidates array (astro)... exiting...\n"); 
                    exit(EXIT_FAILURE);        
                  } 


                }
 

              } else {

                idx1 = candi[i+1][6];

                if(!maxsnridx) {  
                  maxsnridx = (candf[idx][4] > candf[idx1][4] ? idx : idx1); 
                  maxi = i; 
                  candsnr = candf[maxsnridx][4];    
                } else { 
                  if(candf[idx][4] > candsnr) {
                    maxsnridx = idx; maxi = i; 
                    candsnr = candf[idx][4]; 
                  } else if(candf[idx1][4] > candsnr) {   
                    maxsnridx = idx1; maxi = i+1; 
                    candsnr = candf[idx1][4];
                  }  
                }
              }
            }

            // Number of unique candidates in a given frame 
            trig->frameinfo[frcount][2] = frgoodcands; 
            printf("%d/%d\n", trig->frameinfo[frcount][2], trig->frameinfo[frcount][1]); 


          } else { 
            printf("Problem with %s...\n", filename);  
            perror (filename);
          }

          memset(filename, 0, sizeof(filename));
          fclose(data); 

      } 
   
    } 

  } 

  (void) closedir(dp);

  trig->frcount = frcount; 
  trig->goodcands = goodcands; 

  printf("Total number of candidates from all frames: %d\n", trig->goodcands);

  // Looking for coincidences (the same integer values) between frames
  //------------------------------------------------------------------

  // Sorting the first 4 columns of allcandi
  for (i=0; i<4; i++) {
    colnum = i;
    qsort(allcandi, trig->goodcands, sizeof(int *), way2compare);
  }

  int **imtr; 
  int weight=1, coindx=0, maxweight=0, numc; 
  int minnc=10; //#mb mininum number of concidences to register (+-1 - check!) 

  // Maximal possible amount of coincidences with a given threshold minnc 
  numc = (trig->goodcands)/minnc; 

  imtr = matrix(numc, 2); 

  // Coincidcenes: counting rows in a sorted table 
  for (i=0; i<(trig->goodcands-1); i++) {
   
    int diff=0; 
    for(j=0; j<4; j++) 
      // distance measure to compare rows
      //#mb optimize  
      diff += (allcandi[i][j] - allcandi[i+1][j])*(allcandi[i][j] - allcandi[i+1][j]);    

    if(!diff) { 

      weight++; 
      if(weight==minnc) // add row to table if weight crosses the threshold 
        coindx++; 
     
    } else {

      if(weight>minnc) { 
        imtr[coindx][0] = i;
        imtr[coindx][1] = weight+1;
      }  

      weight=0; 
    }     
   
  } 

  colnum = 1;
  qsort(imtr, numc, sizeof(int *), way2compare);

  // Maximum conincidence 
  j = imtr[0][0]+1;
  int w = imtr[0][1]; 
  FLOAT_TYPE meanf=0, means=0, meand=0, meana=0, mesnr=0; 

  printf("Maximum coincidence: %d. Parameters:\n", w);
 
  for(i=imtr[0][1]; i>0; i--) {  
    int k = j-i; 
    int f = allcandi[k][6]; 
    printf("%4d %4d %4d %4d %4d %4d %5le %5le %5le %5le %5le\n", 
           allcandi[k][0], allcandi[k][1], allcandi[k][2], allcandi[k][3], allcandi[k][4], allcandi[k][5], 
           allcandf[f][0], allcandf[f][1], allcandf[f][2], allcandf[f][3], allcandf[f][4]); 

    meanf += allcandf[f][0]; 
    means += allcandf[f][1]; 
    meand += allcandf[f][2]; 
    meana += allcandf[f][3];
    mesnr += pow(allcandf[f][4], 2);  

  }

  j = imtr[0][0]; 
  printf("Mean parameters:\n"); 
  printf("%4d %4d %4d %4d %22le %5le %5le %5le %5le\n", 
    allcandi[j][0], allcandi[j][1], allcandi[j][2], allcandi[j][3], 
    meanf/w, means/w, meand/w, meana/w, sqrt(mesnr));  


  // Freeing auxiliary arrays at the end 
  for(i=0; i<candsize; i++) { 
    free(candi[i]);
    free(candf[i]); 
  } 
   
  for(i=0; i<allcandsize; i++) { 
    free(allcandi[i]);
    free(allcandf[i]);
  }

  for(i=0; i<numc; i++) 
    free(imtr[i]); 

  free(candi);
  free(candf); 
  free(allcandi); 
  free(allcandf);
  free(imtr); 

}

