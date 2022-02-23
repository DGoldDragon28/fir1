
#include "wavrw.h"

#include <string.h>

int wav16_read_head(wav_file_t * wav) {
    return (fread(&wav->hdr, sizeof(struct wav_hdr), 1, wav->file) != 1)
	    || strncmp("RIFF", wav->hdr.w_riff, 4)
            || strncmp("WAVE", wav->hdr.w_wave, 4)
	    || strncmp("fmt ", wav->hdr.w_fmt, 4)
	    || strncmp("data", wav->hdr.w_dhead, 4)
	    || wav->hdr.w_type != 1
	    || wav->hdr.w_brate != (wav->hdr.w_srate * wav->hdr.w_align)
	    || (wav->hdr.w_align<<3) != (wav->hdr.w_bitdepth * wav->hdr.w_nchannels)
	    || wav->hdr.w_bitdepth != 16;
}

int wav16_read_sample(wav_file_t * wav, int16_t * ret) {
    return fread(&ret, wav->hdr.w_align, 1, wav->file) != 1;
}

int wav16_write_head(wav_file_t * wav) {
    return (fwrite(&wav->hdr, sizeof(struct wav_hdr), 1, wav->file) == 1);
}

int wav16_write_sample(wav_file_t * wav, int16_t * sample) {
    return fwrite(&sample, wav->hdr.w_align, 1, wav->file) != 1;
}


