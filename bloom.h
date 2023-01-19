#ifndef BLOOM_H
#define BLOOM_H

void pre_process(int *image, vec3 *pre_process, int w, int h);
vec3 lookup(vec3 *pre_process, int x, int y, int w, int h);
void blur_fast(int *image, int *result, int w, int h, int k, int c);
void bloom_debug_controls(int pressed[], int *BLOOM, int *spread, float *intensity);

#endif // BLOOM_H
