//the equivalent of Zhou Wang's SSIM matlab code from http://www.cns.nyu.edu/~zwang/files/research/ssim/index.html
//with K1 = 0.01, K2 = 0.03 and windows = ones(8) since this enables a fast implementation using summed area tables.
//the measure is described in "Image quality assessment: From error measurement to structural similarity"
//was originally adapted from the matlab code for the universal image quality index

//code was written with possible vectorization with ICC in mind, add restricts and vector alignment pragmas
//apparently all the loops are too hard for ICC to grok though, so it will need to be done manually

//Credits to Zhou Wang's and Mfa

#include <stdio.h>
#include <windows.h>
#include "avisynth.h"
#include <malloc.h>
#include <string.h>
#include "SSIM.h"
#include "info.h"
#include <math.h>

#define C1 (float)(64 * 64 * 0.01*255*0.01*255)
#define C2 (float)(64 * 64 * 0.03*255*0.03*255)

SSIM::SSIM(PClip _child1, PClip _child2, const char* _fname, const char* _fname2, const bool _lumimask, const bool _scaled, IScriptEnvironment* env) :
GenericVideoFilter(_child1), child2(_child2), fname(_fname), fname2(_fname2), log(NULL), log2(NULL), lumimask(_lumimask), scaled(_scaled) {

	const VideoInfo& vi2 = child2->GetVideoInfo();

	if ((!vi.IsYV12()) || (!vi2.IsYV12())) 
		env->ThrowError("SSIM: YV12 colorspace required");

	if(vi.num_frames!=vi2.num_frames)
		env->ThrowError("SSIM: These clips don't have same length");

	if((vi.width!=vi2.width) || (vi.height!=vi2.height))
		env->ThrowError("SSIM: These clips don't have same resolution");

	if ((fname[0] != 0) && (fname2[0] != 0)) {
		log = fopen(fname  , "wt");
		log2 = fopen(fname2, "wt");
		if (log && log2) 
			fprintf(log2,"SSIM: Structural Similarity Index Metric 0.23\n");
		else
			env->ThrowError("SSIM: unable to create files %s", fname);
	}

	widthY          = vi.width;
	heightY         = vi.height;
	widthUV         = widthY/2;
	heightUV        = heightY/2;
	strideUV        = (widthUV / 4 + 1) * 4;
	nbFrames        = 0;
	summedQuality   = 0.0f;
	summedWeights   = 0.0f;
	img12_sum_block = (short*)_aligned_malloc(heightUV * widthUV * 2, 16);

	//stride chosen to align the short arrays on 16 byte boundaries
	//dont care about a little waste for the int arrays
	stride = (widthY / 8 + 1) * 8;

	//these circular buffer will contain the total sum along the corresponding column
	//in the original images and per pixel functions thereoff needed for the simularity measure
	//one line will also be used as a temporary for calculating the sum for the last 8 lines
	img1_sum      = (short *)_aligned_malloc(9 * stride * 2, 16);
	img2_sum      = (short *)_aligned_malloc(9 * stride * 2, 16);
	img1_sq_sum   = (int *)_aligned_malloc(9 * stride * 4, 16);
	img2_sq_sum   = (int *)_aligned_malloc(9 * stride * 4, 16);
	img12_mul_sum = (int *)_aligned_malloc(9 * stride * 4, 16);
}

SSIM::~SSIM() {
	double global=summedQuality/summedWeights;
	if(scaled){
		global=100*pow(global,8);
		fprintf(log2,"Average SSIM= %.2f\n",global);
	} else 
		fprintf(log2,"Average SSIM= %f\n",global);
	fclose(log);
	fclose(log2);
	_aligned_free(img12_sum_block);
	_aligned_free(img1_sum);
	_aligned_free(img2_sum);
	_aligned_free(img1_sq_sum);
	_aligned_free(img2_sq_sum);
	_aligned_free(img12_mul_sum);
} 

inline double SSIM::similarity(int muX, int muY, int preMuX2, int preMuY2, int preMuXY2) {
	int muX2, muY2, muXY, thetaX2, thetaY2, thetaXY;

	muX2 = muX*muX;
	muY2 = muY*muY;
	muXY = muX*muY;

	thetaX2 = 64 * preMuX2 - muX2;
	thetaY2 = 64 * preMuY2 - muY2;
	thetaXY = 64 * preMuXY2 - muXY;

	return (2 * muXY + C1) * (2 * thetaXY + C2) / ((muX2 + muY2 + C1) * (thetaX2 + thetaY2 + C2));
}


double SSIM::image_quality(const unsigned char *img1, const unsigned char *img2, int stride_img1, int stride_img2, int width, int height) {
	int x, y, x2, y2, img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block, temp;

	double planeQuality, weight, mean;

	short *img1_sum_ptr1, *img1_sum_ptr2;
	short *img2_sum_ptr1, *img2_sum_ptr2;
	int *img1_sq_sum_ptr1, *img1_sq_sum_ptr2;
	int *img2_sq_sum_ptr1, *img2_sq_sum_ptr2;
	int *img12_mul_sum_ptr1, *img12_mul_sum_ptr2;

	planeQuality = 0;
	if(lumimask)
		planeSummedWeights = 0.0f;
	else
		planeSummedWeights = (height - 7) * (width - 7);

	//some prologue for the main loop
	temp = 8 * stride;

	img1_sum_ptr1      = img1_sum + temp;
	img2_sum_ptr1      = img2_sum + temp;
	img1_sq_sum_ptr1   = img1_sq_sum + temp;
	img2_sq_sum_ptr1   = img2_sq_sum + temp;
	img12_mul_sum_ptr1 = img12_mul_sum + temp;

	for (x = 0; x < width; x++) {
		img1_sum[x]      = img1[x];
		img2_sum[x]      = img2[x];
		img1_sq_sum[x]   = img1[x] * img1[x];
		img2_sq_sum[x]   = img2[x] * img2[x];
		img12_mul_sum[x] = img1[x] * img2[x];

		img1_sum_ptr1[x]      = 0;
		img2_sum_ptr1[x]      = 0;
		img1_sq_sum_ptr1[x]   = 0;
		img2_sq_sum_ptr1[x]   = 0;
		img12_mul_sum_ptr1[x] = 0;
	}

	//the main loop
	for (y = 1; y < height; y++) {
		img1 += stride_img1;
		img2 += stride_img2;

		temp = (y - 1) % 9 * stride;

		img1_sum_ptr1      = img1_sum + temp;
		img2_sum_ptr1      = img2_sum + temp;
		img1_sq_sum_ptr1   = img1_sq_sum + temp;
		img2_sq_sum_ptr1   = img2_sq_sum + temp;
		img12_mul_sum_ptr1 = img12_mul_sum + temp;

		temp = y % 9 * stride;

		img1_sum_ptr2      = img1_sum + temp;
		img2_sum_ptr2      = img2_sum + temp;
		img1_sq_sum_ptr2   = img1_sq_sum + temp;
		img2_sq_sum_ptr2   = img2_sq_sum + temp;
		img12_mul_sum_ptr2 = img12_mul_sum + temp;

		for (x = 0; x < width; x++) {
			img1_sum_ptr2[x]      = img1_sum_ptr1[x] + img1[x];
			img2_sum_ptr2[x]      = img2_sum_ptr1[x] + img2[x];
			img1_sq_sum_ptr2[x]   = img1_sq_sum_ptr1[x] + img1[x] * img1[x];
			img2_sq_sum_ptr2[x]   = img2_sq_sum_ptr1[x] + img2[x] * img2[x];
			img12_mul_sum_ptr2[x] = img12_mul_sum_ptr1[x] + img1[x] * img2[x];
		}

		if (y > 6) {
			//calculate the sum of the last 8 lines by subtracting the total sum of 8 lines back from the present sum
			temp = (y + 1) % 9 * stride;

			img1_sum_ptr1      = img1_sum + temp;
			img2_sum_ptr1      = img2_sum + temp;
			img1_sq_sum_ptr1   = img1_sq_sum + temp;
			img2_sq_sum_ptr1   = img2_sq_sum + temp;
			img12_mul_sum_ptr1 = img12_mul_sum + temp;

			for (x = 0; x < width; x++) {
				img1_sum_ptr1[x]      = img1_sum_ptr2[x] - img1_sum_ptr1[x];
				img2_sum_ptr1[x]      = img2_sum_ptr2[x] - img2_sum_ptr1[x];
				img1_sq_sum_ptr1[x]   = img1_sq_sum_ptr2[x] - img1_sq_sum_ptr1[x];
				img2_sq_sum_ptr1[x]   = img2_sq_sum_ptr2[x] - img2_sq_sum_ptr1[x];
				img12_mul_sum_ptr1[x] = img12_mul_sum_ptr2[x] - img12_mul_sum_ptr1[x];
			}

			//here we calculate the sum over the 8x8 block of pixels
			//this is done by sliding a window across the column sums for the last 8 lines
			//each time adding the new column sum, and subtracting the one which fell out of the window
			img1_block      = 0;
			img2_block      = 0;
			img1_sq_block   = 0;
			img2_sq_block   = 0;
			img12_mul_block = 0;

			//prologue, and calculation of simularity measure from the first 8 column sums
			for (x = 0; x < 8; x++) {
				img1_block      += img1_sum_ptr1[x];
				img2_block      += img2_sum_ptr1[x];
				img1_sq_block   += img1_sq_sum_ptr1[x];
				img2_sq_block   += img2_sq_sum_ptr1[x];
				img12_mul_block += img12_mul_sum_ptr1[x];
			}

			if(lumimask)
			{
				y2 = y - 7;
				x2 = 0;

				if (luminance)
				{
					mean = (img2_block + img1_block) / 128.0f;

					if (!(y2%2 || x2%2))
						*(img12_sum_block + y2/2 * widthUV + x2/2) = img2_block + img1_block;
				}
				else
				{
					mean = *(img12_sum_block + y2 * widthUV + x2);
					mean += *(img12_sum_block + y2 * widthUV + x2 + 4);
					mean += *(img12_sum_block + (y2 + 4) * widthUV + x2);
					mean += *(img12_sum_block + (y2 + 4) * widthUV + x2 + 4);

					mean /= 512.0f;
				}

				weight = mean < 40 ? 0.0f :
					(mean < 50 ? (mean - 40.0f) / 10.0f : 1.0f);
				planeSummedWeights += weight;

				planeQuality += weight * similarity(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);
			}
			else
				planeQuality += similarity(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);

			//and for the rest
			for (x = 8; x < width; x++) {
				img1_block      = img1_block + img1_sum_ptr1[x] - img1_sum_ptr1[x - 8];
				img2_block      = img2_block + img2_sum_ptr1[x] - img2_sum_ptr1[x - 8];
				img1_sq_block   = img1_sq_block + img1_sq_sum_ptr1[x] - img1_sq_sum_ptr1[x - 8];
				img2_sq_block   = img2_sq_block + img2_sq_sum_ptr1[x] - img2_sq_sum_ptr1[x - 8];
				img12_mul_block = img12_mul_block + img12_mul_sum_ptr1[x] - img12_mul_sum_ptr1[x - 8];

				if(lumimask)
				{
					y2 = y - 7;
					x2 = x - 7;

					if (luminance)
					{
						mean = (img2_block + img1_block) / 128.0f;

						if (!(y2%2 || x2%2))
							*(img12_sum_block + y2/2 * widthUV + x2/2) = img2_block + img1_block;
					}
					else
					{
						mean = *(img12_sum_block + y2 * widthUV + x2);
						mean += *(img12_sum_block + y2 * widthUV + x2 + 4);
						mean += *(img12_sum_block + (y2 + 4) * widthUV + x2);
						mean += *(img12_sum_block + (y2 + 4) * widthUV + x2 + 4);

						mean /= 512.0f;
					}

					weight = mean < 40 ? 0.0f :
					  (mean < 50 ? (mean - 40.0f) / 10.0f : 1.0f);
					planeSummedWeights += weight;

					planeQuality += weight * similarity(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);
				}
				else
					planeQuality += similarity(img1_block, img2_block, img1_sq_block, img2_sq_block, img12_mul_block);
			}
		}
	}

	if (planeSummedWeights == 0)
		return 1.0f;
	else
		return planeQuality / planeSummedWeights;
}

PVideoFrame __stdcall SSIM::GetFrame(int n, IScriptEnvironment* env) {

	PVideoFrame ref = child->GetFrame(n, env);
	PVideoFrame alt = child2->GetFrame(n, env);
	PVideoFrame dst = env->NewVideoFrame(vi);

	const unsigned char* refPtrY = ref->GetReadPtr();
	const unsigned char* refPtrU = ref->GetReadPtr(PLANAR_U);
	const unsigned char* refPtrV = ref->GetReadPtr(PLANAR_V);
	const int ref_pitchY         = ref->GetPitch();
	const int ref_pitchUV        = ref->GetPitch(PLANAR_U);

	const unsigned char* altPtrY = alt->GetReadPtr();
	const unsigned char* altPtrU = alt->GetReadPtr(PLANAR_U);
	const unsigned char* altPtrV = alt->GetReadPtr(PLANAR_V);
	const int alt_pitchY         = alt->GetPitch();
	const int alt_pitchUV        = alt->GetPitch(PLANAR_U);

	unsigned char* dstPtrY = dst->GetWritePtr();
	unsigned char* dstPtrU = dst->GetWritePtr(PLANAR_U);
	unsigned char* dstPtrV = dst->GetWritePtr(PLANAR_V);
	const int dst_pitchY   = dst->GetPitch();
	const int dst_pitchUV  = dst->GetPitch(PLANAR_U);

	env->BitBlt(dstPtrY, dst_pitchY, altPtrY, alt_pitchY, widthY, heightY);
	env->BitBlt(dstPtrU, dst_pitchUV, altPtrU, alt_pitchUV, widthUV, heightUV);
	env->BitBlt(dstPtrV, dst_pitchUV, altPtrV, alt_pitchUV, widthUV, heightUV);

	double a,b,c,d;
	double frameWeight;
	char stringbuffer[128];

	luminance = TRUE;
	a = image_quality(refPtrY, altPtrY, ref_pitchY, alt_pitchY, widthY, heightY);
	luminance = FALSE;

	frameWeight = planeSummedWeights / ((widthY - 7) * (heightY - 7));
	if (frameWeight == 0)
		a = b = c = 1.0f;
	else
	{
		b = image_quality(refPtrU, altPtrU, ref_pitchUV, alt_pitchUV, widthUV, heightUV);
		//fprintf(log," U=%f ",b);

		c = image_quality(refPtrV, altPtrV, ref_pitchUV, alt_pitchUV, widthUV, heightUV);
		//fprintf(log," V=%f\n",c);
	}

	d = (0.8f*a)+(0.1*(b+c));
	summedWeights += frameWeight;
	summedQuality += d * frameWeight;

	nbFrames++;
	sprintf(stringbuffer, "n=%d/%d", n, vi.num_frames);
	DrawString(dst, 0, 0, stringbuffer);
	sprintf(stringbuffer, "Y=%.4f  U=%.4f  V=%.4f", a, b, c);
	DrawString(dst, 0, 1, stringbuffer);
	if(lumimask){
		sprintf(stringbuffer, "SSIM=%.4f Weight=%.4f",d,frameWeight);
		fprintf(log,"%f,%f\n",d,frameWeight);
	} else {
		sprintf(stringbuffer, "SSIM=%.4f",d);
		fprintf(log,"%f\n",d);
	}
	DrawString(dst, 0, 2, stringbuffer);
	return dst;
} 

AVSValue __cdecl Create_SSIM(AVSValue args, void* user_data, IScriptEnvironment* env) {
	return new SSIM(args[0].AsClip(), args[1].AsClip(), args[2].AsString("results.csv"), args[3].AsString("globalSSIM.txt"), args[4].AsBool(true), args[5].AsBool(true), env);  
}

extern "C" __declspec(dllexport) const char* __stdcall AvisynthPluginInit2(IScriptEnvironment* env) {
	env->AddFunction("SSIM", "ccss[lumimask]b[scaled]b", Create_SSIM, 0);
	return "`SSIM' SSIM plugin";
}
