  #include "mex.h"
		#define out	  plhs[0]
		#define out2	  plhs[1]

		#define AScan	  prhs[0]
		#define pix_vect  prhs[1]
		#define rec_pos   prhs[2]
		#define send_pos  prhs[3]
		#define speed	  prhs[4]
		#define res	  prhs[5]
		#define timeint   prhs[6]
		#define IMAGE_XYZ prhs[7]
		#define IMAGE_SUM prhs[8]


		#define interp_ratio 5	  // resizes ascan from 3000x1 -> 15000x1 (lin. interp.)


		void as2v_complex(double* a, double* b, int c, double* d, double* e,
		int f, double* g, double* h, double* i, double* j, double* k, double* l,
		double* m, double*n,int o, int p, double *q, double *r) ;

		void mexFunction(int nlhs, mxArray* plhs[],
				 int nrhs, const mxArray* prhs[])
		{
		    //int width_test;
		    int n_AScan;
		    int n_X;
		    int n_Y;
		    int n_Z;
		    int n_IMAGE;
		    int* IMAGE_ptr;
		    int* BUFFER_ptr;
		    int setImageDim[2];
		    int setBufferDim[2];

		    double* pr;
		    double* pi;
		    mxArray* buffer;

		    if (nlhs > 2) mexErrMsgTxt("Too many output arguments.");
		    switch(nrhs)
		    {
		     default:
			mexErrMsgTxt("Incorrect number of arguments.");
		     case 0:
			mexPrintf("\naddSig2Vol_2 SSE1 Assembler Optimized \n\n\t� 2006.03.21. M.Zapf FZK-IPE\n\n");
			break;
		     case 9:

			n_AScan = mxGetNumberOfElements(AScan);    //gesamtanzahl elemente
			IMAGE_ptr	= mxGetPr(IMAGE_XYZ);
			n_X		= *IMAGE_ptr;
			n_Y		= *(IMAGE_ptr+1);
			n_Z		= *(IMAGE_ptr+2);
			n_IMAGE 	= n_X*n_Y*n_Z;
			setImageDim[0]	= n_IMAGE;
			setImageDim[1]	= 1;        //z.b: 400000x1

			BUFFER_ptr	= mxGetDimensions(AScan);
			setBufferDim[0] = (*BUFFER_ptr) * interp_ratio;
			setBufferDim[1] = 1;   //z.b: 400000x1

			//if (n_AScan<(2*wid+2)) mexErrMsgTxt("1. Array not 2 times +2 greater value 3 ");
			//mexPrintf("n_Z:  %i\n\n", n_Z);
			//mexPrintf("n_Y :  %i\n\n", n_Y);
			//mexPrintf("n_X :  %i\n\n", n_X);
			//mexPrintf("n_IMAGE :  %i\n\n", n_IMAGE);


			//anlegen puffer fuer xsum

			if (mxIsComplex(AScan))
			   {
			   buffer = mxCreateDoubleMatrix(0,0,mxCOMPLEX); //Sum buffer laenge ascan
			   mxSetDimensions(buffer,setBufferDim,mxGetNumberOfDimensions(AScan)); //bsp. 3000x1  -> (3000,1) ,2
			   pr=mxMalloc(interp_ratio*n_AScan*sizeof(double));
			   pi=mxMalloc(interp_ratio*n_AScan*sizeof(double));
			   mxSetPr(buffer,pr);
			   mxSetPi(buffer,pi);

			   out	   = mxCreateDoubleMatrix(0,0,mxCOMPLEX);      //out     = mxCreateDoubleMatrix(n_Index,1,mxCOMPLEX);
			   mxSetDimensions(out,setImageDim,2); //bsp. 3000x1  -> (3000,1) ,2
			   pr=mxMalloc(n_IMAGE*sizeof(double));
			   pi=mxMalloc(n_IMAGE*sizeof(double));
			   mxSetPr(out,pr);
			   mxSetPi(out,pi);
			   }
			 else  //real
			   {
			   buffer = mxCreateDoubleMatrix(0,0,mxREAL); //Sum buffer laenge ascan
			   mxSetDimensions(buffer,setBufferDim,mxGetNumberOfDimensions(AScan)); //bsp. 3000x1  -> (3000,1) ,2
			   pr=mxMalloc(interp_ratio*n_AScan*sizeof(double));
			   mxSetPr(buffer,pr);

			   out	   = mxCreateDoubleMatrix(0,0,mxREAL);      //out     = mxCreateDoubleMatrix(n_Index,1,mxCOMPLEX);
			   mxSetDimensions(out,setImageDim,2); //bsp. 3000x1  -> (3000,1) ,2
			   pr=mxMalloc(n_IMAGE*sizeof(double));
			   mxSetPr(out,pr);
			   }

			   // combined REAL & COMPLEX VERSION
			   as2v_complex(mxGetPr(out),mxGetPr(AScan),n_AScan,mxGetPr(buffer),mxGetPr(pix_vect),n_X,mxGetPr(rec_pos),mxGetPr(send_pos),mxGetPr(speed),mxGetPr(res),mxGetPr(timeint),mxGetPi(AScan),mxGetPi(buffer),mxGetPi(out),n_Y,n_Z,mxGetPr(IMAGE_SUM),mxGetPi(IMAGE_SUM));


			out2 = buffer;
			//mxDestroyArray(buffer);
			}

		return;
		}

