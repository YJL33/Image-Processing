class SSIM : public GenericVideoFilter {   

	int widthY;
	int heightY;
	int heightUV;
	int widthUV;
	int strideUV;
	int nbFrames;
	int stride;
	const char* fname;
	const char* fname2;
	bool lumimask;
	bool luminance;
	bool scaled;
	double summedQuality;
	double planeSummedWeights;
	double summedWeights;

	short *img12_sum_block;

	short* img1_sum;
	short* img2_sum;
	int* img1_sq_sum;
	int* img2_sq_sum;
	int* img12_mul_sum;

public:
	SSIM(PClip _child1, PClip _child2, const char *fname, const char *fname2, const bool lumimask, const bool scaled, IScriptEnvironment* env);
	~SSIM();
	PVideoFrame __stdcall GetFrame(int n, IScriptEnvironment* env);
	double image_quality(const unsigned char *img1, const unsigned char *img2, int stride_img1, int stride_img2, int width, int height);
	double similarity(int img1_block, int img2_block, int img1_sq_block, int img2_sq_block, int img12_mul_block);
	PClip child2;
	FILE* log;
	FILE* log2;
};
