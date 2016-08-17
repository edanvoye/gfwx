
#include "main.h"
#include "gfwx.h"

#define VERSION 0x00010000 // 1.0
static const DWORD FOURCC_GFWX = mmioFOURCC('G','F','W','X');

#if _WIN64
TCHAR szDescription[] = TEXT("GFWX Codec (64 bits) v1.0");
TCHAR szName[] = TEXT("GfwxCodec64");
#else
TCHAR szDescription[] = TEXT("GFWX Codec (32 bits) v1.0");
TCHAR szName[] = TEXT("GfwxCodec32");
#endif

BOOL QueryAbout()
{
    return FALSE;
}

DWORD About(HWND hwnd)
{
    return ICERR_ERROR;
}

BOOL QueryConfigure()
{
    return FALSE;
}

DWORD Configure(HWND hwnd)
{
    return ICERR_ERROR;
}

DWORD GetState(LPVOID pv, DWORD dwSize)
{
    return 0; // no state information
}

DWORD SetState(LPVOID pv, DWORD dwSize)
{
    return 0; // no state information
}

DWORD GetInfo(ICINFO* icinfo, DWORD dwSize)
{
    if (icinfo == NULL)
        return sizeof(ICINFO);

    if (dwSize < sizeof(ICINFO))
        return 0;

    icinfo->dwSize            = sizeof(ICINFO);
    icinfo->fccType           = ICTYPE_VIDEO;
    icinfo->fccHandler        = FOURCC_GFWX;
    icinfo->dwFlags           = 0;

    icinfo->dwVersion         = VERSION;
    icinfo->dwVersionICM      = ICVERSION;
    MultiByteToWideChar(CP_ACP, 0, szDescription, -1, icinfo->szDescription, sizeof(icinfo->szDescription)/sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP, 0, szName, -1, icinfo->szName, sizeof(icinfo->szName)/sizeof(WCHAR));

    return sizeof(ICINFO);
}

bool IsInputFormatSupported(LPBITMAPINFOHEADER lpbiIn)
{
	if (!lpbiIn)
		return FALSE;

	// Formats supported by the Compressor

	if (lpbiIn->biCompression == BI_RGB && lpbiIn->biBitCount == 24)
		return TRUE;
	if (lpbiIn->biCompression == BI_RGB && lpbiIn->biBitCount == 32)
		return TRUE;
	if (lpbiIn->biCompression == mmioFOURCC('Y', '8', ' ', ' ') && lpbiIn->biBitCount == 8)
		return TRUE;
	if (lpbiIn->biCompression == mmioFOURCC('Y', '8', '0', '0') && lpbiIn->biBitCount == 8)
		return TRUE;
	//if (lpbiIn->biCompression == mmioFOURCC('Y', '1', '0', ' ') && lpbiIn->biBitCount == 16)
	//	return TRUE;
	//if (lpbiIn->biCompression == mmioFOURCC('Y', '1', '2', ' ') && lpbiIn->biBitCount == 16)
	//	return TRUE;
	//if (lpbiIn->biCompression == mmioFOURCC('P', 'Y', '1', '0') && lpbiIn->biBitCount == 16)
	//	return TRUE;
	//if (lpbiIn->biCompression == mmioFOURCC('U', 'Y', 'V', 'Y') && lpbiIn->biBitCount == 16)
	//	return TRUE;

	return FALSE;
}

DWORD CompressQuery(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	if (!lpbiIn)
		return ICERR_BADFORMAT;

	return (IsInputFormatSupported(lpbiIn)) ? ICERR_OK : ICERR_BADFORMAT;
}

DWORD CompressBegin(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	// Initialization

	return ICERR_OK;
}

DWORD CompressEnd()
{
	// Cleanup

	return ICERR_OK;
}

DWORD CompressGetSize(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	// Worst case scenario is full uncompressed size * 2
	return (lpbiIn->biWidth * abs(lpbiIn->biHeight) * lpbiIn->biBitCount) / 4;
}

bool FillHeaderForInput(LPBITMAPINFOHEADER lpbiIn, GFWX::Header* header)
{
	if (lpbiIn->biCompression == BI_RGB && lpbiIn->biBitCount == 24)
	{
		// Compress 24bit RGB 
		{
			// set up parameters for lossless GFWX
			int layers = 1;                               // just one image layer
			int channels = 3;                             // 3 interleaved channels
			int bitDepth = 8;            // BitDepthAuto selects 8 or 16 based on type
			int quality = GFWX::QualityMax;               // ranges from 1 (crappy) to 1024 (lossless)
			int chromaScale = 1;                          // chroma quality is divided by this number
			int blockSize = GFWX::BlockDefault;           // probably fine
			int filter = GFWX::FilterLinear;              // FilterLinear is usually best for lossless
			int quantization = GFWX::QuantizationScalar;  // only one choice here anyway
			int encoder = GFWX::EncoderContextual;        // EncoderContextual compresses more than EncoderFast
			int intent = GFWX::IntentBGR;                 // opencv uses BGR instead of RGB

														  // put the image dimensions and parameters into a header
			new (header) GFWX::Header(lpbiIn->biWidth, abs(lpbiIn->biHeight), layers, channels, bitDepth, quality,
				chromaScale, blockSize, filter, quantization, encoder, intent);

			return true;
		}
	}
	else if (lpbiIn->biCompression == BI_RGB && lpbiIn->biBitCount == 32)
	{
		// Compress 32bit RGB 
		{
			// set up parameters for lossless GFWX
			int layers = 1;                               // just one image layer
			int channels = 4;                             // 3 interleaved channels
			int bitDepth = 8;							  // BitDepthAuto selects 8 or 16 based on type
			int quality = GFWX::QualityMax;               // ranges from 1 (crappy) to 1024 (lossless)
			int chromaScale = 1;                          // chroma quality is divided by this number
			int blockSize = GFWX::BlockDefault;           // probably fine
			int filter = GFWX::FilterLinear;              // FilterLinear is usually best for lossless
			int quantization = GFWX::QuantizationScalar;  // only one choice here anyway
			int encoder = GFWX::EncoderContextual;        // EncoderContextual compresses more than EncoderFast
			int intent = GFWX::IntentBGRA;                 // opencv uses BGR instead of RGB

														  // put the image dimensions and parameters into a header
			new (header) GFWX::Header(lpbiIn->biWidth, abs(lpbiIn->biHeight), layers, channels, bitDepth, quality,
				chromaScale, blockSize, filter, quantization, encoder, intent);

			return true;
		}
	}
	else if ((lpbiIn->biCompression == mmioFOURCC('Y', '8', ' ', ' ') || lpbiIn->biCompression == mmioFOURCC('Y', '8', '0', '0'))
		&& lpbiIn->biBitCount == 8)
	{
		// Compress 8 bit Grayscale 
		{
			// set up parameters for lossless GFWX
			int layers = 1;                               // just one image layer
			int channels = 1;                             // 1 mono channel
			int bitDepth = 8;            // BitDepthAuto selects 8 or 16 based on type
			int quality = GFWX::QualityMax;               // ranges from 1 (crappy) to 1024 (lossless)
			int chromaScale = 1;                          // chroma quality is divided by this number
			int blockSize = GFWX::BlockDefault;           // probably fine
			int filter = GFWX::FilterLinear;              // FilterLinear is usually best for lossless
			int quantization = GFWX::QuantizationScalar;  // only one choice here anyway
			int encoder = GFWX::EncoderContextual;        // EncoderContextual compresses more than EncoderFast
			int intent = GFWX::IntentMono;

			// put the image dimensions and parameters into a header
			new (header) GFWX::Header(lpbiIn->biWidth, abs(lpbiIn->biHeight), layers, channels, bitDepth, quality,
				chromaScale, blockSize, filter, quantization, encoder, intent);

			return true;
		}
	}

	// TODO other formats

	return false;
}

DWORD CompressGetFormat(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	if (!IsInputFormatSupported(lpbiIn))
		return ICERR_BADFORMAT;

	if (!lpbiOut)
		return sizeof(BITMAPINFOHEADER) + sizeof(GFWX::Header); // size of returned structure
																  // Fill output structure
	lpbiOut->biSize = sizeof(BITMAPINFOHEADER) + sizeof(GFWX::Header);
	lpbiOut->biWidth = lpbiIn->biWidth;
	lpbiOut->biHeight = lpbiIn->biHeight;
	lpbiOut->biPlanes = 1;
	lpbiOut->biXPelsPerMeter = lpbiIn->biXPelsPerMeter;
	lpbiOut->biYPelsPerMeter = lpbiIn->biYPelsPerMeter;
	lpbiOut->biClrUsed = 0;
	lpbiOut->biClrImportant = 0;

	lpbiOut->biSizeImage = 0; // will be filled later in Compress
	lpbiOut->biBitCount = 32;
	lpbiOut->biCompression = FOURCC_GFWX;

	GFWX::Header* header = (GFWX::Header*)(&lpbiOut[1]);
	FillHeaderForInput(lpbiIn, header);

	return ICERR_OK;
}

DWORD Compress(ICCOMPRESS* icinfo, DWORD dwSize)
{
	// TODO, should we flip-y the image before compression if the input biHeight is negative ?
	// See comment in Decompress

	if (!IsInputFormatSupported(icinfo->lpbiInput))
		return ICERR_BADFORMAT;

	if (icinfo->lpckid)
		*icinfo->lpckid = FOURCC_GFWX;

	{
		const unsigned char* in_frame = (unsigned char*)icinfo->lpInput;
		unsigned char* out_frame = (unsigned char*)icinfo->lpOutput;

		// Every frame is a keyframe as there is no inter-frame compression
		*icinfo->lpdwFlags = AVIIF_KEYFRAME;

		GFWX::Header* header = (GFWX::Header*)(&icinfo->lpbiOutput[1]);

		{
			//if (lpbiIn->biCompression == BI_RGB)
			//	int transform_rgb[] = GFWX_TRANSFORM_A710_BGR;    // handy predefined A710 transform (optional)
			int * transform = 0; // TODO

			// compress the image into the byte buffer (the last two zeros are for optional metadata and size)
			ptrdiff_t size = GFWX::compress(in_frame, *header, out_frame, CompressGetSize(icinfo->lpbiInput, icinfo->lpbiOutput), transform, 0, 0);

			// [NOTE] you can check if (size == GFWX::ErrorOverflow), which means buffer was too small
			if (size == GFWX::ErrorOverflow)
				return ICERR_MEMORY;

			icinfo->lpbiOutput->biSizeImage = (DWORD)size;

			return ICERR_OK;
		}
	}

	return ICERR_ERROR;
}

DWORD DecompressBegin(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	// Initialization

	return ICERR_OK;
}

DWORD DecompressGetPalette(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	// Not Supported
	return ICERR_BADFORMAT;
}

DWORD DecompressEnd()
{
	// Cleanup

	return ICERR_OK;
}

DWORD DecompressGetFormat(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	// Return the default output format for the specified input

	if (lpbiIn->biCompression != FOURCC_GFWX)
	{
		// This is the wrong codec
		return ICERR_BADFORMAT;
	}

	if (!lpbiOut)
	{
		return sizeof(BITMAPINFOHEADER) + sizeof(GFWX::Header); // size of returned structure
	}

	// Fill output structure
	lpbiOut->biSize = sizeof(BITMAPINFOHEADER) + sizeof(GFWX::Header);
	lpbiOut->biWidth = lpbiIn->biWidth;
	lpbiOut->biHeight = lpbiIn->biHeight;
	lpbiOut->biPlanes = 1;
	lpbiOut->biXPelsPerMeter = lpbiIn->biXPelsPerMeter;
	lpbiOut->biYPelsPerMeter = lpbiIn->biYPelsPerMeter;
	lpbiOut->biClrUsed = 0;
	lpbiOut->biClrImportant = 0;

	const GFWX::Header* header = (GFWX::Header*)(&lpbiOut[1]);

	// Choose output format by reading the GFWX Header
	{
		// Return default suggested format for this data
		if (header->bitDepth == 8 && header->channels == 3 && header->layers == 1 && header->intent == GFWX::IntentBGR)
		{
			// 24 bit RGB
			lpbiOut->biBitCount = 24;
			lpbiOut->biCompression = BI_RGB;
			lpbiOut->biSizeImage = (lpbiOut->biWidth * abs(lpbiOut->biHeight) * lpbiOut->biBitCount) / 8;
			return ICERR_OK;
		}
		else if (header->bitDepth == 8 && header->channels == 4 && header->layers == 1 && header->intent == GFWX::IntentBGRA)
		{
			// 24 bit RGB
			lpbiOut->biBitCount = 32;
			lpbiOut->biCompression = BI_RGB;
			lpbiOut->biSizeImage = (lpbiOut->biWidth * abs(lpbiOut->biHeight) * lpbiOut->biBitCount) / 8;
			return ICERR_OK;
		}
		else if (header->bitDepth == 8 && header->channels == 1 && header->layers == 1 && header->intent == GFWX::IntentMono)
		{
			// 8 bit Grayscale (Y8)
			lpbiOut->biBitCount = 8;
			lpbiOut->biCompression = mmioFOURCC('Y', '8', ' ', ' ');
			lpbiOut->biSizeImage = (lpbiOut->biWidth * abs(lpbiOut->biHeight) * lpbiOut->biBitCount) / 8;
			return ICERR_OK;
		}

		// TODO: else more formats
	}

	return ICERR_BADFORMAT;
}

DWORD DecompressQuery(LPBITMAPINFOHEADER lpbiIn, LPBITMAPINFOHEADER lpbiOut)
{
	// Given an input format and an output format, decided if a supported conversion exists

	if (!lpbiIn)
		return ICERR_BADFORMAT;

	if (lpbiIn->biCompression != FOURCC_GFWX)
	{
		return ICERR_BADFORMAT;
	}

	if (lpbiIn->biSize < sizeof(BITMAPINFOHEADER) + sizeof(GFWX::Header))
	{
		return ICERR_BADFORMAT;
	}

	if (lpbiOut)
	{
		// Decompressing to a different size is not supported
		if (lpbiIn->biWidth != lpbiOut->biWidth || abs(lpbiIn->biHeight) != abs(lpbiOut->biHeight))
			return ICERR_BADFORMAT;

		// Verify output format by reading the GFWX Header
		{
			const GFWX::Header* header = (GFWX::Header*)(&lpbiIn[1]);

			// Verify that the output format is compatible
			if (header->bitDepth == 8 && header->channels == 3 && header->layers == 1 && header->intent == GFWX::IntentBGR)
			{
				// 24 bit RGB
				if (lpbiOut->biCompression == BI_RGB && lpbiOut->biBitCount == 24)
					return ICERR_OK;
			}
			else if (header->bitDepth == 8 && header->channels == 4 && header->layers == 1 && header->intent == GFWX::IntentBGRA)
			{
				// 24 bit RGB
				if (lpbiOut->biCompression == BI_RGB && lpbiOut->biBitCount == 32)
					return ICERR_OK;
			}
			else if (header->bitDepth == 8 && header->channels == 1 && header->layers == 1 && header->intent == GFWX::IntentMono)
			{
				// 8 bit Grayscale (Y8)
				if (lpbiOut->biCompression == mmioFOURCC('Y', '8', ' ', ' ') && lpbiOut->biBitCount == 8)
					return ICERR_OK;
				if (lpbiOut->biCompression == mmioFOURCC('Y', '8', '0', '0') && lpbiOut->biBitCount == 8)
					return ICERR_OK;
				if (lpbiOut->biCompression == mmioFOURCC('U', 'Y', 'V', 'Y') && lpbiOut->biBitCount == 16)
					return ICERR_OK;
			}

			// TODO: else more formats
		}

		return ICERR_BADFORMAT;
	}

	return ICERR_OK;
}

DWORD Decompress(ICDECOMPRESS* icinfo, DWORD dwSize)
{
	if (icinfo->lpbiInput->biCompression != FOURCC_GFWX)
		return ICERR_BADFORMAT;

	// TODO: Verify when image has to be flipped vertically
	// If output is UYVY: Positive biHeight implies top-down image (top line first) [http://www.fourcc.org/yuv.php#UYVY]
	// If output is BI_RGB: For uncompressed RGB bitmaps, if biHeight is positive, the bitmap is a bottom-up DIB with the origin at the lower left corner. If biHeight is negative, the bitmap is a top-down DIB with the origin at the upper left corner.
	// If output is a YUV variant: For YUV bitmaps, the bitmap is always top-down, regardless of the sign of biHeight. Decoders should offer YUV formats with positive biHeight, but for backward compatibility they should accept YUV formats with either positive or negative biHeight.
	// For compressed formats, biHeight must be positive, regardless of image orientation. [http://msdn.microsoft.com/en-us/library/windows/desktop/dd318229%28v=vs.85%29.aspx]

	{
		icinfo->lpbiOutput->biSizeImage = (icinfo->lpbiOutput->biWidth * abs(icinfo->lpbiOutput->biHeight) * icinfo->lpbiOutput->biBitCount) >> 3;

		const unsigned char* in_frame = (unsigned char*)icinfo->lpInput;
		unsigned char* out_frame = (unsigned char*)icinfo->lpOutput;

		// read the header first (with 0 pointer for image)
		GFWX::Header header;
		ptrdiff_t result = GFWX::decompress((char*)0, header, in_frame, icinfo->lpbiInput->biSizeImage, 0, true);
		if (result != GFWX::ResultOk)
			return ICERR_BADFORMAT;    // GFWX::ErrorMalformed for a bad file, or positive for truncated file

		// Handle various formats
		if (header.bitDepth == 8 && header.channels == 3 && header.layers == 1 && header.intent == GFWX::IntentBGR)
		{
			// RGB 24bit

			result = GFWX::decompress(out_frame, header, in_frame, icinfo->lpbiInput->biSizeImage, 0, false);
			if (result == GFWX::ResultOk)
				return ICERR_OK;
		}
		else if (header.bitDepth == 8 && header.channels == 1 && header.layers == 1 && header.intent == GFWX::IntentMono && 
			(icinfo->lpbiOutput->biCompression == mmioFOURCC('Y', '8', ' ', ' ') || icinfo->lpbiOutput->biCompression == mmioFOURCC('Y', '8', '0', '0')) && icinfo->lpbiOutput->biBitCount == 8)
		{
			// Grayscale 8 bit

			result = GFWX::decompress(out_frame, header, in_frame, icinfo->lpbiInput->biSizeImage, 0, false);
			if (result == GFWX::ResultOk)
				return ICERR_OK;
		}
		else if (header.bitDepth == 8 && header.channels == 1 && header.layers == 1 && header.intent == GFWX::IntentMono && 
			icinfo->lpbiOutput->biCompression == mmioFOURCC('U', 'Y', 'V', 'Y') && icinfo->lpbiOutput->biBitCount == 16)
		{
			// Grayscale 8 bit, convert Y8 to UYVY

			result = GFWX::decompress(out_frame, header, in_frame, icinfo->lpbiInput->biSizeImage, 0, false);
			if (result == GFWX::ResultOk)
			{
				// INPLACE Convert MONO8 to YUV422 (U and V will be zero)
				int* destination = (int *)(out_frame);
				for (int i = (icinfo->lpbiInput->biWidth*icinfo->lpbiInput->biHeight)/2-1; i>0; i--)
				{
					unsigned char pixel1 = out_frame[i * 2 + 0];
					unsigned char pixel2 = out_frame[i * 2 + 1];
					unsigned int yuv = (pixel1 << 8) | (pixel2 << 24) | 0x00800080;
					destination[i] = yuv;
				}

				return ICERR_OK;
			}
		}

		// TODO more formats
	}

	return ICERR_BADFORMAT;
}

BOOL WINAPI DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT WINAPI DriverProc(DWORD dwDriverID, HDRVR hDriver, UINT uiMessage, LPARAM lParam1, LPARAM lParam2) 
{

    switch (uiMessage) {
    case DRV_LOAD:
        return (LRESULT)1L;

    case DRV_FREE:
        return (LRESULT)1L;

    case DRV_OPEN:
        return (LRESULT)1L;

    case DRV_CLOSE:
        return (LRESULT)1L;

    case DRV_QUERYCONFIGURE:
        return (LRESULT)1L;

    case DRV_CONFIGURE:
        Configure((HWND)lParam1);
        return DRV_OK;

    case ICM_CONFIGURE:
        //  return ICERR_OK if you will do a configure box, error otherwise
        if (lParam1 == -1)
            return QueryConfigure() ? ICERR_OK : ICERR_UNSUPPORTED;
        else
            return Configure((HWND)lParam1);

    case ICM_ABOUT:
        //  return ICERR_OK if you will do a about box, error otherwise
        if (lParam1 == -1)
            return QueryAbout() ? ICERR_OK : ICERR_UNSUPPORTED;
        else
            return About((HWND)lParam1);

    case ICM_GETSTATE:
        return GetState((LPVOID)lParam1, (DWORD)lParam2);

    case ICM_SETSTATE:
        return SetState((LPVOID)lParam1, (DWORD)lParam2);

    case ICM_GETINFO:
        return GetInfo((ICINFO*)lParam1, (DWORD)lParam2);

    case ICM_GETDEFAULTQUALITY:
        if (lParam1) {
            *((LPDWORD)lParam1) = 1000;
            return ICERR_OK;
        }
        break;

    case ICM_COMPRESS:
        return Compress((ICCOMPRESS*)lParam1, (DWORD)lParam2);

    case ICM_COMPRESS_QUERY:
        return CompressQuery((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_COMPRESS_BEGIN:
        return CompressBegin((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_COMPRESS_GET_FORMAT:
        return CompressGetFormat((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_COMPRESS_GET_SIZE:
        return CompressGetSize((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_COMPRESS_END:
        return CompressEnd();

    case ICM_DECOMPRESS_QUERY:
        // The ICM_DECOMPRESS_QUERY message queries a video decompression driver to determine if it supports a specific input format or if it can decompress a specific input format to a specific output format.
        return DecompressQuery((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_DECOMPRESS:
        return Decompress((ICDECOMPRESS*)lParam1, (DWORD)lParam2);

    case ICM_DECOMPRESS_BEGIN:
        return DecompressBegin((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_DECOMPRESS_GET_FORMAT:
        // The ICM_DECOMPRESS_GET_FORMAT message requests the output format of the decompressed data from a video decompression driver.
        return DecompressGetFormat((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_DECOMPRESS_GET_PALETTE:
        return DecompressGetPalette((LPBITMAPINFOHEADER)lParam1, (LPBITMAPINFOHEADER)lParam2);

    case ICM_DECOMPRESS_END:
        return DecompressEnd();

        // Driver messages

    case DRV_DISABLE:
    case DRV_ENABLE:
        return (LRESULT)1L;

    case DRV_INSTALL:
    case DRV_REMOVE:
        return (LRESULT)DRV_OK;
    }

    if (uiMessage < DRV_USER)
        return DefDriverProc(dwDriverID, hDriver, uiMessage, lParam1, lParam2);
    else
        return ICERR_UNSUPPORTED;
}

